/********************************************************
Function   :imcontour
Syntax     :B=imcontour(A)
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

  int opencv_imcontour(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    Mat gray,image;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    retrieveImage(image, 1);
    cvtColor(image, gray, CV_BGR2GRAY);
    Canny(gray, gray, 100, 200, 3);
    /// Find contours   
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);
    findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Draw contours
    Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
    } 
    bitwise_not(drawing,drawing);    
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
