//*******************************************************************************************************
// Authors : Kevin George
//
// filterSpeckles(inImage, newval, maxSpeckleSize, maxDiff, outImage );
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
  
  int opencv_initCameraMatrix2D(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    double *objectPoints = NULL;
    double *imagePoints = NULL;
    double width,height; //For image size
    double aspectRatio;
    int size1_r,size1_c;
    int size2_r,size2_c;
    Mat npoints; //output


    nputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints, Size imageSize, double aspectRatio=1
    //-> Get calibration pattern points 
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &objectPoints); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    size1_r = iRows;
    size1_c = iCols;
    iRows = 0;
    iCols = 0;

    //-> Get projections of the calibration pattern points 
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &imagePoints); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    size2_r = iRows;
    size2_c = iCols;
    iRows = 0;
    iCols = 0;

    //-> Get width 
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr3, &width);
    if(intErr)
    {
       return intErr;
    }

    //-> Get height
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr4, &height);
    if(intErr)
    {
       return intErr;
    }

    //-> Get aspectRatio
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr5, &aspectRatio);
    if(intErr)
    {
       return intErr;
    }

    vector< vector<Point> > calib_points(size1_r, size1_c);
    vector< vector<Point> > projections(size2_r,  size2_c);

    for(int i=0; i<size1_r;i++)
    {
        for(int j=0; i<size1_c;j++)
        {
            calib_points[i][j] = Point(3,4);
            //Point(objectPoints[i+j]);
        }   
    }

    for(int i=0; i<size2_r;i++)
    {
        for(int j=0; i<size2_c;j++)
        {
            projections[i][j] = Point(4,3);
            //Point(imagePoints[i+j]);
        }   
    }

    //-> Calling initCameraMatrix2D function
    npoints = initCameraMatrix2D(calib_points, projections, Size(width,height), aspectRatio);
    

    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(npoints.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,npoints,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
