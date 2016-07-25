//Checking if error message pops up when input is 2D point set instead of 3D i.e Nx2 instead of Nx3
in2 = [ 25 40  ;
12 33 ;
1 11 ;
23 44 ;
56 67 ;
25 54 ;
20 36 ;
24 16 ];

out2 = convertPointsFromHomogeneous(in2);
//output->
//                                         !--error 999 
//Please enter 3D points matrix of size Nx3.


//Checking if error message pops up when input is 2D point set instead of 3D i.e Nx2 instead of Nx3
in = [ -75 -90 ;
-120 1 ];

output = convertPointsFromHomogeneous(in);
//output-> 
//  !--error 999 
//Please enter 3D points matrix of size Nx3.

//Checking if error message pops up when input is 2D point set instead of 3D i.e Nx2 instead of Nx3
in3 = [  10 20;
12 15 ;
10 38 ;
18 35 ; 
3 4 ];

out3 = convertPointsFromHomogeneous(in3);
//output-> 
//  !--error 999 
//Please enter 3D points matrix of size Nx3.
