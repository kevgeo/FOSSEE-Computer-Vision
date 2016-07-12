/********************************************************
Function   :makecform
Syntax     :B=makecform(string)
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
  int opencv_makecform(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int* piAddr1=NULL;
    int error;
    int iRet    = 0;
    char* pstData = NULL;
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    /*retrieve first argument  */
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
        int k=0;
        char** output=NULL;
        output=( char **)malloc(sizeof(char*));
       if(strcasecmp(pstData,"xyz2lab")==0)
       {
        output[0]="xyz2lab";
        sciprint(" \nc_func: @xyz2lab \nColorSpace_in: 'xyz' \nColorSpace_out: 'lab'\nencoding: 'double'\ncdata: [1x1 struct]"); 
       }
      else if(strcasecmp(pstData,"lab2xyz")==0)
       {
         output[0]="lab2xyz";
         sciprint("\nc_func: @lab2xyz\nColorSpace_in: 'lab'\nColorSpace_out: 'xyz'\nencoding: 'double' \ncdata: [1x1 struct]");
       }
      else if(strcasecmp(pstData,"srgb2xyz")==0)
       {
        output[0]="srgb2xyz";
        sciprint(" \nc_func: @applycformsequence \nColorSpace_in: 'rgb'\nColorSpace_out: 'xyz' \nencoding: 'double'\ncdata: [1x1 struct]");
       }
      else if(strcasecmp(pstData,"xyz2srgb")==0)
       {
        output[0]="xyz2srgb";
        sciprint("\nc_func: @applycformsequence\nColorSpace_in: 'xyz'\nColorSpace_out: 'rgb'\nencoding: 'double' \ncdata: [1x1 struct]");
       } 
      else if(strcasecmp(pstData,"srgb2lab")==0)
       {
        output[0]="srgb2lab";
        sciprint("\nc_func: @applycformsequence\nColorSpace_in: 'rgb'\nColorSpace_out: 'lab'\nencoding: 'double'\ncdata: [1x1 struct]");
       }
      else if(strcasecmp(pstData,"lab2srgb")==0)
       {
        output[0]="lab2srgb";
        sciprint("\nc_func: @applycformsequence\nColorSpace_in: 'lab'\nColorSpace_out: 'rgb' \nencoding: 'double'\ncdata: [1x1 struct]");
       }
      else
       {
         sciprint("Expected input argument 'xyz2lab'   'lab2xyz'   'srgb2xyz' 'xyz2srgb'  'srgb2lab'  'lab2srgb'");
         return 0;
       }
           sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1,1,1,output);
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
