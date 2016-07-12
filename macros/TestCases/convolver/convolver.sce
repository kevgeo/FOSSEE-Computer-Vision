**************************************************************************
//output-> shows the error on console
//-> means that output is correct and no modification needs to be done
**************************************************************************

I1 = imread("box.png");
a = [0 0 0; 0 1 0; 0 0 0];

//convolver(I1,"size",9,"values",a,"scalar",1)
//-> Returns an image

//I2 = convolver(I1,"size",9,"values",a,"scalar",0);
//output-> Invalid scalar value. Please enter a non negative Double value.

//I2 = convolver(I1,"size",9,"values",a,"scalar",);
//output-> in getScalarDouble: Unable to get argument #7

//I2 = convolver(I1,"size",8,"scalar",1,"values",a); // a is size of 9 elements
//output-> Invalid size entered

//I2 = convolver(I1,"values",a,"size",9,"scalar",1);
//-> Correct output is given

//I2 = convolver(I1,"values",a,"ize",9,"sclar",1);
//output-> Wrong name-value argument pair has been entered by user

//I2 = convolver(I1,"scalar",1,"size",9,"values",a);
//-> output is wrong, need to correct error for this test case

//I2 = convolver(I1,"scalar",1,"values",a,"size",9);
//-> output is correct

//I2 = convolver(I1,"scalar",1,"size",9,"size",a);
//output->Please provide optional arguments only once

//I2 = convolver(I1,"scalar",1,"size",9,"scalar",a);
//output->Please provide optional arguments only once

//I2 = convolver(I1,"scalar",1,"size",9,a);
//output->in getMatrixOfString: Invalid argument type, string matrix expected

//I2 = convolver(I1,"scalar",1,,9,a);
//output->in getMatrixOfString: Invalid argument type, string matrix expected
 
//I2 = convolver(I1,"values","size","scalar");
//output->in getMatrixOfDouble: Invalid argument type, double matrix expected
 
//I2 = convolver(I1,"values",a,"size","scalar");
//output->in getScalarDouble: Unable to get argument #5
//        in getMatrixOfDouble: Invalid argument type, double matrix expected


