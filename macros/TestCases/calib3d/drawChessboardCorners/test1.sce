inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img = drawChessboardCorners(inImage,7,7,coordinates,found);
//output is correct