'''
Created on Jul 2, 2010

@author: gnulinux
'''
import ogre.renderer.OGRE as ogre
#from cgkit.cgtypes import vec3, quat
from PZEntity import Entity
import numpy as np
from array import array
from PFieldPathFinding import PFieldPathFinder
import copy

class EntityManager():
    '''
    classdocs
    '''


    def __init__(self,scaleDict):
        '''
        Constructor
        '''
        #self._entList = []
        #self._rdrEntManager = renderEntityManager
        #self._pff = 
        self._pff = PFieldPathFinder(scaleDict)
        self._pff.loadMaps("testcitymap3_new.png")
        
    def update(self, sw, dt):
        self._pff.updateEnts(self._entArray)
        #self._rdrEntManager.updateEnts(self._entList, dt) 
    def update2(self, dt):
        self._pff.updateEnts(self._entArray)
        for x in self._entArray:
            ent = x[0]
            #if ent._climb:
            ent._worldPos = ent._worldPos + ent._worldOrient.zAxis()*2*130*dt
                #send out the msgs
                #a = [i, [ent._worldPos.x, ent._worldPos.y, ent._worldPos.z], [ent._worldOrient.x, ent._worldOrient.y, ent._worldOrient.z, ent._worldOrient.w]]
        self._rdrEntManager.update2(self._entArray)
            
    def setScaleDict(self, scaleDict):
        self._scaleDict = scaleDict
    def reset(self):
        for ent, copy in zip(self._entArray, self._entCopy):
            ent[0]._worldPos = copy[0]
            ent[0]._worldOrient = copy[1]
            
    def createEntities(self, pos, orient, resource="ninja.mesh"):
        numOfEnts = pos[0].shape[0]
        self._entArray = np.empty([numOfEnts, 1], type(Entity))
        self._entCopy = []
        for i, (ii, jj, kk) in enumerate(zip(pos[0], pos[1], pos[2])):
            wp = ogre.Vector3(float(ii), float(jj), float(kk))
            og = ogre.Quaternion(orient.w, orient.x, orient.y, orient.z)
            self._entArray[i] = Entity(resource, i, worldPos = wp, o=og)
            self._entCopy.append((wp, og))
        #self._rdrEntManager.createEntities(self._entArray)    
            
            
    def createEntity(self,resource="ninja.mesh"):
        
        #ent = Entity(resource,len(self._entList),worldPos=initParams[0],orient=initParams[1])
        
        #self._entList.append(ent)
        #self._rdrEntManager.insert(ent,inspect=False)
        return;
        
        