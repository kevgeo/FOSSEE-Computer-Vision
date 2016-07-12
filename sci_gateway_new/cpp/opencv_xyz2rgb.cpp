/* ==================================================================== */
/* Author :Tess Zacharias                                              */
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
    float X,Y,Z,R,G,B;
};

Color xyz2rgb1(float X, float Y, float Z)
{
        
         float x = X; 
         float y = Y; 
         float z = Z; 
         float R1 = 3.2404542*x-1.5371385*y-0.4985314*z;
         float G1 =-0.9692660*x+1.8760108*y+0.0415560 *z;
         float B1 = 0.0556434*x-0.2040259*y+1.0572252*z;
         if( R1 > 0.0031308 )
		R1 =  1.055 * ( pow( R1, 1./2.4 ) ) - 0.055;
	 else
		R1 = 12.92 * R1;
         if( G1 > 0.0031308 )
		G1 =  1.055 * ( pow( G1, 1./2.4 ) ) - 0.055;
	 else
		G1 = 12.92 * G1;
         if( B1 > 0.0031308 )
		B1 =  1.055 * ( pow( B1, 1./2.4 ) ) - 0.055;
	 else
		B1= 12.92 * B1;
         
          /*R1 = R1*REF_X;
         G1 = G1*REF_Y;
         B1 = B1*REF_Z; */ 
         /* R1 = R1 * 255;
          G1 = G1 * 255;
          B1 = B1 * 255;*/
    Color rgb;
    rgb.R= R1;
    rgb.G= G1;
    rgb.B= B1;
    return rgb;

} 
 Color xyz2rgb2(float X, float Y, float Z,char *s)
{
    double REF_X1;
    double REF_Y1;
    double REF_Z1;
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
         float x = X; 
         float y = Y; 
         float z = Z;
         float R1 = 3.2406*x-1.5372*y-0.4986*z;
         float G1 =-0.9689*x+1.8758*y+0.0415 *z;
         float B1 = 0.0557*x-0.2040*y+1.0570*z;
         if( R1 > 0.0031308 )
		R1 =  1.055 * ( pow( R1, 1./2.4 ) ) - 0.055;
	 else
		R1 = 12.92 * R1;
         if( G1 > 0.0031308 )
		G1 =  1.055 * ( pow( G1, 1./2.4 ) ) - 0.055;
	 else
		G1 = 12.92 * G1;
         if( B1 > 0.0031308 )
		B1 =  1.055 * ( pow( B1, 1./2.4 ) ) - 0.055;
	 else
		B1= 12.92 * B1;
         
          /*R1 = R1*REF_X;
         G1 = G1*REF_Y;
         B1 = B1*REF_Z;*/ 
        /* R1 = R1 * 255;
         G1 = G1 * 255;
         B1 = B1 * 255; */
    Color rgb;
    rgb.R= R1;
    rgb.G= G1;
    rgb.B= B1;
    return rgb;
         

}  
  int opencv_xyz2rgb(char *fname, unsigned long fname_len)
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
        float X,Y,Z;
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
                                          X= (pstDataR[k]);
                                          Y= (pstDataG[k]);
                                          Z= (pstDataB[k++]);
                                          out=xyz2rgb1(X,Y,Z);
                                            //cout<<R<<"\n";
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
