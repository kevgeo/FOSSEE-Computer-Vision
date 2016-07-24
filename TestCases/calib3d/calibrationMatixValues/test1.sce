//Checking if function works
camera = [2 33 12
3 5 6
1 56 7];

width = 100;

height = 200;

apertureWidth = 1;

apertureHeight = 1;

[fovx fovy focal ppoint apectRatio ] = calibrationMatrixValues(camera,width,height,apertureWidth,apertureHeight);

//output is correct
