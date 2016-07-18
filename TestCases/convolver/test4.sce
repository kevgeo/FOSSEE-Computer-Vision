I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"size",8,"scalar",1,"values",a); // a is size of 9 elements
//output-> Invalid size entered
