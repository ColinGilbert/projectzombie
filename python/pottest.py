#!/usr/bin/env python
# This code is Public Domain and was written for Python-Ogre 1.0.
"""Python-Ogre Beginner Tutorial 05: Buffered Input."""

 
import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import SampleFramework as sf
from multiprocessing import Process, Queue
import time

from multiprocessing import Process, Queue
from PZEntity import Entity
from PZEntityManager import EntityManager as PZEntityManager
from PZRenderEntityManager import RenderEntityManager
from PFieldPathFinding import PFieldPathFinder
from pyconsole import Console
import AIProc
import numpy as np

import array
import code
import sys


class TutorialFrameListener(sf.FrameListener, OIS.KeyListener):
    """A FrameListener class that handles basic user input."""
 
    def __init__(self, renderWindow, camera, sceneManager, console, aiProc = 0,fromQueue=0, toQueue=0, entMgr=0, numToGen=256, multi=0):
        # Subclass any Python-Ogre class and you must call its constructor.
        sf.FrameListener.__init__(self, renderWindow, camera)
        OIS.KeyListener.__init__(self)
        # Populate the camera and scene manager containers.
        self.camNode = camera.parentSceneNode.parentSceneNode
        self.threadOn = multi
        self.sceneManager = sceneManager
        self.eventProcessor = None
 
        self.rotate = 0.1
        self.move = 60*103
        self.direction = ogre.Vector3(0, 0, 0)
 
        self.keepRendering = True
        
        self.console = console
        self.setupInput()
        self.threadStarted = False
        #self.timeScale = 0/1000.0
        self.timer = ogre.Timer()
        self.fromQueue = fromQueue
        self.toQueue = toQueue
        self.entMgr = entMgr
        self.aiProc = aiProc
        self.numToGen = numToGen
        #self._inputMgr = InputMgr(console)
    
    def computeScaleDesc(self):
        basisMeters = 1.9 #1.9 meters.
        basisHeightInUnits = 195.74609375 #number of units for ninja.mesh as a basis.
        metersPerUnit = basisMeters / basisHeightInUnits
        nUnitsToOneMeter = 1.0 / metersPerUnit # 1.0 meters / metersPerUnit; unit is in Units.
        return {"metersPerUnit":metersPerUnit, "nUnitsToOneMeter":nUnitsToOneMeter}
    
    def initEntities(self):
        self.SCALE_DICT = self.computeScaleDesc()
        scale = self.SCALE_DICT['nUnitsToOneMeter']
        #create the entity managers
        self._rdrEntManager = RenderEntityManager(self.sceneManager, 
                                                  read = self.fromQueue,
                                                  write = self.toQueue)
        self._rdrEntManager.setScaleDict(self.SCALE_DICT)
        print "Creating random entities:"
        #values all expressed as meters.
        #create the AI thread.
        if(self.threadOn):
            print "Creating AI Process!"
            #self.t = AIThread(self._rdrEntManager.getCTX(), self.SCALE_DICT)
            #thread.daemon = False
            #$time.sleep(1.0)
            #self.thread.start()
            #self.threadStarted = True
            self.createRandomEntities(0,self.numToGen,xlo=10*scale,xhi=500*scale,zlo=10*scale,zhi=20*scale)
            print "AI thread started."
            self.aiProc.start()
            print "Waiting On AI thread."
            finished = self.fromQueue.get(True, 1000000)
            
            #time.sleep(5.0) #wait for to warm up
            
        else:
            self._entManager = PZEntityManager(self.SCALE_DICT)
            self._entManager._rdrEntManager = self._rdrEntManager
            self.createRandomEntities(self._entManager,self.numToGen,xlo=10*scale,xhi=500*scale,zlo=10*scale,zhi=20*scale)
            
            
        
        
    def createRandomEntities(self,entMgr,numToGen=10,xlo=0,xhi=512,ylo=0,yhi=1,zlo=0,zhi=512):
        """This method will create a bunch of random entities."""
        degree = ogre.Degree(-180.0).valueRadians()
        axis = ogre.Vector3(0,1,0)
        o = ogre.Quaternion(degree, axis)
        if(entMgr):
            randX = np.random.randint(xlo,xhi,numToGen)
            randY = np.random.randint(ylo,yhi,numToGen)
            randZ = np.random.randint(zlo,zhi,numToGen) 
            entMgr.createEntities([randX, randY, randZ], orient=o)
        
        self._rdrEntManager.createEntities(numToGen,orient=o)
        #self._entManager.createEntities([randX,randY,randZ], orient=o)
        
        #for i,j,k in zip(randX,randY,randZ):
         #   pos = ogre.Vector3(int(i),int(j),int(k))
            #orient = ogre.Quaternion()
          #  orient = ogre.Quaternion(degree, axis) #from axis angle
           # self._entManager.createEntity(initParams=[pos,orient])
    def setupInput(self):
        #windowHnd = self.renderWindow.getCustomAttributeInt("WINDOW")
        #self.InputManager = OIS.createPythonInputSystem([("WINDOW", str(windowHnd))])
        self.Keyboard = self.InputManager.createInputObjectKeyboard(OIS.OISKeyboard, True)
        self.Keyboard.setEventCallback(self)
    def _setupInput(self):
        
        # Initialize OIS.
        windowHnd = self.renderWindow.getCustomAttributeInt("WINDOW")
        self.InputManager = OIS.createPythonInputSystem([("WINDOW", str(windowHnd))])
 
        # Create all devices, only catch joystick exceptions since most people use Key/Mouse.
        #self.Keyboard = self.InputManager.createInputObjectKeyboard(OIS.OISKeyboard, self.bufferedKeys)
        #self.Keyboard = self.InputManager.createInputObjectKeyboard(OIS.OISKeyboard,True)
        #self.Keyboard.setEventCallback(self._inputMgr)
        self.Mouse = self.InputManager.createInputObjectMouse(OIS.OISMouse, self.bufferedMouse)
        
        try:
            self.Joy = self.InputManager.createInputObjectJoyStick(OIS.OISJoyStick, self.bufferedJoy)
        except:
            self.Joy = False
 
        #Set initial mouse clipping size.
        self.windowResized(self.renderWindow)
        
        # Register as a Window listener.
        ogre.WindowEventUtilities.addWindowEventListener(self.renderWindow, self);
        
    
    def frameStarted(self, frameEvent):
        if(self.renderWindow.isClosed()):
            return False
 
        
        # Capture and update each device, this will also trigger any listeners.
        #self.Keyboard.capture()
        self.Mouse.capture()
        
        # Process unbuffered key input.
        #if not self.keyPressed2(frameEvent):
        #   return False
 
        # Translate the camera based on time.
        """
        self.camNode.translate(self.camNode.orientation
            * self.direction
            * frameEvent.timeSinceLastFrame, ogre.Node.TS_LOCAL)
        """
        elapsedTimeMills = self.timer.getMillisecondsCPU()/1000.0
            
        #rotMat = ogre.Matrix3()
        #self.camNode.orientation.ToRotationMatrix(rotMat)
        #if(frameEvent.timeSinceLastEvent > 0.00001):
        #self.camera.moveRelative(self.direction*frameEvent.timeSinceLastEvent)
        self.camera.moveRelative(self.direction*elapsedTimeMills)
        #self.camera.moveRelative(self.direction)
        self.timer.reset()
        
        #update the entities
        if(self.threadOn):
            self._rdrEntManager.update()
        else:
            self._entManager.update2(elapsedTimeMills)
        """
        self.camNode.translate(rotMat,
            self.direction
            * frameEvent.timeSinceLastFrame,ogre.Node.TS_WORLD)
        """
        #self.direction = ogre.Vector3(0, 0, 0)
        #self.camNode.translate(self.camNode.getPosition()+self.direction)
        
        #self.camNode.translate(self.direction*frameEvent.timeSinceLastFrame)
        # Check for Key release to stop moving the camera.
        #self.keyReleased(frameEvent)
        # Process unbuffered mouse input.
        self.mousePressed(frameEvent)
        self.mouseDragged(frameEvent)
         
        return self.keepRendering
    
    def keyPressed(self, evt):
        if evt.key == OIS.KC_ESCAPE:
            self.keepRendering = False
            return
        if evt.key == OIS.KC_W:
            self.direction.z = -self.move
        if evt.key == OIS.KC_S:
            self.direction.z = self.move
            
        if evt.key == OIS.KC_A:
            self.direction.x = -self.move
        if evt.key == OIS.KC_D:
            self.direction.x = self.move
            
        self.console.keyPressed(evt)
        
    def keyReleased(self, evt):
        
        if evt.key == OIS.KC_W:
            self.direction.z = 0
        if evt.key == OIS.KC_S:
            self.direction.z = 0
        if evt.key == OIS.KC_A:
            self.direction.x = 0
        if evt.key == OIS.KC_D:
            self.direction.x = 0
        
    def keyPressed2(self, frameEvent):
        
        #self.console.keyPressed(self.Keyboard)
        
        # Stop Rendering if Escape was pressed.
        if self.Keyboard.isKeyDown(OIS.KC_ESCAPE):
            return False
        # Attach the camera to PitchNode1.
        if self.Keyboard.isKeyDown(OIS.KC_1):
            self.camera.parentSceneNode.detachObject(self.camera)
            self.camNode = self.sceneManager.getSceneNode("CamNode1")
            self.sceneManager.getSceneNode("PitchNode1").attachObject(self.camera)
        # Attach the camera to PitchNode2.
        if self.Keyboard.isKeyDown(OIS.KC_2):
            self.camera.parentSceneNode.detachObject(self.camera)
            self.camNode = self.sceneManager.getSceneNode("CamNode2")
            self.sceneManager.getSceneNode("PitchNode2").attachObject(self.camera)
        # Move Forward.
        if self.Keyboard.isKeyDown(OIS.KC_UP) or self.Keyboard.isKeyDown(OIS.KC_W):
            self.direction.z -= self.move
        # Move Backward.
        if self.Keyboard.isKeyDown(OIS.KC_DOWN) or self.Keyboard.isKeyDown(OIS.KC_S):
            self.direction.z += self.move
        # Strafe Left.
        if self.Keyboard.isKeyDown(OIS.KC_LEFT) or self.Keyboard.isKeyDown(OIS.KC_A):
            self.direction.x -= self.move
        # Strafe Right.
        if self.Keyboard.isKeyDown(OIS.KC_RIGHT) or self.Keyboard.isKeyDown(OIS.KC_D):
            self.direction.x += self.move
        # Move Up.
        if self.Keyboard.isKeyDown(OIS.KC_PGUP) or self.Keyboard.isKeyDown(OIS.KC_Q):
            self.direction.y += self.move
        # Move Down.
        if self.Keyboard.isKeyDown(OIS.KC_PGDOWN) or self.Keyboard.isKeyDown(OIS.KC_E):
            self.direction.y -= self.move
        return True
 
    def keyReleased2(self, frameEvent):
        # Undo change to the direction vector when the key is released to stop movement.
        if self.Keyboard.isKeyDown(OIS.KC_UP) or self.Keyboard.isKeyDown(OIS.KC_W):
            self.direction.z += self.move
        # Move Backward.
        if self.Keyboard.isKeyDown(OIS.KC_DOWN) or self.Keyboard.isKeyDown(OIS.KC_S):
            self.direction.z -= self.move
        # Strafe Left.
        if self.Keyboard.isKeyDown(OIS.KC_LEFT) or self.Keyboard.isKeyDown(OIS.KC_A):
            self.direction.x += self.move
        # Strafe Right.
        if self.Keyboard.isKeyDown(OIS.KC_RIGHT) or self.Keyboard.isKeyDown(OIS.KC_D):
            self.direction.x -= self.move
        # Move Up.
        if self.Keyboard.isKeyDown(OIS.KC_PGUP) or self.Keyboard.isKeyDown(OIS.KC_Q):
            self.direction.y -= self.move
        # Move Down.
        if self.Keyboard.isKeyDown(OIS.KC_PGDOWN) or self.Keyboard.isKeyDown(OIS.KC_E):
            self.direction.y += self.move
 
    def mousePressed(self, frameEvent):
        ms = self.Mouse.getMouseState()
        # Toggle the light.
        if ms.buttonDown(OIS.MB_Left):
            light = self.sceneManager.getLight('Light1')
            light.visible = not light.visible
 
    def mouseDragged(self, frameEvent):
        # Rotate the camera.
        ms = self.Mouse.getMouseState()
        if ms.buttonDown(OIS.MB_Right):
            self.camNode.yaw(ogre.Degree(-self.rotate
                * ms.X.rel).valueRadians())
            #self.camNode.getChild(0).pitch(ogre.Degree(-self.rotate
                #* ms.Y.rel).valueRadians())
            self.camNode.pitch(ogre.Degree(-self.rotate
                * ms.Y.rel).valueRadians())
 
class TutorialApplication(sf.Application):
    """Application class."""
    
    def __init__(self,aiProc=0,fromQueue=0, toQueue=0, entMgr=0,numToGen=256, multi=0):
        sf.Application.__init__(self)
        self.fromQueue = fromQueue
        self.toQueue = toQueue
        self.entMgr = entMgr
        self.aiProc = aiProc
        self.numToGen = numToGen
        self.multi = multi
    def _chooseSceneManager(self):
        self.sceneManager = self.root.createSceneManager(ogre.ST_EXTERIOR_CLOSE);
 
    
    def _createScene(self):
        self.console = Console(self.root,self)
        #self.console.addLocals(self.toQueue)
        
        
        sceneManager = self.sceneManager
        sceneManager.ambientLight = 0.25, 0.25, 0.25
        
        sceneManager.setWorldGeometry('terrain.cfg')
        
        
        

 
        ent = sceneManager.createEntity("Ninja", "ninja.mesh")
        node = sceneManager.getRootSceneNode().createChildSceneNode("NinjaNode")
        #node.attachObject(ent)
 
        light = sceneManager.createLight("Light1")
        light.type = ogre.Light.LT_POINT
        light.position = 250, 150, 250
        light.diffuseColour = 1, 1, 1
        light.specularColour = 1, 1, 1
 
        # create the first camera node/pitch node
        node = sceneManager.getRootSceneNode().createChildSceneNode("CamNode1",
            (-400, 200, 400))
        node.yaw(ogre.Degree(-45))
        node = node.createChildSceneNode("PitchNode1")
        node.attachObject(self.camera)
 
        # create the second camera node/pitch node
        node = sceneManager.getRootSceneNode().createChildSceneNode("CamNode2",
            (0, 200, 400))
        node.createChildSceneNode("PitchNode2")
        
        
        
    
            
    def _createCamera(self):
        self.camera = self.sceneManager.createCamera("PlayerCam")
        self.camera.nearClipDistance = 5
 
    def _createFrameListener(self):
        self.frameListener = TutorialFrameListener(self.renderWindow,
            self.camera,
            self.sceneManager,
            self.console,
            aiProc = self.aiProc,
            fromQueue = self.fromQueue,
            toQueue = self.toQueue,
            numToGen = self.numToGen,
            multi=self.multi)
        self.frameListener.initEntities()
        self.root.addFrameListener(self.frameListener)
        
        self.frameListener.showDebugOverlay(False)

#class QuaternionPickle():
    #def __init__(self,):
#        pass
#@staticmethod
if __name__ == '__main__':
    try:
        numToGen = 1024
        multi = True
        fromQueue = Queue()
        toQueue = Queue()
        fileName = ""
        aiProc = 0
        if(multi):
            aiProc = Process(target=AIProc.run,args=(fromQueue,numToGen,toQueue))
            aiProc._daemonic = True
        
        ta = TutorialApplication(aiProc, fromQueue=fromQueue, numToGen=numToGen, multi=multi, toQueue = toQueue)
        ta.go()
        print "finished go"
        ta.console.parent = 0
        print "delete go"
    except ogre.OgreException, e:
        print e
