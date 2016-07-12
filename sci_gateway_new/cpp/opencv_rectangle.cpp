/********************************************************
    Author: Sukul Bagai 
*********************************************************
   return_image = circle(input_image , x_coordinate_of_point1 , y_coordinate_point1, x_coordinate_of_point2 , y_coordinate_point2 , r_value,g_value,b_value,thickness,linetype,shift);
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
  
  int opencv_rectangle(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    int *piAddr8  = NULL;
    int *piAddr9  = NULL;
    int *piAddr10  = NULL;
    int *piAddr11  = NULL;
    int i,j,k;
    double thickness=1 , linetype=8,shift =0 ,point1_x ,point1_y,point2_x ,point2_y,r_value,g_value,b_value;


    //checking input argument
    CheckInputArgument(pvApiCtx, 11, 11);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    
    Mat src;
    retrieveImage(src,1);
   
    //for value of x coordinate of point1
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2,&point1_x);
    if(intErr)
        return intErr;
   

    //for value of y coordinate of point1
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3,&point1_y);
     if(intErr)
        return intErr;

    //for value of x coordinate of point2
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4,&point2_x);
    if(intErr)
        return intErr;
   

    //for value of y coordinate of point2
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5,&point2_y);
     if(intErr)
        return intErr;

    //for value of R value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6 ,&r_value);
    if(intErr)
        return intErr;

      //for value of G value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr7 ,&g_value);
     if(intErr)
        return intErr;

    // for B value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,8,&piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr8,&b_value);
     if(intErr)
        return intErr;

    // for thickness of circle  default: 1
    sciErr = getVarAddressFromPosition(pvApiCtx,9,&piAddr9);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr9,&thickness);
    if(intErr)
        return intErr;

      //for line type of circle default: 8 
    sciErr = getVarAddressFromPosition(pvApiCtx,10,&piAddr10);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
  
    intErr = getScalarDouble(pvApiCtx, piAddr10,&linetype);
    if(intErr)
        return intErr;

 // for shift in circle defulat : 0
     sciErr = getVarAddressFromPosition(pvApiCtx,11,&piAddr11);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr11,&shift);
    if(intErr)
        return intErr;
   
    // checking the parmeters for correct values   
    if( r_value <0 || r_value >255)
     {
          r_value=0;
          sciprint(" r value of colour should be between 0 and 255 , using 0 instead\n");
     }
     if( g_value <0 || g_value >255)
     {
          g_value=0;
          sciprint("  g value of colour should be between 0 and 255 , using 0 instead\n");
     }
    if( b_value <0 || b_value >255)
     {
          b_value=0;
          sciprint(" b value of colour should be between 0 and 255 , using 0 instead\n");
     }
    if ((linetype!=0) && (linetype!=4)&& (linetype!=8))
    {
             linetype=8;
             sciprint("Only 0/4/8 allowed , using 8 instead\n");
     }

      // Draw the circles detected
    rectangle( src, Point(point1_x,point1_y), Point(point2_x,point2_y), Scalar(r_value,g_value,b_value), thickness, linetype, shift);
     
//// sending the image
    string tempstring = type2str(src.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,src,1);
    free(checker);
    
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;


 }

}
 
