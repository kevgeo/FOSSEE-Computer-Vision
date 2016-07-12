/********************************************************
Author: Vinay Bhat
********************************************************
Usage: return_image = roipoly(input_image, column_list, row_list)
Example:
  im = roipoly(image, [0 100 100 0], [0 0 100 100])
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

  int opencv_roipoly(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRowsR=0,iColsR=0,iColsC=0,iRowsC=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    double *pstDataR = NULL;
    double *pstDataC = NULL;
    int i, number_of_points;
    int lineType = 8;

    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    // Get the input image from the Scilab environment
    Mat image;
    retrieveImage(image, 1);

    // Get the address of 2nd argument, the column list
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    // Get the address of the 3rd agument, the row list
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    // Get the column list in the form of a matrix 
    // No. of columns = No. of elements in the list
    // No. of rows = 1
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRowsC, &iColsC, &pstDataC);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iRowsC != 1)
    {
        sciprint("Please enter a list of column coordinates.\n");
        return 0;
    }
    
    // Get the row list in the form of a matrix
    // No. of columns = No. of elements in the list
    // No. of rows = 1
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRowsR, &iColsR, &pstDataR);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iRowsR != 1)
    {
        sciprint("Please enter a list of row coordinates.\n");
        return 0;
    }

    if (iColsC != iColsR)
    {
        sciprint("Please ensure number of elements in both row and column lists are equal.\n");
        return 0;
    }

    // Number of points is number of columns
    number_of_points = iColsC;

    // Create the n points which define
    // the polygon
    Point points[1][number_of_points];
    
    for (i = 0; i < number_of_points; i++)
    {
        if (pstDataR[i] < 0 || pstDataC[i] < 0)
        {
            sciprint("Coordinates cannot be negative.\n");
            return 0;
        }
        else
            points[0][i] = Point(pstDataR[i], pstDataC[i]);
    }

    const Point* ppt[1] = { points[0] };

    int npt[] = { number_of_points };

    // Create a new, white, blank image same size as of input
    Mat img = Mat::zeros(image.size(), image.type());

    // Call the fillPoly OpenCV function
    // Fill the blank image in the polygon specified
    // by the points
    fillPoly(img, ppt, npt, 1, Scalar(255, 255, 255), lineType);

    string tempstring = type2str(img.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, img, 1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
