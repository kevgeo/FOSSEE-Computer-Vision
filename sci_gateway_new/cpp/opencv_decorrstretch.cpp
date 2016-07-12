/***************************************************
Author : Dhruti Shah
***************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>
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
  
  
    Mat dstretch(Mat& input)
    {
       CV_Assert(input.channels() > 1);
     
       Mat dataMu, dataSigma, eigDataSigma, scale, stretch;
       Mat targetMean = Mat();

       Mat data = input.reshape(1, input.rows*input.cols);
       
       meanStdDev(input, dataMu, dataSigma);

       PCA pca(data, Mat(), CV_PCA_DATA_AS_ROW);

       sqrt(pca.eigenvalues, eigDataSigma);
       scale = Mat::diag(1/eigDataSigma);
       stretch = Mat::diag(dataSigma);
     
       stretch.convertTo(stretch, CV_32F);
     
       Mat zmudata;
       Mat repMu = repeat(dataMu.t(), data.rows, 1);
       subtract(data, repMu, zmudata, Mat(), CV_32F);

       Mat transformed = zmudata*(pca.eigenvectors.t()*scale*pca.eigenvectors*stretch);
       add(transformed, repMu, transformed, Mat(), CV_32F);

       Mat dstr32f = transformed.reshape(input.channels(), input.rows);
       return dstr32f;
    } 

  int opencv_decorrstretch(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    char **type = NULL;
    int i,j,k;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    Mat lab;
    cvtColor(image, lab, CV_BGR2Lab);
    Mat dstrlab32f = dstretch(lab);
    Mat dstrlab8u;
    dstrlab32f.convertTo(dstrlab8u, CV_8UC3);
    Mat dstrlab2bgr;
    cvtColor(dstrlab8u, dstrlab2bgr, CV_Lab2BGR);
    Mat dstrbgr32f = dstretch(image);
    Mat dstrbgr8u;
    dstrbgr32f.convertTo(dstrbgr8u, CV_8UC3);


    string tempstring = type2str(dstrbgr8u.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,dstrbgr8u,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}

