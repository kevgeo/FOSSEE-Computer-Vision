/********************************************************************************************************
              Author : Kevin George

-> Syntax : opticalFlowFarneback("Filename",..,"NumPyramidLevels,..,"PyramidScale",..,
                                "Num_Iteration",..,"NeighborhoodSize",..,"FilterSize",..)

-> First argument "FileName" is necessary,rest are optional
*******************************************************************************************************/
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <unistd.h>

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

    //-> Name,Value Pair Variables
            char *fileName = NULL; //-> Stores fileName 
            int NumPyramidLevels;
            int NeighborhoodSize;
            int NumIteration;
            int FilterSize;
            double PyramidScale;
    bool objectStatus3 = false;

    int opencv_opticalFlowFarneback( char *fname, unsigned long fname_len)
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
        int num_InputArgs;  //-> gives total number of arguments
        int iRows, iCols;
        int *piLen = NULL;
        char **pstData = NULL;  //-> why double pointer?? and what is it
        char *currentArg = NULL; //-> Stores current string representing 'name' of name,value pair arguments
        bool *providedArgs = NULL; //-> Used to check that optional argument is not entered more than once 

        //-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        //-> Checks number of input and output arguments
        CheckInputArgument(pvApiCtx, 0  , 5);                     
        CheckOutputArgument(pvApiCtx, 1, 5);

        //-> Count number of input arguments
        num_InputArgs = *getNbInputArgument(pvApiCtx);

        providedArgs = (bool*) malloc(sizeof(bool) * 6);

//************************************************  Retrieval of Name, Value Argument Pair *************************************************
        for(int iter = 1; iter<= num_InputArgs; iter++)
        {
            //-> Getting address of next argument
            sciErr = getVarAddressFromPosition(pvApiCtx, iter, &piAddr); 
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

            currentArg = pstData[0];
            free(pstData);
            iRows=0;
            iCols=0;
            free(piLen);

//****************************************************** Name,Value - NumPyramidLevels *****************************************************************
 
            if(strcmp(currentArg, "FileName")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[0])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
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

                    fileName = pstData[0];
                    free(pstData);
                    iRows=0;
                    iCols=0;
                    free(piLen);
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
            else if(strcmp(currentArg, "NumPyramidLevels")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[1])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    intErr = getScalarInteger32(pvApiCtx, piAddr, &NumPyramidLevels); 
                    if(intErr)
                    {
                        return intErr; 
                    }   

                    //-> Checking if values are in proper range. Same for all optional arguments
                    if( NumPyramidLevels < 0)
                    {
                        Scierror(999," Invalid Value for NumPyramidLevels. Please enter a non negative Double value\\n");
                        return 0;
                    }       

                    providedArgs[1] = 1; 
                }
                else if(providedArgs[1]) // Send an error message if an argument is provided more than once. Same for all optional arguments.
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
//****************************************************** Name,Value - PyramidScale *****************************************************************

            else if(strcmp(currentArg, "PyramidScale")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[2])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    if(!isDoubleType(pvApiCtx, piAddr))
                    {
                        Scierror(999," Invalid Value for PyramidScale. Please enter a non negative Double value\\n");
                        return 0;
                    }
                    getScalarDouble(pvApiCtx, piAddr, &PyramidScale);

                    //-> Checking if values are in proper range. Same for all optional arguments
                    if( PyramidScale < 0)
                    {
                        Scierror(999," Invalid Value for NumPyramidLevels. Please enter a non negative Double value\\n");
                        return 0;
                    }       

                    providedArgs[2] = 1;    
                }

                else if(providedArgs[2]) // Send an error message if an argument is provided more than once. Same for all optional arguments.
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

//****************************************************** Name,Value - NumIteration *****************************************************************

            else if(strcmp(currentArg, "NumIteration")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[3])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    intErr = getScalarInteger32(pvApiCtx, piAddr, &NumIteration); 
                    if(intErr)
                    {
                        return intErr; 
                    }   

                    //-> Checking if values are in proper range. Same for all optional arguments
                    if( NumIteration < 0)
                    {
                        Scierror(999," Invalid Value for NumPyramidLevels. Please enter a non negative Double value\\n");
                        return 0;
                    }       

                    providedArgs[3] = 1; 
                }

                else if(providedArgs[3]) // Send an error message if an argument is provided more than once. Same for all optional arguments.
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
//****************************************************** Name,Value - NeighborhoodSize *****************************************************************

            else if(strcmp(currentArg, "NeighborhoodSize")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[4])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    intErr = getScalarInteger32(pvApiCtx, piAddr, &NeighborhoodSize); 
                    if(intErr)
                    {
                        return intErr; 
                    }   

                    //-> Checking if values are in proper range. Same for all optional arguments
                    if( NeighborhoodSize < 0)
                    {
                        Scierror(999," Invalid Value for size of pixel Neighborhood. Please enter a non negative Double value\\n");
                        return 0;
                    }       

                    providedArgs[4] = 1; 
                }

                else if(providedArgs[4]) // Send an error message if an argument is provided more than once. Same for all optional arguments.
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
//****************************************************** Name,Value - FilterSize *****************************************************************

             else if(strcmp(currentArg, "FilterSize")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[5])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    intErr = getScalarInteger32(pvApiCtx, piAddr, &FilterSize); 
                    if(intErr)
                    {
                        return intErr; 
                    }   

                    //-> Checking if values are in proper range. Same for all optional arguments
                    if( FilterSize < 0)
                    {
                        Scierror(999," Invalid Value for size of pixel Neighborhood. Please enter a non negative Double value\\n");
                        return 0;
                    }       

                    providedArgs[5] = 1; 
                }

                else if(providedArgs[5]) // Send an error message if an argument is provided more than once. Same for all optional arguments.
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

            else
           {
               Scierror(999, "Incorrect Name type. Please re-enter the correct Name-Value pair arguments\n"); 
                    return 0;  
            }

        }//-> Braces for ending of long for loop

//******************************************************** Giving Default Arguments ******************************************************************* 

        if(providedArgs[0] == 0)
        {
            NumPyramidLevels = 3;
        }


        if(providedArgs[1] == 0)
        {
            PyramidScale = 0.5;
        }

        if(providedArgs[2] == 0)
        {
            NumIteration = 3;
        }

        if(providedArgs[3] == 0)
        {
            NeighborhoodSize = 5;
        }

        if(providedArgs[4] == 0)
        {
            FilterSize = 15;
        }
//**************************************************************Actual Processing*******************************************************************************

        objectStatus3 = true;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    int opencv_readopticalFlowFarneback(char *fname, unsigned long fname_len)
   {
        SciErr sciErr;
        int *piAddr = NULL;
        int *outList = NULL;
        unsigned char *red = NULL;
        unsigned char *green = NULL;
        unsigned char *blue = NULL; 

        if(!objectStatus3)
        {
            Scierror(999,"Set up Foreground Detection properties through ForegroundDetection() first.\n");
            return 0;
        }

        Mat image_prev, image_next, flow, GetImg, cflow;
        int s = 2;
        //char fileName[100] = "ped.avi";
        //VideoCapture stream1(fileName);   
        
        //if(!(stream1.read(GetImg))) //get one frame from video
         //    return 0;
        
         retrieveImage(GetImg,1);

        resize(GetImg, image_prev, Size(GetImg.size().width/s, GetImg.size().height/s) );
        cvtColor(image_prev, image_prev, CV_BGR2GRAY);

        //namedWindow( "prvs", WINDOW_NORMAL);
        //namedWindow( "next", WINDOW_NORMAL);
        namedWindow( "OpticalFlowFarneback", WINDOW_NORMAL);

        //while(true)
        //{
            //if(!(stream1.read(GetImg))) //get one frame from video   
              //  break;
            
            retrieveImage(GetImg,2);
            //-> Resize
            resize(GetImg, image_next, Size(GetImg.size().width/s, GetImg.size().height/s) );
            cvtColor(image_next, image_next, CV_BGR2GRAY);

            calcOpticalFlowFarneback(image_prev,image_next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

            cvtColor(image_prev, cflow, CV_GRAY2BGR);
            
            //drawOptFlowMap(flow, cflow, 10, CV_RGB(0, 255, 0));
            int step = 10;
            for(int y = 0; y < cflow.rows; y += step)
            {       
                for(int x = 0; x < cflow.cols; x += step)
                {
                    const Point2f& fxy = flow.at< Point2f>(y, x);
                    line(cflow, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                         CV_RGB(0, 255, 0) );
                    //circle(cflowmap, Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), 1, color, -1);
                }
            }

            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &outList);
            if(sciErr.iErr)
            {
                    printError(&sciErr, 0);
                    return 0;
            }

            red = (unsigned char *)malloc(sizeof(unsigned char)*cflow.rows*cflow.cols);
            green = (unsigned char *)malloc(sizeof(unsigned char)*cflow.rows*cflow.cols);
            blue = (unsigned char *)malloc(sizeof(unsigned char)*cflow.rows*cflow.cols);

            for(int k=0;k<cflow.rows;k++)
            {
                for(int p=0;p<cflow.cols;p++)
                {
                    Vec3b intensity = cflow.at<Vec3b>(k, p);
                    red[k+cflow.rows*p]=intensity.val[2];
                    green[k+cflow.rows*p]=intensity.val[1];
                    blue[k+cflow.rows*p]=intensity.val[0];
                }
            }

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, cflow.rows, cflow.cols, red);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 2, cflow.rows, cflow.cols, green);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }                   
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 3, cflow.rows, cflow.cols, blue);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                free(red);
                free(green);
                free(blue); 


            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;  
            ReturnArguments(pvApiCtx);
            return 0;

            //imshow("OpticalFlowFarneback", cflow);

            //->Display
            //imshow("prvs", image_prev);
            //imshow("next", image_next);

            //if (waitKey(5) >= 0)   
              // break;   

          //  image_prev = image_next.clone();
           // sleep(0.5);
        //}

          
   }

 }