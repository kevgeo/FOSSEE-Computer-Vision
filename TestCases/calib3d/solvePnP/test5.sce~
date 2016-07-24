//Checking if error message pops up when camera matrix is 3x4 instead of 3x3
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

camera = [ 1 0 0 11;
0 1 0 0;
0 0 1 12];

dist = [0
0
0
0];

[rvec tvec] = solvePnP(obpts,impts,camera,dist,1,"CV_ITERATIVE");

//output->
//                                       !--error 999 
//Please make sure that camera matrix is 3x3.
 
