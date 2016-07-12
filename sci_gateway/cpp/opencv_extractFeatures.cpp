/****************************************************************************************************************************************************************************************
*	Author:	Umang Agrawal																				*
*	Code:	extractFeatures.cpp																			*
*	Function Format:	[featureVector ValidPoints] = extractFeatures( image, Points, Optional Arguments.....)									*
*		Requirements:	Image should be graysale																*
*		Necessary Arguments:		Name			Value														*
*						"Class"			[ "cornerPoints", "MSERRegions", "SURFPoints", "BRISKPoints", "Matrix" ]					*
*																							*
*		Optional Arguments:		Name			Value														*
*						"Method"		[ "SURF", "FREAK", "Block", "BRISK" ]										*
*						"BlockSize"		Any Odd Integer Value												*
*						"SURFSize"		64 or 128													*
*						"Upright"		Boolean True or False												*
*						"Metric"		Metric Matrix of order Mx1											*
*						"Orientation"		Orientation Matrix of Order Mx1											*
*						"Axes"			Axes Matrix of Order Mx2											*
*						"Scale"			Scale Matrix of Order Mx1											*
*						"SignOfLaplacian"	SignOfLaplacian Matrix of Order Mx1										*
*																							*
****************************************************************************************************************************************************************************************/

#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"

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

	/*string type2str(int type) 
	{
  		string r;
  		uchar depth = type & CV_MAT_DEPTH_MASK;
  		uchar chans = 1 + (type >> CV_CN_SHIFT);
  		switch ( depth ) 
		{
    		case CV_8U:  r = "8U"; break;
    		case CV_8S:  r = "8S"; break;
    		case CV_16U: r = "16U"; break;
    		case CV_16S: r = "16S"; break;
    		case CV_32S: r = "32S"; break;
    		case CV_32F: r = "32F"; break;
    		case CV_64F: r = "64F"; break;
    		default:     r = "User"; break;
  		}
  		r += "C";
  		r += (chans+'0');
		return r;
	}*/

	int opencv_extractFeatures(char *fname, unsigned long fname_len)
	{
		//......................................................................................................................................................................................
		//Declarations
		// Error Handling		
		SciErr sciErr;

		// Address of Various Scilab API
		int *piAddr1 = NULL;
		int *piAddr2 = NULL;
		int *piAddr3 = NULL;
		int *piAddr4 = NULL;

		// Number of input and output Arguments
		int inputArguments = 0;
		int out_params;

		// Matrix Reading Argument
		int iPrec = 0;
		int noItemInList = 0;
		int iRows,iCols;
		int Rows, Cols;	
		int *pilen = NULL;
		char **arg = NULL;
		char **method = NULL;
		char **clas = NULL;
		int length_M = 0;

		// Necessary Output Arguments
		double *valid_points = NULL;	
		double numBits = 0;
		double numFeatures = 0;
		double *featureVector = NULL;	

		// Count of various Arguments
		int count_method=0, count_block=0, count_surf=0, count_upright=0 ,count_metric=0;
		int count_orient=0, count_scale=0, count_axes=0, count_sign=0, count_class=0;
		
		// Iterator
		int i;
		
		//Input Arguments Parameters
		double *metric = NULL;
		double *orient = NULL;
		double *axes = NULL;
		double *scale = NULL;
		double *sign = NULL;
		double block_size = 0;
		double upright = 1;
		double surf_size = 64;

		// Keypoint Vector
		vector<KeyPoint> keypoints;
		vector<KeyPoint> copy_k;

		//Image Container
		Mat image;

		//Output Argument
		int feature_rows, feature_cols;
		int valid_rows;
		double count;
		double *metric_o = NULL;
		double *scale_o = NULL;	
		double *orientation_o = NULL;
		double *axes_o = NULL;
		double *sign_o = NULL;

		// Checks on Number of Input and Output Arguments
		CheckInputArgument(pvApiCtx, 4, 22);
		CheckOutputArgument(pvApiCtx, 2, 9);

		inputArguments = *getNbInputArgument(pvApiCtx);
		
		//.......................................................................................................................................................................................
		//Checking for the Presence of Valid Number of Arguments
		if( inputArguments%2 == 0)
		{
			
			//Argument 1 - Image
			sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			if(isListType(pvApiCtx, piAddr1))
			{
				sciErr = getListItemNumber(pvApiCtx, piAddr1, &noItemInList);
				if( noItemInList != 1)
				{
					Scierror(999,"Expecting a grayscale image as first Argument\n");
					return 0;
				}
				retrieveImage(image, 1);
			}
			else
			{
				if( !isDoubleType(pvApiCtx, piAddr1))
				{
					Scierror(999,"Expecting a grayscale Image as first Argument\n");
					return 0;
				}
				retrieveImage(image, 1);
			}

			
			//..............................................................................................................................................................................
			//Argument 2 - Location Co-ordinates
			sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			if( !isVarMatrixType(pvApiCtx, piAddr2))
			{
				Scierror(999,"Expecting a Matrix of Mx2 order depicting the Location Coordinates as 2nd Argument.\n");
				return 0;
			}
			//Checking the type of location coordinates
			if(isDoubleType(pvApiCtx, piAddr2))
			{
				//for double location co-ordinates 
				double *points = NULL;	
				sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &points);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;	
				}
				length_M = iRows;
				KeyPoint *X = new KeyPoint;
				for(int i=0; i<iRows; i++)
				{
					X->pt = Point2f(points[i],points[iRows+i]);
					X->size = 7;
					keypoints.push_back(*X);
					copy_k.push_back(*X);
				}
			}
			else if(isIntegerType(pvApiCtx, piAddr2))
			{
				sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddr2, &iPrec);
        			if(sciErr.iErr)
        			{
            				printError(&sciErr, 0);
           				return 0;
    			    	}
				switch(iPrec)
			        {
			           	case SCI_UINT8: 	//for unsigned integer 8 location co-ordinates
            					{				
						unsigned char *points = NULL;
						sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddr2, &iRows, &iCols, &points);
						if(sciErr.iErr)						
						{
							printError(&sciErr, 0);
							return 0;	
						}
						length_M = iRows;
						KeyPoint *X = new KeyPoint;
						for(int i=0; i<iRows; i++)
						{
							X->pt = Point2f(points[i],points[iRows+i]);
							X->size = 7;
							keypoints.push_back(*X);
							copy_k.push_back(*X);
						}
						break;
						}
					case SCI_UINT16:	//for unsigned integer 16 location co-ordinates
						{
						unsigned short *points = NULL;	
						sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddr2, &iRows, &iCols, &points);
						if(sciErr.iErr)
						{
							printError(&sciErr, 0);
							return 0;	
						}
						length_M = iRows;
						KeyPoint *X = new KeyPoint;
						for(int i=0; i<iRows; i++)
						{
							X->pt = Point2f(points[i],points[iRows+i]);
							X->size = 7;
							keypoints.push_back(*X);
							copy_k.push_back(*X);
						}
						break;
						}
					case SCI_INT32:
						{
						int *points = NULL;
						sciErr = getMatrixOfInteger32(pvApiCtx, piAddr2, &iRows, &iCols, &points);
						if(sciErr.iErr)
						{
							printError(&sciErr, 0);
							return 0;	
						}
						length_M = iRows;
						KeyPoint *X = new KeyPoint;
						for(int i=0; i<iRows; i++)
						{
							X->pt = Point2f(points[i],points[iRows+i]);
							X->size = 7;
							keypoints.push_back(*X);
							copy_k.push_back(*X);
						}
						break;
						}
				}
			}
			else
			{
				Scierror(999,"Expecting a Double or an Integer Matrix as Location Coordinates in 2nd Argument.\n");
				return 0;
			}


			//.............................................................................................................................................................................
			//Argument 3 - Class
			sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			//Check for Argument type
			if( !isStringType(pvApiCtx, piAddr3))
			{
				Scierror(999,"Expecting String Argument \"Class\" as Argument 3.\n");
				return 0;
			}
			//Matrix of Stings
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &Rows, &Cols, NULL, NULL);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			pilen = (int*)malloc(sizeof(int) * Rows * Cols);
			//second call to retrieve the length of the string
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &Rows, &Cols, pilen, NULL);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				free(pilen);
				return 0;
			}
			arg = (char**)malloc(sizeof(char*) * Rows * Cols);
			for(int j=0;j< Rows * Cols; j++)
			{
				arg[j] = (char*)malloc(sizeof(char) * (pilen[j] + 1));
			}
			//third call to retrieve data
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &Rows, &Cols, pilen, arg);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				free(pilen);
				free(arg);
				return 0;
			}
			
			//Checking Whether Argument 3 is Method or not
			if(strcmp(arg[0],"Class") == 0)
			{
				//If method extracting the Value of the Argument
				if(count_class != 0)
				{
					Scierror(999,"Class Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);				
				sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !isStringType(pvApiCtx, piAddr4))
				{
					Scierror(999, "%s: Wrong type of 2nd argument #%d. A string is expected.\n", fname, 1);
					return 0;
				}
				//Matrix of Stings
				sciErr = getMatrixOfString(pvApiCtx, piAddr4, &Rows, &Cols, NULL, NULL);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				pilen = (int*)malloc(sizeof(int) * Rows * Cols);
				//second call to retrieve the length of the string
				sciErr = getMatrixOfString(pvApiCtx, piAddr4, &Rows, &Cols, pilen, NULL);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					free(pilen);
					return 0;
				}
				clas = (char**)malloc(sizeof(char*) * Rows * Cols);
				for(int j=0;j< Rows*Cols; j++)
				{
					clas[j] = (char*)malloc(sizeof(char) * (pilen[j] + 1));
				}
				//third call to retrieve data
				sciErr = getMatrixOfString(pvApiCtx, piAddr4, &Rows, &Cols, pilen, clas);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					free(pilen);
					free(clas);
					return 0;
				}
				count_class += 1;
			}
			else 
			{
				Scierror(999,"Expecting \"Class\" as the 3rd Argument\n");
				return 0;
			}
		}
		else
		{
			Scierror(999,"Required Arguments are Missing\n");
			return 0;
		}
		



		//..........................................................................................................................................................................................
		//Getting Optional Input Arguments
		for( i=5; i<= inputArguments; i++)
		{
			sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddr3);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			//Check for Argument type
			if( !isStringType(pvApiCtx, piAddr3))
			{
				Scierror(999, "%s: Wrong type of argument #%d. A string is expected.\n", fname, 1);
				return 0;
			}
			//Matrix of Stings
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &Rows, &Cols, NULL, NULL);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			pilen = (int*)malloc(sizeof(int) * Rows * Cols);
			//second call to retrieve the length of the string
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &Rows, &Cols, pilen, NULL);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				free(pilen);
				return 0;
			}
			arg = (char**)malloc(sizeof(char*) * Rows * Cols);
			for(int j=0;j< Rows * Cols; j++)
			{
				arg[j] = (char*)malloc(sizeof(char) * (pilen[j] + 1));
			}
			//third call to retrieve data
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &Rows, &Cols, pilen, arg);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				free(pilen);
				free(arg);
				return 0;
			}



			//.................................................................................................................................................................................
			//Reading the Value of Optional Arguments
			//If Optional Argument is Method
			if(strcmp(arg[0],"Method") == 0)
			{
				if(count_method!= 0)
				{
					Scierror(999,"Method Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);				
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !isStringType(pvApiCtx, piAddr4))
				{
					Scierror(999, "%s: Wrong type of 2nd argument #%d. A string is expected.\n", fname, 1);
					return 0;
				}
				//Matrix of Stings
				sciErr = getMatrixOfString(pvApiCtx, piAddr4, &Rows, &Cols, NULL, NULL);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				pilen = (int*)malloc(sizeof(int) * Rows * Cols);
				//second call to retrieve the length of the string
				sciErr = getMatrixOfString(pvApiCtx, piAddr4, &Rows, &Cols, pilen, NULL);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					free(pilen);
					return 0;
				}
				method = (char**)malloc(sizeof(char*) * Rows * Cols);
				for(int j=0;j< Rows*Cols; j++)
				{
					method[j] = (char*)malloc(sizeof(char) * (pilen[j] + 1));
				}
				//third call to retrieve data
				sciErr = getMatrixOfString(pvApiCtx, piAddr4, &Rows, &Cols, pilen, method);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					free(pilen);
					free(method);
					return 0;
				}
				i++;
				count_method += 1;
			}
			//.................................................................................................................................................................................
			//If Optional Argument is Metric
			else if(strcmp(arg[0],"Metric") == 0)
			{
				if(count_metric != 0)
				{
					Scierror(999,"Metric Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);				
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !isVarMatrixType(pvApiCtx, piAddr4))
				{
					Scierror(999,"Expecting a Matrix of Mx1 order depicting the Metric Values as Value of Metric Argument.\n");
					return 0;
				}
				sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &metric);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;	
				}
				if( iCols != 1)
				{
					Scierror(999,"Expecting a single Column Metric Matrix\n");
					return 0;
				}
				if( iRows != length_M)
				{
					Scierror(999,"Expecting a M Row matrix having M number of elements corresponding to M rows of location coordinates\n");
					return 0;
				}
				i++;
				count_metric += 1;
			}
			//.................................................................................................................................................................................
			//If Optional Argument is BlockSize
			else if(strcmp(arg[0],"BlockSize") == 0)
			{
				if(count_block != 0)
				{
					Scierror(999,"BlockSize has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !(isDoubleType(pvApiCtx, piAddr4)) )
				{
					Scierror(999,"BlockSize Value should be an odd Integer\n");
					return 0;
				}
				if(getScalarDouble(pvApiCtx, piAddr4, &block_size))
				{
					Scierror(999,"Cannot Read Block Size Value");
					return 0;
				}
				if(int(block_size)%2 == 0)
				{
					Scierror(999,"Expecting a odd value for Block Size");
					return 0;
				}
				i++;
				count_block += 1;
			}
			//.................................................................................................................................................................................
			//If Optional Argument is Upright
			else if(strcmp(arg[0],"Upright") == 0)
			{
				if(count_upright != 0)
				{
					Scierror(999,"Upright Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !(isDoubleType(pvApiCtx, piAddr4)) )
				{
					Scierror(999,"Upright Value must be a logic scalar\n");
					return 0;
				}
				if(getScalarDouble(pvApiCtx, piAddr4, &upright))
				{
					Scierror(999,"Cannot Read Upright Value\n");
					return 0;
				}
				if( !(int(upright)==0 || int(upright)==1) )
				{
					Scierror(999,"Upright Value must be a logic scalar\n");
					return 0;
				}
				i++;
				count_upright += 1;
			}
			//.................................................................................................................................................................................
			//If Optional Argument is SURFSize
			else if(strcmp(arg[0],"SURFSize") == 0)
			{
				if(count_surf != 0)
				{
					Scierror(999,"SURFSize Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !(isDoubleType(pvApiCtx, piAddr4)) )
				{
					Scierror(999,"Surf Size Value should be an Integer\n");
					return 0;
				}
				if(getScalarDouble(pvApiCtx, piAddr4, &surf_size))
				{
					Scierror(999,"Cannot Read Surf Size Value");
					return 0;
				}
				if( int(surf_size) != 64 && int(surf_size) !=128 )
				{
					Scierror(999,"Expecting value 64 or 128 for Surf Size\n");
					return 0;
				}
				i++;
				count_surf += 1;
			}
			//.................................................................................................................................................................................
			//If Optional Argument is Orientation
			else if(strcmp(arg[0],"Orientation") == 0)
			{
				if(count_orient != 0)
				{
					Scierror(999,"Orientation Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);				
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !isVarMatrixType(pvApiCtx, piAddr4))
				{
					Scierror(999,"Expecting a Matrix of Mx1 order depicting the Orientation Values as Value of Orientation Argument.\n");
					return 0;
				}
				sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &orient);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;	
				}
				if( iCols != 1)
				{
					Scierror(999,"Expecting a single Column Orientation Matrix\n");
					return 0;
				}
				if( iRows != length_M)
				{
					Scierror(999,"Expecting a M Row matrix having M number of elements corresponding to M rows of location coordinates\n");
					return 0;
				}
				i++;
				count_orient += 1;
			}
			//.................................................................................................................................................................................
			//If Optional Argument is Axes
			else if(strcmp(arg[0],"Axes") == 0)
			{
				if(count_axes != 0)
				{
					Scierror(999,"Axes Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);				
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !isVarMatrixType(pvApiCtx, piAddr4))
				{
					Scierror(999,"Expecting a Matrix of Mx2 order depicting the Axes as Value of Axes Argument.\n");
					return 0;
				}
				sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &axes);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;	
				}
				if( iCols != 2)
				{
					Scierror(999,"Expecting a 2 Column Axes Matrix\n");
					return 0;
				}
				if( iRows != length_M)
				{
					Scierror(999,"Expecting a M Row matrix having M number of elements corresponding to M rows of location coordinates\n");
					return 0;
				}
				i++;
				count_axes += 1;
			}
			//................................................................................................................................................................................
			//If Optional Argument is Scale
			else if(strcmp(arg[0],"Scale") == 0)
			{
				if(count_scale != 0)
				{
					Scierror(999,"Scale Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);				
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !isVarMatrixType(pvApiCtx, piAddr4))
				{
					Scierror(999,"Expecting a Matrix of Mx1 order depicting the Scale as Value of Scale Argument.\n");
					return 0;
				}
				sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &scale);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;	
				}
				if( iCols != 1)
				{
					Scierror(999,"Expecting a single Column Scale Matrix\n");
					return 0;
				}
				if( iRows != length_M)
				{
					Scierror(999,"Expecting a M Row matrix having M number of elements corresponding to M rows of location coordinates\n");
					return 0;
				}
				i++;
				count_scale += 1;	
			}
			//................................................................................................................................................................................
			//If Optional Argument is SignOfLaplacian
			else if(strcmp(arg[0],"SignOfLaplacian") == 0)
			{
				if(count_sign != 0)
				{
					Scierror(999,"SignOfLaplacian Argument has been called twice\n");
					return 0;
				}
				free(arg);
				free(pilen);				
				sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Check for Argument type
				if( !isVarMatrixType(pvApiCtx, piAddr4))
				{
					Scierror(999,"Expecting a Matrix of Mx1 order depicting the Sign Of Laplacian as Value of SignOfLaplacian Argument.\n");
					return 0;
				}
				sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &sign);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;	
				}
				if( iCols != 1)
				{
					Scierror(999,"Expecting a single Column Axes Matrix\n");
					return 0;
				}
				if( iRows != length_M)
				{
					Scierror(999,"Expecting a M Row matrix having M number of elements corresponding to M rows of location coordinates\n");
					return 0;
				}
				i++;
				count_sign += 1;
				//Check for Validity of Laplacian Matrix
				for(int i=0; i<iRows; i++)
				{
					if( !(sign[i]==0 || sign[i]==1 || sign[i]==-1))
					{
						Scierror(999,"SignOfLaplacian Matrix should contain only 0,1 or -1 as its elements.\n");
						return 0;
					}
				}
			}
			else
			{
				Scierror(999, "Invalid Argument Name. Provide Valid Arguments\n");
				return 0;
			}
		}


		//Reading Part Completed
		//.........................................................................................................................................................................................
		//.........................................................................................................................................................................................
		//Checking for Presence of All necessary Arguments for the computation of specified Method		
		if(strcmp(clas[0], "cornerPoints") == 0)
		{
			if(count_metric==0)
			{
				Scierror(999,"For cornerPoints Object \"Metric\" Matrix is necessary.\n");
				return 0;
			}
			if(count_method==0)
			{
				method = (char**)malloc(sizeof(char*) * 1 * 1);
				method[0] = (char*)malloc(sizeof(char) * 6);
				strcpy(method[0],"FREAK");
			}
			for( i=0; i<keypoints.size(); i++)
			{
				keypoints[i].response = metric[i];
			}
		}
		else if(strcmp(clas[0], "BRISKPoints") == 0)
		{
			if(count_metric==0 || count_scale==0 || count_orient==0)
			{
				Scierror(999,"For BRISKPoints Object \"Metric\" Matrix, \"Orientation\" Matrix and \"Scale\" Matrix are necessary\n");
				return 0;
			}
			if(count_method==0)
			{
				method = (char**)malloc(sizeof(char*) * 1 * 1);
				method[0] = (char*)malloc(sizeof(char) * 6);
				strcpy(method[0],"FREAK");
			}
			for( i=0; i<keypoints.size(); i++)
			{
				keypoints[i].response = metric[i];
				keypoints[i].angle = orient[i];
			}
		}
		else if(strcmp(clas[0], "MSERRegions") == 0)
		{
			if(count_orient==0 || count_axes==0)
			{
				Scierror(999,"For MSERRegions \"Orientation\" Matrix and \"Axes\" Matrix re necessary.\n");
				return 0;
			}
			if(count_method==0)
			{
				method = (char**)malloc(sizeof(char*) * 1 * 1);
				method[0] = (char*)malloc(sizeof(char) * 5);
				strcpy(method[0],"SURF");
			}
			for( i=0; i<keypoints.size(); i++)
			{
				keypoints[i].angle = orient[i];
			}
		}
		else if(strcmp(clas[0], "SURFPoints") == 0)
		{
			if(count_scale==0 || count_metric==0 || count_sign==0 || count_orient==0 )
			{
				Scierror(999,"For SURFPoints Object \"Metric\" Matrix \"Orientation\" Matrix \"Scale\" Matrix and \"SignOfLaplacian\" Matrix is necessary.\n");
				return 0;
			}
			if(count_method==0)
			{
				method = (char**)malloc(sizeof(char*) * 1 * 1);
				method[0] = (char*)malloc(sizeof(char) * 5);
				strcpy(method[0],"SURF");
			}
			for( i=0; i<keypoints.size(); i++)
			{
				keypoints[i].response = metric[i];
				keypoints[i].angle = orient[i];
			}
		}
		else if(strcmp(clas[0], "Matrix") == 0)
		{
			if(count_block==0)
			{
				Scierror(999,"For Matirx Object \"BlockSize\" argument is necessary.\n");
				return 0;
			}
			if(count_method==0)
			{
				method = (char**)malloc(sizeof(char*) * 1 * 1);
				method[0] = (char*)malloc(sizeof(char) * 6);
				strcpy(method[0],"Block");
			}
		}
		else 
		{
			Scierror(999,"Provide a Valid Class object\n");
			return 0;
		}
				



		//All Input Checks has been completed.
		//.........................................................................................................................................................................................
		//Processing....
		
		Mat descriptor;
		

		//FREAK
		if(strcmp(method[0], "FREAK")==0)
		{	
			FREAK obj( int(upright), 1, 22.0f, 4 );

			obj.compute(image, keypoints, descriptor);
			numBits = descriptor.size[1];
			numFeatures = descriptor.size[0];
			
			valid_points = (double *)malloc(sizeof(double) * keypoints.size() * 2); 

			for( int i=0; i<keypoints.size(); i++)
			{
				valid_points[i] = keypoints[i].pt.x;
				valid_points[keypoints.size() + i] = keypoints[i].pt.y;
			}
		
			featureVector = (double*)malloc(sizeof(double)*descriptor.size[0]*descriptor.size[1]);
			for( int i=0; i<descriptor.size[0]; i++)
				for( int j=0; j<descriptor.size[1]; j++)
				{
					*(featureVector + j*descriptor.size[0] + i) = int( descriptor.at<uchar>(i,j));
				}
			feature_rows = descriptor.size[0];
			feature_cols = descriptor.size[1];
			valid_rows = descriptor.size[0];
			count = descriptor.size[0];
			
			if( strcmp(clas[0],"cornerPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}
			}
			else if(strcmp(clas[0],"BRISKPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				scale_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						scale_o[iterator] = scale[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}
			}
			else if(strcmp(clas[0],"MSERRegions") == 0)
			{
				axes_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						axes_o[iterator] = axes[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}
			}
			else if(strcmp(clas[0],"SURFPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				scale_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						scale_o[iterator] = scale[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}

				sign_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						sign_o[iterator] = sign[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}
			}	
		}

		//..........................................................................................................................................................................................
		//SURF
		else if(strcmp(method[0], "SURF")==0)
		{
			SURF obj;
			if( int(upright) == 1)
			{
				if( int(surf_size) == 64)
					obj = SURF(100, 4, 3, 0, 1);
				else if ( int(surf_size) == 128)
					obj = SURF(100, 4, 3, 1, 1); 
			}
			else if( int(upright) == 0)
			{
				if( int(surf_size) == 64)
					obj = SURF(100, 4, 3, 0, 0);
				else if ( int(surf_size) == 128)
					obj = SURF(100, 4, 3, 1, 0); 
			}			

			obj.compute(image, keypoints, descriptor);
			numBits = descriptor.size[1];
			numFeatures = descriptor.size[0];
			
			valid_points = (double *)malloc(sizeof(double) * keypoints.size() * 2); 

			for( int i=0; i<keypoints.size(); i++)
			{
				valid_points[i] = keypoints[i].pt.x;
				valid_points[keypoints.size() + i] = keypoints[i].pt.y;
			}
		
			featureVector = (double*)malloc(sizeof(double)*descriptor.size[0]*descriptor.size[1]);
			for( int i=0; i<descriptor.size[0]; i++)
				for( int j=0; j<descriptor.size[1]; j++)
				{
					*(featureVector + j*descriptor.size[0] + i) = int( descriptor.at<uchar>(i,j));
				}
			feature_rows = descriptor.size[0];
			feature_cols = descriptor.size[1];
			valid_rows = descriptor.size[0];
			count = descriptor.size[0];

			if( strcmp(clas[0],"cornerPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}	
					if(iterator == keypoints.size())
						break;				
				}
			}
			else if(strcmp(clas[0],"BRISKPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				scale_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						scale_o[iterator] = scale[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}
			}
			else if(strcmp(clas[0],"MSERRegions") == 0)
			{
				axes_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						axes_o[iterator] = axes[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}	
			}
			else if(strcmp(clas[0],"SURFPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				scale_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						scale_o[iterator] = scale[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}

				sign_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						sign_o[iterator] = sign[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}
			}
		}

		//.........................................................................................................................................................................................
		//BRISK
		else if(strcmp(method[0], "BRISK")==0)
		{
			BRISK obj;
			
			obj.compute(image, keypoints, descriptor);
			numBits = descriptor.size[1];
			numFeatures = descriptor.size[0]; 

			valid_points = (double *)malloc(sizeof(double) * keypoints.size() * 2); 

			for( int i=0; i<keypoints.size(); i++)
			{
				valid_points[i] = keypoints[i].pt.x;
				valid_points[keypoints.size() + i] = keypoints[i].pt.y;
			}
		
			featureVector = (double*)malloc(sizeof(double)*descriptor.size[0]*descriptor.size[1]);
			for( int i=0; i<descriptor.size[0]; i++)
				for( int j=0; j<descriptor.size[1]; j++)
				{
					*(featureVector + j*descriptor.size[0] + i) = int( descriptor.at<uchar>(i,j));
				}
			feature_rows = descriptor.size[0];
			feature_cols = descriptor.size[1];
			valid_rows = descriptor.size[0];
			count = descriptor.size[0];
	
			if( strcmp(clas[0],"cornerPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}
			}
			else if(strcmp(clas[0],"BRISKPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				scale_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						scale_o[iterator] = scale[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}
			}
			else if(strcmp(clas[0],"MSERRegions") == 0)
			{
				axes_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						axes_o[iterator] = axes[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}
			}
			else if(strcmp(clas[0],"SURFPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				scale_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						scale_o[iterator] = scale[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}
				
				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}

				sign_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						sign_o[iterator] = sign[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}
			}
		}

		//.........................................................................................................................................................................................
		//Block
		else if(strcmp(method[0], "Block")==0)
		{
			vector <KeyPoint> valid;
			for( i=0; i<keypoints.size(); i++)
			{
				if( keypoints[i].pt.x - (block_size-1)/2 < 0 || keypoints[i].pt.y - (block_size-1)/2 < 0)
					continue;
				if( keypoints[i].pt.x + (block_size-1)/2 > image.cols || keypoints[i].pt.y + (block_size-1)/2 > image.rows)
					continue;
				valid.push_back(keypoints[i]);
			}
			valid_points = (double *)malloc(sizeof(double)*valid.size()*2);
			featureVector = (double *)malloc(sizeof(double)*valid.size()*int(block_size)*int(block_size));
			for( int i=0; i<valid.size(); i++)
			{
				int iterator = 0;
				valid_points[i] = valid[i].pt.x;
				valid_points[valid.size() + i] = valid[i].pt.y;
				for( int j=int(valid[i].pt.x-(int(block_size/2))); j<=int(valid[i].pt.x+(int(block_size/2))); j++)
					for( int k=int(valid[i].pt.y-(int(block_size/2))); k<=int(valid[i].pt.y+(int(block_size/2))); k++)
					{
						*(featureVector + i*int(block_size)*int(block_size) + iterator) = image.at<uchar>(j,k);
						iterator = iterator+1;
					}	
			}
			feature_rows = valid.size();
			feature_cols = int(block_size)*int(block_size);
			valid_rows = valid.size();
			numFeatures = valid.size();
			numBits = int(block_size)*int(block_size);
			count = valid.size();

			if( strcmp(clas[0],"cornerPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == valid[iterator].pt.x && copy_k[i].pt.y == valid[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}	
			}
			else if(strcmp(clas[0],"BRISKPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				scale_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						scale_o[iterator] = scale[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}
			}
			else if(strcmp(clas[0],"MSERRegions") == 0)
			{
				axes_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						axes_o[iterator] = axes[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}
			}
			else if(strcmp(clas[0],"SURFPoints") == 0)
			{
				metric_o = (double*)malloc(sizeof(double) * valid_rows);
				int iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						metric_o[iterator] = metric[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				scale_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						scale_o[iterator] = scale[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}

				orientation_o = (double*)malloc(sizeof(double) * valid_rows);
				for(int i=0; i<valid_rows; i++)
				{
					orientation_o[i] = (keypoints[i].angle * 3.14)/180 ;
				}

				sign_o = (double*)malloc(sizeof(double) * valid_rows);
				iterator = 0;
				for(int i=0; i<copy_k.size(); i++)
				{
					if( copy_k[i].pt.x == keypoints[iterator].pt.x && copy_k[i].pt.y == keypoints[iterator].pt.y )
					{
						sign_o[iterator] = sign[i];
						iterator++;
					}
					if(iterator == keypoints.size())
						break;					
				}
			}
		}
		else
		{
			Scierror(999,"Input a valid method\n");
			return 0;
		} 

		

		//.........................................................................................................................................................................................
		//Output Arguments
		out_params = *getNbOutputArgument(pvApiCtx);

		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, feature_rows, feature_cols, featureVector);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, valid_rows, 2, valid_points);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
				
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, 1, 1, &numBits);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 4, 1, 1, &numFeatures);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
		AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
		AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;

		if(strcmp(clas[0],"cornerPoints") == 0)
		{
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 5, 1, 1, &count);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;

			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 6, valid_rows, 1, metric_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx) + 6;
		}
		else if(strcmp(clas[0],"BRISKPoints") == 0)
		{
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 5, valid_rows, 1, scale_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;
		
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 6, valid_rows, 1, metric_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx) + 6;
			
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 7, valid_rows, 1, orientation_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 7) = nbInputArgument(pvApiCtx) + 7;
		
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 8, 1, 1, &count);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 8) = nbInputArgument(pvApiCtx) + 8;
		}
		else if(strcmp(clas[0],"MSERRegions") == 0)
		{
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 5, 1, 1, &count);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;
		
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 6, valid_rows, 1, axes_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx) + 6;
			
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 7, valid_rows, 1, orientation_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 7) = nbInputArgument(pvApiCtx) + 7;
		}
		else if(strcmp(clas[0],"SURFPoints") == 0)
		{
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 5, 1, 1, &count);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;
		
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 6, valid_rows, 1, scale_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 6) = nbInputArgument(pvApiCtx) + 6;
			
			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 7, valid_rows, 1, metric_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 7) = nbInputArgument(pvApiCtx) + 7;

			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 8, valid_rows, 1, sign_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 8) = nbInputArgument(pvApiCtx) + 8;

			sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 9, valid_rows, 1, orientation_o);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			AssignOutputVariable(pvApiCtx, 9) = nbInputArgument(pvApiCtx) + 9;
		}

		free(valid_points);
		free(featureVector);
		free(method);
		free(clas);

		ReturnArguments(pvApiCtx);

		return 0;
	}
}
