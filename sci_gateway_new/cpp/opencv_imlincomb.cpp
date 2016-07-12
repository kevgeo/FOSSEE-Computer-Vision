/********************************************************
Function   :imlincomb
Syntax     :C=imlincomb(x1,A1,x2,A2...xn,An)
            A1...An image matrix,x1...xn double values
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
  int opencv_imlincomb(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    Mat A,B,C,tmp;
    int* piAddr2=NULL;
    int* piAddr4=NULL;
    int iType = 0;
    int iRet    = 0;
    double value= 0;
    CheckOutputArgument(pvApiCtx, 1, 1);
    for(int i=1;i<=Rhs;i=i+2)
     {
       sciErr = getVarAddressFromPosition(pvApiCtx,i,&piAddr2);
       if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

       if(isDoubleType(pvApiCtx, piAddr2))
       {
	 if(isScalar(pvApiCtx, piAddr2))
	   {
	      iRet = getScalarDouble(pvApiCtx, piAddr2, &value);
           }
       } 
       if(i!=Rhs)
       {
         retrieveImage(A,i+1);
         A.convertTo(A,CV_64F);
         C=A*value;
         if((i+1)==2)
         {
           C.copyTo(tmp);
           A.copyTo(B);
         }
         else
         {
           add(C,tmp,tmp);
         
         }
        }
       else
        {
          add(tmp,value,tmp);
        }
       A.release();
       C.release();  
      } 
    int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(tmp.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,tmp,1);
    free(checker); 
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
  }
}
