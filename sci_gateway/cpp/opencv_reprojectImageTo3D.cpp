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
  
  int opencv_reprojectImageTo3D(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
     
    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    //Input
    Mat image;
    double *ddmat = NULL; //disparity-disparity matrix
    double handlemissingvalues;
  

    //-> Get disparity image.
    retrieveImage(image,1);

    //-> Get disparity-disparity matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &ddmat);
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

    intErr = getScalarDouble(pvApiCtx, piAddr3, &handlemissingvalues); 
    if(intErr)
    {
        return intErr; 
    }

    Mat_<double> Q(iRows,iCols);
    Mat image2(image.size(), CV_32F);
    
    if( handlemissingvalues == 0)
        reprojectImageTo3D(image, image2, Q, false, CV_32F);

    else if( handlemissingvalues == 1)
        reprojectImageTo3D(image, image2, Q, true, CV_32F);    

    //-> Returning Image
    string tempstring = type2str(image2.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, image2, 1);
    free(checker);

    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
