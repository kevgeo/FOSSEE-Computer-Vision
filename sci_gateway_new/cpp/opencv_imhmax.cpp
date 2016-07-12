/***************************************************
Author : Yash S. Bhalgat
****************************************************
Usage : transformed_image = imhmax(input_img, threshold, conn);
		input is a 2-D array
		conn must be equal to 4 or 8
***************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <algorithm>
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
  
  int opencv_imhmax(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int i,j,k;
    double maximum, minimum;
    double threshold, conn;

    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for value of threshold
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &threshold);
    if(intErr)
        return intErr;

    //for value of conn
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &conn);
    if(intErr)
        return intErr;
    

    //taking the cases which can lead to an error
    if(threshold<0)
    {
        sciprint("Positive Value Required for threshold. 1 value was used instead");
        threshold=1;
    }
    
    if(conn!=8) conn=8;

    //connectivity taken is 8
	//main computations
    Mat transformed_image(image.rows,image.cols,image.type());
	transformed_image = image;
	
	for(int x=1; x<image.rows-1; x++){
		for(int y=1; y<image.cols-1; y++){
			double myarray[] = {image.at<double>(x-1,y-1),image.at<double>(x,y-1),image.at<double>(x+1,y-1),image.at<double>(x-1,y),image.at<double>(x+1,y),image.at<double>(x-1,y+1),image.at<double>(x,y+1),image.at<double>(x+1,y+1)};
			maximum = *max_element(myarray, myarray+8);
			minimum = *min_element(myarray, myarray+8);
			if(image.at<double>(x,y) >= maximum){
				if(image.at<double>(x,y)-minimum < threshold) transformed_image.at<double>(x,y) = minimum;
			}
		}
	}
	
	sciprint("Threshold is: %f", &threshold);	
    
    //returning image
    string tempstring = type2str(transformed_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,transformed_image,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
