/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : return_mean=mean(sourceimage)                               */
/* ==================================================================== */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <sciprint.h>
using namespace cv;
using namespace std;
extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>
#include<string.h>
#include "../common.h"
int mean1(char *fname, unsigned long fname_len)
{

     // Error management variable
        SciErr sciErr;
     // Variables declaration //
        int t,i,j,k;
        Scalar mean11;
    //function to check number of input variable
        CheckInputArgument(pvApiCtx, 1, 1);
    //function to check number of output variable
        CheckOutputArgument(pvApiCtx, 1, 1);
    // get Address of first input 
        Mat image;
        retrieveImage(image,1);
    //block of code for calculating meanimage
        mean11=mean(image,noArray());
        double m=0;
        int count=0;
        for(int i=0;i<4;i++)
         {
              if(mean11.val[i]!=0)
                {
                   m=m+mean11.val[i];
                   count++;
                }
      
         }
         m=m/count;
         int error;
         error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,m);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }  
  ////////// Return the output arguments to the Scilab engine //////////
         AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
         ReturnArguments(pvApiCtx);
         return 0;
}
/* ==================================================================== */
}
