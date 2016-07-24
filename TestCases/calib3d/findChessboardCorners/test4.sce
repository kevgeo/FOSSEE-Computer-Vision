//Checking if error message pops up when wrong flag name is given
inImage = imread("chessboard3.png");

[found coordinates ] = findChessboardCorners(inImage,12,12,"CV_CALIB_CB_AD_THRESH");

//output->
//Wrong flag value used. Look at documentation for correct flag names.
 
