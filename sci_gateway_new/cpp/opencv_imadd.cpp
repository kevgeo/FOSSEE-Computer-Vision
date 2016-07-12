/********************************************************
Function   :imadd
Syntax     :C=imadd(A,B)
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
  int opencv_imadd(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    Mat A,B,C;
    int* piAddr2=NULL;
    int iType = 0;
    int iRet    = 0;
    double value= 0;
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);
    retrieveImage(A,1);
    sciErr =getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    sciErr = getVarType(pvApiCtx, piAddr2, &iType);
    if(iType==sci_list)
    {
       retrieveImage(B,2);
       Size s1=A.size();
       Size s2=B.size();
       if(s1!=s2)
        { 
          sciprint("\nBoth input should be same size");
          return 0;
        }
       add(A,B,C);
    }
    else
    {
      if(isDoubleType(pvApiCtx, piAddr2))
       {
	 if(isScalar(pvApiCtx, piAddr2))
	   {
	      iRet = getScalarDouble(pvApiCtx, piAddr2, &value);
           }
       } 
      add(A,value,C);
    }  
    
    int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(C.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,C,1);
    free(checker); 
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
  }
}
