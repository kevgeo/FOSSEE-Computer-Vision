/********************************************************
    Author: V Srinivas
*********************************************************
   imwarp(image,matrix(2*3))=output.
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
  #include <sciprint.h>
  #include "../common.h"
  
  int opencv_imwarp(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr1 = NULL;
    //int *piAddr2 = NULL;
    double *tform;
    int maxx,maxy,minx,miny;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);//two arguments 1 takes the image and the other takes the transformation matrix
    CheckOutputArgument(pvApiCtx, 1, 1);
    //retieve Image
    Mat image;
    retrieveImage(image,1);
    //retreive the transformation matrix 
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows, &iCols,&tform);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }  
    if(iRows!=2 || iCols!=3)//checking if the transformation matrix is of the desired size
    {
        sciprint("Please enter a valid transformation matrix");
        return -1;
    }
    Mat dst;
    //This and the next few lines are dedicated to finding the size of the frame in which the whole transformed image can be viewed.
    double x [4]={0,0,image.cols-1,image.cols-1};
    double y [4]={0,image.rows-1,0,image.rows-1};
    int modx[4];
    int mody[4];
    for(int i=0;i<4;i++)// the modifications performed by the matrix is performed on the edges of the image to find the optimum size. This works as the transformation is linear so the edges always are on the extreme points. 
    {
        modx[i]=(int)(tform[0] * x[i] + tform[1] * y[i] + tform[4]);
        mody[i]=(int)(tform[2] * x[i] + tform[3] * y[i] + tform[5]);
        //cout<<*(modx+i)<<" "<<*(mody+i)<<endl;
    }
    Mat trans=(Mat_<double>(2,3)<<tform[0],tform[1],tform[4],tform[2],tform[3],tform[5]);//creating a matrix of type Mat.
    minx=modx[0];maxx=modx[0];miny=mody[0];maxy=mody[0];
    for(int i=0;i<4;i++)//used to find the maximum and minimum of image coordinates.
    {
        if(modx[i]<minx)
        {minx=modx[i];}
        if(modx[i]>maxx)
        {maxx=modx[i];}
        if(mody[i]<miny)
        {miny=mody[i];}
        if(mody[i]>maxy)
        {maxy=mody[i];}
    }
    warpAffine(image,dst,trans,Size(maxx-minx,maxy-miny));
    string tempstring = type2str(dst.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,dst,1); 
    free(checker);




    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;
    


 }

}
