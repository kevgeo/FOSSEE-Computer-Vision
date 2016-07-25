    //*******************************************************************************************************
// Authors : Kevin George
//
// drawChessboardCorners(Image,Size(pts_rows,pts_cols),corners,bool(patternWasFound));
//               
//*******************************************************************************************************

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
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
  
  int opencv_drawChessboardCorners(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    //int i,j,k ;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //-> Input
    Mat Image;
    double pts_rows,pts_cols; // Points per row and column
    double *coords = NULL;
    //double *xcoords = NULL;
    //double *ycoords = NULL;
    double patternWasFound; 
    
    //-> Get projection matrix
    retrieveImage(Image,1);

    //-> Get points per row value
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr2, &pts_rows);
    if(intErr)
    {
       return intErr;
    }


    //-> Get points per column value
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &pts_cols);
    if(intErr)
    {
       return intErr;
    }

    //-> Get coordinates of corner
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &coords);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   
    
    int size = (iRows*iCols)/2;
    vector<Point2f> corners(size); // To store xcoords and ycoords
    int j = 0;
    for(int i=0; i<size; i++)
    {
        corners[i].x = coords[j++];
        corners[i].y = coords[j++];
    }

    //-> Get value telling whether pattern was found
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr5, &patternWasFound);
    if(intErr)
    {
       return intErr;
    }

    drawChessboardCorners(Image,Size(pts_rows,pts_cols),corners,bool(patternWasFound));
     
    string tempstring = type2str(Image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,Image,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    
    //-> Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;
 }

}
