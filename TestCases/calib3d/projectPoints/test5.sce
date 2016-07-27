//Checking if error message pops up when camera matrix is Nx2 instead of Nx3
obpts = [ .5 .5 -.5;
	.5 .5 .5;
	-.5 .5 .5;
	-.5 .5 -.5;
	.5 -.5 -.5;
	-.5 -.5 -.5;
	-.5 -.5 .5];

camera = [ 1 0 ;
0 1 ;
0 0 ]

dist = [0
0
0
0];

rvec = [0 0 0];

tvec = [0 0 0];

projPts = projectPoints(obpts,rvec,tvec, camera, dist, 2);

//output->
//                !--error 999 
//Please make sure that camera matrix is 3x3.
 
