//Syntax  int getKernelType(InputArray, Px,py)
/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : returnkernel_type= getkerneltype(InputArray,point px,point py) */
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
  int getkerneltype(char *fname, unsigned long fname_len)
    {
    // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
	int* piAddr1		= NULL;
        int* piAddr2            = NULL;
        int* piAddr3            = NULL;
        int* piAddr4            = NULL;
        int* piAddr5            = NULL;
        int* piAddr6            = NULL;
        int* piAddrNew          = NULL;
	int* piLen		= NULL;
	double* pstData	        = NULL;
        double* pstData2;
        double *rrows;
        double *rcols;
        int error;        
        double pi1,pi2;
         // checking input argument 
        CheckInputArgument(pvApiCtx,3,3);
        //checking output argument
        CheckOutputArgument(pvApiCtx, 1, 1);
        //for first argument 
        // get Address of first input  
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
       //check for any error
        if(sciErr.iErr)   
        {
          printError(&sciErr, 0);
          return 0;
        }   
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &piRows, &piCols,&pstData2);
        if(sciErr.iErr)
        {
		printError(&sciErr, 0);
		return 0;
        } 
        Mat kernel=Mat(piRows,piCols,CV_64F);
        int k=0,m=0;
         for(int j=0;j<piCols;j++)
           {
                for(int i=0;i<piRows;i++)
                  {
                          kernel.at<double>(i,j)=pstData2[k++];
                          
                  }
           }
         cout<<kernel;
        //second argument
        sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
        if (sciErr.iErr)
        {
        printError(&sciErr, 0);
        return 0;
        }
         //this function will fetch value of second argument
         error=getScalarDouble(pvApiCtx,piAddr2,&pi1); 
               if(error!=0)
               {
                  sciprint("error in retrieving second argument");
               }  
       
        //third argument
        sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
        if (sciErr.iErr)
        {
        printError(&sciErr, 0);
        return 0;
        }
        //this function will fetch value of third argument
         error=getScalarDouble(pvApiCtx,piAddr3,&pi2); 
               if(error!=0)
               {
                  sciprint("error in retrieving second argument");
               }  
        Point anchor=Point(pi1,pi2);
        double res;
        res=getKernelType(kernel, anchor);
        error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,res);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         } 

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
         return 0;
      
      
  }
}
