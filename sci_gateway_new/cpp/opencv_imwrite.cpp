/********************************************************
Author: Sukul Bagai
********************************************************/

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
  #include "BOOL.h"
  #include <localization.h>
  #include "../common.h"
  
  int opencv_imwrite(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr1 = NULL;
    int *piLen = NULL;
    char **pstData=NULL;
    int i,j,k;    

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    //CheckOutputArgument(pvApiCtx, 0, 0) ;
    Mat image;
    retrieveImage(image, 1); 

    //Now, retriving path
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    //image path will contain the path where the image has to be written
    string image_path=pstData[0];

    //  image_path="\""+image_path+"\"";

    //writes to the path
    imwrite(image_path,image);

    return 0;
  }
/* ==================================================================== */
}
