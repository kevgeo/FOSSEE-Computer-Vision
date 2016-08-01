function [output] = showMatchedFeatuers(image1,image2)
	img1 = mattolist(image1);
	img2 = mattolist(image2);
	a = opencv_showMatchedFeatures(img1,img2)
	d = size(a);
	for i = 1:d
		output(:,:,i) = a(i);
	end
endfunction

