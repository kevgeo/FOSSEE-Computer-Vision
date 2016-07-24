//*******************************************************************************************************
// Authors : Kevin George
//
// RQDecomp3x3(src, mtxR, mtxQ,Qx,Qy,Qz);
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
  
  int opencv_RQDecomp3x3(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 2, 5);

    //-> Input
    double *src = NULL;
    Mat srcMatrix = (Mat_<double>(3,3));
    
    //-> Outputs
    Mat mtxR  = (Mat_<double>(3,3)); 
    Mat mtxQ  = (Mat_<double>(3,3)); 
    //-> Optionl outputs
    Mat Qx = (Mat_<double>(3,3));
    Mat Qy = (Mat_<double>(3,3)); 
    Mat Qz = (Mat_<double>(3,3));
    
    //-> Get input matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &src);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if( iRows!=3 || iCols!=3 )
    {
        Scierror(999,"Input matrix should be 3x3.\n");
            return 0;
    }

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            srcMatrix.at<double>(i,j) = src[i+j*3];
        }
    }


    //-> Calling decomposeProjectionMatrix function
    RQDecomp3x3(srcMatrix, mtxR, mtxQ,Qx,Qy,Qz);
    
    //-> Variable for returning output matrices
    double *traingularMatrix = NULL;
    double *orthogonalMatrix = NULL;
    double *rotX = NULL;
    double *rotY = NULL;
    double *rotZ = NULL;

    traingularMatrix = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            traingularMatrix[i+j*3] = mtxR.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, 3, 3, traingularMatrix); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;

    orthogonalMatrix = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            orthogonalMatrix[i+j*3] = mtxQ.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, 3, 3, orthogonalMatrix); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2; 

    rotX = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            rotX[i+j*3] = Qx.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+3, 3, 3, rotX); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3;

    rotY = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            rotY[i+j*3] = Qy.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+4, 3, 3, rotY); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx)+4;

    rotZ = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            rotZ[i+j*3] = Qz.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+5, 3, 3, rotZ); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx)+5;


    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
 }

}
