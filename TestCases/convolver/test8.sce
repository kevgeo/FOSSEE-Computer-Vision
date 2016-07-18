I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"scalar",1,"values",a,"size",9);
//-> output is correct
