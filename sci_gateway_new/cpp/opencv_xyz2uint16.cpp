/*--------------------------------------  
 function-lab2uint16
 Author :Tess Zacharias    
------------------------------------- */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include<stdio.h>
#include <sciprint.h>
#include<string.h>
using namespace cv;
using namespace std;
extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "../common.h"
  struct Color
  {
    float R,G,B,X,Y,Z;
  };
 Color xyz2uint16(float X, float Y, float Z)
 {
   /* xyz to uint16 conversion  */
  float R1=round((65535 * (X / (1 + (32767/32768))))/2);
  float G1=round((65535 * (Y / (1 + (32767/32768))))/2);
  float B1=round((65535 * (Z / (1 + (32767/32768))))/2);
  
  Color rgb;
  rgb.R = R1;
  rgb.G = G1;
  rgb.B = B1;
    return rgb;
 }
 int opencv_xyz2uint16(char *fname, unsigned long fname_len)
 {
        SciErr sciErr;
  	int iRows= 0;
	int iCols= 0;
        int* piAddr=NULL;
        int* piAddr1=NULL;
        int* piAddr3=NULL;
        int i,j;
        int error;
        Color out;
        float X,Y,Z;
        int *piAddrNew = NULL;                      
        CheckOutputArgument(pvApiCtx,1,1);
        int k=0;
        double *pstDataR = NULL;
        double *pstDataG = NULL;
        double *pstDataB = NULL; 
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        if(sciErr.iErr)   
          {
               printError(&sciErr, 0);
               return 0;
          }
        sciErr =  getMatrixOfDoubleInList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
        if(sciErr.iErr)
          {
               printError(&sciErr, 0);
               return 0;
          }
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        if(sciErr.iErr)   
          {
               printError(&sciErr, 0);
               return 0;
          }   
        sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 2, &iRows, &iCols, &pstDataG);
        if(sciErr.iErr)
          {
               printError(&sciErr, 0);
               return 0;
          }
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        if(sciErr.iErr)   
          {
               printError(&sciErr, 0);
               return 0;
          }   
         sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 3, &iRows, &iCols, &pstDataB);
         if(sciErr.iErr)
          {
               printError(&sciErr, 0);
               return 0;
          }
         int m=0;
         unsigned short *r1,*g1,*b1;
         r1=(unsigned short *)malloc(sizeof(unsigned short )*iRows*iCols);
         g1=(unsigned short  *)malloc(sizeof(unsigned short )*iRows*iCols);
         b1=(unsigned short  *)malloc(sizeof(unsigned short )*iRows*iCols);
         for(int i=0;i<iRows;i++)
           {
             for(int j=0;j<iCols;j++)
              {
                   X= (pstDataR[k]);
                   Y= (pstDataG[k]);
                   Z= (pstDataB[k++]);
                   out=xyz2uint16(X,Y,Z);
                   r1[m]=(out.R);
                   g1[m]=(out.G);
                   b1[m++]=(out.B);  
                }
            }
                     
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &piAddrNew);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, iRows,iCols, r1);
    free(r1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 2, iRows, iCols, g1);
    free(g1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 3, iRows, iCols, b1);
    free(b1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

              
             AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);            
            return 0;                
}
}
  
