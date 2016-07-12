I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"scalar",1,,9,a);
//output->in getMatrixOfString: Invalid argument type, string matrix expected
