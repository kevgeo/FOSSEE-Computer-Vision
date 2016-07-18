I1 = imread("left.jpg");

values = extractHOGFeatures(I3,"BlockSize",[8,8],"CellSize",[8,8],"NumBins",9);  
//-> No error when shifting name-value pair arguments randomly
