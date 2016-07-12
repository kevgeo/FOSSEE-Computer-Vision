//*******************************************************************************************************
// Authors : Kevin George
//
// sample inputs : outImage = drawMatches(image , keypoints , outimage , colour, flags);       
//                  (points: [0 0;0 100;100 0; 100 100])
//
// drawMatches (image1 , keypoints1 , image2 , keypoints2,good_matches
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
  
  int opencv_drawMatches(char *fname, unsigned long fname_len)
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
    int size2,size3;
    Mat outImage;
    double *matchcolour = NULL;
    double *pointcolour = NULL;
    double *KeypointData1 = NULL;
    double *KeypointData2 = NULL;
    double *indexPairs = NULL;
    double *matchMetric = NULL;

    //checking input argument
    CheckInputArgument(pvApiCtx, 10, 10);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
    //-> Get first image 
    Mat image1,image2;
    retrieveImage(image1,1);
    
    //-> Get first image keypoint values 
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &KeypointData1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    //-> Stores double the size of keypoints
    size2 = iRows*iCols;
    size3 = size2/2;
    vector<KeyPoint> keypoints_1(size3);
    
    i = 0;
    k = 0;
    double x,y;
    while( i < size2 )
    {
         x = KeypointData1[i++];
         y = KeypointData1[i++];
         //keypoints_1[0].push_back( Point2f(x, y) );
         keypoints_1[k].pt = Point2f(x, y);

    }

    //-> Get second image
    retrieveImage(image1,3);
    
    //-> Get second image keypoint values 
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &KeypointData2);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    //-> Stores double the size of keypoints
    size2 = iRows*iCols;
    size3 = size2/2;
    vector<KeyPoint> keypoints_2(size3);
    
    i = 0;
    k = 0;
    double x2,y2;
    while( i < size2 )
    {
         x2 = KeypointData2[i++];
         y2 = KeypointData2[i++];
         keypoints_2[k].pt = Point2f(x2, y2);

    }
    //-> to retreive the values of goodmatches
    
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
         return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &iRows, &iCols, &indexPairs); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    int size4 = iRows*iCols;
    vector< DMatch > good_matches(size4);
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6); 
    if (sciErr.iErr)
    {
          printError(&sciErr, 0); 
         return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr6, &iRows, &iCols, &matchMetric); 
    if(sciErr.iErr)
    {
         printError(&sciErr, 0);
         return 0;
    }   
        
    for(int i=0; i< size4; i++)
    {
        good_matches[i].queryIdx = indexPairs[i]; 
        good_matches[i].trainIdx = indexPairs[ static_cast<int>(size4)+i];
        good_matches[i].distance = matchMetric[i];
           
    }


    //-> to retreive the values of matchcolour
    sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr7, &iRows, &iCols, &matchcolour); 
    if(sciErr.iErr)
    {
         printError(&sciErr, 0);
         return 0;
    }

    //-> to retreive the values of pointcolour
    sciErr = getVarAddressFromPosition(pvApiCtx,8,&piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr8, &iRows, &iCols, &pointcolour); 
    if(sciErr.iErr)
    {
         printError(&sciErr, 0);
         return 0;
    }


	// call the drawKeypoints function of opencv
    drawMatches( image1, keypoints_1, image2, keypoints_2,
                good_matches,outImage, Scalar(matchcolour[0],matchcolour[1],matchcolour[2]), 
                Scalar(pointcolour[0],pointcolour[1],pointcolour[2]),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
   
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
