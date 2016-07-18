//**************************************************************************
//"output->" shows the error on console
//"->" means that output is correct and no modification needs to be done
//**************************************************************************

a = [1 2 3; 4 5 6; 7 8 9];
//isfilterseparable("data",a,"Size",3);
//output-> Invalid size given

//isfilterseparable("data",a,"Size",8);
//output-> Invalid size given

//isfilterseparable("Size",9,"data",a)
//-> statement is accepted even when name value pair arguments are reversed

//isfilterseparable("Sze",9,"data",a)
//output-> Wrong name-value argument pair has been entered by user

//isfilterseparable("Size",9,"Size",a)
//output-> Please provide optional arguments only once

//isfilterseparable("Size",9,"data",8)
//output-> Invalid Argument. Data must be entered as matrix of values.

//isfilterseparable("Size",a,"data",a)
//output-> in getScalarDouble: Wrong type for input argument #2: A scalar expected.

//isfilterseparable("Size",9,"dta",a)
//output-> Wrong name-value argument pair has been entered by user 


