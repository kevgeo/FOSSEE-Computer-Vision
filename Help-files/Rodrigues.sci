// Converts a rotation vector to rotation matrix or vice-versa
//
// Calling Sequence
// [rotation jacobian]  = Rodrigues(rot);
// 
// Parameters 
// rot : Rotation matrix or vector
//
// Description
// The function converts a rotation vector to rotation matrix or matrix to vector and also returns an optional output which contains matrix of
// partial derivatives of first output with respect to input.
//
// Examples
// //Vector values
// rotvec = [12;
// 5;
// 7];
// //Converting "rotvec" vector to a rotation matrix
// [rotmatrix jacobian2 ] = Rodrigues(rotvec);
//
// Author
// Kevin George
// 
