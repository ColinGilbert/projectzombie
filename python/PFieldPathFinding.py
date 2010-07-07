'''
Created on Jul 2, 2010

@author: gnulinux
'''
import ogre.renderer.OGRE as ogre
#from cgkit.cgtypes import vec3, quat
#from cgkit import sl as sl
import potential_map2 as pm

class PFieldPathFinder():
    '''
    classdocs
    '''
    def __init__(self, scaleDict):
        '''
        Constructor
        '''
        self.constantGoal = ogre.Vector3(406,500,0)
        self.unitsPerBlock = scaleDict['nUnitsToOneMeter'] #we do this because one units block is one meter. We need to write a class for this.
        self.YAXIS = ogre.Vector3(0.0, 1.0, 0.0)
        self.targetCell = ogre.Vector3(0,0,0)
        self.cell = ogre.Vector3(0,0,0)
        self.dir = ogre.Vector3(0, 0, 0)
    def loadMaps(self, fileName):
        #lm = ogre.LogManager.getSingleton()
        #load the obstacle potential map.
        #lm.logMessage("loading obstacle potential map",ogre.LML_NORMAL)
        print "loading obstacle potential map"
        obsPMap = pm.GetObsPMap(fileName)
        imgShape = obsPMap.shape
        #lm.logMessage("loading obstacle gradient map",ogre.LML_NORMAL)
        print "loading obstacle gradient map"
        obsGradMap = pm.np.gradient(obsPMap)
        print "loading obstacle contour map"
        (cx, cy) = pm.GetContourMap(obsGradMap)
        #lm.logMessage("loading goal potential map", ogre.LML_NORMAL)
        print "loading goal potential map"
        goalPMap = pm.GetUnitPMap([(int(self.constantGoal.x), int(self.constantGoal.y))], 
                                  imgSize = imgShape,)
        #lm.logMessage("loading combined potential map", ogre.LML_NORMAL)
        print "loading combined potential map"
        combinePMap = obsPMap + goalPMap
        self.obsPMap = obsPMap
        self.combinePMap = combinePMap
        #self.obsGradMap = obsGradMap
        self.obsCx = cx
        self.obsCy = cy
        self.NROWS = self.obsPMap.shape[0]
        self.NCOLS = self.obsPMap.shape[1]    
        #lm.logMessage("Done loading maps", ogre.LML_NORMAL)
        print "Done loading maps"
        
    def updateGoals(self, ents):
        for ent in ents:
            #depending on group update their goals. Not implement.
            #For now update to a constant goal.
            ent.setGoal(self.constantGoal)
            
    def updateEnts(self, ents):
        """
        This method expects numpy arrays
        """
        for i in range(0, ents.shape[0]):
            self._update(ents[i][0])
        
        #for i in range(0, ents.shape[0]):
            #self._update(ents[i][0])
    def _clampX(self, x):
        return int(ogre.Math.Clamp(x, 0, self.NCOLS-1))
    def _clampY(self, y):
        return int(ogre.Math.Clamp(y, 0, self.NROWS-1))
    def _checkNeighbors(self, idx, idy, compF):
        
        dx = 1
        dy = 1
        potMap = self.combinePMap
        #potMap = self.obsPMap
        #compute current block. I KNOW we can do this in a loop, but since it's only 8 neighbors.
        maxPot = potMap[idy, idx]
        cellX = idx
        cellY = idy
        #Compute the potential for each of the 8 neighbor blocks.
        #We only have to clamp the first row and first col and last row.
        #[0,0]
        curY = self._clampY(idy - dy)
        startX = self._clampX(idx - dx)
        curX = startX
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        #[0,1]
        curX += dx
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        #[0,2]
        curX = self._clampX(curX + dx)
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        #[1,0]
        curY = idy
        curX = startX
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        #[1,2]
        curX = self._clampX(curX + dx*2)
        #skipping [1,1]
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        #[2,0]
        curY = self._clampY(idy + dy)
        curX = startX
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        #[0,1]
        curX += dx
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        #[0,2]
        curX = self._clampX(curX + dx)
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        return cellX, cellY
    
    def compGreaterThan(self, a, b):
        return a > b
    
    def compLessThan(self, a, b):
        return a < b
    
    def _update(self, ent):
        #compute the current index based on x,y,z
        idx = self._clampX(ent._worldPos.x / self.unitsPerBlock)
        idy = self._clampY(ent._worldPos.z / self.unitsPerBlock)
        #need to flip y
        
        if ent._climb: #ascending
            [cellX, cellY] = self._checkNeighbors(idx, idy, self.compGreaterThan)
            if cellX == idx and cellY == idy:
                ent._climb = False  
                self._curPot = 500000000
            else:
                self._climb(idx, idy, cellX, cellY, ent)
                return
        #descending
        if ent._exitDescent == True: #We go one extra step to get out of crest
            ent._climb = True
            ent._exitDescent = False
        #we keep on walking in the direction
        #we always look one step ahead.
        #grab the obstacle contour map at current cell
        self.cell.x = idx
        self.cell.y = idy
        self.dir.x = -self.obsCy[idx, idy] #was negative 
        self.dir.z = self.obsCx[idx, idy]  
        self.targetCell = self.cell + self.dir
        self.targetCell.normalise()

        curPot = self.combinePMap[idx, idy]
        nextPot = self.combinePMap[self.targetCell.x, self.targetCell.z]
        if nextPot >= curPot:
            if not ent._climb:
                ent._exitDescent = True
        ent._worldOrient.FromAngleAxis(ogre.Math.ATan2(self.dir.x, self.dir.z), self.YAXIS)
        
    def _climb(self, idx, idy, cellX, cellY, ent):
        #Go in the direction of that cell    
        self.targetCell.x  = cellX
        #self.targetCell.y =  0.0
        self.targetCell.z = cellY
        self.cell.x = idx
        #self.cell.y = 0.0
        self.cell.z = idy
        self.targetCell = self.targetCell - self.cell #the new z
        self.targetCell.normalise()
        #dir.normalise()
        ent._worldOrient.FromAngleAxis(ogre.Math.ATan2(self.targetCell.x, self.targetCell.z), self.YAXIS)
         


     