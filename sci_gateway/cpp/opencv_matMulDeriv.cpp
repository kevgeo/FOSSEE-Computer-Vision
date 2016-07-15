//*******************************************************************************************************
// Authors : Kevin George
//
// matMulDeriv(inImage, newval, maxSpeckleSize, maxDiff, outImage );
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
  
  int opencv_matMulDeriv(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    //-> Input
    double *matrix1;
    double *matrix2;

    //-> Get first matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &matrix1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    int rows1 = iRows;
    int cols1 = iCols;

    //-> Get second matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &matrix2);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
         return 0;
    }   

    int rows2 = iRows;
    int cols2 = iCols;
    
    if( cols1 != rows2 )
    {
        sciprint("Make sure that the dimensions of both matrices meets the property of matrix multiplication");
        return 0;
    }


    Mat A(rows1,cols1,CV_32F);
    Mat B(rows2,cols2,CV_32F);
    
    k = 0;
    for(int i=0; i<rows1; i++)
    {
        for(int j=0; j<cols1; j++)
            A.at<double>(i,j) = matrix1[k++];
    }

    k = 0;
    for(int i=0; i<rows2; i++)
    {
        for(int j=0; j<cols2; j++)
            B.at<double>(i,j) = matrix2[k++];
    }

    Mat dABdA,dABdB;
 
    //-> Calling filterSpeckles function
    matMulDeriv(A, B, dABdA, dABdB );
    

    double *output1 = NULL;
    output1 = (double*)malloc(sizeof(double)*rows1*cols2);
    double *output2 = NULL;
    output2 = (double*)malloc(sizeof(double)*rows1*cols2);

    k = 0;
    //-> First output derivative matrix
    for(int i=0; i<rows1; i++)
    {
        for(int j=0; j<cols2; j++)
            output1[k++] = dABdA.at<double>(i,j);
    }

    k = 0;
    //-> Second output derivative matrix
    for(int i=0; i<rows1; i++)
    {
        for(int j=0; j<cols2; j++)
            output2[k++] = dABdB.at<double>(i,j);
    }

  sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1 , rows1, cols2, output1); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2 , rows1, cols2, output2); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    
   
    
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
