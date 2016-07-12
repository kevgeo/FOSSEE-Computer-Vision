/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : [a,b,c,e,f]=fitellipse((InputArray points); 
a,b,c,d,e are parameters of rotated rect returned
/* ==================================================================== */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <sciprint.h>
using namespace cv;
using namespace std;
  extern "C"
  {
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "../common.h"
  int fitellipse(char *fname, unsigned long fname_len)
    {
    // Error management variable
        SciErr sciErr;
    //variable info
	int iRows		= 0;
	int iCols		= 0;
        int piRows		= 0;
	int piCols		= 0;
	int* piAddr		= NULL;
        int* piAddr2            = NULL;
        int* piAddr3            = NULL;
        int* piAddr4            = NULL;
        int* piAddr5            = NULL;
        int* piAddr6            = NULL;
        int* piAddrNew            = NULL;
	int* piLen		= NULL;
	double* pstData	        = NULL;
        int* pstData2    	= NULL;
        double *rrows;
        double *rcols;
        double sigmaSpace;
        int error;
         ////checking input argument 
        CheckInputArgument(pvApiCtx,1,1);
         //checking output argument
        CheckOutputArgument(pvApiCtx,5,5);
         //for first argument 
         // get Address of first input  
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
         //check for any error
             if(sciErr.iErr)   
               {
               printError(&sciErr, 0);
               return 0;
               }      
           //retrieve input array
          //SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols,&pstData);
             if(sciErr.iErr)
               {
               printError(&sciErr, 0);
               return 0;
               }
        int k=0;
        vector<Point> contours;
        for(int i=0;i<iCols;i++)
         {
                  
               contours.push_back(Point2f(pstData[i],pstData[i+1])) ;   
               i++;
         }
       RotatedRect rect;
       rect=fitEllipse(contours);
       double size1,size2,center1,center2,angle;
       //from rotated rect datatype we can fetch required size,point,angle
       Size2f sz;      //size2f function for floating point values 
       Point2f cen;
       sz=rect.size;
       cen=rect.center;
       angle=rect.angle;
       size1=sz.width;
       size2=sz.height;
       center1=cen.x;
       center2=cen.y;
             
        error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,size1);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }  
        error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+2,size2);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }  
        error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+3,center1);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }   
        error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+4,center2);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }   
        error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+5,angle);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }     
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
        AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
        AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;
        AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;
        ReturnArguments(pvApiCtx);      
}
}
