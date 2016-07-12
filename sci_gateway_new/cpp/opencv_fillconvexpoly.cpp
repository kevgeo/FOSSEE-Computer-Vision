/*************************************************
Authors : Abhilasha Sancheti & Sukul Bagai
**************************************************
 sample inputs : b=fillconvexpoly(a,points,4,0,0,0,8,0);       (points: [0 0;0 100;100 0; 100 100])
 fillConvexpoly( image , points , r_value , g_value , b_value , linrtype,shift);
*************************************************/

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
  
  int opencv_fillconvexpoly(char *fname, unsigned long fname_len)
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
    int *piAddr7  = NULL;
    int *piAddr8  = NULL;
    int i,j,k ;
    double *pstData = NULL;
    double npts ,r_value,g_value ,b_value ,linetype=8 ,shift=0;
    
 
   

    //checking input argument
    CheckInputArgument(pvApiCtx, 8, 8);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
//retreive the value of clockwise parameter 
    Mat image;
    retrieveImage(image,1);
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 
///// store values in a contour
    
    Point *pts = (Point *)malloc(iRows * 2 * sizeof(double));
    for ( i = 0 ; i < iRows ; i++)
    {
          pts[i].x = *(pstData + i);
          pts[i].y = *(pstData + 1*iRows + i);
    }
   
  /// to retrieve the number of points         
     sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &npts);
    if(intErr)
    {
       return intErr;
    } 
       
//// to retreive the value of r_value
        sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4, &r_value);
    if(intErr)
    {
       return intErr;
    } 
 /// to retreive the value of g_value
        sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr5, &g_value);
    if(intErr)
    {
       return intErr;
    } 
  
  /// to retreive the value of b_value
        sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
     intErr = getScalarDouble(pvApiCtx, piAddr6, &b_value);
    if(intErr)
    {
       return intErr;
    } 
  /// to retreive the value of linetype
        sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr7, &linetype);
    if(intErr)
    {
       return intErr;
    } 
/// to retreive the value of shift
        sciErr = getVarAddressFromPosition(pvApiCtx,8,&piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr8, &shift);
    if(intErr)
    {
       return intErr;
    } 
    
   

	 // call the fillconvexpoly function of opencv
    fillConvexPoly(image,pts,npts,Scalar(r_value,g_value,b_value),linetype,shift);
   
     //temp variable was not needed, hence has been discarded
    string tempstring = type2str(image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,image,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;


 }

}
