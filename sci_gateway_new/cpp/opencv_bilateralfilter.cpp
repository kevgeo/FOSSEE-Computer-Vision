/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : return_image=bilateralFilter(InputArray src, int d, double sigmaColor, double sigmaSpace ) */
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
  #include <localization.h> 
  #include "../common.h"  
  int bilateralfilter(char *fname, unsigned long fname_len)
    {
    // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
	int* piAddr		= NULL;
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
        unsigned char *pstDataR = NULL;
        unsigned char *pstDataG = NULL;
        unsigned char *pstDataB = NULL;
        int noofitem;
        double d;
        double sigmaColor;
        double sigmaSpace;
        int error;
        // checking input argument 
        CheckInputArgument(pvApiCtx,4,4);
        //checking output argument
        CheckOutputArgument(pvApiCtx, 1, 1);
        //for first argument 
        // get Address of first input  
       Mat img;
       retrieveImage(img,1);
       //second argument
             sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
             if (sciErr.iErr)
             {
               printError(&sciErr, 0);
               return 0;
             }
    //this function will fetch value of second argument
             error=getScalarDouble(pvApiCtx,piAddr2,&d) ;  
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
              error=getScalarDouble(pvApiCtx,piAddr3,&sigmaColor) ;  
              if(error!=0)
              {
                 sciprint("error in retrieving second argument");
              }      
             
 //fourth argument
             sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
             if (sciErr.iErr)
             {
               printError(&sciErr, 0);
               return 0;
             }
//this function will fetch value of fourth argument
             error=getScalarDouble(pvApiCtx,piAddr4,&sigmaSpace) ;  
             if(error!=0)
             {
                 sciprint("error in retrieving second argument");
             }    
             Mat dst;
         //calling open cv bilateral filter function
             bilateralFilter(img,dst,d,sigmaColor,sigmaSpace,BORDER_DEFAULT );
             string tempstring = type2str(dst.type());
             char* checker = (char *)malloc(tempstring.size() + 1);
             memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
             returnImage(checker,dst,1);
             //returning output variable    
             AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
             ReturnArguments(pvApiCtx); 
             return 0;
                
}
}
