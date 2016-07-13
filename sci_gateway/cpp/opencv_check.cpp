//*******************************************************************************************************
// Authors : Kevin George
//
// findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,flags);
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
    
  int opencv_check(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr  = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k ;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //-> Input
    double *objectpoints = NULL ;
    double *imagepoints = NULL;
    double width,height; // For Image Size
    //-> Output
    Mat cameraMatrix;
    Mat distCoeffs;
    Vector<Mat> rvecs;
    Vector<Mat> tvecs;    

    //-> Get corner coordinates
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &objectpoints); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    int size = iRows*iCols;
    int size2 = size/2;
    double *output = NULL;
    output = (double*)malloc(sizeof(double)*size);
    for(int i=0;i<9;i++)
    {
      output[i] = i;
    }
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, 2, size2, output); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
        
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 
    ReturnArguments(pvApiCtx);
    return 0;     

  }

}