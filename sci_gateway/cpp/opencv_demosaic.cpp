/********************************************************
    Author: V Srinivas
*********************************************************
  void cvtColor(InputArray src, int code, int dstCn=0 )
********************************************************/
//Demosaicing using cvt color filter

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
  
  int opencv_demosaic(char *fname, unsigned long fname_len)
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
    CheckInputArgument(pvApiCtx, 2,2);
    CheckOutputArgument(pvApiCtx, 1,1) ;


     Mat src;
     retrieveImage(src,1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

     //Now, we will retrieve the string from the input parameter. For this, we will require 3 calls
    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
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

    

     Mat dst;
     
    if(strcmp(codeString[0], "gbrg") == 0)
              code = CV_BayerGR2RGB;
    else if(strcmp(codeString[0], "bggr") == 0)
              code = CV_BayerRG2RGB;
    else if(strcmp(codeString[0], "rggb") == 0)
              code = CV_BayerBG2RGB;        
    else if(strcmp(codeString[0], "grbg") == 0)
              code = CV_BayerGB2RGB;
    else{

        sciprint("Invalid Bayer filter code");
        return 0;
    }
    
    cvtColor(src, dst, code, 0);

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