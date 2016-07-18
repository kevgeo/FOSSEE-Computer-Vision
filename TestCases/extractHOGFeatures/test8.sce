I1 = imread("left.jpg");

values = extractHOGFeatures(I3,"CellSize",[16,4],"BlockSize",[8,8],"BlockOverlap",[16,8],"NumBins",9);
//output-> CellSize values cannot be more than BlockSize values.
