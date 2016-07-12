/********************************************************
Author: Vinay

Function: ind2gray(image, colormap)
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
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


  int opencv_fsamp2(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int cRows=0,cCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    // Mat image, imgcpy;
    // retrieveImage(image, 1);
    // string tempstring = type2str(image.type());
    // char *imtype;
    // imtype = (char *)malloc(tempstring.size() + 1);
    // memcpy(imtype, tempstring.c_str(), tempstring.size() + 1);
    // bool integer = true;
    // int scale = 1;
    // double error = 0;

    if (nbInputArgument(pvApiCtx) == 1) {
        Mat hd, hdcpy;
        retrieveImage(hdcpy, 1);
        hdcpy.convertTo(hd, CV_64F);
        rotate180(hd);

        fftshift(hd).copyTo(hd);

        rotate180(hd);


        Mat padded;                            //expand input image to optimal size
        int m = getOptimalDFTSize( hd.rows );
        int n = getOptimalDFTSize( hd.cols ); // on the border add zero values
        copyMakeBorder(hd, padded, 0, m - hd.rows, 0, n - hd.cols, BORDER_CONSTANT, Scalar::all(0));

        Mat planes[] = {Mat_<double>(padded), Mat::zeros(padded.size(), CV_64F)};
        Mat complexI;
        merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

        dft(complexI, complexI, DFT_INVERSE | DFT_SCALE);            // this way the result may fit in the source matrix

        split(complexI, planes);   

        fftshift(planes[0]).copyTo(planes[0]);
        fftshift(planes[1]).copyTo(planes[1]);
        rotate180(planes[0]);
        rotate180(planes[1]);

        double *re = (double *)malloc(planes[0].rows * planes[0].cols * sizeof(double));
        double *im = (double *)malloc(planes[0].rows * planes[0].cols * sizeof(double));

        for(int i=0;i<planes[0].rows;i++)
        {
            for(int j=0;j<planes[0].cols;j++)
            {
                re[i+planes[0].rows*j]=planes[0].at<double>(i, j);
                //cout<<planes[0].at<double>(i, j)<<" ";
                im[i+planes[0].rows*j]=planes[1].at<double>(i, j);
            }
            //cout<<endl;
        }



        sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &piAddrNew);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }


        //Adding the R value matrix to the list
        //Syntax : createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData)
        sciErr = createComplexMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, planes[0].rows,planes[0].cols, re, im);
        free(re);
        free(im);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

    }
    else if (nbInputArgument(pvApiCtx)==2 || nbInputArgument(pvApiCtx)==3) {
        sciprint("1 or 4 arguments expected.");
        return 0;
    }



    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);            
    return 0;

  }
/* ==================================================================== */
}

