// Perform convolution on image with kernel
//
// Calling Sequence
// outputImg = convolver(Image,"size",<value>,"values",<value>,"scalar",<value>);
// 
// Parameters 
// Image : Input image matrix
// int size :  This is basically the dimension of the kernel. Since only square matrix kernel is supported, size is either column or row size of kernel.
// array(float) values : Kernel or filter
// int scalar : The value which is multiplied to the whole kernel
//
// Description
// The function extracts the HOG feature values from an image and stores them in a vector.
//
// Examples
// Reading image
// I1 = imread("lena.jpeg");
// //This kernel will give same image with no change
// a = [0 0 0; 0 1 0; 0 0 0];
// //convolution on image is done with this kernel
// outImg = convolver(I1,"size",3,"values",a,"scalar",1);
// //-> Returns an image
//
// Author
// Kevin George
// 
