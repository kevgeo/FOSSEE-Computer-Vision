/***************************************************
Author : Shashank Shekhar
***************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
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

int opencv_detectSURFFeatures(char *fname, unsigned long fname_len)
{
    
    SciErr sciErr;


    int *piAddr = NULL;
    int *piAddrVal = NULL;
    int val_position;
    int *piLen = NULL;
    int iRows, iCols = 0;
    char ** pstData = NULL;
    char *currentArg = NULL;
    int valOfCA;
    int argPresence[4];
    for(int i=0;i<4;i++)
        argPresence[i]=0;

    vector<KeyPoint> myPoints;

    int *LocationList = NULL;
    int *OrientationList = NULL;
    int *MetricList = NULL;
    int *LaplacianList = NULL;
    int *ScaleList = NULL;

    double *LocationData = NULL;
    double *OrientationData = NULL;
    double *MetricData = NULL;
    int *LaplacianData = NULL;	
    double *ScaleData = NULL;

    int noOfarguments = *getNbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1,9);
    CheckOutputArgument(pvApiCtx,1,6);    

    Mat image;
    retrieveImage(image,1);


    if(image.channels()!=1)
    {
        Scierror(999," Feature is not meant for RGB images\n");
        return 0;        
    }

//********************************************* Valid Argument Count Check *****************************************

    if(noOfarguments%2==0)
    {
        Scierror(999," Invalid No of Arguments \n");
        return 0;
    } 

//*********************************************  Optional Input Arguments  ***************************************** 

    double MetricThreshold = 0;
    double NumOctaves = 0;
    double NumScaleLevels = 0;
    double *ROImat = NULL;

//*********************************************  Retrieval of Name, Value Argument Pair ****************************


    for(int i=2;i<=noOfarguments;i=i+2)
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

        if(strcmp(currentArg,"MetricThreshold")==0)
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
                Scierror(999," Invalid Value for MetricThreshold. Please enter a non negative Double value\\n");
                return 0;
    	    }
	
			
       	    getScalarDouble(pvApiCtx, piAddrVal, &MetricThreshold);
                  
            if(MetricThreshold<0)
            {
                Scierror(999," Invalid Value for MetricThreshold. Please enter a non negative Double value\\n");
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
	
                Scierror(999," Invalid Value for NumOctaves. Recommended Values are between 1 and 4\n");
                return 0;
	    }       
             
	 getScalarDouble(pvApiCtx, piAddrVal, &NumOctaves);
                
                    if(NumOctaves<1)
                    {
                        Scierror(999," Invalid Value for NumOctaves. Recommended Values are between 1 and 4\n");
                        return 0;
                    }
                

            argPresence[1]=1;     
        }
        else if(strcmp(currentArg,"NumScaleLevels")==0)
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
	
               Scierror(999," Invalid Value for NumScaleLevels. Recommended values are between 3 and 6\n");
                return 0;
	    }   
                getScalarDouble(pvApiCtx, piAddrVal, &NumScaleLevels);
                
                    if(NumScaleLevels<3)
                    {
                        Scierror(999," Invalid Value for NumScaleLevels. Please enter an integer value greater than or equal to 3\n");
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
                Scierror(999,"Enter a List of 4 double arguments\n");
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
               sciprint("Arguments cannot be negative\n");
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
            Scierror(999, "Error: \"%s\" name for input argument is not valid.\n", currentArg); 
            return 0; 
        }

    }





//*********************************************  End of OIA  ********************************************************

int nOctaveLayers;

    if(argPresence[2]==1)
	nOctaveLayers = NumScaleLevels-2;	


    if(argPresence[0]==0)
        MetricThreshold=1000;
    if(argPresence[1]==0)
        NumOctaves=3;
    if(argPresence[2]==0)
        nOctaveLayers = 2;
    if(argPresence[3]==0)
    {
	ROImat=(double *)malloc(sizeof(double)*4);
        ROImat[0]=0;
        ROImat[1]=0;	
        ROImat[2]=image.cols;
        ROImat[3]=image.rows;       
    }


    Rect masker(ROImat[0], ROImat[1], ROImat[2], ROImat[3]);
    
	Mat mask(image.size(), CV_8UC1, Scalar::all(0));
	mask(masker).setTo(Scalar::all(255));
   


    SURF my_object(MetricThreshold, NumOctaves, nOctaveLayers);

    my_object.operator()(image, mask, myPoints, noArray());	

    int size= myPoints.size();

//*************************************************************** Location ***************************************************************

	 ScaleData = (double *)malloc(sizeof(double)*size);
	 OrientationData = (double *)malloc(sizeof(double)*size);
  	 MetricData = (double *)malloc(sizeof(double)*size);
 	 LaplacianData = (int *)malloc(sizeof(int)*size);

    iRows=size;
    iCols=2;
    int k=0;
     
    LocationData = (double *)malloc(sizeof(double)*iRows*iCols);

    for(int i=0;i<size;i++)
    {
       
        LocationData[k++] = myPoints[i].pt.x;
        LocationData[k++] = myPoints[i].pt.y;
	    OrientationData[i] = 0;
	    MetricData[i] = myPoints[i].response;
	    LaplacianData[i] = myPoints[i].class_id;
	    double q=myPoints[i].size;                 
//          Scale  =  Current Filter Size * Base Filter Scale / Base Filter Size
        q = q*1.2/9;
        q = round(1000.0*q)/(1000.0);
        ScaleData[i] = q;
     
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, size,2, LocationData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

//*************************************************************** Orientation ***************************************************************

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, size,1, OrientationData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

     AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2; 
        
 //***********************************************************  Metric ********************************************************************   

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, size,1, MetricData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
 
//************************************************************  SignOfLaplacian ***********************************************************
    
    sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 4, size,1, LaplacianData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;
//***********************************************************  Scale  *******************************************************************
 
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 5, size,1, ScaleData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;

//***********************************************************  Size *********************************************************************
      
    createScalarInteger32(pvApiCtx,nbInputArgument(pvApiCtx) + 6, size);
      
    AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx) + 6;

	
    //Returning the Output Variables as arguments to the Scilab environment
	
    ReturnArguments(pvApiCtx);
    return 0;

}
/* ==================================================================== */
}
