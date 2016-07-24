//Checking if error message pops up when impts is a N x 4 matrix instead of N x 2 
obpts = [ .5 .5 -.5;
	.5 .5 .5;
	-.5 .5 .5;
	.5 .5 .5;
	.5 -.5 -.5;
	-.5 -.5 -.5;
	-.5 -.5 .5];

impts = [282 274 120 320;
	397 227 122 321;
	577 276 121 233;
	462 378 124 121;
	270 479 345 222;
	450 523 111 134;
	566 476 113 456];

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
//        !--error 999 
//Please make sure that imagePoints entered are 2D i.e having 2 coordinate values.
