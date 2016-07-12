/*
 * graycoprops
 *
 * graycoprops in scilab
 * Please refer to :
 * http://www.cse.unsw.edu.au/~icml2002/workshops/MLCV02/MLCV02-Bevk.pdf, p.3.
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of graycoprops method of matlab
// Usage:
// graycoprops(GLCM,property) : Calculates the Property of the input Gray level co-occurence matrix 

// Known Changes from Matlab:
/*
 * 1) None, as of now - Matching exactly - but does not use structures - for properties
 */

#include <numeric>
#include <math.h>
#include <vector>
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

double return_contrast(Mat image)

{
  double contrast = 0;

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      contrast = contrast + (pow((i - j), 2) * image.at<double>(i, j));
    }
  }
  return contrast;
}

double return_correlation(Mat image)

{
  double correlation = 0;

  std::vector<int> mg_rows;
  std::vector<int> mg_cols;

  // Meshgrid of row and column indices
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      mg_rows.push_back(j);
      mg_cols.push_back(i);
    }
  }

  // for (int i=0; i<rows.size();i++)
  //  sciprint("%d\n", rows[i]);

  Mat flat_image = image.reshape(1, 1);

  double mean_row = 0;
  double mean_col = 0;
  double std_row = 0;
  double std_col = 0;

  for (int i = 0; i < mg_rows.size(); i++)
    mean_row = mean_row + mg_rows[i] * flat_image.at<double>(i);

  for (int i = 0; i < mg_cols.size(); i++)
    mean_col = mean_col + mg_cols[i] * flat_image.at<double>(i);

  for (int i = 0; i < mg_rows.size(); i++)
    std_row =
        std_row + (pow(mg_rows[i] - mean_row, 2) * flat_image.at<double>(i));

  std_row = sqrt(std_row);

  for (int i = 0; i < mg_cols.size(); i++)
    std_col =
        std_col + (pow(mg_cols[i] - mean_col, 2) * flat_image.at<double>(i));

  std_col = sqrt(std_col);

  for (int i = 0; i < mg_rows.size(); i++)
    correlation =
        correlation + ((mg_rows[i] - mean_row) * (mg_cols[i] - mean_col) *
                       flat_image.at<double>(i));

  double denom = std_col * std_row;
  if (denom != 0) {
    correlation = correlation / denom;
  }

  // sciprint("\n");

  // sciprint("MR: %f",mean_row);
  // sciprint("MC: %f",mean_col);

  // sciprint("SR: %f",std_row);
  // sciprint("SC: %f",std_col);
  return correlation;
}

double return_energy(Mat image)

{
  double energy = 0;

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      energy = energy + pow(image.at<double>(i, j), 2);
    }
  }
  return energy;
}

double return_homogeneity(Mat image)

{
  double homogeneity = 0;

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      homogeneity = homogeneity + (image.at<double>(i, j) / (1 + abs(i - j)));
    }
  }
  return homogeneity;
}

int opencv_graycoprops(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;

  int *piAddr1 = NULL;

  int error;

  // String holding the second argument
  int iRet = 0;
  char *pstData = NULL;

  // Checking input argument
  CheckInputArgument(pvApiCtx, 1, 2);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // Get input image

  Mat image;
  retrieveImage(image, 1);

  // converting image to float

  image.convertTo(image, CV_64FC1, 1, 0);

  // for (int i = 0; i < image.rows; i++) {
  //   for (int j = 0; j < image.cols; j++) {
  //     sciprint("%f ", image.at<double>(i, j));
  //   }

  //   sciprint("\n");
  // }

  // Error Checks

  if (image.channels() == 1) {
    // Normalizing image

    double s = cv::sum(image)[0];
    image = image / s;

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr1);

    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }

    if (isStringType(pvApiCtx, piAddr1)) {
      if (isScalar(pvApiCtx, piAddr1)) {
        iRet = getAllocatedSingleString(pvApiCtx, piAddr1, &pstData);
      }
    }
    if (strcmp(pstData, "contrast") == 0) {
      double contrast = return_contrast(image);
      intErr =
          createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, contrast);
      if (intErr) return intErr;

      // Assigning the list as the Output Variable
      AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
      // Returning the Output Variables as arguments to the Scilab environment
      ReturnArguments(pvApiCtx);
      return 0;
    }

    else if (strcmp(pstData, "correlation") == 0) {
      double correlation = return_correlation(image);

      intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1,
                                  correlation);
      if (intErr) return intErr;

      // Assigning the list as the Output Variable
      AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
      // Returning the Output Variables as arguments to the Scilab environment
      ReturnArguments(pvApiCtx);
      return 0;
    } else if (strcmp(pstData, "energy") == 0) {
      double energy = return_energy(image);

      intErr =
          createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, energy);
      if (intErr) return intErr;

      // Assigning the list as the Output Variable
      AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
      // Returning the Output Variables as arguments to the Scilab environment
      ReturnArguments(pvApiCtx);
      return 0;
    }

    else if (strcmp(pstData, "homogeneity") == 0) {
      double homogeneity = return_homogeneity(image);

      intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1,
                                  homogeneity);
      if (intErr) return intErr;

      // Assigning the list as the Output Variable
      AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
      // Returning the Output Variables as arguments to the Scilab environment
      ReturnArguments(pvApiCtx);
      return 0;
    }

    else {
      sciprint("\nUnknown Parameter Name: %s\n", pstData);
       return 0;
    }
  }

  else if (image.channels() == 3) {
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr1);

    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }

    if (isStringType(pvApiCtx, piAddr1)) {
      if (isScalar(pvApiCtx, piAddr1)) {
        iRet = getAllocatedSingleString(pvApiCtx, piAddr1, &pstData);
      }
    }
    if (strcmp(pstData, "contrast") == 0) {
      vector<Mat> rgb;
      split(image, rgb);

      double *contrast = (double *)malloc(sizeof(double) * 3);
      double s;

      for (int i = 0; i < 3; i++) {
        s = cv::sum(rgb[i])[0];
        rgb[i] = rgb[i] / s;
        contrast[2-i] = return_contrast(rgb[i]);
      }

      sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx), 1, 3,
                                    contrast);

      free(contrast);

      if (sciErr.iErr) {
        printError(&sciErr, 0);
        return 0;
      }

      // Assigning the list as the Output Variable
      AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx);
      // Returning the Output Variables as arguments to the Scilab environment
      ReturnArguments(pvApiCtx);
      return 0;
    }

  else if (strcmp(pstData, "correlation") == 0) {
      vector<Mat> rgb;
      split(image, rgb);

      double *correlation = (double *)malloc(sizeof(double) * 3);
      double s;

      for (int i = 0; i < 3; i++) {
        s = cv::sum(rgb[i])[0];
        rgb[i] = rgb[i] / s;
        correlation[2-i] = return_correlation(rgb[i]);
      }

      sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx), 1, 3,
                                    correlation);

      free(correlation);

      if (sciErr.iErr) {
        printError(&sciErr, 0);
        return 0;
      }

      // Assigning the list as the Output Variable
      AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx);
      // Returning the Output Variables as arguments to the Scilab environment
      ReturnArguments(pvApiCtx);
      return 0;
    }

    else if (strcmp(pstData, "energy") == 0) {
      vector<Mat> rgb;
      split(image, rgb);

      double *energy = (double *)malloc(sizeof(double) * 3);
      double s;

      for (int i = 0; i < 3; i++) {
        s = cv::sum(rgb[i])[0];
        rgb[i] = rgb[i] / s;
        energy[2-i] = return_energy(rgb[i]);
      }

      sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx), 1, 3,
                                    energy);

      free(energy);

      if (sciErr.iErr) {
        printError(&sciErr, 0);
        return 0;
      }

      // Assigning the list as the Output Variable
      AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx);
      // Returning the Output Variables as arguments to the Scilab environment
      ReturnArguments(pvApiCtx);
      return 0;
    }

    else if (strcmp(pstData, "homogeneity") == 0) {
      vector<Mat> rgb;
      split(image, rgb);

      double *homogeneity = (double *)malloc(sizeof(double) * 3);
      double s;

      for (int i = 0; i < 3; i++) {
        s = cv::sum(rgb[i])[0];
        rgb[i] = rgb[i] / s;
        homogeneity[2-i] = return_homogeneity(rgb[i]);
      }

      sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx), 1, 3,
                                    homogeneity);

      free(homogeneity);

      if (sciErr.iErr) {
        printError(&sciErr, 0);
        return 0;
      }

      // Assigning the list as the Output Variable
      AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx);
      // Returning the Output Variables as arguments to the Scilab environment
      ReturnArguments(pvApiCtx);
      return 0;
    }
        else {
      sciprint("\nUnknown Parameter Name: %s\n", pstData);
       return 0;
    }

  } else {
    sciprint("Invalid number of channels in the image(must be 1 or 3)\n");
    return 0;
  }

  // sciprint("\n");

  //  for (int i = 0; i < new_image.rows; i++) {
  //    for (int j = 0; j < new_image.cols; j++) {
  //      sciprint("%f ", new_image.at<double>(i,j));

  //    }

  // sciprint("\n");
  //  }

  // new_image is sent to scilab as output
}
/* ==================================================================== */
}

