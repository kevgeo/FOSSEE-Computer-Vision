function opticalFlowFarneback(varargin)
	[lhs rhs] = argn(0);
	if(rhs>12)
		error(msprintf("Too many input arguments"));
	end
	if(rhs==0) then
		opencv_opticalFlowFarneback();
	elseif (rhs==2) then
		opencv_opticalFlowFarneback(varargin(0),varargin(1));
	elseif(rhs==4) then
		opencv_opticalFlowFarneback(varargin(0),varargin(1),varargin(2),varargin(3));
	elseif(rhs==6) then
		opencv_opticalFlowFarneback(varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5));
	elseif(rhs==8) then
		opencv_opticalFlowFarneback(varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7));
	elseif(rhs==10) then
		opencv_opticalFlowFarneback(varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8),varargin(9));
	elseif(rhs==12) then
		opencv_opticalFlowFarneback(varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8),varargin(9),varargin(10),varargin(11));
	end
endfunction
