/*
 * bwlookup
 *
 * 2*2 and 3*3 non linear filtering of a binary image based on a lookup table -
 *lut which is taken as input from the user
 *
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of bwlookup
// Usage:
// bwlookup(I,lut)

// I is the input binary grayscale image. If the image is not binary, it is
// converted to one.
// lut is a 1*16 double vector [2*2 filtering], or a [1*512] double vector [3*3
// filtering]
// The indexing method used is the same as Matlab bwlookup:
// http://in.mathworks.com/help/images/ref/bwlookup.html

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

int opencv_bwlookup(char* fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;

  int* piAddr = NULL;

  // checking input argument
  CheckInputArgument(pvApiCtx, 2, 2);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // get input matrix

  Mat image;
  retrieveImage(image, 1);

  if (image.channels() > 1) {
    sciprint("The image must be grayscale.");
    return -1;
  }

  double* lut;

  int iRows = 0, iCols = 0;
  sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return -1;
  }
  sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &lut);
  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return -1;
  }

  // Error Checking

  if (!((iCols == 16) || (iCols == 512))) {
    sciprint("Expected LUT (argument 2) to have 16 or 512 elements.\n");

    return -1;
  }

  if (iRows != 1) {
    sciprint("Expected input number 2, LUT, to be a vector.\n");
    return -1;
  }

  if (image.channels() != 1) {
    sciprint("Expected input number 1, A, to be two-dimensional.\n");
    return -1;
  }

  // temporary copy of image to perform computation
  // converting the image to a binary image
  Mat tempimg = Mat::zeros(image.size(), CV_8U);

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      if (image.at<double>(i, j) != 0) tempimg.at<uchar>(i, j) = 1;
    }
  }

  // pad the temporary copy of the image with zeroes to handle border cases
  copyMakeBorder(tempimg, tempimg, 1, 1, 1, 1, BORDER_CONSTANT, 0);

  // output images
  Mat new_image = Mat::zeros(image.size(), CV_32F);

  // temporary variables
  int ii, jj;
  int index;

  // 2*2 filtering
  if (iCols == 16) {
    for (int i = 0; i < image.rows; i++) {
      for (int j = 0; j < image.cols; j++) {
        ii = i + 1;
        jj = j + 1;

        index = tempimg.at<uchar>(ii, jj) * 1 +
                tempimg.at<uchar>(ii, jj + 1) * 2 +
                tempimg.at<uchar>(ii + 1, jj) * 4 +
                tempimg.at<uchar>(ii + 1, jj + 1) * 8;

        new_image.at<float>(i, j) = lut[ 0, index ];
      }
    }
  }

  // 3*3 filtering
  if (iCols == 512) {
    for (int i = 0; i < image.rows; i++) {
      for (int j = 0; j < image.cols; j++) {
        ii = i + 1;
        jj = j + 1;

        index = tempimg.at<uchar>(ii - 1, jj - 1) * 1 +
                tempimg.at<uchar>(ii - 1, jj) * 2 +
                tempimg.at<uchar>(ii - 1, jj + 1) * 4 +
                tempimg.at<uchar>(ii, jj - 1) * 8 +
                tempimg.at<uchar>(ii, jj) * 16 +
                tempimg.at<uchar>(ii, jj + 1) * 32 +
                tempimg.at<uchar>(ii + 1, jj - 1) * 64 +
                tempimg.at<uchar>(ii + 1, jj) * 128 +
                tempimg.at<uchar>(ii + 1, jj + 1) * 256;

        new_image.at<float>(i, j) = lut[ 0, index ];
      }
    }
  }

  //   sciprint("\n");

  //  for (int i = 0; i < new_image.rows; i++) {
  //    for (int j = 0; j < new_image.cols; j++) {
  //      sciprint("%i ", new_image.at<uchar>(i,j));

  //    }

  // sciprint("\n");
  //  }

  // new_image is sent to scilab as output

  int temp = nbInputArgument(pvApiCtx) + 1;
  string tempstring = type2str(new_image.type());
  char* checker;
  checker = (char*)malloc(tempstring.size() + 1);
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
