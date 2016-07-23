//*******************************************************************************************************
// Authors : Kevin George
//
// estimateAffine3D(src, dst, out, inliers, ransacThreshold, confidence);
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
  
  int opencv_estimateAffine3D(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr  = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    double *first = NULL;
    double *second = NULL;
    Mat out(3,4,CV_64F);
    vector<uchar> inliers;
    double ransacThreshold,confidence;

    //-> Get first input 3d point set
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &first);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if( iCols!=3 )
    {
        Scierror(999,"Please enter 3D points.\n");
            return 0;
    }

    int first_rows = iRows;
    int first_cols = iCols;

    //-> Get second input 3d point set
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &second);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if( iCols!=3 )
    {
        Scierror(999,"Please enter 3D points.\n");
            return 0;
    }

    int second_rows = iRows;
    int second_cols = iCols;

    //-> Get ransacThreshold value
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr3, &ransacThreshold);
    if(intErr)
    {
       return intErr;
    }

    //-> Get confidence value
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr4, &confidence);
    if(intErr)
    {
       return intErr;
    }
  
    if( confidence<0 || confidence >1)
    {
        Scierror(999, "confidence value should be between 0 and 1.\n"); 
        return 0; 
    }   

    int size = (iRows*iCols)/3;
    //-> Stroing x,y,z coordinates
    vector<Point3f> src(size);
    vector<Point3f> dst(size);

    j = 0;
    for(int i=0; i<size; i++)
    {
        src[i].x = first[j];
        src[i].y = first[j+1*first_rows];
        src[i].z = first[j+2*first_rows];
        j++;
    }
    
    /*
    for(int i=0; i<first_rows; i++)
    {
        for(int j=0; j<first_cols; j++)
        {
            src[k].x = first[i+j*first_rows];
            src[k].y = first[i+j*first_rows];
            src[k].z = first[i+j*first_rows];
            k++;       
        }
    }   
    */
    j = 0;
    for(int i=0; i<size; i++)
    {
        dst[i].x = second[j];
        dst[i].y = second[j+1*second_rows];
        dst[i].z = second[j+2*second_rows];
        j++;
    }
    
    /*
    k = 0;
    for(int i=0; i<second_rows; i++)
    {
        for(int j=0; j<second_cols; j++)
        {
            dst[k].x = second[i+j*second_rows];
            dst[k].y = second[i+j*second_rows];
            dst[k].z = second[i+j*second_rows];
            k++;       
        }
    }
    */
    //-> Calling estimateAffine3D function
    int ret = estimateAffine3D(src, dst, out, inliers,ransacThreshold, confidence);
    

    double *aff = NULL; // affine transformation matrix
    aff = (double*)malloc(sizeof(double)*3*4);
     k = 0;
    for(int i = 0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            aff[k++] = out.at<double>(i,j);
        }
    }

    int size2 = inliers.size(); // number of inlier points
    double *inlier_pts = NULL;
    inlier_pts = (double*)malloc(sizeof(double)*size2);
    k = 0; // index for inlier_pts
    for(int i=0; i<size2; i++)
    {
        inlier_pts[k++] = inliers[i];
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1 , 3, 4, aff); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, size2, 1, inlier_pts); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
