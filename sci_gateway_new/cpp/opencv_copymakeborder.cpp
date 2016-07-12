/********************************************************
    Author: Shubheksha Jalan
*********************************************************
  void copyMakeBorder(InputArray image, int top, int bottom, int left, int right, int borderType, const Scalar& value=Scalar() )
********************************************************/
// for BORDER_CONSTANT, enter a 4 element array as Scalar. For any other border type, pass 0.


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
  
  int opencv_copymakeborder(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6 = NULL;
    int *piAddr7 = NULL;
    int i,j,k;
    char **borderType;
    double top, bottom, right, left;
    double *value = NULL;
     //checking input argument
    CheckInputArgument(pvApiCtx, 6, 7);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for top
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr2, &top);
     if(intErr)
    {
        return intErr;
    }   

    //for bottom
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr3, &bottom);
    if(intErr)
    {
        return intErr;
    }   

     //for left
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr4, &left);
    if(intErr)
    {
        return intErr;
    }   

     //for bottom
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr5, &right);
    if(intErr)
    {
        return intErr;
    }


      sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    //Now, we will retrieve the string from the input parameter. For this, we will require 3 calls
    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr6, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr6, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    borderType = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
        borderType[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination

    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr6, &iRows, &iCols, piLen, borderType);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //for array of Scalar object
     sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr7, &iRows, &iCols ,&value);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    Mat new_image(Size(image.cols+left+right, image.rows+top+bottom), image.type());  

    if(strcmp(borderType[0], "BORDER_CONSTANT") == 0  && iCols == 4 )
          copyMakeBorder(image, new_image, top, bottom, left, right, BORDER_CONSTANT,  Scalar(value[0], value[1], value[2], value[3]));
    else if(strcmp(borderType[0], "BORDER_CONSTANT") == 0 && iCols == 3)
          copyMakeBorder(image, new_image, top, bottom, left, right, BORDER_CONSTANT, Scalar(value[0], value[1], value[2]));
    else if(strcmp(borderType[0], "BORDER_DEFAULT") == 0)
          copyMakeBorder(image, new_image, top, bottom, left, right, BORDER_DEFAULT);
    else if(strcmp(borderType[0], "BORDER_REPLICATE") == 0)
          copyMakeBorder(image, new_image, top, bottom, left, right, BORDER_REPLICATE);
    else if(strcmp(borderType[0], "BORDER_REFLECT") == 0)
          copyMakeBorder(image, new_image, top, bottom, left, right, BORDER_REFLECT);
    else if(strcmp(borderType[0], "BORDER_REFLECT_101") == 0)
          copyMakeBorder(image, new_image, top, bottom, left, right, BORDER_REFLECT_101);
    else if(strcmp(borderType[0], "BORDER_WRAP") == 0)
          copyMakeBorder(image, new_image, top, bottom, left, right, BORDER_WRAP);

    // namedWindow ("window", 1);
    // imshow("window", new_image);
    //  waitKey(0);
    //  destroyWindow("window");

free(borderType);

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
