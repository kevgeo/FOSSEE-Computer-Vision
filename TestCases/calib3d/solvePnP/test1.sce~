obpts = [ .5 .5 -.5 .5 .5 -.5 -.5;
.5 .5 .5 .5 -.5 -.5 -.5;
-.5 .5 .5 -.5 -.5 -.5 .5]

impts = [ 282 397 577 462 270 450 566
274 227 276 378 479 523 476]

camera = [ 1 0 0;
0 1 0;
0 0 1]

dist = [0
0
0
0]

[out1 out2] = solvePnP(obpts,impts,camera,dist,1,"CV_ITERATIVE");

//output is correct
