//Checking if function works for different values
obpts = [ 1 2 -3;
	3.3 1.5 .2;
	1.1 2.5 2.4;
	.2 1.5 3.2;
	2.3 -1.5 -.5;
	-1 -3 5.5;
	-1.5 -.5 3.5];

impts = [282 274;
	397 227;
	577 276;
	462 378;
	270 479;
	450 523;
	566 476];

camera = [ 1 0 0;
0 1 0;
0 0 1]

dist = [0
0
0
0];

[rvec tvec] = solvePnP(obpts,impts,camera,dist,1,"CV_EPNP");

//output is correct
