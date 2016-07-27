//*******************************************************************************************************
// Authors : Kevin George
//
// solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs
//                        ,rvec,tvec,useExtrinsicGuess,flags);
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
  
  int opencv_solvePnP(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr  = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int *piAddr6  = NULL;
    int *piAddr7  = NULL;
    int i,j,k ;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    double list_size;
    //checking input argument
    CheckInputArgument(pvApiCtx, 6, 6);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    double *objectPoints = NULL; 
    double *imagePoints = NULL; 
    double *cameraMatrix = NULL;
    double *distCoeffs = NULL;
    double useExtrinsicGuess;
    char *flags = NULL; //-> Stores current string representing 'name' of name,value pair arguments
    
    //output
    Mat rvec(3,1,cv::DataType<double>::type);
    Mat tvec(3,1,cv::DataType<double>::type);

    int Rows,Cols;
    
    //-> Get object points
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

    if( iCols!=3 )
    {
        Scierror(999,"Please make sure that objectPoints entered are 3D i.e having 3 coordinate values.\n");
            return 0;
    }

    int size = (iRows*iCols)/3;
    vector<Point3f> obPts(size);
    k = 0;
    for(int i=0; i<size; i++)
    {
        obPts[i].x = objectPoints[k];
        obPts[i].y = objectPoints[k+1*iRows];
        obPts[i].z = objectPoints[k+2*iRows];
        k++;
    }   

     //-> Get image points
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &imagePoints);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    if( iCols!=2 )
    {
        Scierror(999,"Please make sure that imagePoints entered are 2D i.e having 2 coordinate values.\n");
            return 0;
    }
    
    int size2 = (iRows*iCols)/2;
    vector<Point2f> imPts(size2);    
    k = 0;
    for(int i=0; i<size; i++)
    {
        imPts[i].x = imagePoints[k];
        imPts[i].y = imagePoints[k+1*iRows];
        k++;
    }

    if( size != size2 )
    {
        Scierror(999,"The number of object points and image points should be the same.\n");
            return 0;
    }

    //-> Get camera matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows, &iCols, &cameraMatrix);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    if( iRows!=3 || iCols !=3 )
    {
        Scierror(999,"Please make sure that camera matrix is 3x3.\n");
            return 0;
    }
    
    Mat cameramatrix(3,3,CV_32F);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            cameramatrix.at<double>(i,j) = cameraMatrix[i+j*3];
        }
    }

    //-> Get distortion coefficients
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &distCoeffs);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    
    if( iCols == 1)
    {
        if( iRows!=4 && iRows!=5 && iRows!=8 )
        {
            Scierror(999,"Please enter column vector of distortion coefficients either with size 4,5 or 8.\n");
            return 0;
        }
    }

    Mat distortioncoeffs(iRows,1,DataType<double>::type);

    for(int i=0; i<iRows; i++)
    {
        distortioncoeffs.at<double>(i,0) = distCoeffs[i]; 
    }

    //-> Get useExtrinsicGuess value
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr5, &useExtrinsicGuess);
    if(intErr)
    {
       return intErr;
    }
    
    //-> Getting flags string
            sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

            //-> Extracting name of next argument takes three calls to getMatrixOfString
            //-> First call to get rows and columns
            sciErr = getMatrixOfString(pvApiCtx, piAddr6, &iRows, &iCols, NULL, NULL); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

            piLen = (int*) malloc(sizeof(int) * iRows * iCols);

            //-> Second call to retrieve length of each string
            sciErr = getMatrixOfString(pvApiCtx,  piAddr6,  &iRows,  &iCols,  piLen,  NULL); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            
            pstData = (char**) malloc(sizeof(char*) * iRows * iCols);
            for(int iterPstData = 0; iterPstData < iRows * iCols; iterPstData++)
            {
                pstData[iterPstData] = (char*) malloc(sizeof(char) * piLen[iterPstData] + 1); 
            }

            //-> Third call to retrieve data
            sciErr = getMatrixOfString(pvApiCtx, piAddr6, &iRows, &iCols, piLen, pstData); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }   

            flags = pstData[0];
            free(pstData);
            iRows=0;
            iCols=0;
            free(piLen);

    bool ret;
    //-> Calling solvePnP function
    if(useExtrinsicGuess == 0)
    {
        if(strcmp (flags,"CV_ITERATIVE") == 0 )
        {   ret = solvePnP(obPts, imPts, cameramatrix, distortioncoeffs
                        ,rvec,tvec,false,CV_ITERATIVE);
        }

        else if(strcmp (flags,"CV_P3P") == 0)
        {   ret = solvePnP(obPts, imPts, cameramatrix, distortioncoeffs
                        ,rvec,tvec,false,CV_P3P);
        }

        else if(strcmp (flags,"CV_EPNP") == 0)
        {   ret = solvePnP(obPts, imPts, cameramatrix, distortioncoeffs
                        ,rvec,tvec,false,CV_EPNP);
        }   
    }

    else if(useExtrinsicGuess == 1)
    {
        if(strcmp (flags,"CV_ITERATIVE") == 0)
        {   ret = solvePnP(obPts, imPts, cameramatrix, distortioncoeffs
                        ,rvec,tvec,true,CV_ITERATIVE);
        }

        else if(strcmp (flags,"CV_P3P") == 0)
        {   ret = solvePnP(obPts, imPts, cameramatrix, distortioncoeffs
                        ,rvec,tvec,true,CV_P3P);
        }

        else if(strcmp (flags,"CV_EPNP") == 0)
        {   ret = solvePnP(obPts, imPts, cameramatrix, distortioncoeffs
                        ,rvec,tvec,true,CV_EPNP);
        } 
    
    }

    double *output1 = NULL; 
    output1 = (double*)malloc(sizeof(double)*3);

    for(int i=0; i<3; i++)
    {
        output1[i] = rvec.at<double>(i);
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1 , 3, 1, output1); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;

    double *output2 = NULL; 
    output2 = (double*)malloc(sizeof(double)*3);

    for(int i=0; i<3; i++)
    {
        output2[i] = tvec.at<double>(i);
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2 , 3, 1, output2); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2; 

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
