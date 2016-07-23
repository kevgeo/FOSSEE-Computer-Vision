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
  
  int opencv_composeRT(char *fname, unsigned long fname_len)
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
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2);

    //-> Input
    double *r1 = NULL; //first rotation vector
    double *r2 = NULL; //second rotation vector
    double *t1 = NULL; //first translation vector
    double *t2 = NULL; //second translation vector

    
    //-> Outputs
    Mat rvec3,tvec3;
    
    //-> Get first rotation std::vector<char> v;
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &r1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

   if( iCols>1)
    {
        Scierror(999,"Only column vector is allowed. Make sure that the inputs are column vectors.\n");
            return 0;
    }
    
    if( iRows!=3)
    {
        Scierror(999,"Only row vector of size 3x1 is allowed. Make sure that the inputs are of this size.\n");
            return 0;
    }

    Mat rvec1(iRows,iCols,DataType<double>::type);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            rvec1.at<double>(i,j) = r1[i+j*iRows];
        }
    }

    //-> Get first translation vector
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &t1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if(iCols>1)
    {
        Scierror(999,"Only column vector is allowed. Make sure that the inputs are column vectors.\n");
            return 0;
    }
    if( iRows!=3)
    {
        Scierror(999,"Only row vector of size 3x1 is allowed. Make sure that the inputs are of this size.\n");
            return 0;
    }
    
    Mat tvec1(iRows,iCols,DataType<double>::type);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            tvec1.at<double>(i,j) = t1[i+j*iRows];
        }
    }

    //-> Get second rotation vector
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows, &iCols, &r2);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if(iCols>1)
    {
        Scierror(999,"Only column vector is allowed. Make sure that the inputs are column vectors.\n");
            return 0;
    }

    if( iRows!=3)
    {
        Scierror(999,"Only row vector of size 3x1 is allowed. Make sure that the inputs are of this size.\n");
            return 0;
    }
    
    Mat rvec2(iRows,iCols,DataType<double>::type);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            rvec2.at<double>(i,j) = r2[i+j*iRows];
        }
    }

    //-> Get second translation vector
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &t2);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if(iCols>1)
    {
        Scierror(999,"Only column vector is allowed. Make sure that the inputs are column vectors.\n");
            return 0;
    }
    if( iRows!=3)
    {
        Scierror(999,"Only row vector of size 3x1 is allowed.  Make sure that the inputs are of this size.\n");
            return 0;
    }
    
    Mat tvec2(iRows,iCols,DataType<double>::type);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            tvec2.at<double>(i,j) = t2[i+j*iRows];
        }
    }

    //-> Calling composeRT function
    composeRT(rvec1, tvec1, rvec2, tvec2, rvec3, tvec3);
    
    //-> Variable for returning output matrices
    double *r3 = NULL;
    double *t3 = NULL;

    iRows = rvec3.rows;
    iCols = rvec3.cols;

    r3 = (double*)malloc(sizeof(double)*iRows*iCols);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            r3[i+j*iRows] = rvec3.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, iRows, iCols, r3); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;

    iRows = tvec3.rows;
    iCols = tvec3.cols;

    t3 = (double*)malloc(sizeof(double)*iRows*iCols);
    for(int i=0; i<iRows; i++)
    {
        for(int j=0; j<iCols; j++)
        {
            t3[i+j*iRows] = tvec3.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, iRows, iCols, t3); 
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
