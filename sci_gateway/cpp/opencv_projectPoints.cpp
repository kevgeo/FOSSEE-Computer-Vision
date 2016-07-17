//*******************************************************************************************************
// Authors : Kevin George
//
// projectPoints(objectPoints,rvec,tvec,cameraMatrix,distCoeffs,aspectRatio);
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
  
  int opencv_projectPoints(char *fname, unsigned long fname_len)
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
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    //-> Input
    double *objectPoints = NULL; 
    double *rvec = NULL;
    double *tvec = NULL; 
    double *cameraMatrix = NULL;
    double *distCoeffs = NULL;
    double aspectRatio;
    int num_InputArgs;

    //-> Get object points
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &objectPoints);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 


    int size = (iRows*iCols)/3;
    vector<Point3f> obPts(size);
    k = 0;
    for(int i=0; i<size; i++)
    {
        obPts[i].x = objectPoints[k++];
        obPts[i].y = objectPoints[k++];
        obPts[i].z = objectPoints[k++];
    }

    //-> Get rotation vector
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &rvec);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    Mat Rvec(3,1,cv::DataType<double>::type);
    for(int i=0; i<3; i++)
        Rvec.at<double>(i) = rvec[i];

    //-> Get translation vector
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows, &iCols, &tvec);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    Mat Tvec(3,1,cv::DataType<double>::type);
    for(int i=0; i<3; i++)
        Tvec.at<double>(i) = tvec[i]; 

    //-> Get camera matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &cameraMatrix);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    Mat cameramatrix(3,3,CV_32F);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            cameramatrix.at<double>(i,j) = cameraMatrix[i+j*3];
        }
    }

    //-> Get distortion coefficients
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &iRows, &iCols, &distCoeffs);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    Mat distortioncoeffs(iRows,1,DataType<double>::type);

    for(int i=0; i<iRows; i++)
    {
        distortioncoeffs.at<double>(i) = distCoeffs[i]; 
    }


    //-> Get aspect ratio value
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr6, &aspectRatio);
    if(intErr)
    {
       return intErr;
    }    

    Mat jacobian;
    vector<Point2f> imagePoints;
    // Calling projectPoints function
    projectPoints(obPts,Rvec,Tvec,cameramatrix,distortioncoeffs,imagePoints,jacobian,aspectRatio);

    size = imagePoints.size();
    double *output = NULL;
    output = (double*)malloc(sizeof(double)*size*2);

    j = 0;
    //Storing x and y coordinates
    for(int i=0; i<size; i++)
    {
        output[j++] = imagePoints[i].x;
        output[j++] = imagePoints[i].y;
    }
    
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, 2, size, output); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

   
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;    
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
