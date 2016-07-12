/*
 * gabor
 *
 * gabor in scilab
 *
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of gabor method of matlab
// Usage:
// gabor(wavelength,orientation) - Generates a gabor kernel with the given wavelength and orientation
// wavelength: pixels/cycle of the sinusoidal carrier, must be >=2
// orientation: orientation of the filter in degrees, must be between [0,360]

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

int opencv_gabor(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;
  int iRows = 0, iCols = 0;
  int *piAddr = NULL;
  int *piAddr1 = NULL;
  int *piAddr2 = NULL;

  int error;

  double wavelength;
  double orientation;

  // Get the number of input arguments
  int inputarg = *getNbInputArgument(pvApiCtx);

  // String holding the second argument
  int iRet = 0;
  char *pstData = NULL;

  // Checking input argument
  CheckInputArgument(pvApiCtx, 2, 2);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // Geting the wavelength
  sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);

  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return 0;
  }

  intErr = getScalarDouble(pvApiCtx, piAddr1, &wavelength);
  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return intErr;
  }

  if (wavelength < 2) {
    sciprint("Wavelength must be >=2");
    return 0;
  }
  // Getting the orientation
  sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);

  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return 0;
  }

  intErr = getScalarDouble(pvApiCtx, piAddr2, &orientation);
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

  Mat new_image = getGaborKernel(cv::Size(K_size, K_size), sigma, orientation,
                                 wavelength, 0.5, 0);
  // sciprint("\n");

  //    for (int i = 0; i < new_image.rows; i++) {
  //      for (int j = 0; j < new_image.cols; j++) {
  //        sciprint("%f ", new_image.at<double>(i,j));

  //      }

  //   sciprint("\n");
  //    }

  // new_image is sent to scilab as output

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
