    //*******************************************************************************************************
// Authors : Kevin George
//
//[a b ]= findCirclesGrid(inImage,7,7,"CALIB_CB_SYMMETRIC_GRID")
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
  
  int opencv_findCirclesGrid(char *fname, unsigned long fname_len)
  {

        SciErr sciErr;
        int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr  = NULL;
    int *piLen = NULL;
    char **pstData = NULL;  //-> why double pointer?? and what is it
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    //Input
    Mat image;
    double pts_row, pts_colum; //Points per row and column
    char *flags = NULL; //-> Stores current string representing 'name' of name,value pair arguments
    //Output
    vector<Point2f> centers;

    //-> Get 8-bit grayscale or color image.
    retrieveImage(image,1);

    //-> Get number of circles per row
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr2, &pts_row);
    if(intErr)
    {
       return intErr;
    }

    if( pts_row<=0 )
    {
        Scierror(999,"Number of circles per row should be more than zero.\n");
            return 0;
    }

    //-> Get number of circles per column
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3); 
    if (sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    intErr = getScalarDouble(pvApiCtx, piAddr3, &pts_colum);
    if(intErr)
    {
       return intErr;
    }
   
    if( pts_colum<=0 )
    {
        Scierror(999,"Number of circles per column should be more than zero.\n");
            return 0;
    }

    //-> Getting flags string
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

            flags = pstData[0];
            free(pstData);
            iRows=0;
            iCols=0;
            free(piLen);

    bool patternfound;
    //-> Calling findCirclesGrid function
    if(strcmp (flags,"CALIB_CB_SYMMETRIC_GRID") == 0 )
    {   
         patternfound = findCirclesGrid(image, Size(pts_row,pts_colum), centers,CALIB_CB_SYMMETRIC_GRID+CALIB_CB_CLUSTERING);

    }

    else if(strcmp (flags,"CALIB_CB_ASYMMETRIC_GRID") == 0 )
    {   
        patternfound = findCirclesGrid(image, Size(pts_row,pts_colum), centers,CALIB_CB_ASYMMETRIC_GRID);        
    }

    else if(strcmp (flags,"CALIB_CB_CLUSTERING") == 0 )
    {   
        patternfound = findCirclesGrid(image, Size(pts_row,pts_colum), centers,CALIB_CB_CLUSTERING);       
    }   
    
     else 
    {
        sciprint("Wrong flag value used. Look at documentation for correct flag names.\n");
        return 0;
    }
    

    //-> X & Y coordinates of detected centers
    double *xcoords;
    double *ycoords;
    int coords_size = centers.size();
   
    xcoords = (double*)malloc(sizeof(double)*coords_size);
    ycoords = (double*)malloc(sizeof(double)*coords_size);
    
    for(int i=0; i<centers.size();i++)
    {
        xcoords[i] = centers[i].x;
        ycoords[i] = centers[i].y;
    }

    
    int size2 = 2*centers.size();
    int j = 0;
    double *coords = NULL; // This will be returned as output, having the coordinates
    coords = (double*)malloc(sizeof(double)*size2);
    int k = 0;
    int i = 0;
    while( j < size2 )
    {
        coords[j++] = xcoords[k++];
        coords[j++] = ycoords[i++];  
    }
    
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1 , 2, coords_size, coords); 
    if(sciErr.iErr)
    {
        printError(&sciErr, 0); 
        return 0; 
    }

    double patternfound2 = double(patternfound);
    intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+2, patternfound2);
    if(intErr)
    {
       return intErr;
    }

    //-> Returning outputs
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

 }

}
