function [output] = drawChessboardCorners(inImage,pts_row,pts_col,coordinates,found)
	img1 = mattolist(inImage);
	img = opencv_drawChessboardCorners(img1,pts_row,pts_col,coordinates,found);
	d = size(img);
	for i = 1:d
		output(:,:,i) = img(i);
	end
endfunction
