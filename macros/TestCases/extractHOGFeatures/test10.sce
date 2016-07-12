I1 = imread("left.jpg");

values = extractHOGFeatures(I3,"CellSize",[4,4],"BlockSize",[8,8],"NumBins",9);
//-> No error
