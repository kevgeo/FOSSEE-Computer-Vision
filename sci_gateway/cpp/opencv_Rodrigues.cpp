//*******************************************************************************************************
// Authors : Kevin George
//
// Rodrigues(src, dst, jacobian);
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
  
  int opencv_Rodrigues(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddr = NULL;
    int iRows=0,iCols=0;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 2) ;

     
    //-> Input
    double *src = NULL;

    //-> Output
    Mat dstMatrix,jacobianMatrix;

    //-> Get the input rotation vector
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

    if( (iRows==3 && iCols!=3) )
    {
        if( iCols!= 1)
        {    
            Scierror(999,"Invalid Argument. 3x3 matrix, 1x3 and 3x1 vectors are only allowed.\n");
                      return 0;
        }
    }
    
    if( (iCols==3 && iRows!=3) )
    {
        if( iRows!= 1)
        {    
            Scierror(999,"Invalid Argument. 3x3 matrix, 1x3 and 3x1 vectors are only allowed.\n");
                      return 0;
        }
    }

    Mat srcMatrix(iRows,iCols,DataType<double>::type);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            srcMatrix.at<double>(i,j) = src[i+j*iRows];
        }
    }
    
    
    //-> Calling Rodrigues function
    Rodrigues(srcMatrix, dstMatrix, jacobianMatrix);
    
    iRows = dstMatrix.rows;
    iCols = dstMatrix.cols;
    
    double *dst = NULL;
    dst = (double*)malloc(sizeof(double)*iRows*iCols);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            dst[i+j*iRows] = dstMatrix.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, iRows, iCols, dst); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;

    iRows = jacobianMatrix.rows;
    iCols = jacobianMatrix.cols;
    
    double *jacobian = NULL;
    jacobian = (double*)malloc(sizeof(double)*iRows*iCols);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            jacobian[i+j*iRows] = jacobianMatrix.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, iRows, iCols, jacobian); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
