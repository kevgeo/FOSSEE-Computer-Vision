/*
 * integralImage
 *        
 * integralImage in scilab
 *     
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of integralImage method of matlab
// Usage: 
// integralImage(I) : Calculate the integral image of I, I must be grayscale

// method : 'upright' (default)
// method : 'rotated' The area sums are calulated over a rectangle, which is rotated 45 degrees


// Known Changes from Matlab:
/*
 * 1) None, as of now
 */

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C" {
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>
#include "sciprint.h"
#include "../common.h"

int opencv_integralImage(char *fname, unsigned long fname_len) {
	SciErr sciErr;
	int intErr = 0;


	int *piAddr1 = NULL;

	int error;

// String holding the second argument
	int iRet = 0;
	char* pstData = NULL;

// Checking input argument
	CheckInputArgument(pvApiCtx, 1, 2);
	CheckOutputArgument(pvApiCtx, 1, 1);

// Get input image

	Mat image;
	retrieveImage(image, 1);



  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      sciprint("%f ", image.at<double>(i,j));

    }
    
 sciprint("\n");	
  }

// Error Checks

	if (image.channels() > 1) {
		sciprint("The image must be grayscale.");
		return 0;
	}

// Output variables holding integralImage, squared integralImage, integralImage over rectangle rotated by 45 degrees
	Mat new_image, integralimage, squaredimage, rotatedimage;

	integral(image, integralimage, squaredimage, rotatedimage, -1);

// Get the number of input arguments
	int inputarg = *getNbInputArgument(pvApiCtx);

	if (inputarg == 1)
		integralimage.copyTo(new_image);

	if (inputarg == 2)

	{
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr1);

		if (sciErr.iErr) {
			printError(&sciErr, 0);
			return 0;
		}

		if (isStringType(pvApiCtx, piAddr1)) {
			if (isScalar(pvApiCtx, piAddr1)) {

				iRet = getAllocatedSingleString(pvApiCtx, piAddr1, &pstData);
			}
		}
		if (strcmp(pstData, "rotated") == 0) {
			rotatedimage.copyTo(new_image);
		} else if (strcmp(pstData, "upright") == 0) {
			integralimage.copyTo(new_image);
		} else {
			sciprint("Unknown Parameter Name:%s\n", pstData);

		}

	}

	  // sciprint("\n");

  //  for (int i = 0; i < new_image.rows; i++) {
  //    for (int j = 0; j < new_image.cols; j++) {
  //      sciprint("%f ", new_image.at<double>(i,j));

  //    }

  // sciprint("\n");
  //  }

  // new_image is sent to scilab as output

	int temp = nbInputArgument(pvApiCtx) + 1;
	string tempstring = type2str(new_image.type());
	char *checker;
	checker = (char *) malloc(tempstring.size() + 1);
	memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
	returnImage(checker, new_image, 1);
	free(checker);

	//Assigning the list as the Output Variable
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//Returning the Output Variables as arguments to the Scilab environment
	ReturnArguments (pvApiCtx);
	return 0;

}
/* ==================================================================== */
}

