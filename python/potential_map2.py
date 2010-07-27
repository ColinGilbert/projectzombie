import numpy as np
import scipy
from scipy import ndimage
from scipy import misc
import mahotas
import functools
import math
import struct as st
#from enthought.mayavi import mlab

TOLERANCE = 0.0001
TRANSLATE_UP = 0
OPPU_SCALE = 240.0
OPPB_SCALE = 360.0
PCLIFF_SCALE = -60.0
"""
This function will return a tuple of OPPBase potential functions.
"""
def OPPUPotential():
    def OPPUP1(d, D):
        #scale = 240.0 +
        if d < TOLERANCE:
            return 180
        else:
            return (OPPU_SCALE + TRANSLATE_UP)/d*(D-2)
    def OPPUP2(d, D):
        if d < TOLERANCE:
            return 80
        else:
            return OPPU_SCALE + TRANSLATE_UP
    def OPPUP3(d, D):
        scale2 = 0.24
        if d < TOLERANCE:
            return 90
        else:
            return OPPU_SCALE+TRANSLATE_UP-scale2*(d-D)
    return (OPPUP1, OPPUP2, OPPUP3)

def OPPBPotential():
    def OPPBP1(d, D):
        if d < TOLERANCE:
            return 0
        else:
            return (OPPB_SCALE + TRANSLATE_UP)/(D-2)*d
    def OPPBP2(d, D):
        if d < TOLERANCE:
            return 0
        else:
            return OPPB_SCALE + TRANSLATE_UP
    def OPPBP3(d, D):
        if d < TOLERANCE:
            return 0
        else:
            return OPPB_SCALE + TRANSLATE_UP -(d-D)*0.32
    return (OPPBP1, OPPBP2, OPPBP3)

class Entity():
    def __init__(self, D, R, WEIGHT, P,):
        self.D = D
        self.R = R
        self.P = P
        self.WEIGHT = WEIGHT

#0.1429
OPPU_Tank = Entity(7, 1000, 0.5, OPPUPotential())
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
    #print "PCliff called shift is: ", shift
    #scale = -180.0
    #if d > 1.0 - TOLERANCE and d < 4.0 + TOLERANCE:
     #   y = -80.0
    #else: 
    y = (PCLIFF_SCALE + TRANSLATE_UP)/(d**params)
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
    #return ndimage.distance_transform_cdt(gray < 1,"cityblock")
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
    #return ndimage.distance_transform_cdt(Y < 1, "cityblock")

def greaterThan(dSqr, value):
    return dSqr > value
def lessThan(dSqr, value):
    return dSqr < value

def Cross(u, v):
    z = [0, 0, 1]
    c = np.cross(z, [u, v])
    return (c[0], c[1])

def cleanUp(map, cleanZeros = False):
    map[np.isinf(map)] = 0
    map[np.isnan(map)] = 0
    if cleanZeros:
        for i in range(0, map.shape[0]):
            for j in range(0, map.shape[1]):
                if map[i,j] == 0 and (i == 0 or i > map.shape[0]):
                    map[i,j] = map[i+1,j]                
    return map


def test(fileName="city3_obs.png",scale=False):
    
    print "Computing PMaps:"
    obsPMapShift = GetObsPMap(fileName, shift=2.0, scale=scale)
    #obsPMap = GetObsPMap(fileName, shift=2)
    imgShape = obsPMapShift.shape
    print "obsPMap shape: ",imgShape
    print "COmputing gradient maps:"
    obsGradMapShift = np.gradient(obsPMapShift)
    #obsGradMapShiftU = obsPMapShift[0]
    #obsGradMapShiftV = obsPMapShift[1]
    obsGradMapShiftU = cleanUp(obsGradMapShift[0], cleanZeros=True)
    obsGradMapShiftV = cleanUp(obsGradMapShift[1], cleanZeros=True)
    print "Computing contour maps"
    #(cx, cy) = GetContourMap((obsGradMapShiftU, obsGradMapShiftV))
    #()
    #cx = cleanUp(cx)
    #cy = cleanUp(cy)
    print "computing combined map"
    multi = 1
    if scale:
       multi = 2 
       #308,0,303
    goal = (303*multi, 308*multi)
    goalPMap = GetUnitPMap([goal], imgSize = imgShape)
    combinePMap = obsPMapShift + goalPMap
    cGradMap = np.gradient(combinePMap)
    combinePMapU = cGradMap[0]
    combinePMapV = cGradMap[1]
    #combinePMapU = cleanUp(cGradMap[0])
    #combinePMapV = cleanUp(cGradMap[1])
    
    (cx, cy) = GetContourMap((combinePMapU, combinePMapV))
    #()
    cx = cleanUp(cx)
    cy = cleanUp(cy)
    
    #combinePMapU = cGradMap[0]
    #combinePMapV = cGradMap[1]
    return {"obsGradMapShiftU":obsGradMapShiftU, "obsGradMapShiftV":obsGradMapShiftV,
            "cx":cx, "cy":cy,
            "combinePMap":combinePMap,
            "combineGradMapU":combinePMapU,
            "combineGradMapV":combinePMapV}

def writeDataFiles(fileName = "data/testdata.zdt"):
    
    results = test()
    gradU = results["combineGradMapU"]
    gradV = results["combineGradMapV"]
    cu = results["cx"]
    cv = results["cy"]
    file = open(fileName,"wb")
    #write the header. The header consists of:
    #A unsigned int denoting a file code.
    #Followed by the shape of the number arrays, in both U and V. Since right now we assume the shapes are the same, we only
    #output a single shape for all num arrays which we output. 
    #Note: alternatively we can specify number of bytes instead of number of items.
    code = 420
    str = st.pack("L", code)
    file.write(str)
    print "writing file code..."
    shapeU = gradU.shape[0]
    shapeV = gradU.shape[1]
    str = st.pack("LL", shapeU, shapeV)
    print "Writing shape U,V"
    file.write(str)
    #Next we simply output (project) the num arrays into a 1D array. Since the shape U,V are given, one can re-project this 1D array
    #back into it's original dimensions.
    print "writing gradU"
    writeNumArray(file, gradU)
    print "writing gradV"
    writeNumArray(file, gradV)
    print "Writing cu"
    writeNumArray(file, cu)
    print "Writing cv"
    writeNumArray(file, cv)
    file.close()
    print "Finished writing file."
def writeNumArray(file, numArray):
    """We assume here that numArray is 2D."""
    for row in numArray:
        for col in row:
            str = st.pack("f", float(col)) #pack as float 
            file.write(str)
                
    
def GetContourMap(grads):
    x = np.empty(grads[0].shape)
    y = np.empty(grads[1].shape)
    for i, (rowU, rowV) in enumerate(zip(grads[0], grads[1])):
        tempU = []
        tempV = []
        for eU, eV in zip(rowU, rowV ):
            
            dotU = eU*eU
            dotV = eV*eV
            #if dotU < 0.0001 and dotV < 0.0001:
                #tempU.append(0.0) #pick arbitrary direction to get out of this craziness
                #tempV.append(0.0)
            #else:
            norm = np.sqrt(dotU+dotV)
            (u, v) = Cross(eU/norm, eV/norm)
            if math.isnan(u) or math.isnan(v):
                u = 0.0
                v = 0.0
            tempU.append(u)
            tempV.append(v)
        
            
            
            
        x[i] = tempU
        y[i] = tempV
    return (x, y)    

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




def GetObsPMap(obsFileName='testcitymap_new.png', shift=2, scale=False):
    
    if (scale):
        obsGray = scipy.misc.pilutil.imresize(scipy.misc.pilutil.imread('data/'+obsFileName, flatten=True),
                                          (1026, 1026))
    else:
        obsGray = scipy.misc.pilutil.imread('data/'+obsFileName, flatten=True)
    obsDMap = GetDistanceMapObstacles(obsGray)
    #cliff = functools.partial(PCliff, shift)
    obsPMap = GetPotentialMap(obsDMap, PCliff, shift)
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

