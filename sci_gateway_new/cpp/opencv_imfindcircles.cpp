/********************************************************
Function   :imfindcircles
Syntax     :B=imfindcircles(A)
Author: Tess  Zacharias

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

  int opencv_imfindcircles(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddrNew  = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    Mat src_gray,image;
    double Rmin,Rmax;
    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 2) ;
    retrieveImage(image, 1);
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &Rmin);
    if(intErr)
    {
        return intErr;
    }   
    if(Rmin!= round(Rmin) || Rmin<=0)
    {
        sciprint("The value of minium Radius must be an integer\n");
        return 0;
    }  
    if(Rmin<5)
    {
        sciprint("The value of minium Radius too small\n");
        return 0;
    }  
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &Rmax);
    if(intErr)
    {
        return intErr;
    } 
    if(Rmax!= round(Rmax) || Rmax<=0)
    {
        sciprint("The value of maximum Radius must be an integer\n");
        return 0;
    }  
    cvtColor(image,src_gray, CV_BGR2GRAY);
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
    vector<Vec3f> circles;
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 2, 10, 200, 100,Rmin,Rmax);
    int k=circles.size();
    double *radius=NULL;
    radius=( double *)malloc(sizeof(double)*k);
    double *c1=NULL;
    c1=( double *)malloc(sizeof(double)*k);
    double *c2=NULL;
    c2=( double *)malloc(sizeof(double)*k);
    for( size_t i= 0; i < circles.size(); i++ )
    {
      c1[i]=cvRound(circles[i][0]);
      c2[i]=cvRound(circles[i][1]);
      radius[i]=cvRound(circles[i][2]);
    }
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 2, &piAddrNew);
    if(sciErr.iErr)
     {
        printError(&sciErr, 0);
        return 0;
     }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piAddrNew, 1, 1, k, c1);
    free(c1); 
    if(sciErr.iErr)
     {
        printError(&sciErr, 0);
        return 0;
     } 
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piAddrNew, 2, 1, k, c2);
    free(c2); 
    if(sciErr.iErr)
     {
        printError(&sciErr, 0);
        return 0;
     } 
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1; 
    if(Lhs==2)
     {   
        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) +2,1,k,radius);
        free(radius);
        if (sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }  
       AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
     }
    ReturnArguments(pvApiCtx);
    return 0;         

  }
}
