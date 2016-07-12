/********************************************************
Author: Dhruti Shah
********************************************************
Usage: return_image = imrect(input_image, x_list, y_list)
Example:
  im = imrect(image, [x1 x2], [y1 y2])
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
  #include "sciprint.h"
  #include "../common.h"

  int opencv_imrect(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRowsR=0,iColsR=0,iColsC=0,iRowsC=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    double *pstDataY = NULL;
    double *pstDataX = NULL;
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    // Get the input image from the Scilab environment
    Mat image;
    retrieveImage(image, 1);

    // Get the address of 2nd argument, the x-coordinate list
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    // Get the address of the 3rd agument, the y-coordinate list
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    // Get the x-coordinate list in the form of a matrix 
    // No. of columns = 2
    // No. of rows = 1
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRowsC, &iColsC, &pstDataX);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    // Get the y-coordinate list in the form of a matrix
    // No. of columns = 2
    // No. of rows = 1
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRowsR, &iColsR, &pstDataY);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
  
    Point points[1][2];
    
    for (int i = 0; i < 2 ; i++)
    {
        if (pstDataY[i] < 0 || pstDataX[i] < 0)
        {
            sciprint("Coordinates cannot be negative.\n");
            return 0;
        }
        else
            points[0][i] = Point(pstDataX[i], pstDataY[i]);
    }

    rectangle(image,points[0][0],points[0][1],Scalar(0,0,0),2);

    string tempstring = type2str(image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, image, 1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}