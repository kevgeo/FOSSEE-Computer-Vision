inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img = drawChessboardCorners(inImage,7,7,coordinates,found);
//output is correct

//Checking if function works for different image
inImage2 = imread("1.jpg");

[found2 coordinates2 ] = findChessboardCorners(inImage2,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img2 = drawChessboardCorners(inImage2,7,7,coordinates2,found2);
//ouput is correct

//Checking if function works for different image
inImage3 = imread("left12.jpg");

[found3 coordinates3 ] = findChessboardCorners(inImage3,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");

img3 = drawChessboardCorners(inImage3,9,6,coordinates3,found3);
//output is correct

//Checking if function works for different image
inImage4 = imread("left01.jpg");

[found4 coordinates4 ] = findChessboardCorners(inImage4,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");

//Checking if function works for different image
img4 = drawChessboardCorners(inImage4,9,6,coordinates4,found4);
//output is correct

