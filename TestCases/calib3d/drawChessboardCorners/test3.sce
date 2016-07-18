inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img = drawChessboardCorners(inImage,7,7,coordinates,);

//output->
//API Error:
//	in getScalarDouble: Unable to get argument #5
//	in getMatrixOfDouble: Invalid argument type, double matrix expected
