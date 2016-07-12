/********************************************************
Author: Sukul Bagai
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
  #include "../common.h"

  int opencv_imcontrast(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int i,j,k;
    double alpha, beta;

    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;


    Mat image;
    retrieveImage(image, 1); 

    //for value of alpha
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &alpha);
    if(intErr)
    {
        return intErr;
    }   

    //for value of beta
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &beta);
    if(intErr)
    {
        return intErr;
    }   


    //we copy the original image into the new_image, and change values of each pixel using value of alpha and beta given by the user
    Mat new_image = Mat::zeros( image.size(), image.type() );

    //changes the attributes of each individual pixel
    for( int y = 0; y < image.rows; y++ )
    { 
        for( int x = 0; x < image.cols; x++ )
        { 
            for( int c = 0; c < 3; c++ )
            {
                new_image.at<Vec3b>(y,x)[c] =
                saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
            }
        }
    }

    
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
