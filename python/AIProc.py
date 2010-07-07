'''
Created on Jul 4, 2010

@author: gnulinux
'''
import numpy as np
from multiprocessing import Process, Queue
from Queue import Empty
import zmq
from ogre.renderer import OGRE as ogre
import sys
import time
import random
import copy 
from PZEntityManager import EntityManager
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

def run(write,numToGen,read):
    
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
    dt = 0.016
    count = 0.0   
    start = time.clock()   
    id=0
    endNew = 0.0
    #scale = 130.0
    #scale = 8.0
    #TC = ogre.Vector3(0.0, 0.0, 1.0*scale)
    TC = 5.0
    TDir = ogre.Vector3()
    Vnew = ogre.Vector3(0.0, 0.0, 0.0)
    Snew = ogre.Vector3(0.0, 0.0, 0.0)
    c = 0.3
    m = 0.3
    k1 = 0.0
    k2 = 0.0
    write.put(True) #send finished loading message
    while(1): #run "forever"
        startAll = time.clock()    
        #entManager.update(0, dt)
        potUpdate = time.clock() - startAll
        startNew = time.clock()
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
            
            #ent._worldPos = Snew
            ent.V = Vnew
            #ent._worldPos = ent._worldPos + ent._worldOrient.zAxis()*4 * 130 * dt
                #send out the msgs
            a = (i, [ent._worldPos.x, ent._worldPos.y, ent._worldPos.z], [ent._worldOrient.x, ent._worldOrient.y, ent._worldOrient.z, ent._worldOrient.w], id, endNew, potUpdate)
                #a = (i, [ent._worldPos.x, ent._worldPos.y, ent._worldPos.z], [ent._worldOrient.x, ent._worldOrient.y, ent._worldOrient.z, ent._worldOrient.w])
                #pos = array('f', ())
            write.put_nowait(a)
            id = id + 1
        endNew = time.clock() - startNew   
        end = time.clock()
        dt = end-startAll
        count+=dt
        try:
            ctrlMsg = read.get_nowait()
            if ctrlMsg[0] == "trust":
                TC = ctrlMsg[1]
            elif ctrlMsg[0] == "drag":
                c = ctrlMsg[1]*scale
            elif ctrlMsg[0] == "mass":
                m = ctrlMsg[2]
            elif ctrlMsg[0] == "reset":
                entManager.reset()
        except(Empty):
            pass        
        
    
         
