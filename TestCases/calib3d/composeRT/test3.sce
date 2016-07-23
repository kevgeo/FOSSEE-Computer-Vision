//Checking if error message pops up when row vector size is not 3

TransVec1 = [4
2
3
4];

RotVec1 = [2.3
4.5
7.8
3.2];

RotVec2 = [ 2.1
4
5
1.2 ];

TransVec2 = [2  
3
1
13];

[RotVec3 TransVec3] = composeRT(RotVec1,TransVec1,RotVec2,TransVec2);

//output->
//                                                                   !--error 999 
//Only row vector of size 3x1 is allowed. Make sure that the inputs are of this size.
 
