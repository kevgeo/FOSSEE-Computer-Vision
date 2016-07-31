// Extracts HOG feature values from image
//
// Calling Sequence
// values = extractHOGFeatures(Image);
// values = extractHOGFeatures(Image,"CellSize",<value>,"BlockSize",<value>,"BlockOverlap",<value>,"NumBins",<value>);
// 
// Parameters 
// Image : Input image matrix
// double CellSize : Determines the cell size in pixels.
// double BlockSize : Determines the block size in pixels.
// double BlockOverlap : Specifies the amount of block overlap. It must be a multiple of CellSize.
// int NumBins : Specifies the number of bins.
//
// Description
// The function extracts the HOG feature values from an image and stores them in a vector.
//
// Examples
// //Reading image
// I1 = imread("lena.jpeg");
// //Extracting HOG feature values from image
// values = extractHOGFeatures(I1);
//
// Author
// Kevin George
// 
