/********************************************************
    Author: Dhruti Shah
*********************************************************
  void imattributes(image)
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
  #include "sciprint.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "../common.h"
   
  int opencv_imattributes(char *fname, unsigned long fname_len)
  {

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1,1) ;


     Mat image;
     retrieveImage(image,1);

     int row  = image.rows;
     int column  = image.cols;
     int type = image.type();
     string r;
      uchar depth = type & CV_MAT_DEPTH_MASK;
      uchar chans = 1 + (type >> CV_CN_SHIFT);

      switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
      }

      r += "C";
      r += (chans+'0');

    
     int flag_binary=1;
    
     for(int i =0; i<row ; i++)
      {
        for(int j=0; j<column; j++)
        {
          if(image.at<uchar>(i,j)!=0 || image.at<uchar>(i,j)!=255)
          {
            flag_binary=0;
            break;
          }
        }
        if (flag_binary=0)
          break;
      }

      string image_type;
      //uchar chans = 1 + (type >> CV_CN_SHIFT);
      if (flag_binary==1)
        image_type = "Binary";
      else if (chans==1)
        image_type= "Intensity";
      else if (chans==3)
        image_type= "Truecolor";

      char image_type1[100];
      strcpy(image_type1, image_type.c_str());

      char image_class[100];
      strcpy(image_class, r.c_str());             

      sciprint("Width (columns) \t%d\n",column);
      sciprint("Height (rows) \t%d\n",row);
      sciprint("Class \t%s\n",image_class);
      sciprint("Image Type \t%s\n",image_type1);

      if(flag_binary!=1 && chans==1)
      {
        int min_intensity=255,max_intensity=0;
        for(int i=0; i<row; i++)
        {
          for(int j=0; j<column; j++)
          {
            if(image.at<uchar>(i,j)<min_intensity)
              min_intensity=image.at<uchar>(i,j);
            if (image.at<uchar>(i,j)>max_intensity)
              max_intensity=image.at<uchar>(i,j);
          }
        }
        
        sciprint("Min intensity \t%d\n",min_intensity);
        sciprint("Max intensity \t%d\n",max_intensity);
      }
      
      return 0;              
}
/* ==================================================================== */
}
