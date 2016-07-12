/********************************************************
Function   :viscircles
Syntax     :B=viscircles(A,centers,radii)
Author: Tess  Zacharias

********************************************************/

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

  int opencv_viscircles(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr = 0;
    int *piAddr1= NULL;
    int *piAddr2= NULL;
    int *piAddr3= NULL;
    int *piAddr4= NULL;
    char* pstData = NULL;
    char* pstData1 = NULL;
    Mat src_gray,image;
    CheckInputArgument(pvApiCtx, 3, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    double *Cx = NULL;
    double *Cy = NULL;
    double *radius=NULL;
    int iRows1	= 0;
    int iCols1	= 0;
    int iRows2	= 0;
    int iCols2	= 0;
    int iRet    = 0;
    int iRet1    = 0;
    double n;
    retrieveImage(image, 1);
    Mat drawing = Mat::zeros(image.size(), CV_8UC3 );
    sciErr =getVarAddressFromPosition(pvApiCtx,2,&piAddr1);
    if(sciErr.iErr)   
     {
        printError(&sciErr, 0);
        return 0;
     }
    sciErr =  getMatrixOfDoubleInList(pvApiCtx, piAddr1, 1, &iRows1, &iCols1, &Cx);
    if(sciErr.iErr)
     {
        printError(&sciErr, 0);
        return 0;
     }
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr1);
    if(sciErr.iErr)   
     {
        printError(&sciErr, 0);
        return 0;
     }   
    sciErr =  getMatrixOfDoubleInList(pvApiCtx, piAddr1, 2, &iRows1, &iCols1, &Cy);
    if(sciErr.iErr)
     {
        printError(&sciErr, 0);
        return 0;
     }
    sciErr =getVarAddressFromPosition(pvApiCtx,3,&piAddr2);
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows2, &iCols2,&radius);
    if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
    for( int i = 1; i < iCols1; i++ )
     {
        Point center(cvRound(Cx[i]), cvRound(Cy[i]));
        int r = cvRound(radius[i]);
        if(Rhs==3)
        {
        circle( drawing, center, r, Scalar(0,0,255), 1, 8, 0 );
        }
        else if(Rhs==5)
        {
           sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr3);
	   if(sciErr.iErr)
	   {
		printError(&sciErr, 0);
		return 0;
	   }
          if(isStringType(pvApiCtx, piAddr3))
	   {
            if(isScalar(pvApiCtx, piAddr3))
	     {
	       iRet = getAllocatedSingleString(pvApiCtx, piAddr3, &pstData);
             }
           }
         else
           {
             sciprint(" The Fourth argument should be string  ");
             return 0;
           }  
         if((strcasecmp(pstData,"LineWidth")!=0)&&(strcasecmp(pstData,"EdgeColor")!=0))
           {
              sciprint("\nIf Fourth argument must be a  string :'LineWidth' or 'EdgeColor'");
                return 0;
           }  
         if(strcasecmp(pstData,"EdgeColor")==0)
           {
                sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr4);
	        if(sciErr.iErr)
	        {
		  printError(&sciErr, 0);
		  return 0;
	        }
                if(isStringType(pvApiCtx, piAddr4))
	        {
                    if(isScalar(pvApiCtx, piAddr4))
	            {
	               iRet1 = getAllocatedSingleString(pvApiCtx, piAddr4, &pstData1);
                    }
                }
               else
                {
                  sciprint(" The Fifth argument should be string  ");
                  return 0;
                }
              if(strcasecmp(pstData1,"b")==0)
                {
                    circle( drawing, center, r, Scalar(0,255,255), 2, 8, 0 );
                }
              else if(strcasecmp(pstData1,"r")==0)
                {
                    circle( drawing, center, r, Scalar(255,255,0), 2, 8, 0 );
                }
              else if(strcasecmp(pstData1,"g")==0)
                {
                    circle( drawing, center, r, Scalar(255,0,255), 2, 8, 0 );
                }
              else if(strcasecmp(pstData1,"y")==0)
                {
                    circle( drawing, center, r, Scalar(255,0,0), 2, 8, 0 );
                }
               else if(strcasecmp(pstData1,"m")==0)
                {
                    circle( drawing, center, r, Scalar(0,255,0), 2, 8, 0 );
                }
               else if(strcasecmp(pstData1,"c")==0)
                {
                    circle( drawing, center, r, Scalar(0,0,255), 2, 8, 0 );
                }
              else if(strcasecmp(pstData1,"k")==0)
                {
                    circle( drawing, center, r, Scalar(255,255,255), 2, 8, 0 );
                }
              else if(strcasecmp(pstData1,"w")==0)
                {
                    circle( drawing, center, r, Scalar(0,0,0), 2, 8, 0 );
                }
              else
                {
                    sciprint("\nThe Fifth argument must be a  either of string value 'b','c','r','g''m','k','w','y'");
                    return 0;
                }
          }  
        if(strcasecmp(pstData,"LineWidth")==0)
          {
            sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr4);
            if (sciErr.iErr)
            {
               printError(&sciErr, 0);
               return 0;
            }
            intErr = getScalarDouble(pvApiCtx, piAddr4, &n);
            if(intErr)
            {
               return intErr;
            }   
            // The error checks for the function
            if(n != round(n) || n<=0)
            {
               sciprint("The value of n must be an integer\n");
               return 0;
            }
            circle( drawing, center, r, Scalar(255,255,0), n, 8, 0 );
          }
       }  
     }
    bitwise_not(drawing,drawing);
    string tempstring = type2str(drawing.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,drawing,1);
    free(checker); 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;        

  }
}
