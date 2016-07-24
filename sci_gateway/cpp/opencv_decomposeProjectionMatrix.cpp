    //*******************************************************************************************************
// Authors : Kevin George
//
// decomposeProjectionMatrix(projMatrix, cameraMatrix, rotMatrix, transVect,
//                                rotMatrixX,rotMatrixY,rotMatrixZ,eulerAngles);
//               
//*******************************************************************************************************

#include <numeric>
#include "opencv2/core/core.hpp"
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
  #include <sciprint.h>
  #include "../common.h"
  
  int opencv_decomposeProjectionMatrix(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddr = NULL;
    int iRows=0,iCols=0;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 3, 7);

    double *input = NULL;

    //-> Input
    Mat projMatrix = (Mat_<double>(3,4));
    
    //-> Outputs
    Mat cameraMatrix = (Mat_<double>(3,3)); 
    Mat rotMatrix = (Mat_<double>(3,3)); 
    Mat transVect = (Mat_<double>(4,1));
    //-> Optionl outputs
    Mat rotMatrixX = (Mat_<double>(3,3));
    Mat rotMatrixY = (Mat_<double>(3,3)); 
    Mat rotMatrixZ = (Mat_<double>(3,3));
    Mat eulerAngles; 
    
    //-> Get projection matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &input);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if( iRows!=3 || iCols!=4 )
    {
        Scierror(999,"Projection matrix should be 3x4.\n");
            return 0;
    }

    int k = 0;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            projMatrix.at<double>(i,j) = input[i+j*iRows];
        }
    }

    //-> Calling decomposeProjectionMatrix function
    decomposeProjectionMatrix(projMatrix, cameraMatrix, rotMatrix, transVect,
                                rotMatrixX,rotMatrixY,rotMatrixZ,eulerAngles);
    
    double *output1; //For camera matrix 
    output1 = (double*)malloc(sizeof(double)*9);
    
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            output1[i+j*3] = cameraMatrix.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, 3, 3, output1); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 

    double *output2; //For rotMatrix 
    output2 = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            output2[i+j*3] = rotMatrix.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, 3, 3, output2); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2;
    
    double *output3; //For transvect 
    output3 = (double*)malloc(sizeof(double)*4);
    for(int i=0; i<4; i++)
    {
        output3[i] = transVect.at<double>(i,0);   
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+3, 4, 1, output3); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3;

    double *output4; //For rotMatrX
    output4 = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            output4[i+j*3] = rotMatrixX.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+4, 3, 3, output4); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx)+4;

    double *output5; //For rotMatrY
    output5 = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            output5[i+j*3] = rotMatrixY.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+5, 3, 3, output5); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx)+5;
   
    double *output6; //For rotMatrZ
    output6 = (double*)malloc(sizeof(double)*9);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            output6[i+j*3] = rotMatrixZ.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+6, 3, 3, output6); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx)+6;

    double *output7; //For eulerangles
    output7 = (double*)malloc(sizeof(double)*3);
    for(int i=0; i<3; i++)
    {
        output7[i] = eulerAngles.at<double>(i);
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+7, 3, 1, output7); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 7) = nbInputArgument(pvApiCtx)+7;


    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
 }

}
