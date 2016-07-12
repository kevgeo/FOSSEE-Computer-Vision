/***************************************************
Author : Yash S. Bhalgat
****************************************************
Usage : T = convmtx2(H, m, n);
Example:
	
***************************************************/

#include <numeric>
#include <limits>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
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
  
  int opencv_convmtx2(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int i,j,k;
    double m, n;

    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat H;
    retrieveImage(H,1);

    //for value of m
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &m);
    if(intErr)
        return intErr;

    //for value of n
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &n);
    if(intErr)
        return intErr;


    //taking the cases which can lead to an error
    
    int s1 = H.rows;
	int s2 = H.cols;
    
    Mat T;
    T = Mat::zeros((m-s1+1)*(n-s2+1),m*n,H.type());
    
    if(m<H.rows)
    {
        sciprint("m should be greater than filter height");
        m = H.rows;
    }
    if(n<H.cols)
    {
        sciprint("n should be greater than filter width");
        n = H.cols;
    }


    //my function called
    //my_convmtx2(H, T, m, n);
    k = 0;

	for(int i=0;i<m-s1+1;i++){
		for(int j=0;j<n-s2+1;j++){
			for(int p=0;p<s1;p++){
				Mat roiPart1 = H.rowRange(p,p+1).colRange(0,s2);
				Mat roiPart2 = T.rowRange(k,k+1).colRange((i+p)*n+j,(i+p)*n+j+s2);
				roiPart1.copyTo(roiPart2);
//				for(int q=0;q<s2;q++)
//					T.at<double>(k,(i+p)*n+j+q)=H.at<double>(p,q);
			}
			k++;
		}
	}
    
    //printf("%f %f %f" , H.at<double>(0,0), H.at<double>(1,1), H.at<double>(2,2));
    
    //namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    //imshow( "Display window", T ); 
        
    //returning image
    string tempstring = type2str(T.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,T,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}

