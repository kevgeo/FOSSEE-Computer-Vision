/* ==================================================================== */
/* Author :Priyanka Hiranandani NIT Surat                               */
/* ==================================================================== */
/* Syntax : return_image==boxfilter(InputArray src, int ddepth,size.x,size,y,point.x,point.y, bool normalize)*/
/* ==================================================================== */
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
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
  int boxfilter(char *fname, unsigned long fname_len)
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
        int* piAddr7            = NULL;
        int* piAddrNew            = NULL;
	int* piLen		= NULL;
	double* pstData	        = NULL;
        char** pstData2    	= NULL;
        double *rrows;
        double *rcols;
        int noofitem;
        double ddepth;
        bool normalize;
        int borderType,i;
        int error;
        double sz1,sz2;      
        double pi1,pi2;
  // checking input argument 
        CheckInputArgument(pvApiCtx,7,7);
  //checking output argument
        CheckOutputArgument(pvApiCtx, 1, 1);
  //for first argument 
        Mat img;
        retrieveImage(img,1);
    //second argument
             sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
             if (sciErr.iErr)
             {
              printError(&sciErr, 0);
              return 0;
             }
      //this function will fetch value of second argument
             error=getScalarDouble(pvApiCtx,piAddr2,&ddepth); 
             if(error!=0)
             {
                sciprint("error in retrieving second argument");
                return 0;
             } 
  //third argument
             sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
             if (sciErr.iErr)
             {
               printError(&sciErr, 0);
               return 0;
             }
  //this function will fetch value of third argument
             error=getScalarDouble(pvApiCtx,piAddr3,&sz1); 
             if(error!=0)
             {
                sciprint("error in retrieving second argument");
                return 0;
             } 
            
 //fourth argument
             sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
             if (sciErr.iErr)
             {
                printError(&sciErr, 0);
                return 0;
             }
  //this function will fetch value of fourth argument
             error=getScalarDouble(pvApiCtx,piAddr4,&sz2); 
             if(error!=0)
             {
                sciprint("error in retrieving second argument");
             }      
 //fifth argument
             sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
             if (sciErr.iErr)
             {
                printError(&sciErr, 0);
                return 0;
             }
  //this function will fetch value of fifth argument
             error=getScalarDouble(pvApiCtx,piAddr5,&pi1); 
             if(error!=0)
             {
                sciprint("error in retrieving second argument");
             } 
    //sixth argument
             sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
             if (sciErr.iErr)
             {
                  printError(&sciErr, 0);
                  return 0;
             }
   //this function will fetch value of sixth argument
             error=getScalarDouble(pvApiCtx,piAddr6,&pi2); 
             if(error!=0)
             {
                sciprint("error in retrieving second argument");
             } 
   //seventh argument
             sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
             if (sciErr.iErr)
             {
                printError(&sciErr, 0);
                return 0;
             }
     //this function will fetch value of seventh argument
             sciErr = getMatrixOfString(pvApiCtx, piAddr7, &iRows, &iCols, NULL, NULL);
             if(sciErr.iErr)
             {
                 printError(&sciErr, 0);
                 return 0;
             }
             piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
             sciErr = getMatrixOfString(pvApiCtx, piAddr7, &iRows, &iCols, piLen, NULL);
             if(sciErr.iErr)
             {
                printError(&sciErr, 0);
                return 0;
             }

             pstData2 = (char**)malloc(sizeof(char*) * iRows * iCols);
             for(i = 0 ; i < iRows * iCols ; i++)
             {
               pstData2[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
             }
    //third call to retrieve data
             sciErr = getMatrixOfString(pvApiCtx, piAddr7, &iRows, &iCols, piLen, pstData2);
             if(sciErr.iErr)
             {
                 printError(&sciErr, 0);
                 return 0;
             }
             if(!strcmp(pstData2[0],"True"))
             {
                   normalize=true;
             }
             else if(!strcmp(pstData2[0],"False"))
             {
                   normalize=true;

             }
            //size constructor
             Size ksize=Size(sz1,sz2);
           //anchor constructor
             Point anchor=Point(pi1,pi2);
             Mat dst;
          //call to open cv boxfilter
             boxFilter(img,dst,ddepth,ksize,anchor,normalize,BORDER_DEFAULT);
          //returning variable to scilab console
             string tempstring = type2str(dst.type());
             char* checker = (char *)malloc(tempstring.size() + 1);
             memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
             returnImage(checker,dst,1);
             //returning output variable    
             AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
             ReturnArguments(pvApiCtx); 
             return 0;
             

   }
}
