
//Reading grayscale image
I1 = imread("box.png");

//Kernel or filter and convolution on image is done with this kernel
//This kernel will give same image with no change
a = [0 0 0; 0 1 0; 0 0 0];

//Checking if error message comes if we don't add "values" string in front of 'a' variable
//Perforing convolution
out1 = convolver(I1,"scalar",1,"size",3,a);
//output->in getMatrixOfString: Invalid argument type, string matrix expected


//Reading image
I2 = imread("lena.jpeg");

//Kernel or filter and convolution on image is done with this kernel
//This kernel will give same image with no change
a = [0 0 0; 0 1 0; 0 0 0];

//Checking if error message comes if we don't add "values" string in front of 'a' variable
//Perforing convolution
out2 = convolver(I2,"scalar",1,"size",3,a);
//output->in getMatrixOfString: Invalid argument type, string matrix expected


//Reading image
I3 = imread("clc1.jpg");

//Kernel or filter and convolution on image is done with this kernel
//This kernel will give same image with no change
a = [0 0 0; 0 1 0; 0 0 0];

//Checking if error message comes if we don't add "values" string in front of 'a' variable
//Perforing convolution
out3 = convolver(I3,"scalar",1,"size",3,a);
//output->in getMatrixOfString: Invalid argument type, string matrix expected


//Reading grayscale image
I4 = imread("left01.jpg");

//Kernel or filter and convolution on image is done with this kernel
//This kernel will give same image with no change
a = [0 0 0; 0 1 0; 0 0 0];

//Checking if error message comes if we don't add "values" string in front of 'a' variable
//Perforing convolution
out4 = convolver(I4,"scalar",1,"size",3,a);
//output->in getMatrixOfString: Invalid argument type, string matrix expected


