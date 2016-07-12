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
  
  int opencv_solvePnPRansac(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr  = NULL;
    int *piAddr5  = NULL;
    int *piAddr7  = NULL;
    int *piAddr8  = NULL;
    int *piAddr9  = NULL;
    int i,j,k ;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 9, 9);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    Mat objectPoints, imagePoints, cameraMatrix, distCoeffs;
    double useExtrinsicGuess;
    char *flags = NULL; //-> Stores current string representing 'name' of name,value pair arguments
    double iterations, reprojectionError,minInliersCount;
    Mat rvec,tvec,inliers; //output

    //-> Get array of object points
    retrieveImage(objectPoints,1);

    //-> Get array of corresponding image points
    retrieveImage(imagePoints,2);

    //-> Get camera matrix
    retrieveImage(cameraMatrix,3);

    //-> Get vector of distortion coefficients
    retrieveImage(distCoeffs,4);

    //-> Get useExtrinsicGuess value
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr5, &useExtrinsicGuess);
    if(intErr)
    {
       return intErr;
    }
    
    //-> Getting flags string
            sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr); 
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

    //-> Get number of iterations 
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr7); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr7, &iterations);
    if(intErr)
    {
       return intErr;
    }

    //-> Get inlier threshold value
    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddr8); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr8, &reprojectionError);
    if(intErr)
    {
       return intErr;
    }

    //-> Get number of inliers
    sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddr9); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr9, &minInliersCount);
    if(intErr)
    {
       return intErr;
    }

    //-> Calling solvePnP function
    if(useExtrinsicGuess == 0)
    {
        if(strcmp (flags,"CV_ITERATIVE") )
        {   solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs
                        ,rvec,tvec,false, int(iterations),reprojectionError,int(minInliersCount),
                          inliers,CV_ITERATIVE);
        }

        else if(strcmp (flags,"CV_P3P") )
        {   
            solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs
                        ,rvec,tvec,false, int(iterations),reprojectionError,int(minInliersCount),
                          inliers, CV_P3P);
        }

        else if(strcmp (flags,"CV_EPNP") )
        {   
            solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs
                        ,rvec,tvec,false, int(iterations),reprojectionError,int(minInliersCount),
                          inliers, CV_EPNP);
        }   
    }

    else if(useExtrinsicGuess == 1)
    {
        if(strcmp (flags,"CV_ITERATIVE") )
        {   
            solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs
                        ,rvec,tvec,true, int(iterations),reprojectionError,int(minInliersCount),
                          inliers, CV_ITERATIVE);
        }

        else if(strcmp (flags,"CV_P3P") )
        {   
            solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs
                        ,rvec,tvec,true, int(iterations),reprojectionError,int(minInliersCount),
                          inliers, CV_P3P);
        }

        else if(strcmp (flags,"CV_EPNP") )
        {   
            solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs
                        ,rvec,tvec,true, int(iterations),reprojectionError,int(minInliersCount),
                          inliers, CV_EPNP);
        } 
    
    }

    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(rvec.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,rvec,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    //temp variable was not needed, hence has been discarded
    string tempstring2 = type2str(tvec.type());
    checker = (char *)malloc(tempstring2.size() + 1);
    memcpy(checker, tempstring2.c_str(), tempstring2.size() + 1);
    returnImage(checker,tvec,2); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
