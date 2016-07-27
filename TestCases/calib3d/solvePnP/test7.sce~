//Checking if error message pops up when number of object and image points differ
obpts = [ .5 .5 -.5;
	.5 .5 .5;
	-.5 .5 .5;
	-.5 .5 -.5;
	.5 .5 -.5;];



impts = [282 274;
	397 227;
	577 271;
	462 318;];

camera = [ 3 0 0;
1 2 0;
1 0 0]

dist = [0
0
0
0];

iterations = 5;

reprojectionError = 2;

minInliersCount = 3;

[rvec tvec] = solvePnP(obpts,impts,camera,dist,1,"CV_EPNP");

//output->
//                                                   !--error 999 
//The number of object points and image points should be the same.
