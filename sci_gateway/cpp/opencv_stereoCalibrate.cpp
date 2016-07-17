//*******************************************************************************************************
// Authors : Kevin George
//
// filterSpeckles(inImage, newval, maxSpeckleSize, maxDiff, outImage );
//               
//*******************************************************************************************************

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include <sciprint.h>
  #include "../common.h"
  
  int opencv_stereoCalibrate(char *fname, unsigned long fname_len)
  {
	  	SciErr sciErr;
	    int intErr = 0;
	    int iRows=0,iCols=0;
	    int *piAddr = NULL;
	    int *piAddr2  = NULL;
	    int *piAddr3  = NULL;
	    int *piAddr4  = NULL;
	    int *piAddr5  = NULL;
	    int *piAddr6  = NULL;
	    int *piAddr7  = NULL;
	    int *piAddr8  = NULL;
	    int *piAddr9  = NULL;
	    int *piAddr10  = NULL;
	     int *piAddrChild = NULL;
	    int iPrec = 0,iItem = 0;
	    int i,j,k ;
	    //checking input argument
	    CheckInputArgument(pvApiCtx, 10, 10);
	    CheckOutputArgument(pvApiCtx, 1, 4) ;

	    //-> Input
	    double *objectPoints = NULL;
	    double *imagePoints = NULL;
	    double *imagePoints2 = NULL;
	    double *cameramatrix1 = NULL;
	    double *distcoeffs1 = NULL;
	    double *cameramatrix2 = NULL;
	    double *distcoeffs2 = NULL;
	    double list_size;
	    double term_crit_count,term_crit_eps; // term criteria paramerters
	    double width,height; //For image size
	    double aspectRatio;
	    int size1_r,size1_c;
	    int size2_r,size2_c;
	    
	    vector<vector<Point3f> > obPts;
	    vector<vector<Point2f> > imPts;
	    vector<vector<Point2f> > imPts2;
	    int Rows,Cols;

	    //-> Output
	    Mat rotationMatrix;
  		Mat translationVector;
 		Mat essentialMatrix;
  		Mat fundamentalMatrix;
	    
	    //-> Get list size 
	    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0); 
	        return 0; 
	    }

	    intErr = getScalarDouble(pvApiCtx, piAddr, &list_size);
	    if(intErr)
	    {
	       return intErr;
	    }

	    for(int idx=1; idx<=list_size; idx++)
	    {
	        //-> Get calibration pattern points 
	        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
	        if (sciErr.iErr)
	        {
	            printError(&sciErr, 0); 
	            return 0; 
	        }

	        sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr2, idx, &iRows, &iCols ,&objectPoints);
	        if(sciErr.iErr)
	        {
	            printError(&sciErr, 0);
	            return 0;
	        }
	        int size = (iRows*iCols)/3;
	        Rows = iRows;
	        Cols = iCols;
	        vector<Point3f> values2(size);

	        //j = 0;
	        for(int i=0; i<Rows; i++)
	        {
	            for(int j=0; j<Cols; j++)
	            {
		            if(j==0)
		            	values2[i].x = objectPoints[i+j*Rows];
		            
		            else if(j==1)
		            	values2[i].y = objectPoints[i+j*Rows];
		            else
		            	values2[i].z = objectPoints[i+j*Rows];
	        	}
	        }

	        obPts.push_back(values2);
	    }

	  for(int idx=1; idx<=list_size; idx++)
      {
        //-> Get projections of calibration pattern points
        //   observed by first camera 
        sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr3, idx, &iRows, &iCols ,&imagePoints);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        int size = (iRows*iCols)/2;
        vector<Point2f> values1(size);

        j = 0;
        for(int i=0; i<size; i++)
        {
            values1[i].x = imagePoints[j++];
            values1[i].y = imagePoints[j++];
        }

        imPts.push_back(values1);
     }


	for(int idx=1; idx<=list_size; idx++)
      {
        //-> Get projections of calibration pattern points
        //   observed by second camera 
        sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr4, idx, &iRows, &iCols ,&imagePoints2);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        int size = (iRows*iCols)/2;
        vector<Point2f> values3(size);

        j = 0;
        for(int i=0; i<size; i++)
        {
            values3[i].x = imagePoints2[j++];
            values3[i].y = imagePoints2[j++];
        }

        imPts2.push_back(values3);
     }

    //-> Get first camera matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &iRows, &iCols, &cameramatrix1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    //-> Need to store the camera matrix 
    //   in a Mat object
    Mat cameraMatrix1(3,3,DataType<double>::type);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            cameraMatrix1.at<double>(i,j) = cameramatrix1[i+j*3];
        }
    }

    //-> Get first distortion coefficients
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr6, &iRows, &iCols, &distcoeffs1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    Mat distCoeffs1(iRows,1,DataType<double>::type);

    for(int i=0; i<iRows; i++)
    {
        distCoeffs1.at<double>(i,0) = distcoeffs1[i]; 
    }

    //-> Get second camera matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr7); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr7, &iRows, &iCols, &cameramatrix2);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    //-> Need to store the camera matrix 
    //   in a Mat object
    Mat cameraMatrix2(3,3,DataType<double>::type);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            cameraMatrix2.at<double>(i,j) = cameramatrix2[i+j*3];
        }
    }

    //-> Get second distortion coefficients
    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddr8); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr8, &iRows, &iCols, &distcoeffs2);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    Mat distCoeffs2(iRows,1,DataType<double>::type);

    for(int i=0; i<iRows; i++)
    {
        distCoeffs2.at<double>(i,0) = distcoeffs2[i]; 
    }

    //-> Get width of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddr9); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr9, &width);
    if(intErr)
    {
       return intErr;
    }

    //-> Get height of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddr10); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr10, &height);
    if(intErr)
    {
       return intErr;
    }

    double rms = stereoCalibrate(obPts, imPts, imPts2,
                  cameraMatrix1, distCoeffs1,
                  cameraMatrix2, distCoeffs2,
                  Size(width,height), rotationMatrix, translationVector, essentialMatrix, fundamentalMatrix,
                  TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5),
                  CV_CALIB_FIX_ASPECT_RATIO +
                  CV_CALIB_ZERO_TANGENT_DIST +
                  CV_CALIB_SAME_FOCAL_LENGTH +
                  CV_CALIB_RATIONAL_MODEL +
                  CV_CALIB_FIX_K3 + CV_CALIB_FIX_K4 + CV_CALIB_FIX_K5);


    //-> Return rotation matrix 
    Rows = rotationMatrix.rows;
    Cols = rotationMatrix.cols;
    double *rotMatrix = NULL;
    rotMatrix = (double*)malloc(sizeof(double)*Rows*Cols);

    for(int i=0; i<Rows; i++)
    {
    	for(int j=0; j<Cols; j++)
    	{
    		rotMatrix[i+j*Rows] = rotationMatrix.at<double>(i,j);
    	}
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1 , Rows, Cols, rotMatrix); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
  	//Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

	//-> Return translation vector 
    Rows = translationVector.rows;
    Cols = translationVector.cols;
    double *transVector = NULL;
    transVector = (double*)malloc(sizeof(double)*Rows*Cols);

    for(int i=0; i<Rows; i++)
    {
    	for(int j=0; j<Cols; j++)
    	{
    		transVector[i+j*Rows] = translationVector.at<double>(i,j);
    	}
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2 , Rows, Cols, transVector); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
  	//Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    //-> Return essential matrix 
    Rows = essentialMatrix.rows;
    Cols = essentialMatrix.cols;
    double *essMatrix = NULL;
    essMatrix = (double*)malloc(sizeof(double)*Rows*Cols);

    for(int i=0; i<Rows; i++)
    {
    	for(int j=0; j<Cols; j++)
    	{
    		essMatrix[i+j*Rows] = essentialMatrix.at<double>(i,j);
    	}
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+3 , Rows, Cols, essMatrix); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
  	//Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;

    //-> Return fundamental matrix 
    Rows = fundamentalMatrix.rows;
    Cols = fundamentalMatrix.cols;
    double *fundMatrix = NULL;
    fundMatrix = (double*)malloc(sizeof(double)*Rows*Cols);

    for(int i=0; i<Rows; i++)
    {
    	for(int j=0; j<Cols; j++)
    	{
    		fundMatrix[i+j*Rows] = fundamentalMatrix.at<double>(i,j);
    	}
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+4 , Rows, Cols, fundMatrix); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
  	//Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
  	return 0;
  }
}