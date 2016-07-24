//Checking if error message pops up when camera matrix size is 3x4 instead of 3x3
cameraMatrix = [ 12 23 11 15;
34 11 45 22;
12 14 67 23;]
	
dist = [0
0
0
0
0
0];

new_cameraMatrix = getOptimalNewCameraMatrix(cameraMatrix,dist,1280,1024,0.3,1280,1024,0);

//output->
//                                                                                          !--error 999 
//Please make sure that camera matrix is 3x3.
