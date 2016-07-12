/********************************************************
    Author: Sukul Bagai
*********************************************************
   rectangle_dimensions_arry(height, width, x, y) = boundingRect ( vector<point> )
********************************************************/

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
  
  int opencv_boundingRect(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr1 = NULL;
    int *piAddrNew = NULL;
    int i,j,k ;
    double *points;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);
    
//retreive the points 
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows, &iCols,&points);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }  
    
    vector<Point> actualPoints;
    for(int i=0;i<iRows;i++)
    {
      actualPoints.push_back(Point(*(points + i),*(points + iRows + i)));
    }
    
    Rect returnRect;
    returnRect = boundingRect(actualPoints);
    
    double arr[4];
    arr[0]=returnRect.height;
    arr[1]=returnRect.width;
    arr[2]=returnRect.x;
    arr[3]=returnRect.y;
    
  sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 4, arr);
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
