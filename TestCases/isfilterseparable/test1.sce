//Filter/Kernel
kernel1 = [1 2 3; 4 5 6; 7 8 9];
//Executing function
isfilterseparable("filter",kernel1)
//output-> 0 which means it is not separable

//Sobel kernel
sobel = [-1 0 1; -2 0 2; -1 0 1];
//Executing function
isfilterseparable("filter",sobel)
//output-> 1 which means it is separable

kernel2 = [1 0 1; -1 -2 0; 0 1 -1];
//Executing function
isfilterseparable("filter",kernel2)
//output-> 1 which means it is separable

kernel3 = [2 0 2; 0 1 0; 0 0 0]
//Executing function
isfilterseparable("filter",kernel3)
//output-> 1 which means it is separable


sobel2 = [1 0 -1; 2 0 -2; 1 0 -1]
//Executing function
isfilterseparable("filter",sobel2)
//output-> 1 which means it is separable

