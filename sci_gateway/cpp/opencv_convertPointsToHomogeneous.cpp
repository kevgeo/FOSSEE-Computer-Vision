//*******************************************************************************************************
// Authors : Kevin George
//
// sample inputs : outImage = convertPointsToHomogeneous(inImage, outImage)
// convertPointsToHomogeneous (inImage, outImage)
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
  
  int opencv_convertPointsToHomogeneous(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    double *input = NULL;
    //-> Get points
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &input);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if( iCols!=2 )
    {
        Scierror(999, "Please enter 2D points matrix of size Nx2.\n"); 
                    return 0; 
    }

    int size = 0;
    /*int check = 0;
    if( (iRows*iCols)%2 == 0 )
         size = (iRows*iCols)/2;
    else
    {
        size = ((iRows*iCols)/2)+1;
        check = 1;
    } 
    */
    size = iRows;
    vector<Point2f> src(size);
    int j = 0;
    /*for(int i = 0; i<size; i++)
    {
        src[i].x = input[j++];
        if( check==1 && i==(size-1) )
            break;
         else   
            src[i].y = input[j++];
    }
    */

    for(int i = 0; i<size; i++)
    {
        src[i].x = input[j];
        src[i].y = input[j+1*iRows];
        j++;
    }

    vector<Point3f> dst;
    //-> Calling convertPointsToHomogeneous function
    convertPointsToHomogeneous(src, dst);
    int size2 = 0;
    
    /*if( check == 1) 
        size2 = dst.size()*2;
    else
        size2 = (dst.size()*2)+1;
    */
    size2 = dst.size()*3;
    double *output = NULL;
    output = (double*)malloc(sizeof(double)*size2);
    /*
    for(int i=0; i<dst.size(); i++)
    {
        output[j++] = dst[i].x;
        if(check == 1)
        {    
            if( i == (dst.size()-1) ) 
                output[j++] = dst[i].z;
            else
                output[j++] = dst[i].y;
        }
        else
        {
            output[j++] = dst[i].y;
            if( i == (dst.size()-1) ) 
                output[j++] = dst[i].z;   
        }
    }
    */
    j = 0;
    for(int i=0; i<dst.size(); i++)
    {
        output[j] = dst[i].x;
        output[j+1*iRows] = dst[i].y;
        output[j+2*iRows] = dst[i].z;
        j++;      
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, dst.size(), 3, output); 
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
