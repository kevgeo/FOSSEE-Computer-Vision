function [triMatrix orthoMatrix RotMatrixX RotMatrixY RotMatrixZ] = RQDecomp3x3(matrix)

	[triMatrix orthoMatrix RotMatrixX RotMatrixY RotMatrixZ] = opencv_RQDecomp3x3(matrix)

endfunction
