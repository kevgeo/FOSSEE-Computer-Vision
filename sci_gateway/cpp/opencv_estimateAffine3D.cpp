//*******************************************************************************************************
// Authors : Kevin George
//
// estimateAffine3D(src, dst, out, inliers, ransacThreshold, confidence);
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
  
  int opencv_estimateAffine3D(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    Mat src,dst;
    Mat out,inliers;
    double ransacThreshold,confidence;

    //-> Get first input 3d point set
    retrieveImage(src,1);

    //-> Get second input 3d point set
    retrieveImage(dst,2);

    //-> Get ransacThreshold value
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr3, &ransacThreshold);
    if(intErr)
    {
       return intErr;
    }

    //-> Get confidence value
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr4, &confidence);
    if(intErr)
    {
       return intErr;
    }
  
    if( confidence<0 || confidence >1)
    {
        Scierror(999, "confidence value should be between 0 and 1.\n"); 
        return 0; 
    }   

    //-> Calling estimateAffine3D function
    int ret = estimateAffine3D(src, dst, out, inliers, ransacThreshold, confidence);
    

    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(out.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,out,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    //temp variable was not needed, hence has been discarded
    string tempstring2 = type2str(inliers.type());
    checker = (char *)malloc(tempstring2.size() + 1);
    memcpy(checker, tempstring2.c_str(), tempstring2.size() + 1);
    returnImage(checker,inliers,2); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
