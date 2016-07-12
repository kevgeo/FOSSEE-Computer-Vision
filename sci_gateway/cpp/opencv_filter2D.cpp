/***************************************************
Author : Sukul Bagai
****************************************************
Usage : return_image = filter2D(input_image,"<ddepth>",kernel_matrix,anchor_x,anchor_y,delta);
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
  
  int opencv_filter2D(char *fname, unsigned long fname_len)
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
    char **ddepth = NULL;
    int i,j,k;
    double *kernel,anchorX,anchorY,delta;

    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

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

    //for kernel matrix
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows, &iCols ,&kernel);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    int n=iRows;
    double kernelArray[n][n];
    //assigning values to actual kernelMatrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            kernelArray[i][j]=kernel[(i*n)+j];
    // converting the array to a matrix, so that we can pass it into the filter2D function    
    Mat kernelMatrix(n, n, CV_32FC1, &kernelArray);

    //for value of anchorX
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &anchorX);
    if(intErr)
        return intErr;

    //for value of anchorY
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &anchorY);
    if(intErr)
        return intErr;

    //for value of delta
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6, &delta);
    if(intErr)
        return intErr;

    //error check
    if(anchorX >= n || anchorY >=n)
    {
        sciprint("Invalid anchor point given. Centre point (-1,-1) was used instead");
        anchorY = -1;
        anchorX = -1;
    }
    Mat new_image(image.rows,image.cols,image.type());
    Point pt(anchorX,anchorY);

    int actualddepth;

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

    //applying function
    filter2D(image,new_image,actualddepth,kernelMatrix,pt,delta);

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
