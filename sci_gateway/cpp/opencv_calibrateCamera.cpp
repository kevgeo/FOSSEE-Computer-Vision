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
    
  int opencv_calibrateCamera(char *fname, unsigned long fname_len)
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
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2);

   nputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints, Size imageSize, InputOutputArray cameraMatrix, 
   InputOutputArray distCoeffs, OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs, int flags=0, 
   TermCriteria criteria=TermCriteria( TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON
    
    //-> Input
    double *objectpoints = NULL ;
    double *imagepoints = NULL;
    double width,height; // For Image Size
    //-> Output
    Mat cameraMatrix;
    Mat distCoeffs;
    Vector<Mat> rvecs;
    Vector<Mat> tvecs;    

    //-> Get feature point in first image
    //retrieveImage(points1,1);

    //-> Get calibration points
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

    //-> Get image points
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &imagepoints); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    //-> Get width value
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

    //-> Get column value
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

    calibrateCamera

    /*
    //-> Getting flags string
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

            //-> Extracting name of next argument takes three calls to getMatrixOfString
            //-> First call to get rows and columns
            sciErr = getMatrixOfString(pvApiCtx, piAddr4, &iRows, &iCols, NULL, NULL); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

            piLen = (int*) malloc(sizeof(int) * iRows * iCols);

            //-> Second call to retrieve length of each string
            sciErr = getMatrixOfString(pvApiCtx,  piAddr4,  &iRows,  &iCols,  piLen,  NULL); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            
            pstData = (char**) malloc(sizeof(char*) * iRows * iCols);
            for(int iterPstData = 0; iterPstData < iRows * iCols; iterPstData++)
            {
                pstData[iterPstData] = (char*) malloc(sizeof(char) * piLen[iterPstData] + 1); 
            }

            //-> Third call to retrieve data
            sciErr = getMatrixOfString(pvApiCtx, piAddr4, &iRows, &iCols, piLen, pstData); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }   

            flags = pstData[0];
            free(pstData);
            iRows=0;
            iCols=0;
            free(piLen);

    if(strcmp(flags,"CV_CALIB_CB_ADAPTIVE_THRESH"))
    {
        found = findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,CV_CALIB_CB_ADAPTIVE_THRESH);
    }

    else if(strcmp(flags,"CV_CALIB_CB_NORMALIZE_IMAGE"))
    {
        found = findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,CV_CALIB_CB_NORMALIZE_IMAGE);
    }

    else if(strcmp(flags,"CV_CALIB_CB_FILTER_QUADS"))
    {
        found = findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,CV_CALIB_CB_FILTER_QUADS);
    }

    else if(strcmp(flags,"CALIB_CB_FAST_CHECK"))
    {
        found =  findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,CALIB_CB_FAST_CHECK);
    }       

    else 
    {
        found = findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners);
    }
    */

    double found2 = double(found);

    //-> X & Y coordinates of detected corners
    double *xcoords;
    double *ycoords;
    int coords_size = corners.size();
    //vector<double> xcoords(corners.size());
    //vector<double> ycoords(corners.size());
   
    xcoords = (double*)malloc(sizeof(double)*coords_size);
    ycoords = (double*)malloc(sizeof(double)*coords_size);
    
    for(int i=0; i<corners.size();i++)
    {
        xcoords[i] = corners[i].x;
        ycoords[i] = corners[i].y;
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, coords_size, 1, xcoords); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, coords_size, 1, ycoords); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+3, found2);
    if(intErr)
    {
       return intErr;
    }

    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2;
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3; 
    

    //-> Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
 }

}
