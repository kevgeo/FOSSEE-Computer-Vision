//Checking if error message pops up when objectPoints matrix is a 2D point set instead 0f a 3D point set
//2D as in x & y coordinate values, 3D as in x,y & z coordinate values 

a = [18.0 18.0 0;
25.0 110.0 0;
26.0 226.0 0;
29.0 327.0 0];

imagePoints = list(a);

b = [144.00 1011.0;
237.0 801.0;
242.0 583.0;
271.0 421.0];


objectPoints = list(b);

[output1] = initCameraMatrix2D(1,objectPoints,imagePoints,1280,1024,0);

//output->
//      !--error 999 
//Please enter an objectPoints matrix which is of N x 3 dimension.
 
