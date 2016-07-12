//*******************************************************************************************************
// Authors : Kevin George
//
// sample inputs : outImage = drawKeypoints(image , keypoints , outimage , colour, flags);       
//                  (points: [0 0;0 100;100 0; 100 100])
// drawKeypoints (image , keypoints , outimage , colour, flags);
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
  
  int opencv_drawKeypoints(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    int *piAddr8  = NULL;
    int i,j,k ;
    double *pstData = NULL;
    double npts ,r_value,g_value ,b_value ,linetype=8 ,shift=0;
    Mat outImage;
    double *colour = NULL;
    double *KeypointData = NULL; 

    //checking input argument
    CheckInputArgument(pvApiCtx, 10, 10);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
    //retrieve the value of clockwise parameter 
    Mat image;
    retrieveImage(image,1);
    
    //retrieve the keypoint values 
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &KeypointData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    //-> Stores double the size of keypoints
    int size2 = iRows*iCols;
    int size3 = size2/2;
    vector<KeyPoint> keypoints_1(size3);
    
    i = 0;
    k = 0;
    double x,y;
    while( i < size2 )
    {
         x = KeypointData[i++];
         y = KeypointData[i++];
         //keypoints_1[0].push_back( Point2f(x, y) );
         keypoints_1[k].pt = Point2f(x, y);

    }
    
    /// to retreive the value of colour
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &colour); 
    if(sciErr.iErr)
    {
         printError(&sciErr, 0);
         return 0;
    }

	// call the drawKeypoints function of opencv
    drawKeypoints(image, keypoints_1,outImage,Scalar(colour[0],colour[1],colour[2]),DrawMatchesFlags::DEFAULT);

   
    //temp variable was not needed, hence has been discarded
    string tempstring = type2str(outImage.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,outImage,1); //here, remove the temp as a parameter as it is not needed, and instead add 1 as the third parameter. 1 denotes that the first output       argument will be this variable
    free(checker); //free memory taken up by checker

 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;


 }

}
