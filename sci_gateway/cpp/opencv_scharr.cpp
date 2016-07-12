/********************************************************
    Author: Sukul Bagai
*********************************************************
   return_image = scharr(input_image, ddepth, dx, dy, scale, delta);
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
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
  
  int opencv_scharr(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6 = NULL;
    int *piAddr7 = NULL;
    char **ddepth = NULL;
    int i,j,k;
    double dx,dy,scale,delta;


    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for ddepth
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //Now, we will retrieve the string from the input parameter. For this, we will require 3 calls
    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    ddepth = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
        ddepth[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, ddepth);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //for value of dx
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &dx);
    if(intErr)
        return intErr;

    //for value of dy
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &dy);
    if(intErr)
        return intErr;

    //for value of scale
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &scale);
    if(intErr)
        return intErr;

    //for value of delta
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6, &delta);
    if(intErr)
        return intErr;

    Mat new_image(image.rows,image.cols,image.type());

    if(strcmp(ddepth[0],"CV_8U")==0)
        Scharr(image,new_image,CV_8U,dx,dy,scale,delta);
    else if(strcmp(ddepth[0],"CV_16U")==0)
        Scharr(image,new_image,CV_16U,dx,dy,scale,delta);
    else if(strcmp(ddepth[0],"CV_16S")==0)
        Scharr(image,new_image,CV_16S,dx,dy,scale,delta);
    else if(strcmp(ddepth[0],"CV_32F")==0)
        Scharr(image,new_image,CV_32F,dx,dy,scale,delta);
    else if(strcmp(ddepth[0],"CV_64F")==0)
        Scharr(image,new_image,CV_64F,dx,dy,scale,delta);

    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_image,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
