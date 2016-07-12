/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* overloaded function                                                  */
/* Syntax : return_matrix_3_xyz_values=lab2xyz(matrix_of_3_labvalues a) */
/* ==================================================================== */
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
  #define REF_X 95.047; // Observer= 2°, Illuminant= D65 
  #define REF_Y 100.000;
  #define REF_Z 108.883;

    struct Color
    {
     float R,G,B,X,Y,Z,L,a,b;
    };
      
    Color lab2xyz1(float l, float a, float b)
    {
    float y = (l + 16) / 116;
    float x = a / 500 + y;
    float z = y - b / 200;
    if(pow(y,3) > 0.008856) 
    { 
       y = pow(y,3); 
    }
    else 
    {
       y =(y-16/116)/7.787; 
    }
    if (pow(x,3) > 0.008856)
    {
       x = pow(x,3);
    }
    else
    { 
       x = (x-16/116)/7.787;
    }
    if (pow(z,3) > 0.008856)
    {
       z = pow(z,3); 
    }
    else
    { 
       z =(z-16/ 116 ) / 7.787; 
    }

    Color xyz;
    xyz.X= x * REF_X;
    xyz.Y = y * REF_Y;
    xyz.Z = z * REF_Z;
    return xyz;
 
    }
 
    Color lab2xyz2(float l, float a, float b,char *s)
    {
    double REF_X1;
    double REF_Y1;
    double REF_Z1;
    //if else condition for checking various input arguments and assign values accordingly
    if(!strcmp(s,"a"))
    {
          REF_X1=1.0985; 
          REF_Y1= 1.0000;
          REF_Z1=0.3558;
    }
    else if(!strcmp(s,"c"))
    {
          REF_X1=0.9807; 
          REF_Y1= 1.0000;
          REF_Z1=1.1822;
    }
    else if(!strcmp(s,"d50"))
    {
          REF_X1=0.9642; 
          REF_Y1= 1.0000;
          REF_Z1=0.8251;
    }
    else if(!strcmp(s,"d65"))
    {
          REF_X1=0.9504;
          REF_Y1= 1.0000;
          REF_Z1= 1.0888;
    }
    else if(!strcmp(s,"icc"))
    {
          REF_X1=0.962; 
          REF_Y1=1.000;
          REF_Z1= 0.8249;
    }
     else if(!strcmp(s,"e"))
    {
          REF_X1=1.000;  
          REF_Y1=1.000;
          REF_Z1=1.000;
    }
    float y = (l + 16) / 116;
    float x = a / 500 + y;
    float z = y - b / 200;
    if (pow(y,3) > 0.008856)
    {
      y=pow(y,3);
    }
    else 
    { 
      y=(y-16/116)/7.787; 
    }
    if(pow(x,3) > 0.008856) 
    {
      x=pow(x,3); 
    }
    else 
    { 
      x=(x-16/116)/7.787; 
    }
    if (pow(z,3) > 0.008856)
    {
      z=pow(z,3); 
    }
    else 
    {
      z=(z-16/116)/7.787; 
    }
    Color xyz;
    xyz.X= x * REF_X1;
    xyz.Y = y * REF_Y1;
    xyz.Z = z * REF_Z1;
    return xyz;
    }  
    int lab2xyz(char *fname, unsigned long fname_len)
    {
    // Error management variable
    SciErr sciErr;
    //variable info
    int iRows=0;
    int iCols=0;
    int piRows=0;
    int piCols=0;
    int* piAddr=NULL;
    int* piAddr1=NULL;
    int* piAddr3=NULL;
    double *value=NULL;
    int* piLen=NULL;
    char **val;
    int i,j;
    int error;
    Color out;
    double check;
    Mat img,dst;
    float R,G,B;
    int k=0;
    int *piAddrNew = NULL; 
    double *pstDataR=NULL;
    double *pstDataG=NULL;
    double *pstDataB=NULL; 
    double *r,*g,*b;
    int m=0;                     
    //checking output argument
    CheckOutputArgument(pvApiCtx,1,1);
    //retrive address of the list
    sciErr=getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    //retrive the matrix of the R values   
    sciErr=getMatrixOfDoubleInList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //retrive address of the list
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }   
    //retrive the matrix of the G values 
    sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 2, &iRows, &iCols, &pstDataG);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //retrive address of the list
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }   
    //retrive the matrix of the B values 
    sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 3, &iRows, &iCols, &pstDataB);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }         
    r=(double *)malloc(sizeof(double)*iRows*iCols);
    g=(double *)malloc(sizeof(double)*iRows*iCols);
    b=(double *)malloc(sizeof(double)*iRows*iCols);
    for(int i=0;i<iRows;i++)
    {
       for(int j=0;j<iCols;j++)
        {
           R=(pstDataR[k]);
           G=(pstDataG[k]);
           B= (pstDataB[k++]);
           out=lab2xyz1(R,G,B);      
           r[m]=(out.X)/100;
           g[m]=(out.Y)/100;
           b[m++]=(out.Z)/100;  
         }
     }                     
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &piAddrNew);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //Adding the R value matrix to the list
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, iRows,iCols, r);
    free(r);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //Adding the G value matrix to the list
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 2, iRows, iCols, g);
    free(g);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //Adding the B value matrix to the list
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 3, iRows, iCols, b);
    free(b);
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
