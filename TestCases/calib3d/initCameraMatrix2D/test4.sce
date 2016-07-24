//Checking if function works for different values
a = [14 11.0;
217.0 83.4;
22.0 33.3;
211.0 321];

in1 = list(a)


b = [1.0 21.0 15;
25.0 13.0 23;
223.0 26.0 2;
179.0 52.0 13];

in2 = list(b)

[output1] = initCameraMatrix2D(1,in2,in1,1280,1024,0);

//output is correct
