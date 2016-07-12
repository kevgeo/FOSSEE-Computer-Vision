/********************************************************
Author: Vinay Bhat & Kevin George
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

//**************************************************************** Actual Processing ***********************************************************    
    vector<vector<Point2f> > points(1);
    number_of_points = iColsC;
    for (int i = 0; i < number_of_points; i++)
    {   
        //points[0][i] = Point2f(pstDataR[i], pstDataC[i]);
        points[0].push_back( Point2f(pstDataR[i], pstDataC[i]));
    }

     Mat bimask( image.size(), CV_32FC1 );

     for( int i = 0; i < image.rows; i++ )
    {   
        for( int j = 0; j < image.cols; j++ )
        { 
          bimask.at<float>( i,j) = pointPolygonTest( points[0], Point2f(i,j), true ); 
        }
    }

    //imshow("image",bimask);
    //waitKey(0);    

    string tempstring = type2str(bimask.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, bimask, 1);
    free(checker);


    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
