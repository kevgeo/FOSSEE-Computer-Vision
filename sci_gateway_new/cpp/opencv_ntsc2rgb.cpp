/* ==================================================================== */
/* Author  :Tess Zacharias                                              */
/* ==================================================================== */
/* overloaded function                                                  */
/* Syntax : return_matrix_3_xyz_values=lab2xyz(matrix_of_3_labvalues a)  */
/* Syntax : return_matrix_3_xyz_values=lab2xyz(matrix_of_3_labvalues a,String1 Name,String2 value)  */
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
 // #include "../common.h"
  #define REF_X .95047; // Observer= 2°, Illuminant= D65
  #define REF_Y 1.00000;
  #define REF_Z 1.08883;

struct Color
{
    float R,G,B,Y,I,Q;
};


Color ntsc2rgb1(float Y, float I, float Q)
{
        
        // float y=Y/255.0;
        // float i=I/255.0;
        // float q=Q/255.0;
         float r= 1.000*Y+0.956*I+0.621*Q;
         float g =1.000*Y-0.272*I-0.647*Q;
         float b =1.000*Y-1.106*I+1.703*Q;
         
            
    Color rgb;
    rgb.R= r;
    rgb.G= g;
    rgb.B= b;
    return rgb;

} 
 int opencv_ntsc2rgb(char *fname, unsigned long fname_len)
    {
    // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
        int* piAddr=NULL;
        int* piAddr1=NULL;
        int* piAddr3=NULL;
        double *value=NULL;
        int* piLen		= NULL;
        char **val;
        int i,j;
        int error;
        Color out;
        double check;
        Mat img,dst;
        float Y,I,Q;
        int *piAddrNew = NULL;                      
         //checking output argument
          CheckOutputArgument(pvApiCtx,1,1);
         //checking if number of input arguments are 3
        
         int k=0;
        // SciErr getMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal)
                    double *pstDataR = NULL;
                    double *pstDataG = NULL;
                    double *pstDataB = NULL; 
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
         int m=0;
         double *r,*g,*b;
         r=(double *)malloc(sizeof(double)*iRows*iCols);
         g=(double *)malloc(sizeof(double)*iRows*iCols);
         b=(double *)malloc(sizeof(double)*iRows*iCols);
                               for(int i=0;i<iRows;i++)
                               {
                                  for(int j=0;j<iCols;j++)
                                   {
                                         // Vec3d intensity = img.at<Vec3d>(i,j);
                                          Y= (pstDataR[k]);
                                          I= (pstDataG[k]);
                                          Q= (pstDataB[k++]);
                                          out=ntsc2rgb1(Y,I,Q);
                                           // cout<<R<<"\n";
                                            //cout<<G<<"\n";
                                            //cout<<B<<"\n";       
                                            r[m]=(out.R);
                                            g[m]=(out.G);
                                            b[m++]=(out.B);  
                                   }
                               }
                     
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &piAddrNew);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    //Adding the R value matrix to the list
    //Syntax : createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData)
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

 
