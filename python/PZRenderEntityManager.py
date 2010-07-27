'''
Created on Jul 2, 2010

@author: gnulinux
'''
#from multiprocessing import Queue
from Queue import Empty
from ogre.renderer import OGRE as ogre
import zmq
import numpy as np
from random import randint
            
class RenderEntityManager():
    '''
    classdocs
    '''
    def __init__(self,sceneManager,pipe=0,cmdQueue=0, numToGen=0):
        '''
        Constructor
        '''
        self._scnManager = sceneManager
        #self._rdrNodes = []
        self._rdrNodesArray = None

        self.pipe = pipe
        self.cmdQueue = cmdQueue
        self.tempPos = ogre.Vector3(0,0,0)
        self.tempOrient = ogre.Quaternion()
        self._msgPeekTimes = numToGen/2
        self.numToGen = numToGen
        self.animDamping = 0.5
        #self._rdrEnts = []
    def updateEnts(self, entList, dt):
        for i in range(0, len(entList)):
            self._update(entList[i], self._rdrNodes[i], dt) #assume the order correspond!
    def update(self, dt):
        try:
            for i in range(0, self._msgPeekTimes):
                if not self.pipe.poll():
                    break;
                a = self.pipe.recv()
                self.updateRdrNode(a,dt)
        except(Empty):
            pass #yield no more msgs for now.
    def update2(self, entArray):
        for idx, ent in enumerate(entArray):
            e = ent[0]
            #self.tempPos.x = e._worldPos.x 
            #self.tempPos.y = e._worldPos.y
            #self.tempPos.z = e._worldPos.z
            #self.tempOrient.x = e._worldOrient.x
            #self.tempOrient.y = e._worldOrient.y
            #self.tempOrient.z = e._worldOrient.z
            #self.tempOrient.w = e._worldOrient.w
            self._rdrNodesArray[idx][0].setPosition(e._worldPos)
            self._rdrNodesArray[idx][0].setOrientation(e._worldOrient)
               
    def updateRdrNode(self, a, dt):
        idx = a[0]
        sqrLen = a[6]
        self.tempPos.x = a[1][0]
        self.tempPos.y = a[1][1]
        self.tempPos.z = a[1][2]
        self.tempOrient.x = a[2][0]
        self.tempOrient.y = a[2][1]
        self.tempOrient.z = a[2][2]
        self.tempOrient.w = a[2][3]
        if a[3] != 0 and a[3] % 100000 == 0:
            print "id: " + str(a[3])+"\n"
            print "sendOverhead: " + str(a[4])+"\n"
            print "pot update overhead: " + str(a[5])+"\n"
            print "sqrLen: " + str(sqrLen)
            #print "pos: ",self.tempPos
            #print "orient: ", self.tempOrient
        
        
        #self.tempPos = self.tempPos + self.tempOrient.zAxis()*2*130.*dt
        self._rdrNodesArray[idx][0].setVisible(True)
        self._rdrNodesArray[idx][0].setPosition(self.tempPos)
        self._rdrNodesArray[idx][0].setOrientation(self.tempOrient)
        if idx < self._animStateArray.shape[0]:
            self._animStateArray[idx][0].addTime(sqrLen*dt*self.animDamping)#)self.animDamping)#self._animSpeedsArray[idx][0]*dt)
    """    
    def _update2(self, ent, rdrNode, dt):
        if ent._climb:
            ent._worldPos = ent._worldPos + ent._worldOrient.zAxis()*2*130.0*dt
            rdrNode.setPosition(ent._worldPos)
            #rdrNode.getAttachedObject(0).setOrientation(ent._worldOrient)
            rdrNode.setOrientation(ent._worldOrient)
    """        
    def setScaleDict(self, scaleDict):
        self.scaleDict = scaleDict
        
    def normalizeEnt(self, theEnt, entNode, normalEnt, normalNode,mesh, containNode=None):
        """This method will attempt to normalize entities to normalEnt WARNING: HACK"""
        #figure out max's of the bounding box
        entBox = theEnt.getWorldBoundingBox(True)
        normalBox = normalEnt.getWorldBoundingBox(True)
        #move theEnd's node to ground level
        entMin = entBox.getMinimum()
        entMax = entBox.getMaximum()
        normalMax = normalBox.getMaximum()
        
        
        if(entMin.y < 0.0):
            #print "found entMax.y < 0.0"
            pos = entNode.getPosition()
            entNode.setPosition(pos.x, 0 - 2*entMin.y, pos.z)
        #scale in y.
        scaleY = normalMax.y / entMax.y 
        #scale = entNode.getScale()
        #if mesh != "ninja.mesh":
        #rotates into normal
        #print "scale",scaleY
        if mesh=="ninja.mesh" or mesh== "jaiqua.mesh": #or mesh=="ogrehead.mesh":
            degree = ogre.Degree(-180.0).valueRadians()
            axis = ogre.Vector3(0,1,0)
            o = ogre.Quaternion(degree, axis)
            entNode.setOrientation(o)
            entNode.setInheritOrientation(True)
            """
            if mesh=="jaiqua.mesh": #WE are doing this hack crap is because we're not aligning to center of mass between model and Bullet.
                                    #reasons is because I'm don't feel like thinking about this right now. Later, yes.
                entNode.translate(0.0, 16.0, 0.0, ogre.Node.TS_PARENT)
            else:
                entNode.translate(0.0, 7.0, 0.0, ogre.Node.TS_PARENT)
            """
            #if mesh=="ogrehead.mesh":        
        if mesh=="robot.mesh":
            degree = ogre.Degree(-90.0).valueRadians()
            axis = ogre.Vector3(0,1,0)
            o = ogre.Quaternion(degree, axis)
            entNode.setOrientation(o)
            entNode.setInheritOrientation(True)
               
        if mesh=="robot.mesh":
            scaleY *= 1.8
        entNode.setScale(scaleY, scaleY, scaleY)
        #move "center of mass" to origin
        entBox = theEnt.getWorldBoundingBox(True)
        center = entBox.getCenter()
        radius = theEnt.getBoundingRadius()
        #print "entity center: ", center
        #entNode.translate(ogre.Vector3(-radius, -radius, -radius))
        
    def createEntities(self, numToCreate, numOfWeapons, orient, multiCreate=False):
        self._rdrNodesArray = np.empty([numToCreate + numOfWeapons, 1], ogre.SceneNode)
        self._animStateArray = np.empty([numToCreate, 1], ogre.AnimationState)
        self._animSpeedsArray = np.empty([numToCreate, 1], float)
        meshList=["ninja.mesh",
                  "jaiqua.mesh",
                  "robot.mesh"]
        
        normalEnt = self._scnManager.createEntity("NORM_ENT", "ninja.mesh")
        normalNode = node = self._scnManager.getRootSceneNode().createChildSceneNode("NORMAL_NODE")
        normalNode.setOrientation(orient)
        normalNode.setInheritOrientation(True)
        #entList =[self._scnManager.createEntity("ENT_"+str(i), meshList[rand])]
        
        for i in range(0, numToCreate):
            #entity = x[0]
            idx = randint(0,2)
            #ogreEnt = entList[idx]
            ogreEnt = self._scnManager.createEntity("ENT_"+str(i), meshList[idx])
            ogreEnt.setCastShadows(True)
            self._animStateArray[i] = ogreEnt.getAnimationState('Walk')
            self._animStateArray[i][0].Enabled = True
            self._animStateArray[i][0].addTime(ogre.Math.RangeRandom(0.5, 1.5)) #randomize start animation
            self._animSpeedsArray[i] = ogre.Math.RangeRandom(0.5, 1.5)
            node = self._scnManager.getRootSceneNode().createChildSceneNode("ENT_"+str(i)+"NODE")
            nodeChild = node.createChildSceneNode()
            nodeChild.attachObject(ogreEnt)
            self.normalizeEnt(ogreEnt, nodeChild, normalEnt, normalNode, meshList[idx], node)
            if multiCreate:
                self.sendEntData(ogreEnt, nodeChild, node)
            self._rdrNodesArray[i] = node
        if multiCreate:
            self.cmdQueue.join() #wait for people to complete the tasks. Maybe it's better to use events here.
            #rotate cylinder x axis to align up with z
            
        degree = ogre.Degree(-180.0).valueRadians()
        axis = ogre.Vector3(0, 1, 0)
        rotate = ogre.Quaternion(degree, axis)
        
        for i in range(numToCreate, numToCreate + numOfWeapons):
            ogreEnt = self._scnManager.createEntity("ENT_"+str(i), "cylinder.mesh") 
            aabb  = ogreEnt.getWorldBoundingBox(True)
            center = aabb.getCenter()
            #print "weapon center: ", center
            node = self._scnManager.getRootSceneNode().createChildSceneNode("ENT_"+str(i)+"NODE")
            nodeChild = node.createChildSceneNode()
            nodeChild.attachObject(ogreEnt)
            nodeChild.setOrientation(rotate)
            nodeChild.setScale(5, 5, 200)
            node.translate(center*-1)
            #node.setVisible(True)
            self._rdrNodesArray[i] = node
        #send weapon information
        pos = ogre.Vector3(0, 0, 0)
        self.sendWeaponsData(numOfWeapons, pos, rotate)
        if multiCreate:
            print "Join on command queue for sending weapons data."
            self.cmdQueue.join()    
    def sendWeaponsData(self, numOfWeapons, pos, orient):
        r = 1.0
        pos= (pos.x, pos.y, pos.z)
        o = (orient.w, orient.x, orient.y, orient.z)
        weaponStruct = {"radius":r, "numOfWeapons":numOfWeapons, "pos":pos, "orient":o}
        self.cmdQueue.put(weaponStruct)
        
    def sendEntData(self, ent, nodeChild, node):
        #entBox = ent.getWorldBoundingBox(True)
        entBox = ent.getBoundingBox()
        max = entBox.getMaximum()
        min = entBox.getMinimum()
        radius = ent.getBoundingRadius()
        sndMin = (min.x, min.y, min.z)
        sndMax =  (max.x, min.y, min.z)
        self.cmdQueue.put({"min":sndMin, "max":sndMax, "r":radius})