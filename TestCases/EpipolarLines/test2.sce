//Reading first image
I1 = imread("left.jpg");
//Reading second image 
I2 = imread("right.jpg");
//Drawing epipolar lines
//Checking if error message comes up when "I" variable hasn't been defined
[out1 out2] = epipolarlines(I2,I); 
//output->Undefined variable: I

//Reading first image
I3 = imread("1.jpg");
//Reading second image 
I4 = imread("2.jpg");
//Drawing epipolar lines
//Checking if error message comes up when "I" variable hasn't been defined
[out3 out4] = epipolarlines(I3,I); 
//output->Undefined variable: I

//Reading first image
I5 = imread("left07.jpg");
//Reading second image 
I6 = imread("right07.jpg");
//Drawing epipolar lines
//Checking if error message comes up when "I" variable hasn't been defined
[out5 out6] = epipolarlines(I6,I); 
//output->Undefined variable: I

//Reading first image
I7 = imread("deskLeft.png");
//Reading second image 
I8 = imread("deskRight.png");
//Drawing epipolar lines
//Checking if error message comes up when "I" variable hasn't been defined
[out7 out8] = epipolarlines(I8,I); 
//output->Undefined variable: I


