/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : return_n*n_matrix=dctmtx(double n)  */

/* ==================================================================== */
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
  #define PI 3.14

    int dctmtx(char *fname, unsigned long fname_len)
    {
    // Error management variable
    SciErr sciErr;
    //variable info
    int iRows=0;
    int iCols=0;
    int piRows=0;
    int piCols=0;
    int* piAddr2=NULL;
    int* piAddr1=NULL;
    int* piAddr3=NULL;
    double *value=NULL;
    int* piLen=NULL;
    char **val;
    int i,j;
    int error;
    double n;
    Mat img,dst;                     
    //checking input and output argument
    CheckInputArgument(pvApiCtx,1,1);
    CheckOutputArgument(pvApiCtx,1,1);
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //this function will fetch value of first argument
    error=getScalarDouble(pvApiCtx,piAddr1,&n) ;  
    if(error!=0)
    {
         sciprint("error in retrieving second argument");
    } 
    int k=0;
    double *mat;
    int s=n;
    mat=(double *)malloc(sizeof(double)*n*n);
    for(int i=0;i<n;i++)
    {
         for(int j=0;j<n;j++)
           {
              if(i==0)
              {
                  mat[i+s*j]=(1/(sqrt(n)));
              }
              else 
              {
                  double temp,temp1;
                  temp1=2/n;
                  temp=(2*j+1)*i*PI;
                  temp=temp/(2*n);
                  temp=(sqrt(temp1))*(cos(temp));
                  mat[i+ s*j]= temp;      
               }
            }
    }        
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1,n,n,mat);
    free(mat);
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
