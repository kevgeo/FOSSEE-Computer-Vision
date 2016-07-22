/*********************************************************************************
*Author : Kevin George
*
*-> To execute, epipolarlines(I1,I2)
*   where I1.1 & I2 are images
*
* This will display the matched points and the epipolar lines.
*********************************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <stdio.h>
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
    //# include "../common.cpp"

    int opencv_epipolarlines(char *fname, unsigned long fname_len)
    {
    	SciErr sciErr;
    	int intErr=0;

        //-> Mat containers for images
        Mat img_1;
        Mat img_2;

        //-> Address of Various Arguments
        int *piAddr = NULL;

        //-> Local variables
        int *outList = NULL;
        unsigned char *red = NULL;
        unsigned char *green = NULL;
        unsigned char *blue = NULL; 

        int *outList2 = NULL;
        unsigned char *red2 = NULL;
        unsigned char *green2 = NULL;
        unsigned char *blue2 = NULL; 

        //-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        CheckInputArgument(pvApiCtx, 2, 2);                     //Check on Number of Input Arguments
        CheckOutputArgument(pvApiCtx, 2, 2);                    //Check on Number of Output Arguments

    	//-> Read Image
        retrieveImage( img_1, 1);
        retrieveImage(img_2, 2);

        //-> Count number of input arguments
        //num_InputArgs = *getNbInputArgument(pvApiCtx);

        //-> Based on number of input arguments

//*****************************************************  Actual Processing  *************************************************************

        //Mat img_1 = imread( "left.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    	//Mat img_2 = imread( "right.jpg", CV_LOAD_IMAGE_GRAYSCALE );

        cvtColor(img_1, img_1, CV_BGR2GRAY);
        cvtColor(img_2, img_2,CV_BGR2GRAY);

    	//-- Step 1: Detect the keypoints using SURF Detector
    	int minHessian = 400;

    	SurfFeatureDetector detector( minHessian );

    	std::vector<KeyPoint> keypoints_1, keypoints_2;

    	detector.detect( img_1, keypoints_1 );
    	detector.detect( img_2, keypoints_2 );

    	//-- Step 2: Calculate descriptors (feature vectors)
    	SurfDescriptorExtractor extractor;

    	Mat descriptors_1, descriptors_2;

      	extractor.compute( img_1, keypoints_1, descriptors_1 );
      	extractor.compute( img_2, keypoints_2, descriptors_2 );

    	//-- Step 3: Matching descriptor vectors using FLANN matcher
    	FlannBasedMatcher matcher;
    	std::vector< DMatch > matches;
    	matcher.match( descriptors_1, descriptors_2, matches );

    	double max_dist = 0; double min_dist = 100;
    
    	//-- Quick calculation of max and min distances between keypoints
	    for( int i = 0; i < descriptors_1.rows; i++ )
	    { double dist = matches[i].distance;
	      if( dist < min_dist ) min_dist = dist;
	      if( dist > max_dist ) max_dist = dist;
	    }

	    std::vector< DMatch > good_matches;
	    vector<Point2f> points1,points2;
    
	    for( int i = 0; i < descriptors_1.rows; i++ )
	    { //if( matches[i].distance <= max(2*min_dist, 0.02) )
	    	if( matches[i].distance <= max(2*min_dist, 0.2) )
	      { 
	      	good_matches.push_back( matches[i]);
	      	points2.push_back( keypoints_2[matches[i].trainIdx].pt );
	      	points1.push_back( keypoints_1[matches[i].queryIdx].pt ); 
	      }

	    }

  //***************** Finding Fundamental Matrix **************************

	  vector<uchar> states;
	  Mat f = findFundamentalMat(points1, points2, FM_LMEDS, 3, 0.99, states);
	  //Mat F = findFundamentalMat(points1, points2, FM_LMEDS, 3, 0.99, states);
	  string window = "epipolarlines";
	  //drawEpipolarLines(window, fundamental_matrix, img_1, img_2, points1, points2);
	 

	  vector<Vec<float,3> > epilines1, epilines2;
	  computeCorrespondEpilines(points1, 1, f, epilines1); //Index starts with 1
	  computeCorrespondEpilines(points2, 2, f, epilines2);

	  cvtColor(img_1,img_1,COLOR_GRAY2BGR);
	  cvtColor(img_2,img_2,COLOR_GRAY2BGR);
	  int inlierDistance = -1;
	 // cout<<points1.size()<<endl<<points2.size( );
	  cv::RNG rng(0);
	  for(int i=0; i<points1.size(); i++)
	  {
	    /*if(inlierDistance > 0)
	    {
	      if(distancePointLine(points1[i], epilines2[i]) > inlierDistance ||
	        distancePointLine(points2[i], epilines1[i]) > inlierDistance)
	      {
	        //The point match is no inlier
	        continue;
	      }
	    } */

	    //Mat outImg(img1.rows, img1.cols*2, CV_8UC3);
	    //Rect rect1(0,0, img1.cols, img1.rows);

//************************************************************* Draw epipolar lines function ********************************************************
	     cv::Scalar color(rng(256),rng(256),rng(256));
	     line(img_1, Point(0,-epilines1[i][2]/epilines1[i][1]), Point(img_1.cols,-(epilines1[i][2]+epilines1[i][0]*img_1.cols)/epilines1[i][1]),color);
	     circle(img_1, points1[i], 3, color, -1, CV_AA);

	     line(img_2, Point(0,-epilines1[i][2]/epilines1[i][1]), Point(img_2.cols,-(epilines1[i][2]+epilines1[i][0]*img_2.cols)/epilines1[i][1]),color);
	     circle(img_2, points2[i], 3, color, -1, CV_AA);
	  } 

//************************************************ Returning first image **************************************************

        /*
	  	if( img_1.channels() == 1)
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &outList);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            red = (unsigned char *)malloc(sizeof(unsigned char)*img_1.rows*img_1.cols);

            for(int k=0;k<img_1.rows;k++)
                for(int p=0;p<img_1.cols;p++)
                    red[k+img_1.rows*p]=img_1.at<uchar>(k, p);

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, img_1.rows, img_1.cols, red);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }                       
            free(red);
        }

        else
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &outList);
            if(sciErr.iErr)
            {
                    printError(&sciErr, 0);
                    return 0;
            }

            red = (unsigned char *)malloc(sizeof(unsigned char)*img_1.rows*img_1.cols);
            green = (unsigned char *)malloc(sizeof(unsigned char)*img_1.rows*img_1.cols);
            blue = (unsigned char *)malloc(sizeof(unsigned char)*img_1.rows*img_1.cols);

            for(int k=0;k<img_1.rows;k++)
            {
                for(int p=0;p<img_1.cols;p++)
                {
                    Vec3b intensity = img_1.at<Vec3b>(k, p);
                    red[k+img_1.rows*p]=intensity.val[2];
                    green[k+img_1.rows*p]=intensity.val[1];
                    blue[k+img_1.rows*p]=intensity.val[0];
                }
            }

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, img_1.rows, img_1.cols, red);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 2, img_1.rows, img_1.cols, green);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }                   
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 3, img_1.rows, img_1.cols, blue);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                free(red);
                free(green);
                free(blue); 

        }

        */
		  /*imshow("title", img_1);
		  waitKey(0);
		  imshow("title2", img_2);
		  waitKey(0);*/

          /*
		  if( img_2.channels() == 1)
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, 1, &outList2);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            red2 = (unsigned char *)malloc(sizeof(unsigned char)*img_2.rows*img_2.cols);

            for(int k=0;k<img_2.rows;k++)
                for(int p=0;p<img_2.cols;p++)
                    red2[k+img_2.rows*p]=img_2.at<uchar>(k, p);

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, outList2, 1, img_2.rows, img_2.cols, red2);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }                       
            free(red2);
        }

        else
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, 3, &outList2);
            if(sciErr.iErr)
            {
                    printError(&sciErr, 0);
                    return 0;
            }

            red2 = (unsigned char *)malloc(sizeof(unsigned char)*img_2.rows*img_2.cols);
            green2 = (unsigned char *)malloc(sizeof(unsigned char)*img_2.rows*img_2.cols);
            blue2 = (unsigned char *)malloc(sizeof(unsigned char)*img_2.rows*img_2.cols);

            for(int k=0;k<img_2.rows;k++)
            {
                for(int p=0;p<img_2.cols;p++)
                {
                    Vec3b intensity2 = img_2.at<Vec3b>(k, p);
                    red2[k+img_2.rows*p]=intensity2.val[2];
                    green2[k+img_2.rows*p]=intensity2.val[1];
                    blue2[k+img_2.rows*p]=intensity2.val[0];
                }
            }

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, outList2, 1, img_2.rows, img_2.cols, red2);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, outList2, 2, img_2.rows, img_2.cols, green2);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }                   
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, outList2, 3, img_2.rows, img_2.cols, blue2);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                free(red2);
                free(green2);
                free(blue2); 

        }
        */
        //-> Returning first image
        string tempstring = type2str(img_1.type());
        char *checker;
        checker = (char *)malloc(tempstring.size() + 1);
        memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
        returnImage(checker, img_1, 1);
        free(checker);

        //-> Returning second image
        string tempstring2 = type2str(img_2.type());
        char *checker2;
        checker2 = (char *)malloc(tempstring2.size() + 1);
        memcpy(checker2, tempstring2.c_str(), tempstring2.size() + 1);
        returnImage(checker2, img_2, 2);
        free(checker2);

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
        ReturnArguments(pvApiCtx);
    	return 0;
    }

}