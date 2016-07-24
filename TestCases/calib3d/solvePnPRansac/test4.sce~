//Checking if error message pops up when dist vector is of size 6 when permissible size is 4,5 or 8
obpts = [ .5 .5 -.5;
	.5 .5 .5;
	-.5 .5 .5;
	.5 .5 .5;
	.5 -.5 -.5;
	-.5 -.5 -.5;
	-.5 -.5 .5];



impts = [282 274;
	397 227;
	577 276;
	462 378;
	270 479;
	450 523;
	566 476];

camera = [ 1 0 0;
0 1 0;
0 0 1];

dist = [0
0
0
0
0
0];

iterations = 5;

reprojectionError = 1;

minInliersCount = 3;

[rvec tvec] = solvePnPRansac(obpts,impts,camera,dist,1,"CV_ITERATIVE",iterations,reprojectionError,minInliersCount);

//output->
//                                                                                                                    !--error 999 
//Please enter column vector of distortion coefficients either with size 4,5 or 8.
