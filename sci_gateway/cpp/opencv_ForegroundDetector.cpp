/***************************************************
Author : Shashank Shekhar
**************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"	
#include <opencv2/video/background_segm.hpp>
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

    bool fdAdaptLearningRate;
    double fdNumTrainingFrames=-1;
    double fdLearningRate=-1;
    double fdMinimumBackgroundRatio=-1;
    double fdNumGaussians=-1;
    double fdInitialVariance;
    int iBool;
    bool objectStatus;
    Ptr<BackgroundSubtractor> fdObj;

	int opencv_ForegroundDetector(char *fname, unsigned long fname_len)
	{

        SciErr sciErr;
        int *piAddr = NULL;
        int *piAddrVal = NULL;
        int iRows=0;
        int iCols=0;
        int flag;
        int *piLen = NULL;
        char **pstData = NULL;
        char *currentArg = NULL;
        int noOfarguments;
        double tempLearningRate;
        Ptr<BackgroundSubtractor> obj;
        int val_position;
        int iv_bool=false;
        int argPresence[6];
        for(int i=0;i<6;i++)
            argPresence[i]=0;

        CheckInputArgument(pvApiCtx, 0,12);
        CheckOutputArgument(pvApiCtx,0,1);

        noOfarguments = *getNbInputArgument(pvApiCtx);

        if(noOfarguments%2!=0)
        {
            Scierror(999," Invalid No of Arguments \n");
            return 0;
        }

//----------------------------------------------------------------- Retrieving Arguments -------------------------------------------------------------------------------
        
        for(int i=1;i<=noOfarguments;i+=2)
        {

    // first get the property
            sciErr = getVarAddressFromPosition(pvApiCtx,i,&piAddr);
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
            currentArg = pstData[0];

            if(strcmp(currentArg, "AdaptLearningRate")==0)
            {
                val_position = i+1;

                if(argPresence[0]==1)
                {
                    Scierror(999,"Do not enter the same parameter\n");
                    return 0;
                }

                sciErr = getVarAddressFromPosition(pvApiCtx,val_position,&piAddrVal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if(!isBooleanType(pvApiCtx, piAddrVal))
                {
                    Scierror(999,"Invalid Value for AdaptLearningRate. A boolean value expected.\n");
                    return 0;
                }

                if(!isScalar(pvApiCtx, piAddrVal))
                {
                    Scierror(999,"Invalid Value for AdaptLearningRate. A boolean value expected.\n");
                    return 0;
                } 
                getScalarBoolean(pvApiCtx, piAddrVal, &iBool);
                if(iBool == 0)
                    fdAdaptLearningRate = false;
                else
                    fdAdaptLearningRate = true;

                argPresence[0]=1;

            }
            else if(strcmp(currentArg, "NumTrainingFrames")==0)
            {
                val_position=i+1; 
                
                if(argPresence[1]==1)
                {
                    Scierror(999,"Do not enter the same parameter\n");
                    return 0;
                }

                sciErr = getVarAddressFromPosition(pvApiCtx,val_position,&piAddrVal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if(!isDoubleType(pvApiCtx, piAddrVal) || isVarComplex(pvApiCtx,piAddrVal) || !isScalar(pvApiCtx, piAddrVal))
                {
                    Scierror(999," Invalid Value for NumTrainingFrames.\n");
                    return 0;
                }   
        
                getScalarDouble(pvApiCtx, piAddrVal, &fdNumTrainingFrames);
                    
                if(fdNumTrainingFrames<0)
                {
                    Scierror(999," Invalid Value for NumTrainingFrames. Enter a positive value\n");
                    return 0;
                }
                argPresence[1]=1;        
            }    
            else if(strcmp(currentArg, "LearningRate")==0)
            {
                val_position=i+1; 
                
                if(argPresence[2]==1)
                {
                    Scierror(999,"Do not enter the same parameter\n");
                    return 0;
                }

                sciErr = getVarAddressFromPosition(pvApiCtx,val_position,&piAddrVal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if(!isDoubleType(pvApiCtx, piAddrVal) || isVarComplex(pvApiCtx,piAddrVal) || !isScalar(pvApiCtx, piAddrVal))
                {
                    Scierror(999," Invalid Value for LearningRate.\n");
                    return 0;
                }   
        
                getScalarDouble(pvApiCtx, piAddrVal, &tempLearningRate);
                    
                if(tempLearningRate <0)
                {
                    Scierror(999," Invalid Value for LearningRate. Enter a positive value\n");
                    return 0;
                }
                argPresence[2]=1;        
            }
            else if(strcmp(currentArg, "MinimumBackgroundRatio")==0)
            {
                val_position=i+1; 
                
                if(argPresence[3]==1)
                {
                    Scierror(999,"Do not enter the same parameter\n");
                    return 0;
                }

                sciErr = getVarAddressFromPosition(pvApiCtx,val_position,&piAddrVal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if(!isDoubleType(pvApiCtx, piAddrVal) || isVarComplex(pvApiCtx,piAddrVal) || !isScalar(pvApiCtx, piAddrVal))
                {
                    Scierror(999," Invalid Value for MinimumBackgroundRatio.\n");
                    return 0;
                }   
        
                getScalarDouble(pvApiCtx, piAddrVal, &fdMinimumBackgroundRatio);
                    
                if(fdMinimumBackgroundRatio<0)
                {
                    Scierror(999," Invalid Value for MinimumBackgroundRatio. Enter a positive value\n");
                    return 0;
                }
                argPresence[3]=1;        

            }
            else if(strcmp(currentArg, "NumGaussians")==0)
            {
                val_position=i+1; 
                
                if(argPresence[4]==1)
                {
                    Scierror(999,"Do not enter the same parameter\n");
                    return 0;
                }

                sciErr = getVarAddressFromPosition(pvApiCtx,val_position,&piAddrVal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                if(!isDoubleType(pvApiCtx, piAddrVal) || isVarComplex(pvApiCtx,piAddrVal) || !isScalar(pvApiCtx, piAddrVal))
                {
                    Scierror(999," Invalid Value for NumGaussians.\n");
                    return 0;
                }   
        
                getScalarDouble(pvApiCtx, piAddrVal, &fdNumGaussians);
                    
                if(fdNumGaussians<0)
                {
                    Scierror(999," Invalid Value for NumGaussians. Enter a positive value\n");
                    return 0;
                }
                argPresence[4]=1;        

            }
            else if(strcmp(currentArg, "InitialVariance")==0)
            {
                val_position=i+1;              
                if(argPresence[5]==1)
                {
                    Scierror(999,"Do not enter the same parameter\n");
                    return 0;
                }
  
                sciErr = getVarAddressFromPosition(pvApiCtx,val_position,&piAddrVal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                flag=0;
                if(isStringType(pvApiCtx, piAddrVal))
                {
                    //first call to get rows and columns 
                    sciErr = getMatrixOfString(pvApiCtx, piAddrVal, &iRows, &iCols, NULL, NULL);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
                    
                    //second call to retrieve length of each string
                    sciErr = getMatrixOfString(pvApiCtx, piAddrVal, &iRows, &iCols, piLen, NULL);
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
                    sciErr = getMatrixOfString(pvApiCtx, piAddrVal, &iRows, &iCols, piLen, pstData);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    if(strcmp(pstData[0],"Auto")!=0)
                    {
                        Scierror(999," Invalid Argument for InitialVariance. Did you mean 'Auto'\n ");
                        return 0;
                    }
                    iv_bool=true;
                    flag=55;
                }
                if(flag==0)
                {
                    if(!isDoubleType(pvApiCtx, piAddrVal) || isVarComplex(pvApiCtx,piAddrVal) || !isScalar(pvApiCtx, piAddrVal))
                    {
                        Scierror(999," Invalid Value for InitialVariance.\n");
                        return 0;
                    }   
                    
                    getScalarDouble(pvApiCtx, piAddrVal, &fdInitialVariance);
                        
                    if(fdInitialVariance<0)
                    {
                        Scierror(999," Invalid Value for InitialVariance. Enter a positive value\n");
                        return 0;
                    }
                }
                argPresence[5] = 1;     
            }
            else
            {
                    Scierror(999,"Invalid Argument %s\n", currentArg);
                    return 0;
            }
        }


//      --------------------------------------------- End of OIA --------------------------------------------------------

        if(argPresence[0]==1)
        {
            if(fdAdaptLearningRate==true)
            {
                if(argPresence[2]==0)
                {
                    fdLearningRate = 0.005;
                }
                else
                    fdLearningRate = tempLearningRate;
            }
        }   
        if(argPresence[1]==0)
            fdNumTrainingFrames = 150;
        if(argPresence[3]==0)
            fdMinimumBackgroundRatio = 0.7;
        if(argPresence[4]==0)
            fdNumGaussians = 5;
        if(argPresence[5]==1)
        {
            if(iv_bool)
                fdInitialVariance = 900;
        }
        else
            fdInitialVariance = 900;

        sciprint("\n");
        if(fdAdaptLearningRate==true)
            sciprint("Learning Rate : %f\n", fdLearningRate);
        sciprint("NumTrainingFrames : %f\n", fdNumTrainingFrames);
        sciprint("MinimumBackgroundRatio : %f\n", fdMinimumBackgroundRatio);
        sciprint("NumGaussians : %f\n", fdNumGaussians);
        sciprint("InitialVariance : %f\n", fdInitialVariance);
        sciprint("\n");
        fdObj = new BackgroundSubtractorMOG(fdNumTrainingFrames,fdNumGaussians,fdMinimumBackgroundRatio);
        objectStatus = true;
//      ------------------------------------------------------------------------------------------------------------------ 
    
        createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx)+1, 1);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

        ReturnArguments(pvApiCtx);
        return 0;
    }

    int opencv_readForegroundMask(char *fname, unsigned long fname_len)
    {

        SciErr sciErr;
        int *piAddr = NULL;
        int *outList = NULL;
        unsigned char *red = NULL;
        unsigned char *green = NULL;
        unsigned char *blue = NULL; 

        if(!objectStatus)
        {
            Scierror(999,"Set up Foreground Detection properties through ForegroundDetection() first.\n");
            return 0;
        }

        CheckInputArgument(pvApiCtx, 1,1);
        CheckOutputArgument(pvApiCtx,1,1);


        Mat frame;
        Mat foregoundMask;
        retrieveImage(frame,1);
        fdObj->operator()(frame, foregoundMask, fdLearningRate);
    
        if(foregoundMask.channels()==1)
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &outList);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            red = (unsigned char *)malloc(sizeof(unsigned char)*foregoundMask.rows*foregoundMask.cols);

            for(int k=0;k<foregoundMask.rows;k++)
                for(int p=0;p<foregoundMask.cols;p++)
                    red[k+foregoundMask.rows*p]=foregoundMask.at<uchar>(k, p);

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, foregoundMask.rows, foregoundMask.cols, red);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }                       
            free(red);
        }
        else
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &outList);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            red = (unsigned char *)malloc(sizeof(unsigned char)*foregoundMask.rows*foregoundMask.cols);
            green = (unsigned char *)malloc(sizeof(unsigned char)*foregoundMask.rows*foregoundMask.cols);
            blue = (unsigned char *)malloc(sizeof(unsigned char)*foregoundMask.rows*foregoundMask.cols);

            for(int k=0;k<foregoundMask.rows;k++)
            {
                for(int p=0;p<foregoundMask.cols;p++)
                {
                    Vec3b intensity = foregoundMask.at<Vec3b>(k, p);
                    red[k+foregoundMask.rows*p]=intensity.val[2];
                    green[k+foregoundMask.rows*p]=intensity.val[1];
                    blue[k+foregoundMask.rows*p]=intensity.val[0];
                }
            }

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, foregoundMask.rows, foregoundMask.cols, red);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 2, foregoundMask.rows, foregoundMask.cols, green);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }                   
            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 3, foregoundMask.rows, foregoundMask.cols, blue);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            free(red);
            free(green);
            free(blue);
        }
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;  
        ReturnArguments(pvApiCtx);        

        return 0;
    }


}