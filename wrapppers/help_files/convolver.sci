function [output] = convolver(image1,ksize,values1,scalar)
	image = mattolist(image1)
	a = opencv_convolver(image,ksize,values1,scalar);
	d = size(a);
	for i = 1:d
		output(:,:,i) = a(i);
	end
endfunction
