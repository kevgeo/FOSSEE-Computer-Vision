/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : rotated_image=warpaffline(InputArray src, InputArray M,int size_x,int size_y ,string flag, string borderMode);                                                          */
/* ==================================================================== */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
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
  int warpaffine(char *fname, unsigned long fname_len)
    {
    // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
	int* piAddr		= NULL;
        int *piAddr7           =NULL;
        int* piAddr2            = NULL;
        int* piAddr3            = NULL;
        int* piAddr4            = NULL;
        int* piAddr5            = NULL;
        int* piAddr6            = NULL;
        int* piAddrNew            = NULL;
	int* piLen		= NULL;
	double* pstData	        = NULL;
        int* pstData2    	= NULL;
        double *rrows;
        double *rcols;
        int error;

         ////checking input argument 
        CheckInputArgument(pvApiCtx,6,6);
         //checking output argument
        CheckOutputArgument(pvApiCtx, 1, 1);
         //for first argument 
         // get Address of first input  
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        //retrieving image
         Mat img,dst;
         retrieveImage(img,1);
    //second argument
            double *m;
           //fetching address of second argument
               sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
            //checking for error if any
               if (sciErr.iErr)
                {
                   printError(&sciErr, 0);
                   return 0;
                }
             //this function will fetch value of second argument
               sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols ,&m);
               if(sciErr.iErr)
	        {
		  printError(&sciErr, 0);
		  return 0;
        	}     

               Mat m1=Mat(2,3,CV_64F);
               int k=0;
                for(int j=0;j<3;j++)
                    {
                       for(int i=0;i<2;i++)
                           {
                            m1.at<double>(i,j)=m[k++];
                        //    cout<<   m1.at<double>(i,j)<<"\n";
                           } 
                    }
                                   
  cout<<m1;
  //third argument
               double sz1,sz2;
            //fetching address of third argument
               sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
             //checking for error if any
               if (sciErr.iErr)
               {
                printError(&sciErr, 0);
                return 0;
               }
             //this function will fetch value of third argument
               error=getScalarDouble(pvApiCtx,piAddr3,&sz1); 
               if(error!=0)
               {
                  sciprint("error in retrieving second argument");
               }  
 //fourth argument
              //fetching address of fourth argument
               sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
               if (sciErr.iErr)
               {
               printError(&sciErr, 0);
               return 0;
               }
                //this function will fetch value of fourth argument
              error=getScalarDouble(pvApiCtx,piAddr4,&sz2); 
               if(error!=0)
               {
                  sciprint("error in retrieving second argument");
               }  
   //fifth argument
               //fetching address of fifth argument
               char **flag;
               sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
               if (sciErr.iErr)
               {
               printError(&sciErr, 0);
               return 0;
               }
               sciErr = getMatrixOfString(pvApiCtx, piAddr5, &iRows, &iCols, NULL, NULL);
               piLen = (int*)malloc(sizeof(int) * iRows * iCols);
               //second call to retrieve length of each string of first argument
               sciErr = getMatrixOfString(pvApiCtx, piAddr5, &iRows, &iCols, piLen, NULL);
               if(sciErr.iErr)
               {
                  printError(&sciErr, 0);
                  return 0;
               }
               flag = (char**)malloc(sizeof(char*) * iRows * iCols);
               for(int i = 0 ; i < iRows * iCols ; i++)
               {
                   flag[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
               }
               //third call to retrieve data of each string of first argument
               sciErr = getMatrixOfString(pvApiCtx, piAddr5, &iRows, &iCols, piLen,flag);
               if(sciErr.iErr)
               {
                  printError(&sciErr, 0);
                  return 0;
               }
                 //this function will fetch value of fifth argument
              
//sixth argument
               //fetching address of sixth argument
               char **bordermode;
               sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
               if (sciErr.iErr)
               {
               printError(&sciErr, 0);
               return 0;
               }
               sciErr = getMatrixOfString(pvApiCtx, piAddr6, &iRows, &iCols, NULL, NULL);
               piLen = (int*)malloc(sizeof(int) * iRows * iCols);
               //second call to retrieve length of each string of first argument
               sciErr = getMatrixOfString(pvApiCtx, piAddr6, &iRows, &iCols, piLen, NULL);
               if(sciErr.iErr)
               {
                  printError(&sciErr, 0);
                  return 0;
               }
               bordermode = (char**)malloc(sizeof(char*) * iRows * iCols);
               for(int i = 0 ; i < iRows * iCols ; i++)
               {
                   bordermode[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
               }
               //third call to retrieve data of each string of first argument
               sciErr = getMatrixOfString(pvApiCtx, piAddr6, &iRows, &iCols, piLen,bordermode);
               if(sciErr.iErr)
               {
                  printError(&sciErr, 0);
                  return 0;
               }
               //calling size constructor
               Size ksize=Size(sz1,sz2);
             
               
              
               
               //calling warpaffline function
             
              int flag1,border; 
              if(!strcmp(flag[0],"INTER_NEAREST"))
              flag1=INTER_LINEAR;
              else if(!strcmp(flag[0],"INTER_LINEAR"))
              flag1=INTER_LINEAR;
              else if(!strcmp(flag[0],"INTER_AREA"))
              flag1=INTER_AREA;
              else if(!strcmp(flag[0],"INTER_CUBIC"))
              flag1=INTER_CUBIC;
               else if(!strcmp(flag[0],"INTER_LANCZOS4"))
              flag1=INTER_LANCZOS4;
              if(!strcmp(bordermode[0]," BORDER_TRANSPARENT"))
              border=BORDER_TRANSPARENT;
              else if(!strcmp(bordermode[0],"BORDER_CONSTANT"))
              border=BORDER_CONSTANT;
              else if(!strcmp(bordermode[0],"BORDER_WRAP"))
              border=BORDER_WRAP;
              else if(!strcmp(bordermode[0],"BORDER_REFLECT_101"))
              border=BORDER_REFLECT_101;
               warpAffine(img, dst,m1, ksize,flag1,border);            
               string tempstring = type2str(dst.type());
              char* checker = (char *)malloc(tempstring.size() + 1);
              memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
              returnImage(checker,dst,1);
             //returning output variable    
              AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
              ReturnArguments(pvApiCtx);   
                  
}
}
