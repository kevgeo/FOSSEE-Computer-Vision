/***************************************************
Author : Dhruti Shah
****************************************************
Usage : return_image = grayconnected(input_image,row,column,tolerance);
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
  
  int opencv_grayconnected(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k;
    double row, column, tolerance=32;

    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for value of row
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &row);
    if(intErr)
        return intErr;

    //for value of column
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &column);
    if(intErr)
        return intErr;

    if(nbInputArgument(pvApiCtx)==4)
    {
        //for value of tolerance
        sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        intErr = getScalarDouble(pvApiCtx, piAddr4, &tolerance);
        if(intErr)
            return intErr;
    }


    if(row<0)
    {
        sciprint("Positive Value Required for Row.");
        return 0;
    }
    if(column<0)
    {
        sciprint("Positive Value Required for Column.");
        return 0;
    }
    if(tolerance<0)
    {
        sciprint("Positive Value Required for tolerance.");
        return 0;
    }
    

    Mat gray_image;
    cvtColor( image, gray_image, CV_BGR2GRAY );
    Mat temp_image = Mat::zeros( image.size(), image.type() );
    Mat image_to_display = Mat::zeros( image.size(), image.type() );

    int value = gray_image.at<uchar>(row,column);

    for(int i=0;i<image.cols;i++)
    {
        for(int j=0;j<image.rows;j++)
        {
            int value1 = gray_image.at<uchar>(i,j);
            if(value1>=value-tolerance && value1<=tolerance+value)
                temp_image.at<uchar>(i,j)=255;
            else
                temp_image.at<uchar>(i,j)=0;
        }
    }

    Mat labelled_image(image.size(), image.type() );
    int nlabels = connectedComponents(temp_image , labelled_image );
    int label = labelled_image.at<uchar>(row,column);

    for(int i=0;i<image.cols;i++)
    {
        for(int j=0;j<image.rows;j++)
        {
            if(labelled_image.at<uchar>(i,j)==label)
                image_to_display.at<uchar>(i,j)=255;
        }
    }

    //returning image
    string tempstring = type2str(image_to_display.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,image_to_display,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
