/********************************************************
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
 // #include "string.h"
  #include "../common.h"

  int opencv_corner(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    char* pstData = NULL;
    char* pstData1 = NULL;
    int iRet    = 0;
    int iRet1    = 0;
    CheckInputArgument(pvApiCtx, 1, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    Mat src,src_gray;
    int thresh = 200;
    int max_thresh = 255;
    retrieveImage(src, 1);
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    Mat dst, dst_norm, drawing;
    dst = Mat::zeros( src.size(), CV_32FC1 );
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    int maxCorners = 23;
    RNG rng(12345);
    if(nbInputArgument(pvApiCtx)==1)
    {
       cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
       normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
       convertScaleAbs( dst_norm, drawing );
       for( int j = 0; j < dst_norm.rows ; j++ )
       { 
         for( int i = 0; i < dst_norm.cols; i++ )
          {
            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
               circle( drawing, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
              }
          }
        }
      }
     else if(nbInputArgument(pvApiCtx)==3)
     {
           sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
	   if(sciErr.iErr)
	   {
		printError(&sciErr, 0);
		return 0;
	   }
          if(isStringType(pvApiCtx, piAddr2))
	   {
            if(isScalar(pvApiCtx, piAddr2))
	     {
	       iRet = getAllocatedSingleString(pvApiCtx, piAddr2, &pstData);
             }
           }
         else
           {
             sciprint(" The second argument should be string  ");
             return 0;
           }
         if(strcasecmp(pstData,"Method")==0)
           {
              sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
              if (sciErr.iErr)
              {
                printError(&sciErr, 0);
                return 0;
              }
              if(isStringType(pvApiCtx, piAddr3))
	      {
                 if(isScalar(pvApiCtx, piAddr3))
	          {
	            iRet1 = getAllocatedSingleString(pvApiCtx, piAddr3, &pstData1);
                  }
             }
             else
             {
             sciprint(" The third argument should be string  ");
             return 0;
             }
             if(strcasecmp(pstData1,"Harris")==0)
             {
               cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
               normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
               convertScaleAbs( dst_norm, drawing );
               for( int j = 0; j < dst_norm.rows ; j++ )
                { 
                  for( int i = 0; i < dst_norm.cols; i++ )
                   {
                     if( (int) dst_norm.at<float>(j,i) > thresh )
                       {
                        circle( drawing, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
                       }
                   }
                 }
              }
             else  if(strcasecmp(pstData1,"MinEigenvalue")==0)
              {
                if( maxCorners < 1 ) 
                 {  
                   maxCorners = 1; 
                 }
                /// Parameters for Shi-Tomasi algorithm
                vector<Point2f> corners;
                double qualityLevel = 0.01;
                double minDistance = 10;
                int blockSize = 3;
                bool useHarrisDetector = false;
                double k = 0.04;
                /// Copy the source image
                drawing = src.clone();
                goodFeaturesToTrack( src_gray,corners,maxCorners,qualityLevel,minDistance,Mat(),blockSize,useHarrisDetector,k );
                int r = 4;
                for( int i = 0; i < corners.size(); i++ )
                 { 
                   circle( drawing, corners[i], r, Scalar(rng.uniform(0,255), rng.uniform(0,255),rng.uniform(0,255)), -1, 8, 0 );
                 }
              }
            else
              {
                  sciprint(" The third  argument must be 'Harris' or 'Mineigenvalue' ");
                  return 0;
              }
         }
        else
         {
            sciprint(" The second argument must be 'Method' ");
            return 0;
         }   
      }  
    string tempstring = type2str(drawing.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,drawing,1);
    free(checker); 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
}
