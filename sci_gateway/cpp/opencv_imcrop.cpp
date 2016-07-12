/********************************************************
Author: Sukul Bagai
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
  #include "sciprint.h"
  #include "../common.h"

  int opencv_imcrop(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k;
    double x, y, width, height;

    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image, 1);

    //for value of top-left x-coordinate
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &x);
    if(intErr)
    {
        return intErr;
    }   

    //for value top-left y-coordinate
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &y);
    if(intErr)
    {
        return intErr;
    }

    //for value of width
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &width);
    if(intErr)
    {
        return intErr;
    }   

    //for value of height
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &height);
    if(intErr)
    {
        return intErr;
    }   

    if(x>=image.cols || y>=image.rows || x<0 || y<0)
    {
        sciprint("Invalid x or y value\n");
        return 0;
    }
    if(width<=0 || height<=0 || x+width > image.cols || y+height > image.rows)
    {
        sciprint("Invalid width or height value\n");
        return 0;
    }
    //defining a temporary rectangle, that denotes the area that has to be cropped into the new image
    Rect myROI(x, y, width, height);

    // Crop the full image to that image contained by the rectangle myROI
    // Note that this doesn't copy the data
    Mat croppedRef(image, myROI);

    Mat cropped;
    // Copy the data into new matrix
    croppedRef.copyTo(cropped);

    string tempstring = type2str(cropped.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,cropped,1);
    free(checker); 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
