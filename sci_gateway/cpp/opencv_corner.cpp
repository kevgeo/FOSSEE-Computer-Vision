/***************************************************
Author : Rohit Suri
***************************************************/
#include <numeric>
#include <string.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <math.h>
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
    
    /*Calling syntax: corner(I) or corner(I, Name, Value)   
       which uses additional options specified by one or more Name, Value pair arguments.             
       Arguments Allowed: MinQuality, FilterSize, ROI, SensitivityFactor
       MinQuality        : The minimum accepted quality of corners represents a fraction of the
                           maximum corner metric value in the image. Larger values can be used 
                           to remove erroneous corners.
       FilterSize        : Gaussian filter dimension
       ROI               : Rectangular region for corner detection
       SensitivityFactor : A scalar value, K, where 0 < K < 0.25, specifying the sensitivity 
                           factor used in the Harris detection algorithm.                    
       Method            : Whether to use 'Harris' or 'MinimumEigenValue' technique                       */

    int opencv_corner(char *fname, unsigned long fname_len)
    {
        // Error management variables
        SciErr sciErr; 
        int intErr; 
        
        //------Local variables------//        
        int *location = NULL; 
        double *metric = NULL; 
        int *piAddr = NULL;       
        int *piLen = NULL; 
        int nbInputArguments; 
        char **pstData = NULL; 
        char *currentArg = NULL; 
        bool *providedArgs = NULL; 
        double *matrixOfRoi;  // ROI[xStart, yStart, width, height]
        int iRows, iCols; 
        Mat sourceImage, dst, dstThresholded, ucharDstThresholded, extended; 
        vector<vector<Point> > contours; 
        char *method;
        double filterSize = 5, minQuality = 0.01, sensitivityFactor = 0.04, blockSize = 2, maxDst, localMax; 
        bool *included = NULL; 
        int pointCount = 0, localMaxPos; 
        double tempForSwapping;
        int coordinateMin, coordinatePos;
        
        //------Check number of parameters------//
        CheckInputArgument(pvApiCtx, 1, 11); 
        CheckOutputArgument(pvApiCtx, 1, 3); 
        
        //------Get input arguments------//
        retrieveImage(sourceImage, 1); 
        if(sourceImage.channels() != 1)
        {
            Scierror(999, "The input image is not a grayscale image."); 
            return 0; 
        }
        matrixOfRoi = (double*) malloc(sizeof(double) * 4); 
        providedArgs = (bool*) malloc(sizeof(bool) * 5); 
        memset(providedArgs, 0, 5); 
        method = "Harris";
        matrixOfRoi[0] = 0; 
        matrixOfRoi[1] = 0; 
        matrixOfRoi[2] = sourceImage.cols; 
        matrixOfRoi[3] = sourceImage.rows; 
        nbInputArguments = *getNbInputArgument(pvApiCtx); 
        
        // The following loop is for checking if optional arguments have been provided
        for(int iter=2; iter <= nbInputArguments; iter++)
        {
            // Getting address of next argument
            sciErr = getVarAddressFromPosition(pvApiCtx, iter, &piAddr); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            
            // Extracting name of next argument takes three calls to getMatrixOfString
            sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            
            piLen = (int*) malloc(sizeof(int) * iRows * iCols); 
            
            sciErr = getMatrixOfString(pvApiCtx,  piAddr,  &iRows,  &iCols,  piLen,  NULL); 
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
            
            sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            
            currentArg = pstData[0]; 
            // providedArgs[] makes sure that no optional argument is provided more than once
            if(strcmp(currentArg, "MinQuality")==0)
            {
                if(iter+1<=nbInputArguments && !providedArgs[0])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    intErr = getScalarDouble(pvApiCtx, piAddr, &minQuality); 
                    if(intErr)
                    {
                        return intErr; 
                    }
                    // Checking if values are in proper range. Same for all optional arguments
                    if(minQuality < 0 || minQuality > 1)
                    {
                        Scierror(999, "Error: Please provide proper value for \"%s\". Permissible range is [0, 1].\n", currentArg); 
                        return 0; 
                    }
                    providedArgs[0] = 1; 
                }
                else if(providedArgs[0]) // Send an error message if an argument is provided more than once. Same for all optional arguments.
                {
                    Scierror(999, "Please provide optional arguments only once.\n"); 
                    return 0; 
                }
                else // Send an error message if name of argument is given but type is incorrect. Same for all optional arguments.
                {
                    Scierror(999, "Incorrect number of arguments provided. Please check the documentation for more information.\n"); 
                    return 0; 
                }
            }
            else if(strcmp(currentArg, "FilterSize")==0)
            {
                if(iter+1 <= nbInputArguments  && !providedArgs[1])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    intErr = getScalarDouble(pvApiCtx, piAddr, &filterSize); 
                    if(intErr)
                    {
                        return intErr; 
                    }
                    providedArgs[1] = 1; 
                    if(filterSize!=1 && filterSize!=3 && filterSize!=5 && filterSize!=7)
                    {
                        Scierror(999, "Error: Please provide proper value for \"%s\". Permissible values are {1, 3, 5, 7}.\n", currentArg); 
                        return 0; 
                    }
                }
                else if(providedArgs[1])
                {
                    Scierror(999, "Please provide optional arguments only once.\n"); 
                    return 0; 
                }
                else
                {
                    Scierror(999, "Incorrect number of arguments provided. Please check the documentation for more information.\n"); 
                    return 0; 
                }
            }
            else if(strcmp(currentArg, "ROI")==0)
            {
                if(iter+1 <= nbInputArguments && !providedArgs[2])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    if(!isDoubleType(pvApiCtx, piAddr) || isVarComplex(pvApiCtx, piAddr))
                    {
                        Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, iter); 
                        return 0; 
                    }
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &matrixOfRoi); 
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    if(iRows!=1 || iCols!=4)
                    {
                        Scierror(999, "Incorrect dimension of matrix for argument ROI.\n"); 
                        return 0; 
                    }
                    providedArgs[2] = 1; 
                    if(matrixOfRoi[0] < 0 || matrixOfRoi[0] > sourceImage.cols || matrixOfRoi[1] < 0 ||
                        matrixOfRoi[1] > sourceImage.rows || matrixOfRoi[2] < 0 || matrixOfRoi[0] + matrixOfRoi[2] > sourceImage.cols
                        || matrixOfRoi[3] < 0 || matrixOfRoi[1] + matrixOfRoi[3] > sourceImage.rows)
                    {
                        Scierror(999, "Error: Please provide proper values for \"%s\".\n", currentArg); 
                        return 0; 
                    }
                }
                else if(providedArgs[2])
                {
                    Scierror(999, "Please provide optional arguments only once.\n"); 
                    return 0; 
                }
                else
                {
                    Scierror(999, "Incorrect number of arguments provided. Please check the documentation for more information.\n"); 
                    return 0; 
                }
            }
            else if(strcmp(currentArg, "SensitivityFactor")==0)
            {
                if(iter+1 <= nbInputArguments && !providedArgs[3])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    intErr = getScalarDouble(pvApiCtx, piAddr, &sensitivityFactor); 
                    if(intErr)
                    {
                        return intErr; 
                    }
                    providedArgs[3] = 1; 
                    if(sensitivityFactor < 0 || sensitivityFactor > 0.25)
                    {
                        Scierror(999, "Error: Please provide proper value for \"%s\". Permissible values are [0, 0.25].\n", currentArg); 
                        return 0; 
                    }
                }
                else if(providedArgs[3])
                {
                    Scierror(999, "Please provide optional arguments only once.\n"); 
                    return 0; 
                }
                else
                {
                    Scierror(999, "Incorrect number of arguments provided. Please check the documentation for more information.\n"); 
                    return 0; 
                }
            }
            else if(strcmp(currentArg, "Method")==0)
            {
                if(iter+1 <= nbInputArguments && !providedArgs[4])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    piLen = (int*) malloc(sizeof(int) * iRows * iCols); 
                    
                    sciErr = getMatrixOfString(pvApiCtx,  piAddr,  &iRows,  &iCols,  piLen,  NULL); 
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
                    
                    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    method = pstData[0];
                    providedArgs[4] = 1; 
                    if(strcmp(method, "Harris") !=0 && strcmp(method, "MinimumEigenValue") !=0)
                    {
                        Scierror(999, "Error: Please provide proper value for \"%s\". Permissible values are [\"Harris\" , \"MinimumEigenValue\"].\n", currentArg); 
                        return 0; 
                    }
                }
                else if(providedArgs[4])
                {
                    Scierror(999, "Please provide optional arguments only once.\n"); 
                    return 0; 
                }
                else
                {
                    Scierror(999, "Incorrect number of arguments provided. Please check the documentation for more information.\n"); 
                    return 0; 
                }
            }
            else
            {
                Scierror(999, "Error: \"%s\" name for input argument is not valid.\n", currentArg); 
                return 0; 
            }
        }
        
        //------Actual processing------//
        if(strcmp(method, "Harris") == 0)
        {
            cornerHarris(sourceImage(Rect(matrixOfRoi[0], matrixOfRoi[1], matrixOfRoi[2], matrixOfRoi[3])), dst, blockSize, filterSize, sensitivityFactor, BORDER_DEFAULT); 
        }
        else
        {
            cornerMinEigenVal(sourceImage(Rect(matrixOfRoi[0], matrixOfRoi[1], matrixOfRoi[2], matrixOfRoi[3])), dst, blockSize, filterSize, BORDER_DEFAULT);
        }
        // Finding the maximum value at a pixel in dst        
        maxDst = 0; 
        for(int rowIter=0; rowIter < dst.rows; rowIter++)
        {
            for(int colIter=0; colIter < dst.cols; colIter++)
            {
                if(dst.at<float>(rowIter, colIter) > maxDst)
                {
                    maxDst = dst.at<float>(rowIter, colIter); 
                }
            }
        }
        
        /* First, we threshold dst according to the minQuality expected by the user.
           We then find contours in the thresholded image and check each pixel's value
           to compute the local maxima. A extended mat is created because contours 
           having edges on the boundaries of the images are not considered by 
           findContours                                                                */
        threshold(dst, dstThresholded, maxDst*minQuality, 255, THRESH_BINARY); 
        dstThresholded.convertTo(ucharDstThresholded, CV_8UC1); 
        extended = Mat(dst.rows+2, dst.cols+2, CV_8UC1); 
        for(int rowIter = 0; rowIter < extended.rows; rowIter++)
        {
            for(int colIter = 0; colIter < extended.cols; colIter++)
            {
                if(rowIter == 0 || rowIter == extended.rows - 1
                || colIter == 0 || colIter == extended.cols - 1)
                {
                    extended.at<uchar>(rowIter, colIter) = 0; 
                }
                else
                {
                    extended.at<uchar>(rowIter, colIter) = ucharDstThresholded.at<uchar>(rowIter-1, colIter-1); 
                }
            }
        }
        
        findContours(extended, contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE); 
        pointCount = contours.size(); 
        location = (int*) malloc(sizeof(int) * pointCount * 2); 
        metric = (double*) malloc(sizeof(double)*pointCount); 
        for(int rowIter = 0; rowIter < contours.size(); rowIter++)
        {
            localMax = 0; 
            localMaxPos = 0; 
            for(int colIter=0; colIter<contours[rowIter].size(); colIter++)
            {
                if(dst.at<float>(contours[rowIter][colIter].y-1, contours[rowIter][colIter].x-1)>localMax)
                {
                    localMax = dst.at<float>(contours[rowIter][colIter].y-1, contours[rowIter][colIter].x-1); 
                    localMaxPos = colIter; 
                }
            }
            if(localMax > maxDst*minQuality)
            {
                location[rowIter] = contours[rowIter][localMaxPos].x-1 + matrixOfRoi[0]; 
                location[pointCount + rowIter] = contours[rowIter][localMaxPos].y-1 + matrixOfRoi[1]; 
                metric[rowIter] = dst.at<float>(location[pointCount + rowIter], location[rowIter]); 
            }
        }
        
        // To return coordinates in order
        for(int iter1 = 0; iter1 < pointCount - 1; iter1++)
        {
            coordinateMin = location[iter1];
            coordinatePos = iter1;
            for(int iter2 = iter1 + 1; iter2 < pointCount; iter2++)
            {
                if(location[iter2] < coordinateMin)
                {
                        coordinateMin = location[iter2];
                        coordinatePos = iter2;
                }
                else if(location[iter2] == coordinateMin)
                {
                    if(location[pointCount + iter2] < location[pointCount + coordinatePos])
                    {
                        coordinateMin = location[iter2];
                        coordinatePos = iter2;
                    }
                }
            }
            // Swapping x coordinate
            tempForSwapping = location[coordinatePos];
            location[coordinatePos] = location[iter1];
            location[iter1] = tempForSwapping; 
            // Swapping y coordinate
            tempForSwapping = location[pointCount + coordinatePos];
            location[pointCount + coordinatePos] = location[pointCount + iter1];
            location[pointCount + iter1] = tempForSwapping; 
            // Swapping metric
            tempForSwapping = metric[coordinatePos];
            metric[coordinatePos] = metric[iter1];
            metric[iter1] = tempForSwapping; 
        }
        
        //------Create output arguments------//
        sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx)+1, pointCount, 2, location); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, pointCount, 1, metric); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx)+3, 1, 1, &pointCount); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        
        //------Return Arguments------//
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2; 
        AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3; 
        ReturnArguments(pvApiCtx); 
        return 0; 
    }
/* ==================================================================== */
}