#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
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
  int opencv_houghlinesp(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int i,j,k;
    double rho, theta, minLineLength = 0, maxLineGap = 0, threshold;
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;


    Mat image;
    retrieveImage(image, 1); 
    cvtColor(image, image, CV_BGR2GRAY);
    //for value of rho
     sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
   intErr = getScalarDouble(pvApiCtx, piAddr2, &rho);
     if(intErr)
    {
        return intErr;
    }   

    //for value of theta
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
   intErr = getScalarDouble(pvApiCtx, piAddr3, &theta);
     if(intErr)
    {
        return intErr;
    }   

    //for value of threshold
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &threshold);
     if(intErr)
    {
        return intErr;
    }   
     //for value of minLineLength
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5, &minLineLength);
     if(intErr)
    {
        return intErr;
    }   

     //for value of maxLineGap
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6, &maxLineGap);
     if(intErr)
    {
        return intErr;
    }   

    vector < vector <double> > output;

    HoughLinesP(image, output, rho, theta, threshold, minLineLength, maxLineGap);

//create matrix from 2-D vector
    int row = output.size();
    double *cor = (double *)malloc(row*4*sizeof(double));


    for (int i=0;i<row;i++)
    {
        *(cor + i*2 +0)=output[i][0];
 //       cout<<output[i][0]<<" "<<output[i][1];
        *(cor + i*2 +1)=output[i][1];
        *(cor + i*2 +2)=output[i][2];
        *(cor + i*2 +3)=output[i][3];
    }
  sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, 4, cor);
  if(sciErr.iErr)
  {
      printError(&sciErr, 0);
      return 0;
  }
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;


 }

}