/***************************************************
Author : Sukul Bagai
****************************************************
Usage : return_image = sepFilter2D(input_image,"<ddepth>",kernel_x,kernel_y,anchor_x,anchor_y,delta,"<bordertype>");
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
  
  int opencv_sepFilter2D(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6 = NULL;
    int *piAddr7 = NULL;
    int *piAddr8 = NULL;
    char **ddepth = NULL;
    char **borderType = NULL;
    int i,j,k;
    double *kernelX,*kernelY,anchorX,anchorY,delta,border,actualddepth;

    //checking input argument
    CheckInputArgument(pvApiCtx, 8, 8);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);
cout<<"here1";
    //for ddepth
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

    ddepth = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
        ddepth[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, ddepth);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //for kernel_x matrix
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows, &iCols ,&kernelX);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    int n;

    if(iRows == 1)
        n=iCols;
    else
        n=iRows;
    double kernelArray_x[n];
    //assigning values to actual kernelMatrix
    for(i=0;i<n;i++)
            kernelArray_x[i]=kernelX[i];
    // converting the array to a matrix, so that we can pass it into the filter2D function    
    Mat kernelMatrix_x(1, n, CV_32FC1, &kernelArray_x);

    //for kernel_y matrix
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols ,&kernelY);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    if(iRows == 1)
        n=iCols;
    else
        n=iRows;
    double kernelArray_y[n];
    //assigning values to actual kernelMatrix
    for(i=0;i<n;i++)
            kernelArray_y[i]=kernelY[i];
    
    // converting the array to a matrix, so that we can pass it into the filter2D function    
    Mat kernelMatrix_y(1, n, CV_32FC1, &kernelArray_y);

    //for value of anchorX
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &anchorX);
    if(intErr)
        return intErr;

    //for value of anchorY
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6, &anchorY);
    if(intErr)
        return intErr;

    //for value of delta
    sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr7, &delta);
    if(intErr)
        return intErr;

    //for border
    sciErr = getVarAddressFromPosition(pvApiCtx,8, &piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfString(pvApiCtx, piAddr8, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr8, &iRows, &iCols, piLen, NULL);
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
    sciErr = getMatrixOfString(pvApiCtx, piAddr8, &iRows, &iCols, piLen, borderType);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //error check
    if(anchorX >= n || anchorY >=n)
    {
        sciprint("Invalid anchor point given. Centre point (-1,-1) was used instead");
        anchorY = -1;
        anchorX = -1;
    }
    Mat new_image(image.rows,image.cols,image.type());
    Point pt(anchorX,anchorY);

    if(strcmp(ddepth[0],"CV_8U")==0)
        actualddepth=CV_8U;
    else if(strcmp(ddepth[0],"CV_16U")==0)
        actualddepth=CV_16U;
    else if(strcmp(ddepth[0],"CV_16S")==0)
        actualddepth=CV_16S;
    else if(strcmp(ddepth[0],"CV_32F")==0)
        actualddepth=CV_32F;
    else if(strcmp(ddepth[0],"CV_64F")==0)
        actualddepth=CV_64F;
    else
    {
        sciprint("Invalid type %s used. CV_8U was used instead by default",ddepth[0]);
        actualddepth = CV_8U;
    }
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

    //applying function
    sepFilter2D(image,new_image,actualddepth,kernelMatrix_x,kernelMatrix_y,pt,delta,border);

    //returning image
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
