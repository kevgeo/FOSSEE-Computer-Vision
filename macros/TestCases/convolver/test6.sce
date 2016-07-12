I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

I2 = convolver(I1,"values",a,"ize",9,"sclar",1);
//output-> Wrong name-value argument pair has been entered by user
