//Checking if function works
inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");
//output is correct
 
//Checking if function works for different image
inImage2 = imread("1.jpg");

[found2 coordinates2 ] = findChessboardCorners(inImage2,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");
//output is correct
 
//Checking if function works for different image
inImage3 = imread("left12.jpg");

[found3 coordinates3 ] = findChessboardCorners(inImage3,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");
//output is correct


//Checking if function works for different image
inImage4 = imread("left01.jpg");

[found4 coordinates4 ] = findChessboardCorners(inImage4,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");
//output is correct

