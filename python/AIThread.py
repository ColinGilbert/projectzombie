'''
Created on Jul 4, 2010

@author: gnulinux
'''
import threading
import zmq
from ogre.renderer import OGRE as ogre
import sys
import time
import random
import copy 
from PZEntityManager import EntityManager
class AIThread(threading.Thread):
    '''
    classdocs
    '''


    def __init__(self, ctx, scaleDict):
        '''
        Constructor
        '''
        threading.Thread.__init__(self)
        #elf.l = threading.local()
        self.entManager = EntityManager(scaleDict)#copy.deepcopy(entityManager)
        #self.sw = ctx.socket(zmq.PUB)
        self.sw = ctx.socket(zmq.PAIR)
        self.sw.setsockopt(zmq.HWM,0)
        #sw.setsockopt(zmq.SUBSCRIBE,"")
        self.sw.connect("inproc://pipe")
        #self.timer = ogre.Timer()
        #self.local = local
    def run(self):
        dt = 0.016
        count = 0.0   
        start = time.clock()   
        id=0
        endNew = 0.0
        while(1): #run "forever"
            startAll = time.clock()    
            self.entManager.update(0, dt)
            potUpdate = time.clock() - startAll
            startNew = time.clock()
            for i in range(0, self.entManager._entArray.shape[0]):
            #for i, x in enumerate(self._entArray):
                ent = self.entManager._entArray[i][0]
                if ent._climb:
                    ent._worldPos = ent._worldPos + ent._worldOrient.toMat3().getColumn(2)*1 * 130 * dt
                    #send out the msgs
                    a = (i, [ent._worldPos.x, ent._worldPos.y, ent._worldPos.z], [ent._worldOrient.x, ent._worldOrient.y, ent._worldOrient.z, ent._worldOrient.w], id, endNew, potUpdate)
                    #pos = array('f', ())
                    self.sw.send_pyobj(a)
                id = id + 1
            endNew = time.clock() - startNew   
            end = time.clock()
            dt = end-startAll
            count+=dt
            
            
        
def test():
    ctx = zmq.Context(1)
    s = ctx.socket(zmq.SUB)
    s.setsockopt(zmq.SUBSCRIBE,"")
    s.bind("inproc://pipe")
    thread = AIThread(0, ctx)
    time.sleep(1.0)
    print "starting thread"
    thread.start()
    #recieve for 10 seconds
    start = time.time()
    end = time.time() 
    elapsed = end - start
    print "end", end
    print "elapsed:"
    while(elapsed < 10.0): #less than 10 sec
        try:
            a = s.recv_pyobj(zmq.NOBLOCK)
            if a:
                i = a[0]
                #print "i: ",i
                if i % 1000 == 0:
                    print a[1]
                    print a[2]
        except(zmq.ZMQError):
            pass
        end = time.time()
        elapsed = end - start
    
    print "finished 10 secs"
    #thread.join(1)    
    
         
