/********************************************************
Author: Dhruti Shah
********************************************************
Usage: ssim_value = ssim (image, reference)
Example:
  val = ssim(image, ref)
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

  int opencv_ssim(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
        
    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    // Get the input image from the Scilab environment
    Mat image;
    retrieveImage(image, 1);

    Mat ref;
    retrieveImage(ref, 2);

    double mean_img=0, mean_ref=0, std_img=0, std_ref=0, covar=0, mean_xy=0;

    for (int i = 0; i < image.rows; ++i)
    {
      for (int j = 0; j < image.cols; ++j)
      {
        mean_img += image.at<uchar>(i,j);
        mean_ref += ref.at<uchar>(i,j);
        mean_xy += (image.at<uchar>(i,j)*ref.at<uchar>(i,j));
      }
    }

    int no_of_pixels = image.rows * image.cols;
    mean_img = mean_img/no_of_pixels;
    mean_ref = mean_ref/no_of_pixels;
    mean_xy = mean_xy/no_of_pixels;
    covar = mean_xy - (mean_img * mean_ref);

    for (int m = 0; m < image.rows; ++m)
    {
      for (int n = 0; n < image.cols; ++n)
      {
        std_img += ( (image.at<uchar>(m,n) - mean_img) * (image.at<uchar>(m,n) - mean_img) ) ;
        std_ref += ( (ref.at<uchar>(m,n) - mean_ref) * (ref.at<uchar>(m,n) - mean_ref) ) ;
      }
    }

    std_ref = std_ref/no_of_pixels;
    std_ref = sqrt(std_ref);

    std_img = std_img/no_of_pixels;
    std_img = sqrt(std_img);

    double value;
    value = ( (2*mean_img*mean_ref) + 0.0001 ) * ( (2*covar) + 0.0009 );
    value = value / ( ((mean_img*mean_img) + (mean_ref*mean_ref) + 0.0001) * ((std_img*std_img) + (std_ref*std_ref) + 0.0009) );

    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1,value);
    if(intErr)
    {
        return intErr;
    }

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    
    return 0;

  }
/* ==================================================================== */
}