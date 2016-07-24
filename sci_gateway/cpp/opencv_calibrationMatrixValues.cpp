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
    
  int opencv_calibrationMatrixValues(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr  = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k ;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 5, 5);

    //-> Input
    double *cameramatrix = NULL;
    double width,height; // For Image Size
    double apertureWidth,apertureHeight; 
    //-> Output
    double fovx;
    double fovy;
    double focalLength;
    Point2d principalPoint;
    double aspectRatio;

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
        Scierror(999,"Camera matrix should be 3x3.\n");
            return 0;
    }

    //-> Need to store the camera matrix 
    //   in a Mat object
    Mat cameraMatrix(3,3,DataType<double>::type);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            cameraMatrix.at<double>(i,j) = cameramatrix[i+j*3];
        }
    }

    //-> Get width of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr2, &width);
    if(intErr)
    {
       return intErr;
    }

     if( width<=0 )
    {
        Scierror(999,"Image width should be more than zero.\n");
            return 0;
    }

    //-> Get height of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &height);
    if(intErr)
    {
       return intErr;
    }

    if( height<=0 )
    {
        Scierror(999,"Image height should be more than zero.\n");
            return 0;
    }

    //-> Get aperture width
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr4, &apertureWidth);
    if(intErr)
    {
       return intErr;
    }
    
    //-> Get aperture height
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr5, &apertureHeight);
    if(intErr)
    {
       return intErr;
    }

    //-> Calling calibrationMatrixValues function
    calibrationMatrixValues(cameraMatrix, Size(width,height), apertureWidth, apertureHeight, fovx, fovy, 
    focalLength, principalPoint, aspectRatio);
    
    //-> Returning output field of view along horizontal axis
    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, fovx);
    if(intErr)
    {
       return intErr;
    }
    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;

    //-> Returning output field of view along vertical axis
    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, fovy);
    if(intErr)
    {
       return intErr;
    }
    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2; 

    //-> Returning focal length
    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+3, focalLength);
    if(intErr)
    {
       return intErr;
    }
    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3; 

    //-> Returning principal point
    double *coords = NULL; // coordinates of principal point
    coords = (double*)malloc(sizeof(double)*2);
    coords[0] = principalPoint.x;
    coords[1] = principalPoint.y;

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+4 , 1, 2, coords); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    //-> Returning aspect ratio
    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+5, aspectRatio);
    if(intErr)
    {
       return intErr;
    }
    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx)+5;

    //-> Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
 }

}
