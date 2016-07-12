/********************************************************
Author: Vinay

Function: h = ftrans2(b, t)
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;

void rotate180(Mat &m) {
    double temp;
    for (int i=0; i<(m.rows+1)/2; i++) {
        int k = m.cols;
        if ((i+1)>=((m.rows+1)/2) && m.rows%2!=0) {
            k = (m.cols+1)/2;
        }
        for (int j=0; j<k; j++) {
            temp = m.at<double>(i, j);
            m.at<double>(i, j) = m.at<double>(m.rows-i-1, m.cols-j-1);
            m.at<double>(m.rows-i-1, m.cols-j-1) = temp;
        }
    }

}  

Mat fftshift(Mat m) {
    int a = m.rows/2;
    int b = m.cols/2;
    Mat r = Mat::zeros(m.size(), m.type());
    for (int i=0; i<m.rows; i++) {
        for (int j=0; j<m.cols; j++) {
            r.at<double>((i+a)%m.rows, (j+b)%m.cols) = m.at<double>(i, j);
        }
    }
    return r;
}

extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "sciprint.h"
  #include "../common.h"

  int opencv_ftrans2(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int cRows=0,cCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat b, bcpy, t;
    Mat P0, P1, P2, h, hh;

    retrieveImage(bcpy, 1);
    bcpy.convertTo(b, CV_64F);

    if (b.cols == 1) {
        transpose(b, b);
    }
    else if (b.rows == 1) {

    }
    else {
        sciprint("b at argument 1 expected to be vector");
        return 0;
    }

    if (b.cols%2 == 0) {
        sciprint("b at argument 1 must be of odd length");
        return 0;
    }

    Mat c = b.clone();
    rotate180(c);

    int zeroCount = 0;
    double eps = 1.5e-5;

    for (int i=0; i<b.cols; i++) {
        if (abs(b.at<double>(0, i) - c.at<double>(0, i)) > eps) {
            zeroCount = b.cols;
            break;
        }
        if (b.at<double>(0, i) == 0) {
            zeroCount++;
        }
    } 
    if (zeroCount == b.cols) {
        sciprint("b at argument 1 must be nonzero and symmetric");
        return 0;
    }


    if (nbInputArgument(pvApiCtx) == 2) {
        Mat tcpy;
        retrieveImage(tcpy, 2);
        tcpy.convertTo(t, CV_64F);

    }
    else {
        Mat tcpy;
        double data[3][3] = {{1.0/8, 2.0/8, 1.0/8},{2.0/8, -4.0/8, 2.0/8},{1.0/8, 2.0/8, 1.0/8}};
        tcpy = Mat(3, 3, CV_64F, &data);
        t = tcpy.clone();
    }
        
    rotate180(b);

    fftshift(b).copyTo(b);

    rotate180(b);

    int inset1 = (t.rows-1)/2;
    int inset2 = (t.cols-1)/2;
    int n = (b.cols-1)/2;

    Mat a = Mat::zeros(1, n+1, CV_64F);
    a.at<double>(0, 0) = b.at<double>(0,0);
    for (int i=1; i<=n; i++) {
        a.at<double>(0, i) = 2*b.at<double>(0, i);
    }

    P0 = Mat::ones(1, 1, CV_64F);
    t.copyTo(P1);
    h = (P1*a.at<double>(0, 1));

    h.at<double>(inset1,inset2) = h.at<double>(inset1,inset2) + a.at<double>(0, 0);

    for (int i=2; i<=n; i++) {
        Mat src = t.clone();

        int additionalRows = P1.rows-1, additionalCols = P1.cols-1;

        copyMakeBorder(src, src, (additionalRows+1)/2, additionalRows/2, (additionalCols+1)/2, additionalCols/2, BORDER_CONSTANT, Scalar(0));
        
        Point anchor(P1.cols - P1.cols/2 - 1, P1.rows - P1.rows/2 - 1);
        int borderMode = BORDER_CONSTANT;
        Mat P11;
        flip(P1, P11,-1);
        filter2D(src, P2, src.depth(), P11, anchor, 0, borderMode);

        P2 = P2 * 2;

        for (int x=0; x<P0.rows; x++) {
            for (int y=0; y<P0.cols; y++) {
                P2.at<double>(x + 2*inset1, y+ 2*inset2) = P2.at<double>(x + 2*inset1, y+ 2*inset2) - P0.at<double>(x, y);
            }
        }
        h.copyTo(hh);
        h = (P2*a.at<double>(0, i));
        for (int x=0; x<P1.rows; x++) {
            for (int y=0; y<P1.cols; y++) {
                h.at<double>(x + inset1, y + inset2) = h.at<double>(x + inset1, y + inset2) + hh.at<double>(x, y);
            }
        }

        P1.copyTo(P0);
        P2.copyTo(P1);

    }

    rotate180(h);

    Mat hcopy;
    h.copyTo(hcopy);

    string tempstring = type2str(hcopy.type());
    char *imtype;
    imtype = (char *)malloc(tempstring.size() + 1);
    memcpy(imtype, tempstring.c_str(), tempstring.size() + 1);
    returnImage(imtype,hcopy,1);
    free(imtype);


    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);            
    return 0;

  }
/* ==================================================================== */
}

