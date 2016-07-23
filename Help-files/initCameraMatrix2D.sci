// Returns a new optimal camera matrix
//
// Calling Sequence
// new_cameraMatrix = initCameraMatrix2D(Size,objectPoints,imagePoints,ImageSize_width,ImageSize_height,aspectRatio);
// 
// Parameters 
// Size : Number of vectors of calibration points and its projections
// 0bjectPoints : This is a vector of vector of calibration points
// ImagePoints : This is a vector of vector of projections of calibration points
// ImageSize_width : Specifies the width of image in pixels
// ImageSize_height : Specifies the height of image in pixels
// AspectRatio : If it is zero or negtive, the focal lengths in x an y direction are estimated indepen-dently. Else it is fx=(fy*AspectRatio)
//
// Description
// This function takes as input the calibration points and their projections and returns a camera matrix.
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
// new_cameraMatrix = initCameraMatrix2D(1,objectPoints,imagePoints,1280,1024,0);
//
// Author
// Kevin George
// 
