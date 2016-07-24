// Calculates useful camera properties from the camera matrix
//
// Calling Sequence
// [fovx fovy focal ppoint aspectRatio ] = calibrationMatrixValues(cameraMatrix,width,height,apertureWidth,apertureHeight);
// 
// Parameters
// Input 
// matrixofdouble cameraMatrix : Input camera matrix
// double width : width of image
// double height : height of image
// double apertureWidth : width of sensor
// double apertureHeight : height of sensor
// Output
// fovx : field of view along horizontal sensor axis
// fovy : field of view along vertical sensor axis
// focal : focal length of lens(mm)
// ppoint : principal point in pixels
// aspectRatio : It is ratio of focal length(pixels) in vertical direction to focal length in horizontal direction.
//
// Description
// The function computes the useful camera properties from the camera matrix such as focal length,aspect ratio and field of view.
//
// Examples
// camera = [2 33 12
// 3 5 6
// 1 56 7];
// width = 100;
// height = 200;
// apertureWidth = 1;
// apertureHeight = 1;
// [fovx fovy focal ppoint apectRatio ] = calibrationMatrixValues(camera,width,height,apertureWidth,apertureHeight);
//
// Author
// Kevin George
// 
