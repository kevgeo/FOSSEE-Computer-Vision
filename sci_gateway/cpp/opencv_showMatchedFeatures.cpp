/*********************************************************************************
*Author : Kevin George
*
*-> To execute, showMatchedFeatures(I1,I2)
*   where I1 & I2 are images
*
*Output-> Returns an image showing the matched features between the 
          two images I1 & I2 
*********************************************************************************/
#include <numeric>
#include <string.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include <math.h>
#include <vector>

using namespace cv; 
using namespace std; 

extern "C"
{
    #include "api_scilab.h"
    #include "Scierror.h"
    #include "BOOL.h"
    #include <localization.h>
    #include "sciprint.h"
    #include "../common.h"
    // # include "../common.cpp"

    int opencv_showMatchedFeatures(char *fname, unsigned long fname_len)
    {

    	//-> Error Management variables
    	SciErr sciErr;
    	int intErr=0;

        //-> Mat containers for images
        Mat image_1;
        Mat image_2;

        //-> Address of Various Arguments
        int *piAddr = NULL;

        //-> Local variables
        int minHessian = 400;  //-> For SURF algorithm
        int num_InputArgs; 
        //-> KeyPoint is data structure to store a point feature
        //   detected by a keypoint detector(OpenCV) 
            vector<KeyPoint> keypoints_1;
            vector<KeyPoint> keypoints_2;
        Mat descriptors_1, descriptors_2;
        Mat img_matches;
        vector< DMatch > good_matches;
        vector< DMatch > matches;
        double max_dist = 0; double min_dist = 100;
        int *outList = NULL;
        unsigned char *red = NULL;
        unsigned char *green = NULL;
        unsigned char *blue = NULL; 
        /*double *indexPairs = NULL;
        double *matchMetric = NULL;
        double size=1;
        int iRows, iCols; */
    
        //KeyPoint *X = new KeyPoint; //-> Remember to free this Kevin

        //-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        CheckInputArgument(pvApiCtx, 2, 2);                     //Check on Number of Input Arguments
        CheckOutputArgument(pvApiCtx, 1, 1);                    //Check on Number of Output Arguments

    	//-> Read Image
        retrieveImage( image_1, 1);
        retrieveImage(image_2, 2);

        //-> Count number of input arguments
        //num_InputArgs = *getNbInputArgument(pvApiCtx);

        //-> Based on number of input arguments


//******************************************************  Retrieval of 3rd,4th,5th args  *******************************************************************

    /*  This is code which wasn't working when working with matchfeatures function
        Have to find a way to make it work with matchfeatures function s      
         /*************** Size of good matches **************
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

        sciErr = getScalarDouble(pvApiCtx, piAddr, &size); 
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            } 

        if(getScalarDouble(pvApiCtx, piAddr, &size))
        {
            Scierror(999,"Cannot Read Upright Value\n");
            return 0;
        }

        vector <DMatch> good_matches(size);   
   

        /*************** For indexPairs *************
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &indexPairs); 
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

        
            //-> Extract X and Ycoordinates of KeyPoints
        
            //X->pt = Point2f(keypoints[i],keypoints[iRows+i]);
            //keypoints_1.push_back(*X);
  

        /*************** For matchMetric **************
        sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &matchMetric); 
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }   
        
        for(int i=0; i< size; i++)
        {
            good_matches[i].queryIdx = indexPairs[i]; 
            good_matches[i].trainIdx = indexPairs[ static_cast<int>(size)+i];
            good_matches[i].distance = matchMetric[i];
           
        }
            */
        
//*****************************************************  Actual Processing  *************************************************************
      
        //-- Step 1: Calculate keypoints
        SurfFeatureDetector detector( minHessian );
        detector.detect( image_1, keypoints_1 );
        detector.detect( image_2, keypoints_2 );

        //-- Step 2: Calculate descriptors (feature vectors)
        SurfDescriptorExtractor extractor;
        extractor.compute( image_1, keypoints_1, descriptors_1 );
        extractor.compute( image_2, keypoints_2, descriptors_2 );

        //-- Step 3: Matching descriptor vectors using FLANN matcher
        FlannBasedMatcher matcher;
        matcher.match( descriptors_1, descriptors_2, matches );

        //-- Quick calculation of max and min distances between keypoints
        for( int i = 0; i < descriptors_1.rows; i++ )
        { 
            double dist = matches[i].distance;
            if( dist < min_dist ) min_dist = dist;
            if( dist > max_dist ) max_dist = dist;
        }

        for( int i = 0; i < descriptors_1.rows; i++ )
        { 
            if( matches[i].distance <= max(2*min_dist, 0.02) )
            //if( matches[i].distance <= (2*min_dist) )
            { good_matches.push_back( matches[i]); }
        }

        //-> Draw only "good" matches
        drawMatches( image_1, keypoints_1, image_2, keypoints_2,
                good_matches,img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

        
        //-> Returning Image
        string tempstring = type2str(img_matches.type());
        char *checker;
        checker = (char *)malloc(tempstring.size() + 1);
        memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
        returnImage(checker, img_matches, 1);
        free(checker);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

}
