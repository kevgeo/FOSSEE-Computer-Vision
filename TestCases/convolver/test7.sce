I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"scalar",1,"size",9,"values",a);
//-> output is wrong, need to correct error for this test case
