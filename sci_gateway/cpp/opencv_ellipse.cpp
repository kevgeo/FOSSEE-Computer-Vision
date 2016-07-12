/********************************************************
    Author: Sukul Bagai
*********************************************************
   return_image = ellipse(input_image , x_coordinate_of_centre , y_coordinate_centre , first_axis, second_axis, angle, startAngle, endAngle, r_value,g_value,b_value,thickness,linetype,shift);
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
  
  int opencv_ellipse(char *fname, unsigned long fname_len)
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
    int *piAddr12  = NULL;
    int *piAddr13  = NULL;
    int *piAddr14  = NULL;
    int i,j,k;
    double thickness=1 , linetype=8, shift=0 ,centre_x ,centre_y,first_axis,second_axis,angle,startAngle,endAngle,r_value,g_value,b_value;

    //checking input argument
    CheckInputArgument(pvApiCtx, 14, 14);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    
    Mat src;
    retrieveImage(src,1);
   
    //for value of x coordinate of centre
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2,&centre_x);
    if(intErr)
        return intErr;
   

    //for value of y coordinate of centre
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3,&centre_y);
     if(intErr)
        return intErr;

    //for value of first_axis
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4,&first_axis);
   if(intErr)
        return intErr;

    //for value of second_axis
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5,&first_axis);
   if(intErr)
        return intErr;

    //for value of angle
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6,&angle);
   if(intErr)
        return intErr;

    //for value of startAngle
    sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr7,&startAngle);
   if(intErr)
        return intErr;

    //for value of endAngle
    sciErr = getVarAddressFromPosition(pvApiCtx,8,&piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr8,&endAngle);
   if(intErr)
        return intErr;

    //for value of R value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,9,&piAddr9);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr9 ,&r_value);
    if(intErr)
        return intErr;

      //for value of G value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,10,&piAddr10);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr10 ,&g_value);
     if(intErr)
        return intErr;

    // for B value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,11,&piAddr11);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr11,&b_value);
     if(intErr)
        return intErr;

    // for thickness of circle  default: 1
    sciErr = getVarAddressFromPosition(pvApiCtx,12,&piAddr12);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr12,&thickness);
    if(intErr)
        return intErr;

      //for line type of circle default: 8 
    sciErr = getVarAddressFromPosition(pvApiCtx,13,&piAddr13);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
  
    intErr = getScalarDouble(pvApiCtx, piAddr13,&linetype);
    if(intErr)
        return intErr;

 // for shift in circle default : 0
     sciErr = getVarAddressFromPosition(pvApiCtx,14,&piAddr14);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr14,&shift);
    if(intErr)
        return intErr;
   
    // checking radius parameter 
    if (first_axis < 0)
    {
        first_axis=3;
        sciprint("first_axis should be positive , 3 value was used instead\n");
    }
    if (second_axis < 0)
    {
        second_axis=3;
        sciprint("second_axis should be positive , 3 value was used instead\n");
    }
    if (first_axis < second_axis)
    { 
        double temp;
        temp=first_axis;
        first_axis=second_axis;
        second_axis=temp;
        sciprint("first_axis should be greater than second_axis. Values swapped.\n");
    }
    if(angle < 0 || angle > 360)
    {
      angle=45;
      sciprint("Imvalid angle value used. Using 45 instead");
    }
    if(startAngle < 0 || startAngle > 360)
    {
      startAngle=45;
      sciprint("Imvalid startAngle value used. Using 120 instead");
    }
    if(endAngle < 0 || endAngle > 360)
    {
      endAngle=45;
      sciprint("Imvalid endAngle value used. Using 120 instead");
    }
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
    //temporary pt variable, to use in function as centre
    Point pt(centre_x,centre_y);
    Size sz(first_axis,second_axis);
      /// Draw the circles detected
   
    ellipse( src, pt, sz, angle, startAngle, endAngle, Scalar(r_value,g_value,b_value), thickness, linetype, shift);
     
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
 
