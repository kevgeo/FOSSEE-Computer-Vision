inImage = imread("chessboard3.png");

[a b ]= findCirclesGrid(inImage,7,7,"CALIB_CB_SYMMETRIC_GRID");

//output is correct
