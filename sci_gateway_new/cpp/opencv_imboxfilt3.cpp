/***************************************************
Author : Yash S. Bhalgat
****************************************************
Usage :
	1) box_filtered_image = imboxfilt3(input_img);
		In this usage, the default filter size of 3x3 is used.
	
	2) box_filtered_image = imboxfilt3(input_img, filter_height, filter_width);

Example : 
	img = imread("lena.jpg");
	imshow(img);
	box_filtered_img = imboxfilt3(img, 9, 9);
	imshow(box_filtered_img);
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
  
  int opencv_imboxfilt3(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int i,j,k;
	
	//Default filter size
	double filter_height = 3;
	double filter_width = 3;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
	
	int inputarg = *getNbInputArgument(pvApiCtx);
	
	
    Mat image;
    retrieveImage(image,1);
	
	
	if(inputarg >= 2){
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
	}


    //taking the cases which can lead to an error
    Mat filtered_image(image.rows,image.cols,image.type());
    
    if(filter_height<0)
    {
        sciprint("Positive Value Required for Height. 3 value was used instead");
        filter_height=3;
    }
    if(filter_width<0)
    {
        sciprint("Positive Value Required for Width. 3 value was used instead");
        filter_width=3;
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
    //if both filter_size is not given, default value of 3x3 is used
    boxFilter(image,filtered_image, -1, sz);

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
