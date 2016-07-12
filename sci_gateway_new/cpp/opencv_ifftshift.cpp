/********************************************************
Function   :ifftshift
Syntax     :B=ifftshift(A)
Author     : Tess  Zacharias
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include "string.h"
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
  int opencv_ifftshift(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    Mat image;
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    retrieveImage(image,1);
    int cx = floor(image.cols/2);
    int cy = floor(image.rows/2);
    Mat q0(image, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(image, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(image, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(image, Rect(cx, cy, cx, cy)); // Bottom-Right
    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);   
    int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,image,1);
    free(checker); 
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
  }
}
