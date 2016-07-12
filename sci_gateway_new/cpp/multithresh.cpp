/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : return_matrix_of_threshold_levels=multithresh(InputArray src,int level)  */
/* ==================================================================== */
/*The function return various threshold levels:*/
/* ==================================================================== */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include<stdio.h>
#include <sciprint.h>
using namespace cv;
using namespace std;


extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "../common.h"
 
    //global variables
    float **p,**s,**h;
    float **as,**ds;
    float fast(int n,int i,int j,int index[])
    {
        int k,index1;
        float s;
        float max = INT_MIN;
        if(i>256)
        {
          return  INT_MIN;
        }  
        if(n==0)
          return h[i][j];
        if(ds[i][n]!=-1.000000)
          return ds[i][n];
        for(k=i;k<=256;k++)
        {
           s=h[i][k]+fast(n-1,k+1,j,index); 
           if(s>max)
           {
               max=s;
               as[i][n]=k;
               index[n]=k;
           
           }
        }
        return ds[i][n]=max;
     }
 
    int multithresh(char *fname, unsigned long fname_len)
    {
        // Error management variable
        SciErr sciErr;
        //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
        int* piAddr2=NULL            ;
        double thresh;
        float C;
        int i,j;
        int error;
        //checking input argument 
        CheckInputArgument(pvApiCtx,2,2);
        //checking output argument
        CheckOutputArgument(pvApiCtx,1,1);
        //for first argument 
        Mat img;
        retrieveImage(img,1);
        //second argument
        sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
        if (sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }
        //this function will fetch value of fifth argument
        error=getScalarDouble(pvApiCtx,piAddr2,&thresh) ;  
        if(error!=0)
        {
           sciprint("error in retrieving second argument");
        }      
        //code for histogram calculation    
        Mat imageh; 
        cvtColor(img, imageh, CV_BGR2GRAY);
        // number of bins
        int histSize[1] = {256}; 
        // min andax pixel value
        float hranges[2] = {0, 256}; 
        const float* ranges[1] = {hranges};
        int channels[1] = {0}; // only 1 channel used
        cv::MatND hist;
        calcHist(&imageh, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
        //code for ostu method for histogram calculation
        float *hi;
        hi=(float *)malloc(sizeof(float)*257);
        float nPixels = imageh.total();
        for(i=1;i<257;i++)
        {
            hi[i]=(float)(hist.at<float>(i-1));     
        }
        p=(float **)malloc(sizeof(float *)*257);
        for(i=0;i<257;i++)
        {
            p[i]=(float *)malloc(sizeof(float)*257);
        }
        s=(float **)malloc(sizeof(float *)*257);
        for(i=0;i<257;i++)
        {
            s[i]=(float *)malloc(sizeof(float)*257);
        }
        h=(float **)malloc(sizeof(float *)*257);
        for(i=0;i<257;i++)
        {
            h[i]=(float *)malloc(sizeof(float)*257);
        }
        for(i=0;i<257;i++)
        {
           for(j=0;j<257;j++)
               {
                   p[i][j]=0;
                   s[i][j]=0;
                   h[i][j]=0;   
               }
        }
        p[1][0]=0;
        s[1][0]=0;
        for(i=0;i<256;i++)
        {
           p[1][i+1]=p[1][i]+hi[i+1];
           s[1][i+1]=s[1][i]+((i+1)*(hi[i+1]));
        }
        for(i=2;i<257;i++)
        {
           for(j=1;j<257;j++)
               {
                   p[i][j]=p[1][j]-p[1][i-1]; 
                   s[i][j]=s[1][j]-s[1][i-1];       
               }
        }
        float temp,temp1;            
        for(j=1;j<257;j++)
        {
           for(i=1;i<=j;i++)
               {
                   temp=(s[i][j])*(s[i][j]);
                   temp1=(temp/p[i][j]);
                   h[i][j]=temp1;
                }
         }
         as=(float **)malloc(sizeof(float *)*257);
         for(i=0;i<257;i++)
         {
            as[i]=(float *)malloc(sizeof(float)*257);
         }
         ds=(float **)malloc(sizeof(float *)*257);
         for(i=0;i<257;i++)
         {
            ds[i]=(float *)malloc(sizeof(float)*257);
         }
         for(i=0;i<257;i++)
         {
             for(j=0;j<257;j++)
                {
                   ds[i][j]=-1.000000;
                }
         }
         int n=(int )thresh;
         int index[n+1];                
         fast(n,1,256,index);
         int a=0;
         double *output=NULL;int k=0;
         output=( double *)malloc(sizeof(double)*n);
         for(i=n;i>=1;i--)
         {
              a=as[a+1][i];
              output[k]=a;
              k++;
         }      
         for(i=0;i<257;i++)
         {
             free(h[i]);
             free(as[i]);
             free(ds[i]);
         }
         free(h);
         free(as);
         free(ds);                    
         sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1,1,n,output);
         free(output); 
         if (sciErr.iErr)
         {
            printError(&sciErr, 0);
            return 0;
         } 
         AssignOutputVariable(pvApiCtx,1) = nbInputArgument(pvApiCtx) + 1;
         ReturnArguments(pvApiCtx);
         return 0;                
         }
}
