/*  function-rgb2lab
 Author :Tess Zacharias      */
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
    float R,G,B,X,Y,Z,Ls,as,bs,r,g,b;
};
Color rgb2xyz1(float R, float G, float B)
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
    Color xyz;
    xyz.X= xr;
    xyz.Y= yr;
    xyz.Z= zr;
    return xyz;

} 
 Color rgb2xyz2(float R, float G, float B,char *s)
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
         float eps = 216.f/24389.f;
         float k = 24389.f/27.f;
         float  fx,fy,fz,X1,Y1,Z1,xr,yr,zr;
         float r = R/255.f; //R 0..1
         float g = G/255.f; //G 0..1
         float b = B/255.f; //B 0..1
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
         X1 =  0.4124*r     + 0.3576*g + 0.1805*b;
         Y1 =  0.2126*r     + 0.7152 *g + 0.0722 *b;
         Z1 =  0.0193*r     + 0.1192*g + 0.9505 *b;
         xr = X1;
         yr = Y1;
         zr = Z1;
        Color xyz;
        xyz.X= xr;
        xyz.Y= yr;
        xyz.Z= zr;
        return xyz;

}  

int opencv_rgb2xyz(char *fname, unsigned long fname_len)
    {
    // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
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
        int *piAddrNew = NULL;                      
        CheckOutputArgument(pvApiCtx,1,1);
        int m=0;
        retrieveImage(img,1);
        double *r1,*g1,*b1;
        r1=(double *)malloc(sizeof(double)*img.rows*img.cols);
        g1=(double *)malloc(sizeof(double)*img.rows*img.cols);
        b1=(double *)malloc(sizeof(double)*img.rows*img.cols);
        for(int i=0;i<img.rows;i++)  
          {
            for(int j=0;j<img.cols;j++)
               {
                  Vec3b intensity = img.at<Vec3b>(i,j);
                  R= intensity.val[2];
                  G= intensity.val[1];
                  B= intensity.val[0];
                  out=rgb2xyz1(R,G,B);
                  r1[m]=out.X;
                  g1[m]=out.Y;
                  b1[m++]=out.Z;    
                  }
          }
    //creating the list that will be passed to the Scilab enviroment
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &piAddrNew);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    //Adding the R value matrix to the list
    //Syntax : createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData)
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, img.rows,img.cols, r1);
    free(r1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Adding the G value matrix to the list
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 2, img.rows, img.cols, g1);
    free(g1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Adding the B value matrix to the list
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
