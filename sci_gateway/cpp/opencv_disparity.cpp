//*******************************************************************************************************
// Authors : Kevin George
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
  
  int opencv_disparity(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
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
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 13, 13);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    //Input
    Mat image1,image2;
    Mat g1,g2, disp, disp8;
  
    double NumofDisparities,minDisparity;
    double sadwindowSize,P1,P2;
    double prefilterCap,maxDiff,uniquenessRatio;
    double SpeckleWindowSize,SpeckleRange,fullDP; 


    int num_args = *getNbInputArgument(pvApiCtx);
    if( num_args!=13 )
    {
        Scierror(999,"Function requires 13 arguments as input.\n");
            return 0;
    }

    //-> Get first stereo image.
    retrieveImage(image1,1);

    //-> Get second stereo image.
    retrieveImage(image2,2);

    if( image1.channels()==3 || image1.channels()==4 )
    {    
        cvtColor(image1, g1, CV_BGR2GRAY);
        cvtColor(image2, g2, CV_BGR2GRAY);
    }

    else
    {
        g1 = image1;
        g2 = image2;
    }

    //-> Get numofDisparities
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &NumofDisparities);
    if(intErr)
    {
       return intErr;
    }

    int num_disparities = double(NumofDisparities);
    if( num_disparities==0 || num_disparities%16!=0 )
    {
        Scierror(999,"NumofDisparities value should be a multiple of 16.\n");
            return 0;
    }

    //-> Get minimum possible disparity value
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr4, &minDisparity);
    if(intErr)
    {
       return intErr;
    }
   
    //-> Get matched blocked size
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr5, &sadwindowSize);
    if(intErr)
    {
       return intErr;
    }
   
    //-> Get first parameter controlling disparity smoothness
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr6, &P1);
    if(intErr)
    {
       return intErr;
    }

    //-> Get second parameter controlling disparity smoothness
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr7); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr7, &P2);
    if(intErr)
    {
       return intErr;
    }

    //-> Get maximum allowed difference value
    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddr8); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr8, &maxDiff);
    if(intErr)
    {
       return intErr;
    }

    //-> Get prefilterCap
    sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddr9); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr9, &prefilterCap);
    if(intErr)
    {
       return intErr;
    }

    //-> Get unique ratio value
    sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddr10); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr10, &uniquenessRatio);
    if(intErr)
    {
       return intErr;
    }

    //-> Get speckle window size value
    sciErr = getVarAddressFromPosition(pvApiCtx, 11, &piAddr11); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr11, &SpeckleWindowSize);
    if(intErr)
    {
       return intErr;
    }

    //-> Get speckle range value
    sciErr = getVarAddressFromPosition(pvApiCtx, 12, &piAddr12); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr12, &SpeckleRange);
    if(intErr)
    {
       return intErr;
    }

    //-> Get fullDp value   
    sciErr = getVarAddressFromPosition(pvApiCtx, 13, &piAddr13); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr13, &fullDP);
    if(intErr)
    {
       return intErr;
    }

    
    StereoSGBM sbm;
    sbm.SADWindowSize = sadwindowSize;
    sbm.numberOfDisparities = num_disparities;
    sbm.preFilterCap = prefilterCap; 
    sbm.minDisparity = minDisparity; 
    sbm.uniquenessRatio = uniquenessRatio;
    sbm.speckleWindowSize = SpeckleWindowSize;
    sbm.speckleRange = SpeckleRange;
    sbm.disp12MaxDiff = maxDiff;
    if( fullDP==1 )
        sbm.fullDP = true;
    else if( fullDP==0 )
        sbm.fullDP = false;
    sbm.P1 = P1;
    sbm.P2 = P2;
    sbm(g1, g2, disp);

    normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);

    //-> Returning Image
    string tempstring = type2str(disp8.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, disp8, 1);
    free(checker);

    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
