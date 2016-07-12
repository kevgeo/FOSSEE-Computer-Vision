/********************************************************
    Author: Shubheksha Jalan
*********************************************************
 void getRectSubPix(InputArray image, Size patchSize, Point2f center, OutputArray patch, int patchType=-1 )¶
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
  
  int opencv_getrectsubpix(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6 = NULL;
    int *piAddr7 = NULL;
    int i,j,k;
    double centerX, centerY, alpha, beta, patchType;
 

     //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for first value of size
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr2, &alpha);
     if(intErr)
    {
        return intErr;
    }   


    //for second value of size
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr3, &beta);
    if(intErr)
    {
        return intErr;
    }   

     //for x coordinate of center
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr4, &centerX);
    if(intErr)
    {
        return intErr;
    }   

     //for y coordinate of center
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &centerY);
    if(intErr)
    {
        return intErr;
    }   
     

      //for patchType
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6, &patchType);
    if(intErr)
    {
        return intErr;
    }


    Size patchSize = Size(alpha, beta);

    Point2f center = Point2f(centerX, centerY);

    Mat patch;

    getRectSubPix(image, patchSize, center, patch, patchType);   
     

    String tempstring = type2str(patch.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, patch, 1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
