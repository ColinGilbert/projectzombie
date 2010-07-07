#!/usr/bin/env python
import ogre.renderer.OGRE as ogre
from ogre.renderer.OGRE import Vector3,Quaternion

#from cgkit.cgtypes import vec3, quat

class Entity():
    """This defines the constructor"""
    def __init__(self, resource, id, worldPos=Vector3(0,0,0), o=Quaternion(1.0,0.0,0.0,0.0)):
        self.resource = resource
        self.id = "ENTITY_"+str(id)
        self._worldPos = worldPos
        self._worldOrient = o
        self._goal = Vector3(0,0,0)
        self._climb = True
        self._curPot = 5000000 #used during descending phase
        self._exitDescent = False
        self.V = Vector3(0.0, 0.0, 0.0)

    def setWorldPos(self, pos):
        self._worldPos = pos
    def setWorldOrient(self, orient):
        self._worldOrient = orient
    def setGoal(self, goal):
        self._goal = goal
        
