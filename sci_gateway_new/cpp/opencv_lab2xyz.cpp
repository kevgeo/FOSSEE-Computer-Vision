/*--------------------------------------  
 function-lab2xyz
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
  struct Color
  {
    float X,Y,Z,L,a,b;
  };
  Color lab2xyz1(float L, float a, float b)
{
    float y = (L + 16) / 116;
    float x = a / 500 + y;
    float z = y - b / 200;

    if ( pow( y , 3 ) > 0.008856 ) { y = pow( y , 3 ); }
    else { y = ( y - 16 / 116 ) / 7.787; }
    if ( pow( x , 3 ) > 0.008856 ) { x = pow( x , 3 ); }
    else { x = ( x - 16 / 116 ) / 7.787; }
    if ( pow( z , 3 ) > 0.008856 ) { z = pow( z , 3 ); }
    else { z = ( z - 16 / 116 ) / 7.787; }

    Color xyz;
    xyz.X= x * REF_X;
    xyz.Y = y * REF_Y;
    xyz.Z = z * REF_Z;
    return xyz;


} 
Color lab2xyz2(float L, float a, float b,char *s)
{
    double REF_X1;
    double REF_Y1;
    double REF_Z1;
    if(!strcasecmp(s,"a"))
    {
          REF_X1=1.0985; 
          REF_Y1= 1.0000;
          REF_Z1=0.3558;
    }
    else if(!strcasecmp(s,"c"))
    {
             REF_X1=0.9807; 
          REF_Y1= 1.0000;
          REF_Z1=1.1822;
    }
    else if(!strcasecmp(s,"d50"))
    {
           REF_X1=0.9642; 
          REF_Y1= 1.0000;
          REF_Z1=0.8251;
    }
     else if(!strcasecmp(s,"d65"))
    {
            REF_X1=0.9504;
          REF_Y1= 1.0000;
          REF_Z1= 1.0888;
    }
    else if(!strcasecmp(s,"icc"))
    {
           REF_X1=0.962; 
          REF_Y1=1.000;
          REF_Z1= 0.8249;
    }
     else if(!strcasecmp(s,"d55"))
    {
          REF_X1=0.962; 
          REF_Y1=1.000;
          REF_Z1= 0.8249;
    }
    float y = (L + 16) / 116;
    float x = a / 500 + y;
    float z = y - b / 200;

    if ( pow( y , 3 ) > 0.008856 ) { y = pow( y , 3 ); }
    else { y = ( y - 16 / 116 ) / 7.787; }
    if ( pow( x , 3 ) > 0.008856 ) { x = pow( x , 3 ); }
    else { x = ( x - 16 / 116 ) / 7.787; }
    if ( pow( z , 3 ) > 0.008856 ) { z = pow( z , 3 ); }
    else { z = ( z - 16 / 116 ) / 7.787; }

    Color xyz;
    xyz.X= x * REF_X1;
    xyz.Y = y * REF_Y1;
    xyz.Z = z * REF_Z1;
    return xyz;
}  
Color lab2xyz3(float L, float a, float b,char *s)
{
    double REF_X1;
    double REF_Y1;
    double REF_Z1;
    if(!strcasecmp(s,"sRGB"))
    {
          REF_X1=0.9504;
          REF_Y1= 1.0000;
          REF_Z1= 1.0888;
    }
    else if(!strcasecmp(s,"adobe-rgb-1998"))
    {
          REF_X1=0.9504;
          REF_Y1= 1.0000;
          REF_Z1= 1.0888; 
    }
    float y = (L + 16) / 116;
    float x = a / 500 + y;
    float z = y - b / 200;

    if ( pow( y , 3 ) > 0.008856 ) { y = pow( y , 3 ); }
    else { y = ( y - 16 / 116 ) / 7.787; }
    if ( pow( x , 3 ) > 0.008856 ) { x = pow( x , 3 ); }
    else { x = ( x - 16 / 116 ) / 7.787; }
    if ( pow( z , 3 ) > 0.008856 ) { z = pow( z , 3 ); }
    else { z = ( z - 16 / 116 ) / 7.787; }

    Color xyz;
    xyz.X= x * REF_X1;
    xyz.Y = y * REF_Y1;
    xyz.Z = z * REF_Z1;
    return xyz;
}  
int opencv_lab2xyz(char *fname, unsigned long fname_len)
{
   SciErr sciErr;
   int* piAddr1=NULL;
   int* piAddr2=NULL;
   int* piAddr3=NULL;
   int* piAddrNew=NULL;
   int iRows		= 0;
   int iCols		= 0;
   char* pstData = NULL;
   char* pstData1 = NULL;
   CheckInputArgument(pvApiCtx,1,3);
   CheckOutputArgument(pvApiCtx,1,1);
   int m=0;
   int iType = 0;
   double *value=NULL;
   int iPrec= 0;
   Mat img,k;
   int iRet    = 0;
   int iRet1    = 0;
   Color out;
   float L,a,b;
   sciErr =getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
   sciErr = getVarType(pvApiCtx, piAddr1, &iType);
   if(nbInputArgument(pvApiCtx)==3)
   {  
      /*retrieve second argument  */
      sciErr = getVarAddressFromPosition(pvApiCtx, 2,&piAddr2);
      if(sciErr.iErr)
      {
         printError(&sciErr, 0);
	 return 0;
      }               
     if(isStringType(pvApiCtx, piAddr2))
      {
	if(isScalar(pvApiCtx, piAddr2))
         {
	   iRet = getAllocatedSingleString(pvApiCtx, piAddr2, &pstData);
         }
      }
      else
      {
        sciprint(" The second argument should be string  ");
        return 0;
      }
      if((strcasecmp(pstData,"whitepoint")!=0)&&(strcasecmp(pstData,"Colorspace")!=0))    
       {
          sciprint("\n Second argument should be any of these string :'whitepoint','Colorspace'");
          return 0;
       }
      /*retrieve third argument  */
      sciErr = getVarAddressFromPosition(pvApiCtx, 3,&piAddr3);
      if(sciErr.iErr)
      {
          printError(&sciErr, 0);
	  return 0;
      }               
      if(isStringType(pvApiCtx, piAddr3))
      {
         if(isScalar(pvApiCtx, piAddr2))
           {
	     iRet1 = getAllocatedSingleString(pvApiCtx, piAddr3, &pstData1);
           }
       }
       else
       {
          sciprint(" The third argument should be string  ");
          return 0;
       }
       if((strcasecmp(pstData,"whitepoint")==0))
       {
         if((strcasecmp(pstData1,"d65")!=0)&&(strcasecmp(pstData1,"d50")!=0)&&(strcasecmp(pstData1,"d55")!=0)&&(strcasecmp(pstData1,"icc")!=0)&&(strcasecmp(pstData1,"icc")!=0)&&(strcasecmp(pstData1,"a")!=0)&&(strcasecmp(pstData1,"c")!=0))
           {
              sciprint("\nIf Second argument is 'whitepoint' then Third argument should be any of these  string :'a','d65','d55','d50','icc','c'");
                return 0;
            }
         }
       if((strcasecmp(pstData,"Colorspace")==0))
       {
         if((strcasecmp(pstData1,"sRGB")!=0)&&(strcasecmp(pstData1,"adobe-rgb-1998")!=0))
           {
              sciprint("\nIf Second argument is 'Colorspace' then Third argument should be any of these  string :'sRGB','adobe-rgb-1998'");
                return 0;
           }
       }    
    }  
    switch(iType)
    {
      case sci_matrix:
      {
        /* retrieve matrix values  */
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows, &iCols,&value);
        if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }
         double *output=NULL;
        output=( double *)malloc(sizeof(double)*iRows*iCols);
        int j=0;
        if(nbInputArgument(pvApiCtx)==1)
        {
          for(int i=0;i<iRows;i++)
          {
            out=lab2xyz1(value[j],value[j+1],value[j+2]);
            output[j]=out.X;
            output[j+1]=out.Y;
            output[j+2]=out.Z;
            j=j+3;
          }
        }
        else if(nbInputArgument(pvApiCtx)==3)
        {
          for(int i=0;i<iRows;i++)
          {
            if(!strcasecmp(pstData1,"d65")||!strcasecmp(pstData1,"d50")||!strcasecmp(pstData1,"d55")||!strcasecmp(pstData1,"icc")||!strcasecmp(pstData1,"icc")||!strcasecmp(pstData1,"a")||!strcasecmp(pstData1,"c"))
            {
               out=lab2xyz2(value[j],value[j+1],value[j+2],pstData1);
            }
            else if(!strcasecmp(pstData1,"sRGB")||!strcasecmp(pstData1,"adobe-rgb-1998"))
            {
               out=lab2xyz3(value[j],value[j+1],value[j+2],pstData1);
            }
            output[j]=out.X;
            output[j+1]=out.Y;
            output[j+2]=out.Z;
            j=j+3;
          }
        }
       sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1,iRows,iCols,output);
       free(output);
       if (sciErr.iErr)
           {
                printError(&sciErr, 0);
                return 0;
           }         
        break;
       }
      case sci_list:
       {
        /*retrieve image values as list  */
        int k=0;
        double *pstDataR = NULL;
        double *pstDataG = NULL;
        double *pstDataB = NULL; 
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
        if(sciErr.iErr)   
        {
           printError(&sciErr, 0);
           return 0;
        }
        sciErr =  getMatrixOfDoubleInList(pvApiCtx, piAddr1, 1, &iRows, &iCols, &pstDataR);
        if(sciErr.iErr)
        {
           printError(&sciErr, 0);
           return 0;
        }
       sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
       if(sciErr.iErr)   
       {
           printError(&sciErr, 0);
           return 0;
       }   
       sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr1, 2, &iRows, &iCols, &pstDataG);
       if(sciErr.iErr)
       {
           printError(&sciErr, 0);
           return 0;
       }
       sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
       if(sciErr.iErr)   
       {
           printError(&sciErr, 0);
           return 0;
       }   
       sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr1, 3, &iRows, &iCols, &pstDataB);
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
                  if(nbInputArgument(pvApiCtx)==1)
                  {
                    out=lab2xyz1(L,a,b);
                  }
                  if(nbInputArgument(pvApiCtx)==3)
                  {
                   if(!strcasecmp(pstData1,"d65")||!strcasecmp(pstData1,"d50")||!strcasecmp(pstData1,"d55")||!strcasecmp(pstData1,"icc")||!strcasecmp(pstData1,"icc")||!strcasecmp(pstData1,"a")||!strcasecmp(pstData1,"c"))
                    {
                     out=lab2xyz2(L,a,b,pstData1);
                    }
                   else if(!strcasecmp(pstData1,"sRGB")||!strcasecmp(pstData1,"adobe-rgb-1998"))
                    {
                     out=lab2xyz3(L,a,b,pstData1);
                    }
                  }
                  r1[m]=out.X;
                  g1[m]=out.Y;
                  b1[m++]=out.Z;  
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
     break;
     }
    default:
        sciprint("input argument should be either of double matrix or image !\n"); // Should never happen
  }
 
      
     AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
     ReturnArguments(pvApiCtx);            
     return 0;                
 }
}  
