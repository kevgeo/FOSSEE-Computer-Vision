//Checking if error message pops up when "dist" size is 6 instead of 4,5 and 8
obpts = [ .5 .5 -.5;
	.5 .5 .5;
	-.5 .5 .5;
	-.5 .5 -.5;
	.5 -.5 -.5;
	-.5 -.5 -.5;
	-.5 -.5 .5];

camera = [ 1 0 0;
0 1 0;
0 0 1]

dist = [0
0
0
0
0
0];

rvec = [0 0 0];

tvec = [0 0 0];

projPts = projectPoints(obpts,rvec,tvec, camera, dist, 2);

//output->
//          !--error 999 
//Please enter column vector of distortion coefficients either with size 4,5 or 8.
