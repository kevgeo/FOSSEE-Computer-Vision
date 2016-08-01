function [output1 output2] = epipolarlines(image1,image2)
	img1 = mattolist(image1);
	img2 = mattolist(image2);
	[a b] = opencv_epipolarlines(img1,img2)
	d = size(a);
	for i = 1:d
		output1(:,:,i) = a(i);
	end
	e = size(b);
	for i = 1:e
		output2(:,:,i) = b(i);
	end
endfunction

