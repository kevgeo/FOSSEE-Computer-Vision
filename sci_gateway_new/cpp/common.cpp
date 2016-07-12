/***************************************************
Author : Sukul Bagai
***************************************************/

//takes the matrix type as parameter and returns a string of the type
string type2str(int type) 
{
    string r;
    uchar depth = type & CV_MAT_DEPTH_MASK;
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
    return r;
}

//takes matrix type as parameter and returns no. of channels of the image matrix
int no_of_channels(int type)
{
    uchar chans = 1 + (type >> CV_CN_SHIFT);
    return chans;
}

//function used to retrieve image matrix from Scilab environment
/*
    We need to retrieve image from the Scilab environment
    We do this by getting a List of 2-D matrices. The size of the list, in our case is known(either 3 or 1,
    3 for a coloured image and 1 for a grayscale image), hence we do not require the dynamic capability of a list.
    Incase of a 3-channel image, the first matrix will be a matrix with dimensions img.rows X img.cols, and will denote the R(red)
    values of the image. Similarly, the second will have the G(green) values, and the third will have B(blue) values.
    For a single-channeled image, we will have only a single matrix
*/
int retrieveImage(Mat &image,int pos)
{

    SciErr sciErr;
    int iRows=0,iCols=0,i,j,k=0;
    int *piAddr = NULL;
    int *piAddrChild = NULL;
    int iPrec = 0,iItem = 0;

    //retrieving number of items in the list and type of data(integer/float)
    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getListItemAddress(pvApiCtx,piAddr,1,&piAddrChild);
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

    if(isIntegerType(pvApiCtx, piAddrChild))
    {
        //getting precision
        sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddrChild, &iPrec);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        switch(iPrec)
        {
            case SCI_UINT8: //for unsigned integer 8
            {
                if(iItem==3)
                {
                    unsigned char *pstDataR = NULL;
                    unsigned char *pstDataG = NULL;
                    unsigned char *pstDataB = NULL; 
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive the matrix of the R values   
                    sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive address of the list
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }   
                    //retrive the matrix of the G values 
                    sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddr, 2, &iRows, &iCols, &pstDataG);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive address of the list
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }   
                    //retrive the matrix of the B values 
                    sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddr, 3, &iRows, &iCols, &pstDataB);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
                    image = Mat(iRows,iCols,CV_8UC3);

                    /*Now that we have the 3 matrices(R,G,B), we need to assign those values to the pixels. The following code does this*/
                    k=0;
                    for(i=0;i<iRows;i++)
                    {
                        for(j=0;j<iCols;j++)
                        {
                          image.at<Vec3b>(i,j)[2]=pstDataR[i+iRows*j];
                          image.at<Vec3b>(i,j)[1]=pstDataG[i+iRows*j];
                          image.at<Vec3b>(i,j)[0]=pstDataB[i+iRows*j];
                        }
                    }
                }
                else
                {
                    unsigned char *pstDataR = NULL;
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive the matrix of the R values   
                    sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    
                    //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
                    image = Mat(iRows,iCols,CV_8UC1);

                    //Assigning matrix value to pixel
                    k=0;
                    for(i=0;i<iRows;i++)
                        for(j=0;j<iCols;j++)
                          image.at<uchar>(i,j)=pstDataR[i+iRows*j];
                }
                break;
            }
            case SCI_UINT16: //for unsigned integer 16
            {
                if(iItem==3)
                {
                    short unsigned int *pstDataR = NULL;
                    short unsigned int *pstDataG = NULL;
                    short unsigned int *pstDataB = NULL;
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive the matrix of the R values   
                    sciErr = getMatrixOfUnsignedInteger16InList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive address of the list
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }   
                    //retrive the matrix of the G values 
                    sciErr = getMatrixOfUnsignedInteger16InList(pvApiCtx, piAddr, 2, &iRows, &iCols, &pstDataG);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive address of the list
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }   
                    //retrive the matrix of the B values 
                    sciErr = getMatrixOfUnsignedInteger16InList(pvApiCtx, piAddr, 3, &iRows, &iCols, &pstDataB);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
                    image = Mat(iRows,iCols,CV_16UC3);

                    //Now that we have the 3 matrices(R,G,B), we need to assign those values to the pixels. The following code does this
                    k=0;
                    for(i=0;i<iRows;i++)
                    {
                        for(j=0;j<iCols;j++)
                        {
                          image.at<Vec3s>(i,j)[2]=pstDataR[i+iRows*j];
                          image.at<Vec3s>(i,j)[1]=pstDataG[i+iRows*j];
                          image.at<Vec3s>(i,j)[0]=pstDataB[i+iRows*j];
                     //i+iRows*j
                        }
                    }
                }
                else
                {
                    short unsigned int *pstDataR = NULL;
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive the matrix of the R values   
                    sciErr = getMatrixOfUnsignedInteger16InList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    
                    //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
                    image = Mat(iRows,iCols,CV_16UC1);

                    //Assigning matrix values to pixels
                    k=0;
                    for(i=0;i<iRows;i++)
                        for(j=0;j<iCols;j++)
                          image.at<ushort>(i,j)=pstDataR[i+iRows*j];
                }
                break;
            }
            case SCI_INT16: //for signed integer 16
            {
                if(iItem==3)
                {
                    short int *pstDataR = NULL;
                    short int *pstDataG = NULL;
                    short int *pstDataB = NULL;
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive the matrix of the R values   
                    sciErr = getMatrixOfInteger16InList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive address of the list
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }   
                    //retrive the matrix of the G values 
                    sciErr = getMatrixOfInteger16InList(pvApiCtx, piAddr, 2, &iRows, &iCols, &pstDataG);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive address of the list
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }   
                    //retrive the matrix of the B values 
                    sciErr = getMatrixOfInteger16InList(pvApiCtx, piAddr, 3, &iRows, &iCols, &pstDataB);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
                    image = Mat(iRows,iCols,CV_16SC3);

                    //Now that we have the 3 matrices(R,G,B), we need to assign those values to the pixels. The following code does this
                    k=0;
                    for(i=0;i<iRows;i++)
                    {
                        for(j=0;j<iCols;j++)
                        {
                          image.at<Vec3s>(i,j)[2]=pstDataR[i+iRows*j];
                          image.at<Vec3s>(i,j)[1]=pstDataG[i+iRows*j];
                          image.at<Vec3s>(i,j)[0]=pstDataB[i+iRows*j];
                        }
                    }
                }
                else
                {
                    short int *pstDataR = NULL;
                    sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
                    if(sciErr.iErr)   
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    //retrive the matrix of the R values   
                    sciErr = getMatrixOfInteger16InList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    
                    //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
                    image = Mat(iRows,iCols,CV_16SC1);

                    //Assigning image matrix values to pixels
                    k=0;
                    for(i=0;i<iRows;i++)
                        for(j=0;j<iCols;j++)
                          image.at<short>(i,j)=pstDataR[i+iRows*j];
                }
                break;
            }
        }
    }
    else //for floating point/ double precision values
    {
        if(iItem==3)
        {
            double *pstDataR = NULL;
            double *pstDataG = NULL;
            double *pstDataB = NULL;
            sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
            if(sciErr.iErr)   
            {
                printError(&sciErr, 0);
                return 0;
            }
            //retrive the matrix of the R values   
            sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            //retrive address of the list
            sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddrChild);
            if(sciErr.iErr)   
            {
                printError(&sciErr, 0);
                return 0;
            }               
            //retrive the matrix of the G values 
            sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 2, &iRows, &iCols, &pstDataG);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            //retrive address of the list
            sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddrChild);
            if(sciErr.iErr)   
            {
                printError(&sciErr, 0);
                return 0;
            }   
            //retrive the matrix of the B values 
            sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 3, &iRows, &iCols, &pstDataB);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
            image = Mat(iRows,iCols,CV_64FC3);

            //Now that we have the 3 matrices(R,G,B), we need to assign those values to the pixels. The following code does this
            k=0;
            for(i=0;i<iRows;i++)
            {
                for(j=0;j<iCols;j++)
                {
                    image.at<Vec3d>(i,j)[2]=pstDataR[i+iRows*j];
                    image.at<Vec3d>(i,j)[1]=pstDataG[i+iRows*j];
                    image.at<Vec3d>(i,j)[0]=pstDataB[i+iRows*j];
                }
            }
        }
        else
        {
            double *pstDataR = NULL;
            sciErr = getVarAddressFromPosition(pvApiCtx,pos,&piAddr);
            if(sciErr.iErr)   
            {
                printError(&sciErr, 0);
                return 0;
            }
            //retrive the matrix of the R values   
            sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
            image = Mat(iRows,iCols,CV_64FC1);

            //Assigning image matrix values to pixels
            k=0;
            for(i=0;i<iRows;i++)
                for(j=0;j<iCols;j++)
                    image.at<double>(i,j)=pstDataR[i+iRows*j];
        }
    }
    return 1;
}

//function that returns an image matrix to the Scilab environment
/*
    What we now need, is to pass the image matrix to the Scilab environment.
    We do this by passing a List of 2-D matrices. The size of the list, in our case is known(either 3 or 1,
    3 for a coloured image and 1 for a grayscale image), hence we do not require the dynamic capability of a list.
    Incase of a 3-channel image, the first matrix will be a matrix with dimensions img.rows X img.cols, and will denote the R(red)
    values of the image. Similarly, the second will have the G(green) values, and the third will have B(blue) values.
    For a single-channeled image, we will have only a single matrix
*/
int returnImage(char *checker,Mat img,int pos)
{
    int i,j,k=0;
    int *piAddrNew  = NULL;
    SciErr sciErr;
    int num=no_of_channels(img.type()); //num now contains no. of channels of the image
    //creating the list that will be passed to the Scilab enviroment
    if(num==3)
        sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + pos, 3, &piAddrNew);
    else
        sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + pos, 1, &piAddrNew);
    if(sciErr.iErr)
    {
       printError(&sciErr, 0);           
       return 0;
    }
    if(strcmp(checker,"8U")==0) //for Unsigned Integer 8
    {
        if(num==3)
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            unsigned char *r = (unsigned char *)malloc(img.rows * img.cols * sizeof(unsigned char));
            unsigned char *g = (unsigned char *)malloc(img.rows * img.cols * sizeof(unsigned char));
            unsigned char *b = (unsigned char *)malloc(img.rows * img.cols * sizeof(unsigned char));

            //The next block of code retrieves the RGB values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
            {
                for(j=0;j<img.cols;j++)
                {
                    Vec3b intensity = img.at<Vec3b>(i, j);
                    r [i+img.rows*j]=intensity.val[2];
                    g [i+img.rows*j]=intensity.val[1];
                    b [i+img.rows*j]=intensity.val[0];
                }
            }


            //Adding the R value matrix to the list
            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols, r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the G value matrix to the list
            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 2, img.rows,img.cols,g);
            free(g);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the B value matrix to the list
            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 3, img.rows,img.cols, b);
            free(b);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        else
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            unsigned char *r = (unsigned char *)malloc(img.rows * img.cols * sizeof(unsigned char));
            
            //The next block of code retrieves the image colour values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
                for(j=0;j<img.cols;j++)
                    r[i+img.rows*j]=img.at<uchar>(i, j);


            //Adding the image colour value matrix to the list
            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1,img.rows,img.cols, r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

        }
    }
    else if(strcmp(checker,"16U")==0) //for Unsigned Integer 16
    {
        if(num==3)
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            short unsigned int *r = (short unsigned int *)malloc(img.rows * img.cols * sizeof(short unsigned int));
            short unsigned int *g = (short unsigned int *)malloc(img.rows * img.cols * sizeof(short unsigned int));
            short unsigned int *b = (short unsigned int *)malloc(img.rows * img.cols * sizeof(short unsigned int));

            //The next block of code retrieves the RGB values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
            {
                for(j=0;j<img.cols;j++)
                {
                    Vec3s intensity = img.at<Vec3s>(i, j);
                    r[i+img.rows*j]=intensity.val[2];
                    g[i+img.rows*j]=intensity.val[1];
                    b[i+img.rows*j]=intensity.val[0];
                }
            }

            //Adding the R value matrix to the list
            sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols,r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the G value matrix to the list
            sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 2, img.rows,img.cols,g);
            free(g);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the B value matrix to the list
            sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 3, img.rows,img.cols, b);
            free(b);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        else
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            short unsigned int *r = (short unsigned int *)malloc(img.rows * img.cols * sizeof(short unsigned int));
            
            //The next block of code retrieves the image colour values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
                for(j=0;j<img.cols;j++)
                    r[i+img.rows*j]=img.at<ushort>(i, j);

            //Adding the image colour value matrix to the list
            sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols,r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }

    }
    else if(strcmp(checker,"16S")==0) //for Signed Integer 16
    {
        if(num==3)
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            short int *r = (short int *)malloc(img.rows * img.cols * sizeof(short int));
            short int *g = (short int *)malloc(img.rows * img.cols * sizeof(short int));
            short int *b = (short int *)malloc(img.rows * img.cols * sizeof(short int));
            //The next block of code retrieves the RGB values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
            {
                for(j=0;j<img.cols;j++)
                {
                    Vec3s intensity = img.at<Vec3s>(i, j);
                    r[i+img.rows*j]=intensity.val[2];
                    g[i+img.rows*j]=intensity.val[1];
                    b[i+img.rows*j]=intensity.val[0];
                }
            }

            //Adding the R value matrix to the list
            sciErr = createMatrixOfInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols, r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the G value matrix to the list
            sciErr = createMatrixOfInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 2, img.rows,img.cols, g);
            free(g);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the B value matrix to the list
            sciErr = createMatrixOfInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 3, img.rows,img.cols,b);
            free(b);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }

        else
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            short int *r = (short int *)malloc(img.rows * img.cols * sizeof(short int));
            
            //The next block of code retrieves the image colour values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
                for(j=0;j<img.cols;j++)
                    r[i+img.rows*j]=img.at<short>(i, j);

            //Adding the image colour value matrix to the list
            sciErr = createMatrixOfInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols,r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

        }
    }
    else if(strcmp(checker,"32S")==0) //for Signed Integer 16
    {
        if(num==3)
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            int *r = (int *)malloc(img.rows * img.cols * sizeof(int));
            int *g = (int *)malloc(img.rows * img.cols * sizeof(int));
            int *b = (int *)malloc(img.rows * img.cols * sizeof(int));
            //The next block of code retrieves the RGB values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
            {
                for(j=0;j<img.cols;j++)
                {
                    Vec3i intensity = img.at<Vec3i>(i, j);
                    r[i+img.rows*j]=intensity.val[2];
                    g[i+img.rows*j]=intensity.val[1];
                    b[i+img.rows*j]=intensity.val[0];
                }
            }

            //Adding the R value matrix to the list
            sciErr = createMatrixOfInteger32InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols,r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the G value matrix to the list
            sciErr = createMatrixOfInteger32InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 2, img.rows,img.cols,g);
            free(g);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the B value matrix to the list
            sciErr = createMatrixOfInteger32InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 3, img.rows,img.cols,b);
            free(b);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        else
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            int *r = (int *)malloc(img.rows * img.cols * sizeof(int));
            
            //The next block of code retrieves the image colour values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
                for(j=0;j<img.cols;j++)
                    r[i+img.rows*j]=img.at<int>(i, j);

            //Adding the image colour value matrix to the list
            sciErr = createMatrixOfInteger32InList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1,img.rows,img.cols, r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

        }
    }

    else if(strcmp(checker,"32F")==0) //for Float 32
    {
        if(num==3)
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            double *r = (double *)malloc(img.rows * img.cols * sizeof(double));
            double *g = (double *)malloc(img.rows * img.cols * sizeof(double));
            double *b = (double *)malloc(img.rows * img.cols * sizeof(double));
            //The next block of code retrieves the RGB values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
            {
                for(j=0;j<img.cols;j++)
                {
                    Vec3d intensity = img.at<Vec3d>(i, j);
                    r[i+img.rows*j]=intensity.val[2];
                    g[i+img.rows*j]=intensity.val[1];
                    b[i+img.rows*j]=intensity.val[0];
                }
            }

            //Adding the R value matrix to the list
            sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols,r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the G value matrix to the list
            sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 2, img.rows,img.cols,g);
            free(g);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the B value matrix to the list
            sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 3, img.rows,img.cols,b);
            free(b);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        else
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            double *r = (double *)malloc(img.rows * img.cols * sizeof(double));
            //The next block of code retrieves the image colour values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
                for(j=0;j<img.cols;j++)
                    r[i+img.rows*j]=img.at<float>(i, j);

            //Adding the image colour value matrix to the list
            sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1,img.rows,img.cols,r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
    }
    else if(strcmp(checker,"64F")==0) //For Float 64
    {
        if(num==3)
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            double *r = (double *)malloc(img.rows * img.cols * sizeof(double));
            double *g = (double *)malloc(img.rows * img.cols * sizeof(double));
            double *b = (double *)malloc(img.rows * img.cols * sizeof(double));
            //The next block of code retrieves the RGB values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
            {
                for(j=0;j<img.cols;j++)
                {
                    Vec3d intensity = img.at<Vec3d>(i, j);
                    r[i+img.rows*j]=intensity.val[2];
                    g[i+img.rows*j]=intensity.val[1];
                    b[i+img.rows*j]=intensity.val[0];
                }
            }

            //Adding the R value matrix to the list
            sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols,r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the G value matrix to the list
            sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 2, img.rows,img.cols,g);
            free(g);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            //Adding the B value matrix to the list
            sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 3, img.rows,img.cols, b);
            free(b);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        else
        {
            //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
            double *r = (double *)malloc(img.rows * img.cols * sizeof(double));

            //The next block of code retrieves the image colour values at a specified pixel, and assigns it to the matrices
            for(i=0;i<img.rows;i++)
                for(j=0;j<img.cols;j++)
                    r[i+img.rows*j]=img.at<double>(i, j);

            //Adding the image colour value matrix to the list
            sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+pos , piAddrNew, 1, img.rows,img.cols, r);
            free(r);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
    }
    
}
