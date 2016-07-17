//*******************************************************************************************************
// Authors : Kevin George
//
// reprojectImageTo3D(disparity, outImage, PTMatrix, handleMissingValues, ddepth );
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
  
  int opencv_reprojectImageTo3D(char *fname, unsigned long fname_len)
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
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    //-> Input
    Mat disparity;
    double *PTMatrix;
    double handleMissingValues = 0;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    char *ddepth = NULL; //-> Stores current string representing 'name' of name,value pair arguments
    int *piLen = NULL;

    //-> Output
    Mat outImage;

    //-> Get disparity image
    retrieveImage(disparity,1);

    //-> Get perspective transformation matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &PTMatrix);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 

    Mat ptMatrix(4,4,DataType<double>::type);
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            ptMatrix.at<double>(i,j) = PTMatrix[i+j*4];
        }
    }


    //-> Get handleMissingValues
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &handleMissingValues);
    if(intErr)
    {
       return intErr;
    }

    //-> Getting ddepth values
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    //-> Extracting name of next argument takes three calls to getMatrixOfString
    //-> First call to get rows and columns
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    piLen = (int*) malloc(sizeof(int) * iRows * iCols);

    //-> Second call to retrieve length of each string
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

    //-> Third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    ddepth = pstData[0];
    free(pstData);
    iRows=0;
    iCols=0;
    free(piLen); 

    if( handleMissingValues == 0)
    {
        if( ddepth == "CV_16S")
            reprojectImageTo3D(disparity, outImage, ptMatrix, false , CV_16S);

        else if( ddepth == "CV_32S")
            reprojectImageTo3D(disparity, outImage, ptMatrix, false , CV_32S);
        
        else
            reprojectImageTo3D(disparity, outImage, ptMatrix, false , CV_32F );
    
    }

    else if( handleMissingValues == 1)
    {
        if( ddepth == "CV_16S")
            reprojectImageTo3D(disparity, outImage, ptMatrix, true , CV_16S);

        else if( ddepth == "CV_32S")
            reprojectImageTo3D(disparity, outImage, ptMatrix, true , CV_32S);
        
        else
            reprojectImageTo3D(disparity, outImage, ptMatrix, true , CV_32F );    
    
    }
    

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
