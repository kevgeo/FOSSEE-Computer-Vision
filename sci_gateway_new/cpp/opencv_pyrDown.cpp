/***************************************************
Author : Sukul Bagai
****************************************************
  return_image = pyrDown(input_image, dstsize_width, distsize_height, border;
***************************************************/

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
  #include <sciprint.h>
  #include "../common.h"
  
  int opencv_pyrDown(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piLen = NULL;
    char **borderType = NULL;
    double dstsize_width,dstsize_height;
    int border;
    int i;

    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for value of dstsize_width
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &dstsize_width);
    if(intErr)
        return intErr;

    //for value of dstsize_width
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3 ,&dstsize_height);
    if(intErr)
        return intErr;

    //for border
    sciErr = getVarAddressFromPosition(pvApiCtx,4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfString(pvApiCtx, piAddr4, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr4, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    borderType = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        borderType[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr4, &iRows, &iCols, piLen, borderType);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    

    //error checks
    if(dstsize_height<0)
    {
        sciprint("Positive Value Required for Height. 1 value was used instead\n");
        dstsize_height=1;
    }
    if(dstsize_width<0)
    {
        sciprint("Positive Value Required for Width. 1 value was used instead\n");
        dstsize_width=1;
    }
    if((dstsize_width != image.cols*2) && (dstsize_width != image.cols*2+1) && (dstsize_width != image.cols*2-1))
    {
        sciprint("Incorrect dstsize_width value was used. src.cols*2+1 value was calculated and used instead\n");
        dstsize_width = image.cols*2+1;
    }
    if((dstsize_height != image.rows*2) && (dstsize_height != image.rows*2+1) && (dstsize_height != image.rows*2-1))
    {
        sciprint("Incorrect dstsize_height value was used. src.rows*2+1 value was calculated and used instead\n");
        dstsize_height = image.rows*2+1;
    }

    Mat new_image(dstsize_height,dstsize_width,image.type());

    if(strcmp(borderType[0], "BORDER_CONSTANT") == 0)
        border = BORDER_CONSTANT;
    else if(strcmp(borderType[0], "BORDER_REPLICATE") == 0)
        border = BORDER_REPLICATE;
    else if(strcmp(borderType[0], "BORDER_REFLECT") == 0)
        border = BORDER_REFLECT;
    else if(strcmp(borderType[0], "BORDER_REFLECT_101") == 0)
        border = BORDER_REFLECT_101;
    else if(strcmp(borderType[0], "BORDER_WRAP") == 0)
        border = BORDER_WRAP;
    else
        border = BORDER_DEFAULT;

    pyrDown(image,new_image,Size(dstsize_width,dstsize_height),border);

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