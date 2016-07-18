I1 = imread("left.jpg");

values = extractHOGFeatures(I3,"CellSize",[8,4],"BlockSize",[8,8],"BlockOverlap",[18,8],"NumBins",9);
//output-> BlockOverlap values cannot be more than 16.

