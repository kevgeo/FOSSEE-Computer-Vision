//Checking if function works with different values
camera = [5 3.2 1.2
3.3 5 6.4
11 6.5 2];

width = 1020;

height = 800;

apertureWidth = 10;

apertureHeight = 11;

[fovx fovy focal ppoint apectRatio ] = calibrationMatrixValues(camera,width,height,apertureWidth,apertureHeight);

//output is correct
