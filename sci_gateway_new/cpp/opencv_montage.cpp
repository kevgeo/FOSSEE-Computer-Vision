/***************************************************
Author : Sukul Bagai
***************************************************/

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

int opencv_montage(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iLen        = 0;
    //variable info
    int iRows       = 0;
    int iCols       = 0;
    int *piAddr     = NULL;
    int *piAddr2     = NULL;
    int *piAddr3     = NULL;
    int *piAddrChild = NULL;
    int *piAddrNew  = NULL;
    int piRows      = 0;
    int piCols      = 0;        
    int *piLen      = NULL;
    char **pstData  = NULL;
    int **pstData1  = NULL;
    int i,j,k=0,iItem=0,height,width,intErr=0;
    double new_rows,new_cols;
    unsigned char *pstDataR = NULL;
    unsigned char *pstDataG = NULL;
    unsigned char *pstDataB = NULL;
    vector <Mat> image;
    Mat tempimage;


    //Checking number of input and output arguments (enviromnet variable, min arguments, max arguments)
    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    

    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getListItemNumber(pvApiCtx,piAddr,&iItem);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    for(int x=0;x<iItem;x++)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        if(sciErr.iErr)   
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getListItemAddress(pvApiCtx,piAddr,x+1,&piAddrChild);
        if(sciErr.iErr)   
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddrChild, 1, &iRows, &iCols, &pstDataR);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        if(sciErr.iErr)   
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getListItemAddress(pvApiCtx,piAddr,x+1,&piAddrChild);
        if(sciErr.iErr)   
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddrChild, 2, &iRows, &iCols, &pstDataG);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        if(sciErr.iErr)   
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getListItemAddress(pvApiCtx,piAddr,x+1,&piAddrChild);
        if(sciErr.iErr)   
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddrChild, 3, &iRows, &iCols, &pstDataB);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        tempimage = Mat(iRows,iCols,CV_8UC3);
        k=0;
        for(i=0;i<iRows;i++)
        {
            for(j=0;j<iCols;j++)
            {
              tempimage.at<Vec3b>(i,j)[2]=pstDataR[k];
              tempimage.at<Vec3b>(i,j)[1]=pstDataG[k];
              tempimage.at<Vec3b>(i,j)[0]=pstDataB[k++];
            }
        }
        image.push_back(tempimage);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &new_rows);
    if(intErr)
        return intErr;

    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &new_cols);
    if(intErr)
        return intErr;

    for(i=1;i<iItem;i++)
    {
        if((image[i].rows != image[i-1].rows) || (image[i].cols != image[i-1].cols))
        {
            sciprint("Images not of same size. Montage did not take place\n");
        }
    }
    if( ((new_rows-1)*new_cols >= iItem) || ((new_rows*new_cols) < iItem) )
    {
        sciprint("Given Row and Column Dimensions do not match with number of images in list. Montage cannot be created. Please check arguments\n");
        return 0;
    }
    height=new_rows*image[0].rows;
    width=new_cols*image[0].cols;

    Mat new_image = Mat::zeros(height,width,CV_8UC3);

    k=0;
    for(int x=0;x<new_rows;x++)
    {
        for(int y=0;y<new_cols;y++)
        {/*
            if((k)>=iItem)
            {
                new_image.at<Vec3b>(image[0].rows*x+i,image[0].cols*y+j)[2]=black_image.at<Vec3b>(i,j)[2];
                new_image.at<Vec3b>(image[0].rows*x+i,image[0].cols*y+j)[1]=black_image.at<Vec3b>(i,j)[1];
                new_image.at<Vec3b>(image[0].rows*x+i,image[0].cols*y+j)[0]=black_image.at<Vec3b>(i,j)[0];     
            }*/
            if(k<iItem)
            {
                for(i=0;i<image[x+y].rows;i++)
                {
                    for(j=0;j<image[x+y].cols;j++)
                    {
                        new_image.at<Vec3b>(image[k].rows*x+i,image[k].cols*y+j)[2]=image[k].at<Vec3b>(i,j)[2];
                        new_image.at<Vec3b>(image[k].rows*x+i,image[k].cols*y+j)[1]=image[k].at<Vec3b>(i,j)[1];
                        new_image.at<Vec3b>(image[k].rows*x+i,image[k].cols*y+j)[0]=image[k].at<Vec3b>(i,j)[0];
                    }
                }
            }
            k++;
        }
    }

    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_image,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);

    return 0;
}
/* ==================================================================== */
}
