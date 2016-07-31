// Reprojects a disparity image to 3D space.
//
// Calling Sequence
// disp3D = reprojectImageTo3D(disparitymat,Q,handlemissingvalue);
// 
// Parameters 
// Input
// disparitymat : Disparity image
// Q : Perspective transformation matrix(4x4) which can be obtained from stereoRectify()
// handleMissingValues : Indicates whether the function should handle missing values(points where disparity is not computed)
// Output
// disp3D : 3-channel floating-point image of the same size as disparitymat
//
// Description
// The function transforms a single-channel disparity map to a 3-channel image representing a 3D surface. 
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
// disp = disparitySGBM(I1,I2,numofDisparities,minDisparity,sadwindowsize,p1,p2,maxDiff,prefilterCap,uniquenessratio,speckleWindowSize,SpeckleRange,fullDP);
// cam1=[100 0 105;0 106 107; 0 0 1];
// cam2=[100 0 105;0 106 107; 0 0 1];
// dis1=[0 0 0 4 5];
// dis2=[0 0 0 4 5];
// R=[1 2 3;4 5 6;0 8 7];
// T=[0 0 45];
// ImageSize = [480 640];
// [R1,R2,P1,P2,Q] = stereoRectify(cam1,dis1,cam2,dis2,ImageSize,R,T);
// handlemissingvalue = 0;
// disp3D = reprojectImageTo3D(disparitymat,Q,handlemissingvalue);
//
// Author
// Kevin George
// 
