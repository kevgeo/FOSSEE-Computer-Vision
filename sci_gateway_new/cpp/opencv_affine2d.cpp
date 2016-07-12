/*******************************************************************
Author : Yash S. Bhalgat
********************************************************************
Usage : T = affine2d(A);
Output: T - 3x3 matrix equivalent to tform.T matrix in Matlab output
********************************************************************/

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
  
  int opencv_affine2d(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr1  = NULL;
    char **ddepth = NULL;
    int i,j,k;
    double *inpMatrix;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    //for inputMatrix
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows, &iCols ,&inpMatrix);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    int n=iRows;
    double inputMatrix[n][n];
    //assigning values to actual kernelMatrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            inputMatrix[i][j]=inpMatrix[(i*n)+j];
    // converting the array to a matrix, so that we can pass it into the filter2D function    
	
	Mat inpMat(n, n, CV_64FC1, &inputMatrix);
	
	Point2f src[3];
	Point2f res[3];
    
	Point3d s1 = Point3d(0,0,1);
	Point3d s2 = Point3d(10,0,1);
	Point3d s3 = Point3d(0,5,1);
	//cout<<s1<<endl;
	
	Mat s = (Mat)s1;
	Mat t = inpMat*s;
	Mat r = t.rowRange(0, t.rows-1);
	Mat ss = s.rowRange(0, s.rows-1);
	src[0] = (Point2f)ss;
	res[0] = (Point2f)r;
	
	s = (Mat)s2;
	t = inpMat*s;
	r = t.rowRange(0, t.rows-1);
	ss = s.rowRange(0, s.rows-1);
	src[1] = (Point2f)ss;
	res[1] = (Point2f)r;
	
	s = (Mat)s3;
	t = inpMat*s;
	r = t.rowRange(0, t.rows-1);
	ss = s.rowRange(0, s.rows-1);
	src[2] = (Point2f)ss;
	res[2] = (Point2f)r;
	
	Mat warp_mat = getAffineTransform(src, res);
	
	Mat outputMatrix = Mat::zeros(n, n, warp_mat.type());
	Mat warp_mat_t = warp_mat.t();
	for(i=0; i<3; i++)
		for(j=0; j<2; j++)
			outputMatrix.at<double>(i,j) = warp_mat_t.at<double>(i,j);
	
	outputMatrix.at<double>(0,2) = 0;
	outputMatrix.at<double>(1,2) = 0;
	outputMatrix.at<double>(2,2) = 1.0;
	
    //returning image
    string tempstring = type2str(outputMatrix.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,outputMatrix,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
    
  }
/* ==================================================================== */
}

