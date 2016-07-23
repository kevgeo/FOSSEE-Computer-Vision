// Computes the affine transformation between two 3D point sets.
//
// Calling Sequence
// [affMat inliers] =estimateAffine3D(3Dpts_1,3Dpts_2,ransacThreshold,confidence);
// 
// Parameters 
// 3Dpts_1 : first point set
// 3Dpts_2 : second point set
// ransacThreshold : maximum reprojection error
// confidence : confidence value for estimated transformation
//
// Description
// The fucnction takes as input two 3D points sets and computes the affine transformation matrix "affMat"
// and the inlier points are stored in vector "inliers".
//
// Examples
// first = [ 0 0 1;
// 0 3 1;
// 4 3 1;
// 2 2 2;
// 7 1 2;
// 8 0 2];
// second = [ 0 0 3;
// 0 3 3;
// 4 3 3;
// 2 2 4;
// 7 1 4;
// 8 0 4];
//[affMat inliers] =estimateAffine3D(first,second,3,0.99);
//
// Author
// Kevin George
// 
