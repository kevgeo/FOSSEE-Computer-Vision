I1 = imread("left.jpg");

values = extractHOGFeatures(I3,"CellSize",[8,8],"BlockSize",[16,8],"BlockOverlap",[8,8],"NumBins",9);
//-> No error
