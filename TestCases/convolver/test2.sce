I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"size",9,"values",a,"scalar",0);
//output-> Invalid scalar value. Please enter a non negative Double value.
