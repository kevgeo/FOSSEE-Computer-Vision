/*************************************************
Author: Sukul Bagai
**************************************************/

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

  int opencv_getStructuringElement(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr1 = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k,type;
    char **gettype = NULL;
    double cols,rows,anchorX,anchorY;

    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);      
    //checking for error if any
    if (sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows, &iCols, NULL, NULL);
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string of first argument
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    gettype= (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
         gettype[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
   //third call to retrieve data of each string of first argument
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows, &iCols, piLen, gettype);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    
    if(strcmp("MORPH_RECT",gettype[0])==0)
      type=MORPH_RECT;
    else if(strcmp("MORPH_ELLIPSE",gettype[0])==0)
      type=MORPH_ELLIPSE;
    else if(strcmp("MORPH_CROSS",gettype[0])==0)
      type=MORPH_CROSS;

    //for cols
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0; 
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2 ,&cols);
    if(intErr)
        return intErr;

    //for rows
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0; 
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &rows);
    if(intErr)
        return intErr;

    //for anchorX
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0; 
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4 ,&anchorX);
    if(intErr)
        return intErr;

    //for anchorY
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0; 
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5 ,&anchorY);
    if(intErr)
        return intErr;

    Mat temp = getStructuringElement(type,Size(rows,cols),Point(anchorX,anchorY));

    double *m = (double *)malloc(temp.rows*temp.cols*sizeof(double));
    for(i=0;i<temp.rows;i++)
    {
      for(j=0;j<temp.cols;j++)
      {
        uchar intensity = temp.at<uchar>(i, j);
        *(m + i*temp.cols + j) = intensity;
      }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, temp.rows, temp.cols, m);
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
