//*******************************************************************************************************
// Authors : Kevin George
//
// sample inputs : newpoints1, newpoints2 = correctMatches(F, points1, points2)
// correctMatches ( F, points1, points2, newpoints1, newpoints2 )
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
  
  int opencv_correctMatches(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k ;
    double *pstData = NULL;
    double *rectdata = NULL;
    double groupThreshold, eps;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat F, points1, points2, newpoints1, newpoints2;
    
    //-> Get fundamental matrix
    retrieveImage(F,1);

    //-> Get first set of points
    retrieveImage(points1,2);

    //-> Get second set of points
    retrieveImage(points2,3);

    //-> Calling correctMatches function
    correctMatches( F, points1, points2, newpoints1, newpoints2);

    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(newpoints1.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,newpoints1,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    //temp variable was not needed, hence has been discarded
    string tempstring2 = type2str(newpoints2.type());
    checker = (char *)malloc(tempstring2.size() + 1);
    memcpy(checker, tempstring2.c_str(), tempstring2.size() + 1);
    returnImage(checker,newpoints2,2); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
