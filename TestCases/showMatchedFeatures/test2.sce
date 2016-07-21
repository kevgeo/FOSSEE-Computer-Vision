
//Reading first image
I1 = imread("left.jpg");
//Reading second image
I2 = imread("right.jpg");

//Executing function 
//Checking if error message comes up when "I" variable hasn't been defined
out1 = showMatchedFeatures(I2,I); 
//output->Undefined variable: I

//Reading first image
I3 = imread("left01.jpg");
//Reading second image
I4 = imread("right01.jpg");

//Executing function 
//Checking if error message comes up when "I" variable hasn't been defined
out2 = showMatchedFeatures(I4,I); 
//output->Undefined variable: I


//Reading first image
I6 = imread("left12.jpg");
//Reading second image
I7 = imread("right12.jpg");

//Executing function 
//Checking if error message comes up when "I" variable hasn't been defined
out3 = showMatchedFeatures(I6,I); 
//output->Undefined variable: I


//Reading first image
I8 = imread("board1.jpg");
//Reading second image
I9 = imread("board2.jpg");

//Executing function 
//Checking if error message comes up when "I" variable hasn't been defined
out4 = showMatchedFeatures(I9,I); 
//output->Undefined variable: I


