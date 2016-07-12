/********************************************************
    Author: Abhilasha Sancheti & Sukul Bagai
*********************************************************
   return_image = clipline( width, height, x1 , y1 , x2,y2 )
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
  
  int opencv_clipline(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr1 = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int i,j,k;
    double x1,y1,x2,y2,width,height;


    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    
    //for width of the rectangle    
     sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr1);
    if (sciErr.iErr) 
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr1 ,&width);
    if(intErr)
      return intErr;

    // for height of the rectangle
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2,&height);
    if(intErr)
      return intErr;
  
    //for value of x coordinate of first point
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &x1);
    if(intErr)
        return intErr;

    //for value of y coordinate of first point
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4,&y1);
    if(intErr)
      return intErr;

    //for value of x coordinate of second point
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5,&x2);
    if(intErr)
      return intErr;

    ///for value of y coordinate of second point
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6 ,&y2);
     if(intErr)
      return intErr;
    

    if(height<0)
    {
        sciprint("Positive Value Required for Height. 1 value was used instead");
        height=1;
    }
    if(width<0)
    {
        sciprint("Positive Value Required for Width. 1 value was used instead");
        width=1;
    }  
    //temporary pt1 variable, to use in function as first line point
    Point pt1(x1,y1);
    //temporary pt2 variable, to use in function as second line point
    Point pt2(x2,y2);
    // image size
    Size sz(width,height);
    Rect  r(0,0,width,height);
   //// calling open cv clipLine function       
   
    bool ans = clipLine( sz , pt1, pt2);
    int actualans=(int)ans;
     
  // returning the result to the scilab environment
   // Syntax: SciErr createMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piBool)
  intErr= createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, actualans);
    if(intErr)
       return intErr;


    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;


 }

}
 
