//*******************************************************************************************************
// Authors : Kevin George
//
// decomposeProjectionMatrix(projMatrix, cameraMatrix, rotMatrix, transVect,
//                                rotMatrixX,rotMatrixY,rotMatrixZ,eulerAngles);
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
  
  int opencv_decomposeProjectionMatrix(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //-> Input
    Mat projMatrix = (Mat_<double>(3,4));
    //-> Outputs
    Mat cameraMatrix = (Mat_<double>(3,3)); 
    //Mat cameraMatrix;
    Mat rotMatrix = (Mat_<double>(3,3)); 
    Mat transVect = (Mat_<double>(4,1));
    //-> Optionl outputs
    Mat rotMatrixX = (Mat_<double>(3,3));
    Mat rotMatrixY = (Mat_<double>(3,3)); 
    Mat rotMatrixZ = (Mat_<double>(3,3));
    Mat eulerAngles; 
    
    //-> Get projection matrix
    retrieveImage(projMatrix,1);

    

    //-> Calling decomposeProjectionMatrix function
    decomposeProjectionMatrix(projMatrix, cameraMatrix, rotMatrix, transVect,
                                rotMatrixX,rotMatrixY,rotMatrixZ,eulerAngles);
    
    double *output1; //For camera matrix 
    output1 = (double*)malloc(sizeof(double)*9);
    int k = 0;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            output1[k++] = cameraMatrix.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, 3, 3, output1); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 

   /*
    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(cameraMatrix.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,cameraMatrix,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    */
    /*
    //temp variable was not needed, hence has been discarded
    string tempstring2 = type2str(rotMatrix.type());
    checker = (char *)malloc(tempstring2.size() + 1);
    memcpy(checker, tempstring2.c_str(), tempstring2.size() + 1);
    returnImage(checker,rotMatrix,2); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    //temp variable was not needed, hence has been discarded
    string tempstring3 = type2str(transVect.type());
    checker = (char *)malloc(tempstring3.size() + 1);
    memcpy(checker, tempstring3.c_str(), tempstring3.size() + 1);
    returnImage(checker,transVect,3); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
   */
    /*
    //temp variable was not needed, hence has been discarded
    string tempstring4 = type2str(rotMatrixX.type());
    checker = (char *)malloc(tempstring4.size() + 1);
    memcpy(checker, tempstring4.c_str(), tempstring4.size() + 1);
    returnImage(checker,rotMatrixX,4); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;

    //temp variable was not needed, hence has been discarded
    string tempstring5 = type2str(rotMatrixY.type());
    checker = (char *)malloc(tempstring5.size() + 1);
    memcpy(checker, tempstring5.c_str(), tempstring5.size() + 1);
    returnImage(checker,rotMatrixY,5); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;

    //temp variable was not needed, hence has been discarded
    string tempstring6 = type2str(rotMatrixZ.type());
    checker = (char *)malloc(tempstring6.size() + 1);
    memcpy(checker, tempstring6.c_str(), tempstring6.size() + 1);
    returnImage(checker,rotMatrixZ,6); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx) + 6;

    //temp variable was not needed, hence has been discarded
    string tempstring7 = type2str(eulerAngles.type());
    checker = (char *)malloc(tempstring7.size() + 1);
    memcpy(checker, tempstring7.c_str(), tempstring7.size() + 1);
    returnImage(checker,eulerAngles,7); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 7) = nbInputArgument(pvApiCtx) + 7;
    */

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
 }

}
