/****************************************************************************/
/*  FILE  :  userfile.c                                                     */
/*    Function, which are specific to a certain application                 */
/*    not generally applicable                                              */
/*    10.9.1997,  update March 2006                                         */
/****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "closeall.h"
#include "discharg.h"
#include "disopt.h"
#include "globcor.h"
#include "initial.h"
#include "input.h"
#include "radiat.h"
#include "scaling.h"
#include "snowinput.h"
#include "snowmodel.h"
#include "tindex.h"
#include "turbul.h"
#include "userfile.h"
#include "writeout.h"

#include "variabex.h"      /* all global VARIABLES */



/*************************************************************/
/* FUNCTION  readalbedo29 */
/*   this function is specific to application to Storglaciaren */
/*************************************************************/
 
void readalbedo29()

 {
   FILE   *inalbedosnow=NULL;
   char   filenamealbedo[61];
   int    jdalb;
   float  a1,a2,a3,a4,a5,a6,a7,a8,a9,a10;  

    strcpy(dummy,inpath);
    strcat(dummy,"29tag94.dat");   /* $$$$$$ NAME File with daily albedo $$$$$ */

    if ((inalbedosnow = fopen (dummy,"rt")) == NULL)               /*OPEN FILE*/
     {  printf("\n Input-File '29tag94.dat' not found \n\n");
       exit(1);
     }

    fgets(rest,200,inalbedosnow);   fgets(rest,200,inalbedosnow);  

   for (i=1; i<=1000; i++)
     albedosnow[i] = 0.0;   
  
 fprintf(outcontrol,"function readalbedo29\n");

  while (fscanf(inalbedosnow,"%f%f%f%f%f%f%f%f%f%f",&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10) != EOF)
  {     
    jdalb = (int)a1;  
    albedosnow[jdalb] = a9;
    fprintf(outcontrol,"%d %5.2f\n",jdalb,albedosnow[jdalb]);
  /*  printf("%d %5.2f\n",jdalb,albedosnow[jdalb]);   */
   
  }
   closefile(&inalbedosnow,filenamealbedo);

 }

/*************************************************************/
/* FUNCTION  readalbedo16 */
/*    for 1993*/
/*************************************************************/
 
void readalbedo16()

 {
   FILE   *inalbedosnow=NULL;
   char   filenamealbedo[61];
   int    jdalb;
   float  a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12;  

    strcpy(dummy,inpath);
    strcat(dummy,"16tag93.dat");   /* $$$$$$ NAME File with daily albedo $$$$$ */

    if ((inalbedosnow = fopen (dummy,"rt")) == NULL)               /*OPEN FILE*/
     {  printf("\n Input-File '16tag93.dat' not found \n\n");
       exit(1);
     }

    fgets(rest,200,inalbedosnow);   fgets(rest,200,inalbedosnow);  

   for (i=1; i<=1000; i++)
     albedosnow[i] = 0.0;   
  
 fprintf(outcontrol,"function readalbedo16\n");

  while (fscanf(inalbedosnow,"%f%f%f%f%f%f%f%f%f%f%f%f",
	 &a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11,&a12) != EOF)
  {     
    jdalb = (int)a1;  
    albedosnow[jdalb] = a10;
    if ((jdalb > 213) && (jdalb < 234))
       albedosnow[jdalb] = 0.6;

    fprintf(outcontrol,"%d %5.2f\n",jdalb,albedosnow[jdalb]);
 /*   printf("%d %5.2f\n",jdalb,albedosnow[jdalb]);  */
   
  }

  closefile(&inalbedosnow,filenamealbedo);

 }


/*********************************************************************/
/*  FUNCTION : tempsurfacerock                                       */
/*      calculation of surface temperature of area outside glacier   */
/*      needed for longwave incoming acc. to Pluess, 1997            */
/*      Parameterization derived from Kaerkevagge data               */
/*      2 different functions fitted to account for daily hysteresis */
/*      needs global radiation for grid cell                         */
/*      called from main for each grid cell                          */
/*********************************************************************/

void tempsurfacerock()
{ 
 
  if (griddgmglac[i][j] == nodata)   /*outside glacier*/
  {
    if(SNOW[i][j] == 0)      /*if snow-covered, different function*/
    { 

     if(GLOBAL[i][j] > 10)
      { if(zeit <= 12)      /*FUNCTION BEFORE NOON: EXPONENTIAL*/
         surftemp[i][j] = exp(0.00177*GLOBAL[i][j]) * 2.3217 + tempint[i][j];
        else  /*AFTERNOON*/   
         { surftemp[i][j] = 4.991 * log(GLOBAL[i][j]) - 15.4 + tempint[i][j];
           if(surftemp[i][j] < tempint[i][j])
             surftemp[i][j] = tempint[i][j];
         }  /*endelse afternoon*/

      }  /*endif Global>10*/
      else      /*DURING NIGHT - no global rad*/
        surftemp[i][j] = tempint[i][j];

     }  /*endif not snow-covered*/
   }  /*endif outside glacier*/    

  return;
}


/*********************************************************************/
/*  FUNCTION:  iceheatStorglac                                       */
/*     ice heat flux decreases linearly as function of time          */
/*********************************************************************/

void iceheatStorglac()
{  
  float maxiceheat=5;      /*maximal heat loss by ice heat flux*/
  float minday=180,maxday=235;  /*julian days of max and 0, linear interpolation*/

  if(surface[i][j]==3)      /*ice exposed*/
   { if(jd < minday)
      ICEHEAT[i][j] = maxiceheat;    
     else
      { if(jd > maxday)
         ICEHEAT[i][j] = 0;
        else
         ICEHEAT[i][j] = maxiceheat - (jd-180)*maxiceheat/(maxday-minday);
      }  /*else*/
   } /*endif*/

 return;
}



/*********************************************************************/
/*  FUNCTION:  openphotovernagt                                      */
/*     open file with photodata (once) and read first date           */
/*********************************************************************/

void openphotovernagt()

{  float  monatphoto,tagphoto,yearphoto;
   int    leapyes;

  /*** OPEN FILE photo__.dat FILE ****/
    strcpy(dummy,inpath);
    strcat(dummy,namealb[1]);

/*printf("\nphotofile used:  %s\n",&namealb[1]); 			compiler problem CHR*/
printf("\nphotofile used:  %s\n",namealb[1]);

      if ((inalbedo = fopen(dummy,"rt")) == NULL)      /*OPEN NEW ALBEDO-FILE*/
       { printf("\n\n Photo-File  %s  not found \n\n",namealb[1]);
         exit(20);
       }

/*** READ FIRST DATE AND CONVERT TO JULIAN DAY ***/
   fscanf(inalbedo, "%f%f%f",&tagphoto,&monatphoto,&yearphoto);
    if (monatphoto == 1. )   { jdphoto=1.; }
    if (monatphoto == 2. )   { jdphoto=32.; }
    if (monatphoto == 3. )   { jdphoto=60.; }
    if (monatphoto == 4. )   { jdphoto=91.; }
    if (monatphoto == 5. )   { jdphoto=121.; }
    if (monatphoto == 6. )   { jdphoto=152.; }
    if (monatphoto == 7. )   { jdphoto=182.; }
    if (monatphoto == 8. )   { jdphoto=213.; }
    if (monatphoto == 9. )   { jdphoto=244.; }
    if (monatphoto == 10. )  { jdphoto=274.; }
    if (monatphoto == 11. )  { jdphoto=305.; }
    if (monatphoto == 12. )  { jdphoto=335.; }

    jdphoto += tagphoto-1;

    if( ((int)yearphoto%4) == 0)
        leapyes=1;
    if( ((int)yearphoto%100) == 0)
        leapyes=0;
    if( ((int)yearphoto%400) == 0)
        leapyes=1;
    if( (leapyes=1) && (jdphoto > 59))
        jdphoto += 1;

printf("openphotovernagt: tag=%3.0f mon=%3.0f year=%4.0f jdphoto = %6.2f  jd= %6.2f \n\n",
         tagphoto,monatphoto,yearphoto,jdphoto,jd);

  /*** ASSUME GLACIER IS SNOW-COVERED IF DATE BEFORE FIRST PHOTO AVAILABLE ***/
  
      for (i=1;i<=nrows;i++)
        for (j=1;j<=ncols;j++)
              surface[i][j] = 1;      /*SNOW SURFACE*/           
    
 return;
}


/*********************************************************************/
/*  FUNCTION:  readphotovernagt                                      */
/*     read one days matrix and next date                            */
/*     for application VERNAGTFERNER                                 */
/*********************************************************************/

void readphotovernagt()
{ 
  int    rowphoto=42, colphoto=50, leapyes;
  int    iphoto,jphoto;
  float  wert;
  int    jj,ifivestep,anzahlrows=0;
  float  monatphoto,tagphoto,yearphoto;
  int    surface2output=0;
        
 while (jdphoto+0.02 <= jd)
 {
   for (i=1;i<=nrows;i++) 
     for (j=1;j<=ncols;j++)                
        surface[i][j] = 1;    /*SNOW*/

  i=1;

  for (iphoto=1; iphoto<=rowphoto; iphoto++)
   {  
    j=0;     /*counter re-set before new line begins*/
    for (jphoto=1; jphoto<=colphoto; jphoto++)
     { 
       fscanf(inalbedo,"%f",&wert);
        if(wert == 1)                     /*PHOTO 1=ICE*/
	  wert = 3;    /*MODEL 3=ICE*/
	if(wert == 0)                     /*PHOTO 0=SNOW*/
	  wert = 1;    /*MODEL 1=SNOW*/

  /*** FILL VALUE 5 TIMES: 100 m -> 20 m GRID ***/
        for (jj=1;jj<=5;jj++)
          { 
            j = j+1;       /*must increase continously for whole line*/
    	    surface[i][j] = wert;
          }
     }  /*endfor next column*/

     i+=5;     /*fill only every 5. row at right place (every 100 m)*/
   }  /*endfor next row*/

  /*** FILL REST ROWS OF ARRAY 5 TIMES SAME ROW, THEN NEW DATA ***/
   ifivestep  = 1;
   anzahlrows = 0;

   for (i=1;i<=nrows;i++)
    {  
      for (j=1;j<=ncols;j++)                
        { surface[i][j] = surface[ifivestep][j];             
        }  /*endfor columns*/

      anzahlrows += 1;
      if(anzahlrows == 5)
        {  anzahlrows = 0;
           ifivestep += 5;
        }
    } /*NEXT ROW*/

/* CELLS OUTSIDE GLACIER SET TO NODATA*/
   for (i=1;i<=nrows;i++)
    {    
      for (j=1;j<=ncols;j++)  
        { if(griddgmdrain[i][j] == nodata)
	    surface[i][j] = nodata;
        }
    }

/*** WRITE TO FILE ***/ 
 if(surface2output==1)  
  {
   sprintf(namesurf,"sur%03d%02d.bin",(int)jd,(int)zeit);
   openoutfile(&outsurf,outpath,namesurf);
   statist(surface,x);
   fwrite(&(x[1]),sizeof(float),12,outsurf);     /*FIRST 12 ROWS*/
   for (i=1;i<=nrows;i++)        /* for all rows (and columns in fwrite command) */
     fwrite(&(surface[i][1]),sizeof(float),ncols,outsurf);
   if(outsurf)    
     closefile(&outsurf,namesurf);     
  }

/*** READ NEXT DATE */
   fscanf(inalbedo, "%f%f%f",&tagphoto,&monatphoto,&yearphoto);
    if (monatphoto == 1. )   { jdphoto=1.; }
    if (monatphoto == 2. )   { jdphoto=32.; }
    if (monatphoto == 3. )   { jdphoto=60.; }
    if (monatphoto == 4. )   { jdphoto=91.; }
    if (monatphoto == 5. )   { jdphoto=121.; }
    if (monatphoto == 6. )   { jdphoto=152.; }
    if (monatphoto == 7. )   { jdphoto=182.; }
    if (monatphoto == 8. )   { jdphoto=213.; }
    if (monatphoto == 9. )   { jdphoto=244.; }
    if (monatphoto == 10. )  { jdphoto=274.; }
    if (monatphoto == 11. )  { jdphoto=305.; }
    if (monatphoto == 12. )  { jdphoto=335.; }

    jdphoto += tagphoto-1;

    if( ((int)yearphoto%4) == 0)
        leapyes=1;
    if( ((int)yearphoto%100) == 0)
        leapyes=0;
    if( ((int)yearphoto%400) == 0)
        leapyes=1;
    if( (leapyes=1) && (jdphoto > 59))
        jdphoto += 1;

   /*   printf("\nread next date: tag=%3.0f monat=%3.0f year=%4.0f jdphoto=%6.2f \n",tagphoto,monatphoto,yearphoto,jdphoto);  */

 }  /*endwhile*/

 return;
}

/******************      ZONGO      ********************************************/


/******************************************************************/
/* FUNCTION  albedosnownewzgo         (adapted to Zongo glacier)  */
/*           new function using days since snow fall and air temp */ 
/*    called from function albedocalczgo for each grid cell       */
/*    no sqrt(nd) BUT in the exponentiel                          */
/******************************************************************/

void albedosnownewzgo()
{
    float  albk1=0.005, albk2=-1.1, albk3=0.022;
    float  albmax=0.9;      /*maximum snow albedo value*/
    float  tempfactor,daydecay;

 if(snowprec == 0)          /*no snow fall, decrease in albedo*/
  {
    numbdays[i][j] += timestep/24.0;    /*number of days in days*/

   if(tempint[i][j] > 0)
     tempfactor = log(tempint[i][j]+1);   /*faster decay with higher temp*/
   else
     tempfactor = 0.1;    /*no temp induced decay, only number of days*/

   daydecay = albk1*tempfactor*exp(albk2*(numbdays[i][j]));  /*sqrt is deleted JES*/
    if(daydecay > albk1)     /*to avoid values > 1, if exponent approaches 0*/
      daydecay = albk1;

   ALBEDO[i][j] = ALBALT[i][j] - daydecay;     /*albedo decay*/


  /* albedo drop by 0.05 if snow has melted and firn below */
    if((snowtofirn[i][j] == 0) && (SNOW[i][j] == 0))
     {  ALBEDO[i][j] = ALBALT[i][j] - 0.05;
        snowtofirn[i][j] = 1;
          /*array controls that this albedo drop only occurs once, boolean meaning*/
     }

    if(ALBEDO[i][j] < albmin)          /*albedo cannot be lower than albmin*/
      ALBEDO[i][j] = albmin;


 /*to avoid that albedo drops to fast, if hours of snow fall and no snow alternate;
   albedo is allowed only to drop to the value of ALBEDO BEFORE SNOW fall; after
   that the decay continues as if there were no snow fall, ALBBEFSNOW initialized to 2*/
    if((ALBEDO[i][j] < ALBBEFSNOW[i][j]) && (ALBBEFSNOW[i][j] != 2)) 
     {  ALBEDO[i][j]   = ALBBEFSNOW[i][j];
        numbdays[i][j] = ndbefsnow[i][j];
        ALBBEFSNOW[i][j] = 2;
          /*set to 0 to make sure, that this loop is done only if there was snow fall
            during the previous time step. If it was not to 0, albedo would not at all be
            allowed to fall below ALBBEFSNOW which would be wrong*/
     }
  
  ALBEDO[i][j] = albmin;       /*first check  JES*/ 
  }
 else   /*SNOW FALL, ALBEDO INCREASES */
  {    /*first time step with snowfall, albedo decay has occurred before*/
       /*numbdays should only increase, not decrease, if 2. condition was not employed
         ndbefsnow could be very small (after 1 hour of snow fall) and subsequent decay
         would be very fast and thus too rapid*/
    if((numbdays[i][j] > 0) && (ALBALT[i][j] < ALBBEFSNOW[i][j])) 
     { ALBBEFSNOW[i][j] = ALBALT[i][j];    /*store albedo of previous time step*/
       ndbefsnow[i][j]  = numbdays[i][j];
     }

    ALBEDO[i][j] = ALBALT[i][j] + albk3 * snowprec;   /*albedo increased*/
      if(ALBEDO[i][j] > albmax)        /*upper limit of albedo*/
        ALBEDO[i][j] = albmax;
    numbdays[i][j] = 0;
  }

 return;
}

/******************************************************************/
/*  FUNCTION : longinstationbruntsvernagt                         */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION */
/*             FROM PARAMETERIZATION by Brunts                    */ 
/*	       specified for Vernagt, because climate station not */
/*	       in area to be calculated                           */ 
/******************************************************************/

void longinstationbruntsvernagt()

{  float  sigma=0.0000000567;
   i=93;
   j=151;

/* Not possible to call vappress() directly in case the climate station
   is not in area calculated */
   
   e = 6.1078 * exp(17.08085*tempint[i][j]/(234.175+tempint[i][j])) * hum;

   LWin = sigma * pow((tempint[i][j] + 273.15),4)*(0.52 + 0.065 * sqrt(e/100));

   return;
}


/**********************************************************/
/* FUNCTION  measuredlapserate                            */
/*   CALCULATION OF TEMPERATURE LAPSE RATE FROM DATA FROM 2 STATIONS */
/*   called from new main (lapserate.c) every time step  
     before function 'tempinterpol' */
/*   the temperature of the 2. climate station must be in the
     column 'cloud'   */
/*  for application King George Island  1/2001            */
/* APRIL 2010, THIS OPTION IS IMPLEMENTED VIA INPUT.DAT*/
/**********************************************************/

void measuredlapserate()

{   int row2 = 291;     /*row of 2 climate station*/
    int col2 = 104;
    float  elevationdiff = 170;
    float  tempgradmin   = -1.4;   /*minimum lapse to avoid strange values due to meas errors*/
    float  tempgradmax   = 1.0;

/*TEMPGRAD IS NEGATIVE IF TEMPERATURE DECREASES WITH ELEVATION*/

/*IF ELEVATION DIFFERENCE TAKEN FROM DEM*/
/*  if(griddgm[row2][col2] >= griddgm[rowclim][colclim])
      tempgrad =(cloud-temp)/((griddgm[row2][col2]-griddgm[rowclim][colclim])/100);
    else
      tempgrad =(cloud-temp)/((griddgm[rowclim][colclim]-griddgm[row2][col2])/100); */

/*IF ELEVATION DIFFERENCE GIVEN DIRECTLY*/
    tempgrad = (cloud-temp)/(elevationdiff/100);

/*SET LIMITS TO COMPUTED TEMPERATURE LAPSE RATE*/
    if(tempgrad > tempgradmax)
      tempgrad = tempgradmax;      
    if(tempgrad < tempgradmin)
      tempgrad = tempgradmin;

/*printf("   zeit =%3.0f temp=%8.2f  temp2=%7.2f   el=%7.2f  el2=%8.2f   tempgrad =%7.2f\n",
zeit,temp,cloud,griddgm[rowclim][colclim],griddgm[row2][col2],tempgrad);  */

  return;
}

