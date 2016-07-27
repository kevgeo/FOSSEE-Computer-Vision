//Checking if function works when obpts and impts values have been changed
obpts = [ .5 .5 -.5;
	.5 .5 .5;
	-.5 .5 .5;
	-.5 .5 -.5;
	.5 .5 -.5;];



impts = [282 274;
	397 227;
	577 271;
	462 318;
	271 479;];

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

[rvec tvec] = solvePnPRansac(obpts,impts,camera,dist,1,"CV_ITERATIVE",iterations,reprojectionError,minInliersCount);

projPts = projectPoints(obpts,rvec,tvec, camera, dist, 4);

//output is correct
