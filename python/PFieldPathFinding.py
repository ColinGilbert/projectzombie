'''
Created on Jul 2, 2010

@author: gnulinux
'''
import ogre.renderer.OGRE as ogre
#from cgkit.cgtypes import vec3, quat
#from cgkit import sl as sl
import potential_map2 as pm
import ogre.physics.bullet as bullet
import ogre.physics.OgreBulletC as bulletC
import math

class PFieldPathFinder():
    '''
    classdocs
    '''
    def __init__(self, scaleDict):
        '''
        Constructor
        '''
        #self.constantGoal = ogre.Vector3(460,0,256)
        self.constantGoal = ogre.Vector3(308,0,303)
        self.unitsPerBlock = scaleDict['nUnitsToOneMeter'] #we do this because one units block is one meter. We need to write a class for this.
        self.YAXIS = ogre.Vector3(0.0, 1.0, 0.0)
        self.targetCell = ogre.Vector3(0,0,0)
        self.cell = ogre.Vector3(0,0,0)
        self.dir = ogre.Vector3(0, 0, 0)
        self.dirToGoal = ogre.Vector3(0, 0, 0)
        self.potMapReso = 0
        self.descendVec = ogre.Vector3(0, 0, 0)
        self.force = bullet.btVector3()
        self.thrustOffset = 0.0
        self.impulseForce = 5.0
        self.impulse=False
        self.q11 = ogre.Vector2()
        self.q21 = ogre.Vector2()
        self.q12 = ogre.Vector2()
        self.q22 = ogre.Vector2()
        self.fr10 = ogre.Vector2()
        self.fr11 = ogre.Vector2()
        self.fr20 = ogre.Vector2()
        self.fr21 = ogre.Vector2()
        self.fp = ogre.Vector2()
    def loadMaps(self, fileName):
        #lm = ogre.LogManager.getSingleton()
        #load the obstacle potential map.
        #lm.logMessage("loading obstacle potential map",ogre.LML_NORMAL)
        print "loading obstacle potential map"
        obsPMapShift = pm.GetObsPMap(fileName, shift=2.0, scale=False) #We use Cliff2 to shift the local maxima more into the contour region.
        #obsPMapShift[pm.np.isinf(obsPMapShift)] = 0 
        #obsPMap = pm.GetObsPMap(fileName, shift=2, scale=True) #We use this to keep a wide region for contours.
        imgShape = obsPMapShift.shape
        print "ImgShape: ", imgShape
        self.potMapReso = imgShape[0] / 513.0
        #lm.logMessage("loading obstacle gradient map",ogre.LML_NORMAL)
        print "loading obstacle gradient map"
        #obsGradMapShift = pm.np.gradient(obsPMap)
        #obsGradMapShift[0] = pm.cleanUp(obsGradMapShift[0])
        #obsGradMapShift[1] = pm.cleanUp(obsGradMapShift[1])
        obsGradMap = pm.np.gradient(obsPMapShift)
        #obsGradMap[0] = pm.cleanUp(obsGradMap[0], cleanZeros=True)
        #obsGradMap[1] = pm.cleanUp(obsGradMap[1], cleanZeros=True)
        print "loading obstacle contour map"
        (cx, cy) = pm.GetContourMap((obsGradMap[0], obsGradMap[1]))
        #cx = pm.cleanUp(cx)
        #cy = pm.cleanUp(cy)
        #self.OPPUPotential = pm.OPPBPotential()
        #lm.logMessage("loading goal potential map", ogre.LML_NORMAL)
        print "loading goal potential map"
        goalPMap = pm.GetUnitPMap([(int(self.constantGoal.x), int(self.constantGoal.z))], 
                                  imgSize = imgShape)
        #lm.logMessage("loading combined potential map", ogre.LML_NORMAL)
        print "loading combined potential map"
        combinePMap = obsPMapShift + goalPMap
        self.combinePMap = combinePMap
        #self.combinePMap[pm.np.isinf(self.combinePMap)] = 0
        #self.combinePMap[pm.np.isnan(self.combinePMap)] = 0
        combineGradMap = pm.np.gradient(self.combinePMap)
        #normalize grad map
        #norms = pm.np.apply_along_axis(pm.np.linalg.norm, 
        #                               0, combineGradMap)
        #self.combineGradMapU = combineGradMap[0] / norms
        #self.combineGradMapV = combineGradMap[1] / norms
        self.combineGradMapU = combineGradMap[0]
        self.combineGradMapV = combineGradMap[1]
        print "combineGradMapU shape: ", self.combineGradMapU.shape
        #self.obsPMap = obsPMap2
        
        #self.obsGradMap = obsGradMap
        self.obsCx = cx
        self.obsCy = cy
        self.NROWS = self.obsCx.shape[0]
        self.NCOLS = self.obsCy.shape[1]    
        #lm.logMessage("Done loading maps", ogre.LML_NORMAL)
        print "Done loading maps"
        
    def updateGoals(self, ents):
        for ent in ents:
            #depending on group update their goals. Not implement.
            #For now update to a constant goal.
            ent.setGoal(self.constantGoal)
            
    def updateEnts(self, ents, bulletEnts, thrustOffset):
        """
        This method expects numpy arrays
        """
        self.thrustOffset = thrustOffset
        for i in range(0, ents.shape[0]):
            ent = ents[i][0]
            bulletEnt = bulletEnts[i][0]
            self._update(ent, bulletEnt)
            #update the bullet force.
            """
            if ent.mode != ent.AT_GOAL:
                obj = bulletEnt.object
                #apply force in the direction of orientation.
                dir = ent._worldOrient.zAxis()
                #dir = ogre.Vector3(0.0, 0.0, -1.0)
                thrust = ent._thrust + thrustOffset
                force.setValue(dir.x*thrust, dir.y*thrust, dir.z*thrust)
                obj.applyCentralForce(force)
            """
            
    def initEnts(self, ents):
        """
        This method is to initialize the entities into a starting state
        """
        for i in range(0, ents.shape[0]):
            ent = ents[i][0]
            self._update(ent,None)
            
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
        #[2,1]
        curX += dx
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        #[2,2]
        curX = self._clampX(curX + dx)
        pot = potMap[curY, curX]
        if compF(pot, maxPot):
            maxPot = pot
            cellY = curY
            cellX = curX
        return cellX, cellY
    
    def compGreaterThan(self, a, b):
        return a - b >= 0.0001 
    
    def compLessThan(self, a, b):
        return 0.0001 < b - a
    
    def bilerpMap(self, mapX, mapY, xi, yi, x, y):
        
        #return float(mapX[yi, xi]), float(mapY[yi, xi])
        
        """
        This method will bilinear interpolate values in a map, given indices xi, yi, and real positions x, y.
        """
        x2 = self._clampX(xi + 1)
        y2 = self._clampY(yi + 1)
        q11 = self.q11
        q11.x = xi
        q11.y = yi
        q12 = self.q12
        q12.x = xi
        q12.y = y2
        q21 = self.q21
        q21.x = x2
        q21.y = yi
        q22 = self.q22
        q22.x = x2
        q22.y = y2
        
        #[xf, tint] = math.modf(x)
        
        self.fr10.x = float(mapX[q11.y, q11.x])
        self.fr10.y = float(mapY[q11.y, q11.x])
        self.fr11.x = float(mapX[q21.y, q21.x])
        self.fr11.y = float(mapY[q21.y, q21.x])
        self.fr20.x = float(mapX[q12.y, q12.x])
        self.fr20.y = float(mapY[q12.y, q12.x])
        self.fr21.x = float(mapX[q22.y, q22.x])
        self.fr21.y = float(mapY[q22.y, q22.x])
        
    
        
        if (x2 - xi) > 0.000000001:
            self.fr10 = (x2 - x)/(x2 - xi) * self.fr10 \
            + (x - xi)/(x2 - xi) * self.fr11
        
    
            self.fr20 = (x2 - x)/(x2-xi) * self.fr20 \
            + (x - xi)/(x2 - xi) * self.fr21
       
        fp = self.fp
        #interpolate y
        if (y2 - yi) > 0.000000001:
            fp = (y2 - y)/(y2 - yi) * self.fr10 + (y - yi)/(y2 - yi)*self.fr20
        else:
            fp.x = self.fr10.x
            fp.y = self.fr10.y
        return fp.x, fp.y
    
    def _update(self, ent, bulletEnt):
        try:
            #compute the current index based on x,y,z
            worldPosX = ent._worldPos.x / self.unitsPerBlock * self.potMapReso
            worldPosY = ent._worldPos.z / self.unitsPerBlock * self.potMapReso
            idx = self._clampX(worldPosX)
            idy = self._clampY(worldPosY)
            #check to see if we're near goal. 
            self.cell.x = idx
            self.cell.z = idy
            radToGoal = (self.constantGoal - self.cell).squaredLength()
            
            if radToGoal <= 8.0:
                #print "AT GOAL!"
                ent.mode = ent.AT_GOAL
                return 
            if ent.mode == ent.CLIMB: #ascending
                
                #self.dir.x = float(self.combineGradMapV[idy, idx])
                [self.dir.x, self.dir.z] = self.bilerpMap(self.combineGradMapV, self.combineGradMapU, idx, idy, worldPosX, worldPosY)
                #self.dir.z = float(self.combineGradMapU[idy, idx])
                self.targetCell = self.cell + self.dir
                #self.targetCell.x = self._clampX(self.targetCell.x)
                #self.targetCell.z = self._clampY(self.targetCell.z)
                #self.descendVec.x = float(self.combineGradMapV[self.targetCell.z, self.targetCell.x])
                #self.descendVec.z = float(self.combineGradMapU[self.targetCell.z, self.targetCell.x])
                [self.descendVec.x, self.descendVec.z] = self.bilerpMap(self.combineGradMapV, self.combineGradMapU, 
                                                                        self._clampX(self.targetCell.x), 
                                                                        self._clampY(self.targetCell.z), 
                                                                        self.targetCell.x, 
                                                                        self.targetCell.z)
                
                dpp = self.dir.dotProduct(self.descendVec.normalisedCopy())
                
                if dpp < -.00001:
                        ent.mode = ent.DESCEND
                        #self.impulse = True    
                else:
                        ent._worldOrient.FromAngleAxis(ogre.Math.ATan2(self.dir.x, self.dir.z), self.YAXIS)
                        self.updatePhysics(ent, bulletEnt, self.impulse)
                        self.impulse = False
                        return
            #print "descending! "
            #self.dir.x = self.obsCy[idy, idx] #was negative 
            #self.dir.z = self.obsCx[idy, idx] 
            [self.dir.x, self.dir.z] = self.bilerpMap(self.obsCy, self.obsCx, idx, idy, worldPosX, worldPosY)
            
            #self.targetCell = self.cell + self.dir*2
            self.targetCell = self.cell + self.dir
            #self.targetCell.z = self._clampY(self.targetCell.z)
            #self.targetCell.x = self._clampX(self.targetCell.x)
            #self.descendVec.x = float(self.combineGradMapV[self.targetCell.z, self.targetCell.x])
            #self.descendVec.z = float(self.combineGradMapU[self.targetCell.z, self.targetCell.x])
            [self.descendVec.x, self.descendVec.z] = self.bilerpMap(self.combineGradMapV, self.combineGradMapU, 
                                                                        self._clampX(self.targetCell.x), 
                                                                        self._clampY(self.targetCell.z), 
                                                                        self.targetCell.x, 
                                                                        self.targetCell.z)
            
            #self.dir.x = float(self.obsCy[self.targetCell.z, self.targetCell.x])
            #self.dir.z = float(self.obsCx[self.targetCell.z, self.targetCell.x])
            
            dpp = self.dir.dotProduct(self.descendVec.normalisedCopy())
            if ent.mode == ent.EXIT_DESCEND:
                ent.mode = ent.CLIMB
            if dpp > 0.0:
          
                if ent.mode != ent.EXIT_DESCEND: #so we go one more step
                    ent.mode = ent.EXIT_DESCEND
            ent._worldOrient.FromAngleAxis(ogre.Math.ATan2(self.dir.x, self.dir.z), self.YAXIS)
            self.updatePhysics(ent, bulletEnt, self.impulse)
            self.impulse = False
            
        except(Exception):
            raise
    
    def updatePhysics(self, ent, bulletEnt, impulse):
        if bulletEnt == None:
            return
        force = self.force
        thrustOffset = self.thrustOffset
        if ent.mode != ent.AT_GOAL:
                obj = bulletEnt.object
                #apply force in the direction of orientation.
                dir = ent._worldOrient.zAxis()
                #dir = ogre.Vector3(0.0, 0.0, -1.0)
                thrust = ent._thrust + thrustOffset
                force.setValue(dir.x*thrust, dir.y*thrust, dir.z*thrust)
                obj.applyCentralForce(force)
                if impulse:
                    force.setValue(dir.x*self.impulseForce,
                                   dir.y*self.impulseForce,
                                   dir.z*self.impulseForce)
                    obj.applyCentralImpulse(force)
        
    def _climb(self, idx, idy, cellX, cellY, ent):
        #Go in the direction of that cell    
        self.targetCell.x  = cellX
        #self.targetCell.y =  0.0
        self.targetCell.z = cellY
        self.cell.x = idx
        #self.cell.y = 0.0
        self.cell.z = idy
        self.targetCell = self.targetCell - self.cell #the new z
        #self.targetCell.normalise()
        #dir.normalise()
        ent._worldOrient.FromAngleAxis(ogre.Math.ATan2(self.targetCell.x, self.targetCell.z), self.YAXIS)
         


     