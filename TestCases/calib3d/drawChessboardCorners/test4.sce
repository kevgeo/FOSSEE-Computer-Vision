//Checking if error message pops up when wrong Image variable is used as input
inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img = drawChessboardCorners(Image,7,200,coordinates,found);

//output->
//                              !--error 4 
//Undefined variable: Image

//Checking if error message pops up when wrong Image variable is used as input
inImage2 = imread("1.jpg");

[found2 coordinates2 ] = findChessboardCorners(inImage2,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img2 = drawChessboardCorners(Image,7,7,coordinates2,found2);
//output->
//                              !--error 4 
//Undefined variable: Image

//Checking if error message pops up when wrong Image variable is used as input
inImage3 = imread("left12.jpg");

[found3 coordinates3 ] = findChessboardCorners(inImage3,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");

img3 = drawChessboardCorners(Image,9,6,coordinates3,found3);
//output->
//                              !--error 4 
//Undefined variable: Image

//Checking if error message pops up when wrong Image variable is used as input
inImage4 = imread("left01.jpg");

[found4 coordinates4 ] = findChessboardCorners(inImage4,9,6,"CV_CALIB_CB_ADAPTIVE_THRESH");

img4 = drawChessboardCorners(Image,9,6,coordinates4,found4);
//output->
//                              !--error 4 
//Undefined variable: Image
