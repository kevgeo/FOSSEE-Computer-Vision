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
extern Gatefunc opencv_roipoly;
extern Gatefunc opencv_convertPointsToHomogeneous;
extern Gatefunc opencv_convertPointsFromHomogeneous;
extern Gatefunc opencv_Rodrigues;
extern Gatefunc opencv_projectPoints;
extern Gatefunc opencv_solvePnP;
extern Gatefunc opencv_solvePnPRansac;
extern Gatefunc opencv_findCirclesGrid;
extern Gatefunc opencv_stereoCalibrate;
extern Gatefunc opencv_calibrateCamera;
extern Gatefunc opencv_findChessboardCorners;
extern Gatefunc opencv_drawChessboardCorners;
extern Gatefunc opencv_filterSpeckles;
extern Gatefunc opencv_disparity;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imread,"imread"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_VideoReader,"VideoReader"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_roipoly,"opencv_roipoly"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_convertPointsToHomogeneous,"convertPointsToHomogeneous"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_convertPointsFromHomogeneous,"convertPointsFromHomogeneous"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_Rodrigues,"opencv_Rodrigues"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_projectPoints,"projectPoints"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_solvePnP,"solvePnP"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_solvePnPRansac,"solvePnPRansac"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_findCirclesGrid,"findCirclesGrid"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_stereoCalibrate,"stereoCalibrate"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_calibrateCamera,"calibrateCamera"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_findChessboardCorners,"findChessboardCorners"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_drawChessboardCorners,"drawChessboardCorners"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_filterSpeckles,"filterSpeckles"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_disparity,"disparity"},
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
