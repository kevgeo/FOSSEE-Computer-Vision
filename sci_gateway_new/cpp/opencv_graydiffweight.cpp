/********************************************************
Authors: Dhruti Shah 
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
  #include <sciprint.h>
  #include "../common.h"
  int opencv_graydiffweight(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k;
    double refGrayVal;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;


    Mat image;
    retrieveImage(image, 1); 
    cvtColor( image, image, CV_BGR2GRAY );

    //for value of refGrayVal
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
   intErr = getScalarDouble(pvApiCtx, piAddr2, &refGrayVal);
     if(intErr)
    {
        return intErr;
    }   

    double value1, value2;
    Mat new_image = Mat::zeros(image.size(),image.type());
    for(int i=0; i<image.rows; i++)
    {
        for(int j=0; j<image.cols; j++)
        {
            value1 = image.at<uchar>(i,j);
            value2 = abs(value1-refGrayVal);
            new_image.at<uchar>(i,j)=value2;
        }
    }

    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_image,1);
    free(checker); 

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}

