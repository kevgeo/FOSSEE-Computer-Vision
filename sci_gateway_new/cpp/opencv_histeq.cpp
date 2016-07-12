//histeq function is for grayscale image
/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : return_image=histeq(rgbimage);
/* ==================================================================== */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>
#include<string.h>
#include "../common.h"
int histeq(char *fname, unsigned long fname_len)
{

    // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
	int* piAddr		= NULL;
        int* piAddrNew          = NULL;
	int* piLen		= NULL;
	char** pstData	        = NULL;
        int * pstData2	        = NULL;
        int* piAddr3            =NULL;
        int* piAddr4            =NULL;
        double *rrows;
        double *rcols;
        int t;
    //checking input argument
         CheckInputArgument(pvApiCtx,1,1);
    //checking output argument
         CheckOutputArgument(pvApiCtx,1,1) ;
    //for first argument 
            Mat img;
            retrieveImage(img,1);
            Mat dst; 
            ////open cv function to equalize the histogram
            equalizeHist(img,dst); 
            string tempstring = type2str(dst.type());
            char* checker = (char *)malloc(tempstring.size() + 1);
            memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
            returnImage(checker,dst,1);
            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);
            return 0; 
    } 
}
/* ==================================================================== */

