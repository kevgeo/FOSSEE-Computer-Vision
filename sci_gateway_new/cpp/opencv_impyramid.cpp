/********************************************************
Function   :impyramid
Syntax     :B=impyramid(A,direction)
*direction :expand or reduce
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
 
  int opencv_impyramid(char *fname, unsigned long fname_len)
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
        double *value=NULL;
        int* piLen		= NULL;
        char **val;
        int i,j,k=0,l=0;
        int error;
        String s;
        int iRet    = 0;
        char* pstData = NULL;
    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image, 1);
    sciErr = getVarAddressFromPosition(pvApiCtx, 2,&piAddr3);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isStringType(pvApiCtx, piAddr3))
	{
		if(isScalar(pvApiCtx, piAddr3))
		{
			
                        iRet = getAllocatedSingleString(pvApiCtx, piAddr3, &pstData);
                 }
        }
   
	  
     k=image.rows;
     l=image.cols; 
     Mat new_image;
     if(strcmp(pstData,"expand")==0)
       {
          Size size((k*2)-1,(l*2)-1); 
          resize(image,new_image,size); 
       }
     else if(strcmp(pstData,"reduce")==0)
       {
          Size size(k/2,l/2); 
          resize(image,new_image,size); 
       }
     else
       {
          sciprint("Not valid 2nd argument");
       }
   
   

    int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_image,1);
    free(checker); 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
   
  }
/* ==================================================================== */
}
