/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : rotated_image=imrotate(sourcrimage,angle);                                                          */
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
  int imrotate(char *fname, unsigned long fname_len)
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
        int* piAddrNew            = NULL;
	int* piLen		= NULL;
	double pstData;
        int* pstData2    	= NULL;
        double *rrows;
        double *rcols;
        int error;
   //function to check number of input variable 
        CheckInputArgument(pvApiCtx, 2, 2);
   //function to check number of output variable
           CheckOutputArgument(pvApiCtx, 1, 1);
   // get Address of first input 
           Mat img; 
           retrieveImage(img,1); 
        //second argument
           //fetching address of second argument
           sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
           //checking for error if any
           if (sciErr.iErr)
           {
           printError(&sciErr, 0);
           return 0;
           }
           //this function will fetch second argument i.e angle
           error=getScalarDouble(pvApiCtx,piAddr2,&pstData); 
           if(error!=0)
           {
                  sciprint("error in retrieving second argument");
           }        
        //creating distination matrix 
           Mat dst;
        //following set of functions for image rotation
           Point2f pt((img.cols)/2., (img.rows)/2.);    
           Mat r1 = getRotationMatrix2D(pt, pstData, 1.0);
           warpAffine(img, dst, r1, Size(dst.cols, dst.rows));
        //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
           string tempstring = type2str(dst.type());
           char* checker = (char *)malloc(tempstring.size() + 1);
           memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
           returnImage(checker,dst,1);
       //returning output arguments
           AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
           ReturnArguments(pvApiCtx);
           return 0;
     
   
   
 }
}
