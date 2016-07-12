/********************************************************
    Author: Abhilasha Sancheti & Sukul Bagai
*********************************************************
   return_image = puttext(image ,x1 ,y1 ,fontscale,r_value,g_value,b_value,text,fontface ,thickness,linetype,bottomleft(0/1));
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
  
  int opencv_puttext(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piLen2 = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    int *piAddr8  = NULL;
    int *piAddr9  = NULL;
    int *piAddr10  = NULL;
    int *piAddr11 = NULL;
    int *piAddr12 = NULL;
    int i,j,k , bottom , fontFace;
    char **fontface = NULL,**text= NULL;
    double thickness=1 ,linetype=8,bottomleft ;
    double x1,y1,fontscale,r_value,g_value,b_value;
    bool bottomLeftOrigin;

    //checking input argument
    CheckInputArgument(pvApiCtx, 9, 12);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    
    Mat src;
    retrieveImage(src,1);
    
    //for value of x coordinate of  point of bootom-left corner of the text string
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2,&x1);
     if(intErr)
      return intErr;

    //for value of y coordinate of point of bootom-left corner of the text string
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3 ,&y1);
    if(intErr)
      return intErr;

    //for value of fontscale
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4,&fontscale);
     if(intErr)
      return intErr;
 
    //for value of R value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5,&r_value);
     if(intErr)
      return intErr;
  
      //for value of G value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr) 
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr6 ,&g_value);
    if(intErr)
      return intErr;

    //for value of Bvalue of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr7 ,&b_value);
    if(intErr)
      return intErr;
    
    /// for text to be written on the image 
    //Syntax : SciErr getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
    //fisrt call to retrieve dimensions
    sciErr = getVarAddressFromPosition(pvApiCtx,8,&piAddr8);
   //checking for error if any
   if (sciErr.iErr)
   {
    printError(&sciErr, 0);
    return 0;
   }
    sciErr = getMatrixOfString(pvApiCtx, piAddr8, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr8, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    text = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        text[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr8, &iRows, &iCols, piLen, text);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    ///for the fontface of text
    sciErr = getVarAddressFromPosition(pvApiCtx,9,&piAddr9);
    //checking for error if any
    if (sciErr.iErr)
    {
     printError(&sciErr, 0);
     return 0;
    }
     sciErr = getMatrixOfString(pvApiCtx, piAddr9, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    piLen2 = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr9, &iRows, &iCols, piLen2, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    fontface = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        fontface[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr9, &iRows, &iCols, piLen2, fontface);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
   
    // for thickness of text
    sciErr = getVarAddressFromPosition(pvApiCtx,10,&piAddr10);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr10,&thickness);
    if(intErr)
      return intErr;

   //for line type of text default: 8 
    sciErr = getVarAddressFromPosition(pvApiCtx,11,&piAddr11);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
  
    intErr = getScalarDouble(pvApiCtx, piAddr11,&linetype);
    if(intErr)
      return intErr;

    // for shift in line defulat : 0
     sciErr = getVarAddressFromPosition(pvApiCtx,12,&piAddr12);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr12,&bottomleft);
    if(intErr)
      return intErr;

   
    if (bottomleft == 0)
        bottomLeftOrigin = false;
    else if (bottomleft == 1)
        bottomLeftOrigin = true;
    else
    {
       bottomLeftOrigin = false;
       sciprint("Only 0 and 1 acceptable , taking false instead");  
    }
    /// setting fontface  FONT_HERSHEY_SIMPLEX, FONT_HERSHEY_PLAIN, FONT_HERSHEY_DUPLEX, FONT_HERSHEY_COMPLEX, FONT_HERSHEY_TRIPLEX, FONT_HERSHEY_COMPLEX_SMALL,
    // FONT_HERSHEY_SCRIPT_SIMPLEX, or FONT_HERSHEY_SCRIPT_COMPLEX,
    if(strcmp(fontface[0],"FONT_HERSHEY_SIMPLEX")==0)
           fontFace = FONT_HERSHEY_SIMPLEX;
    else if (strcmp(fontface[0],"FONT_HERSHEY_PLAIN")==0)
          fontFace= FONT_HERSHEY_PLAIN;
    else if (strcmp(fontface[0],"FONT_HERSHEY_DUPLEX")==0)
          fontFace= FONT_HERSHEY_DUPLEX;
    else if (strcmp(fontface[0],"FONT_HERSHEY_COMPLEX")==0)
          fontFace= FONT_HERSHEY_COMPLEX;
    else if (strcmp(fontface[0],"FONT_HERSHEY_TRIPLEX")==0)
          fontFace = FONT_HERSHEY_TRIPLEX;
    else if (strcmp(fontface[0],"FONT_HERSHEY_COMPLEX_SMALL")==0)
          fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    else if (strcmp(fontface[0],"FONT_HERSHEY_SCRIPT_SIMPLEX")==0)
         fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    else if (strcmp(fontface[0],"FONT_HERSHEY_SCRIPT_COMPLEX")==0)
         fontFace = FONT_HERSHEY_SCRIPT_COMPLEX;
    else
    {
        sciprint("Invalid fontface given. FONT_HERSHEY_PLAIN was used instead");
        fontFace = FONT_HERSHEY_PLAIN;
    }
    //temporary pt variable, to use in function as bottom left corner
    Point pt1(x1,y1);
     /// checking the parmeters for correct values   
    if( r_value <0 || r_value >255)
     {
          r_value=0;
          sciprint(" r value of colour should be between 0 and 255 , using 0 instead");
     }
     if( g_value <0 || g_value >255)
     {
          g_value=0;
          sciprint("  g value of colour should be between 0 and 255 , using 0 instead");
     }
    if( b_value <0 || b_value >255)
     {
          b_value=0;
          sciprint(" b value of colour should be between 0 and 255 , using 0 instead");
     }
    if ((linetype!=0) && (linetype!=4)&& (linetype!=8))
    {
             linetype=8;
             sciprint("Only 0/4/8 allowed , using 8 instead");
     }
    
           
  
   //syntax: putText(Mat& img, const string& text, Point org, int fontFace, double fontScale, Scalar color, int thickness=1, int lineType=8, bool bottomLeftOrigin=false )
    putText( src, text[0], pt1, fontFace, fontscale, Scalar(b_value,g_value,r_value), thickness, linetype, bottomLeftOrigin );

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
 
