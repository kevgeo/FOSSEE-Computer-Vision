/********************************************************
Author: Vinay Bhat
********************************************************
Usage: return_image = imhmin(input_image, h)
Example:
  im = imhmin(image, 80)
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

  void imhmin_imreconstruct(Mat, Mat, Mat&);
  
  int opencv_imhmin(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddr = NULL;
    double h;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    // Get the input image from the Scilab environment
    Mat image;
    retrieveImage(image, 1);

    // Get the address of 2nd argument, the H-Minima transform scalar
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    // Get the H-Minima transform scalar
    intErr = getScalarDouble(pvApiCtx, piAddr, &h);
    if(intErr)
    {
        return intErr;
    }   

    if (h < 0)
    {
        sciprint("Please enter a nonnegative scalar for H-minima transform.\n");
        return 0;
    }
    
    Mat gray_image, m, dst;
    cvtColor(image, gray_image, CV_BGR2GRAY);
    min((gray_image+h), 255, m);
    imhmin_imreconstruct(gray_image, m, dst);
    
    string tempstring = type2str(dst.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, dst, 1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
  void imhmin_imreconstruct(Mat g, Mat f, Mat& dest)
  {
    Mat m0, m1, m;
    m1 = f;
    do {
      m0 = m1.clone();
      erode(m0, m, Mat());
      max(g, m, m1);
    } while(countNonZero(m1 != m0) != 0);
    dest = m1.clone();
  }  

/* ==================================================================== */
}
