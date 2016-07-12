/***************************************************
Author : Sukul Bagai
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
    #include "sciprint.h"
    #include "../common.h"
    #include "../common.cpp"

int opencv_imread(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iLen        = 0;
    //variable info
    int iRows       = 0;
    int iCols       = 0;
    int piRows      = 0;
    int piCols      = 0;  
    int *piAddr = NULL;      
    int *piLen      = NULL;
    char **pstData  = NULL;
    int **pstData1  = NULL;
    int i,j,k=0;


    //Checking number of input and output arguments (enviromnet variable, min arguments, max arguments)
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    
    // get Address of inputs
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);

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
    
    //creating image matrix, by getting the image at the specified path
    Mat img = imread(pstData[0],CV_LOAD_IMAGE_COLOR);

    string tempstring = type2str(img.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,img,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;
}
/* ==================================================================== */
}
