//Checking if error message pops up when input is 3x3 instead of 3x4
inpt =[ 3 0 5 
1 3 14 ;
10 20 1 ];

[cam rot trans rotX rotY rotZ euler] = decomposeProjectionMatrix(inpt);

//output->
//                                    !--error 999 
//Projection matrix should be 3x4.
 

