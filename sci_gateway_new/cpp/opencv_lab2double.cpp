/*--------------------------------------  
 function-lab2double
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
  #define REF_X .95047; // Observer= 2°, Illuminant= D65
  #define REF_Y 1.00000;
  #define REF_Z 1.08883;
 int opencv_lab2double(char *fname, unsigned long fname_len)
 {
        SciErr sciErr;
    	int iRows		= 0;
	int iCols		= 0;
        int* piAddr=NULL;
        int* piAddr1=NULL;
        int* piAddr3=NULL;
        int i,j;
        int error;
        float L,a,b;
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
         double *r1,*g1,*b1;
         r1=(double *)malloc(sizeof(double)*iRows*iCols);
         g1=(double *)malloc(sizeof(double)*iRows*iCols);
         b1=(double *)malloc(sizeof(double)*iRows*iCols);
                               for(int i=0;i<iRows;i++)
                               {
                                  for(int j=0;j<iCols;j++)
                                   {
                                          L= (pstDataR[k]);
                                          a= (pstDataG[k]);
                                          b= (pstDataB[k++]);
                                          r1[m]=L;
                                          g1[m]=a;
                                          b1[m++]=b;  
                                   }
                               }
                     
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &piAddrNew);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, iRows,iCols, r1);
    free(r1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 2, iRows, iCols, g1);
    free(g1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 3, iRows, iCols, b1);
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
  
