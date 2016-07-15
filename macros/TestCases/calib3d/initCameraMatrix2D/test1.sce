a = [ 144.00 237.00 242.00 271.00;
1011.0 801.0 583.0 421.0];

in1 = list(a)

b = [ 18.0 25.0 26.0 29.0;
18.0 110.0 226.0 327.0;
0 0 0 0];

in2 = list(b)

[output1 output2] = initCameraMatrix2D(1,in2,in1,1280,1024,0);

//output is correct
