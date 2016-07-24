// Computes pose of object from 2D and 3D points using Ransac method.
//
// Calling Sequence
// [rvec tvec] = solvePnP(objectPoints,imagePoints,cameraMatrix,distortionCoeffs,useExtrinsicGuess,
//                         flags,IterationsCount,reprojectionError,minInliersCount);
// 
// Parameters 
// Input
// MatrixOfDouble objectPoints : array of object points
// MatrixOfDouble imagePoints : array of corresponding image points
// MatrixOfDouble cameraMatrix : input matrix
// MatrixOfDouble distortionCoeffs - vector of distortion coefficients
// ScalarDouble useExtrinsicGuess : If true(1), function provides rotation and translation vector values
// MatrixOfString flags : method for computing PNP
//         1.CV_ITERATIVE - based on Levenberg-Marquardt optimization.
//         2.CV_P3P - based on paper,“Complete Solution Classification for the Perspective-Three-Point Problem”, 
//	              by X.S. Gao, X.-R. Hou, J. Tang, H.-F. Chang.
//         3.CV_EPNP - based on paper “EPnP: Efficient Perspective-n-Point Camera Pose Estimation”,
//                     by F.Moreno-Noguer, V.Lepetit and P.Fua . 
// IterationsCount : Number of iterations
// reprojectionError : This is threshold for inlier points
// minInliersCount : Number of inlier points to be found
// Output
// MatrixOfDouble rvec : Rotation vector 
// MatrixOfDouble tvec : Translation vector
//
// Description
// The function computes pose of object from 2D and 3D points using Ransac method.
//
// Examples
// obpts = [ .5 .5 -.5;
// 	     .5 .5 .5;
//	     -.5 .5 .5;
//	      .5 .5 .5;
//	     .5 -.5 -.5;
//	     -.5 -.5 -.5;
//  	     -.5 -.5 .5];
// impts = [282 274;
//	    397 227;
//	    577 276;
//          462 378;
// 	    270 479;
//          450 523;
//          566 476];
// camera = [ 1 0 0;
//            0 1 0;
//            0 0 1];
// dist = [0
//         0
//         0
//         0];
//iterations = 5;
//reprojectionError = 1;
//minInliersCount = 3;
//[rvec tvec] = solvePnPRansac(obpts,impts,camera,dist,1,"CV_ITERATIVE",iterations,reprojectionError,minInliersCount);
//
// Author
// Kevin George
// 
