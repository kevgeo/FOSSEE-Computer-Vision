//Rotation matrix values
src = [3 4 5;
2 3 1;
3 4 5];
//src which is a rotation matrix is converted to a rotation vector "dst"
//optional output jacobian which is a matrix of partial derivatives of output array
[dst jacobian ] = Rodrigues(src);
//output is correct


//Rotation vector values
rotvec = [12;
5;
7];
//rotvec which is a rotation vector is converted to a rotation matrix "dst2"
//optional output jacobian2 which is a matrix of partial derivatives of output array
[dst2 jacobian2 ] = Rodrigues(rotvec);
//output is correct

//matrix values
matrix = [12 20;
5 20;
7 12];
//Checking if error message pops up as the function only accepts 3x3 matrices or 1x3 and 3x1 vectors
[dst3 jacobian3 ] = Rodrigues(matrix);
//output->
//                                      !--error 999 
//Invalid Argument. 3x3 matrix, 1x3 and 3x1 vectors are only allowed.

//Projection matrix
inpt =[ 1 0 5 1
0 1 4 2;
0 0 1 3];
//Using decomposeProjection matrix to give rotation matrix or vector
[cam rot trans rotX rotY rotZ euler] = decomposeProjectionMatrix(inpt);

[dst4 jacobian4] = Rodrigues(rot);
