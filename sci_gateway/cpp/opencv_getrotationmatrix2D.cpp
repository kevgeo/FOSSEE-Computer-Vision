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

    Mat res = getRotationMatrix2D(center, angle, scale);

    /*int *outList = NULL;
    unsigned char *red = NULL;
    unsigned char *green = NULL;
    unsigned char *blue = NULL; 

    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &outList);
    if(sciErr.iErr)
    {
          printError(&sciErr, 0);
          return 0;
    }

    red = (unsigned char *)malloc(sizeof(unsigned char)*res.rows*res.cols);
    green = (unsigned char *)malloc(sizeof(unsigned char)*res.rows*res.cols);
    blue = (unsigned char *)malloc(sizeof(unsigned char)*res.rows*res.cols);
    
    for(int k=0;k<res.rows;k++)
    {
        for(int p=0;p<res.cols;p++)
                {
                    Vec3b intensity = res.at<Vec3b>(k, p);
                    red[k+res.rows*p]=intensity.val[2];
                    green[k+res.rows*p]=intensity.val[1];
                    blue[k+res.rows*p]=intensity.val[0];
                }
     }

     sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, res.rows, res.cols, red);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 2, res.rows, res.cols, green);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }                   
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 3, res.rows, res.cols, blue);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                free(red);
                free(green);
                free(blue); 

                */
    double *m = (double *)malloc(res.rows*res.cols*sizeof(double));
    for(i=0;i<res.rows;i++)
    {
      for(j=0;j<res.cols;j++)
      {
        uchar intensity = res.at<uchar>(i, j);
        *(m + i*res.cols + j) = intensity;
      }
    } 

      
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, res.rows, res.cols, m);

  //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0; 

  }
/* ==================================================================== */
}

