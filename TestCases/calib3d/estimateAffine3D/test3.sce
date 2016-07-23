first = [ 0 0 4 2 7 8;  
0 3 3 2 1 0;
1 1 1 2 2 2 ]

second = [ 0 0 4 2 7 8;
0 3 3 2 1 0;
3 3 3 4 4 4]

[affMat inliers] =estimateAffine3D(first,second,3,0.5);

//output->
//      !--error 999 
//Please enter 3D points.
