/********************************************************************************************************
              Author : Kevin George

-> Syntax : KalmanFilter("FileName",..,"dynamParams(int)",..,"measureParams(int)",..,"controlParams(int)",..,
                                "type(int)",..,"colour(string)",.."drawpath",..
                                "delay",..)

source-http://www.robot-home.it/blog/en/software/ball-tracker-con-filtro-di-kalman/ 
*******************************************************************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"	
#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <unistd.h> //-> For sleep function
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
    
    int opencv_KalmanFilter(char *fname, unsigned long fname_len)
    {

        //Error variables
        SciErr sciErr;
        int intErr = 0;

        //-> Address of Various Arguments
        int *piAddr = NULL;
        //int *piAddr2 = NULL;

        //-> Local variables
        int num_InputArgs;  //-> gives total number of arguments
        int iRows, iCols;
        int *piLen = NULL;
        char **pstData = NULL;  //-> why double pointer?? and what is it
        char *currentArg = NULL; //-> Stores current string representing 'name' of name,value pair arguments
        bool *providedArgs = NULL; //-> Used to check that optional argument is not entered more than once
        //-> Name,Value Pair Variables
            double dynamParams,measureParams,controlParams;
            int type;
            //char *colour = NULL;
            string colour;
            string drawPath; 
            string sec;
            char *filename;

        //-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        //-> Checks number of input and output arguments
        CheckInputArgument(pvApiCtx, 2, 16);                     
        CheckOutputArgument(pvApiCtx, 1, 1);                    

        //-> Count number of input arguments
        num_InputArgs = *getNbInputArgument(pvApiCtx);

        providedArgs = (bool*) malloc(sizeof(bool) * 7);
        for(int i=0; i<7; i++)
            providedArgs[i] = 0;

        //-> Based on number of input arguments


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

//****************************************************** Name,Value - Filename *****************************************************************            
            if(strcmp(currentArg, "FileName")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[5])
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

                    filename = pstData[0];
                    free(pstData);
                    iRows=0;
                    iCols=0;
                    free(piLen);

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
//****************************************************** Name,Value - dynamParams *****************************************************************


            else if(strcmp(currentArg, "dynamParams")==0)
            {

                if(iter+1<= num_InputArgs && !providedArgs[0])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }

                    intErr = getScalarDouble(pvApiCtx, piAddr, &dynamParams); 
                    if(intErr)
                    {
                        return intErr; 
                    }   

                    if( dynamParams <= 0)
                    {
                        Scierror(999," Invalid Value for dynamParams. Please enter value more than zero.\n");
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

//****************************************************** Name,Value - measureParams *****************************************************************
            else if(strcmp(currentArg, "measureParams")==0)
            {

                if(iter+1<= num_InputArgs && !providedArgs[1])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }

                    intErr = getScalarDouble(pvApiCtx, piAddr, &measureParams); 
                    if(intErr)
                    {
                        return intErr; 
                    }   

                    if( measureParams <= 0)
                    {
                        Scierror(999," Invalid Value for measureParams. Please enter value more than zero.\n");
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

//****************************************************** Name,Value - controlParams *****************************************************************            

            else if(strcmp(currentArg, "controlParams")==0)
            {

                if(iter+1<= num_InputArgs && !providedArgs[2])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }

                    intErr = getScalarDouble(pvApiCtx, piAddr, &controlParams); 
                    if(intErr)
                    {
                        return intErr; 
                    }   

                    if( controlParams <= 0)
                    {
                        Scierror(999," Invalid Value for controlParams. Please enter value more than zero.\n");
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

//****************************************************** Name,Value - type *****************************************************************            

            else if(strcmp(currentArg, "type")==0)
            {

                if(iter+1<= num_InputArgs && !providedArgs[3])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }

                    intErr = getScalarInteger32(pvApiCtx, piAddr, &type); 
                    if(intErr)
                    {
                        return intErr; 
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


//****************************************************** Name,Value - colour *****************************************************************            


            else if(strcmp(currentArg, "colour")==0)
            {

                if(iter+1<= num_InputArgs && !providedArgs[4])
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

                    colour = pstData[0];
                    free(pstData);
                    iRows=0;
                    iCols=0;
                    free(piLen);

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

//****************************************************** Name,Value - drawpath *****************************************************************            

            else if(strcmp(currentArg, "drawpath")==0)
            {

                if(iter+1<= num_InputArgs && !providedArgs[6])
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

                    drawPath = pstData[0];
                    free(pstData);
                    iRows=0;
                    iCols=0;
                    free(piLen);

                   providedArgs[6] = 1;
                   if( drawPath!="true" && drawPath!="True" && drawPath!="False" && drawPath!="false" )
                    {
                        Scierror(999,"Invalid value for drawpath");
                        return 0;
                    }
                }

                else if(providedArgs[6]) // Send an error message if an argument is provided more than once. Same for all optional arguments.
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

//****************************************************** Name,Value - delay *****************************************************************            

            else if(strcmp(currentArg, "delay")==0)
            {

                if(iter+1<= num_InputArgs && !providedArgs[7])
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

                    sec = pstData[0];
                    free(pstData);
                    iRows=0;
                    iCols=0;
                    free(piLen);

                   providedArgs[7] = 1;
                   if( sec!="true" && sec!="True" && sec!="False" && sec!="false" )
                    {
                        Scierror(999,"Invalid value for delay");
                        return 0;
                    }

                }

                else if(providedArgs[7]) // Send an error message if an argument is provided more than once. Same for all optional arguments.
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

        }//-> Braces of big for loop
    
//******************************************************** Giving Default Arguments ******************************************************************* 

        if(providedArgs[0] == 0)
        {
            dynamParams = 6;
        }


        if(providedArgs[1] == 0)
        {
            measureParams = 4;
        }

        if(providedArgs[2] == 0)
        {
            controlParams = 0;
        }

        if(providedArgs[3] == 0)
        {
            type = CV_32F;
        }

        if(providedArgs[4] == 0)
        {
            colour = "red";
        }
    	
//**************************************************************Actual Processing*******************************************************************************

        //objectStatus2 = true;
        
        //SciErr sciErr;
        //int *piAddr = NULL;
        //int *outList = NULL;
        //unsigned char *red = NULL;
        //unsigned char *green = NULL;
        //unsigned char *blue = NULL; 
    
        vector<Point> crosses;
        KalmanFilter kf(dynamParams, measureParams, controlParams, type);

        Mat state(dynamParams, 1, type);  // [x,y,v_x,v_y,w,h]
        Mat meas(measureParams, 1, type);    // [z_x,z_y,z_w,z_h]

        setIdentity(kf.transitionMatrix);

        kf.measurementMatrix = cv::Mat::zeros(measureParams, dynamParams, type);
        kf.measurementMatrix.at<float>(0) = 1.0f;
        kf.measurementMatrix.at<float>(7) = 1.0f;
        kf.measurementMatrix.at<float>(16) = 1.0f;
        kf.measurementMatrix.at<float>(23) = 1.0f;

        
        kf.processNoiseCov.at<float>(0) = 1e-2;
        kf.processNoiseCov.at<float>(7) = 1e-2;
        kf.processNoiseCov.at<float>(14) = 5.0f;
        kf.processNoiseCov.at<float>(21) = 5.0f;
        kf.processNoiseCov.at<float>(28) = 1e-2;
        kf.processNoiseCov.at<float>(35) = 1e-2;   

        // Measures Noise Covariance Matrix R
        setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));
        // <<<< Kalman Filter

        VideoCapture cap(filename);
        cap.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, 768);

        Mat frame;
        //retrieveImage(frame,1);


        char ch = 0;
        double ticks = 0;
        bool found = false;

        int notFoundCount = 0;

        //double precTick = ticks;
        //-> measures a function execution time
        //-> counting number of ticks after certain event
        //ticks = (double) cv::getTickCount();

        //double dT = (ticks - precTick) / cv::getTickFrequency(); //seconds

        //Mat res;
        //frame.copyTo( res );
        cv::Mat res;
        cv::Mat res2;

        //-> Storing last frame in variable 'res2'
        //Get total number of frames in the video
        //Won't work on live video capture
        const int frames = cap.get(CV_CAP_PROP_FRAME_COUNT);

        //Seek video to last frame
        cap.set(CV_CAP_PROP_POS_FRAMES,frames-1);

        //Capture the last frame
        cap>>frame;
        frame.copyTo( res2 );

        //Rewind video
        cap.set(CV_CAP_PROP_POS_FRAMES,0);



        // Frame acquisition
        cap >> frame;
        frame.copyTo( res );
        
    while (ch != 'q' && ch != 'Q' && !res.empty() )    
    {    
        double precTick = ticks;
        ticks = (double) cv::getTickCount();

        double dT = (ticks - precTick) / cv::getTickFrequency(); //seconds  

        if (found)
        {
            // >>>> Matrix A
            kf.transitionMatrix.at<float>(2) = dT;
            kf.transitionMatrix.at<float>(9) = dT;
            // <<<< Matrix A

            state = kf.predict();
            
            Rect predRect;
            predRect.width = state.at<float>(4);
            predRect.height = state.at<float>(5);
            predRect.x = state.at<float>(0) - predRect.width / 2;
            predRect.y = state.at<float>(1) - predRect.height / 2;

            Point center;
            center.x = state.at<float>(0);
            center.y = state.at<float>(1);
            
            circle(res, center, 2, CV_RGB(255,0,0), -1);
            rectangle(res, predRect, CV_RGB(255,0,0), 2);
        }

        // >>>>> Noise smoothing
        Mat blur;
        GaussianBlur(frame, blur, cv::Size(5, 5), 3.0, 3.0);
        // <<<<< Noise smoothing

        // >>>>> HSV conversion
        Mat frmHsv;
        cvtColor(blur, frmHsv, CV_BGR2HSV);
        // <<<<< HSV conversion

        // >>>>> Color Thresholding
        // Note: change parameters for different colors
        Mat rangeRes = cv::Mat::zeros(frame.size(), CV_8UC1);
        
//************************** Tracks object with range of RGB values  *******************************************************************

        if(colour == "red")
        {    cv::inRange(frmHsv, cv::Scalar(0, 153, 127),
                    cv::Scalar(4, 230, 179), rangeRes);
        }

        else if(colour == "green")
        {
            cv::inRange(frmHsv, cv::Scalar(30,0,0),
                    cv::Scalar(100,255,255), rangeRes);
        }

        else if(colour == "white")
        {
            cv::inRange(frmHsv, cv::Scalar(0,0,80),
                    cv::Scalar(255,50,120), rangeRes);
        }

        else if(colour == "orange" || colour == "yellow")
        {
            cv::inRange(frmHsv, cv::Scalar(15, 204, 204),
                    cv::Scalar(20, 255, 255), rangeRes);
        }

        else
        {
            Scierror(999,"The colour entered was mispelled or it is not there in the options of colour to track\n");
            return 0;
        }       

        // <<<<< Color Thresholding

        // >>>>> Improving the result
        erode(rangeRes, rangeRes, cv::Mat(), cv::Point(-1, -1), 2);
        dilate(rangeRes, rangeRes, cv::Mat(), cv::Point(-1, -1), 2);
        // <<<<< Improving the result

        // Thresholding viewing
        cv::imshow("Threshold", rangeRes);

        // >>>>> Contours detection
        vector<vector<cv::Point> > contours;
        findContours(rangeRes, contours, CV_RETR_EXTERNAL,
                         CV_CHAIN_APPROX_NONE);
        // <<<<< Contours detection

        // >>>>> Filtering
        vector<vector<cv::Point> > balls;
        vector<cv::Rect> ballsBox;
        
        for (size_t i = 0; i < contours.size(); i++)
        {
            Rect bBox;
            bBox = cv::boundingRect(contours[i]);

            float ratio = (float) bBox.width / (float) bBox.height;
            if (ratio > 1.0f)
                ratio = 1.0f / ratio;

            // Searching for a bBox almost square
            if (ratio > 0.75 && bBox.area() >= 400)
            {
                balls.push_back(contours[i]);
                ballsBox.push_back(bBox);
            }
        }
        // <<<<< Filtering

        // >>>>> Detection result
        // -> Gives the green colour box which detects
        for (size_t i = 0; i < balls.size(); i++)
        {
            drawContours(res, balls, i, CV_RGB(20,150,20), 1);
            rectangle(res, ballsBox[i], CV_RGB(0,255,0), 2);

            Point center;
            center.x = ballsBox[i].x + ballsBox[i].width / 2;
            center.y = ballsBox[i].y + ballsBox[i].height / 2;
            circle(res, center, 2, CV_RGB(20,150,20), -1);

            crosses.push_back(Point(center.x,center.y));

            stringstream sstr;
            sstr << "(" << center.x << "," << center.y << ")";
            putText(res, sstr.str(),
                        cv::Point(center.x + 3, center.y - 3),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(20,150,20), 2);
        }
        // <<<<< Detection result

        // >>>>> Kalman Update
        if (balls.size() == 0)
        {
            notFoundCount++;
            if( notFoundCount >= 100 )
            {
                found = false;
            }
            /*else
                kf.statePost = state;*/
        }
        else
        {
            notFoundCount = 0;

            meas.at<float>(0) = ballsBox[0].x + ballsBox[0].width / 2;
            meas.at<float>(1) = ballsBox[0].y + ballsBox[0].height / 2;
            meas.at<float>(2) = (float)ballsBox[0].width;
            meas.at<float>(3) = (float)ballsBox[0].height;

            if (!found) // First detection!
            {
                // >>>> Initialization
                kf.errorCovPre.at<float>(0) = 1; // px
                kf.errorCovPre.at<float>(7) = 1; // px
                kf.errorCovPre.at<float>(14) = 1;
                kf.errorCovPre.at<float>(21) = 1;
                kf.errorCovPre.at<float>(28) = 1; // px
                kf.errorCovPre.at<float>(35) = 1; // px

                state.at<float>(0) = meas.at<float>(0);
                state.at<float>(1) = meas.at<float>(1);
                state.at<float>(2) = 0;
                state.at<float>(3) = 0;
                state.at<float>(4) = meas.at<float>(2);
                state.at<float>(5) = meas.at<float>(3);
                // <<<< Initialization

                found = true;
            }
            
            else
                kf.correct(meas); // Kalman Correction

        }
        // <<<<< Kalman Update  
        // Final result
        cv::imshow("Tracking", res);
        ch = cv::waitKey(1);

        if( sec == "true" || sec == "True")
          sleep(1);

        // Frame acquisition
        cap >> frame;
        frame.copyTo( res );

    }  //end of big main loop

    //-> Drawing crosses    
        if( drawPath == "true" || drawPath == "True")
        {
            for(int i=0; i<crosses.size();i++)
            {
               //-> horizontal line
               line( res2, Point(crosses[i].x-5, crosses[i].y), Point(crosses[i].x+5, crosses[i].y), CV_RGB(255,0,0), 2 ) ;
               //-> vertical line
               line( res2, Point(crosses[i].x, crosses[i].y-5), Point(crosses[i].x, crosses[i].y+5), CV_RGB(255,0,0), 2 ) ;
               //line( res2, Point(crosses[i].x-15, crosses[i].y), Point(crosses[i].x+15, crosses[i].y), CV_RGB(0,255,0), 2 ) ;
            }

            cv::imshow("Tracking", res2);
            waitKey(0); 
        }

        //AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;  
        ReturnArguments(pvApiCtx);        
        destroyAllWindows();
        return 0;
    }
}