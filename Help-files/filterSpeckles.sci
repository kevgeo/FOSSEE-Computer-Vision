// Filters off smalll speckles(blobs) in disparity map
//
// Calling Sequence
// img = filterSpeckles(disp,newval,maxSpeckleSize,maxDiff);
// 
// Parameters 
// Input
// disp: disparity map
// newval: disparity value to paint off the speckles
// maxSpeckleSize: maximum size to consider as speckle
// maxDiff: Maximum difference value between neighbour disparity pixels to put them into same speckle(blob). 
// Output
// img: filtered disparity image
//
// Description
// The function filters off smalll speckles(blobs) in the disparity map.
//
// Examples
// //Reading first stereo image
// I1 = imread("stereo1.png");
// //Reading second stereo image
// I2 = imread("stereo2.png");
// //Number of disparities
// numofDisparities = 144;
// //Minimum of disparity value
// minDisparity = 0;
// //Matched blocked size
// sadwindowsize = 3;
// //Get first parameter controlling disparity smoothness
// p1 = 36;
// //Get second parameter controlling disparity smoothness
// p2 = 288;
// //Get maximum allowed difference value
// maxDiff = 1;
// //Get prefilterCap value
// prefilterCap = 10;
// //Get unique ratio value
// uniquenessratio = 10;
// //Get speckle window size value 
// speckleWindowSize = 100;
// //Get speckle range value
// SpeckleRange = 32;
// //Get fullDp value
// fullDP = 1;
// disp = disparity(I1,I2,numofDisparities,minDisparity,sadwindowsize,p1,p2,maxDiff,prefilterCap,uniquenessratio,speckleWindowSize,SpeckleRange,fullDP);
// //Get disparity value to paint off speckles
// newval = 5;
// //Get maximum speckle size
// maxSpeckleSize = 12;
// //Get maxxDiff value
// maxDiff = 3;
// img = filterSpeckles(disp,newval,maxSpeckleSize,maxDiff);
//
// Author
// Kevin George
// 
