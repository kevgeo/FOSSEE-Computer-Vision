/*************************************************************************************
Author : Yash S. Bhalgat

References : Kaiming He, Jian Sun, Xiaoou Tang, "Guided Image Filtering", IEEE Transactions 
			on Pattern Analysis & Machine Intelligence, vol.35, no. 6, 
			pp. 1397-1409, June 2013, doi:10.1109/TPAMI.2012.213
***************************************************************************************
Usage :
	Covers all the cases of I and p being grayscale and RGB

	1) guidedFiltered_image = imguidedfilter(input_img);
		In this usage, the image itself is used as the guidance image.
	
	2) guidedFiltered_image = imguidedfilter(input_img, guidance_img);
	
	The following two functionalities are not provided in Matlab in this way:
	3) guidedFiltered_image = imguidedfilter(input_img, guidance_img, r);
	4) guidedFiltered_image = imguidedfilter(input_img, guidance_img, r, eps);

Example : 
	img = imread("lena.jpg");
	imshow(img);
	guidedFiltered_image = imguidedfilter(img, img, 9);
	imshow(guidedFiltered_image);
***********************************************************************/

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
  
  int opencv_imguidedfilter(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k;
	
	//Default filter size
	double r= 4;
	double eps= 0.2 * 0.2 * 255 * 255;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
	
	int inputarg = *getNbInputArgument(pvApiCtx);
	
	
    Mat I;
    retrieveImage(I,1);
	
	Mat p=I;
	
	if(inputarg > 1){
		//for value of filter_height
		//sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
		//if (sciErr.iErr)
		//{
		//	printError(&sciErr, 0);
		//	return 0;
		//}
		//intErr = getScalarDouble(pvApiCtx, piAddr2, &filter_height);
		//if(intErr)
		//	return intErr;
		Mat G;
		retrieveImage(G,1);
		p=G;
		
		if(inputarg > 2){
			sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
			if (sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			intErr = getScalarDouble(pvApiCtx, piAddr3, &r);
			if(intErr)
				return intErr;
		}
		if(inputarg > 3){
			sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
			if (sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			intErr = getScalarDouble(pvApiCtx, piAddr4, &eps);
			if(intErr)
				return intErr;
		}
	}


    Mat guidedFiltered_image;


    //Main function
    //if both filter_size is not given, default value of 3x3 is used
	if(I.channels() == 1){
		Mat mean_p;
		blur(p, mean_p, Size(r, r));
		
		Mat mean_Ip;
		blur(I.mul(p), mean_Ip, Size(r, r));
		
		Mat mean_I;
		blur(I, mean_I, Size(r, r));
		
		Mat mean_II;
		blur(I.mul(I), mean_II, Size(r,r));
		
		Mat var_I = mean_II - mean_I.mul(mean_I);
		
		Mat cov_Ip = mean_Ip - mean_I.mul(mean_p); //covariance of (I, p) in each local patch.

		Mat a = cov_Ip / (var_I + eps);
		Mat b = mean_p - a.mul(mean_I);

		Mat mean_a;
		blur(a, mean_a, Size(r, r));
		Mat mean_b;
		blur(b, mean_b, Size(r, r));

		guidedFiltered_image = mean_a.mul(I) + mean_b;
    }
    else if(I.channels()==3 && p.channels()==1){
		vector<cv::Mat> Ichannels, channels;
		split(I, Ichannels);
		guidedFiltered_image = I;
		//split(guidedFiltered_image, channels);
		
		Mat mean_p, mean_I, mean_Ip;
		Mat mean_II, var_I, cov_Ip, a, b;
		Mat mean_a, mean_b;

		for(i=0; i<3; i++){
			blur(p, mean_p, Size(r, r));
			
			blur(Ichannels[i].mul(p), mean_Ip, Size(r, r));
			
			blur(Ichannels[i], mean_I, Size(r, r));
			
			blur(Ichannels[i].mul(Ichannels[i]), mean_II, Size(r,r));
			
			var_I = mean_II - mean_I.mul(mean_I);
			
			cov_Ip = mean_Ip - mean_I.mul(mean_p); //covariance of (Ichannels[i], p) in each local patch.

			a = cov_Ip / (var_I + eps);
			b = mean_p - a.mul(mean_I);

			blur(a, mean_a, Size(r, r));
			blur(b, mean_b, Size(r, r));

			channels.push_back(mean_a.mul(Ichannels[i]) + mean_b);
		}
		merge(channels,guidedFiltered_image);
	}
	else if(I.channels()==3 && p.channels()==3){
		vector<cv::Mat> Ichannels, channels, pchannels;
		
		split(I, Ichannels);
		guidedFiltered_image = I;
		//split(guidedFiltered_image, channels);
		split(p, pchannels);
		
		Mat mean_p, mean_I, mean_Ip;
		Mat mean_II, var_I, cov_Ip, a, b;
		Mat mean_a, mean_b;

		for(i=0; i<3; i++){
			blur(pchannels[i], mean_p, Size(r, r));
			
			blur(Ichannels[i].mul(pchannels[i]), mean_Ip, Size(r, r));
			
			blur(Ichannels[i], mean_I, Size(r, r));
			
			blur(Ichannels[i].mul(Ichannels[i]), mean_II, Size(r,r));
			
			var_I = mean_II - mean_I.mul(mean_I);
			
			cov_Ip = mean_Ip - mean_I.mul(mean_p); //covariance of (Ichannels[i], pchannels[i]) in each local patch.

			a = cov_Ip / (var_I + eps);
			b = mean_p - a.mul(mean_I);

			blur(a, mean_a, Size(r, r));
			blur(b, mean_b, Size(r, r));

			channels.push_back(mean_a.mul(Ichannels[i]) + mean_b);
		}
		merge(channels,guidedFiltered_image);
	}

    //returning image
    string tempstring = type2str(guidedFiltered_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,guidedFiltered_image,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
