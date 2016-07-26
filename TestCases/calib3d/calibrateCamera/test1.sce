obpts = [ .5 .5 -.5;
	.5 .5 .5;
	-.5 .5 .5;
	-.5 .5 -.5;
	.5 -.5 -.5;
	-.5 -.5 -.5;
	-.5 -.5 .5];



impts = [282 274;
	397 227;
	577 271;
	462 318;
	270 479;
	450 523;
	566 475];

camera1 = [1 0 0
0 1 0
0 0 1];

dist = [0 
0
0
0];

point_counts = 


[rvecs tvecs] = calibrateCamera(1,obpts,impts,1280,1024,camera1,dist)
