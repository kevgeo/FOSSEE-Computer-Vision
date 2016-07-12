//*******************************************************************************************************
// Authors : Kevin George
//
// RQDecomp3x3(src, mtxR, mtxQ,Qx,Qy,Qz);
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
  
  int opencv_RQDecomp3x3(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 5, 5);

   

   //-> Input
    Mat src = (Mat_<double>(3,3));
    //-> Outputs
    Mat mtxR  = (Mat_<double>(3,3)); 
    Mat mtxQ  = (Mat_<double>(3,3)); 
    //-> Optionl outputs
    Mat Qx = (Mat_<double>(3,3));
    Mat Qy = (Mat_<double>(3,3)); 
    Mat Qz = (Mat_<double>(3,3));
    
    //-> Get projection matrix
    retrieveImage(src,1);

    

    //-> Calling decomposeProjectionMatrix function
    RQDecomp3x3(src, mtxR, mtxQ,
                 Qx,Qy,Qz);
    

   //temp variable was not needed, hence has been discarded
    string tempstring = type2str(mtxR.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,mtxR,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    //temp variable was not needed, hence has been discarded
    string tempstring2 = type2str(mtxQ.type());
    checker = (char *)malloc(tempstring2.size() + 1);
    memcpy(checker, tempstring2.c_str(), tempstring2.size() + 1);
    returnImage(checker,mtxQ,2); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    //temp variable was not needed, hence has been discarded
    string tempstring3 = type2str(Qx.type());
    checker = (char *)malloc(tempstring3.size() + 1);
    memcpy(checker, tempstring3.c_str(), tempstring3.size() + 1);
    returnImage(checker,Qx,3); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;

    //temp variable was not needed, hence has been discarded
    string tempstring4 = type2str(Qy.type());
    checker = (char *)malloc(tempstring4.size() + 1);
    memcpy(checker, tempstring4.c_str(), tempstring4.size() + 1);
    returnImage(checker,Qy,4); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;

    //temp variable was not needed, hence has been discarded
    string tempstring5 = type2str(Qz.type());
    checker = (char *)malloc(tempstring5.size() + 1);
    memcpy(checker, tempstring5.c_str(), tempstring5.size() + 1);
    returnImage(checker,Qz,5); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;


    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
 }

}
