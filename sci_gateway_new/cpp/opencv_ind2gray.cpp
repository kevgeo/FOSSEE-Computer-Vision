/********************************************************
Author: Vinay

Function: ind2gray(image, colormap)
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

  int opencv_ind2gray(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int cRows=0,cCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image, imgcpy;
    retrieveImage(image, 1);
    string tempstring = type2str(image.type());
    char *imtype;
    imtype = (char *)malloc(tempstring.size() + 1);
    memcpy(imtype, tempstring.c_str(), tempstring.size() + 1);
    bool integer = true;
    int scale = 1;
    double error = 0;

    if (strcmp(imtype,"8U")==0) {
        integer = true;
        scale = 255;
        error = 0.5;
    }
    else if (strcmp(imtype,"16U")==0) {
        integer = true;
        scale = 65535;
        error = 0.5;
    }
    else if (strcmp(imtype,"32F")==0 || strcmp(imtype,"64F")==0) {
        integer = false;
        scale = 1;
        error = 0;
    }
    else {
        sciprint("Invalid image");
        return 0;
    }
    iRows = image.rows;
    iCols = image.cols;
    image.convertTo(imgcpy, CV_64F);

    Mat cmap, cmapcpy;
    retrieveImage(cmap, 2);
    cRows = cmap.rows;
    cCols = cmap.cols;
    cmap.convertTo(cmapcpy, CV_64F);

    for (int i=0; i<cRows; i++) {
        for (int j=0; j<cCols; j++) {
            if (cmapcpy.at<double>(i,j)<0 || cmapcpy.at<double>(i,j)>1) {
                sciprint("Invalid colormap");
                return 0;
            }
        }
    }

    Mat gray = Mat::zeros(image.size(), CV_64F);


    for (int i=0; i<iRows; i++) {
        for (int j=0; j<iCols; j++) {
            unsigned int temp = (unsigned int)imgcpy.at<double>(i, j);
            if (temp >= cRows) {
                temp = cRows - 1;
            }
            if (!integer) {
                if (temp!=0) {temp-=1;}
            }
            gray.at<double>(i,j) = (0.2989 * cmapcpy.at<double>(temp, 0) + 0.5870 * cmapcpy.at<double>(temp, 1) + 0.1140 * cmapcpy.at<double>(temp, 2)) ;

        }
    }

    Mat grayimage;
    gray.convertTo(grayimage, image.type(), scale, error);
    returnImage(imtype,grayimage,1);
    free(imtype);


    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);            
    return 0;

  }
/* ==================================================================== */
}
