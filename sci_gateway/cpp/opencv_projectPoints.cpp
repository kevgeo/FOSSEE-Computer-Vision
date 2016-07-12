//*******************************************************************************************************
// Authors : Kevin George
//
// projectPoints(objectPoints,rvec,tvec,cameraMatrix,distCoeffs,aspectRatio);
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
  
  int opencv_projectPoints(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr6  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    Mat objectPoints, rvec, tvec, cameraMatrix, distCoeffs;
    Mat imagePoints, jacobian;
    double aspectRatio;
    int num_InputArgs;

    //->get number of arguments entered
    num_InputArgs = *getNbInputArgument(pvApiCtx);
    
    //-> Get array of object points
    retrieveImage(objectPoints,1);

    //-> Get rotation vector values
    retrieveImage(rvec,2);

    //-> Get translation vector values
    retrieveImage(tvec,3);

    //-> Get camera matrix values
    retrieveImage(cameraMatrix,4);

    //-> Get vector of distortion coefficients
    retrieveImage(distCoeffs,5);

    //-> Get aspect ratio value
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr6, &aspectRatio);
    if(intErr)
    {
       return intErr;
    }    

    // Calling projectPoints function
    projectPoints(objectPoints,rvec,tvec,cameraMatrix,distCoeffs,imagePoints,jacobian,aspectRatio);
    

    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(imagePoints.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,imagePoints,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    //temp variable was not needed, hence has been discarded
    string tempstring2 = type2str(jacobian.type());
    checker = (char *)malloc(tempstring2.size() + 1);
    memcpy(checker, tempstring2.c_str(), tempstring2.size() + 1);
    returnImage(checker,jacobian,2); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
