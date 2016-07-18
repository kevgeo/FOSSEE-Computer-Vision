//**************************************************************************
//"output->" shows the error on console
//"->" means that output is correct and no modification needs to be done
//**************************************************************************
I1 = imread("left.jpg");

values = extractHOGFeatures(I1);
//-> Correct output is given 

values = extractHOGFeatures(I1,"CellSize",[8,8]);
//->Correct output is given

values = extractHOGFeatures(I1,"CellSize",[8,8,8]);
//output-> Invalid Argument. Only two values for CellSize.

values = extractHOGFeatures(I3,"CellSize",[8,8],"BlockSize",[16,8],"BlockOverlap",[8,8],"NumBins",9);
//-> No error 

values = extractHOGFeatures(I3,"CellSize",[8,8],"BlockSize",[32,8],"BlockOverlap",[8,8],"NumBins",9);
//output-> BlockSize values cannot be more than 16.

values = extractHOGFeatures(I3,"CellSize",[8,32],"BlockSize",[32,8],"BlockOverlap",[8,8],"NumBins",9);
//output-> CellSize values cannot be more than 16.

values = extractHOGFeatures(I3,"CellSize",[8,4],"BlockSize",[8,8],"BlockOverlap",[18,8],"NumBins",9);
//output-> BlockOverlap values cannot be more than 16.

values = extractHOGFeatures(I3,"CellSize",[16,4],"BlockSize",[8,8],"BlockOverlap",[16,8],"NumBins",9);
//output-> CellSize values cannot be more than BlockSize values.

values = extractHOGFeatures(I3,"CellSize",[4,4],"BlockSize",[8,8],"BlockOverlap",[16,8],"NumBins",9);
//output-> BlockOverlap values cannot be more than BlockSize values. 

values = extractHOGFeatures(I3,"CellSize",[4,4],"BlockSize",[8,8],"NumBins",9);
//-> No error

values = extractHOGFeatures(I3,"BlockSize",[8,8],"NumBins",9);
//-> No error

values = extractHOGFeatures(I3,"BlockSize",[8,8],"CellSize",[8,8],"NumBins",9);  
//-> No error when shifting name-value pair arguments randomly

values = extractHOGFeatures(I3,"BlockSize",[8,],"CellSize",[8,8],"NumBins",9);
//output->Invalid Argument.Only two values for BlockSize.
