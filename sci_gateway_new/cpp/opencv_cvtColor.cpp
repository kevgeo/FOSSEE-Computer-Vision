/********************************************************
    Author: Shubheksha Jalan
*********************************************************
  void cvtColor(InputArray src, int code, int dstCn=0 )
********************************************************/
// number of channels in the destination image; if the parameter is 0, the number of the channels is derived automatically from src and code 


#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "sciprint.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "../common.h"
  
  int opencv_cvtColor(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piLen = NULL;
    double dstCn;
    int code; 
    char **codeString = NULL;
    int i, j, k;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 1,1) ;


     Mat src;
     retrieveImage(src,1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

     //Now, we will retrieve the string from the input parameter. For this, we will require 3 calls
    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    codeString = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
        codeString[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination

    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, codeString);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr){
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &dstCn);
    if(intErr){
        return intErr;
    }

     Mat dst;
     
    if(strcmp(codeString[0], "CV_BGR2GRAY") == 0)
              code = CV_BGR2GRAY;
    else if(strcmp(codeString[0], "CV_RGB2GRAY") == 0)
              code = CV_RGB2GRAY;
    else if(strcmp(codeString[0], "CV_GRAY2BGR") == 0)
              code = CV_GRAY2BGR;        
    else if(strcmp(codeString[0], "CV_GRAY2RGB") == 0)
              code = CV_GRAY2RGB;
    else if(strcmp(codeString[0], "CV_BGR2XYZ") == 0)
              code = CV_BGR2XYZ;
    else if(strcmp(codeString[0], "CV_RGB2XYZ") == 0)
              code = CV_RGB2XYZ;
    else if(strcmp(codeString[0], "CV_XYZ2BGR") == 0)
              code = CV_XYZ2BGR;
    else if(strcmp(codeString[0], "CV_XYZ2RGB") == 0)
              code = CV_XYZ2RGB;
    else if(strcmp(codeString[0], "CV_BGR2YCrCb") == 0)
              code = CV_BGR2YCrCb;
    else if(strcmp(codeString[0], "CV_RGB2YCrCb") == 0)
              code = CV_RGB2YCrCb;
    else if(strcmp(codeString[0], "CV_YCrCb2BGR") == 0)
              code = CV_YCrCb2BGR;
    else if(strcmp(codeString[0], "CV_YCrCb2RGB") == 0)
              code = CV_YCrCb2RGB;
    else if(strcmp(codeString[0], "CV_BGR2HSV") == 0)
              code = CV_BGR2HSV;
    else if(strcmp(codeString[0], "CV_RGB2HSV") == 0)
              code = CV_RGB2HSV;
    else if(strcmp(codeString[0], "CV_HSV2BGR") == 0)
              code = CV_HSV2BGR;
    else if(strcmp(codeString[0], "CV_HSV2RGB") == 0)
              code = CV_HSV2RGB;
    else if(strcmp(codeString[0], "CV_BGR2HLS") == 0)
              code = CV_BGR2HLS;
    else if(strcmp(codeString[0], "CV_RGB2HLS") == 0)
              code = CV_RGB2HLS;
    else if(strcmp(codeString[0], "CV_HLS2BGR") == 0)
              code = CV_HLS2BGR;
    else if(strcmp(codeString[0], "CV_HLS2RGB") == 0)
              code = CV_HLS2RGB;
    else if(strcmp(codeString[0], "CV_BGR2Lab") == 0)
              code = CV_BGR2Lab;
    else if(strcmp(codeString[0], "CV_RGB2Lab") == 0)
              code = CV_RGB2Lab;
    else if(strcmp(codeString[0], "CV_Lab2BGR") == 0)
              code = CV_Lab2BGR;
    else if(strcmp(codeString[0], "CV_Lab2RGB") == 0)
              code = CV_Lab2RGB;
    else if(strcmp(codeString[0], "CV_BGR2Luv") == 0)
              code = CV_BGR2Luv;
    else if(strcmp(codeString[0], "CV_RGB2Luv") == 0)
              code = CV_RGB2Luv;
    else if(strcmp(codeString[0], "CV_Luv2BGR") == 0)
              code = CV_Luv2BGR;
    else if(strcmp(codeString[0], "CV_Luv2RGB") == 0)
              code = CV_Luv2RGB;
    else{

        sciprint("Invalid code");
        return 0;
    }
    
    cvtColor(src, dst, code, dstCn);

    // namedWindow ("window", 1);
    // imshow("window", dst);
    // waitKey(0);
    //  destroyWindow("window");


    string tempstring = type2str(dst.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,dst,1); 
    free(checker);




    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;
}
/* ==================================================================== */
}
