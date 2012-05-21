/****************************************************************************/
/*  FILE  :  snowinput.c                                                    */
/*    subsurface model                                                      */
/*    entire file by Carleen Tijm-Reijmer 3/2005                            */
/* update RH, 28 April, 2010*/
/****************************************************************************/
/*===== FUNCTIONS TO INITIALIZE THE SNOW MODEL ===*/
/* first functions to choose right glacier, then default and specific glacier functions */
/* specific functions are:*/
/* glacier_layertemperature    defines snow/firn/ice temperature for each layer */
/* glacier_snowlayerdensity    defines density for each layer in SNOW layer on top */
/*                             of ice/firn */
/* glacier_firnlayerdensity    defines density for each layer in FIRN layer, ice is constant set*/
/*                             in variab.h (densice) */
/* glacier_snowlayermsnow      converts SNOW (cm we) into m of snow with a location dependend */
/*                             density profile, same as used in glacier_snowlayerdensity */  
/**/

#include "function.h"
#include "variabex.h"      /* all global VARIABLES */

/****************************************************************************/
/* FUNCTION  snowlayermsnow                                                 */
/*   this function calculates snow layer thickness in m of snow             */
/*  NOT initialisation, but for functions changegrid and stationoutput*/
/****************************************************************************/

void snowlayermsnow()
 {

/*calculate snow depth in m snow*/
  snowlayer[i][j] = 0.;
  if (surface[i][j] != nodis)
  { if (methodsurftempglac == 4)		/*snow model*/
      snowlayer[i][j] = layerdepth[i][j][k-1] + 0.5*layerthickness[i][j][k-1];
    else
      snowlayer[i][j] = (SNOW[i][j]*10)/densfirn;
  }

 return;
 }

/****************************************************************************/
/* FUNCTION  meltlayermice                                                  */
/*   this function calculates thickness of melted layer in m ice        */
/*  NOT initialisation, but for functions changegrid and stationoutput*/
/****************************************************************************/
 
void meltlayermice()
 { 

/*calculate ice melt in m ice*/
  if (surface[i][j] != nodis)
  { if (methodsurftempglac == 4)
    { if (SNOW[i][j] == 0.)
      { if (layerid[i][j][1] == 2)
        { meltlayer[i][j] -= -(snowprec/denssnow) + ABLA[i][j]/layerdensity[i][j][1];
          snowlayer[i][j] = meltlayer[i][j];}
        if (layerid[i][j][1] == 3)
        { meltlayer[i][j] -= -(snowprec/denssnow) + ABLA[i][j]/densice;
          snowlayer[i][j] = meltlayer[i][j];}
      }
      if ((SNOW[i][j] != 0.) && (meltlayer[i][j] != 0))
      { if (layerid[i][j][1] < 3) meltlayer[i][j] -= -(snowprec/denssnow) + ABLA[i][j]/layerdensity[i][j][1]; 
        if (layerid[i][j][1] == 3) meltlayer[i][j] -= -(snowprec/denssnow) + ABLA[i][j]/densice;
        snowlayer[i][j] = meltlayer[i][j];
      }
      
    }
    else
    { if (SNOW[i][j] == 0.)
      { if (FIRN[i][j] == 0) meltlayer[i][j] -= -(snowprec/denssnow) + ABLA[i][j]/densice;
        if (FIRN[i][j] > 0) meltlayer[i][j] -= -(snowprec/denssnow) + ABLA[i][j]/densfirn;
        snowlayer[i][j] = meltlayer[i][j]; 
      }
      if ((SNOW[i][j] != 0.) && (meltlayer[i][j] != 0))
      { if (FIRN[i][j] == 0) meltlayer[i][j] -= -(snowprec/denssnow) + ABLA[i][j]/densice;
        if (FIRN[i][j] > 0) meltlayer[i][j] -= -(snowprec/denssnow) + ABLA[i][j]/densfirn;
        snowlayer[i][j] = meltlayer[i][j]; 
      }
    }
  }

 return;
 }

/******************************************************************/
/* FUNCTION  choice_layertemperature                                           */
/*    called from initgrid in snowmodel.c     */
/*    choice to be set in variab.h */
/******************************************************************/

void choice_layertemperature()
{
   int inti;
  
  inti=(int)init_layertemperature;
  switch(inti)

   {  case 1: default_layertemp3grad();       /*default: 3 temp gradients*/
              break;
      case 3: stor_layertemperature();       /*Storglaciaren spring 1999*/
              break;
      default : printf("\n\n function choice_layertemperature: ERROR in variab.h: variable choice_layertemperature, must be 1 or 3\n\n");
		      exit(10);
   }

 return;
}

/******************************************************************/
/* FUNCTION  choice_snowlayerdensity                                           */
/*    called from initgrid in snowmodel.c     */
/*    choice to be set in variab.h */
/******************************************************************/

void choice_snowlayerdensity()
{
   int inti;
  
  inti=(int)init_snowlayerdensity;
  switch(inti)

   {  case 1: default_snowlayerdensity();       /*default constant value*/
              break;
      case 2: default_snowlayerdensity3grad();       /*default 3 linear gradients*/
              break;
      case 3: stor_snowlayerdensity();       /*Storglaciaren spring 1999*/
              break;
      default : printf("\n\n function choice_snowlayerdensity: ERROR in variab.h: variable choice_snowlayerdensity, must be 1, 2 or 3\n\n");
		      exit(10);
   }

 return;
}

/******************************************************************/
/* FUNCTION  choice_firnlayerdensity                              */
/*    called from initgrid in snowmodel.c                         */
/*    choice to be set in variab.h */
/******************************************************************/

void choice_firnlayerdensity()
{
   int inti;
  
  inti=(int)init_firnlayerdensity;

  switch(inti)
   {  case 1: default_firnlayerdensity();       /*default constant value*/
              break;
      case 2: default_firnlayerdensity1grad();       /*default 1 linear gradient*/
              break;
      case 3: stor_firnlayerdensity();       /*Storglaciaren spring 1999*/
              break;
      default : printf("\n\n function choice_firnlayerdensity: ERROR in variab.h: variable choice_firnlayerdensity, must be 1, 2 or 3\n\n");
		      exit(10);
   }

 return;
}

/******************************************************************/
/* FUNCTION  choice_snowlayermsnow                                */
/*    called from initgrid in snowmodel.c                         */
/*    choice to be set in variab.h */
/*   options MUST correspond with options in choice_snowlayerdensity */
/******************************************************************/

void choice_snowlayermsnow()
{
   int inti;
  
  inti=(int)init_snowlayerdensity;
/*  inti=(int)init_snowlayermsnow;*/
  switch(inti)

   {  case 1: default_snowlayermsnow();        /*default based on constant snow density value*/
              break;
      case 2: default_snowlayermsnow3grad();   /*default using 3 linear gradients*/
              break;          
      case 3: stor_snowlayermsnow();       /*Storglaciaren spring 1999*/
              break;
   }

 return;
}

/*******************************************************************************/
/* FUNCTION  defualt_layertemperature_3grad                                    */
/*   this function calculates a layer temperature                              */
/*   based on 3 temp gradients (1: between surface and first temp turning point*/
/*  2: between first and second turning point, 3: betwenn second turning point */
/*  and the depth at which the glacier is temperate                            */
/*  the depths at which the temperature gradient changes should be determined  */
/*  from data at simulation start or educated guesses                          */
/*******************************************************************************/
 
void default_layertemp3grad()
 {
     /*temperatures at the turning points, 3 gradients are defined by 4 temperatures at 4 depths
       first temp is at surface, 4th at the depth where the ice is temperate*/
 float  tempsurface;    /*surface temp interpolated to grid cell*/
 float  tempturnpoint1;    /*temp at turning point interpolated to grid cell*/
 float  tgrad1,tgrad2,tgrad3;      /* vertical temperature gradients*/

/*surface temperauture and the temp of the first turning point are distributed acrosse the glacier
  using fixed lapse rates; temp of 2. turning point is assumed spatially constant*/
   tempsurface = tempsurfaceAWS + (griddgm[i][j]-griddgm[rowclim][colclim])/100 * templapserate1;
   tempturnpoint1 = tempturnpoint1AWS + (griddgm[i][j]-griddgm[rowclim][colclim])/100 * templapserate2;

/*compute 3 temperature gradients between surface and depth where ice is temperature*/
   tgrad1 = (tempturnpoint1 - tempsurface)/(depthtempturnpoint1 - 0.0); /*grad first part below surface*/
   tgrad2 = (tempturnpoint2AWS - tempturnpoint1)/(depthtempturnpoint2 - depthtempturnpoint1); /*second part, second gradient*/
   tgrad3 = (0 - tempturnpoint2AWS)/(depthtemperate - depthtempturnpoint2); /*lowest part, third gradient*/
 
/*assign temperature to each layer according to these gradients*/
  if (k == 1)
    layertemperature[i][j][k] = tempsurface;
  else 
  { if (layerdepth[i][j][k] < depthtempturnpoint1 )
      layertemperature[i][j][k] = layertemperature[i][j][k-1] + 
                                  (0.5*layerthickness[i][j][k]+0.5*layerthickness[i][j][k-1])*tgrad1;
    if ((layerdepth[i][j][k] >= depthtempturnpoint1) & (layerdepth[i][j][k] < depthtempturnpoint2))
      layertemperature[i][j][k] = layertemperature[i][j][k-1] + 
                                  (0.5*layerthickness[i][j][k]+0.5*layerthickness[i][j][k-1])*tgrad2;
    if (layerdepth[i][j][k] >= depthtempturnpoint2 )
      layertemperature[i][j][k] = layertemperature[i][j][k-1] + 
                                  (0.5*layerthickness[i][j][k]+0.5*layerthickness[i][j][k-1])*tgrad3;
  }
  if (layertemperature[i][j][k] > 0.0) 
      layertemperature[i][j][k] = 0;
  
 return;
 }
 

/****************************************************************************/
/* FUNCTION  default_layertemperature                                       */
/*   this function calculates a default layer temperature                   */
/*   based on annual averaged temperature and assumption of                 */
/*   temperate below 30 m, and spring start                                 */
/* FUNCTION NOT CALLED ANYWHERE BECAUSE DEFAULT REPLACED BY OTHER FUNCTION*/
/****************************************************************************/
 
 void default_layertemperature()
 {
  float  tannualclim=-9;   /*SG= -3,  estimated annual average temperature climate station*/
  float  tsurfclim=-6.0;     /*SG= -6   estimated start surface temperature*/
  float  tgrad1,tgrad2;      /* vertical temperature gradient top part till climate value tannual*/
  float  tannual,tsurfstart;
  float  ztemperate=30.;		/*SG=30  depth were ice becomes temperate*/
  float  zannual = 10.;		/*SG=10  depth of given tannualclim, usually 10 m temperature*/

/* First attempt: still here to make testing consistent*/
  tannual = tannualclim + (griddgm[i][j]-griddgm[rowclim][colclim])/100 * tempgrad;
  tsurfstart = tsurfclim + (griddgm[i][j]-griddgm[rowclim][colclim])/100 * tempgrad;

  tgrad1 = (tannual - tsurfstart)/(zannual - 0.); /*grad 0 to 10 m depth*/
  tgrad2 = (0.0 - tannual)/(ztemperate - zannual); /*grad 10 to 30 m depth*/
  
  if (k == 1)
    layertemperature[i][j][k] = tsurfstart;
  else 
  { if (layerdepth[i][j][k] < zannual )
      layertemperature[i][j][k] = layertemperature[i][j][k-1] + 
                                  (0.5*layerthickness[i][j][k]+0.5*layerthickness[i][j][k-1])*tgrad1;
    if (layerdepth[i][j][k] >= zannual  )
      layertemperature[i][j][k] = layertemperature[i][j][k-1] + 
                                  (0.5*layerthickness[i][j][k]+0.5*layerthickness[i][j][k-1])*tgrad2;
  }
  if (layertemperature[i][j][k] > 0.0) 
      layertemperature[i][j][k] = 0;
      
 /* layertemperature[i][j][k] = tannual;*/

 return;
 }
 

/*============== SNOW LAYER DENSITY - 2 default options ====================================*/

/****************************************************************************/
/* FUNCTION  default_snowlayerdensity                                          */
/*   this function calculates a default snowlayer density   */
/*   using value for firn density from input.dat                        */
/*   called from function choice_snowlayerdensity()   */
/****************************************************************************/
 
void default_snowlayerdensity()
 {
     layerdensity[i][j][k] = densfirn;

 return;
 }
 
 
/****************************************************************************/
/* FUNCTION  default_snowlayerdensity3grad                                  */
/*   this function calculates snow layer density based on 3 gradients       */
/****************************************************************************/
 
void default_snowlayerdensity3grad()
 {
  double  densgrad1,densgrad2,densgrad3;      /* vertical density gradients*/
  double  densconst1,densconst2,densconst3;
  double  densbottom,denstop;
  double  tmpup,tmpdwn;
  double  tmpdz1,tmpdz2,tmpdz3,tmpdz4;
  double  tmprho1,tmprho2,tmprho3,tmprho4;
  
  denstop = denssurface;
  if (denssurface < denssnow-1.) denstop = denssnow;
  densbottom = densfirn;
  if (FIRN[i][j] > 0.) densbottom = firndensitytop;

/*compute 3 density gradients between surface and depth of lowest firn layer*/
   densgrad1 = (densturnpoint1 - denstop)/(depthdensturnpoint1); /*grad first part below surface*/
   densgrad2 = (densturnpoint2 - densturnpoint1)/(depthdensturnpoint2 - depthdensturnpoint1); /*second part, second gradient*/
   densgrad3 = (densbottom - densturnpoint2)/(depthdensbottom - depthdensturnpoint2); /*lowest part, third gradient*/
   densconst1 = denstop;
   densconst2 = densturnpoint1 - densgrad2*depthdensturnpoint1;
   densconst3 = densturnpoint2 - densgrad3*depthdensturnpoint2;

  tmpup = 0.; 
  if (k > 1) 
    tmpup = layerdepth[i][j][k-1] + 0.5*layerthickness[i][j][k-1];
  tmpdwn = layerdepth[i][j][k] + 0.5*layerthickness[i][j][k];
  
/*assign density to each layer according to these gradients*/
  if ((tmpup < depthdensturnpoint1) && (tmpdwn < depthdensturnpoint1))
    layerdensity[i][j][k] = densconst1 + densgrad1*layerdepth[i][j][k];
  else if ((tmpup < depthdensturnpoint1) && (tmpdwn > depthdensbottom))
  { tmpdz1 = depthdensturnpoint1 - tmpup;
    tmpdz2 = depthdensturnpoint2 - depthdensturnpoint1;
    tmpdz3 = depthdensbottom - depthdensturnpoint2;
    tmpdz4 = tmpdwn - depthdensbottom;
    tmprho1 = ((densconst1 + densgrad1*tmpup)+densturnpoint1)*0.5;
    tmprho2 = (densturnpoint1 + densturnpoint2)*0.5;
    tmprho3 = (densturnpoint2 + densbottom)*0.5;
    tmprho4 = densbottom;
    layerdensity[i][j][k] = (tmpdz1*tmprho1 + tmpdz2*tmprho2 + tmpdz3*tmprho3 + tmpdz4*tmprho4)/layerthickness[i][j][k];
    if ((layerdensity[i][j][k] > densfirn) || (tmprho1 > densfirn) || (tmprho2 > densfirn) || (tmprho3 > densfirn) || (tmprho4 > densfirn))
      printf("A %d %d %d %f %f %f %f %f \n",i,j,k,layerdensity[i][j][k],tmprho1,tmprho2,tmprho3,tmprho4);
  }
  else if ((tmpup < depthdensturnpoint1) && (tmpdwn > depthdensturnpoint2))
  { tmpdz1 = depthdensturnpoint1 - tmpup;
    tmpdz2 = depthdensturnpoint2 - depthdensturnpoint1;
    tmpdz3 = tmpdwn - depthdensturnpoint2;
    tmprho1 = ((densconst1 + densgrad1*tmpup)+densturnpoint1)*0.5;
    tmprho2 = (densturnpoint1 + densturnpoint2)*0.5;
    tmprho3 = ((densconst3 + densgrad3*tmpdwn)+densturnpoint2)*0.5;
    layerdensity[i][j][k] = (tmpdz1*tmprho1 + tmpdz2*tmprho2 + tmpdz3*tmprho3)/layerthickness[i][j][k];
    if ((layerdensity[i][j][k] > densfirn) || (tmprho1 > densfirn) || (tmprho2 > densfirn) || (tmprho3 > densfirn))
      printf("B %d %d %d %f %f %f %f \n",i,j,k,layerdensity[i][j][k],tmprho1,tmprho2,tmprho3);
  }
  else if ((tmpup < depthdensturnpoint1) && (tmpdwn > depthdensturnpoint1))
  { tmpdz1 = depthdensturnpoint1 - tmpup;
    tmpdz2 = tmpdwn - depthdensturnpoint1;
    tmprho1 = ((densconst1 + densgrad1*tmpup)+densturnpoint1)*0.5;
    tmprho2 = ((densconst2 + densgrad2*tmpdwn)+densturnpoint1)*0.5;
    layerdensity[i][j][k] = (tmpdz1*tmprho1 + tmpdz2*tmprho2)/layerthickness[i][j][k];
    if ((layerdensity[i][j][k] > densfirn) || (tmprho1 > densfirn) || (tmprho2 > densfirn))
      printf("C %d %d %d %f %f %f \n",i,j,k,layerdensity[i][j][k],tmprho1,tmprho2);
  }
  else if ((tmpup > depthdensturnpoint1) && (tmpdwn < depthdensturnpoint2))
    layerdensity[i][j][k] = densconst2 + densgrad2*layerdepth[i][j][k];
  else if ((tmpup < depthdensturnpoint2) && (tmpdwn > depthdensbottom))
  { tmpdz1 = depthdensturnpoint2 - tmpup;
    tmpdz2 = depthdensbottom - depthdensturnpoint2;
    tmpdz3 = tmpdwn - depthdensbottom;
    tmprho1 = ((densconst2 + densgrad2*tmpup)+densturnpoint2)*0.5;
    tmprho2 = (densbottom+densturnpoint2)*0.5;
    tmprho3 = densbottom;
    layerdensity[i][j][k] = (tmpdz1*tmprho1 + tmpdz2*tmprho2 + tmpdz3*tmprho3)/layerthickness[i][j][k];
    if ((layerdensity[i][j][k] > densfirn) || (tmprho1 > densfirn) || (tmprho2 > densfirn) || (tmprho3 > densfirn))
      printf("D %d %d %d %f %f %f %f \n",i,j,k,layerdensity[i][j][k],tmprho1,tmprho2,tmprho3);
  }
  else if ((tmpup < depthdensturnpoint2) && (tmpdwn > depthdensturnpoint2))
  { tmpdz1 = depthdensturnpoint2 - tmpup;
    tmpdz2 = tmpdwn - depthdensturnpoint2;
    tmprho1 = ((densconst2 + densgrad2*tmpup)+densturnpoint2)*0.5;
    tmprho2 = ((densconst3 + densgrad3*tmpdwn)+densturnpoint2)*0.5;
    layerdensity[i][j][k] = (tmpdz1*tmprho1 + tmpdz2*tmprho2)/layerthickness[i][j][k];
    if ((layerdensity[i][j][k] > densfirn) || (tmprho1 > densfirn) || (tmprho2 > densfirn))
      printf("E %d %d %d %f %f %f \n",i,j,k,layerdensity[i][j][k],tmprho1,tmprho2);
  }
  else if ((tmpup > depthdensturnpoint2) && (tmpdwn < depthdensbottom))
    layerdensity[i][j][k] = densconst3 + densgrad3*layerdepth[i][j][k];
  else if ((tmpup < depthdensbottom) && (tmpdwn > depthdensbottom))
  { tmpdz1 = depthdensbottom - tmpup;
    tmpdz2 = tmpdwn - depthdensbottom;
    tmprho1 = ((densconst3 + densgrad3*tmpup)+densbottom)*0.5;
    tmprho2 = densbottom;
    layerdensity[i][j][k] = (tmpdz1*tmprho1 + tmpdz2*tmprho2)/layerthickness[i][j][k];
    if ((layerdensity[i][j][k] > densfirn) || (tmprho1 > densfirn) || (tmprho2 > densfirn))
      printf("F %d %d %d %f %f %f \n",i,j,k,layerdensity[i][j][k],tmprho1,tmprho2);
  }
  else if (tmpup > depthdensbottom)
    layerdensity[i][j][k] = densbottom;

  if (layerdensity[i][j][k] > densfirn) 
    layerdensity[i][j][k] = densfirn;

   return;
 }


/*============== FIRN LAYER DENSITY - 2 default options ======================================*/

/****************************************************************************/
/* FUNCTION  default_firnlayerdensity                                       */
/*   this function calculates a default firnlayer density                   */
/*   using value for firn density from input.dat                            */
/****************************************************************************/
 
void default_firnlayerdensity()
 {
     layerdensity[i][j][k] = densfirn ;

 return;
 }

/****************************************************************************/
/* FUNCTION  default_firnlayerdensity1grad                                  */
/*   this function calculates a firnlayer density by a linear gradient      */
/*   called from choice_firnlayerdensity in snowinput.c                     */
/****************************************************************************/

void default_firnlayerdensity1grad()
 {   
     float firndensitygradient;    /*increase in density with increasing depth*/
     
       if((firndensitytop < 200) || (firndensitybottom < 200))
          {  printf(" \n function default_firnlayerdensity1grad: densities wrong %.2f or %.2f \n",firndensitytop,firndensitybottom);
             exit(4);
          }
     
     firndensitygradient = (firndensitybottom-firndensitytop)/firndepthbottom;
       
     layerdensity[i][j][k] = firndensitytop + layerdepth[i][j][k]*firndensitygradient;

     if(layerdensity[i][j][k] > 900)
        layerdensity[i][j][k] = 900;
     

 return;
 }

/*============== CONVERSION SNOW LAYER DENSITY - 2 default options ======================*/
/*============== INVERTS DEFAULT SNOW LAYER DENSITY OPTIONS ======================*/

/****************************************************************************/
/* FUNCTION  default_snowlayermsnow                                         */
/*   this function calculates snow layer thickness in m of snow             */
/*   using value for firn density from input.dat                            */
/****************************************************************************/
 
void default_snowlayermsnow()
 {

  if ((surface[i][j] != nodis) && (SNOW[i][j] > 0))
     snowlayer[i][j] = (SNOW[i][j]*10)/densfirn;
  else
    snowlayer[i][j] = 0.0;

 return;
 }


/****************************************************************************/
/* FUNCTION  default_snowlayermsnow3grad                                         */
/*   this function calculates snow layer thickness in m of snow             */
/*   using value for firn density from input.dat                            */
/****************************************************************************/
 
void default_snowlayermsnow3grad()
 {
  double  densgrad1,densgrad2,densgrad3;      /* vertical density gradients*/
  double  densconst1,densconst2,densconst3;
  double  densbottom,denstop;
  double  tmpdz,tmprho,tmpm;
  double  tmptot,tmpa,tmpb,tmpc,tmpy;
  
if ((surface[i][j] != nodis) && (SNOW[i][j] > 0))
{
  denstop = denssurface;
  if (denssurface < denssnow-1.) denstop = denssnow;
  densbottom = densfirn;
  if (FIRN[i][j] > 0.) densbottom = firndensitytop;

/*compute 3 density gradients between surface and depth of lowest firn layer*/
   densgrad1 = (densturnpoint1 - denstop)/(depthdensturnpoint1); /*grad first part below surface*/
   densgrad2 = (densturnpoint2 - densturnpoint1)/(depthdensturnpoint2 - depthdensturnpoint1); /*second part, second gradient*/
   densgrad3 = (densbottom - densturnpoint2)/(depthdensbottom - depthdensturnpoint2); /*lowest part, third gradient*/
   densconst1 = denstop;
   densconst2 = densturnpoint1 - densgrad2*depthdensturnpoint1;
   densconst3 = densturnpoint2 - densgrad3*depthdensturnpoint2;

  tmprho = (densturnpoint1 + denstop)*0.5;
  tmpdz = depthdensturnpoint1;
  tmpm = tmpdz*tmprho;
  if (SNOW[i][j]*10 > tmpm)
  { tmptot = SNOW[i][j]*10.-tmpm;
    tmprho = (densturnpoint2 + densturnpoint1)*0.5;
    tmpdz = depthdensturnpoint2 - depthdensturnpoint1;
    tmpm = tmpdz*tmprho;
    if (tmptot > tmpm)
    { tmptot = tmptot - tmpm;
      tmprho = (densbottom + densturnpoint2)*0.5;
      tmpdz = depthdensbottom - depthdensturnpoint2;
      tmpm = tmpdz*tmprho;
      if (tmptot > tmpm)
      { tmptot = tmptot - tmpm;
        tmprho = densbottom;
        tmpdz = tmptot/tmprho;
        tmpy = tmpdz + depthdensbottom;
      }
      else
      { tmpa = 0.5*densgrad3;
        tmpb = densconst3;
        tmpc = -0.5*densgrad3*pow(depthdensturnpoint2,2) - densconst3*depthdensturnpoint2-tmptot;
        tmpy = (-tmpb + sqrt( pow(tmpb,2) - 4*tmpa*tmpc ))/(2*tmpa);
      }
    }
    else
    { tmpa = 0.5*densgrad2;
      tmpb = densconst2;
      tmpc = -0.5*densgrad2*pow(depthdensturnpoint1,2) - densconst2*depthdensturnpoint1-tmptot;
      tmpy = (-tmpb + sqrt( pow(tmpb,2) - 4*tmpa*tmpc ))/(2*tmpa);
    }
  }
  else
  { tmpa = 0.5*densgrad1;
    tmpb = densconst1;
    tmpc = -0.5*densgrad1*0.*0. - densconst1*0.-SNOW[i][j]*10.;
    tmpy = (-tmpb + sqrt( pow(tmpb,2) - 4*tmpa*tmpc ))/(2*tmpa);
   }
  snowlayer[i][j] = tmpy;
}
else
  snowlayer[i][j] = 0.0;

 return;
 }


/*===================================================================================*/
/*===== USER FILES: ADD FUNCTIONS HERE FOR NEW APPLICATIONS =========================*/
/*===================================================================================*/

/*===== FUNCTION TO INITIALIZE THE SNOW MODEL FOR STORGLACIAREN 1999 ===*/
 /*   denssnowwe[4] 0,1,2,3 (values given are for 1999 season, 0 allways empty) */
/*   denscoeff[4] 0,1,2,3 (values given are for may 1999, 0 allways empty) NOT USED  */
/*   tempcoeff[4] 0,1,2,3 (values given are for day 127, 1999, 24 h, 0 allways empty) */
/****************************************************************************/
/* FUNCTION  stor_layertemperature                                          */
/*   this function calculates layer temperature                             */
/*   based on measurements at Storglaciaren for may 1999                    */
/****************************************************************************/
 
void stor_layertemperature()
 {
/*  float  tempcoeff[4]={0,-5.08817,-1.94638,0.967214}; */
  float  tempcoeff[4]={0,-5.7912,-0.49947,0.41518}; 
  /* T = tempcoeff[1] + tempcoeff[2]*(m snow depth) + tempcoeff[3]*(m snow depth)^2 */
  float  tgrad1=0.575;        /* 0.43 vertical temperature gradient below top part till 10 m value tannual*/
  float  tgrad2=0.10;        /* 0.06 vertical temperature gradient from 10 m till temperate part at 30 m depth*/
  float  tmptemp,depth;
  float  gradtemp;
  float  tdepth1=2.5;
  float  tdepth2=17.5;		/*was 10*/
  float  tdepth3=25;		/*was 30*/
   
 /* gradtemp = tempgrad;*/
  gradtemp = -1.0;	/* C/100m */

/*temperature*/
  tmptemp = tempcoeff[1] + tempcoeff[2] * layerdepth[i][j][k] + 
            tempcoeff[3] * pow(layerdepth[i][j][k],2);
  layertemperature[i][j][k] = tmptemp + (griddgm[i][j]-griddgm[rowclim][colclim])/100. * gradtemp;
  depth = layerdepth[i][j][k];
  if ((depth <= tdepth2) && (depth > tdepth1 ))
     layertemperature[i][j][k] = layertemperature[i][j][k-1] + 
                                 (0.5*layerthickness[i][j][k]+0.5*layerthickness[i][j][k-1])*tgrad1;
     if (depth > tdepth2 )
       layertemperature[i][j][k] = layertemperature[i][j][k-1] + 
                                   (0.5*layerthickness[i][j][k]+0.5*layerthickness[i][j][k-1])*tgrad2;                                       
     if (depth > tdepth3 )
       layertemperature[i][j][k] = 0.;
     
     /*layertemperature[i][j][k] = layertemperature[i][j][k] - 1.;*/
     
     if (layertemperature[i][j][k] > 0.0) 
         layertemperature[i][j][k] = 0;

 return;
 }
 
 

/****************************************************************************/
/* FUNCTION  stor_snowlayerdensity                                          */
/*   this function calculates snow layer density                            */
/*   based on density measurements at Storglaciaren                         */
/****************************************************************************/
 
void stor_snowlayerdensity()
 {
   double denssnowwe[4]={0,0.0002,0.3414,0.0}; 
   /* cm we = denssnowwe[1]*(cm snow)^2 + denssnowwe[2]*(cm snow) */
   double  sum;

/*density */
   if (k > 1)
     sum = denssnowwe[1] * pow((layerdepth[i][j][k-1]+0.5*layerthickness[i][j][k-1])*100.,2) 
           + denssnowwe[2] *    ((layerdepth[i][j][k-1]+0.5*layerthickness[i][j][k-1])*100.) 
           + denssnowwe[3];
   else
     sum = 0.;
   
   layerdensity[i][j][k] = (( denssnowwe[1] * pow((layerdepth[i][j][k]+0.5*layerthickness[i][j][k])*100.,2) 
                           + denssnowwe[2] *    ((layerdepth[i][j][k]+0.5*layerthickness[i][j][k])*100.) 
                           + denssnowwe[3] - sum ) / (100.*layerthickness[i][j][k]) ) *1000.;

 return;
 }

/****************************************************************************/
/* FUNCTION  stor_firnlayerdensity                                          */
/*   this function calculates firn layer density                            */
/*   based on density measurements at Storglaciaren                         */
/****************************************************************************/
 
void stor_firnlayerdensity()
 {
   /*float  denscoeff[4]={0,186.296,219.988,1.63493}; */
   float  denscoeff[4]={0,186.296,200.0,1.63493}; 
   /* kg/m3 = denscoeff[1] + denscoeff[2]*ln(m snow depth + denscoeff[3]) */
   
   layerdensity[i][j][k] = denscoeff[1] + denscoeff[2] * log(layerdepth[i][j][k] + denscoeff[3]);

 return;
 }
 
/****************************************************************************/
/* FUNCTION  stor_snowlayermsnow                                            */
/*   this function calculates snow layer thickness in m of snow             */
/*   based on density measurements at Storglaciaren                         */
/****************************************************************************/
 
void stor_snowlayermsnow()
 {
   double denssnowwe[4]={0,0.0002,0.3414,0.0}; 
   /* cm we = denssnowwe[1]*(cm snow)^2 + denssnowwe[2]*(cm snow) */

/*initialising the arrays and recalculate snow depth from cm we to m snow*/  
   if ((surface[i][j] != nodis) && (SNOW[i][j] > 0))
     snowlayer[i][j] = (-denssnowwe[2] + sqrt( pow(denssnowwe[2],2) - 4*denssnowwe[1]*(denssnowwe[3]-SNOW[i][j]))) 
                       / (100 * 2 * denssnowwe[1]);
   else
     snowlayer[i][j] = 0.0;

 return;
 }

/******************************************************************/
/* FUNCTION  choice_outputlines                                           */
/*    called from outputsubsurflines in snowinit.c     */
/*    choice to be set in variab.h */
/******************************************************************/

void choice_outputlines()
{
   int inti;
  
  inti=(int)init_outputlines;
  switch(inti)
  
   {  case 1: default_outputlines();       /*default*/
              break;
      case 2: stor_outputlines();       /*Storglaciaren*/
              break;
   }

 return;
}

/******************************************************************/
/* FUNCTION  default_outputlines                                           */
/*    called from choice_outputlines in snowinit.c     */
/*    default is no output lines */
/******************************************************************/

void default_outputlines()
{

  klinesmax = 0;
  iline1[1] = 1.;
  jline1[1] = 1.;
  iline2[1] = 1.;
  jline2[1] = 1.;

 return;
}

/******************************************************************/
/* FUNCTION  stor_outputlines                                           */
/*    called from choice_outputlines in snowinit.c     */
/*    default is no output lines */
/******************************************************************/

void stor_outputlines()
{

  klinesmax = 3;		/*Note kmax is set in outputsubsurflines max is klinesmax = kkmax -1, because array starts at 0*/

  iline1[1] = 49.317; /*row Stake 2*/
  jline1[1] = 115.21; /*col Stake 2*/
  iline2[1] = 42.983; /*row Stake 29*/
  jline2[1] = 25.421; /*col Stake 29*/
  
  iline1[2] = 13.050; /*row Stake 29N9*/
  jline1[2] = 27.529; /*col Stake 29N9*/
  iline2[2] = 69.583; /*row Stake 29S8*/
  jline2[2] = 23.542; /*col Stake 29S8*/

  iline1[3] = 35.583; /*row Stake 18N3*/
  jline1[3] = 62.704; /*col Stake 18N3*/
  iline2[3] = 62.200; /*row Stake 18S5*/
  jline2[3] = 60.829; /*col Stake 18S5*/

 return;
}

/****************************************************************************/
/* FUNCTION  outputsubsurf                                                  */
/*   this function prints out subsurf conditions for given location         */
/*   for each time step one row for each layer                              */
/*   called from main once before time step loop and in time loop for each grid cell */
/****************************************************************************/

void outputsubsurf()
{ 
  int     kk,ki,kj;
  
  jd2=jd;
  if ((zeit == 24) && (inter == factinter))
   jd2 = (int)jd + 1;

  for (kk=1; kk <= outgridnumber; kk++)  /*for each point to be written to file*/
  { 
    if (nsteps == 0)
    { i = stnrow[kk];     /*row of grid point, read from input.dat*/
      j = stncol[kk]; 
      
      fprintf(outsubsurf[kk]," Site i=%d j=%d height=%.2f \n",i,j,griddgm[i][j]);
      fprintf(outsubsurf[kk]," time nr depth(m) thickness(m) temp(C) dens(g/kg) mass(kg/m2) water(kg/m2) refreeze(kg/m2) dsnow(msnow) dsnow(mmwe) id irrwc \n");
      
      fprintf(outcontrol,"Writing subsurface profiles of location i %d j %d \n",i,j);
    }  /*endif*/

    if ((i == stnrow[kk]) && (j == stncol[kk]))    /*for grid cell to be outputted, read from input.dat*/
    {       
      for (k = 1; k <= (int)layeramount[i][j]; k++)     /*for each layer*/
      { 
      if (irrwatercontyes == 1) irreducible_schneider();
      if (irrwatercontyes == 2) irreducible_coleou();
        
	if ((k == 1) && (nsteps == 0))
        { surftempfrommodel();
          fprintf(outsubsurf[kk]," %d.000000 %2d 0.000000 0.000000 %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f %.3f \n",
                (int)jd2,k,surftemp[i][j],layerdensity[i][j][1],layermass[i][j][1],
                layerwatercont[i][j][1],layerrefreeze[i][j][1],snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][1],irrwatercont);
        }
	
        if (nsteps == 0)
        { fprintf(outsubsurf[kk]," %d.000000 %2d %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f %.3f \n",
               (int)jd2,k,layerdepth[i][j][k],layerthickness[i][j][k],layertemperature[i][j][k],
               layerdensity[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],layerrefreeze[i][j][k],
               snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][k],irrwatercont);
        }
        if ((k == 1) && (nsteps > 0))
        { surftempfrommodel();
          fprintf(outsubsurf[kk]," %f %2d 0.000000 0.000000 %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f %.3f \n",
                jd2,k,surftemp[i][j],layerdensity[i][j][1],layermass[i][j][1],
                layerwatercont[i][j][1],layerrefreeze[i][j][1],snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][1],irrwatercont);
        }

        if (nsteps > 0)
        { fprintf(outsubsurf[kk]," %f %2d %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f %.3f \n",
              jd2,k,layerdepth[i][j][k],layerthickness[i][j][k],layertemperature[i][j][k],
              layerdensity[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],layerrefreeze[i][j][k],
              snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][k],irrwatercont);
        }
      }   /*endfor  each layer*/
    }  /*endif right grid cell*/
  }  /*endfor*/
      
 return;
}

/****************************************************************************/
/* FUNCTION  outputsubsurflines                                             */
/*   this function prints out subsurf conditions for given location         */
/*   for each time step one row for each layer                              */
/*   called from main once before time step loop and in time loop for each grid cell */
/****************************************************************************/

void outputsubsurflines()
{ 

  float   a[11],b[11]; /*y = a x + b*/
  float   calc[11],diff[11];
  float   i1[11],j1[11],i2[11],j2[11];
  int     kk,ki,kj;
  int     printyes;
  int     of,cf;
  char dummysubsurf[20];

  of = 0;
  cf = 0;
  if ((i == rowclim) && (j == colclim))
  {if ((methodsurftempglac == 2) || (methodsurftempglac == 4) && (griddgmglac[i][j] != nodata))
    of = 1;
  }
  if ((i == firstrow) && (j == firstcol[firstrow]) && (of == 0))
  {if ((methodsurftempglac == 2) || (methodsurftempglac == 4) && (griddgmglac[i][j] != nodata))
    of = 0;
  else
    of = 1;
  }
  if ((i == lastrow) && (j == lastcol[lastrow])) cf = 1;

  choice_outputlines();

  if (klinesmax > 0)
  {
  
  if (of == 1)
  { for (kk=1; kk <= klinesmax; kk++)
    { strcpy(dummysubsurf,"subsurfline");   /*add in front of outgridname*/
      if (dat_or_txt == 1)
        sprintf(outsubsurflinename,"%1d%03d%04d.dat",kk,(int)jdsurface[newday],(int)year);
      if (dat_or_txt == 2)
        sprintf(outsubsurflinename,"%1d%03d%04d.txt",kk,(int)jdsurface[newday],(int)year);
      strcpy(dummy,outpath);
      strcat(dummy,dummysubsurf);
      strcat(dummy,outsubsurflinename);
      printf("\nsnowmodel output: %f %d %d %s",jd2,i,j,dummy);
       
      if ((outsubsurfline[kk] = fopen(dummy,"wt")) == NULL)
      { printf("\n Error in opening outsubsurfline file \n (file snowinput.c) %s \n",dummy);
	  exit(4);
      } /*ENDIF*/
    } /*ENDFOR*/
  }
  
  for (kk = 1; kk <= klinesmax; kk++)
  {  
    printyes = 0;
   
    a[kk] = (iline1[kk] - iline2[kk])/(jline1[kk] - jline2[kk]);
    b[kk] = iline1[kk] - a[kk]*jline1[kk];
  
    if (kk == 1) 
    { calc[kk] = a[kk]*j + b[kk];
      diff[kk] = fabs(calc[kk]-i);
      if (diff[kk] < 0.50) printyes = kk;
      ki = i;
      kj = j;
    }
    if (kk > 1)
    { calc[kk] = (i - b[kk])/a[kk];
      diff[kk] = fabs(calc[kk]-j);
      if (diff[kk] < 0.50) printyes = kk;
      ki = j;
      kj = i;
    }
      
    if (printyes != 0)
    {      
      for (k = 1; k <= (int)layeramount[i][j]; k++)     /*for each layer*/
      { 
        if (k == 1)
        { surftempfrommodel();
          fprintf(outsubsurfline[kk]," %3d %3d %2d 0.000000 0.000000 %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f \n",
                ki,kj,k,surftemp[i][j],layerdensity[i][j][1],layermass[i][j][1],
                layerwatercont[i][j][1],layerrefreeze[i][j][1],snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][1]);
        }

        fprintf(outsubsurfline[kk]," %3d %3d %2d %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f \n",
              ki,kj,k,layerdepth[i][j][k],layerthickness[i][j][k],layertemperature[i][j][k],
              layerdensity[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],layerrefreeze[i][j][k],
              snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][k]);
      }   /*endfor  each layer*/
  
    }/*end printyes*/
  }/*end kk loop, different lines*/

  if (cf == 1)
  { for (kk=1; kk <= klinesmax; kk++)
    { fclose(outsubsurfline[kk]);
    } /*ENDFOR*/
  }
  
  } /*ENDIF klinesmax loop*/
    
 return;
}
