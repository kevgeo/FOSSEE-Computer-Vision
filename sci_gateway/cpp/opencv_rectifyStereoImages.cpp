
/********************************************************
Author: Diwakar Bhardwaj
*********************************************************
Usage :[image1,image2] = rectifyStereoImages(input_image1,cameramatrix1,Distortion_coff1,input_image2,cameramatrix2,Distortioncoff2,Translationvector,Rotationvector)
********************************************************/

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
  
  int opencv_rectifyStereoImages(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0,iRows2=0,iCols2=0,iRows3=0,iCols3=0,iRows4=0,iCols4=0,iRows5=0,iCols5=0;
    int iRows6=0,iCols6=0,iRows7=0,iCols7=0,iRows8=0,iCols8=0,iRows9=0,iCols9=0,iRows10=0,iCols10=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    int *piAddr8  = NULL;
    int *piAddr9  = NULL;
    int *piAddr10 = NULL;
    //stereoParams
    double * D1real = NULL;//Distortion Matrix  5*1
    double * M1real = NULL;//Camera     Matrix  3*3
    double * P1real = NULL;//Projection Matrix  3*3
    double * R1real = NULL;//Rotation   Matrix  3*3
    double * D2real = NULL;//Distortion Matrix  5*1
    double * M2real = NULL;//Camera     Matrix  3*3
    double * P2real = NULL;//Projection Matrix  3*3
    double * R2real = NULL;//Rotation   Matrix  3*3
    double * Treal  = NULL;//Translation vector 3*1
    double * Rreal  = NULL;//Rotation   Matrix 3*3
    
    Mat D1(5,1,CV_64F);
    Mat M1(3,3,CV_64F);
    Mat D2(5,1,CV_64F);
    Mat M2(3,3,CV_64F);
    Mat T (3,1,CV_64F);
    Mat R (3,3,CV_64F);

    CheckInputArgument(pvApiCtx, 8, 8);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    Mat image1,image2;
    retrieveImage(image1,1);
    
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows2, &iCols2, &M1real);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows3, &iCols3, &D1real);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    retrieveImage(image2,4);

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &iRows5, &iCols5, &M2real);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr6, &iRows6, &iCols6, &D2real);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr7, &iRows7, &iCols7, &Treal);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr7, &iRows8, &iCols8, &Rreal);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if(iRows2!=3 and iCols2!=3 or iRows5!=3 and iCols5!=3)
    {
      sciprint("camera matrix  must be size of 3*3");
    }

    if(iRows3!=1 and iCols3!=5 or iRows6!=1 and iCols6!=5)
    {
      sciprint("Distortion cofficient  matrix  must be size of 5*1");
    }
    if(iRows7!=1 and iCols7!=3)
    {
      sciprint("Translation vector must be 3*1");
    }


    if(iRows8!=3 and iCols8!=3)
    {
      sciprint("Rotation Matrix must be 3*3");
    }
    
    for(int i=0;i<3;i++)
      for(int j=0;j<3;j++)
             M1.at<double>(i,j)=M1real[i+j*3];

    for(int i=0;i<5;i++)
             for(int j=0; j<1; ++j)
                     D1.at<double>(i,j)=D1real[i+j*5];
    
    for(int i=0;i<3;i++)
      for(int j=0;j<3;j++)
             M2.at<double>(i,j)=M2real[i+j*3];

    for(int i=0;i<5;i++)
             for(int j=0; j<1; ++j)
             D2.at<double>(i,j)=D2real[i+j*5];

    for(int i=0; i<3; ++i)
                for(int j=0; j<1; ++j)
                    T.at<double>(i,j)=Treal[i+j*3];

    for(int i=0;i<3;i++)
      for(int j=0;j<3;j++)
             R.at<double>(i,j)=Rreal[i+j*3];
    
    Size imgSize(image1.rows*2,image1.cols*2);
    Mat R1,R2,P1,P2,Q;

    stereoRectify(M1,D1,M2,D2, imgSize, R, T, R1, R2,P1, P2,  Q);


    Mat left, right; //Create matrices for storing input images
 
   // transformation and rectification maps
    Mat camera1map1, camera1map2;
    Mat camera2map1, camera2map2;
    
    initUndistortRectifyMap(M1, D1, R1, P1, imgSize , CV_32F, camera1map1, camera1map2);
    initUndistortRectifyMap(M2, D2, R2, P2, imgSize, CV_32F, camera2map1, camera2map2);
 
    Mat leftStereoUndistorted, rightStereoUndistorted;
    //Rectify and undistort images
    remap(image1, left, camera1map1, camera1map2, INTER_LINEAR);
    remap(image2, right, camera2map1, camera2map2, INTER_LINEAR);
    /*Size size(image1.rows,image1.cols);
    resize(image1,left,size);
    Size size1(image2.rows,image2.cols);
    resize(image2,right,size1);
     Combining left and right rectified image in one image plane
    Size sz1 = left.size();
    Size sz2 = right.size();
    Mat final_image (sz1.height, sz1.width+sz2.width, CV_8UC3);
    final_image.adjustROI(0, 0, 0, -sz2.width);
    left.copyTo(final_image);
    final_image.adjustROI(0, 0, -sz1.width, sz2.width);
    right.copyTo(final_image);
    final_image.adjustROI(0, 0, sz1.width, 0);*/
 
    int temp1 = nbInputArgument(pvApiCtx) + 1;
    string tempstring1 = type2str(left.type());
    char *checker1;
    checker1 = (char *)malloc(tempstring1.size() + 1);
    memcpy(checker1, tempstring1.c_str(), tempstring1.size() + 1);
    returnImage(checker1,left,1);
    free(checker1);

    int temp2 = nbInputArgument(pvApiCtx) + 2;
    string tempstring2 = type2str(right.type());
    char *checker2;
    checker2 = (char *)malloc(tempstring2.size() + 2);
    memcpy(checker2, tempstring2.c_str(), tempstring2.size() + 2);
    returnImage(checker2,right,2);
    free(checker2); 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}