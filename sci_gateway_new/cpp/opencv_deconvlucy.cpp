/********************************************************
Author: Vinay Bhat
********************************************************
Usage: return_image = deconvlucy(input_image, PSF)
       return_image = deconvlucy(input_image, PSF, no_of_iterations)
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

  int opencv_deconvlucy(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddr = NULL;
    int i;
    double h;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    // Get the input image from the Scilab environment
    Mat image, psf;
    retrieveImage(image, 1);
    retrieveImage(psf, 2);

    // Check if number of arguments are three,
    // if so, then the number of iterations is also
    // specified.
    // If not, then set number of iterations as
    // ten.
    if (nbInputArgument(pvApiCtx) == 3)
    {
      // Get the address of the iterations scalar
      sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
      if (sciErr.iErr)
      {
          printError(&sciErr, 0);
          return 0;
      }

      // Get the no. of iterations scalar
      intErr = getScalarDouble(pvApiCtx, piAddr, &h);
      if(intErr)
      {
          return intErr;
      }   

      if (h < 0)
      {
          sciprint("Please enter a nonnegative scalar for number of iterations.\n");
          return 0;
      }
    }
    else 
      h = 10;

    // Check if the input image is grayscale or not,
    // if not, convert it to grayscale.
    if (image.type() != CV_8UC1)
    {
      Mat temp;
      temp = image.clone();
      cvtColor(temp, image, CV_BGR2GRAY);
    }

    // Check if the PSF is grayscale or not,
    // if not, convert it to grayscale.
    if (psf.type() != CV_8UC1)
    {
      Mat temp;
      temp = psf.clone();
      cvtColor(temp, psf, CV_BGR2GRAY);
    }

    // Apply the Lucy-Richardson method for deblurring
    // an image.
    Mat convRes, convRes2, temp;
    temp = image.clone();
    for (i = 0; i < h; i++)
    {
      filter2D(temp, convRes, -1, psf);
      max((temp - convRes), 0, convRes2);
      min((temp + convRes2), 255, convRes);
      temp = convRes.clone();
    }
    image = temp.clone();

    string tempstring = type2str(image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, image, 1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}

