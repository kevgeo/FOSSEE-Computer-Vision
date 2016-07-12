#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>
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
  
  int opencv_erode(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    char **type = NULL;
    int i,j,k;
    double anchorX,anchorY,iterations;
    double *kernel;

    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image,1);

    //for kernel
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &kernel);
    if(sciErr.iErr)
    {
        printError(&sciErr,0);
        return 0;
    }

    double actualkernel[iRows][iCols];
    for(i=0;i<iRows;i++)
        for(j=0;j<iCols;j++)
            actualkernel[i][j]=kernel[(j*iRows)+i];

    Mat kernelMatrix(iRows,iCols,CV_64F,&actualkernel);

    //for value of anchorX
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3 ,&anchorX);
    if(intErr)
        return intErr;

    //for value of anchorY
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4 ,&anchorY);
    if(intErr)
        return intErr;

    //for value of iterations
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5 ,&iterations);
    if(intErr)
        return intErr;

    Mat new_image(image.rows,image.cols,image.type());
    Point pt(anchorX,anchorY);

    erode(image,new_image,kernelMatrix,pt,iterations);

    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_image,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
