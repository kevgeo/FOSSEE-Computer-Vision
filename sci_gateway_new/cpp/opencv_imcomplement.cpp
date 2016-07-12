/********************************************************
Function   :imcomplement
Syntax     :B=imcomplement(A)
Author     : Tess  Zacharias
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include "string.h"
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
  int opencv_imcomplement(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    Mat A,B,C;
    int* piAddr2=NULL;
    int iType = 0;
    int iRet    = 0;
    double value= 0;
    CheckInputArgument(pvApiCtx, 1,1);
    CheckOutputArgument(pvApiCtx, 1, 1);
    retrieveImage(A,1);
    if(A.channels()==3)
    {
       for(int x=0;x< A.rows; x++)  
       {  
         for ( int y=0;y<A.cols;y++)  
          {  
            for(int i=0;i<3;i++)
                A.at<Vec3b>(x,y)[i]=255-A.at<Vec3b>(x,y)[i];
           }
      }
    }
   else
    {
       for(int x=0;x<A.rows;x++)  
       {  
         for ( int y=0; y< A.cols; y++)  
         {  
            A.at<uchar>(x,y)=255-A.at<uchar>(x,y);
         }
       }
    }
    
    int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(A.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,A,1);
    free(checker); 
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
  }
}
