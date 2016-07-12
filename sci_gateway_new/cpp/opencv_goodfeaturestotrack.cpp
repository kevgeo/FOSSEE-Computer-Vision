/********************************************************
    Author: Abhilasha Sancheti & Sukul Bagai
*********************************************************
   corner_points = goodFeaturesToTrack( input_image, maxCorners, qualityLevel, minDistance, blockSize, useHarrisDetector((1 for true)/(0 for false)),k );
********************************************************/


// Abhilasha Sancheti
// sample input :   
//output: (image , corners)
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>
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
  
  int opencv_goodfeaturestotrack(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    
    int i,j, detector;
    double maxCorners  ,qualityLevel,minDistance,blocksize=3 ,k=0.04, usedetector;
    bool useHarrisDetector = false;
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 7, 7);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    Mat image;
    retrieveImage(image,1);
    
    //for maximum corners
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2 ,&maxCorners);
    if(intErr)
        return intErr;
     
     
    //for qualityLevel
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3,&qualityLevel);
   if(intErr)
        return intErr;

   //for minDistance
     sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4 ,&minDistance);
    if(intErr)
       return intErr;
   

   //for blocksize
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5 ,&blocksize);
   if(intErr)
     return intErr;

    //for Harrisusedetector taking integer 1 or 0 
      sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6,&usedetector);
    if(intErr)
        return intErr;
     detector=(int)usedetector;
   
  //for k value
  sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr7 ,&k);
   if(intErr)
        return intErr;
     

     //checking the input parameter usedetector
    if (detector == 1)
      useHarrisDetector = true;
    else if(detector == 0)
      useHarrisDetector = false;
    else
    {
      sciprint("Either 1 or 0 , 0 value was used instead");
      useHarrisDetector = false;
    }

    /// processing of the input image and other parameters
 if( maxCorners < 1 ) { maxCorners = 1; sciprint("maxcorners cannot be less than 1 , using 1 instead");}

  /// Parameters for Shi-Tomasi algorithm
  
  vector<Point2f> corners;
  /// Copy the source image
  Mat src_gray;
  cvtColor( image, src_gray, CV_BGR2GRAY );
  /// Apply corner detection
  goodFeaturesToTrack( src_gray,corners,maxCorners,qualityLevel,minDistance,Mat(),blocksize,useHarrisDetector,k );
  
  int row = corners.size();
  double *cor = (double *)malloc(2*row*sizeof(double *));
  for (int i=0;i<row;i++)
  {
      *(cor + i*2 + 0)=corners[i].x;
      *(cor + i*2 + 1)=corners[i].y;
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
/* ==================================================================== */
}
