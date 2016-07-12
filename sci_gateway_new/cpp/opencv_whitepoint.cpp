/********************************************************
Function   :whitepoint
Syntax     :B=whitepoint(string)
*string    :'d65' , 'd50','a','icc','c','d55'
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
 
  int opencv_whitepoint(char *fname, unsigned long fname_len)
  {
    // Error management variable
        SciErr sciErr;
    //variable info
	
        int* piAddr1=NULL;
        int error;
        int iRet    = 0;
        char* pstData = NULL;
    //checking input argument
        CheckInputArgument(pvApiCtx, 1, 1);
        CheckOutputArgument(pvApiCtx, 1, 1) ;
        sciErr = getVarAddressFromPosition(pvApiCtx, 1,&piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isStringType(pvApiCtx, piAddr1))
	{
		if(isScalar(pvApiCtx, piAddr1))
		{
			
                        iRet = getAllocatedSingleString(pvApiCtx, piAddr1, &pstData);
                 }
        }
       double *output=NULL;int k=0;
       output=( double *)malloc(sizeof(double)*3);
       if(strcmp(pstData,"d65")==0)
       {
        output[0]=0.9504;
        output[1]=1.0000;
        output[2]=1.0889;
       }
      else if(strcmp(pstData,"d50")==0)
       {
        output[0]=0.9642;
        output[1]=1.0000;
        output[2]=0.8251;
       }
      else if(strcmp(pstData,"icc")==0)
       {
        output[0]=0.9642;
        output[1]=1.0000;
        output[2]=0.8249;  
       }
      else if(strcmp(pstData,"a")==0)
       {
        output[0]=1.0985;
        output[1]=1.0000;
        output[2]=0.3558;  
       } 
      else if(strcmp(pstData,"d55")==0)
       {
        output[0]=0.9568;
        output[1]=1.0000;
        output[2]=0.9214;   
       }
      else if(strcmp(pstData,"c")==0)
       {
        output[0]=0.9807;
        output[1]=1.0000;
        output[2]=1.1823; 
       }
      else
       {
         sciprint("Expected input number 1:to match one of these strings:'icc', 'd50', 'd55', 'd65', 'c', 'a'");
         return 0;
       }
           sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1,1,3,output);
           free(output); // Data have been copied into Scilab memory
           if (sciErr.iErr)
                  {
                      //Make sure everything is cleanup in case of error
                     printError(&sciErr, 0);
                     return 0;
                  }              
            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);
            return 0;                
}
}
