/********************************************************
Function   :applycform
Syntax     :B=applycform(image,string)
*string    : 'xyz2lab'   'lab2xyz'   'srgb2xyz'   
             'xyz2srgb'  'srgb2lab'  'lab2srgb'  
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
  #define REF_X .95047; // Observer= 2°, Illuminant= D65
  #define REF_Y 1.00000;
  #define REF_Z 1.08883;
  struct Color
{
    float R,G,B,X,Y,Z,R1,G1,B1,r,g,b;
};
Color rgb2lab(float R, float G, float B)
{
        
         float r = R/255.f; //R 0..1
         float g = G/255.f; //G 0..1
         float b = B/255.f; //B 0..1
         float eps = 216.f/24389.f;
         float k = 24389.f/27.f;
         float  fx,fy,fz;
         if (r <= 0.04045)
             r = r/12.92;
         else
             r = (float)pow((r+0.055)/1.055,2.4);
         if (g <= 0.04045)
             g = g/12.92;
         else
             g = (float)pow((g+0.055)/1.055,2.4);
         if (b <= 0.04045)
             b = b/12.92;
         else
             b = (float)pow((b+0.055)/1.055,2.4);
         float X =  0.4124*r     + 0.3576*g + 0.1805*b;
         float Y =  0.2126*r     + 0.7152 *g + 0.0722 *b;
         float Z =  0.0193*r     + 0.1192*g + 0.9505 *b;
         float xr = X/REF_X;
         float yr = Y/REF_Y;
         float zr = Z/REF_Z;
         if ( xr > eps )
            fx =  (float)pow(xr, 1/3.);
         else
            fx = (float) ((k * xr + 16.) / 116.);
         if ( yr > eps )
            fy =  (float)pow(yr, 1/3.);
         else
            fy = (float) ((k * yr + 16.) / 116.);
         if ( zr > eps )
            fz =  (float)pow(zr, 1/3.);
         else
            fz = (float) ((k * zr + 16.) / 116);
        Color rgb;
        rgb.R1= ( 116 * fy ) - 16;
        rgb.G1= 500*(fx-fy);
        rgb.B1= 200*(fy-fz);
       return rgb;

} 
Color xyz2lab(float R, float G, float B)
{
    float eps = 216.f/24389.f;
    float k = 24389.f/27.f;
    float fx,fy,fz,Ls,as,bs;
    float xr=R/REF_X;
    float yr=G/REF_Y;
    float zr=B/REF_Z;
    if ( xr > eps )
          fx =  (float)pow(xr, 1/3.);
    else
          fx = (float) ((k * xr + 16.) / 116.);
    if ( yr > eps )
          fy =  (float)pow(yr, 1/3.);
    else
          fy = (float) ((k * yr + 16.) / 116.);
    if ( zr > eps )
          fz =  (float)pow(zr, 1/3.);
    else
         fz = (float) ((k * zr + 16.) / 116);
         Ls = ( 116 * fy ) - 16;
         as = 500*(fx-fy);
         bs = 200*(fy-fz);
    Color rgb;
    rgb.R1 =Ls; 
    rgb.G1 =as;
    rgb.B1 =bs;
    return rgb;
} 
Color rgb2xyz(float R, float G, float B)
{
        
         float r = R/255.f; //R 0..1
         float g = G/255.f; //G 0..1
         float b = B/255.f; //B 0..1
         float eps = 216.f/24389.f;
         float k = 24389.f/27.f;
         float  fx,fy,fz;
         if (r <= 0.04045)
             r = r/12.92;
         else
             r = (float)pow((r+0.055)/1.055,2.4);
         if (g <= 0.04045)
             g = g/12.92;
         else
             g = (float)pow((g+0.055)/1.055,2.4);
         if (b <= 0.04045)
             b = b/12.92;
         else
             b = (float)pow((b+0.055)/1.055,2.4);
         float X1 =  0.4124*r     + 0.3576*g + 0.1805*b;
         float Y1 =  0.2126*r     + 0.7152 *g + 0.0722 *b;
         float Z1 =  0.0193*r     + 0.1192*g + 0.9505 *b;
         float xr = X1;
         float yr = Y1;
         float zr = Z1;
    Color rgb;
    rgb.R1= xr;
    rgb.G1= yr;
    rgb.B1= zr;
    return rgb;

} 
Color lab2xyz(float R, float G, float B)
{
    float y = (R+ 16) / 116;
    float x = G / 500 + y;
    float z = y - B/ 200;

    if ( pow( y , 3 ) > 0.008856 ) { y = pow( y , 3 ); }
    else { y = ( y - 16 / 116 ) / 7.787; }
    if ( pow( x , 3 ) > 0.008856 ) { x = pow( x , 3 ); }
    else { x = ( x - 16 / 116 ) / 7.787; }
    if ( pow( z , 3 ) > 0.008856 ) { z = pow( z , 3 ); }
    else { z = ( z - 16 / 116 ) / 7.787; }

    Color rgb;
    rgb.R1= x * REF_X;
    rgb.G1= y * REF_Y;
    rgb.B1= z * REF_Z;
    return rgb;


} 
Color xyz2rgb(float R, float G, float B)
{
   float x = R; 
   float y = G; 
   float z = B; 
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
         
    Color rgb;
    rgb.R1= R1;
    rgb.G1= G1;
    rgb.B1= B1;
    return rgb; 
   

} 
Color lab2rgb(float R, float G, float B)
{
   float y = (R + 16) / 116;
     float x = G / 500 + y;
     float z = y-B / 200;
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
    rgb.R1= R1;
    rgb.G1= G1;
    rgb.B1= B1;
    return rgb;
   

} 
  int opencv_applycform(char *fname, unsigned long fname_len)
  {
     // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int* piAddr2=NULL;
        int* piAddr1=NULL;
        int* piAddr3=NULL;
        int* piaddr=NULL;
        char *value=NULL;
        int* piLen		= NULL;
        char **val;
        int i,j;
        int error;
        Color out;
        double check;
        Mat img;
        float R,G,B;
        char* pstData = NULL;
        int *piAddrNew = NULL;  
        int* piAddr4=NULL;
        int iRet    = 0;
        CheckInputArgument(pvApiCtx,2,2);
        CheckOutputArgument(pvApiCtx,1,1);
        int m=0;
        retrieveImage(img,1);
        double *r1,*g1,*b1;
        r1=(double *)malloc(sizeof(double)*img.rows*img.cols);
        g1=(double *)malloc(sizeof(double)*img.rows*img.cols);
        b1=(double *)malloc(sizeof(double)*img.rows*img.cols);
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
       
        for(int i=0;i<img.rows;i++)  
          {
            for(int j=0;j<img.cols;j++)
               {
                  Vec3b intensity = img.at<Vec3b>(i,j);
                  R= intensity.val[2];
                  G= intensity.val[1];
                  B= intensity.val[0];
                  if(strcasecmp(pstData,"xyz2lab")==0)
                   {
                       out=xyz2lab(R,G,B);
                   }
                  else if(strcasecmp(pstData,"lab2xyz")==0)
                   {
                       out=lab2xyz(R,G,B);
                   } 
                   else if(strcasecmp(pstData,"srgb2xyz")==0)
                   {
                       out=rgb2xyz(R,G,B);
                   } 
                   else if(strcasecmp(pstData,"xyz2srgb")==0)
                   {
                       out=xyz2rgb(R,G,B);
                   } 
                   else if(strcasecmp(pstData,"srgb2lab")==0)
                   {
                       out=rgb2lab(R,G,B);
                   }
                   else if(strcasecmp(pstData,"lab2srgb")==0)
                   {
                       out=rgb2lab(R,G,B);
                   }
                  else
                   {
                      sciprint("Expected input argument 'xyz2lab'   'lab2xyz'   'srgb2xyz' 'xyz2srgb'  'srgb2lab'  'lab2srgb'");
                      return 0;
                   }
                  r1[m]=out.R1;
                  g1[m]=out.G1;
                  b1[m++]=out.B1;  
                  }
          }
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &piAddrNew);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, img.rows,img.cols, r1);
    free(r1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 2, img.rows, img.cols, g1);
    free(g1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 3, img.rows, img.cols, b1);
    free(b1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }  

           // free(checker);  
            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);            
            return 0;       
  }
}
