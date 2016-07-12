/***************************************************
Author : Sukul Bagai , Abhilasha Sancheti
****************************************************
Usage : [size,baseline] = getTextSize("<input text>","<fontface>",fontscale,thickness);
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
  
  int opencv_getTextSize(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piLen = NULL;
    int *piLen2 = NULL;
    char **pstData = NULL;
    char **pstData2 = NULL;
    double font_scale = 0,thickness = 0;
    int i;

    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    //for string
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //Now, we will retrieve the string from the input parameter. For this, we will require 3 calls
    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //for fontface
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
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
    piLen2 = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen2, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstData2 = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        pstData2[i] = (char*)malloc(sizeof(char) * (piLen2[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen2, pstData2);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //for value of font scale
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &font_scale);
    if(intErr)
        return intErr;

    //for value of thickness
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &thickness);
    if(intErr)
        return intErr;

    int fontFace;

    if(strcmp(pstData2[0],"FONT_HERSHEY_SIMPLEX")==0)
           fontFace = FONT_HERSHEY_SIMPLEX;
    else if (strcmp(pstData2[0],"FONT_HERSHEY_PLAIN")==0)
          fontFace= FONT_HERSHEY_PLAIN;
    else if (strcmp(pstData2[0],"FONT_HERSHEY_DUPLEX")==0)
          fontFace= FONT_HERSHEY_DUPLEX;
    else if (strcmp(pstData2[0],"FONT_HERSHEY_COMPLEX")==0)
          fontFace= FONT_HERSHEY_COMPLEX;
    else if (strcmp(pstData2[0],"FONT_HERSHEY_TRIPLEX")==0)
          fontFace = FONT_HERSHEY_TRIPLEX;
    else if (strcmp(pstData2[0],"FONT_HERSHEY_COMPLEX_SMALL")==0)
          fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    else if (strcmp(pstData2[0],"FONT_HERSHEY_SCRIPT_SIMPLEX")==0)
         fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    else if (strcmp(pstData2[0],"FONT_HERSHEY_SCRIPT_COMPLEX")==0)
         fontFace = FONT_HERSHEY_SCRIPT_COMPLEX;

    int baseline;
    Size sz = getTextSize(pstData[0],fontFace,font_scale,thickness,&baseline); 
    double tempsize[2];
    tempsize[0]=sz.width;
    tempsize[1]=sz.height;
    double returnbaseline = (double)baseline;

    sciErr = createMatrixOfDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,1,2,tempsize);
    if(sciErr.iErr)
    {
        printError(&sciErr,0);
        return 0;
    }
    sciErr = createMatrixOfDouble(pvApiCtx,nbInputArgument(pvApiCtx)+2,1,1,&returnbaseline);
    if(sciErr.iErr)
    {
        printError(&sciErr,0);
        return 0;
    }

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}