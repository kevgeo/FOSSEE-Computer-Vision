first = [ 0 0 1;
0 3 1;
4 3 1;
2 2 2;
7 1 2;
8 0 2];

second = [ 0 0 3;
0 3 3;
4 3 3;
2 2 4;
7 1 4;
8 0 4];

[out1 ou2t] =estimateAffine3D(first,second,3,2);

//output->
//                                                !--error 999 
//confidence value should be between 0 and 1.
 
