function [cam rot trans rotX rotY rotZ euler] = decomposeProjectionMatrix(input)

	[cam rot trans rotX rotY rotZ euler] = opencv_decomposeProjectionMatrix(input)

endfunction
