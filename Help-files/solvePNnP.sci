// Computes pose of object from 2D and 3D points
//
// Calling Sequence
// [rvec tvec] = solvePnP(objectPoints,imagePoints,cameraMatrix,distortionCoeffs,useExtrinsicGuess,flags);
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
// Output
// MatrixOfDouble rvec : Rotation vector 
// MatrixOfDouble tvec : Translation vector
//
// Description
// The function computes pose of object from 2D and 3D points.
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
//[rvec tvec] = solvePnP(obpts,impts,camera,dist,1,"CV_ITERATIVE");
//
// Author
// Kevin George
// 
