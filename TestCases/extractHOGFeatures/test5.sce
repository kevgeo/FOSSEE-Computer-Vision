
//Reading grayscale image
I1 = imread("left.jpg");
//Extracting HOG feature values from image
values = extractHOGFeatures(I1,"CellSize",[8,8],"BlockSize",[32,8],"BlockOverlap",[8,8],"NumBins",9);
//output-> BlockSize values cannot be more than 16.

//Reading image
I2 = imread("lena.jpeg");
//Extracting HOG feature values from image
values = extractHOGFeatures(I2,"CellSize",[8,8],"BlockSize",[32,8],"BlockOverlap",[8,8],"NumBins",9);
//output-> BlockSize values cannot be more than 16.

//Reading image
I3 = imread("clc1.jpg");
//Extracting HOG feature values from image
values = extractHOGFeatures(I3,"CellSize",[8,8],"BlockSize",[32,8],"BlockOverlap",[8,8],"NumBins",9);
//output-> BlockSize values cannot be more than 16.

//Reading image
I4 = imread("board1.jpg");
//Extra	cting HOG feature values from image
values = extractHOGFeatures(I4,"CellSize",[8,8],"BlockSize",[32,8],"BlockOverlap",[8,8],"NumBins",9);
//output-> BlockSize values cannot be more than 16.
