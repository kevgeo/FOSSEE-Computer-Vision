//*******************************************************************************************************
// Authors : Kevin George
//
// new_cameraMatrix =  getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, Size(width,height) 
//                                                      ,alpha , newImgSize, validPixROI,centerPrincipalPoint);
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
  
  int opencv_getOptimalNewCameraMatrix(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr  = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    int *piAddr8  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 8, 8);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
    double *cameramatrix = NULL;
    double *distcoeffs = NULL;

    Mat cameraMatrix(3,3,CV_32F); 
    Mat new_cameraMatrix;
    double width,height; // for image size
    double width2,height2; // for new image size
    Rect* validPixROI;
    double alpha,centerPrincipalPoint;
    
    //-> Get camera matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &cameramatrix);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    if( iRows!=3 || iCols !=3 )
    {
        Scierror(999,"Please make sure that camera matrix is 3x3.\n");
            return 0;
    }


    k = 0;
    for(int i=0; i<3;i++)
    {
        for(int j=0; j<3;j++)
        {
            cameraMatrix.at<double>(i,j) = cameramatrix[i+j*3];
        }
    }

    //-> Get distortion coefficients
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &distcoeffs);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }    

    if( iCols == 1)
    {
        if( iRows!=4 && iRows!=5 && iRows!=8 )
        {
            Scierror(999,"Please enter column vector of distortion coefficients either with size 4,5 or 8.\n");
            return 0;
        }
    }

     Mat distCoeffs(iRows,iCols,DataType<double>::type);
     int size = iRows*iCols;
     for(int i=0; i<size; i++)
     {
        distCoeffs.at<double>(i) = distcoeffs[i];
     }

    //-> Get width of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
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

    if( width <=0 )
    {
        Scierror(999,"Original width size must be greater than zero.\n");
            return 0;
    }

    //-> Get height of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
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

    if( height <=0 )
    {
        Scierror(999,"Original height size must be greater than zero.\n");
            return 0;
    }

    Size imgSize(width,height);

    //-> Get alpha value
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }


    intErr = getScalarDouble(pvApiCtx, piAddr5, &alpha);
    if(intErr)
    {
       return intErr;
    }

    if( alpha<0 || alpha>1)
    {
        Scierror(999,"Alpha value must be between 0 and 1.");
    }

    
    //-> Get new width of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr6, &width2);
    if(intErr)
    {
       return intErr;
    }

    if( width2 <=0 )
    {
        Scierror(999,"New width size must be greater than zero.\n");
            return 0;
    }
    //-> Get new height of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr7); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }


    intErr = getScalarDouble(pvApiCtx, piAddr7, &height2);
    if(intErr)
    {
       return intErr;
    }

    if( height2 <=0 )
    {
        Scierror(999,"New height size must be greater than zero.\n");
            return 0;
    }

    Size newImgSize(width2,height2);

    //-> Get centralPrincipalPoint value
    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddr8); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }


    intErr = getScalarDouble(pvApiCtx, piAddr8, &centerPrincipalPoint);
    if(intErr)
    {
       return intErr;
    }

    if( centerPrincipalPoint!=0 && centerPrincipalPoint!=1)
    {
        Scierror(999,"centralPrincipalPoint value must be either be 0(false) or 1(true).");
    }

    if( centerPrincipalPoint == 0)
    {
        new_cameraMatrix =  getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imgSize, 
                                                        alpha , newImgSize, validPixROI,false);

    }

    else if( centerPrincipalPoint == 1)
    {
        new_cameraMatrix =  getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imgSize,
                                                             alpha , newImgSize, validPixROI,true); 
    }
    
    double *output = NULL;
    output = (double*)malloc(sizeof(double)*9);

    k = 0;
    for(int i=0; i<3;i++)
    {
        for(int j=0; j<3;j++)
        {
            output[k++] = new_cameraMatrix.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1 , 3, 3, output); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
