/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* overloaded function                                                  */
/* Syntax : return_matrix_3_xyz_values=xyz2lab(matrix_of_3_labvalues a)  */
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
  #define REF_X .9504; // Observer= 2°, Illuminant= D65
  #define REF_Y 1.00000;
  #define REF_Z 1.08883;

    struct Color
    {
    float R,G,B,X,Y,Z,L,a,b;
    };

    Color xyz2lab3(float X,float Y, float Z)
    {
    float eps = 216.f/24389.f;
    float k = 24389.f/27.f;
    float fx,fy,fz,Ls,as,bs;
    float xr=X/REF_X;
    float yr=Y/REF_Y;
    float zr=Z/REF_Z;
    if (xr > eps)
          fx =  (float)pow(xr, 1/3.);
    else
          fx = (float) ((k * xr + 16.) / 116.);
    if (yr > eps)
          fy =  (float)pow(yr, 1/3.);
    else
          fy = (float) ((k * yr + 16.) / 116.);
    if (zr > eps)
          fz =  (float)pow(zr, 1/3.);
    else
         fz = (float) ((k * zr + 16.) / 116);
         Ls = ( 116 * fy ) - 16;
         as = 500*(fx-fy);
         bs = 200*(fy-fz); 
    Color lab;
    lab.L =Ls; 
    lab.a =as;
    lab.b =bs;
    return lab;
    }
 
 
    int xyz2lab(char *fname, unsigned long fname_len)
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
        double *pstDataR = NULL;
        double *pstDataG = NULL;
        double *pstDataB = NULL; 
        int* piLen= NULL;
        char **val;
        int i,j;
        int error;
        Color out;
        double check;
        Mat img,dst;
        float R,G,B;
        int *piAddrNew = NULL;
        double *r,*g,*b; 
        int k=0;                     
        //checking output argument
        CheckOutputArgument(pvApiCtx,1,1);
        //retrive address of the list       
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        if(sciErr.iErr)   
        {
          printError(&sciErr, 0);
          return 0;
        }
        //retrive the matrix of the R values   
        sciErr =  getMatrixOfDoubleInList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
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
        //dynamically allocating memory to varibles
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
                 out=xyz2lab3(R,G,B);      
                 r[j*iRows+i]=(out.L);
                 g[j*iRows+i]=(out.a);
                 b[j*iRows+i]=(out.b);  
              }
        }
                      
        sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &piAddrNew);
        if(sciErr.iErr)
        {
           printError(&sciErr, 0);
           return 0;
        }
        //Adding the R value matrix to the list to be returned
        sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, iRows,iCols, r);
        free(r);
        if(sciErr.iErr)
        {
           printError(&sciErr, 0);
           return 0;
        }
        //Adding the G value matrix to the list to be returned
        sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 2, iRows, iCols, g);
        free(g);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        //Adding the B value matrix to the list to be returned
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
