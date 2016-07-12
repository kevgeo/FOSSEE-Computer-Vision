//*******************************************************************************************************
// Authors : Kevin George
//
// findHomography(srcPoints, dstPoints, method, ransacReprojThreshold(only for RANSAC method), mask);
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
  
  int opencv_findHomography(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int i,j,k ;
    char **pstData = NULL;
    char *method = NULL; //-> Stores method string
    int *piLen = NULL;
    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat srcPoints, dstPoints, H;
    double ransacReprojThreshold;    

    //-> Get coordinates of points in original plane 
    retrieveImage(srcPoints,1);

    //-> Get coordinates of points in target plane 
    retrieveImage(dstPoints,2);

    //-> Get method
    
    //-> Getting address of next argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    //-> Extracting name of next argument takes three calls to getMatrixOfString
    //-> First call to get rows and columns
    sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, NULL, NULL); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    piLen = (int*) malloc(sizeof(int) * iRows * iCols);

    //-> Second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx,  piAddr3,  &iRows,  &iCols,  piLen,  NULL); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    //-> Third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen, pstData); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    method = pstData[0];
    free(pstData);
    iRows=0;
    iCols=0;
    free(piLen);

    //-> Get ransacReprojThreshold value 
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr4, &ransacReprojThreshold);
    if(intErr)
    {
       return intErr;
    } 


    vector<uchar> states;
    //-> Calling findFundamentalMat function
    
     if(strcmp( method, "FM_LMEDS")==0)
            H = findHomography(srcPoints, dstPoints, FM_LMEDS );
    
     else if(strcmp( method, "FM_RANSAC")==0)
            H = findHomography(srcPoints, dstPoints, FM_RANSAC, ransacReprojThreshold, states);

     else if(strcmp( method, "0")==0)
            H = findFundamentalMat(srcPoints, dstPoints, 0 );


    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(H.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,H,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
