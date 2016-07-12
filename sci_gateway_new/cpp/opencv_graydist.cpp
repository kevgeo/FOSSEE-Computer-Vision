/********************************************************
Author: Vinay Bhat
********************************************************
Usage: return_image = graydist(input_image, mask)
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <limits>
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

  int opencv_graydist(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddr = NULL;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    // Get the input image from the Scilab environment
    Mat marker, mask;
    retrieveImage(marker, 1);
    retrieveImage(mask, 2);
    
    // do conversion of marker to gray
    // do conversion of mask to gray

    vector<Point> sources;
    Mat distances(mask.size(), CV_32F);
    Mat sptSet = Mat::zeros(mask.size(), CV_8UC1);
    for (i = 0; i < mask.cols; i++)
    {
      for (j = 0; j < mask.rows; j++)
      {
        if (mask.at<uchar>(i,j)) {
          sources.push_back(Point(i,j));
          distances.at<float>(i,j) = 0.0;
        }
        else
          distances.at<float>(i,j) = FLT_MAX;
      }
    }

    for (c = 0; c < mask.cols * mask.rows; c++)
    {
      float l = minDistance(distances, sptSet);

    }


    string tempstring = type2str(fin_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, fin_image, 1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
