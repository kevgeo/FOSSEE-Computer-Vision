I1 = imread("left.jpg");
 
I2 = imread("right.jpg");

I3 = epipolarlines(I1,I2);
