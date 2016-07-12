/*************************************************************************************
Author : Yash S. Bhalgat
***************************************************************************************
---------- Performs Contrast Limited Adaptive Histogram Equalisation -------------
Usage :
	1) output_img = adapthisteq(input_img);
		In this usage, the image itself is used as the guidance image.
	
	2) output_img = adapthisteq(input_img, clip_limit);
Example : 
	img = imread("lena.jpg");
	imshow(img);
	output_img = adapthisteq(img, img, 9);
	imshow(output_img);
***********************************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <algorithm>    // std::max

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
  
  
  
  int opencv_adapthisteq(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int i,j,k;
    double clip_limit;
	
	//Default clip limit
	clip_limit = 0.001;
	
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);
	
	int inputarg = *getNbInputArgument(pvApiCtx);
	
	
    Mat input_img;
    retrieveImage(input_img,1);
	
	if(inputarg == 2){
		//for value of clip_limit
		sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		intErr = getScalarDouble(pvApiCtx, piAddr2, &clip_limit);
		if(intErr)
			return intErr;
	}


    //Main function
    //cvtColor(input_img, input_img, CV_RGB2GRAY);
	
	float C[256];
    
    int row = input_img.rows;
    int col = input_img.cols;
    
    int window_x = 90;
    int window_y = 90;
    
    int min_x, min_y, max_x, max_y;
    Mat window_matrix;
    
    //float clip_limit = 0.001;

    int histSize = 256; //from 0 to 255
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	
	Mat H;
    Mat output_img = Mat::zeros(input_img.size(), input_img.type());
	
	if(input_img.channels()==1){
		for(int i=0; i<row; i++){
			for(int j=0; j<col; j++){
				min_x = max(0,i-window_x);
				min_y = max(0,j-window_y);
				max_x = min(row-1,i+window_x);
				max_y = min(col-1,j+window_y);
				window_matrix = input_img(Range(min_x, max_x), Range(min_y, max_y));
				//window_matrix = inputImage(min_x:max_x,min_y:max_y);
				
				if(input_img.at<uchar>(i,j)==0) output_img.at<uchar>(i,j) =0;
				else{
					calcHist(&window_matrix, 1, 0, Mat(), H, 1, &histSize, &histRange);
					
					int N = window_matrix.rows;
					int M = window_matrix.cols;
					H=H/(N*M);
	//				cout<<H<<endl;
					
					for(int z=0; z<256; z++){
						if(H.at<float>(0,z) > clip_limit) H.at<float>(0,z) = clip_limit;
					}
					
					float contrastArea = 1.0 - cv::sum(H).val[0];
					float height = contrastArea / 256.00;
					
					H = H + height;
					//cout<<cv::sum(H).val[0]<<endl;
					C[0] = H.at<float>(0,0)*255;
					for(int k=1; k<256; k++){
						C[k]= C[k-1] + H.at<float>(0,k)*255;
					}
					//cout<<C[255]<<endl;
					output_img.at<uchar>(i,j) = C[input_img.at<uchar>(i,j)];
				}
			}
		}
	}
    else if(input_img.channels()==3){
		vector<cv::Mat> Ichannels;
		split(input_img, Ichannels);
		
		for(int d=0; d<3; d++){
			for(int i=0; i<row; i++){
				for(int j=0; j<col; j++){
					min_x = max(0,i-window_x);
					min_y = max(0,j-window_y);
					max_x = min(row-1,i+window_x);
					max_y = min(col-1,j+window_y);
					
					window_matrix = Ichannels[d](Range(min_x, max_x), Range(min_y, max_y));
										
					if(Ichannels[d].at<uchar>(i,j)==0) output_img.at<Vec3b>(i,j)[d] =0;
					else{
						calcHist(&window_matrix, 1, 0, Mat(), H, 1, &histSize, &histRange);
						
						int N = window_matrix.rows;
						int M = window_matrix.cols;
						H=H/(N*M);
		//				cout<<H<<endl;
						
						for(int z=0; z<256; z++){
							if(H.at<float>(0,z) > clip_limit) H.at<float>(0,z) = clip_limit;
						}
						
						float contrastArea = 1.0 - cv::sum(H).val[0];
						float height = contrastArea / 256.00;
						
						H = H + height;
						//cout<<cv::sum(H).val[0]<<endl;
						C[0] = H.at<float>(0,0)*255;
						for(int k=1; k<256; k++){
							C[k]= C[k-1] + H.at<float>(0,k)*255;
						}
						//cout<<C[255]<<endl;
						output_img.at<Vec3b>(i,j)[d] = C[Ichannels[d].at<uchar>(i,j)];
					}
				}
			}
		}
		
		
	}
    
	

    //returning image
    string tempstring = type2str(output_img.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,output_img,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}

