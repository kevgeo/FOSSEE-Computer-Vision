/***************************************************
Author : Shashank Shekhar
***************************************************/
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

	int opencv_detectBRISKFeatures(char *fname, unsigned long fname_len)
	{	

		SciErr sciErr;
		int *piAddr = NULL;
		int *piAddrVal = NULL;
    	int argPresence[4];
    	for(int i=0;i<4;i++)
	        argPresence[i]=0;
	    int val_position;
	    int iRows = 0;
	    int iCols = 0;
	    int *piLen = NULL;
	    char ** pstData = NULL;
    	char *currentArg = NULL;


//  DECLARING INPUT ARGUMENTS

		double *ROImat = NULL;
		double NumOctaves = 0;
		double MinContrast = 0;
		double MinQuality = 0;


//	DECLARING OUTPUT ARGUMENTS

		vector<KeyPoint> myPoints;
		int *LocationList = NULL;
		double *LocationData = NULL;
		double *OrientationData = NULL;
		double *ScaleData = NULL;
		double *MetricData = NULL;
		int size;

//	ARGUMENT CHECK

		CheckInputArgument(pvApiCtx, 1,9);
    	CheckOutputArgument(pvApiCtx,1,5);

//	RETRIEVING IMAGE

    	Mat image;
    	retrieveImage(image,1);

//  GRAYSCALE VALIDATION

   	    if(image.channels()!=1)
   	 	{
    	    Scierror(999," Feature is not meant for RGB images\n");
        	return 0;        
    	}

//	OBTAINING NUMBER OF ARGUMENTS

    	int noOfarguments = *getNbInputArgument(pvApiCtx);

//	ARGUMENT COUNT CHECK

    	if(noOfarguments%2==0)
   	 	{
    	    Scierror(999," Invalid Number Of Arguments\n");
        	return 0;        
    	}

//	MULTIPLE ARGUMENT HANDLING

        for(int i=2;i<noOfarguments;i=i+2)
            {
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
                free(pstData);
                iRows=0;
                iCols=0;
                free(piLen);

                if(strcmp(currentArg,"MinContrast")==0)
                {
                    val_position=i+1;  
                    
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

                    if(!isDoubleType(pvApiCtx, piAddrVal))
                    {
                        Scierror(999,"Invalid Value for MinContrast. Please enter a value between 0 and 1\n");
                        return 0;
                    }     

                    getScalarDouble(pvApiCtx, piAddrVal, &MinContrast);
                    
                    if(MinContrast<0 || MinContrast>1)
                    {
                        Scierror(999,"Invalid Value for MinContrast. Please enter a value between 0 and 1\n");
                        return 0;
                    }               
                    
                    argPresence[0]=1;       
                }
                else if(strcmp(currentArg,"NumOctaves")==0)
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

                    if(!isDoubleType(pvApiCtx, piAddrVal))
                    {
                        Scierror(999," Invalid Value for NumOctaves. Recommended values are between 1 and 4\n");
                        return 0;
                    }     

                    getScalarDouble(pvApiCtx, piAddrVal, &NumOctaves);
                    if(NumOctaves<1)
                    {
                        Scierror(999," Invalid Value for NumOctaves. Recommended values are between 1 and 4\n");
                        return 0;
                    }
                    argPresence[1]=1;     
                }
                else if(strcmp(currentArg,"MinQuality")==0)
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

                    if(!isDoubleType(pvApiCtx, piAddrVal))
                    {
                        Scierror(999," Invalid Value for MinQuality. Recommended values are between 0 and 1\n");
                        return 0;
                    }

                    getScalarDouble(pvApiCtx, piAddrVal, &MinQuality);

                    if(MinQuality<0 || MinQuality>1)
                    {
                        Scierror(999," Invalid Value for MinQuality. Please enter a value between 0 and 1   \n");
                        return 0;
                    }
          
                    argPresence[2]=1; 
                }
                else if(strcmp(currentArg,"ROI")==0)
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

                    if(!isDoubleType(pvApiCtx, piAddrVal) || isVarComplex(pvApiCtx, piAddrVal))
                    {
                        Scierror(999,"Enter a List of 4 arguments\n");
                        return 0;
                    }

                    sciErr = getMatrixOfDouble(pvApiCtx, piAddrVal, &iRows, &iCols, &ROImat);  
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    if(iRows*iCols!=4)
                    {   
                        Scierror(999,"Invalid Argument\n");
                        return 0;
                    }

                    if(ROImat[0]<0 || ROImat[1]<0 || ROImat[2]<0 || ROImat[3]<0)
                    {   
                        Scierror(999,"Arguments cannot be negative\n");
                        return 0;
                    }       

                    if(ROImat[0]>image.cols || ROImat[1]>image.rows || ROImat[0]+ROImat[2]>image.cols
                        || ROImat[1]+ROImat[3]>image.rows) 
                    {
                        Scierror(999,"Please make sure the arguments are within the image range\n");
                        return 0;
                    }

                    argPresence[3]=1;
                }
                else
                {
                    Scierror(999, "Error: \"%s\" Invalid input argument.\n", currentArg); 
                    return 0; 
                }

            }


//	DEFAULT ARGUMENTS

            if(argPresence[0]==0)
                MinContrast=0.2;
            if(argPresence[1]==0)
                NumOctaves=4;
            if(argPresence[2]==0)
                MinQuality=0.1;
            if(argPresence[3]==0)
            {
                ROImat=(double *)malloc(sizeof(double)*1*4);
                ROImat[0]=0;
                ROImat[1]=0;
                ROImat[2]=image.cols;
                ROImat[3]=image.rows;       
            }

//	CREATING REGION OF INTEREST    

            Rect masker(ROImat[0], ROImat[1], ROImat[2], ROImat[3]);
            
            Mat mask(image.size(), CV_8UC1, Scalar::all(0));
            mask(masker).setTo(Scalar::all(255));

//	PROCESSING BRISK


    	// Setting Threshold ------------------------------------------------->

    	double Threshold = MinContrast * 255;

    	// Limiting NumOctaves ----------------------------------------------->

	    int g = image.rows>image.cols?image.rows:image.cols;
    	int maxNumOctaves = int(floor(log2(g)));
    	if(NumOctaves>maxNumOctaves)
    		NumOctaves=maxNumOctaves;

    	//-----------------------------------------------------------------------

        BRISK myobject(Threshold, NumOctaves);
        myobject.operator()(image, mask, myPoints, noArray());

        // Selecting Points according to MinQuality --------------------------->

        size = myPoints.size();
	
	    double maxMetric = 0;
        double minMetric = 0;

        for(int i=0;i<size;i++)
        {
        	if(myPoints[i].response>maxMetric)
        		maxMetric=myPoints[i].response;
        }
        minMetric = maxMetric*MinQuality;

        int finalSize = 0;
        for(int i=0;i<size;i++)
        {
        	if(myPoints[i].response>=minMetric)
        		finalSize++;
        }
  
  		//------------------------------------------------------------------------

//	LOCATION   ---->

    LocationData = (double *)malloc(sizeof(double)*finalSize*2);
	MetricData = (double *)malloc(sizeof(double)*finalSize);
	OrientationData = (double *)malloc(sizeof(double)*finalSize);
	ScaleData = (double *)malloc(sizeof(double)*finalSize);
    int k=0;
    //   Appending 1x2 Matrices containing the coordinates of the keypoints, as well as assiging other attributes
    for(int i=0;i<size;i++)
    {
    	if(myPoints[i].response>=minMetric)
    	{	        
	        LocationData[k++] = myPoints[i].pt.x;
	        LocationData[k++] = myPoints[i].pt.y;
	        OrientationData[i] = 0;    
	        MetricData[i] = myPoints[i].response;
	        ScaleData[i] = myPoints[i].size;	       
   		}	
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, finalSize, 2, LocationData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
       

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, finalSize, 1, OrientationData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
       
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, finalSize,1, MetricData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 4,finalSize,1, ScaleData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }    

    createScalarInteger32(pvApiCtx,nbInputArgument(pvApiCtx) + 5, finalSize);

//	ASSIGNING   ---->

    for(int i=1;i<=5;i++)
    {
 		AssignOutputVariable(pvApiCtx, i) = nbInputArgument(pvApiCtx) + i;   	
    }

    ReturnArguments(pvApiCtx);
	}

}