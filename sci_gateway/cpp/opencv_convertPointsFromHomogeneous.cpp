//*******************************************************************************************************
// Authors : Kevin George
//
// sample inputs : output = convertPointsFromHomogeneous(input)
// 
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
  
  int opencv_convertPointsFromHomogeneous(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    double *pstData = NULL;
    double *rectdata = NULL;
    double groupThreshold, eps;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    double *input_pts = NULL;
  
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &input_pts);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    int size = iRows * iCols;
    if(size != 4)
    {
      sciprint("Number of points must only be four.\n");
      return 0;
    } 

    Mat1f src(4, 1); 
    src(0) = -70;
    src(1) = -95;
    src(2) = -120;
    src(3) = 1;

    // Reshape to 4 channel matrix
    Mat temp = src.reshape(4);
    Mat dst;

    //-> Calling convertPointsFromHomogeneous function
    convertPointsFromHomogeneous(temp, dst);

    double *pts = NULL; //output
    pts = (double*)malloc(sizeof(double)*4);

    for(int i=0;i<3;i++)
      pts[i] = dst.at<float>(i);

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1 , 1, 3, pts); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
