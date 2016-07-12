/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Created By: Riddhish Bhalodia
Date: 29th September 2015

Usage:

Mat dst =  imfuse(Mat image1,Mat image2,String method,String scaling)

1)  image1   : Input image1
2)  image2   : Input image2
3)  method : Method for the type of display
	
	| Method 		| Function													|
	|				|															|	
	| "montage"		| Display Images side by side								|
	| "blend"		| Display the blend of two Images	alpha=0.5				|
	| "diff"		| Display the absolute difference between Images 			|
	| "falsecolor"	| Display image1 by green channel and image2 by red			|

4)  scaling : The scaling of the images	
	
	| Scaling 		| Function													|
	|				|															|	
	| "none"		| No scaling to the images 									|
	| "joint"		| Scale the joint images									|
	| "independent"	| Scale the images independently before processing 			|
	


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

  int opencv_imfuse(char *fname, unsigned long fname_len){

  	SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piLen1  = NULL;
    int *piLen2  = NULL;
    char **method = NULL;
    char **scaling = NULL;
    
	Mat image1,image2;
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
	
	retrieveImage(image1, 1);
	retrieveImage(image2, 2);

	sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
	//Now, we will retrieve the string from the input parameter. For this, we will require 3 calls
    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen1 = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen1, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    method = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int i = 0 ; i < iRows * iCols ; i++)
        method[i] = (char*)malloc(sizeof(char) * (piLen1[i] + 1));//+ 1 for null termination

    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen1, method);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    // now the fourth argument which is aging a string

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
	//Now, we will retrieve the string from the input parameter. For this, we will require 3 calls
    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen2 = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen2, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    scaling = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int i = 0 ; i < iRows * iCols ; i++)
        scaling[i] = (char*)malloc(sizeof(char) * (piLen2[i] + 1));//+ 1 for null termination

    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen2, scaling);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    char x1[15] = "montage";
	char x2[15] = "diff";
	char x3[15] = "blend";
	char x4[15] = "falsecolor";	
	char y1[15] = "joint";
	char y2[15] = "independent";
	char y3[15] = "none";
    // Error check on method
    if(!(strcmp(method[0],x1)==0 || strcmp(method[0],x2)==0 || strcmp(method[0],x3)==0 || strcmp(method[0],x4)==0 )){
    	sciprint("Method input not correct, choose from 'blend','diff','montage' or 'falsecolor' \n");
        return 0;
    }
    // Error check on scaling

    if(!(strcmp(scaling[0],y1)==0 || strcmp(scaling[0],y2)==0 || strcmp(scaling[0],y3)==0  )){
    	sciprint("Method input not correct, choose from 'none','diff','montage' or 'falsecolor' \n");
        return 0;
    }

    int max_rows,max_cols;
	Mat out;
	max_rows = max(image1.rows,image2.rows);
	max_cols = max(image1.cols,image2.cols);
	if(image1.channels() == 1){ cvtColor(image1,image1,CV_BGR2GRAY);}
	if(image2.channels() == 1){ cvtColor(image2,image2,CV_BGR2GRAY);}

	vector<Mat> rgbChannels(3);
	// for montage and no scaling plus joint scaling

	if(strcmp(method[0],x1)==0 && ((strcmp(scaling[0],y1)==0)||(strcmp(scaling[0],y3)==0))) {		
		out = Mat::zeros(max_rows,2*max_cols,image1.type());	
		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				out.at<Vec3b>(i,j)[0] = image1.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j)[1] = image1.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j)[2] = image1.at<Vec3b>(i,j)[2];
			}
		}
		for(int i=0;i<image2.rows;i++){
			for(int j=0;j<image2.cols;j++){
				out.at<Vec3b>(i,j + max_cols)[0] = image2.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j + max_cols)[1] = image2.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j + max_cols)[2] = image2.at<Vec3b>(i,j)[2];
			}
		}	
	}
	// for montage and scaling independent
	if(strcmp(method[0],x1)==0 && strcmp(scaling[0],y2)==0) {	

		
	    split(image1, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,image1);
		split(image2, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,image2);

		out = Mat::zeros(max_rows,2*max_cols,image1.type());	
		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				out.at<Vec3b>(i,j)[0] = image1.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j)[1] = image1.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j)[2] = image1.at<Vec3b>(i,j)[2];
			}
		}
		for(int i=0;i<image2.rows;i++){
			for(int j=0;j<image2.cols;j++){
				out.at<Vec3b>(i,j + max_cols)[0] = image2.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j + max_cols)[1] = image2.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j + max_cols)[2] = image2.at<Vec3b>(i,j)[2];
			}
		}	
	}
	
	// for diff and no scaling plus joint scaling
	if(strcmp(method[0],x2)==0 && ((strcmp(scaling[0],y1)==0)||(strcmp(scaling[0],y3)==0))) {	
		
		// cvtColor(image1,image1,CV_BGR2GRAY);
		// cvtColor(image2,image2,CV_BGR2GRAY);	
		out = Mat::zeros(max_rows,max_cols,image1.type());

		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				out.at<Vec3b>(i,j)[0] = (image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2])/3;
				out.at<Vec3b>(i,j)[1] = (image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2])/3;
				out.at<Vec3b>(i,j)[2] = (image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2])/3;
			}
		}
		for(int i=0;i<image2.rows;i++){
			for(int j=0;j<image2.cols;j++){
				out.at<Vec3b>(i,j)[0] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
				out.at<Vec3b>(i,j)[1] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
				out.at<Vec3b>(i,j)[2] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
			}
		}
		for(int i=0;i<min(image1.rows,image2.rows);i++){
			for(int j=0;j<min(image1.cols,image2.cols);j++){
				out.at<Vec3b>(i,j)[0] = abs((image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2]) - (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2]))/3;
				out.at<Vec3b>(i,j)[1] = abs((image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2]) - (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2]))/3;
				out.at<Vec3b>(i,j)[2] = abs((image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2]) - (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2]))/3;
			}
		}	
	}	
	// for diff and independent scaling
	if(strcmp(method[0],x2)==0 && strcmp(scaling[0],y2)==0) {	
		
		
	    split(image1, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,image1);
		split(image2, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,image2);

		// cvtColor(image1,image1,CV_BGR2GRAY);
		// cvtColor(image2,image2,CV_BGR2GRAY);	
		out = Mat::zeros(max_rows,max_cols,image1.type());	
		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				out.at<Vec3b>(i,j)[0] = image1.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j)[1] = image1.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j)[2] = image1.at<Vec3b>(i,j)[2];
			}
		}
		for(int i=0;i<image2.rows;i++){
			for(int j=0;j<image2.cols;j++){
				out.at<Vec3b>(i,j)[0] = image2.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j)[1] = image2.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j)[2] = image2.at<Vec3b>(i,j)[2];
			}
		}
		for(int i=0;i<min(image1.rows,image2.rows);i++){
			for(int j=0;j<min(image1.cols,image2.cols);j++){
				out.at<Vec3b>(i,j)[0] = abs(image1.at<Vec3b>(i,j)[0] - image2.at<Vec3b>(i,j)[0]);
				out.at<Vec3b>(i,j)[1] = abs(image1.at<Vec3b>(i,j)[1] - image2.at<Vec3b>(i,j)[1]);
				out.at<Vec3b>(i,j)[2] = abs(image1.at<Vec3b>(i,j)[2] - image2.at<Vec3b>(i,j)[2]);
			}
		}	
	}	
	
	 // for blend and no scaling[0] plus joint scaling[0]
	if(strcmp(method[0],x3)==0 && ((strcmp(scaling[0],y1)==0)||(strcmp(scaling[0],y3)==0))) {	
		out = Mat::zeros(max_rows,max_cols,image1.type());

		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				out.at<Vec3b>(i,j)[0] = image1.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j)[1] = image1.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j)[2] = image1.at<Vec3b>(i,j)[2];
			}
		}
		for(int i=0;i<image2.rows;i++){
			for(int j=0;j<image2.cols;j++){
				out.at<Vec3b>(i,j)[0] = image2.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j)[1] = image2.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j)[2] = image2.at<Vec3b>(i,j)[2];
			}
		}

		for(int i=0;i<min(image1.rows,image2.rows);i++){
			for(int j=0;j<min(image1.cols,image2.cols);j++){
				out.at<Vec3b>(i,j)[0] = 0.5*(image1.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[0]);
				out.at<Vec3b>(i,j)[1] = 0.5*(image1.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[1]);
				out.at<Vec3b>(i,j)[2] = 0.5*(image1.at<Vec3b>(i,j)[2] + image2.at<Vec3b>(i,j)[2]);
			}
		}

	}
	// for blend and independent scaling[0]
	if(strcmp(method[0],x3)==0 && strcmp(scaling[0],y2)==0) {	
		out = Mat::zeros(max_rows,max_cols,image1.type());
		
	    split(image1, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,image1);
		split(image2, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,image2);

		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				out.at<Vec3b>(i,j)[0] = image1.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j)[1] = image1.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j)[2] = image1.at<Vec3b>(i,j)[2];
			}
		}
		for(int i=0;i<image2.rows;i++){
			for(int j=0;j<image2.cols;j++){
				out.at<Vec3b>(i,j)[0] = image2.at<Vec3b>(i,j)[0];
				out.at<Vec3b>(i,j)[1] = image2.at<Vec3b>(i,j)[1];
				out.at<Vec3b>(i,j)[2] = image2.at<Vec3b>(i,j)[2];
			}
		}

		for(int i=0;i<min(image1.rows,image2.rows);i++){
			for(int j=0;j<min(image1.cols,image2.cols);j++){
				out.at<Vec3b>(i,j)[0] = 0.5*(image1.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[0]);
				out.at<Vec3b>(i,j)[1] = 0.5*(image1.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[1]);
				out.at<Vec3b>(i,j)[2] = 0.5*(image1.at<Vec3b>(i,j)[2] + image2.at<Vec3b>(i,j)[2]);
			}
		}

	}
	
	// for falsecolor and no scaling[0] plus joint scaling[0]
	if(strcmp(method[0],x4)==0 && ((strcmp(scaling[0],y1)==0)||(strcmp(scaling[0],y3)==0))) {	
		
		out = Mat::zeros(max_rows,max_cols,image1.type());

		// image 1 green image2 red
		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				out.at<Vec3b>(i,j)[0] = 0;
				out.at<Vec3b>(i,j)[1] = (image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2])/3;
				out.at<Vec3b>(i,j)[2] = 0;
			}
		}
		for(int i=0;i<image2.rows;i++){
			for(int j=0;j<image2.cols;j++){
				out.at<Vec3b>(i,j)[0] = 0;
				out.at<Vec3b>(i,j)[1] = 0;
				out.at<Vec3b>(i,j)[2] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
			}
		}
		for(int i=0;i<min(image1.rows,image2.rows);i++){
			for(int j=0;j<min(image1.cols,image2.cols);j++){
				if(image1.at<Vec3b>(i,j) == image2.at<Vec3b>(i,j)){
					out.at<Vec3b>(i,j)[0] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
					out.at<Vec3b>(i,j)[1] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
					out.at<Vec3b>(i,j)[2] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
				}
				else{
					out.at<Vec3b>(i,j)[0] = 0;
					out.at<Vec3b>(i,j)[1] = (image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2])/3;
					out.at<Vec3b>(i,j)[2] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
				}

			}
		}

	}	
	// for falsecolor and independent scaling[0]
	if(strcmp(method[0],x4)==0 && strcmp(scaling[0],y2)==0) {	
		out = Mat::zeros(max_rows,max_cols,image1.type());
		//scale image 1 and image 2
		
	    split(image1, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,image1);
		split(image2, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,image2);
		// image 1 green image2 red
		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				out.at<Vec3b>(i,j)[0] = 0;
				out.at<Vec3b>(i,j)[1] = (image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2])/3;
				out.at<Vec3b>(i,j)[2] = 0;
			}
		}
		for(int i=0;i<image2.rows;i++){
			for(int j=0;j<image2.cols;j++){
				out.at<Vec3b>(i,j)[0] = 0;
				out.at<Vec3b>(i,j)[1] = 0;
				out.at<Vec3b>(i,j)[2] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
			}
		}
		for(int i=0;i<min(image1.rows,image2.rows);i++){
			for(int j=0;j<min(image1.cols,image2.cols);j++){
				if(image1.at<Vec3b>(i,j) == image2.at<Vec3b>(i,j)){
					out.at<Vec3b>(i,j)[0] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
					out.at<Vec3b>(i,j)[1] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
					out.at<Vec3b>(i,j)[2] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
				}
				else{
					out.at<Vec3b>(i,j)[0] = 0;
					out.at<Vec3b>(i,j)[1] = (image1.at<Vec3b>(i,j)[0] + image1.at<Vec3b>(i,j)[1] + image1.at<Vec3b>(i,j)[2])/3;
					out.at<Vec3b>(i,j)[2] = (image2.at<Vec3b>(i,j)[0] + image2.at<Vec3b>(i,j)[1] + image2.at<Vec3b>(i,j)[2])/3;
				}

			}
		}

	}	

	// for joint scaling[0]
	if(strcmp(scaling[0],y3)==0){
		split(out, rgbChannels);
		normalize(rgbChannels[0],rgbChannels[0],0,255,CV_MINMAX);
		normalize(rgbChannels[1],rgbChannels[1],0,255,CV_MINMAX);
		normalize(rgbChannels[2],rgbChannels[2],0,255,CV_MINMAX);
		merge(rgbChannels,out);
	}

	if(image1.channels()==1 && image2.channels()==1){
		cvtColor(out,out,CV_BGR2GRAY);
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