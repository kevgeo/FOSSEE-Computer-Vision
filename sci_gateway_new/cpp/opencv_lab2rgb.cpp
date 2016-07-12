/* ==================================================================== */
/* Author :Tess Zacharias                                               */
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
    float R,G,B,X,Y,Z,L,a,b;
};

Color lab2rgb1(float L, float a, float b)
{
        
    
     float y = (L + 16) / 116;
     float x = a / 500 + y;
     float z = y - b / 200;
     if ( pow( y , 3 ) > 0.008856 )
      {
             y = pow( y , 3 );
      }
     else 
      { 
             y = ( y - 16 / 116 ) / 7.787;
      }
     if ( pow( x , 3 ) > 0.008856 )
         {
             x = pow( x , 3 );
         }
     else
         { 
            x = ( x - 16 / 116 ) / 7.787;
         }
     if ( pow( z , 3 ) > 0.008856 )
         {
            z = pow( z , 3 );
         }
     else 
         {
           z = ( z - 16 / 116 ) / 7.787;
         }
     float X= x *REF_X;
     float Y= y *REF_Y;
     float Z = z *REF_Z;
     //X = X/100.f; //R 0..1
     //Y= Y/100.f; //G 0..1
     //Z= Z/100.f; //B 0..1
     float R1=3.240479*X-1.537150*Y -0.498535 *Z;
     float G1=-0.969256*X+1.875992 *Y+0.041556* Z;
     float B1=0.055648*X-0.204043*Y +1.057311*Z;
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
	B1 = 12.92 * B1;
          
           
    Color rgb;
    rgb.R= R1;
    rgb.G= G1;
    rgb.B= B1;
    return rgb;

} 
 Color lab2rgb2(float L, float a, float b,char *s)
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
     
     float y = (L + 16) / 116;
     float x = a / 500 + y;
     float z = y - b / 200;
     if ( pow( y , 3 ) > 0.008856 )
      {
             y = pow( y , 3 );
      }
     else 
      { 
             y = ( y - 16 / 116 ) / 7.787;
      }
     if ( pow( x , 3 ) > 0.008856 )
         {
             x = pow( x , 3 );
         }
     else
         { 
            x = ( x - 16 / 116 ) / 7.787;
         }
     if ( pow( z , 3 ) > 0.008856 )
         {
            z = pow( z , 3 );
         }
     else 
         {
           z = ( z - 16 / 116 ) / 7.787;
         }
     float X= x *REF_X;
     float Y= y *REF_Y;
     float Z = z *REF_Z;
     X = X/100.f; //R 0..1
     Y= Y/100.f; //G 0..1
     Z= Z/100.f; //B 0..1
     float R1=3.240479*X-1.537150*Y -0.498535 *Z;
     float G1=-0.969256*X+1.875992 *Y+0.041556* Z;
     float B1=0.055648*X-0.204043*Y +1.057311*Z;
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
	B1 = 12.92 * B1;
          
           
    Color rgb;
    rgb.R= R1*100;
    rgb.G= G1*100;
    rgb.B= B1*100;
    return rgb;

}  
  int opencv_lab2rgb(char *fname, unsigned long fname_len)
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
        float L,a,b;
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
         double *r1,*g1,*b1;
         r1=(double *)malloc(sizeof(double)*iRows*iCols);
         g1=(double *)malloc(sizeof(double)*iRows*iCols);
         b1=(double *)malloc(sizeof(double)*iRows*iCols);
                               for(int i=0;i<iRows;i++)
                               {
                                  for(int j=0;j<iCols;j++)
                                   {
                                         // Vec3d intensity = img.at<Vec3d>(i,j);
                                          L= (pstDataR[k]);
                                          a= (pstDataG[k]);
                                          b= (pstDataB[k++]);
                                          out=lab2rgb1(L,a,b);
                                            //cout<<R<<"\n";
                                            //cout<<G<<"\n";
                                            //cout<<B<<"\n";       
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


    //Adding the R value matrix to the list
    //Syntax : createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData)
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, iRows,iCols, r1);
    free(r1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Adding the G value matrix to the list
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 2, iRows, iCols, g1);
    free(g1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Adding the B value matrix to the list
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
