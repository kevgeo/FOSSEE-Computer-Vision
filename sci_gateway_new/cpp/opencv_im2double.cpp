/********************************************************
Author: Sukul Bagai
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
  #include "../common.h"
  
  int opencv_im2double(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int i,j,k;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    Mat image;
    retrieveImage(image, 1); 

    Mat img_dbprec;
    image.convertTo(img_dbprec, CV_64FC3,1.0/255.0); /// Create Image with Double Precision

    string tempstring = type2str(img_dbprec.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,img_dbprec,1);
    free(checker); 

    

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
