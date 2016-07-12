/********************************************************
    Author: Abhilasha Sancheti 
*********************************************************
   pts = ellipse2Poly(double center_x,double center_y,double width,double height,int angle, int arcStart, int arcEnd, int delta)
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
  
  int opencv_ellipse2poly(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr1 = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    int *piAddr8  = NULL;
    double *pstData = NULL;
    int i,j,k ;
    double center_x,center_y,width,height,arcstart,arcend,delta,angle;
     
   

    //checking input argument
    CheckInputArgument(pvApiCtx, 8, 8);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

//// to retreive the value of x coordinate of center    

     sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr1 ,&center_x);
    if(intErr)
      return intErr;
     
    
    
//// to retreive the value of y coordinate of center
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2,&center_y);
    if(intErr)
       return intErr;

//// to retreive the value of width
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3,&width);
    if(intErr)
       return intErr;

//// to retreive the value of height
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4,&height);
    if(intErr)
       return intErr;

//// to retreive the value of angle parameter
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5,&angle);
    if(intErr)
       return intErr;

//// to retreive the value of arcstart parameter
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6,&arcstart);
    if(intErr)
       return intErr;

//// to retreive the value of arcend parameter
    sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr7,&arcend);
    if(intErr)
       return intErr;

//// to retreive the value of delta parameter
    sciErr = getVarAddressFromPosition(pvApiCtx,8,&piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr8,&delta);
    if(intErr)
       return intErr;

   
    vector<Point> pts;
    Point center(center_x,center_y);
    Size axes(width,height);
    ellipse2Poly(center, axes, angle,  arcstart, arcend,delta, pts);
   
    int row = pts.size();
    double *cor = (double *)malloc(row*2*sizeof(double));
    for (int i=0;i<row;i++)
    {
        *(cor + i*2 +0)=pts[i].x;
     // cout<<pts[i].x<<" "<<pts[i].y;
        *(cor + i*2 +1)=pts[i].y;
    }
    
  sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, 2, cor);
  if(sciErr.iErr)
  {
      printError(&sciErr, 0);
      return 0;
  }
    
  
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;


 }

}