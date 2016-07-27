//Checking if function works for different set of stereo imagess
inImage = imread("left01.jpg");
inImage2 = imread("right01.jpg");
[found coordinates ] = findChessboardCorners(inImage,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");
[found2 coordinates2 ] = findChessboardCorners(inImage2,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");

obpts = [ .5 .5 -.5;
	.5 .5 .5;
	-.5 .5 .5;
	.5 .5 .5;
	.5 -.5 -.5;
	-.5 -.5 -.5;
	-.5 -.5 .5];

camera1 = [1 0 0
0 1 0
0 0 1];

camera2 = camera1;
dist = [0 
0
0
0];
dist2 = dist;
dist1 = dist;
width = 640;
height = 480;
obPts = list(obpts);
coordinates1 = list(coordinates);
coordinates2 = list(coordinates2);

[rotMatrix Transvec EMatrix FMatrix] = stereoCalibrate(1,obPts,coordinates1,coordinates2,camera1,dist1,camera2,dist2,width,height);

//output is correct
