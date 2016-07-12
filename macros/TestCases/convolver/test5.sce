I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"values",a,"size",9,"scalar",1);
//-> Correct output is given

