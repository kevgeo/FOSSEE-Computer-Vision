/********************************************************
Author: Vinay Bhat
********************************************************
Usage: return_image = bwdistgeodesic(input_image, mask)
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

  int opencv_bwdistgeodesic(char *fname, unsigned long fname_len)
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

    if (marker.type() != CV_8UC1)
    { 
      Mat tempM;
      tempM = marker.clone();
      cvtColor(tempM, marker, CV_BGR2GRAY);
    }

    if (mask.type() != CV_8UC1)
    { 
      Mat tempM;
      tempM = mask.clone();
      cvtColor(tempM, mask, CV_8UC1);
    }

    int i = 1;
    
    Mat temp0;
    Mat temp2 = marker.clone();
    Mat fin_image = marker.clone();

    do {
      temp0 = temp2.clone();
      Mat temp1, temp3;
      dilate(temp0, temp1, Mat());
      min(temp1, mask, temp2);
      temp3 = temp2 - temp0;
      temp3.convertTo(temp3, CV_32F);
      temp3 = temp3 * ((255 - i)/255.0);
      temp3.convertTo(temp3, CV_8UC1);
      max(fin_image, temp3, fin_image);
      i++;
    } while((countNonZero(temp0 != temp2) != 0) && (i != 255));


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
