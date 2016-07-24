//Checking if function works in general

a = [144.00 1011.0;
237.0 801.0;
242.0 583.0;
271.0 421.0];

in1 = list(a)


b = [18.0 18.0 0;
25.0 110.0 0;
26.0 226.0 0;
29.0 327.0 0];

in2 = list(b)

cameraMatrix = initCameraMatrix2D(1,in2,in1,1280,1024,0);
	
dist = [0
0
0
0];

new_cameraMatrix = getOptimalNewCameraMatrix(cameraMatrix,dist,1280,1024,0.3,1280,1024,0);

//output is correct
