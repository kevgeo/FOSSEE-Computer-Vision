/*
 * imhistmatch
 *
 * histogram matching in scilab
 *
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of imhistmatch method
// Usage:
// 1) imhistmatch(image,referenceimage)
// 2) imhistmatch(image,referenceimage,outputbins)

// Known Changes from Matlab:
/*
 * Default number of bins is 256 - for both input and output
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

// Matches the input image histogram with the reference image histogram,
// returning the output in n bins

Mat match(Mat image, Mat ref, int bins) {
  Mat dst, hist;
  Mat input_image = Mat::zeros(image.size(), CV_8U);
  Mat refhist;

  int inputbins = 256;

  Mat cdf = Mat(inputbins, 1, CV_8U, cvScalar(0));

  Mat refcdf = Mat(bins, 1, CV_8U, cvScalar(0));

  // Calculating histogram of image
  image.convertTo(dst, CV_8U, 1, 0);

  calcHist(&dst, 1, 0, Mat(), hist, 1, &inputbins, 0);

  // Calculating histogram of reference image

  ref.convertTo(dst, CV_8U, 1, 0);
  calcHist(&dst, 1, 0, Mat(), refhist, 1, &bins, 0);

  hist.copyTo(cdf);
  refhist.copyTo(refcdf);

  // calculate cdf
  for (int h = 1; h < inputbins; h++) {
    float binVal = hist.at<float>(h, 0);

    cdf.at<float>(h, 0) = cdf.at<float>(h, 0) + cdf.at<float>(h - 1, 0);
  }

  // normalize histogram
  for (int h = 0; h < inputbins; h++) {
    cdf.at<float>(h, 0) = cdf.at<float>(h, 0) / cdf.at<float>(inputbins - 1, 0);
  }

  // for reference image

  // calculate cdf
  for (int h = 1; h < bins; h++) {
    float binVal = refhist.at<float>(h, 0);

    refcdf.at<float>(h, 0) =
        refcdf.at<float>(h, 0) + refcdf.at<float>(h - 1, 0);
  }

  // normalize histogram
  for (int h = 0; h < bins; h++) {
    refcdf.at<float>(h, 0) =
        refcdf.at<float>(h, 0) / refcdf.at<float>(bins - 1, 0);
  }

  // for( int h = 0; h < bins; h++ )
  //          {
  //            sciprint("%f\n",refcdf.at<float>(h,0));

  //          }

  //          sciprint("\n");

  // for( int h = 0; h < bins; h++ )
  //          {
  //            sciprint("%f\n",cdf.at<float>(h,0));

  //          }

  float observed_cdf, minval;
  float minindex;

  for (int i = 0; i < input_image.rows; i++)
    for (int j = 0; j < input_image.cols; j++) {
      observed_cdf = cdf.at<float>(image.at<uchar>(i, j), 0);

      minval = 1000;  // will be overwritten

      for (int h = 0; h < bins; h++) {
        if ((abs(refcdf.at<float>(h, 0) - observed_cdf)) < minval) {
          minval = abs(refcdf.at<float>(h, 0) - observed_cdf);
          minindex = h;
        }
        // sciprint("%d\n",minindex);
      }

      input_image.at<uchar>(i, j) = minindex;
    }
  transpose(input_image, input_image);

  return input_image;
}

int opencv_imhistmatch(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;

  int *piAddr = NULL;
  int *piAddrNew = NULL;
  int *piAddr2 = NULL;
  int *piAddr3 = NULL;

  double num_bins = 256;  // default number of bins for histogram calculation

  // checking input argument
  CheckInputArgument(pvApiCtx, 2, 3);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // Get the number of input arguments
  int inputarg = *getNbInputArgument(pvApiCtx);

  // get input matrix

  Mat image, new_image, r, g, b;
  retrieveImage(image, 1);

  Mat ref;
  retrieveImage(ref, 2);

  int case1 = 0;
  if (image.channels() == 1)

  {
    if (ref.channels() != 1) {
      sciprint(
          "If A is a grayscale image, the ref image must also be grayscale\n");
      return 0;

    } else {
      case1 = 1;
    }
  }

  else if (image.channels() == 3)

  {
    if (ref.channels() == 1) {
      case1 = 2;
    }
    if (ref.channels() == 3) {
      case1 = 3;
    }
  }

  else {
    sciprint("Invalid Image\n");
    return 0;
  }

  if (!((image.channels() == 3) || (image.channels() == 1))) {
    sciprint("Invalid Image\n");
    return 0;
  }

  if (!((ref.channels() == 3) || (ref.channels() == 1))) {
    sciprint("Invalid Reference Image\n");
    return 0;
  }

  if (inputarg == 3) {
    // Get the number of bins for histogram calculation
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &num_bins);
    if (sciErr.iErr) {
      return intErr;
    }

    if (num_bins < 1) {
      sciprint("Invalid number of histogram bins\n");
      return 0;
    }
  }

  int bins = (int)num_bins;

  if (case1 == 1) {
    new_image = match(image, ref, bins);

  }

  else if (case1 == 2) {
    Mat rgb[3];

    split(image, rgb);

    r = match(rgb[0], ref, bins);

    g = match(rgb[1], ref, bins);

    b = match(rgb[2], ref, bins);

    vector<Mat> channels;

    channels.push_back(r);
    channels.push_back(g);
    channels.push_back(b);

    merge(channels, new_image);

    transpose(new_image, new_image);

  }

  else if (case1 == 3) {
    Mat rgb[3];
    Mat rgbref[3];

    split(ref, rgbref);
    split(image, rgb);

    r = match(rgb[0], rgbref[0], bins);

    g = match(rgb[1], rgbref[1], bins);

    b = match(rgb[2], rgbref[2], bins);

    vector<Mat> channels;

    channels.push_back(r);
    channels.push_back(g);
    channels.push_back(b);

    merge(channels, new_image);

    transpose(new_image, new_image);
  }

  // Normalizing the final image
  cv::normalize(new_image, new_image, 0, 255, NORM_MINMAX, CV_8U);

  // sciprint("\n");

  //  for (int i = 0; i < new_image.rows; i++) {
  //    for (int j = 0; j < new_image.cols; j++) {
  //      sciprint(" %i ", new_image.at<uchar>(i,j));

  //    }

  // sciprint("\n");
  //  }

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

