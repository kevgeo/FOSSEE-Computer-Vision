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
  
  int opencv_calibrateCamera(char *fname, unsigned long fname_len)
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
	    CheckInputArgument(pvApiCtx, 7, 7);
	    CheckOutputArgument(pvApiCtx, 2, 2);

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

        //-> Get width of image
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr3); 
        if (sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }

        intErr = getScalarDouble(pvApiCtx, piAddr3, &width);
        if(intErr)
        {
           return intErr;
        }

        //-> Get height of image
        sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr4); 
        if (sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }

        intErr = getScalarDouble(pvApiCtx, piAddr4, &height);
        if(intErr)
        {
           return intErr;
        }


        //-> Get camera matrix
        sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr5); 
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
        Mat cameraMatrix(3,3,DataType<double>::type);
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                cameraMatrix.at<double>(i,j) = cameramatrix1[i+j*3];
            }
        }

        //-> Get distortion coefficients
        sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr6); 
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

        Mat distCoeffs(iRows,1,DataType<double>::type);

        for(int i=0; i<iRows; i++)
        {
            distCoeffs.at<double>(i,0) = distcoeffs1[i]; 
        }


        vector<Mat> rvecs;
        vector<Mat> tvecs;
        calibrateCamera(obPts, imPts, Size(width,height), 
                        cameraMatrix, distCoeffs, rvecs, tvecs);
  


    int *outList = NULL;
    
    //-> Return rotation vectors 
    int num = rvecs.size();
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, num, &outList);
    if(sciErr.iErr)
    {
           printError(&sciErr, 0);
           return 0;
    }

    for(int idx=0; idx<num; idx++)
    {
        Rows = rvecs[idx].rows;
        Cols = rvecs[idx].cols;
        double *rotMatrix = NULL;
        rotMatrix = (double*)malloc(sizeof(double)*Rows*Cols);

        for(int i=0; i<Rows; i++)
        {
        	for(int j=0; j<Cols; j++)
        	{
        		rotMatrix[i+j*Rows] = rvecs[idx].at<double>(i,j);
        	}
        }

        sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, outList, idx+1, Rows, Cols, rotMatrix);
      	if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        free(rotMatrix);

    }
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    //-> Return translation vectors 
    int num2 = tvecs.size();
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, num2, &outList);
    if(sciErr.iErr)
    {
           printError(&sciErr, 0);
           return 0;
    }

    for(int idx=0; idx<num2; idx++)
    {
        Rows = tvecs[idx].rows;
        Cols = tvecs[idx].cols;
        double *transMatrix = NULL;
        transMatrix = (double*)malloc(sizeof(double)*Rows*Cols);

        for(int i=0; i<Rows; i++)
        {
            for(int j=0; j<Cols; j++)
            {
                transMatrix[i+j*Rows] = tvecs[i].at<double>(i,j);
            }
        }

        sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+2, outList, idx+1, Rows, Cols, transMatrix);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        free(transMatrix);

    }
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

	
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);

  	return 0;
  }
}