function [rvecs tvecs] = calibrateCamera(list_size,obPts,impts,width,cols,camera1,dist)

	[rvecs tvecs] = opencv_calibrateCamera(list_size,obPts,impts,width,cols,camera1,dist)

endfunction
