//*******************************************************************************************************
// Authors : Kevin George
//
// sample inputs : lines = computeCorrespondEpilines(points, whichImage, F)
// 
// computeCorrespondEpilines(points, whichImage, F, lines)
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
  
  int opencv_computeCorrespondEpilines(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int i,j,k ;
    double *pstData = NULL;
    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    
    Mat points, F, lines;
    int whichImage;
    double number;

     //-> Get the points 
    retrieveImage(points,1);

    //-> Get number denoting the image
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr, &number);
    if(intErr)
    {
       return intErr;
    } 

    whichImage = int(number);

    //-> Get the Fundamental matrix
    retrieveImage(F,3);

    //-> Calling computeCorrespondEpilines function
    computeCorrespondEpilines(points, whichImage, F, lines);

    //temp variable was not needed, hence has been discarded
        string tempstring = type2str(lines.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,lines,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
