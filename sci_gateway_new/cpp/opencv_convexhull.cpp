/********************************************************
    Author: Abhilasha Sancheti & Sukul Bagai
*********************************************************
   hull = convexhull ( contour(vector<point>), clockwise(1/0) , returnpoints(1/0) )
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
  
  int opencv_convexhull(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr1 = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    double *pstData = NULL;
    int i,j,k ;
    double clkwise;
    double returnpoints;
 
   

    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
//retreive the contour parameter 
      sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows, &iCols,&pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 
    //cout<<*(pstData + 0*2 + 0)<<" "<<*(pstData + 0*2 + 1)<<endl;
     //cout<<*(pstData + 1*2 + 2)<<" "<<*(pstData + 1*2 + 3);
///// store values in a contour
    int rows = iRows;
    vector<Point> cont1;
    for ( i = 0 ; i < iRows ; i++)
    {
        Point pt(*(pstData + 0*iRows + i),*(pstData + 1*iRows + i));
        cont1.push_back(pt);
    }
     sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2 ,&clkwise);
    if(intErr)
      return intErr;
     
    
    
//// to retreive the value of returnpoint 
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3,&returnpoints);
    if(intErr)
       return intErr;

   
    bool clkWise, returnPoint;
/// setting clkwise parameter of function
    if ( clkwise ==1)
      clkWise = true;
    else if (clkwise == 0)
     clkWise = false;
    else
     {
        clkWise = false;
        sciprint("only 0/1 acceptable , taking 0 instead");
      }
///// setting returnpoint parameter of function
     if ( returnpoints ==1)
      returnPoint = true;
    else if (returnpoints == 0)
     returnPoint = false;
    else
     {
        returnPoint = true;
        sciprint("only 0/1 acceptable , taking 1 instead");
      }
    
       vector<Point> hull;
       convexHull(Mat(cont1), hull , clkWise , returnPoint);
	  int row = hull.size();
	  double *cor = (double *)malloc(row*2*sizeof(double));
	  for (int i=0;i<row;i++)
	  {
	      *(cor + i*2 +0)=hull[i].x;
      cout<<hull[i].x<<" "<<hull[i].y;
	      *(cor + i*2 +1)=hull[i].y;
	  }
    
  sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, 2, cor);
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
