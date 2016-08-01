function [found coordinates] = findChessboardCorners(image1,pts_row,pts_col,flags)
	img1 = mattolist(image1);
	[found coordinates] = opencv_findChessboardCorners(img1,pts_row,pts_col,flags);
	
endfunction

