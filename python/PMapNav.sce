TOLERANCE = 0.0001 //Setting abitrarily. Mainly we want to avoid C/0.0000001 as this creates a large
//number for our C's. In our case, this won't be a case because of the way 
//we are computing distance transforms based on pixels. That's why we don't set to %eps (machine eps)
//ALSO NOTE: We're not worrying too much about floating point details right now as this is
//mainly for protyping. Safe to say we have to think hard about this later-on.

function y = Pcliff(d,params)
  scale=-80
  if d > TOLERANCE then
    y = scale/(d**2) 
  else
    y = scale
  end 
endfunction

function y = isInRange(d,l,r)
  if (d >= l) & (d <= r) then 
    y = %t
  else
    y = %f
  end
endfunction

//This is temorary so we can create a Unit potential map to examine.
//Later we probably want to implement this so it gets apply a potential map,
//instead of creating two potential maps.
function y = Popp(d,params)
  if isInRange(d,0,params.D-2)
    y = params.P(1)(d,params.D)
  elseif isInRange(d,params.D-2,params.D)
    y = params.P(2)(d,params.D)
  elseif isInRange(d,params.D-2,params.R)
    y = params.P(3)(d,params.D)
  else
    y = 0
  end
  y = y * params.WEIGHT
endfunction

function Y = GetPotentialMap(X, P, params)
  Y = [] 
  for c=X,
    v = []
    for e=c' //column turns into row vector to do for each element in column.
      v = [v; P(e,params)] //append each P(e) into rows. In the end producing a N column vector.
    end
    Y = [Y,v] //append the column vector. In the end producing a MxN transformed output matrix.
  end
endfunction

function Y = GetDistanceMapObstacles(fileName)
  RGB = ReadImage('data\testcitymap_new.png')
  Image = RGB2Gray(RGB)
  EdgeImage = ~SegmentByThreshold(Image, 1)
  Y = double(DistanceTransform(EdgeImage))
endfunction

function Y=GetEdgeImageUnits(Units, Image, UnitDesc)
  if ~exists("UnitDesc","l")
    UnitDesc = [1,1] //A 1 x 1 sized Unit
  end
  for c=Units,
    for i=0:UnitDesc(1)-1 //number of rows
      for j=0:UnitDesc(2)-1 //number of cols
        Image(c(1,:)+i, c(2,:)+j) = uint8(1)
      end
    end
  end
  Y = ~SegmentByThreshold(Image, 1)
endfunction
//This function will get the distance map for a given set of units with position vectors as a
//2xN matric. Where each column vector is the position of the unit.
function Z = GetDistanceMapUnits(X,Image)
  for c=X,
    Image(c(1,:),c(2,:)) = uint8(1)
  end
  EdgeImage = ~SegmentByThreshold(Image, 1)
  Z = double(DistanceTransform(EdgeImage))   
endfunction
//Utility function to save the edge image for units out.
function Z = WriteEdgeImageUnit(X,Image)
  for c=X,
    for i=0:10,
      for j=0:10
        Image(c(1,:)+i,c(2,:)+j) = uint8(1)
      end
    end
  end
  EdgeImage = ~SegmentByThreshold(Image, 1)
  WriteImage(uint8(EdgeImage), 'data\EdgeImageBase.png') 
  Z=[] 
endfunction


function Y = GetParamsTList()
  Y = tlist(["PARAM_LIST","D","R","P","WEIGHT"],[],[],[],[0.1429])  
endfunction

//This function will get a vector of potential functions of OPP Units.
function Y = OPPUPotential()
  
  function i = OPPUP1(d,D)
    scale = 240.0
    if d < TOLERANCE
      i = 80
    else
      i = scale/(d*(D-2))
    end
  endfunction
  function j = OPPUP2(d,D)
    scale = 240.0
    if d < TOLERANCE
      j = 80
    else
      j = scale
    end
  endfunction
  function k = OPPUP3(d,D)
    scale = 240.0
    scale2 = 0.24
    if d < TOLERANCE
      k = 80
    else
      k = scale-scale2*(d-D)
    end
  endfunction
  Y = list(OPPUP1,OPPUP2,OPPUP3)
endfunction
//This function will get a vector of potential functions of OPP Base.
function Y = OPPBPotential()
  function i = OPPBP1(d,D)
    scale = 360.0
    if d < TOLERANCE 
      i = 0
    else 
      i = scale/((D-2)*d)
    end
  endfunction
  function j = OPPBP2(d,D)
    scale = 360.0
    if d < TOLERANCE
      j = 0
    else
      j = scale
    end
  endfunction
  function k = OPPBP3(d,D)
    scale = 360.0
    if d < TOLERANCE
      k = 0
    else
      k = scale - (d - D)*0.32
    end
  endfunction
  Y = list(OPPBP1,OPPBP2,OPPBP3)
endfunction

function Y = GetBiasedScaledPMap(X,bias)
  [nr,nc] = size(X)
  for i=1:nr,
    for j=1:nc,
      t = X(i,j)
      if t > 0.0
        X(i,j) = X(i,j) * bias
      end
    end
  end
  Y = []
endfunction

OPPU_Tank = GetParamsTList()
OPPU_Tank.D = 7
OPPU_Tank.R = 68
OPPU_Tank.P = OPPUPotential()

OPPU_Base = GetParamsTList()
OPPU_Base.D = 12
OPPU_Base.R = 1000
OPPU_Base.P = OPPBPotential()

BasePos = [61 39
250 131]

WORKING_PATH="E:/Users/gnulinux/projects/prjz/scilab/potential_path/"


function Y = ComputeEdgeImages()
  path = pwd()
  disp("Path is: " + path)
  BaseEdgeImg = uint8(zeros(512,512))
  ObsEdgeImg = uint8(zeros(512,512))
  RGB = ReadImage(WORKING_PATH+'data/testcitymap_new.png')
  Image = RGB2Gray(RGB)
  ObsEdgeImg = ~SegmentByThreshold(Image, 1)
  BaseEdgeImg = GetEdgeImageUnits(BasePos,BaseEdgeImg,[10,10])
  Y = tlist(["EdgeImages","BaseEdgeImg","ObsEdgeImg"],[BaseEdgeImg],[ObsEdgeImg])   
endfunction

function WriteEdgeImagesAsPng(EdgeImages)
  nc = size(EdgeImages)
  for i=2:nc,
    WriteImage(uint8(EdgeImages(i)), WORKING_PATH+'data/'+EdgeImages(1)(i)+'.png')
  end
endfunction

function Y = GetDistanceMaps()
  DMapBase = read(WORKING_PATH+'data/BaseEdgeImg.mtd',-1,512)
  DMapObs = read(WORKING_PATH+'data/ObsEdgeImg.mtd',-1,512)
  Y = tlist(["DistanceMapsList","DMapBase","DMapObs"],[DMapBase],[DMapObs])
endfunction

function Y = GetPotentialMaps(DMaps)
  PMapObs = GetPotentialMap(DMaps.DMapObs, Pcliff, 0)
  PMapBase = GetPotentialMap(DMaps.DMapBase, Popp, OPPU_Base)
  PMapCombined = PMapObs + PMapBase
  Y = tlist(["PMapsList","PMapObs","PMapBase","PMapCombined"],[PMapObs],[PMapBase],[PMapCombined])
endfunction



function Y = GetResults()
  BaseImage = uint8(zeros(512,512))
  DistanceMapObs = GetDistanceMapObstacles()
  DistanceMapBase = GetDistanceMapUnits(BasePos,BaseImage)
  WriteEdgeImageUnit(BasePos,BaseImage)
  clear('BaseImage')
  PMapObs = GetPotentialMap(DistanceMapObs, Pcliff, 0)
  PMapBase = GetPotentialMap(DistanceMapBase, Popp, OPPU_Base)
  PMapCombined = PMapObs + PMapBase
  Y = tlist(["PMapNavResults","PMapObs","PMapBase","PMapCombined","DistanceMapObs","DistanceMapBase"],[PMapObs],[PMapBase],[PMapCombined],[DistanceMapObs], [DistanceMapBase])  
endfunction

function Y = GetBaseImage()
  Y = uint8(zeros(512,512))
endfunction




 
  
    