I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"values",a,"size","scalar");
//output->in getScalarDouble: Unable to get argument #5
//        in getMatrixOfDouble: Invalid argument type, double matrix expected

