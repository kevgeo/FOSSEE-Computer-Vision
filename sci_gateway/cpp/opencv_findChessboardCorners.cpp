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
  
  int opencv_findChessboardCorners(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2);

    //-> Input
    Mat inImage;
    double pts_rows,pts_cols; // Points per row and column
    char *flags = NULL; 
    //-> Output
    vector<Point2f> corners; // This will be filled by the detected corners
    bool found;

    //-> Get projection matrix
    retrieveImage(inImage,1);

    //-> Get points per row value
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr2, &pts_rows);
    if(intErr)
    {
       return intErr;
    }


    //-> Get points per column value
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &pts_cols);
    if(intErr)
    {
       return intErr;
    }

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

    if(strcmp(flags,"CV_CALIB_CB_ADAPTIVE_THRESH") == 0)
    {
        found = findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,CV_CALIB_CB_ADAPTIVE_THRESH);
    }

    else if(strcmp(flags,"CV_CALIB_CB_NORMALIZE_IMAGE") == 0)
    {
        found = findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,CV_CALIB_CB_NORMALIZE_IMAGE);
    }

    else if(strcmp(flags,"CV_CALIB_CB_FILTER_QUADS") == 0)
    {
        found = findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,CV_CALIB_CB_FILTER_QUADS);
    }

    else if(strcmp(flags,"CALIB_CB_FAST_CHECK") == 0)
    {
        found =  findChessboardCorners(inImage,Size(pts_rows,pts_cols),corners,CALIB_CB_FAST_CHECK);
    }       

    else 
    {
        sciprint("Wrong flag value used. Look at documentation for correct flag names.\n");
        return 0;
    }


    double found2 = double(found);

    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, found2);
    if(intErr)
    {
       return intErr;
    }

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

    
    int size2 = 2*corners.size();
    int j = 0;
    double *coords = NULL; // This will be returned as output, having the coordinates
    coords = (double*)malloc(sizeof(double)*size2);
    int k = 0;
    int i = 0;
    while( j < size2 )
    {
        coords[j++] = xcoords[k++];
        coords[j++] = ycoords[i++];  
    }
    
    
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2 , 2, coords_size, coords); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    
    

    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2;
    //AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3; 
    

    //-> Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
 }

}
