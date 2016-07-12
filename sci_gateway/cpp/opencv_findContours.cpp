/********************************************************
    Author: Abhilasha Sancheti & Sukul Bagai
*********************************************************
   contours = findcontours(input_image(after canny is applied on it), mode, method, point_x, point_y)
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
  
  int opencv_findContours(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen2=NULL;
    int *piLen3 = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k, findmode , findmethod;
    char **find_method=NULL , **mode = NULL;
    double x,y;
   

    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
    Mat src;
    retrieveImage(src,1);
       
    // for mode parameter of findContours
     sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
             //checking for error if any
     if (sciErr.iErr)
     {
      printError(&sciErr, 0);
      return 0;
     }
     sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
     piLen2 = (int*)malloc(sizeof(int) * iRows * iCols);
     //second call to retrieve length of each string of first argument
     sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen2, NULL);
     if(sciErr.iErr)
     {
        printError(&sciErr, 0);
        return 0;
     }
     mode= (char**)malloc(sizeof(char*) * iRows * iCols);
     for(i = 0 ; i < iRows * iCols ; i++)
     {
         mode[i] = (char*)malloc(sizeof(char) * (piLen2[i] + 1));//+ 1 for null termination
     }
     //third call to retrieve data of each string of first argument
     sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen2, mode);
     if(sciErr.iErr)
     {
        printError(&sciErr, 0);
        return 0;
     }  

     // for method parameter of findContours
      sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
             //checking for error if any
       if (sciErr.iErr)
       {
        printError(&sciErr, 0);
        return 0;
       }
       sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, NULL, NULL);
       piLen3 = (int*)malloc(sizeof(int) * iRows * iCols);
       //second call to retrieve length of each string of first argument
       sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen3, NULL);
       if(sciErr.iErr)
       {
          printError(&sciErr, 0);
          return 0;
       }
       find_method= (char**)malloc(sizeof(char*) * iRows * iCols);
       for(i = 0 ; i < iRows * iCols ; i++)
       {
           find_method[i] = (char*)malloc(sizeof(char) * (piLen3[i] + 1));//+ 1 for null termination
       }
       //third call to retrieve data of each string of first argument
       sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen3, find_method);
       if(sciErr.iErr)
       {
          printError(&sciErr, 0);
          return 0;
       }
       //toget the x coordinate of point parameter of findcontours
        sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr4,&x);
    if(intErr)
       return intErr;
  
           //toget the y coordinate of point parameter of findcontours
        sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr5 ,&y);
    if(intErr)
       return intErr;
     
       //to set hte mode for findcontour
       if (strcmp(mode[0],"CV_RETR_EXTERNAL")==0)
            findmode = CV_RETR_EXTERNAL;
        else if (strcmp(mode[0],"CV_RETR_LIST")==0)
          findmode =  CV_RETR_LIST;
        else if (strcmp(mode[0],"CV_RETR_CCOMP")==0)
          findmode =  CV_RETR_CCOMP;
        else if (strcmp(mode[0],"CV_RETR_TREE")==0)
            findmode =  CV_RETR_TREE;
        else
           {
              findmode =  CV_RETR_TREE;
              sciprint("wrong mode given , using CV_RETR_TREE instead");
           }
        // to set the method for findContours
       if (strcmp(find_method[0],"CV_CHAIN_APPROX_NONE")==0)
            findmethod = CV_CHAIN_APPROX_NONE;
        else if (strcmp(  find_method[0],"CV_CHAIN_APPROX_SIMPLE")==0)
          findmethod =  CV_CHAIN_APPROX_SIMPLE;
        else if (strcmp(find_method[0],"CV_CHAIN_APPROX_TC89_L1")==0)
          findmethod =  CV_CHAIN_APPROX_TC89_L1;
        else if (strcmp(find_method[0],"CV_CHAIN_APPROX_TC89_KCOS")==0)
            findmethod =  CV_CHAIN_APPROX_TC89_KCOS;       
        else
           {
              findmethod =  CV_CHAIN_APPROX_SIMPLE;
              sciprint("wrong method given , using CV_CHAIN_APPROX_SIMPLE instead");
           }
    Point pt(x,y);
    /// Find contours  
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( src, contours, hierarchy, findmode, findmethod, pt);

    //Assigning the list as the Output Variable
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, contours.size(), &piAddrNew);
    if(sciErr.iErr)
    {
       printError(&sciErr, 0);           
       return 0;
    }

    for(i=0;i<contours.size();i++)
    {
        double *arr = (double *)malloc(sizeof(double) * 2 * contours[i].size());

        for(j=0;j<contours[i].size();j++)
        {
            *(arr + j*2 + 0) = contours[i][j].x;
            *(arr + j*2 + 1) = contours[i][j].y;
        }

        sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, i+1, contours[i].size(), 2, arr);
        free(arr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;


 }

}
