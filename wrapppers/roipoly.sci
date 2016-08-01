function[dstImg] = roipoly(srcImg, col_list, row_list)
	
	out = opencv_roipoly(srcImg, col_list, row_list)
	
	channels = size(out)
	
	for i = 1:channels
		dstImg(:,:,i) = out(i)
	end
	
endfunction
