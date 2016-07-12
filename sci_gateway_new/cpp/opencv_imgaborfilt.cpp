/*
 * imgaborfilt
 *
 * imgaborfilt in scilab
 *
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of imgaborfilt in scilab
// Usage:
// imgaborfilt(I,wavelength,orientation) : Perform gabor filtering on a grayscale image with given wavelength and orientation 
// imboxfilt(I,method)
// method : 'upright' (default)
// method : 'rotated' The area sums are calulated over a rectangle, which is
// rotated 45 degrees
// Known Changes from Matlab:
/*
 * 1) None, as of now
 */

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C" {
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>
#include "sciprint.h"
#include "../common.h"

int opencv_imgaborfilt(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;
  int iRows = 0, iCols = 0;
  int *piAddr = NULL;
  int *piAddr1 = NULL;
  int *piAddr2 = NULL;
  int *piAddr3 = NULL;

  int error;

  double wavelength;
  double orientation;

  int borderType = BORDER_REPLICATE;

  // Parameters for convolution - Leave these alone

  /* **********************************************************************************
   */

  Point anchor;
  double delta;
  int ddepth;

  anchor = Point(-1, -1);  // The center is unchanged
  delta = 0;               // No value is added to output
  ddepth = -1;  // The dimentions of input and output images are the same

  /* **********************************************************************************
   */

  // Get the number of input arguments
  int inputarg = *getNbInputArgument(pvApiCtx);

  // String holding the second argument
  int iRet = 0;
  char *pstData = NULL;

  // Checking input argument
  CheckInputArgument(pvApiCtx, 3, 3);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // Get input image

  Mat image;
  retrieveImage(image, 1);

  if (image.channels() > 1) {
    sciprint("The image must be grayscale.");
    return 0;
  }

  // Getting the wavelength
  sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);

  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return 0;
  }

  intErr = getScalarDouble(pvApiCtx, piAddr2, &wavelength);
  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return intErr;
  }

  if (wavelength < 2) {
    sciprint("Wavelength must be >=2");
    return 0;
  }
  // Getting the orientation
  sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);

  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return 0;
  }

  intErr = getScalarDouble(pvApiCtx, piAddr3, &orientation);
  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return intErr;
  }

  if ((orientation < 0) || (orientation > 360)) {
    sciprint("Orientation must be in the range [0,360]");
    return 0;
  }

  double sigma = (1 / CV_PI) * sqrt(log(2) / 2) * 3 *
                 wavelength;  // calculating sigma following matlab convention

  orientation = (orientation / 360) * 2 * CV_PI;  // Converting degree to radian
  int K_size =
      33 +
      16 * (wavelength - 2);  //  size of kernel following matlab convention

  Mat kernel = getGaborKernel(cv::Size(K_size, K_size), sigma, orientation,
                              wavelength, 0.5, 0);

  Mat floatimage;
  image.convertTo(floatimage, CV_32F);  // Converting image to float type

  Mat dst;
  Mat new_image;
  filter2D(floatimage, new_image, CV_32F, kernel);  // Performing convolution

  int temp = nbInputArgument(pvApiCtx) + 1;
  string tempstring = type2str(new_image.type());
  char *checker;
  checker = (char *)malloc(tempstring.size() + 1);
  memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
  returnImage(checker, new_image, 1);
  free(checker);

  // Assigning the list as the Output Variable
  AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
  // Returning the Output Variables as arguments to the Scilab environment
  ReturnArguments(pvApiCtx);
  return 0;
}
/* ==================================================================== */
}
