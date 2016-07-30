//*******************************************************************************************************
// Authors : Kevin George
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
  
  int opencv_filterSpeckles(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr  = NULL;
    int *piAddr4  = NULL;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    //Input
    Mat image;
    double newVal; // disparity value to paint off speckles
    double maxSpeckleSize; // maximum speckle size to consider it a speckle
    double maxDiff;  // maximum difference between neighbor disparity pixels
    //Output

    //-> Get disparity image.
    retrieveImage(image,1);

    //-> Get disparity value to paint off speckles
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr2, &newVal);
    if(intErr)
    {
       return intErr;
    }


    //-> Get maximum speckle size
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &maxSpeckleSize);
    if(intErr)
    {
       return intErr;
    }
   
    //-> Get maximum speckle size
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr4, &maxDiff);
    if(intErr)
    {
       return intErr;
    }
   

    //Calling filterSpeckles function
    filterSpeckles(image,newVal,maxSpeckleSize,maxDiff);

    //-> Returning Image
    string tempstring = type2str(image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, image, 1);
    free(checker);

    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
