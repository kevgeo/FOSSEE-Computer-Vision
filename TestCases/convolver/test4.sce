
//Reading grayscale image
I1 = imread("box.png");

//Kernel or filter and convolution on image is done with this kernel
//This kernel will give same image with no change
a = [0 0 0; 0 1 0; 0 0 0];

//Perforing convolution
//Checking if it accept size 2
//Basically the total number of values kernel contains is size*size
//So if kernel has 9 values, then size should be sqrt(9) which is 3
out1 = convolver(I1,"size",2,"scalar",1,"values",a); 
//output-> Invalid size entered

//Reading image
I2 = imread("lena.jpeg");

//Kernel or filter and convolution on image is done with this kernel
//This kernel will give same image with no change
a = [0 0 0; 0 1 0; 0 0 0];

//Perforing convolution
//Checking if it accept size 2
//Basically the total number of values kernel contains is size*size
//So if kernel has 9 values, then size should be sqrt(9) which is 3
out2 = convolver(I2,"size",2,"scalar",1,"values",a); 
//output-> Invalid size entered



//Reading image
I3 = imread("clc1.jpg");

//Kernel or filter and convolution on image is done with this kernel
//This kernel will give same image with no change
a = [0 0 0; 0 1 0; 0 0 0];

//Perforing convolution
//Checking if it accept size 2
//Basically the total number of values kernel contains is size*size
//So if kernel has 9 values, then size should be sqrt(9) which is 3
out3 = convolver(I3,"size",2,"scalar",1,"values",a); 
//output-> Invalid size entered



//Reading grayscale image
I4 = imread("left01.jpg");

//Kernel or filter and convolution on image is done with this kernel
//This kernel will give same image with no change
a = [0 0 0; 0 1 0; 0 0 0];

//Perforing convolution
//Checking if it accept size 2
//Basically the total number of values kernel contains is size*size
//So if kernel has 9 values, then size should be sqrt(9) which is 3
out4 = convolver(I4,"size",2,"scalar",1,"values",a); 
//output-> Invalid size entered




