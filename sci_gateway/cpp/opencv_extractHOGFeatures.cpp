/*******************************************************************************************************************
              Author : Kevin George

-> Syntax-  extractHOGFeatures(image,"cellsize",..., "BlockSize",...,
                                "BlockOverlap",..., "NumBins",...)

-> NumBins is strictly of integer type
********************************************************************************************************************/
#include <numeric>
#include <string.h>
#include <math.h>
#include <iostream>
#include <bits/stdc++.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/objdetect/objdetect.hpp>

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

    int opencv_extractHOGFeatures(char *fname, unsigned long fname_len)
    {

      	//Error variables
      	SciErr sciErr;
      	int intErr = 0;

        //-> Mat containers for images
        Mat img;
        Mat img_gray;

        //-> Address of Various Arguments
        int *piAddr = NULL;
        //int *piAddr2 = NULL;

        //-> Local variables
        int num_InputArgs;  //-> gives total number of arguments
        int iRows, iCols;
        int featureVector_size;
        int *piLen = NULL;
        char **pstData = NULL;  //-> why double pointer?? and what is it
        char *currentArg = NULL; //-> Stores current string representing 'name' of name,value pair arguments
        bool *providedArgs = NULL; //-> Used to check that optional argument is not entered more than once
        double *featureVector;
        //-> Name,Value pair variables
            double *CellSize = NULL; //->
            double *BlockSize = NULL; //->
            double *BlockOverlap = NULL; //->  
            /*Size CellSize ; //->
            Size BlockSize ; //->
            Size BlockOverlap ; //->    */ 

            double NumBins = 9; //-> In docs, it must be int type
           //vector< int> CellSize; CellSize.push_back(8); CellSize.push_back(8);

        //-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        //-> Checks number of input and output arguments
        CheckInputArgument(pvApiCtx, 1, 9);                     
        CheckOutputArgument(pvApiCtx, 1, 5);                    

        //-> Read Image
        retrieveImage( img, 1);

        //-> Count number of input arguments
        num_InputArgs = *getNbInputArgument(pvApiCtx);

        providedArgs = (bool*) malloc(sizeof(bool) * 5);
        
        //-> Based on number of input arguments

//************************************************************  Optional Input Arguments  ************************************************* 
         vector<KeyPoint> Points;

//************************************************  Retrieval of Name, Value Argument Pair *************************************************
         for(int iter = 2; iter<= num_InputArgs; iter++)
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

//****************************************************** Name,Value - Cellsize *****************************************************************
            if(strcmp(currentArg, "CellSize")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[0])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &CellSize); 
                    if(sciErr.iErr)
                    {
                      printError(&sciErr, 0);
                      return 0;
                    }

                    if(iRows*iCols!=2)
                    {   
                      Scierror(999,"Invalid Argument. Only two values for CellSize.\n");
                      return 0;
                    }

                    //-> Checking if values are in proper range. Same for all optional arguments
                     if(CellSize[0]<0 || CellSize[1]<0)
                     {   
                       sciprint("Arguments cannot be negative\n");
                       return 0;
                     }

                     if(CellSize[0]>16 || CellSize[1]>16)
                     {   
                       sciprint("CellSize values cannot be more than 16.\n");
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
//************************************** Name,Value - BlockSize ************************************          
            else if(strcmp(currentArg, "BlockSize")==0)
            {
              if(iter+1<= num_InputArgs && !providedArgs[1])
              {
                   sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                   if (sciErr.iErr)
                   {
                        printError(&sciErr, 0); 
                        return 0; 
                   }

                   sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &BlockSize); 
                   if(sciErr.iErr)
                   {
                      printError(&sciErr, 0);
                      return 0;
                    }

                   if(iRows*iCols!=2)
                   {   
                      Scierror(999,"Invalid Argument.Only two values for BlockSize.\n");
                      return 0;
                   }

                   //-> Checking if values are in proper range. Same for all optional arguments
                   if(BlockSize[0]<0 || BlockSize[1]<0)
                   {   
                       sciprint("Arguments cannot be negative\n");
                       return 0;
                   }

                   if(BlockSize[0]>16 || BlockSize[1]>16)
                   {   
                      sciprint("BlockSize values cannot be more than 16.\n");
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

         
//********************************************* Name,Value - BlockOverlap ************************************************
            else if(strcmp(currentArg, "BlockOverlap")==0)
            {
                 if(iter+1<= num_InputArgs && !providedArgs[2])
                 {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }

                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &BlockOverlap); 
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                     }

                    if(iRows*iCols!=2)
                    {   
                        Scierror(999,"Invalid Argument. Only two values for BlockOverlap.\n");
                        return 0;
                    }

                   //-> Checking if values are in proper range. Same for all optional arguments
                   if(BlockOverlap[0]<0 || BlockOverlap[1]<0)
                   {   
                       sciprint("Arguments cannot be negative\n");
                       return 0;
                   }

                   if(BlockOverlap[0]>16 || BlockOverlap[1]>16)
                   {   
                      sciprint("BlockOverlap values cannot be more than 16.\n");
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
//*********************************************** *********** Name,Value - NumBins **********************************************************************  
            else if(strcmp(currentArg, "NumBins")==0)
            {
                 if(iter+1<= num_InputArgs && !providedArgs[3])
                 {
                     sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                     if (sciErr.iErr)
                     {
                        printError(&sciErr, 0); 
                        return 0; 
                     }

                     intErr = getScalarDouble(pvApiCtx, piAddr, &NumBins); 
                     if(intErr)
                     {
                        return intErr; 
                     }

                     if( NumBins<= 0)
                     {
                        Scierror(999, "Error: Please provide proper value for \"%s\". NumBins value must be more than zero.\n", currentArg); 
                        return 0; 
                     }
                     providedArgs[3] = 1; 
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
          
          else
          {
               Scierror(999, "Incorrect Name type. Please re-enter the correct Name-Value pair arguments\n"); 
                    return 0;  
          }

         }//-> Brace for closing for loop

//******************************************************** Giving Default Arguments ******************************************************************* 

        if(providedArgs[0] == 0)
        {
            CellSize = (double*) malloc(sizeof(double)*2);
            CellSize[0] = 8;
            CellSize[1] = 8;
        }


        if(providedArgs[1] == 0)
        {
            BlockSize = (double*) malloc(sizeof(double)*2);
            BlockSize[0] = 16;
            BlockSize[1] = 16;
        }

        if(providedArgs[2] == 0)
        {
            BlockOverlap = (double*) malloc(sizeof(double)*2);
            BlockOverlap[0] = 8;
            BlockOverlap[1] = 8;
        }

        if(providedArgs[3] == 0)
        {
            NumBins = 9;
        }

//**************************************************************Actual Processing*******************************************************************************

      //********************** Checking if sizes are correct **********************

      if( CellSize[0] > BlockSize[0] || CellSize[1] > BlockSize[1] )
      {   
          sciprint("CellSize values cannot be more than BlockSize values.\n");
          return 0;
      }  

      if( BlockOverlap[0] > BlockSize[0] || BlockOverlap[1] > BlockSize[1] )
      {   
          sciprint("BlockOverlap values cannot be more than BlockSize values.\n");
          return 0;
      }

        //-> resizing
        //resize(img, img, Size(64,48) ); 
        resize(img, img, Size(64,128) ); 
    
        //-> Convert to grayscale(Necessary Step)
        cvtColor(img, img_gray, CV_RGB2GRAY);

        int numbins = int(NumBins);
        //-> CellSize (8,8) is only supported size
        //   in opencv 2.4.9
        //HOGDescriptor d( Size(64,128), Size(16,16), Size(8,8), Size(8,8), 9);
        HOGDescriptor d(Size(64,128), Size(BlockSize[0],BlockSize[1]), Size(BlockOverlap[0],BlockOverlap[1]),Size(CellSize[0],CellSize[1]), numbins);
        //-> This is our descriptor vector
            
        vector<float> descriptorsValues;
        vector< Point> locations ;
        d.compute( img_gray, descriptorsValues, Size(32,32), Size(0,0), locations);

        //-> Displaying feature vector size
        //sciprint("%d\n",descriptorsValues.size());
        //for(int i =0; i<descriptorsValues.size();i++)
          // sciprint("%f\t", descriptorsValues[i]);
        //cout<< descriptorsValues[i]<< "\t";*/ 
        //sciprint("%d\n",locations.size());

        featureVector_size = descriptorsValues.size();
        featureVector = (double*) malloc(sizeof(double)*featureVector_size); //doesn't have floating point
        for(int i=0; i<featureVector_size; i++)
        {
            featureVector[i] = descriptorsValues[i];
        }
//************************************************************ Output **************************************************************************
        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, featureVector_size, 1, featureVector); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        
        //-> Returning Output
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 
        ReturnArguments(pvApiCtx);
        return 0;    	
    }

}   