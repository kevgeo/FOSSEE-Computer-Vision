/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : [x.center,y.center,r.radius]=minenclosingcircle(InputArray points)
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
  int minimumenclosingcirlce(char *fname, unsigned long fname_len)
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
        int* piAddrNew          = NULL;
	int* piLen		= NULL;
	double *pstData	        = NULL;
        char **pstData2    	= NULL;
        double *rrows;
        double *rcols;
        Point2f center;
        float radius;
        int error;         
         //checking input argument 
        CheckInputArgument(pvApiCtx,1,1);
         //checking output argument
        CheckOutputArgument(pvApiCtx,3,3);
         //for first argument 
         // get Address of first input  
        sciErr =getVarAddressFromPosition(pvApiCtx,1,&piAddr);
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
     
        minEnclosingCircle(contours,center,radius);
    ////////// Return the output arguments to the Scilab engine //////////
        double p1,p2,c; //another way of returning pointer
        p1=center.x;
        p2=center.y;
        c=radius;        
       error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+1,p1);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }  

       error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+2,p2);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }  
       error=createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx)+3,c);
         if(error!=0)
         {
           sciprint("error occurred");
           return 0;    
         }  
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
        AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
        ReturnArguments(pvApiCtx);    

        return 0;     
}
}

