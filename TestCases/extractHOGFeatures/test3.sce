I1 = imread("left.jpg");

values = extractHOGFeatures(I1,"CellSize",[8,8,8]);
//output-> Invalid Argument. Only two values for CellSize.
