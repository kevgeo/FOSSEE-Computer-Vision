first = [ 0 0 4 2 7 8;  
0 3 3 2 1 0;
1 1 1 2 2 2 ]

second = [ 0 0 4 2 7 8;
0 3 3 2 1 0;
3 3 3 4 4 4]

[out1 ou2t] =estimateAffine3D(first,second,3,2)

//output->
//                                                !--error 999 
//confidence value should be between 0 and 1.
 
