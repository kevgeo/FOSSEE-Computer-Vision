I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"scalar",1,"size",9,"scalar",a);
//output->Please provide optional arguments only once
