//Checking if function works
inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");
//output is correct
 
inImage2 = imread("left01.jpg");

[found2 coordinates2 ] = findChessboardCorners(inImage2,8,6,"CV_CALIB_CB_ADAPTIVE_THRESH");
//output is correct

