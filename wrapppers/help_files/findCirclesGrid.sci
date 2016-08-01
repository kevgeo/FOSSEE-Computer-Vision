function [output1 output2] = findCirclesGrid(image1,width,height,flag)
	img1 = mattolist(image1);
	[output1 output2] = opencv_findCirclesGrid(image1,width,height,flag)
endfunction

