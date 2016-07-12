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
  #include "../common.h"

  int opencv_watershed(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    Mat src;
    retrieveImage(src, 1);
    // Create binary image from source image
    Mat bw;
    cvtColor(src, bw, CV_BGR2GRAY);
    threshold(bw, bw, 40, 255, CV_THRESH_BINARY);
    // Perform the distance transform algorithm
    Mat dist;
    distanceTransform(bw, dist, CV_DIST_L2, 3);
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1., cv::NORM_MINMAX);
    //imshow("dist", dist);
    // Threshold to obtain the peaks 
    // This will be the markers for the foreground objects
    threshold(dist, dist, .5, 1., CV_THRESH_BINARY);
    //imshow("dist2", dist);
    // Create the CV_8U version of the distance image
    // It is needed for cv::findContours()
    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);
    // Find total markers
    vector<vector<Point> > contours;
    findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int ncomp = contours.size();
    // Create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(dist.size(), CV_32SC1);
    // Draw the foreground markers
    for (int i = 0; i < ncomp; i++)
		drawContours(markers, contours, i, Scalar::all(i+1), -1);
    // Draw the background marker
    circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
    //imshow("markers", markers*10000);
    // Perform the watershed algorithm
    watershed(src, markers);
    // Generate random colors
    vector<Vec3b> colors;
    for (int i = 0; i < ncomp; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);

		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}
     // Create the result image
     Mat dst = Mat::zeros(markers.size(), CV_8UC3);
     // Fill labeled objects with random colors
     for (int i = 0; i < markers.rows; i++)
	{
	   for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i,j);
			if (index > 0 && index <= ncomp)
				dst.at<cv::Vec3b>(i,j) = colors[index-1];
			else
				dst.at<cv::Vec3b>(i,j) = Vec3b(0,0,0);
		}
	}

	//imshow("dst", dst);
    

    string tempstring = type2str(dst.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,dst,1);
    free(checker); 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
}
