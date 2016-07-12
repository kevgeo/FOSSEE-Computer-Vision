/***************************************************
Author : Tanmay Chaudhari, Shashank Shekhar
**************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
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
    #include "sciprint.h"
    #include "../common.h"
    //#include "../common.cpp"

	double currentTime;
	double duration;
	int fps;
	char *nameOfFile = NULL;
	int opencv_VideoReader(char *fname, unsigned long fname_len)
	{
	    
	    SciErr sciErr;
	    int *piAddr = NULL;
	    int *piLen = NULL;
	    int iRows, iCols = 0;
	    char ** pstData = NULL;
	    int frames;
	    int width;
	    int height;
	    char full_path[PATH_MAX];

	    CheckInputArgument(pvApiCtx,1,1);
	    CheckOutputArgument(pvApiCtx,1,10);	

		sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }

	    if(!isStringType(pvApiCtx, piAddr))
	    {
	        Scierror(999,"Invalid Argument\n");
	        return 0;
	    }

	    //first call to get rows and columns 
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
	        
		//second call to retrieve length of each string
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
	    for(int j = 0 ; j < iRows * iCols ; j++)
	    {
	        pstData[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));//+ 1 for null termination
	    }
		//third call to retrieve data
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }   
	    currentTime = 0; 

	    VideoCapture capture(pstData[0]);
	    nameOfFile = pstData[0];

	    if(!capture.isOpened())
	   	{
		    Scierror(999,"Could not open file\n");
	        return 0;
	   	}
	  
	   	fps = capture.get(CV_CAP_PROP_FPS);
	    frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
	    currentTime = capture.get(CV_CAP_PROP_POS_MSEC);

	    duration = (1000/double(fps))*double(frames);

	    width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	    height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    	currentTime = capture.get(CV_CAP_PROP_POS_MSEC);
    	realpath(pstData[0], full_path);

	    createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstData[0]);
	    createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx) + 2, duration);
	    createScalarInteger32(pvApiCtx,nbInputArgument(pvApiCtx) + 3, fps);
	    createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+4, currentTime);
	    createScalarInteger32(pvApiCtx,nbInputArgument(pvApiCtx) + 5, frames);
	    createScalarInteger32(pvApiCtx,nbInputArgument(pvApiCtx) + 6, width);
	    createScalarInteger32(pvApiCtx,nbInputArgument(pvApiCtx) + 7, height);
	    createSingleString(pvApiCtx, nbInputArgument(pvApiCtx)+8, full_path);

	    for(int i=0;i<=8;i++)
	    {
 			AssignOutputVariable(pvApiCtx, i) = nbInputArgument(pvApiCtx) + i;
 		}
    
	    ReturnArguments(pvApiCtx);
	    return 0;

	}

	int has_frame(char *fname, unsigned long fname_len)
	{
	    SciErr sciErr;

		if(nameOfFile==NULL)
		{
	        Scierror(999,"No video has been read yet.\n");
	        return 0;
		}

	    int *piAddr = NULL;
	    int *piLen = NULL;
	    int iRows, iCols = 0;
	    char ** pstData = NULL;

	    CheckInputArgument(pvApiCtx,1,1);
	    CheckOutputArgument(pvApiCtx,0,1);

//    ------------------------------------------- Fetching String input as Matrix of Sting ---------------------------------------------->	    

	    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }

	    if(!isStringType(pvApiCtx, piAddr))
	    {
	        Scierror(999,"Invalid Argument\n");
	        return 0;
	    }

	    //first call to get rows and columns 
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
	        
		//second call to retrieve length of each string
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
	    for(int j = 0 ; j < iRows * iCols ; j++)
	    {
	        pstData[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));//+ 1 for null termination
	    }
		//third call to retrieve data
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }   

	    if(strcmp(nameOfFile,pstData[0]))
	    {
	    	Scierror(999,"Invalid Video File. You need to read the video using VideoReader.\n");
	        return 0;
	    }

//	-------------------------------------------------------------------------------------------------------------------------->


		if(currentTime<=duration)
			createScalarBoolean(pvApiCtx,nbInputArgument(pvApiCtx) + 1, true);
		else
			createScalarBoolean(pvApiCtx,nbInputArgument(pvApiCtx) + 1, false);

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		ReturnArguments(pvApiCtx);
		
		return 0;
	}

	int read_frame(char *fname, unsigned long fname_len)
	{
		SciErr sciErr;

		if(nameOfFile==NULL)
		{
	        Scierror(999,"No video has been read yet.\n");
	        return 0;
		}

		if(currentTime>duration)
		{
	    	Scierror(999,"No more frames left to be read\n");
	        return 0;
		}

		//sciprint("Current Time of the Video is : %f\n", currentTime);
	    
	    int *piAddr = NULL;
	    int *piLen = NULL;
	    int iRows, iCols = 0;
	    char ** pstData = NULL;
	    int *singleFrameList = NULL;
	    unsigned char *red = NULL;
	    unsigned char *green = NULL;
	    unsigned char *blue = NULL;	

	    CheckInputArgument(pvApiCtx,1,1);
	    CheckOutputArgument(pvApiCtx,1,1);

//  ----------------------------------------Retrieving string argument as matrix of string ------------------------------------------>	    

	    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }

	    if(!isStringType(pvApiCtx, piAddr))
	    {
	        Scierror(999,"Invalid Argument\n");
	        return 0;
	    }

	    //first call to get rows and columns 
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
	        
		//second call to retrieve length of each string
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
	    for(int j = 0 ; j < iRows * iCols ; j++)
	    {
	        pstData[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));//+ 1 for null termination
	    }
		//third call to retrieve data
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }   

	    if(strcmp(nameOfFile,pstData[0]))
	    {
	    	Scierror(999,"Invalid Video File. You need to read the video using VideoReader.\n");
	        return 0;
	    }

//--------------------------------------- Processing Frame at the particular frame -----------------------------------

	    VideoCapture capture(nameOfFile);
	    Mat frame;
	    capture.set(CV_CAP_PROP_POS_MSEC, currentTime);
		capture >> frame;

//--------------------------------------------------------------------------------------------------------------------

	    if(frame.channels()==1)
	    {

	 		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &singleFrameList);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			red = (unsigned char *)malloc(sizeof(unsigned char)*frame.rows*frame.cols);

	    	for(int k=0;k<frame.rows;k++)
                for(int p=0;p<frame.cols;p++)
                    red[k+frame.rows*p]=frame.at<uchar>(k, p);

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, singleFrameList, 1, frame.rows, frame.cols, red);
	    	if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}		                
	
			free(red);
		}
		else
		{

	    	sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &singleFrameList);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			red = (unsigned char *)malloc(sizeof(unsigned char)*frame.rows*frame.cols);
            green = (unsigned char *)malloc(sizeof(unsigned char)*frame.rows*frame.cols);
            blue = (unsigned char *)malloc(sizeof(unsigned char)*frame.rows*frame.cols);

		    for(int k=0;k<frame.rows;k++)
		    {
                for(int p=0;p<frame.cols;p++)
                {
                    Vec3b intensity = frame.at<Vec3b>(k, p);
                    red[k+frame.rows*p]=intensity.val[2];
                    green[k+frame.rows*p]=intensity.val[1];
                    blue[k+frame.rows*p]=intensity.val[0];
                }
            }

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, singleFrameList, 1, frame.rows, frame.cols, red);
	    	if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
    		sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, singleFrameList, 2, frame.rows, frame.cols, green);
	    	if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}	    			
			sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, singleFrameList, 3, frame.rows, frame.cols, blue);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

					free(red);
					free(green);
					free(blue);
		}

		currentTime+=(1000/double(fps));
		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;	

	    ReturnArguments(pvApiCtx);
		
		return 0;
	}
}



