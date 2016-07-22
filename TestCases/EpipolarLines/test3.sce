//Reading first image
I1 = imread("left.jpg");
//Reading second image 
I2 = imread("right.jpg");
//Drawing epipolar lines
//Checking if error message pops up when only one image is given
[out1 out2] = epipolarlines(I2);
//output->
//                    !--error 77 
//epipolarlines: Wrong number of input argument(s): 2 expected.
 

//Reading first image
I3 = imread("1.jpg");
//Reading second image 
I4 = imread("2.jpg");
//Drawing epipolar lines
//Checking if error message pops up when only one image is given
[out3 out4] = epipolarlines(I4);
//output->
//                    !--error 77 
//epipolarlines: Wrong number of input argument(s): 2 expected.
 

//Reading first image
I5 = imread("left07.jpg");
//Reading second image 
I6 = imread("right07.jpg");
//Drawing epipolar lines
//Checking if error message pops up when only one image is given
[out5 out6] = epipolarlines(I5);
//output->
//                    !--error 77 
//epipolarlines: Wrong number of input argument(s): 2 expected.
 



//Reading first image
I7 = imread("deskLeft.png");
//Reading second image 
I8 = imread("deskRight.png");
//Drawing epipolar lines
//Checking if error message pops up when only one image is given
[out7 out8] = epipolarlines(I7);
//output->
//                    !--error 77 
//epipolarlines: Wrong number of input argument(s): 2 expected.
 


