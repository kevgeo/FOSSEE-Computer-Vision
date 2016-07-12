/********************************************************
Function   :bwconvexhull
Syntax     :B=bwconvexhull(A)
            B=bwconvexhull(A,"Object",n)
     n :4 or 8
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

  int opencv_bwconvhull(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    char* pstData = NULL;
    int iRet    = 0;
    double n;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    Mat src,src_gray;
    int thresh = 100,max_thresh = 255;
    retrieveImage(src, 1);
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );
    Mat src_copy = src.clone();
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
    findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> >hull( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
     {  
       convexHull( Mat(contours[i]), hull[i], false );
     }
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar(255,255,255);
       /* if only 1 input argument line connectvity n will take default value 8 */
       if(nbInputArgument(pvApiCtx)==1)
        {
           drawContours( drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
           drawContours( drawing, hull, (int)i, color,CV_FILLED, 8, vector<Vec4i>(), 0, Point());
        }
       else if(nbInputArgument(pvApiCtx)==3)
        {
           /* retrieve second argument  */
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
         if(strcasecmp(pstData,"Object")==0)
           {
              /*retrieve third argument */
              sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
              if (sciErr.iErr)
              {
                printError(&sciErr, 0);
                return 0;
              }
             intErr = getScalarDouble(pvApiCtx, piAddr3, &n);
             if(intErr)
              {
                return intErr;
              }   
            // The error checks for the function
             if((n!=4)&&(n!=8))
              {
                sciprint("The value of line connectivity must be 8 or 4");
                return 0;
              }  
            drawContours( drawing, contours, (int)i, color, 1, n, vector<Vec4i>(), 0, Point() );
            drawContours( drawing, hull, (int)i, color,CV_FILLED, n, vector<Vec4i>(), 0, Point());
          }
         else
          {
             sciprint(" The second argument must be 'Object'");
             return 0;
          }
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
