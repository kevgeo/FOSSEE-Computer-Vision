//Checking if error message pops up when obpts is a N x 2 matrix instead of N x 3 
obpts = [ .5 .5;
	.5 .5 ;
	-.5 .5;
	.5 .5 ;
	.5 -.5 ;
	-.5 -.5;
	-.5 -.5];



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
0];

iterations = 5;

reprojectionError = 1;

minInliersCount = 3;

[rvec tvec] = solvePnPRansac(obpts,impts,camera,dist,1,"CV_ITERATIVE",iterations,reprojectionError,minInliersCount);

//output->
//                                                                                                                  !--error 999 
//Please make sure that objectPoints entered are 3D i.e having 3 coordinate values.
