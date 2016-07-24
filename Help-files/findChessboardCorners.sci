// Finds the internal corners of the chessboard.
//
// Calling Sequence
// [found coordinates ] = findChessboardCorners(Image,patternsize_row,patternsize_col,flags);
// 
// Parameters 
// Input
// Image : Path of image file
// double patternsize : Number of inner corners per chessboard per row and column
// string flags : Apply different methods to find internal corners. These include
//        1.CV_CALIB_CB_ADAPTIVE_THRESH - uses adaptive thresholding
//        2.CV_CALIB_CB_NORMALIZE_IMAGE - normalizes the image
//        3.CALIB_CB_FAST_CHECK - a fast check to detect corners and if none is found, the search is called off
//        4.CV_CALIB_CB_FILTER_QUADS - filter out false quads
// Output
// bool Found : Boolean value stating whether corner has been found(0) or not(1)
// matrixofdouble Corners : array of detected corners
//
// Description
// This function finds the internal corners of the chessboard and outputs the detected corners. 
//
// Examples
// Image = imread("chessboard3.png");
// [found coordinates ] = findChessboardCorners(Image,7,7,"CV_CALIB_CB_ADAPTIVE_THRESH");
//
// Author
// Kevin George
// 
