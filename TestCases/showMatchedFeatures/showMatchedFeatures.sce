//**************************************************************************
//output-> shows the error on console
//-> means that output is correct and no modification needs to be done
//**************************************************************************

I1 = imread("left.jpg");
I2 = imread("right.jpg");

//I3 = showMatchedFeatures(I1,I2);
//-> output is correct

//I3 = showMatchedFeatures(I2,I); //I variable hasn't been defined
//output->Undefined variable: I

//I3 = showMatchedFeatures(I2);
//output->showMatchedFeatures: Wrong number of input argument(s): 2 expected.
 
//I3 = showMatchedFeatures(I2,3);
//output-> API Error:
//	in getListItemAddress: Unable to get address of item #2 in argument #2
//	in getListItemNumber: Invalid argument type, list expected

//I3 = showMatchedFeatures();
//output-> showMatchedFeatures: Wrong number of input argument(s): 2 expected.

	
