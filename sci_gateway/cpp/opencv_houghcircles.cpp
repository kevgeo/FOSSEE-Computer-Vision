/********************************************************
Authors: Sukul Bagai, Abhilasha Sancheti
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
  
  int opencv_houghcircles(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    int i,j,k;
    double *dp =NULL , *mindist = NULL ,*param1= NULL ,*param2=NULL;
    double *minradius =NULL, *maxradius = NULL;

    //checking input argument
    CheckInputArgument(pvApiCtx, 7, 7);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    
    Mat src;
    retrieveImage(src,1);
    
   // for dp parameter of houghCircles function
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols ,&dp);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

 // for minDist parameter of houghCircles function
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows, &iCols ,&mindist);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

     // for param1 parameter of houghCircles function 
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols ,&param1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

      // for param2 parameter of houghCircles function 
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &iRows, &iCols ,&param2);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }  
  // for minradius parameter of houghCircles function 
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr6, &iRows, &iCols ,&minradius);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 
     // for maxradius parameter of houghCircles function 
    sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr7, &iRows, &iCols ,&maxradius);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }  
  
    Mat src_gray;
    cvtColor( src, src_gray, CV_BGR2GRAY );
   
    /// Reduce the noise so we avoid false circle detection
    

    vector<Vec3f> circles;

    /// Apply the Hough Transform to find the circles
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, dp[0], mindist[0], param1[0], param2[0], minradius[0], maxradius[0] );
    int rows = circles.size();
    cout<<circles.size();
    double *output = (double *)malloc(3*rows*sizeof(double *));

    /// Draw the circles detected
    for(int i=0;i<rows;i++)
    {
        *(output + i*3 + 0) = circles[i][0];
        *(output + i*3 + 1) = circles[i][1];
        *(output + i*3 + 2) = circles[i][2];
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, rows, 3, output);
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
 
