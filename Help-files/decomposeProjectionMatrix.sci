// Decomposes a projection matrix into a camera matrix and a rotation matrix. 
//
// Calling Sequence
// [cam rot trans rotX rotY rotZ euler] = decomposeProjectionMatrix(projMatrix);
// 
// Parameters 
// Input
// projMatrix : 3x4 Projection matrix
// Output
// cam : 3x3 Camera matrix
// rot : 3x3 Rotation matrix
// trans : 4x1 Translation vector 
// rotX(optional) : 3x3 Rotation matrix around x axis
// rotY(optional) : 3x3 Rotation matrix around y axis
// rotZ(optional) : 3x3 Rotation matrix around z axis
// euler : contains three euler angles of rotation in degrees
//
// Description
// The function decomposes a projection matrix that returns a camera matrix, rotation matrix and calibratiion matrix.
// It optionally returns rotation matrices around the 3 axes and euler angle.
//
// Examples
// projMatrix =[ 1 0 5 1
// 0 1 4 2;
// 0 0 1 3];
// [cam rot trans rotX rotY rotZ euler] = decomposeProjectionMatrix(inpt);
//
// Author
// Kevin George
// 
