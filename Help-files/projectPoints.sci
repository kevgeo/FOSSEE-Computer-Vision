// Takes in 3D points and outputs the corresponding 2D points in image plane.
//
// Calling Sequence
// projPts = projectPoints(obpts,rvec,tvec, camera, dist, aspectRatio);
// 
// Parameters 
// Input
// obpts : array of object points
// rvec : rotation vector
// tvec : translation vector
// camera : input matrix
// dist : vector of distortion coefficients
// aspectRatio : fixed aspect ratio parameter
// Output
// projPts : Array of image points
//
// Description
// This function returns array of image points(2D).
//
// Examples
// obpts = [ .5 .5 -.5;
// 	.5 .5 .5;
// 	-.5 .5 .5;
// 	-.5 .5 -.5;
// 	.5 -.5 -.5;
// 	-.5 -.5 -.5;
// 	-.5 -.5 .5];
// impts = [282 274;
// 	397 227;
// 	577 271;
// 	462 318;
// 	270 479;
// 	450 523;
//	566 475];
// camera = [ 1 0 0;
// 0 1 0;
// 0 0 1]
// dist = [0
// 0
// 0
// 0];
// iterations = 5;
// reprojectionError = 2;
// minInliersCount = 3;
// [rvec tvec] = solvePnPRansac(obpts,impts,camera,dist,1,"CV_ITERATIVE",iterations,reprojectionError,minInliersCount);
// projPts = projectPoints(obpts,rvec,tvec, camera, dist, 2);
//
// Author
// Kevin George
// 
