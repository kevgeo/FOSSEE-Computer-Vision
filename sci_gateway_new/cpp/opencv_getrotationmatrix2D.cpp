/********************************************************
    Author: Shubheksha Jalan
*********************************************************
Mat getRotationMatrix2D(Point2f center, double angle, double scale)
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
  
  int opencv_getrotationmatrix2D(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k;
    double centerX, centerY, angle, scale;

     //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 1, 1);


     //for x coordinate of center
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr, &centerX);
    if(intErr)
    {
        return intErr;
    }   

    //for y coordinate of center
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr2, &centerY);
     if(intErr)
    {
        return intErr;
    }   


    //for value of angle
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr3, &angle);
    if(intErr)
    {
        return intErr;
    }   

     //for value of scale
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr4, &scale);
    if(intErr)
    {
        return intErr;
    }   

     Point2f center = Point2f(centerX, centerY);

    Mat temp = getRotationMatrix2D(center, angle, scale);

    double *m = (double *)malloc(temp.rows*temp.cols*sizeof(double));
    for(i=0;i<temp.rows;i++)
    {
      for(j=0;j<temp.cols;j++)
      {
        uchar intensity = temp.at<uchar>(i, j);
        *(m + i*temp.cols + j) = intensity;
      }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, temp.rows, temp.cols, m);

  //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}

