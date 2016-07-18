I1 = imread("left.jpg");

values = extractHOGFeatures(I3,"BlockSize",[8,],"CellSize",[8,8],"NumBins",9);
//output->Invalid Argument.Only two values for BlockSize.
