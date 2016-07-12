/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax :return_int=borderInterpolate(int p,int len,int borderType)
/* ==================================================================== */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include<string>
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
  int borderInterpolate(char *fname, unsigned long fname_len)
    {
    // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
	int* piAddr1	= NULL;
        int* piAddr2            = NULL;
        int* piAddr3            = NULL;
        int* piAddr4            = NULL;
        int* piAddr5            = NULL;
        int* piAddr6            = NULL;
        int* piAddrNew            = NULL;
	int* piLen		= NULL;
	double* pstData	        = NULL;
        char **pstData2    	= NULL;
        double *rrows;
        double *rcols;
        double p;
        double len;
        int borderType;
        int error;
       // checking input argument 
        CheckInputArgument(pvApiCtx,3,3);
       //checking output argument
        CheckOutputArgument(pvApiCtx, 1, 1);
      //for first argument 
      // get Address of first input  
            sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
             //check for any error
            if (sciErr.iErr)
            {
               printError(&sciErr, 0);
               return 0;
            }
            //this function will fetch value of first argument
            error=getScalarDouble(pvApiCtx,piAddr1,&p) ;  
            if(error!=0)
            { 
               sciprint("error in retrieving second argument");
            } 
            //second argument
            sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
            if (sciErr.iErr)
            {
              printError(&sciErr, 0);
              return 0;
            }
            //this function will fetch value of second argument
             error=getScalarDouble(pvApiCtx,piAddr2,&len) ;  
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
            sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, NULL, NULL);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            piLen = (int*)malloc(sizeof(int) * iRows * iCols);
            //second call to retrieve length of each string
            sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen, NULL);
            if(sciErr.iErr)
            {
             printError(&sciErr, 0);
             return 0;
            }

            pstData2 = (char**)malloc(sizeof(char*) * iRows * iCols);
            for(int i = 0 ; i < iRows * iCols ; i++)
            {
              pstData2[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
            }
           //third call to retrieve data
            sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen, pstData2);
            if(sciErr.iErr)
            {
               printError(&sciErr, 0);
               return 0;
            }
           if(!strcmp(pstData2[0],"BORDER_REFLECT_101"))
           {
           borderType=BORDER_REFLECT_101;
           }
           else if(!strcmp(pstData2[0],"BORDER_WRAP"))
           {
           borderType=BORDER_WRAP;
           }
           else if(!strcmp(pstData2[0]," BORDER_WRAP"))
           {
           borderType=BORDER_WRAP;
           }
           else if(!strcmp(pstData2[0]," BORDER_TRANSPARENT"))
           {
           borderType= BORDER_TRANSPARENT;
           }
          else if(!strcmp(pstData2[0]," BORDER_CONSTANT"))
           {
           borderType= BORDER_TRANSPARENT;
           }
   //calling open cv function for borderInterpolate
           double result;
           
           result=borderInterpolate(p,len,borderType) ;
           error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,result);
           if(error!=0)
           {
           sciprint("error occurred");
           return 0;    
           } 
      //returning output 
           AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
           ReturnArguments(pvApiCtx);
           return 0;
 }
}

