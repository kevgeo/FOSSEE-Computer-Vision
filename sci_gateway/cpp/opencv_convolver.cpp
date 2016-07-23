/******************************************************************************************
*Author : Kevin George
*
*-> To execute, Image = convolver(I1, size, values,scalar)
*   where 'I1' is image to be convoluted,
*   where 'size' is size of kernel i.e size x size gives total no. of values in kernel,
*   where 'values' contains the values of the kernel 
*   where 'scalar' is a float value
*
*******************************************************************************************/
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

    int opencv_convolver(char *fname, unsigned long fname_len)
    {
    	//-> Error Management variables
    	SciErr sciErr;
    	int intErr=0;

        //-> Mat containers for images
        Mat image_1;
        Mat image_2;

        //-> Address of Various Arguments
        int *piAddr = NULL;

        //-> Local Variables
        int num_InputArgs;  //-> gives total number of arguments
        int *piLen = NULL;
        char **pstData = NULL;  //-> why double pointer?? and what is it 
        char *currentArg = NULL; //-> Stores current string representing 'name' of name,value pair arguments
        bool *providedArgs = NULL; //-> Used to check that optional argument is not entered more than once
        double *values1 = NULL;
        float *values2 =NULL;
        double size;
        double scalar;
        int iRows = 0; 
        int iCols = 0;
        int rows,cols; //-> Used to check if size given is correct
        int *outList = NULL;
        unsigned char *red = NULL;
        unsigned char *green = NULL;
        unsigned char *blue = NULL;

        //-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        CheckInputArgument(pvApiCtx, 7, 7);                     //Check on Number of Input Arguments
        CheckOutputArgument(pvApiCtx, 1, 1);                    //Check on Number of Output Arguments

    	//-> Read Image
        retrieveImage( image_1, 1);

        //-> Count number of input arguments
        num_InputArgs = *getNbInputArgument(pvApiCtx);

        providedArgs = (bool*) malloc(sizeof(bool) * 3);
        for(int i=0; i<3; i++)
            providedArgs[i] = 0;

        //-> Based on number of input arguments
//*****************************************************  Getting Input Arguments  *************************************************************
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
            free(piLen);

//*************************************************** Taking in size of kernel *******************************************
            
            if(strcmp(currentArg, "Size")==0 || strcmp(currentArg, "size")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[0])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }
                                
                    //intErr = getScalarInteger32(pvApiCtx, piAddr, &size);
                    intErr = getScalarDouble(pvApiCtx, piAddr, &size); 
                    if(intErr)
                    {
                         return intErr; 
                    }

                    if( size <= 0)
                    {
                          Scierror(999," Invalid Value size. Please enter a non negative Double value\\n");
                          return 0;
                    }
                
                    if( ((rows*cols) != size*size) && providedArgs[0]==1)
                    {   
                      Scierror(999,"Invalid size given\n");
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

//********************************************* Taking kernel values************************************************
            else if(strcmp(currentArg, "Values")==0 || strcmp(currentArg, "values")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[1])
                {
                    
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }

                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &values1);    
                    //sciErr =  getMatrixOfInteger32(pvApiCtx, piAddr, &iRows, &iCols, &values); 
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    /*if(iRows*iCols!=9 && iRows*iCols!=16 && iRows*iCols!=25 )
                    {   
                	    Scierror(999,"Invalid Argument\n");
                        return 0;
                    }*/
                    rows = iRows;
                    cols = iCols;
                    if( (rows*cols!=size*size) && (providedArgs[0]==1) )
                    {   
                      Scierror(999,"Invalid size entered\n");
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
//************************************************* Taking scalar value ******************************************************
            else if(strcmp(currentArg, "Scalar")==0 || strcmp(currentArg, "scalar")==0)
            {
                if(iter+1<= num_InputArgs && !providedArgs[2])
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0); 
                        return 0; 
                    }

                    intErr = getScalarDouble(pvApiCtx, piAddr, &scalar); 
                    if(intErr)
                    {
                         return intErr; 
                    }

                    if( scalar == 0)
                    {
                          Scierror(999," Invalid scalar value. Please enter value more than zero.\n");
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

            else
            {
                sciprint("Wrong name-value argument pair has been entered by user\n");
            } 

        }//-> End of big for loop

//*****************************************************  Actual Processing  *************************************************************
  		
         values2 = (float*)malloc(sizeof(float)*size*size);
         int idx = 0;
         int size2 = int(size); // we need size2 in the for loop to assig values to float matrix values2
         for(int i=0; i<size; i++)
         {
               for(int j=0; j<size; j++)
                    values2[idx++] = (float)values1[i+j*size2];
         }


        Mat kernel( size,size,CV_32F, values2);
  	    Mat kernel2 = kernel/ (float)(scalar);

        Point anchor;
        double delta;
        int ddepth;

        anchor = Point( -1, -1 );
        delta = 0;
        ddepth = -1;
        filter2D(image_1, image_2, ddepth,kernel2,anchor,delta, BORDER_DEFAULT);

        /*
  	    //-> If grayscale
        if( image_2.channels() == 1)
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &outList);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            red = (unsigned char *)malloc(sizeof(unsigned char)*image_2.rows*image_2.cols);

            for(int k=0;k<image_2.rows;k++)
                for(int p=0;p<image_2.cols;p++)
                    red[k+image_2.rows*p]=image_2.at<uchar>(k, p);

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, image_2.rows, image_2.cols, red);
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

            red = (unsigned char *)malloc(sizeof(unsigned char)*image_2.rows*image_2.cols);
            green = (unsigned char *)malloc(sizeof(unsigned char)*image_2.rows*image_2.cols);
            blue = (unsigned char *)malloc(sizeof(unsigned char)*image_2.rows*image_2.cols);

            for(int k=0;k<image_2.rows;k++)
            {
                for(int p=0;p<image_2.cols;p++)
                {
                    Vec3b intensity = image_2.at<Vec3b>(k, p);
                    red[k+image_2.rows*p]=intensity.val[2];
                    green[k+image_2.rows*p]=intensity.val[1];
                    blue[k+image_2.rows*p]=intensity.val[0];
                }
            }

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, image_2.rows, image_2.cols, red);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 2, image_2.rows, image_2.cols, green);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }                   
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 3, image_2.rows, image_2.cols, blue);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                free(red);
                free(green);
                free(blue); 

        }
        */
        string tempstring = type2str(image_2.type());
        char *checker;
        checker = (char *)malloc(tempstring.size() + 1);
        memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
        returnImage(checker, image_2, 1);
        free(checker);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

}