// Computes rectification transforms for calibrated stereo camera
//
// Calling Sequence
// [R1,R2,P1,P2,Q] = stereoRectify(cam1,dis1,cam2,dis2,ImageSize,R,T);
// 
// Parameters 
// Input
// cam1 : First camera matrix
// dis1 : First camera distortion parameters
// cam2 : Second camera matrix
// dis2 : Second camera distortion parameters
// ImageSize : Size of image
// R : Rotation matrix between coordinate systems of cameras
// T : Translation vector between coordinate systems of cameras
// Output:
// R1 : Rectification transform for first camera
// R2 : Rectification transform for second camera
// P1 : Projection matrix for first camera
// P2 : Projection matrix for second camera
// Q : Disparity-to-depth mapping matrix
// Description
// The function computes the rectification transforms, projection matrices and disparity to depth mapping matrix.
//
// Examples
// cam1=[100 0 105;0 106 107; 0 0 1];
// cam2=[100 0 105;0 106 107; 0 0 1];
// dis1=[0 0 0 4 5];
// dis2=[0 0 0 4 5];
// R=[1 2 3;4 5 6;0 8 7];
// T=[0 0 45];
// ImageSize = [480 640];
// [R1,R2,P1,P2,Q] = stereoRectify(cam1,dis1,cam2,dis2,ImageSize,R,T);
//
// Author
// Kevin George
// 
