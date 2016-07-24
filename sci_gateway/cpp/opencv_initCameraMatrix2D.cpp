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
    int *piAddr6  = NULL;
     int *piAddrChild = NULL;
    int iPrec = 0,iItem = 0;
    int i,j,k ;
    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    double *objectPoints = NULL;
    double *imagePoints = NULL;
    double list_size;
    double width,height; //For image size
    double aspectRatio;
    int size1_r,size1_c;
    int size2_r,size2_c;
    
    vector<vector<Point2f> > imPts;
    vector<vector<Point3f> > obPts;
    //int Rows,Cols;

    //-> Get list size 
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr, &list_size);
    if(intErr)
    {
       return intErr;
    }

    /*//retrieving number of items in the list and type of data(integer/float)
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getListItemAddress(pvApiCtx,piAddr,1,&piAddrChild);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getListItemNumber(pvApiCtx,piAddr,&iItem);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }

    */
    for(int idx=1; idx<=list_size; idx++)
    {
        //-> Get calibration pattern points 
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
        if (sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }

        sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr2, idx, &iRows, &iCols ,&objectPoints);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if( iCols!=3 )
        {
            Scierror(999,"Please enter an objectPoints matrix which is of N x 3 dimension.\n");
                return 0;
        }

        int size = (iRows*iCols)/3;
        //Rows = iRows;
        //Cols = iCols;
        vector<Point3f> values2(size);

        /*j = 0;
        for(int i=0; i<size; i++)
        {
            values2[i].x = objectPoints[j++];
            values2[i].y = objectPoints[j++];
            values2[i].z = objectPoints[j++];
        }
        */
        j = 0;
        for(int i=0; i<size; i++)
        {
            values2[i].x = objectPoints[j];
            values2[i].y = objectPoints[j+1*iRows];
            values2[i].z = objectPoints[j+2*iRows];
            j++;
        }

        obPts.push_back(values2);
    }

  /*  sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &objectPoints); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    */
    

    
    for(int idx=1; idx<=list_size; idx++)
    {
        //-> Get projections of the calibration pattern points 
        sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr3, idx, &iRows, &iCols ,&imagePoints);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if( iCols!=2 )
        {
            Scierror(999,"Please enter an imagePoints matrix which is of N x 2 dimension.\n");
                return 0;
        }

        int size = (iRows*iCols)/2;
        vector<Point2f> values1(size);

        j = 0;
        for(int i=0; i<size; i++)
        {
            values1[i].x = imagePoints[j];
            values1[i].y = imagePoints[j+1*iRows];
            j++;
        }

        imPts.push_back(values1);
    }


    /*sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows, &iCols, &imagePoints); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 
    */
    
    //-> Get width 
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr4, &width);
    if(intErr)
    {
       return intErr;
    }

    if( width <=0 )
    {
        Scierror(999,"Please ented valid width which is more than zero");
    }

    //-> Get height
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr5, &height);
    if(intErr)
    {
       return intErr;
    }

    if( height <=0 )
    {
        Scierror(999,"Please ented valid height which is more than zero");
    }

    //-> Get aspectRatio
    sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr6, &aspectRatio);
    if(intErr)
    {
       return intErr;
    }



    //-> Calling initCameraMatrix2D function
    Mat cameraMatrix = initCameraMatrix2D(obPts, imPts, Size(width,height), aspectRatio);

    
    double *output = NULL;
    output = (double*)malloc(sizeof(double)*9);

    //k = 0;
    for(int i=0; i<3;i++)
    {
        for(int j=0; j<3;j++)
        {
            output[i+j*3] = cameraMatrix.at<double>(i,j);
        }
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1 , 3, 3, output); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;

 }

}
