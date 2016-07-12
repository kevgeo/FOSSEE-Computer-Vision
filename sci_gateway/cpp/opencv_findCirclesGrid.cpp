//*******************************************************************************************************
// Authors : Kevin George
//
//solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs
//                        ,rvec,tvec,true, iterations,reprojectionError,minInliersCount,
//                          inliers, flags);
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
  
  int opencv_findCirclesGrid(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr  = NULL;
    int i,j,k ;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    double pts_row, pts_colum; //Points per row and column
    char *flags = NULL; //-> Stores current string representing 'name' of name,value pair arguments
    Mat centers; //output

    //-> Get 8-bit grayscale or color image.
    retrieveImage(image,1);

    //-> Get number of circles per row
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr2, &pts_row);
    if(intErr)
    {
       return intErr;
    }

    //-> Get number of circles per column
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &pts_colum);
    if(intErr)
    {
       return intErr;
    }
    
    //-> Getting flags string
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

            //-> Extracting name of next argument takes three calls to getMatrixOfString
            //-> First call to get rows and columns
            sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

            piLen = (int*) malloc(sizeof(int) * iRows * iCols);

            //-> Second call to retrieve length of each string
            sciErr = getMatrixOfString(pvApiCtx,  piAddr,  &iRows,  &iCols,  piLen,  NULL); 
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
            sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData); 
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

    
    //-> Calling findCirclesGrid function
    if(strcmp (flags,"CALIB_CB_SYMMETRIC_GRID") )
    {   
         bool patternfound = findCirclesGrid(image, Size(pts_row,pts_colum), centers,CALIB_CB_SYMMETRIC_GRID);

    }

    else if(strcmp (flags,"CALIB_CB_ASYMMETRIC_GRID") )
    {   
        bool patternfound = findCirclesGrid(image, Size(pts_row,pts_colum), centers,CALIB_CB_ASYMMETRIC_GRID);        
    }

    else if(strcmp (flags,"CALIB_CB_CLUSTERING") )
    {   
        bool patternfound = findCirclesGrid(image, Size(pts_row,pts_colum), centers,CALIB_CB_CLUSTERING);       
    }   
    
    else
        bool patternfound = findCirclesGrid(image, Size(pts_row,pts_colum), centers);

    

    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(centers.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,centers,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
