//Checking if error message pops up when number of input arguments is wrong
inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img = drawChessboardCorners(inImage,7,7,coordinates,);

//output->
//API Error:
//	in getScalarDouble: Unable to get argument #5
//	in getMatrixOfDouble: Invalid argument type, double matrix expected

//Checking if error message pops up when number of input arguments is wrong
inImage2 = imread("1.jpg");

[found2 coordinates2 ] = findChessboardCorners(inImage2,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img2 = drawChessboardCorners(inImage2,7,7,coordinates2,);
//output->
//API Error:
//	in getScalarDouble: Unable to get argument #5
//	in getMatrixOfDouble: Invalid argument type, double matrix expected

//Checking if error message pops up when number of input arguments is wrong
inImage3 = imread("left12.jpg");

[found3 coordinates3 ] = findChessboardCorners(inImage3,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");

img3 = drawChessboardCorners(inImage3,9,6,coordinates3,);
//output->
//API Error:
//	in getScalarDouble: Unable to get argument #5
//	in getMatrixOfDouble: Invalid argument type, double matrix expected

//Checking if error message pops up when number of input arguments is wrong
inImage4 = imread("left01.jpg");

[found4 coordinates4 ] = findChessboardCorners(inImage4,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");

img4 = drawChessboardCorners(inImage4,9,6,coordinates4,);
//output->
//API Error:
//	in getScalarDouble: Unable to get argument #5
//	in getMatrixOfDouble: Invalid argument type, double matrix expected

