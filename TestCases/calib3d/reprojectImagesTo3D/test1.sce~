//Reading first stereo image
I1 = imread("stereo1.png");
//Reading second stereo image
I2 = imread("stereo2.png");

// Number of disparities
numofDisparities = 144;
// Minimum of disparity value
minDisparity = 0;
// Matched blocked size
sadwindowsize = 3;

//Get first parameter controlling disparity smoothness
p1 = 36;
//Get second parameter controlling disparity smoothness
p2 = 288;

// Get maximum allowed difference value
maxDiff = 1;

// Get prefilterCap value
prefilterCap = 10;

// Get unique ratio value
uniquenessratio = 10;

// Get speckle window size value 
speckleWindowSize = 100;

// Get speckle range value
SpeckleRange = 32;

// Get fullDp value
fullDP = 1;

disparitymat = disparity(I1,I2,numofDisparities,minDisparity,sadwindowsize,p1,p2,maxDiff,prefilterCap,uniquenessratio,speckleWindowSize,SpeckleRange,fullDP);

cam1=[100 0 105;0 106 107; 0 0 1];
cam2=[100 0 105;0 106 107; 0 0 1];
dis1=[0 0 0 4 5];
dis2=[0 0 0 4 5];
r=[1 2 3;4 5 6;0 8 7];
t=[0 0 45];
ImageSize = [480 640];

[R1,R2,P1,P2,Q ] = stereoRectify(cam1,dis1,cam2,dis2,ImageSize,r,t);

handlemissingvalue = 0;

out = reprojectImageTo3D(disparitymat,Q,handlemissingvalue);
