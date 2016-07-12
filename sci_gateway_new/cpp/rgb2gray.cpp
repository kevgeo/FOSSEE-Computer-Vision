/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : return_image=rgb2gray(sourceimage)                          */
/* ==================================================================== */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"  //new header file for cvtColor function of opencv
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
int rgb2gray(char *fname, unsigned long fname_len)
{

    // Error management variable
        SciErr sciErr;
    //checking input argument
        CheckInputArgument(pvApiCtx,1,1);
    //checking output argument
        CheckOutputArgument(pvApiCtx, 1, 1) 
        Mat img,grayimg;
    //calling function 
        retrieveImage(img,1);
        cvtColor(img, grayimg, CV_BGR2GRAY );
        string tempstring = type2str(grayimg.type());
        char* checker = (char *)malloc(tempstring.size() + 1);
        memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
        returnImage(checker,grayimg,1);
       //returning output variable    
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx); 
        return 0;
}

/* ==================================================================== */
}
