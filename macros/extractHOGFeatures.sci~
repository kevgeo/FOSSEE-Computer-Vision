function [featureVector] = extractHOGFeatuers(I, varargin)
	img = mattolist(I);
	[lhs rhs] = argn(0);
	if rhs<1 then
		error(msprintf("Not enough input arguments"));
	end
	if rhs>9 then
		error(msprintf("Too many input arguments"));
	end	
	l = rhs-1;
	if(l==0) then
		featureVector = opencv_extractHOGFeatures(img);
	elseif (l==2) then
		featureVector = opencv_extractHOGFeatures(img,varargin(0),varargin(1));
	elseif (l==4) then
		featureVector = opencv_extractHOGFeatures(img,varargin(0),varargin(1),varargin(2),varargin(3));
	elseif (l==6) then
		featureVector = opencv_extractHOGFeatures(img,varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5));
	else
		featureVector = opencv_extractHOGFeatures(img,varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7));
	end
		
endfunction
