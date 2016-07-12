/***************************************************
Author : Yash S. Bhalgat
****************************************************
Usage : filtered_image = imgaussfilt3(input_img, filter_height, filter_width, sigmaX, sigmaY);
Example : 
	img = imread("lena.jpg");
	imshow(img);
	filtered_img = imgaussfilt3(img, 9, 9, 3, 3);
	imshow(filtered_img);
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
  
  int opencv_imgaussfilt3(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k;
    double filter_width,filter_height,sigmaX,sigmaY;

    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for value of filter_height
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &filter_height);
    if(intErr)
        return intErr;

    //for value of filter_width
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &filter_width);
    if(intErr)
        return intErr;

    //for value of sigmaX
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &sigmaX);
    if(intErr)
        return intErr;

    //for value of sigmaY
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &sigmaY);
    if(intErr)
        return intErr;

    //taking the cases which can lead to an error
    Mat filtered_image(image.rows,image.cols,CV_8UC3);
    if(filter_height<0)
    {
        sciprint("Positive Value Required for Height. 1 value was used instead");
        filter_height=1;
    }
    if(filter_width<0)
    {
        sciprint("Positive Value Required for Width. 1 value was used instead");
        filter_width=1;
    }
    if((int)filter_height%2==0)
    {
        filter_height+=1;
        sciprint("Odd Value Required for Height. %f value was used instead",&filter_height);
    }
    if((int)filter_width%2==0)
    {
        filter_width+=1;
        sciprint("Odd Value Required for Width. %f value was used instead",&filter_width);
    }

    //temporary size variable, to use in function
    Size sz(filter_height,filter_width);

    //opencv function called
    //if both sigmaX, sigmaY are set to zero, they are calaulated from the filter_size
    GaussianBlur(image,filtered_image,sz,sigmaX,sigmaY);

    //returning image
    string tempstring = type2str(filtered_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,filtered_image,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
