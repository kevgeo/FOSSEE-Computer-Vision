/*********************************************************************************
*Author : Kevin George
*
*-> To execute, estimateGeometricTransform(I1,I2)
*   where I1 & I2 are images
* 
*This program only gives an affine transformation matrix 
*********************************************************************************/
#include <numeric>
#include <string.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include <math.h>
#include <vector>

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
    // # include "../common.cpp"

    int opencv_estimateGeometricTransform(char *fname, unsigned long fname_len)
    {

    	//-> Error Management variables
    	SciErr sciErr;
    	int intErr=0;

        //-> Mat containers for images
        Mat image_1;
        Mat image_2;

        //-> Address of Various Arguments
        int *piAddr = NULL;

        //-> Local variables
        int minHessian = 400;  //-> For SURF algorithm
        int num_InputArgs; 
        //-> KeyPoint is data structure to store a point feature
        //   detected by a keypoint detector(OpenCV) 
            vector<KeyPoint> keypoints_1;
            vector<KeyPoint> keypoints_2;
        Mat descriptors_1, descriptors_2;
        Mat img_matches;
        vector< DMatch > good_matches;
        vector< DMatch > matches;
        vector<Point2f> a,b;
        double max_dist = 0; double min_dist = 100;
        /*double *indexPairs = NULL;
        double *matchMetric = NULL;
        double size=1;
        int iRows, iCols; */
    
        //KeyPoint *X = new KeyPoint; //-> Remember to free this Kevin

        //-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        CheckInputArgument(pvApiCtx, 1, 5);                     //Check on Number of Input Arguments
        CheckOutputArgument(pvApiCtx, 1, 5);                    //Check on Number of Output Arguments

    	//-> Read Image
        retrieveImage( image_1, 1);
        retrieveImage(image_2, 2);

        //-> Count number of input arguments
        //num_InputArgs = *getNbInputArgument(pvApiCtx);

        //-> Based on number of input arguments

//*****************************************************  Actual Processing  *************************************************************
      
        //-- Step 1: Calculate keypoints
        SurfFeatureDetector detector( minHessian );
        detector.detect( image_1, keypoints_1 );
        detector.detect( image_2, keypoints_2 );

        //-- Step 2: Calculate descriptors (feature vectors)
        SurfDescriptorExtractor extractor;
        extractor.compute( image_1, keypoints_1, descriptors_1 );
        extractor.compute( image_2, keypoints_2, descriptors_2 );

        //-- Step 3: Matching descriptor vectors using FLANN matcher
        FlannBasedMatcher matcher;
        matcher.match( descriptors_1, descriptors_2, matches );

        //-- Quick calculation of max and min distances between keypoints
        for( int i = 0; i < descriptors_1.rows; i++ )
        { 
            double dist = matches[i].distance;
            if( dist < min_dist ) min_dist = dist;
            if( dist > max_dist ) max_dist = dist;
        }

        for( int i = 0; i < descriptors_1.rows; i++ )
        { 
            if( matches[i].distance <= max(2*min_dist, 0.02) )
            { good_matches.push_back( matches[i]); }
        }

        //queryidx- keypoints1
  		//traindidx-  keypoints2
  	

	    for(int i=0;i<3;i++)
	    {
	      a.push_back( keypoints_1[good_matches[i].queryIdx].pt );
	      b.push_back( keypoints_2[good_matches[i].trainIdx].pt );
	    }

  	
  	   Mat M = getAffineTransform( Mat(a), Mat(b)  );
       
  	 /*  double output[M.rows][M.cols];

        for(int i=0; i<M.rows; ++i)
       {
          const double* Mi = M.ptr<double>(i);
          for (int j = 0; j < M.cols; j++)
          {
             output[i][j] = Mi[j];
          }
       } */
        double *output = NULL;
        output = (double *)malloc(sizeof(double)*M.rows*M.cols);

  	   //-> Accessing elements of Mat object M
       for(int i=0; i<M.rows; ++i)
   	   {
          const double* Mi = M.ptr<double>(i);
	      for (int j = 0; j < M.cols; j++)
	      {
	         output[i+j] = Mi[j];
	      }
  	   }
        

  	   sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, M.rows, M.cols, output); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }

       //-> Returning Output
       AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
       ReturnArguments(pvApiCtx);

       return 0;
    }

}