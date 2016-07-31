// Finds the camera intrinsic and extrinsic parameters from several views of a calibration pattern
//
// Calling Sequence
// [rvecs tvecs] = calibrateCamera(list_size,obPts,impts,width,height,camera1,dist); 
// 
// Parameters 
// Input
// matrixofdouble obpts : vector of vector of calibration points
// matrixofdouble impts : vector of vector of projections of calibration points
// double width,height : size of image in pixels
// matrixofdouble camera1 : camera matrix
// matrixofdouble dist1 : vector of distortion coefficients
// Output
// rvecs : Output vector of rotation vectors
// tvecs : Output vector of tranlslaton vectors 
//
// Description
// The function finds the camera intrinsic and extrinsic parameters from several views of a calibration pattern.
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
// 	566 475];
// camera1 = [1 0 0
// 0 1 0
// 0 0 1];
// dist = [0 
// 0
// 0
// 0];
// obPts = list(obpts);
// imPts = list(impts)
// [rvecs tvecs] = calibrateCamera(1,obPts,impts,1280,1024,camera1,dist);
//
// Author
// Kevin George
// 
