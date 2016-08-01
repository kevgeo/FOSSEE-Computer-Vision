function [rvec tvec] = solvePnP(obpts,impts,camera,dist,useExtrinsicGuess,flags)

	[rvec tvec] = opencv_solvePnP(obpts,impts,camera,dist,useExtrinsicGuess,flags)

endfunction
