obpts = [ .5 .5 -.5 -.5 .5 -.5 -.5;
.5 .5 .5 .5 -.5 -.5 -.5;
-.5 .5 .5 -.5 -.5 -.5 .5]

impts = [282 397 577 462 270 450 566
274 227 271 318 479 523 475]

camera = [ 1 0 0;
0 1 0;
0 0 1]

dist = [0
0
0
0]

iterations = 5;

reprojectionError = 2;

minInliersCount = 3;

[rvec tvec] = solvePnPRansac(obpts,impts,camera,dist,1,"CV_ITERATIVE",iterations,reprojectionError,minInliersCount);

projPts = projectPoints(obpts,rvec,tvec, camera, dist, 2);

//output is wrong
