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
    def __init__(self,sceneManager,read=0, write=0):
        '''
        Constructor
        '''
        self._scnManager = sceneManager
        #self._rdrNodes = []
        self._rdrNodesArray = None
        self.read = read
        self.tempPos = ogre.Vector3(0,0,0)
        self.tempOrient = ogre.Quaternion()
        #self._rdrEnts = []
    def updateEnts(self, entList, dt):
        for i in range(0, len(entList)):
            self._update(entList[i], self._rdrNodes[i], dt) #assume the order correspond!
    def update(self):
        try:
            #for i in range(0, 1000):
            while(1):
                a = self.read.get_nowait()
                self.updateRdrNode(a)
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
               
    def updateRdrNode(self, a):
        idx = a[0]
        
        if a[3] % 20000 == 0:
            print "id: " + str(a[3])+"\n"
            print "sendOverhead: " + str(a[4])+"\n"
            print "pot update overhead: " + str(a[5])+"\n"
        
        self.tempPos.x = a[1][0]
        self.tempPos.y = a[1][1]
        self.tempPos.z = a[1][2]
        self.tempOrient.x = a[2][0]
        self.tempOrient.y = a[2][1]
        self.tempOrient.z = a[2][2]
        self.tempOrient.w = a[2][3]
        #self.tempPos = self.tempPos + self.tempOrient.zAxis()*2*130.*dt
        self._rdrNodesArray[idx][0].setPosition(self.tempPos)
        self._rdrNodesArray[idx][0].setOrientation(self.tempOrient)
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
        
    def normalizeEnt(self, theEnt, entNode, normalEnt, normalNode,mesh):
        """This method will attempt to normalize entities to normalEnt"""
        #figure out max's of the bounding box
        entBox = theEnt.getWorldBoundingBox()
        normalBox = normalEnt.getWorldBoundingBox()
        #move theEnd's node to ground level
        entMin = entBox.getMinimum()
        entMax = entBox.getMaximum()
        normalMax = normalBox.getMaximum()
        if(entMin.y < 0.0):
            print "found entMax.y < 0.0"
            pos = entNode.getPosition()
            entNode.setPosition(pos.x, pos.y + entMin.y, pos.z)
        #scale in y.
        scaleY = entMax.y / normalMax.y 
        #scale = entNode.getScale()
        if mesh != "ninja.mesh":
            entNode.setScale(scaleY,scaleY, scaleY)
        #rotates into normal
        
        if mesh=="athene.mesh" or mesh=="ogrehead.mesh":
            degree = ogre.Degree(-180.0).valueRadians()
            axis = ogre.Vector3(0,1,0)
            o = ogre.Quaternion(degree, axis)
            entNode.setOrientation(o)
            entNode.setInheritOrientation(True)
        elif mesh=="robot.mesh":
            degree = ogre.Degree(90.0).valueRadians()
            axis = ogre.Vector3(0,1,0)
            o = ogre.Quaternion(degree, axis)
            entNode.setOrientation(o)
            entNode.setInheritOrientation(True)
        
        
    def createEntities(self, numToCreate, orient):
        self._rdrNodesArray = np.empty([numToCreate, 1], ogre.SceneNode)
        meshList=["ninja.mesh",
                  "athene.mesh",
                  "jaiqua.mesh",
                  "robot.mesh",
                  "ogrehead.mesh"]
        
        normalEnt = self._scnManager.createEntity("NORM_ENT", "ninja.mesh")
        normalNode = node = self._scnManager.getRootSceneNode().createChildSceneNode("NORMAL_NODE")
        normalNode.setOrientation(orient)
        normalNode.setInheritOrientation(True)
        for i in range(0, numToCreate):
            #entity = x[0]
            idx = randint(0,4)
            ogreEnt = self._scnManager.createEntity("ENT_"+str(i), meshList[idx])
            
            
            
            node = self._scnManager.getRootSceneNode().createChildSceneNode("ENT_"+str(i)+"NODE")
            nodeChild = node.createChildSceneNode()
            nodeChild.attachObject(ogreEnt)
            self.normalizeEnt(ogreEnt, nodeChild, normalEnt, normalNode, meshList[idx])
            #quat = ogre.Quaternion(entity._worldOrient.w, entity._worldOrient.x, 
             #                      entity._worldOrient.y, entity._worldOrient.z)
            #nodeChild.setOrientation(orient) #we do this (having a child node) is so we orient the entity into place. We should rotate all mesh to face the same way.
            #nodeChild.setInheritOrientation(True)
            #pos = ogre.Vector3(entity._worldPos.x, entity._worldPos.y, entity._worldPos.z)
            #node.setPosition(pos)
            self._rdrNodesArray[i] = node
    
        