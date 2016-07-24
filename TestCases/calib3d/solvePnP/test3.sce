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

[rvec tvec] = solvePnP(obpts,impts,camera,dist,1,"CV_ITERATIVE");

//output->
//!--error 999 
//Please make sure that imagePoints entered are 2D i.e having 2 coordinate values.
