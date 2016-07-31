// Converts the points from homogeneous to Euclidean space
//
// Calling Sequence
// pts_euclidean = convertPointsToHomogeneous(pts_homogenous);
// 
// Parameters 
// Input
// pts_homogenous : vector of N points in homogenous space 
// Output
// pts_euclidean : vector of N-1 points in Euclidean space
//
// Description
// The function converts vector of N points in homogenous space to a vector of N-1 dimensional points in Euclidean space.
//
// Examples
// pts_homogenous = [ -75 -90 12;
// -120 1 34];
// pts_euclidean = convertPointsFromHomogeneous(pts_homogenous);
//
// Author
// Kevin George
// 
