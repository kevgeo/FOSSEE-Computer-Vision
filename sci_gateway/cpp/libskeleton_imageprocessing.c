#ifdef __cplusplus
extern "C" {
#endif
#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
#include <MALLOC.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc opencv_imread;
extern Gatefunc opencv_VideoReader;
extern Gatefunc opencv_convolver;
extern Gatefunc opencv_projectPoints;
extern Gatefunc opencv_solvePnP;
extern Gatefunc opencv_solvePnPRansac;
extern Gatefunc opencv_stereoCalibrate;
extern Gatefunc opencv_findChessboardCorners;
extern Gatefunc opencv_drawChessboardCorners;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imread,"imread"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_VideoReader,"VideoReader"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_convolver,"convolver"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_projectPoints,"projectPoints"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_solvePnP,"solvePnP"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_solvePnPRansac,"solvePnPRansac"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_stereoCalibrate,"stereoCalibrate"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_findChessboardCorners,"findChessboardCorners"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_drawChessboardCorners,"drawChessboardCorners"},
};
 
int C2F(libskeleton_imageprocessing)()
{
  Rhs = Max(0, Rhs);
  if (*(Tab[Fin-1].f) != NULL) 
  {
     if(pvApiCtx == NULL)
     {
       pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
     }
     pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  }
  return 0;
}
#ifdef __cplusplus
}
#endif
