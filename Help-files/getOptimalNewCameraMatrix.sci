// Returns a new optimal camera matrix
//
// Calling Sequence
// new_cameraMatrix = getOptimalNewCameraMatrix(cameraMatrix,distCoeffs,imageSize_width,imageSize_height,alpha,
//                                              new_imageSize_width,new_imageSize_height,CenterPrincipalPoint);
// 
// Parameters 
// cameraMatrix : original camera matrix
// distCoeffs : vector of distortion coefficients
// imageSize : size of image in pixels
// alpha : free scaling parameter between 0 and 1
// new_imageSize : New size after correction. By default, it is set to original image size
// CenterPrincipalPoint : Optional flag specifying whether the principal point should be at the image
// center or not in new camera matrix
//
// Description
// The function takes as input the original camera matrix and array of distortion coefficients to return a new optimal camera matrix.
//
// Examples
// a = [144.00 1011.0;
// 237.0 801.0;
// 242.0 583.0;
// 271.0 421.0];
// imagePoints = list(a)
// b = [18.0 18.0 0;
// 25.0 110.0 0;
// 26.0 226.0 0;
// 29.0 327.0 0];
// objectPoints = list(b)
// cameraMatrix = initCameraMatrix2D(1,objectPoints,imagePoints,1280,1024,0);
// dist = [0
//	   0
//         0
//         0];
// new_cameraMatrix = getOptimalNewCameraMatrix(cameraMatrix,dist,1280,1024,0.3,1280,1024,0);
//
// Author
// Kevin George
// 
