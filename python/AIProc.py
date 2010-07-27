'''
Created on Jul 4, 2010

@author: gnulinux
'''
import numpy as np
from multiprocessing import Process, Queue
from Queue import Empty
#import zmq
from ogre.renderer import OGRE as ogre
import sys
import time
import random
import copy 
from PZEntityManager import EntityManager
from BulletManager import BulletManager
#class AIProc(Process):


def computeDict():
    basisMeters = 1.9 #1.9 meters.
    basisHeightInUnits = 195.74609375 #number of units for ninja.mesh as a basis.
    metersPerUnit = basisMeters / basisHeightInUnits
    nUnitsToOneMeter = 1.0 / metersPerUnit # 1.0 meters / metersPerUnit; unit is in Units.
    return {"metersPerUnit":metersPerUnit, "nUnitsToOneMeter":nUnitsToOneMeter}

entManager = 0
queue = 0

def init():
    '''
    Constructor
    '''
    #Process.__init__(self, target=self.run)
    #elf.l = threading.local()
    global entManager
    global queue
    entManager = EntityManager(computeDict())#copy.deepcopy(entityManager)
    queue = Queue()

def run(pipe, cmdQueue, numToGen, events):
    try:
        """
        This class is a super hack. The way everything works is meant to only for a main process and a single sub process. THis is so because
        the way we are expect things when reading from the pipes and command queues (We're assumig we're the only one's reading from it')
        """
        events["AIEndInit"].clear()
        basisMeters = 1.9 #1.9 meters.
        basisHeightInUnits = 195.74609375 #number of units for ninja.mesh as a basis.
        metersPerUnit = basisMeters / basisHeightInUnits
        nUnitsToOneMeter = 1.0 / metersPerUnit # 1.0 meters / metersPerUnit; unit is in Units.
        dict = {"metersPerUnit":metersPerUnit, "nUnitsToOneMeter":nUnitsToOneMeter}
        scale = nUnitsToOneMeter
        xlo=10*scale
        xhi=500*scale
        zlo=10*scale
        zhi=20*scale
        randX = np.random.randint(xlo, xhi, numToGen)
        randY = np.random.randint(0, 1, numToGen)
        randZ = np.random.randint(zlo, zhi, numToGen)
        degree = ogre.Degree(-180.0).valueRadians()
        axis = ogre.Vector3(0, 1, 0)
        o = ogre.Quaternion(degree, axis)
        entManager = EntityManager(dict)#copy.deepcopy(entityManager)
        entManager.createEntities([randX, randY, randZ], orient=o)
        #read entity dimensions
        entDimensions = []
        while not cmdQueue.empty():
            entDimensions.append(cmdQueue.get())
            cmdQueue.task_done()
        #read weapons struct
        weaponsStruct = None
        #while not cmdQueue.empty():
        weaponsStruct = cmdQueue.get()
        cmdQueue.task_done()
        numOfWeapons = weaponsStruct["numOfWeapons"]
        #bullet manager
        print "Setting up bullet entities"
        try:
            
            bulletManager = BulletManager(dict, numToGen, pipe)
            print "Bullet is being setup."
            bulletManager.setupBullet()
            print "Bullet entities is being setup."
            print "entity dimensions len: ", len(entDimensions)
            bulletManager.setupEnts(entManager, entDimensions)
            bulletManager.setupWeapons(weaponsStruct)
            entManager.setBulletManager(bulletManager)
            
            #loading maps
            entManager.loadMap()
        except(Exception):
            raise    
        dt = 0.016
        count = 0.0   
        start = time.clock()   
        id=0
        endNew = 0.0
        #scale = 130.0
        #scale = 8.0
        #TC = ogre.Vector3(0.0, 0.0, 1.0*scale)
        TC = 0.0
        TDir = ogre.Vector3()
        Vnew = ogre.Vector3(0.0, 0.0, 0.0)
        Snew = ogre.Vector3(0.0, 0.0, 0.0)
        c = 0.3
        #assume weight is 75kg, which is in s^2/m . 
        m = 7.4051 / scale
        k1 = 0.0
        k2 = 0.0
        events["AIEndInit"].set()
        events["AIEndProc"].clear()
        while(1): #run "forever"
            
            try:
                if(not cmdQueue.empty()):
                    ctrlMsg = cmdQueue.get()
                    if ctrlMsg[0] == "thrust":
                        TC = ctrlMsg[1]
                    if ctrlMsg[0] == "drag":
                        c = ctrlMsg[1]*scale
                    if ctrlMsg[0] == "mass":
                        m = ctrlMsg[2]
                    if ctrlMsg[0] == "reset":
                        entManager.reset()
                    if ctrlMsg[0] == "fireWeapon":
                        pos = ctrlMsg[1]
                        orient = ctrlMsg[2]
                        bulletManager.fireWeapon(pos, orient)
                    if ctrlMsg[0] == "QUIT":
                        print "GOT QUIT MSG!"
                        events["AIEndProc"].set()    
                        break
                    cmdQueue.task_done()
            except(Empty):
                pass
            
            startAll = time.clock()    
            entManager.update(0, dt, TC)
            potUpdate = time.clock() - startAll
            startNew = time.clock()
            
            
            bulletManager.update(dt)
            for i in range(entManager._entArray.shape[0]):
                ent = entManager._entArray[i][0]
                obj = bulletManager.bulletEnts[i][0].object
                a = (i, (ent._worldPos.x, ent._worldPos.y, ent._worldPos.z), (ent._worldOrient.x, ent._worldOrient.y, ent._worldOrient.z, ent._worldOrient.w), id, endNew, potUpdate,
                     obj.getLinearVelocity().length2())
                    #a = (i, [ent._worldPos.x, ent._worldPos.y, ent._worldPos.z], [ent._worldOrient.x, ent._worldOrient.y, ent._worldOrient.z, ent._worldOrient.w])
                    #pos = array('f', ())
                try:
                    pipe.send(a)
                except(IOError):
                    pass
            
                id = id + 1
            endNew = time.clock() - startNew   
            end = time.clock()
            dt = end-startAll
            
            """
            for i in range(0, entManager._entArray.shape[0]):
            #for i, x in enumerate(self._entArray):
                ent = entManager._entArray[i][0]
                TDir = ent._worldOrient.zAxis()*TC
                F = TDir - (ent.V * c)
                A = F*(1.0 / m)
                k1 = A * dt
                
                F = (TDir - ((ent.V + k1) * c))
                A = F*(1.0 / m)
                k2 = A * dt
                
                Vnew = ent.V + (k1 + k2) / 2.0
                Snew = ent._worldPos + Vnew * dt    
                
                ent._worldPos = Snew
                ent.V = Vnew
                #ent._worldPos = ent._worldPos + ent._worldOrient.zAxis()*4 * 130 * dt
                    #send out the msgs
                a = (i, [ent._worldPos.x, ent._worldPos.y, ent._worldPos.z], [ent._worldOrient.x, ent._worldOrient.y, ent._worldOrient.z, ent._worldOrient.w], id, endNew, potUpdate,
                     ent.V.squaredLength())
                    #a = (i, [ent._worldPos.x, ent._worldPos.y, ent._worldPos.z], [ent._worldOrient.x, ent._worldOrient.y, ent._worldOrient.z, ent._worldOrient.w])
                    #pos = array('f', ())
                pipe.send(a)
                id = id + 1
                endNew = time.clock() - startNew   
                end = time.clock()
                dt = end-startAll
            """
            #count+=dt
            
        events["AIEndProc"].set()    
    finally:
        events["AIEndInit"].set()
        events["AIEndProc"].set()
         
