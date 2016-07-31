/********************************************************
Author: Diwakar Bhardwaj & Kevin George
*********************************************************
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
//  #include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
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
  
  int opencv_stereoRectify(char *fname, unsigned long fname_len)
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
    //stereoParams
    double * D1real = NULL;//Distortion Matrix  5*1
    double * M1real = NULL;//Camera     Matrix  3*3
    double * R1real = NULL;//Rotation   Matrix  3*3
    double * D2real = NULL;//Distortion Matrix  5*1
    double * M2real = NULL;//Camera     Matrix  3*3
    double * R2real = NULL;//Rotation   Matrix  3*3
    double * Treal  = NULL;//Translation vector 3*1
    double * Rreal  = NULL;//Rotation   Matrix 3*3
    double * ImageSize = NULL; //Image size
    
    Mat_<double> M1(3, 3);
    Mat_<double> D1(5, 1);
    Mat_<double> M2(3, 3);
    Mat_<double> D2(5, 1);
    Mat_<double> T(3, 1);
    Mat_<double> R(3, 3);

    CheckInputArgument(pvApiCtx, 7, 7);
    CheckOutputArgument(pvApiCtx, 5, 5) ;
    
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows2, &iCols2, &M1real);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows3, &iCols3, &D1real);
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
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows5, &iCols5, &M2real);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows6, &iCols6, &D2real);
   if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &iRows, &iCols, &ImageSize);
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
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr6, &iRows8, &iCols8, &Rreal);
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
    
    Size imgSize(ImageSize[0],ImageSize[1]);
    Mat R1,R2,P1,P2,Q;

    stereoRectify(M1,D1,M2,D2,imgSize, R, T, R1, R2,P1, P2, Q);

    double *rot1 = NULL;
    rot1 = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            rot1[i+j*3] = R1.at<double>(i,j);
        }
    }

    double *rot2 = NULL;
    rot2 = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            rot2[i+j*3] = R2.at<double>(i,j);
        }
    }

    double *proj1 = NULL;
    proj1 = (double*)malloc(sizeof(double)*12);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            proj1[i+j*4] = P1.at<double>(i,j);
        }
    }
 
    double *proj2 = NULL;
    proj2 = (double*)malloc(sizeof(double)*12);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            proj2[i+j*4] = P2.at<double>(i,j);
        }
    }

    double *ddmat = NULL;
    ddmat = (double*)malloc(sizeof(double)*16);
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            ddmat[i+j*4] = Q.at<double>(i,j);
        }
    }    

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, 3, 3, rot1); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        
        //-> Returning Output
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 
        
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, 3, 3, rot2); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        
        //-> Returning Output
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2; 
      
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+3, 3, 4, proj1); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        
        //-> Returning Output
        AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3; 
    
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+4, 3, 4, proj2); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        
        //-> Returning Output
        AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx)+4; 
  
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+5, 4, 4, ddmat); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        
        //-> Returning Output
        AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx)+5; 
  
    free(D1real);free(M1real);free(R1real);
    free(D2real);free(M2real);free(R2real);
    free(Treal); free(Rreal);free(ImageSize);
    free(piAddr);free(piAddr2);free(piAddr3);
    free(piAddr4);free(piAddr5);free(piAddr6);free(piAddr7);

        ReturnArguments(pvApiCtx);
        return 0;       
 

  }
}