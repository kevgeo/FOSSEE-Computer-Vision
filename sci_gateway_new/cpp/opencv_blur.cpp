/***************************************************
Author : Sukul Bagai
***************************************************/

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



  int opencv_blur(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    double ksize_width,ksize_height,anchorX,anchorY;

    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for value of ksize_height
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &ksize_height);
    if(intErr)
        return intErr;

    //for value of ksize_width
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3 ,&ksize_width);
    if(intErr)
        return intErr;

    //for value of anchorX
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &anchorX);
    if(intErr)
        return intErr;

    //for value of anchorY
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &anchorY);
    if(intErr)
        return intErr;

    //applying checks on input parameters
    Mat new_image(image.rows,image.cols,image.type());

    //error checks
    if(ksize_height<0)
    {
        sciprint("Positive Value Required for Height. 1 value was used instead");
        ksize_height=1;
    }
    if(ksize_width<0)
    {
        sciprint("Positive Value Required for Width. 1 value was used instead");
        ksize_width=1;
    }
    if(ksize_width != ksize_height)
    {
        sciprint("Blurring Kernel Size not equal. Value of ksize_height used for ksize_width as well");
        ksize_width = ksize_height;
    }
    if(anchorX >= ksize_height || anchorY >= ksize_width)
    {
        sciprint("Invalid anchor points. Default point (-1,-1) used instead");
        anchorY=-1;
        anchorX=-1;
    }

    //defining temporary size and point type variables to use in the function
    Size sz(ksize_height,ksize_width);
    Point pt(anchorX,anchorY);

    //void blur(InputArray src, OutputArray dst, Size ksize, Point anchor=Point(-1,-1), int borderType=BORDER_DEFAULT )
    blur(image,new_image,sz,pt);

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