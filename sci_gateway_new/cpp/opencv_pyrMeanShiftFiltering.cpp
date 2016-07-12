/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax :return_image=pyrMeanShiftFiltering(InputArray src, double sp, double sr); */
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
  int pyrMeanShiftFiltering(char *fname, unsigned long fname_len)
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
        double sp;
        double sr;
        int error;
        TermCriteria termcrit=TermCriteria( TermCriteria::MAX_ITER+TermCriteria::EPS,5,1) ;
       
        // checking input argument 
        CheckInputArgument(pvApiCtx,3,3);
        //checking output argument
        CheckOutputArgument(pvApiCtx, 1, 1);
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
        error=getScalarDouble(pvApiCtx,piAddr2,&sp); 
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
        error=getScalarDouble(pvApiCtx,piAddr3,&sr); 
        if(error!=0)
        {
          sciprint("error in retrieving second argument");
        }  
       //creating variable of type Mat
        Mat dst;
       //open cv function 
        pyrMeanShiftFiltering(img,dst, sp,sr,1,termcrit);
        string tempstring = type2str(dst.type());
        char* checker = (char *)malloc(tempstring.size() + 1);
        memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
        returnImage(checker,dst,1);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
        return 0;
   
  }
}
