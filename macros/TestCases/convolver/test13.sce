I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"values","size","scalar");
//output->in getMatrixOfDouble: Invalid argument type, double matrix expected
