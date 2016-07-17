#ifdef __cplusplus
extern "C" {
#endif
#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
#include <MALLOC.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc opencv_imread;
extern Gatefunc has_frame;
extern Gatefunc read_frame;
extern Gatefunc opencv_VideoReader;
extern Gatefunc opencv_KalmanFilter;
extern Gatefunc opencv_filterSpeckles;
extern Gatefunc opencv_Rodrigues;
extern Gatefunc opencv_reprojectImageTo3D;
extern Gatefunc opencv_RQDecomp3x3;
extern Gatefunc opencv_composeRT;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imread,"imread"},
  {(Myinterfun)sci_gateway_without_putlhsvar,has_frame,"hasFrame"},
  {(Myinterfun)sci_gateway_without_putlhsvar,read_frame,"readFrame"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_VideoReader,"VideoReader"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_KalmanFilter,"KalmanFilter"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_filterSpeckles,"filterSpeckles"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_Rodrigues,"Rodrigues"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_reprojectImageTo3D,"reprojectImageTo3D"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_RQDecomp3x3,"RQDecomp3x3"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_composeRT,"composeRT"},
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
