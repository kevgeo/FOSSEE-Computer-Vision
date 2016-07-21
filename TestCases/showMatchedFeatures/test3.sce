 
//Reading first image
I1 = imread("left.jpg");
//Reading second image
I2 = imread("right.jpg");

//Executing function 
//Checking if error message pops up when only one image is given
out1 = showMatchedFeatures(I2);
//output->showMatchedFeatures: Wrong number of input argument(s): 2 expected.


//Reading first image
I3 = imread("left01.jpg");
//Reading second image
I4 = imread("right01.jpg");

//Executing function
//Checking if error message pops up when only one image is given 
out2 = showMatchedFeatures(I3);
//output->showMatchedFeatures: Wrong number of input argument(s): 2 expected.

//Reading first image
I6 = imread("left12.jpg");
//Reading second image
I7 = imread("right12.jpg");

//Executing function 
//Checking if error message pops up when only one image is given 
out3 = showMatchedFeatures(I7);
//output->showMatchedFeatures: Wrong number of input argument(s): 2 expected.

//Reading first image
I8 = imread("board1.jpg");
//Reading second image
I9 = imread("board2.jpg");

//Executing function 
//Checking if error message pops up when only one image is given 
out3 = showMatchedFeatures(I8);
//output->showMatchedFeatures: Wrong number of input argument(s): 2 expected.

