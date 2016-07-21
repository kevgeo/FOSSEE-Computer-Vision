/*********************************************************************************
*Author : Kevin George
*
*-> To execute, isfilterseparable("Filter",<value>)
*   
*Data-> The matrix containing the values of filter/kernel
*********************************************************************************/
#include <numeric>
#include <string.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include <math.h>
#include <vector>

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

    int opencv_isfilterseparable(char *fname, unsigned long fname_len)
    {
    	//-> Error Management variables
    	SciErr sciErr;
    	int intErr=0;

        //-> Address of Various Arguments
        int *piAddr = NULL;

        //-> Local variables
        double size;
        double *data = NULL;

        Mat s;
  		Mat u;
  		Mat v;

  		int num_InputArgs;  //-> gives total number of arguments
  		int iRows, iCols;
        int rows,cols; //-> Used to check if size given is correct
        double result; //-> Returned by function
        int *piLen = NULL;
        char **pstData = NULL;  //-> why double pointer?? and what is it 
  		char *currentArg = NULL; //-> Stores current string representing 'name' of name,value pair arguments
        bool *providedArgs = NULL; //-> Used to check that optional argument is not entered more than once 

  		//-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        //-> Checks number of input and output arguments
        CheckInputArgument(pvApiCtx, 2 , 2);                     
        CheckOutputArgument(pvApiCtx, 1, 1);

        //-> Count number of input arguments
        num_InputArgs = *getNbInputArgument(pvApiCtx);

        providedArgs = (bool*) malloc(sizeof(bool) * 2);
        for(int i=0; i<2; i++)
            providedArgs[i] = 0;

//*****************************************************  Getting Input Arguments  *************************************************************
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
            free(piLen);

        

//****************************************************** Name,Value - Filter *****************************************************************

            if(strcmp(currentArg, "Filter")==0 || strcmp(currentArg, "filter")==0)
            {
            	if(iter+1<= num_InputArgs && !providedArgs[0])
                {
                	 sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &data); 
                    if(sciErr.iErr)
                    {
                      printError(&sciErr, 0);
                      return 0;
                    }
                    rows = iRows;
                    cols = iCols;
                    if( (rows*cols!=size) && (providedArgs[1]==1))
                    {   
                      Scierror(999,"Invalid Argument. Data must be entered as matrix of values.\n");
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

//****************************************************** Name,Value - Size *****************************************************************
            /*else if(strcmp(currentArg, "Size")==0 || strcmp(currentArg, "size")==0)
            {
            	if(iter+1<= num_InputArgs && !providedArgs[1])
                {
                	 sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                    
                    intErr = getScalarDouble(pvApiCtx, piAddr, &size); 
                    if(intErr)
                    {
                        return intErr; 
                    }   

                    //-> Checking if values are in proper range. Same for all optional arguments
                    if( size < 0)
                    {
                        Scierror(999," Invalid Value for Size. Please enter the correct size\n");
                        return 0;
                    }

                    if( (rows*cols) != size && providedArgs[0]==1)
                    {   
                      Scierror(999,"Invalid size given\n");
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

            else
            {
                sciprint("Wrong name-value argument pair has been entered by user\n");
            }*/

        }//-> End of big for loop
//*****************************************************  Actual Processing  *************************************************************
 
        int size2 = int(rows*cols);
        float *kdata = new float[size2];
        int idx = 0;
        for(int i = 0;i<rows; i++)
        {
        	for(int j = 0; j<rows; j++)
                kdata[idx++] = float(data[i+j*rows]);
        }

 		Mat kernel( rows, cols, CV_32F, kdata);
 		SVD::compute(kernel,s,u,v);

 		int count = 0;
	    for(int i = 0; i < s.rows; i++)
	    {
	      for(int j = 0; j < s.cols; j++)
	        {
                if(i==j)
	           {
                    if( s.at<float>(i,j) != 0)
	                   count++;
                }
	        }
	    }

	    if (count == 1)
	    	 result = 1; //-> Filter is separable

	    else
	       	result = 0; //-> Filter is not separable

        intErr = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+1, result);


        //-> Returning Output
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
	    ReturnArguments(pvApiCtx);
    	return 0;
    }

 }