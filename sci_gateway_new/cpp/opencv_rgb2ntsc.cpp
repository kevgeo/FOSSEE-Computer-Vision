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
    float R,G,B,Y,I,Q;
};
Color rgb2ntsc(float R, float G, float B)
{
        
         float r=R/255.0;
         float g=G/255.0;
         float b=B/255.0;
         float y =  0.299*r     + 0.587*g + 0.114*b;
         float i =  0.596*r     -0.274 *g - 0.322 *b;
         float q =  0.211*r     - 0.523*g + 0.312 *b;
         
            
    Color ntsc;
    ntsc.Y= y;
    ntsc.I= i;
    ntsc.Q= q;
    return ntsc;

} 

int opencv_rgb2ntsc(char *fname, unsigned long fname_len)
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
                  out=rgb2ntsc(R,G,B);
                  r1[m]=out.Y;
                  g1[m]=out.I;
                  b1[m++]=out.Q;  
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
