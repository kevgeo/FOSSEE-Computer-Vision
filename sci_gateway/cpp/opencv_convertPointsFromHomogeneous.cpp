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
  
  int opencv_convertPointsFromHomogeneous(char *fname, unsigned long fname_len)
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

    if( iRows>1 && iCols>1 )
    {
        Scierror(999, "Please enter either row or column vector.\n"); 
                    return 0; 
    }

    int size = (iRows*iCols)/3;
    
    vector<Point3f> src(size);
    int j = 0;
    for(int i = 0; i<size; i++)
    {
        src[i].x = input[j++];
        src[i].y = input[j++];
        src[i].z = input[j++];
    }

    vector<Point2f> dst;
    //-> Calling convertPointsToHomogeneous function
    convertPointsFromHomogeneous(src, dst);
    int size2 = (dst.size()*2)-1;
    double *output = NULL;
    output = (double*)malloc(sizeof(double)*size2);
    j = 0;
    for(int i=0; i<dst.size(); i++)
    {
        output[j++] = dst[i].x;
        output[j++] = dst[i].y;
    }
 
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, 1, j, output); 
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
