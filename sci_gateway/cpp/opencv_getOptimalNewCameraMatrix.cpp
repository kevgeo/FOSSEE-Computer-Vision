//*******************************************************************************************************
// Authors : Kevin George
//
// new_cameraMatrix =  getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, Size(width,height) 
//                                                      ,alpha , newImgSize, validPixROI,centerPrincipalPoint);
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
  
  int opencv_getOptimalNewCameraMatrix(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat cameraMatrix, distCoeffs;
    Mat new_cameraMatrix;
    double width,height; // for image size
    Size newImgSize;
    Rect* validPixROI;
    double alpha,centerPrincipalPoint;
    
    //-> Get input camera matrix
    retrieveImage(cameraMatrix,1);

    //-> Get perspective transformation matrix
    retrieveImage(distCoeffs,2);

    //-> Get width of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &width);
    if(intErr)
    {
       return intErr;
    }

    //-> Get height of image
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }


    intErr = getScalarDouble(pvApiCtx, piAddr4, &height);
    if(intErr)
    {
       return intErr;
    }

    //-> Get alpha value
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }


    intErr = getScalarDouble(pvApiCtx, piAddr5, &alpha);
    if(intErr)
    {
       return intErr;
    }

    if( alpha<0 || alpha>1)
    {
        Scierror(999,"Alpha value must be between 0 and 1.");
    }

    //-> Get centralPrincipalPoint value
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }


    intErr = getScalarDouble(pvApiCtx, piAddr6, &centerPrincipalPoint);
    if(intErr)
    {
       return intErr;
    }

    if( centerPrincipalPoint!=0 && centerPrincipalPoint!=1)
    {
        Scierror(999,"centralPrincipalPoint value must be either be 0(false) or 1(true).");
    }

    if( centerPrincipalPoint == 0)
    {
        new_cameraMatrix =  getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, Size(width,height), 
                                                        alpha , newImgSize, validPixROI,false);

    }

    else if( centerPrincipalPoint == 1)
    {
        new_cameraMatrix =  getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, Size(width,height),
                                                             alpha , newImgSize, validPixROI,true); 
    }
    

    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(new_cameraMatrix.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_cameraMatrix,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
