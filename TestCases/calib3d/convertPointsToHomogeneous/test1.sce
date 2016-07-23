//N dimensional points
in = [30 40 20 10];
//Getting homogenous points
out = convertPointsToHomogeneous(in);
//output is correct

//N dimensional points
in2 = [20 12 11 40 34 14];
//Getting homogenous points
out2 = convertPointsToHomogeneous(in2);
//output is correct

//N dimensional points
in3 = [33 13 45 22 12];
//Getting homogenous points
out3 = convertPointsToHomogeneous(in3);
//output is correct

//N dimensional points
//Checking if it works when column vector is entered instead of row
in4 = [12; 13; 5; 22; 2];
//Getting homogenous points
out4 = convertPointsToHomogeneous(in4);
//output is correct

//N dimensional points
//Checking if it works when matrix instead of vector is entered
in5 = [12 13; 5 20];
//Getting homogenous points
out5 = convertPointsToHomogeneous(in5);
//output->
//                                     !--error 999 
//Please enter either row or column vector.
