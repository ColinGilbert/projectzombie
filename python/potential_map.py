import numpy as np
import scipy
#import pylab
from scipy import ndimage
from scipy import misc
import mahotas
#from enthought.tvtk.tools import mlab
from enthought.mayavi import mlab

TOLERANCE = 0.0001

"""
This function will return a tuple of OPPBase potential functions.
"""
def OPPUPotential():
    def OPPUP1(d, D):
        scale = 240.0
        if d < TOLERANCE:
            return 80
        else:
            return scale/d*(D-2)
    def OPPUP2(d, D):
        scale = 240.0
        if d < TOLERANCE:
            return 80
        else:
            return scale
    def OPPUP3(d, D):
        scale = 240.0
        scale2 = 0.24
        if d < TOLERANCE:
            return 90
        else:
            return scale-scale2*(d-D)
    return (OPPUP1, OPPUP2, OPPUP3)

def OPPBPotential():
    def OPPBP1(d, D):
        scale = 360.0
        if d < TOLERANCE:
            return 0
        else:
            return scale/(D-2)*d
    def OPPBP2(d, D):
        scale = 360.0
        if d < TOLERANCE:
            return 0
        else:
            return scale
    def OPPBP3(d, D):
        scale = 360.0
        if d < TOLERANCE:
            return 0
        else:
            return scale-(d-D)*0.32
    return (OPPBP1, OPPBP2, OPPBP3)

class Entity():
    def __init__(self, D, R, WEIGHT, P,):
        self.D = D
        self.R = R
        self.P = P
        self.WEIGHT = WEIGHT

#0.1429
OPPU_Tank = Entity(7, 1000, 0.1429, OPPUPotential())
OPPU_Base = Entity(12, 1000, 1, OPPBPotential())
OPPU_Goal = Entity(2, 1000, 0.1429, OPPUPotential())

BasePos = ((61, 250),(39, 131))

def WriteDistanceMap(dist,filename):
    f = open('data/'+filename,"w")
    [nr, nc] = dist.shape
    for row in range(0, nr):
        for e in dist[row]:
            f.write(str(e) + " ")
        f.write("\n")
    f.close()
    return
"""
This function computes the potential for Cliffs.
"""
def PCliff(d, params):
    scale = -80
    if d > 1.0 - TOLERANCE and d < 1.0 + TOLERANCE:
        y = 80.0
    if d > TOLERANCE:
        y = scale/(d**2)
    else:
        y = scale
    return y

"""
This function determines if something is in range, given the parameters.
"""
def IsInRange(d, l, r):
    if d >= l and d <= r:
        y = True
    else:
        y = False
    return y

"""
This function determines the potential for Opponent units, given parameters
for the opponent.
"""
def POpp(d, params):
    DF = 2
    if IsInRange(d, 0, params.D-DF):
        y = params.P[0](d, params.D)
    elif IsInRange(d, params.D-DF, params.D):
        y = params.P[1](d, params.D)
    elif IsInRange(d, params.D, params.R):
        y = params.P[2](d, params.D)
    else:
        y = 0
    return y * params.WEIGHT


"""
This function gets the potential map, given a distance map X (numarray),
a potential function P, and a params object to pass into the the
potential function
"""
def GetPotentialMap(X, P, params):
    [nr, nc] = X.shape
    Y = np.zeros((nr, nc), np.float32) #create single precision matrix.
    for ri, row in enumerate(X): #this for each row's
        Y[ri,:] = map(P,row,map(lambda x : params,
                                range(0, nc)))
    return Y

#def DistanceTransform():
 #   return ndimage.distance_transform_cdt

"""
This function will get Distane Map for obstacles, given an image of
the obstacles. It is assumed that obstacles has a value of greater than
0. And the image will be flattened into a byte scaled grayscale image.
"""
def GetDistanceMapObstacles(gray):
    return ndimage.distance_transform_edt(gray < 1)
    #return ndimage.distance_transform_cdt(gray < 1)
"""
This function will get distance map for units, given Units, A base Image, and a UnitDesc structure. The unit description structure is a tuple containing
the height and width of the unit on the given map. 
"""
def GetDistanceMapUnits(Units, RowsCols, UnitDesc=(1,1)):
    Y = scipy.zeros((RowsCols[0], RowsCols[1]))
    for c in Units:
        for i in range(0,UnitDesc[0]): #height
            for j in range(0,UnitDesc[1]): #width
                Y[c[0]+i, c[1]+j] = 1
    return ndimage.distance_transform_edt(Y < 1)
    #return ndimage.distance_transform_cdt(Y < 1)

def greaterThan(dSqr, value):
    return dSqr > value
def lessThan(dSqr, value):
    return dSqr < value

def GetScaleFilteredGradField(U, V, compFunc=greaterThan,distVal=2.0, scaleFactor=0.5):
    F1 = np.zeros((U.shape[0], U.shape[1]))
    F2 = np.zeros((V.shape[0], V.shape[1]))
    #Let's go through each vector and scale those that are above a certain factor.
    def f(dSqr, x):
        if compFunc(dSqr,distVal):
            return x*scaleFactor
        else:
            return x
    for i, (urow, vrow) in enumerate(zip(U, V)):
        usqr = urow**2
        vsqr = vrow**2
        dSqr = usqr + vsqr
        F1[i,:] = map(f, dSqr, urow)
        F2[i,:] = map(f, dSqr, vrow)

    return [F1,F2]


def OperateOnResults(results, level=2.0, downScale=0.05, upScale=20.0,
                     combine = True, scaleUV = True):
    if combine:
        combineMap = results['ObsPMap'] + results['BasePMap']
    else:
        combineMap = results['ObsPMap']
    gradCMap = np.gradient(combineMap)
    u = gradCMap[0]
    v = gradCMap[1]
    #intelligently scale u,v for visualization.
    if scaleUV:
        [sU,sV] = GetScaleFilteredGradField(u, v, greaterThan, level, downScale)
        [sU,sV] = GetScaleFilteredGradField(sU, sV, lessThan, level, upScale)
    else:
        sU = u
        sV = v
    return {'u':u, 'v':v, 'combineMap':combineMap, 'sU':sU, 'sV':sV}



def PlotResultsVectorField(results, onSurf=False):
    x, y = np.mgrid[0:512,0:512]
    w = np.zeros((512, 512))
    if onSurf:
        mlab.quiver3d(x, y, results['combineMap'],
                  results['sU'], results['sV'], w)
    else:
        mlab.quiver3d(results['sU'], results['sV'], w)
        
def PlotResultsFlow(results, onSurf = False):
    x, y = np.mgrid[0:512, 0:512]
    w = np.zeros((512, 512))
    if onSurf:
        return mlab.flow(x, y, results['combineMap'],
                  results['sU'], results['sV'], results['combineMap'])
    else:
        return mlab.flow(results['sU'], results['sV'], w, seed_resolution=100,
                  seed_scale=1.0)

def GetObsPMap(obsFileName='testcitymap_new.png'):
    
    obsGray = scipy.misc.pilutil.imread('data/'+obsFileName, flatten=True)
    obsDMap = GetDistanceMapObstacles(obsGray)
    obsPMap = GetPotentialMap(obsDMap, PCliff, 0)
    return obsPMap
def GetBasePMap(pos,imgSize=(512, 512),sz=(10, 10)):
    baseDMap = GetDistanceMapUnits(pos, imgSize, sz)
    return GetPotentialMap(baseDMap, POpp, OPPU_Base)
def GetUnitPMap(pos, imgSize=(512, 512), pfunc=POpp, entity=OPPU_Tank):
    dMap = GetDistanceMapUnits(pos, imgSize)
    pMap = GetPotentialMap(dMap, pfunc, entity)
    return pMap


def GetResults():
    nr = 512
    nc = 512
    obsPMap = GetObsPMap()
    #[nr,nc] = obsGray.shape
    print "Computing base 1"
    base1PMap = GetBasePMap([BasePos[0]],(nr, nc))
    print "Computing base 2"
    base2PMap = GetBasePMap([BasePos[1]],(nr, nc))
    print "Computing goal1"
    goal1PMap = GetUnitPMap([(219, 26)], (nr, nc), POpp, OPPU_Tank)
    print "Computing total"
    #create some goals
    return {'ObsPMap': obsPMap, 'BasePMap': base1PMap + base2PMap + goal1PMap}

def PlotPotentialSurface(X):
    x = scipy.arange(0, X.shape[0])
    y = scipy.arange(0, X.shape[1])
    fig = mlab.figure()
    s = mlab.SurfRegular(x, y, X)
    fig.add(s)





        

#write out

#WriteDistanceMap(dist,"BaseEdgeImg.mtd")
#WriteDistanceMap(dist2,"ObsEdgeImg.mtd")

    
''''
dist = dist.max() - dist
dist -= dist.min()
dist = dist/float(dist.ptp())*255
dist = dist.astype(np.uint8)
pylab.imshow(dist)
pylab.show()
'''


