//*******************************************************************************************************
// Authors : Kevin George
//
// sample inputs : outImage = drawMatches(image , keypoints , outimage , colour, flags);       
//                  (points: [0 0;0 100;100 0; 100 100])
//
// groupRectangles (image1 , keypoints1 , image2 , keypoints2,good_matches
//               outimage , matchcolour, pointcolour, mask, flags);
//*******************************************************************************************************

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
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
  
  int opencv_groupRectangles(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int i,j,k ;
    double *pstData = NULL;
    double *rectdata = NULL;
    double groupThreshold, eps;
    //checking input argument
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
    //-> Get the rectangle coordinates of top-left corner,
    //   width and height

    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &rectdata);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    //-> Stores number of Rect elements of rectdata
    int size2 = (iRows*iCols)/4;
    //-> Get groupthreshold value 
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr2, &groupThreshold);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    //-> to retreive eps value
    
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);     
         return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &eps); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    int groupthresh = int(groupThreshold);
    vector<Rect> rectList(size2);
    int x,y,width,height;
    int f = 0;
    for(i=0; i<size2; i++)
    {
        x = rectdata[f++];
        y = rectdata[f++];
        width = rectdata[f++];
        height = rectdata[f++];
        rectList[i] = Rect(x,y,width,height) ;
    }

	// call the groupRectangles function of opencv
    groupRectangles(rectList,groupthresh,eps);

    int size3 = rectList.size();
    int rect_size = size3*4;
    double *rectvalues;
    
    rectvalues = (double*) malloc(sizeof(double)*rect_size); //doesn't have floating point
    int idx = 0;
    for(int i=0; i<rectList.size(); i++)
    {
        rectvalues[idx++] = rectList[i].x;
        rectvalues[idx++] = rectList[i].y;
        rectvalues[idx++] = rectList[i].width;
        rectvalues[idx++] = rectList[i].height;
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, rect_size, 1, rectvalues); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
        
    //-> Returning Output
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
