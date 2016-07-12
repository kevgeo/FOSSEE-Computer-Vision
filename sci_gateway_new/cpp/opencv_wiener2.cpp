/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Created By: Riddhish Bhalodia
Date: 14th October 2015

Usage:

void wiener2(Mat img,int n, double sigma)

1)  img   : Input image, grayscale only
2)  n     : filt size
3)  sigma : noise var, if sigma = 0 then the variance is estimated from data

*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

  int opencv_wiener2(char *fname, unsigned long fname_len){
	
	SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    double n;
    double sigma;
    
    
	Mat img;
    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
	
	retrieveImage(img, 1);
	//for value of the scale factor
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &n);
    if(intErr)
    {
        return intErr;
    }   

    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &sigma);
    if(intErr)
    {
        return intErr;
    }   

    // The error checks for the function
    if(n != round(n) || n<=0 || int(n)%2==0)
    {
        sciprint("The value of n must be an odd integer \n");
        return 0;
    }

    Mat out = Mat::zeros(img.size(),img.type());
	sigma=255*255*sigma;
	if(img.channels() !=1){
		sciprint("Grayscale images only!");
		return 0;
	}
	
		int i_min,i_max,j_min,j_max;
		int w = (n-1)/2;

		if(sigma==0){
			double noise_est = 0;
			for(int i=0;i<img.rows;i++){
				for(int j=0;j<img.cols;j++){
					i_min = max(i-w,0);
			        i_max = min(i+w,img.rows-1);
			        j_min = max(j-w,0);
			        j_max = min(j+w,img.cols-1);

			        // find mean of the patch
			        double mean = 0;
			        for(int ii=i_min;ii<(i_max+1);ii++){
			        	for(int jj=j_min;jj<(j_max+1);jj++){
			        		mean = mean + ((double)img.at<uchar>(ii,jj));

			        	}
			        }
			        mean = mean/(n*n);
			        
			        double var=0;
			        for(int ii=i_min;ii<(i_max+1);ii++){
			        	for(int jj=j_min;jj<(j_max+1);jj++){
			        		var = var + ((double)img.at<uchar>(ii,jj))*((double)img.at<uchar>(ii,jj));
			        	}
			        }
			        var = var/(n*n);
			        var = var - mean*mean;
			        noise_est = noise_est + var;
				}
			}
			noise_est = noise_est/(img.rows*img.cols);
			sigma = noise_est;
		}

		for(int i=0;i<img.rows;i++){
			for(int j=0;j<img.cols;j++){
				i_min = max(i-w,0);
		        i_max = min(i+w,img.rows-1);
		        j_min = max(j-w,0);
		        j_max = min(j+w,img.cols-1);

		        // find mean of the patch
		        double mean = 0;
		        for(int ii=i_min;ii<(i_max+1);ii++){
		        	for(int jj=j_min;jj<(j_max+1);jj++){
		        		mean = mean + ((double)img.at<uchar>(ii,jj));

		        	}
		        }
		        mean = mean/(n*n);

		        // find variance of the patch
		        double var=0;
		        for(int ii=i_min;ii<(i_max+1);ii++){
		        	for(int jj=j_min;jj<(j_max+1);jj++){
		        		var = var + ((double)img.at<uchar>(ii,jj))*((double)img.at<uchar>(ii,jj));
		        	}
		        }
		        var = var/(n*n);
		        var = var - mean*mean;
		        
		        double temp;
		        double sum=0;
		        
		       	temp = mean + (fmax(0,(var - sigma))/fmax(var,sigma))*(((double)img.at<uchar>(i,j)) - mean);
		        out.at<uchar>(i,j) = temp;
			}
		}
		
	
	// out is the return image
	string tempstring = type2str(out.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,out,1);
    free(checker); 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    ReturnArguments(pvApiCtx);
    return 0;


  }
}