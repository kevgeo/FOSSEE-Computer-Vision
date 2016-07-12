/***************************************************
Author : Sukul Bagai
****************************************************
Usage : return_image = gaussianblur(input_image,ksize_height,ksize_width,sigmaX,sigmaY);
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
  
  int opencv_gaussianblur(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k;
    double ksize_width,ksize_height,sigmaX,sigmaY;

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
    intErr = getScalarDouble(pvApiCtx, piAddr3, &ksize_width);
    if(intErr)
        return intErr;

    //for value of sigmaX
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &sigmaX);
    if(intErr)
        return intErr;

    //for value of sigmaY
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &sigmaY);
    if(intErr)
        return intErr;

    //applying checks to input parameters, and assigning default values in case of error
    Mat new_image(image.rows,image.cols,CV_8UC3);
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
    if((int)ksize_height%2==0)
    {
        ksize_height+=1;
        sciprint("Odd Value Required for Height. %f value was used instead",&ksize_height);
    }
    if((int)ksize_width%2==0)
    {
        ksize_width+=1;
        sciprint("Odd Value Required for Width. %f value was used instead",&ksize_width);
    }

    //temporary size variable, to use in function
    Size sz(ksize_height,ksize_width);

    //void GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT )
    GaussianBlur(image,new_image,sz,sigmaX,sigmaY);

    //returning image
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
