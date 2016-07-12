/********************************************************
Function   :impixel
Syntax     :C=impixel(A,[1 2 3],[1 2 3])
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
  int opencv_impixel(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int intErr = 0;
    Mat image;
    int* piAddr2=NULL;
    int* piAddr3=NULL;
    double *value1= 0;
    double *value2= 0;
    int iRows1	= 0;
    int iCols1	= 0;
    int iRows2	= 0;
    int iCols2	= 0;
    double n1,n2;
    CheckInputArgument(pvApiCtx, 3,3);
    CheckOutputArgument(pvApiCtx, 1, 1);
    retrieveImage(image,1);
    //retrieve coordinates
    sciErr =getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if(sciErr.iErr)   
     {
       printError(&sciErr, 0);
       return 0;
     }      
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows1, &iCols1,&value1);
    if(sciErr.iErr)
     {
       printError(&sciErr, 0);
       return 0;
     }  
    sciErr =getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if(sciErr.iErr)   
     {
       printError(&sciErr, 0);
       return 0;
     }      
    sciErr= getMatrixOfDouble(pvApiCtx, piAddr3, &iRows2, &iCols2,&value2);
     if(sciErr.iErr)   
     {
       printError(&sciErr, 0);
       return 0;
     }      
    if((iRows1!=iRows2)||(iCols1!=iCols2))
     {
       sciprint("\n Give proper number of coordinates");
       return 0;
     }
    for(int i=0;i<iRows1;i++)
    {
      for(int j=0;j<iCols1;j++)
       {
         n1=value1[i + iRows1 * j];
         if(n1!=round(n1)||n1<=0)
         {
            sciprint("The value of coordinates must be integer\n");
            return 0;
         }
       }
     }  
    for(int i=0;i<iRows2;i++)
    {
      for(int j=0;j<iCols2;j++)
       {
         n2=value2[i + iRows2 * j];
         if(n2!=round(n2)||n2<=0)
         {
            sciprint("The value of coordinates must be integer\n");
            return 0;
         }
       }
     }  
     int m=0;
    double* output=NULL;
    output=( double *)malloc(sizeof(double)*iCols1*3);
    for(int i=0;i<iRows1;i++)
     {
       for(int j=0;j<iCols1;j++)
         {
            Vec3b intensity = image.at<Vec3b>(value1[j-1],value2[j-1]);
            float R= intensity.val[2];
            float G= intensity.val[1];
            float B= intensity.val[0];  
            output[m]=R;
            output[m+1]=G;
            output[m+2]=B; 
            m=m+3; 
          }
     } 
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1,3,iCols1,output);
    free(output); // Data have been copied into Scilab memory
    if (sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }              
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;    
  }
}
