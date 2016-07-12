/********************************************************
    Author: Abhilasha Sancheti & Sukul Bagai
*********************************************************
   [return_image, double] = threshold(input_image, threshold_value, max_value, "<threshold_type>");
********************************************************/
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
  
  int opencv_threshold(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen = NULL;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddrNew2 = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k;
    double thresh,max_val;
    int type; 
    char **gettype = NULL;
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2) ;
    Mat image;
    retrieveImage(image,1);
    if(no_of_channels(image.type()) != 1)
        cvtColor( image, image, CV_BGR2GRAY );
    
    
    //for threshold value (thresh)
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2,&thresh);
    if(intErr)
        return intErr;

    //for max_value
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3,&max_val);
    if(intErr)
        return intErr;


// for type of thresholding (gettype)
       sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
     //checking for error if any
       if (sciErr.iErr)
       {
        printError(&sciErr, 0);
        return 0;
       }
       sciErr = getMatrixOfString(pvApiCtx, piAddr4, &iRows, &iCols, NULL, NULL);
       piLen = (int*)malloc(sizeof(int) * iRows * iCols);
       //second call to retrieve length of each string of first argument
       sciErr = getMatrixOfString(pvApiCtx, piAddr4, &iRows, &iCols, piLen, NULL);
       if(sciErr.iErr)
       {
          printError(&sciErr, 0);
          return 0;
       }
       gettype= (char**)malloc(sizeof(char*) * iRows * iCols);
       for(i = 0 ; i < iRows * iCols ; i++)
       {
           gettype[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
       }
       //third call to retrieve data of each string of first argument
       sciErr = getMatrixOfString(pvApiCtx, piAddr4, &iRows, &iCols, piLen, gettype);
       if(sciErr.iErr)
       {
          printError(&sciErr, 0);
          return 0;
       }
    //for value of type
  
    Mat new_image(image.rows,image.cols,CV_8UC1);
     if(strcmp(gettype[0],"THRESH_BINARY")==0)
           type = 0;
    else if (strcmp(gettype[0],"THRESH_BINARY_INV")==0)
          type=1;
    else if (strcmp(gettype[0],"THRESH_TRUNC")==0)
          type=2;
    else if (strcmp(gettype[0],"THRESH_TOZERO")==0)
          type=3;
    else if (strcmp(gettype[0],"THRESH_TOZERO_INV")==0)
          type=4;
    else
    {
    	sciprint("Wrong type given. THRESH_BINARY used instead");
    	type=0;
    }
    double res = threshold( image ,new_image, thresh, max_val,type);
    // Converting resultant image to 3D matrix
    int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_image,1);
    free(checker);

    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2,res);
    if(intErr)
    {
        return intErr;
    }
   

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
        //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
