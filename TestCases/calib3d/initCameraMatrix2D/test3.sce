//Checking if function works for different values
a = [154 111.0;
117.0 81;
22.0 53.0;
231.0 321.0];

in1 = list(a)


b = [18.0 28.0 11;
25.0 113.0 0;
262.0 126.0 22;
239.0 32.0 13];

in2 = list(b)

[output1] = initCameraMatrix2D(1,in2,in1,1280,1024,0);

//output is correct
