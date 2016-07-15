//*******************************************************************************************************
// Authors : Kevin George
//
// sample inputs : outImage = convertPointsToHomogeneous(inImage, outImage)
// convertPointsToHomogeneous (inImage, outImage)
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
  
  int opencv_convertPointsToHomogeneous(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    double *input = NULL;
    //-> Get points
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

    int size = (iRows*iCols)/2;
    vector<Point2f> src(size);
    int j = 0;
    for(int i = 0; i<size; i++)
    {
        src[i].x = input[j++];
        src[i].y = input[j++];
    }

    vector<Point3f> dst;
    //-> Calling convertPointsToHomogeneous function
    convertPointsToHomogeneous(src, dst);
    
    double *output = NULL;
    output = (double*)malloc(sizeof(double)*size);
    j = 0;
    for(int i=0; i<size; i++)
    {
        output[j++] = dst[i].x;
        output[j++] = dst[i].y;
        output[j++] = dst[i].z;
    }
 
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, 3, size, output); 
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
