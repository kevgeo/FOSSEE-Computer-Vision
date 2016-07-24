//Checking if error message pops up when camera matrix entered is 3x4 instead of 3x3 
camera = [2 33 12 23
3 5 6 32
1 56 7 14];

width = 100;

height = 200;

apertureWidth = 1;

apertureHeight = 1;

[fovx fovy focal ppoint apectRatio ] = calibrationMatrixValues(camera,width,height,apertureWidth,apertureHeight);

//output->
//                                                                                                                !--error 999 
//Camera matrix should be 3x3.
