//Checking if function works for other stereo images
//Reading first stereo image(grayscale)
I1 = imread("left01.jpg");
//Reading second stereo image(grayscale)
I2 = imread("left02.jpg");

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

disp = disparitySGBM(I1,I2,numofDisparities,minDisparity,sadwindowsize,p1,p2,maxDiff,prefilterCap,uniquenessratio,speckleWindowSize,SpeckleRange,fullDP);

