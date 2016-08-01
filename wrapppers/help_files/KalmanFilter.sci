function [output] = KalmanFilter(varargin)
	[lhs,rhs] = argn(0)
	if rhs>10
		error(msprintf(" Too many input arguments"));
	end
	select rhs
	case 0 then
		a = opencv_KalmanFilter();
	case 2 then
		a = opencv_KalmanFilter(varargin(0),varargin(1));
	case 4 then
		a = opencv_KalmanFilter(varargin(0),varargin(1),varargin(2),varargin(3));
	case 6 then
		a = opencv_KalmanFilter(varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5));
	case 8 then
		a = opencv_KalmanFilter(varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7));
	case 10 then
		a = opencv_KalmanFilter(varargin(0),varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8),varargin(9));
	end

	d = size(a);
	for i = 1:d
		output(:,:,i) = a(i);
	end
endfunction
