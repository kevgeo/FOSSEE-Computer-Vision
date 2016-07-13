inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");

img = drawChessboardCorners(Image,7,200,coordinates,found);

//output->
//                              !--error 4 
//Undefined variable: Image
