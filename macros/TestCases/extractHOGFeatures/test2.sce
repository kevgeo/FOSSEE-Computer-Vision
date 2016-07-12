I1 = imread("left.jpg");

values = extractHOGFeatures(I1,"CellSize",[8,8]);
//->Correct output is given
