inpt =[ 1 0 5 1
0 1 4 2;
0 0 1 3];

[cam rot trans rotX rotY rotZ euler] = decomposeProjMatrix(inpt);

//output is correct

