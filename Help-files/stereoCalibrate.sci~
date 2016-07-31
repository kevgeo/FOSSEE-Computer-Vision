// Takes in 3D points and outputs the corresponding 2D points in image plane
//
// Calling Sequence
// [rotMatrix Transvec EMatrix FMatrix] = stereoCalibrate(list_size,calibPoints,ProjcalibPoints1,ProjcalibPoints2,camera1,dist1,camera2,dist2,width,height);
// 
// Parameters 
// Input
// matrixofdouble calibPoints : vector of vector of calibration points
// matrixofdouble ProjcalibPoints1 : vector of vector of projection of calibration points by first camera
// matrixofdouble ProjcalibPoints2 : vector of vector of projection of calibration points by second camera
// matrixofdouble camera1 : first camera matrix
// matrixofdouble dist1 : vector of distortion coefficients for first camera
// matrixofdouble camera2 : second camera matrix
// matrixofdouble dist2 : vector of distortion coefficients for second camera
// double width,height : size of image in pixels
// Output
// rotMatrix : rotation matrix
// TransVec : translation vector
// EMatrix : essential matrix
// FMatrix : fundamental matrix
//
// Description
// The function outputs the corresponding 2D points in image plane.
//
// Examples
// inImage = imread("1.jpg");
// inImage2 = imread("2.jpg");
// [found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");
// [found2 coordinates2 ] = findChessboardCorners(inImage2,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");
// obpts = [0	0	0
// 1	0	0
// 2	0	0
// 3	0	0
// 4	0	0
// 5	0	0
// 6	0	0
// 0	1	0
// 1	1	0
// 2	1	0
// 3	1	0
// 4	1	0
// 5	1	0
// 6	1	0
// 0	2	0
// 1	2	0
// 2	2	0
// 3	2	0
// 4	2	0
// 5	2	0
// 6	2	0
// 0	3	0
// 1	3	0
// 2	3	0
// 3	3	0
// 4	3	0
// 5	3	0
// 6	3	0
// 0	4	0
// 1	4	0
// 2	4	0
// 3	4	0
// 4	4	0
// 5	4	0
// 6	4	0
// 0	5	0
// 1	5	0
// 2	5	0
// 3	5	0
// 4	5	0
// 5	5	0
// 6	5	0
// 0	6	0
// 1	6	0
// 2	6	0
// 3	6	0
// 4	6	0
// 5	6	0
// 6	6	0
// ];
// camera1 = [1 0 0
// 0 1 0
// 0 0 1];
// camera2 = camera1;
// dist = [0 
// 0
// 0
// 0];
// dist2 = dist;
// dist1 = dist;
// width = 700;
// height = 525;
// obPts = list(obpts);
// coordinates1 = list(coordinates);
// coordinates2 = list(coordinates2);
//[rotMatrix Transvec EMatrix FMatrix] = stereoCalibrate(1,obPts,coordinates1,coordinates2,camera1,dist1,camera2,dist2,width,height);
//
// Author
// Kevin George
// 
