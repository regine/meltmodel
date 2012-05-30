/* =========================================================================== */
/*  FILE  disopt.c                                                             */
/*     functions to calculate discharge if discharge or precipitation          */
/*     parameters are optimized                                                */
/*     (not for optimization of melt parameters (ddfopt.c different function)  */
/*     called from main (meltmod.c and degree.c) every time step               */
/*  14.2.1997,  update 7 July 2010 */
/* =========================================================================== */

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

/**************************************************************************/
/*  FUNCTION dischargeopt                                                 */
/*      DISCHARGE and PRECIPITATION CALCULATION FOR DIFFERENT VALUES OF   */
/*      2 OPTIMIATION PARAMETERS                                          */
/*      called from main every time step after melt grid has been calculated */
/**************************************************************************/

void dischargeopt()

{

/*------------------------------------------------------------------------------*/
/*                        OPTIMIZATION HEAD-LOOP  for each timestep             */       

    for (iiopt=1;iiopt<=anzahlopt1;iiopt++)     /*first parameter to optimize*/     
    {                   
 /*-------------------------------------------------------------------------*/       
 /*** COPY NEW VALUE INTO OPTIMIZATION PARAMTER ***/
 /*-------------------------------------------------------------------------*/      
	if (strcmp(optkA ,"firnk3") == 0)        /* first parameter */  
	     firnkons = (iiopt-1)*stepopt1+startopt1;
		
	if (strcmp(optkA, "snowk2") == 0)
	   snowkons = (iiopt-1)*stepopt1+startopt1;  /* increase optimization value no.1*/  

	if (strcmp(optkA, "icek1") == 0)
	   icekons = (iiopt-1)*stepopt1+startopt1;

	if (strcmp(optkA, "qground") == 0)
	   qground = (iiopt-1)*stepopt1+startopt1;

	if (strcmp(optkA, "T0") == 0)
	   T0 = (iiopt-1)*stepopt1+startopt1;

	if (strcmp(optkA, "preccorr") == 0)
	   preccorr = (iiopt-1)*stepopt1+startopt1;

	if (strcmp(optkA, "precgrad") == 0)
	   precgrad = (iiopt-1)*stepopt1+startopt1;
	  
      for (jjopt=1;jjopt<=anzahlopt2;jjopt++)     /* second parameter to optimize */  

      {
	if (strcmp(optkB ,"firnk3") == 0)       /* 2. parameter increased */
	   firnkons = (jjopt-1)*stepopt2+startopt2;

	if (strcmp(optkB, "snowk2") == 0)
	   snowkons = (jjopt-1)*stepopt2+startopt2;       
		
	if (strcmp(optkB, "icek1") == 0)
	   icekons = (jjopt-1)*stepopt2+startopt2;

	if (strcmp(optkB, "qground") == 0)
	   qground = (jjopt-1)*stepopt2+startopt2;

	if (strcmp(optkB, "T0") == 0)
	   T0 = (jjopt-1)*stepopt2+startopt2;

	if (strcmp(optkB, "preccorr") == 0)
	   preccorr = (jjopt-1)*stepopt2+startopt2;

	if (strcmp(optkB, "precgrad") == 0)
	   precgrad = (jjopt-1)*stepopt2+startopt2;


/*------- GRID LOOP - for whole glacier -----------------*/
     /*CALCULATE WATER VOLUMES FOR EACH RESERVOIR FOR EVERY TIME STEP*/

  for (i=firstrow;i<=lastrow;i++)          /* for each row */
    for (j=firstcol[i];j<=lastcol[i];j++)  /* for each col with glacier grids*/
     {         
       if (griddgmdrain[i][j] != nodata)       /*only dgmdrain area*/
	{	 
	 /*if precipitation parameters are optimized rain must be recalculated
	   for any parameter constellation, if not the array calculated before
	   for melt remains constant for one time step and it can be used*/
	  if ((strcmp(optkA, "T0") == 0) ||
	    (strcmp(optkA, "preccorr") == 0) ||
	    (strcmp(optkA, "precgrad") == 0) ||
	    (strcmp(optkB, "T0") == 0) ||
	    (strcmp(optkB, "preccorr") == 0) ||
	    (strcmp(optkB, "precgrad") == 0))
	   {  precipinterpol();       /*correct and interpolate precipitation*/
	      if(onlyglacieryes == 1)
		rainoutsideglac();
	   }  /*endif prec parameter*/

	  negativemelt();      /*set melt to zero, if energy balance negative*/
	  meltrainvol();       /*add melt and rain water*/
	  whichreservoir();    /*sum up water volum for each reservoir*/
		       
	}  /* END IF  DRAIN GRID */
      }    /* END FOR   NEXT GRID CELL */

/*-------- end grid loop - whole grid calculated ------------*/


/*---------------------------------------------------------------------------*/
/***  CALCULATION OF DISCHARGE OF EACH RESERVOIR AND TOTAL DISCHARGE [m3]  ***/
/*---------------------------------------------------------------------------*/

/*DISCHARGE OF EACH AREA (FIRN,SNOW,ICE) OF THE PREVIOUS STEP IS DIFFERENT FOR
  EACH PARAMETER CONSTELLATION, THEREFORE IT MUST BE AN ARRAY, IN ORDER NOT TO
  OVERWRITE THE PREVIOUS DAY VALUE OF THE FIRST PARAMETER CONSTELLATION WITH
  DISCHARGE OF ANOTHER PARAMETER SET */

   qfirnopt[iiopt][jjopt] = qfirnopt [iiopt][jjopt]*3600*timestep;
   qsnowopt[iiopt][jjopt] = qsnowopt[iiopt][jjopt]*3600*timestep;
   qiceopt[iiopt][jjopt] = qiceopt[iiopt][jjopt]*3600*timestep;  /*MUST HERE BE IN m3/timestep!*/  

  /**********************************************************************/          
    qfirnopt[iiopt][jjopt] = 
      (qfirnopt [iiopt][jjopt]*exp(-(timestep/firnkons)) + sumk3 - sumk3*exp(-(timestep/firnkons)));
    qsnowopt[iiopt][jjopt] =
      (qsnowopt[iiopt][jjopt]*exp(-(timestep/snowkons)) + sumk2 - sumk2*exp(-(timestep/snowkons)));
    qiceopt[iiopt][jjopt]  = 
      (qiceopt[iiopt][jjopt]*exp(-(timestep/icekons)) + sumk1 - sumk1*exp(-(timestep/icekons)));
  /**********************************************************************/          

   qfirnopt[iiopt][jjopt] = qfirnopt [iiopt][jjopt]/(3600*timestep);
   qsnowopt[iiopt][jjopt] = qsnowopt[iiopt][jjopt]/(3600*timestep);  /*[m3/s]*/
   qiceopt[iiopt][jjopt] = qiceopt[iiopt][jjopt]/(3600*timestep);

      /*  printf("\n qfirn= %f",qfirn);printf("\nsumk3= %f",sumk3);
	printf("\n qsnow= %f",qsnow);printf("\nsumk2= %f",sumk2);
	printf("\n qice= %f",qice);printf("\nsumk1= %f",sumk1);*/ 

  /*SIMULATED DISCHARGE IN m3/s*/
    qdischber =
     (qfirnopt[iiopt][jjopt] + qsnowopt[iiopt][jjopt] + qiceopt[iiopt][jjopt]) + qground;
    
  /*CUMULATED DISCHARGE VOLUME in m3/s : will be converted into 100000m3 in write2matriz*/
    volumeopt[iiopt][jjopt] = volumeopt[iiopt][jjopt] + qdischber;

/*------------------------------------------------------------------------------*/
/*              summation for r2-CRITERIUM                                      */
/*------------------------------------------------------------------------------*/   
     
       sumr2();     /* SUM UP SUMS FOR r2-CALCULATION*/
       sumr2ln();   /* SUMS FOR ln r2*/
/*-------------------------------------------------------------------------------*/    
		
  /* meltwater integrated over ice, snow, firn area for one parameter constellation*/
	 sumk1 = 0.;        /*ice area */
	 sumk2 = 0.;
	 sumk3 = 0.;
		

   } /*END FOR  - next parameter constellation - 2. parameter*/
     
	if (strcmp(optkB ,"firnk3") == 0)       /* 2. parameter decreased to startvalue*/
	   firnkons = startopt2;

	if (strcmp(optkB, "snowk2") == 0)
	   snowkons = startopt2;       
		
	if (strcmp(optkB, "icek1") == 0)
	   icekons = startopt2;

	if (strcmp(optkB, "qground") == 0)
	   qground = startopt2;       
		
	if (strcmp(optkB, "T0") == 0)
	   T0 = startopt2;

	if (strcmp(optkB, "preccorr") == 0)
	   preccorr = startopt2;       
		
	if (strcmp(optkB, "precgrad") == 0)
	   precgrad = startopt2;


  } /*END FOR - next parameter constellation - 1. parameter*/

  return;
}



/*** =================================================================== ***/    
/***           FUNCTION  write r2-values-matrix to Output-file           ***/
/***                           called from main                          ***/  
/*** =================================================================== ***/
 
void write2matriz()

  {    
     /*WRITE r2-MATRIX*/

       for (iiopt=1;iiopt<=anzahlopt1;iiopt++)    /*FOR EACH ROW = 1.PARAMETER*/
	 {    
	     /* writes first column : value of optimization parameter 1 */
	     fprintf(r2file,"\n%7.2f |",(iiopt-1)*stepopt1+startopt1);
  
	   for (jjopt=1;jjopt<=anzahlopt2;jjopt++)    /*WRITE r2-VALUES ONE ROW*/
	      fprintf(r2file,"%7.4f", r2[iiopt][jjopt]);
	 }  /*NEXT ROW*/


     /*WRITE LOGARITHMIC r2-MATRIX*/

      fprintf(r2file,"\n\n  --- r2 with logarithmic discharges ---");
       for (iiopt=1;iiopt<=anzahlopt1;iiopt++)    /*FOR EACH ROW = 1.PARAMETER*/
	 {    
	     /* writes first column : value of optimization parameter 1 */
	     fprintf(r2file,"\n%7.2f |",(iiopt-1)*stepopt1+startopt1);
  
	   for (jjopt=1;jjopt<=anzahlopt2;jjopt++)    /*WRITE r2-VALUES ONE ROW*/
	      fprintf(r2file,"%7.4f",r2ln[iiopt][jjopt]);
	 }  /*NEXT ROW*/



     /*WRITE DISCHARGE VOLUMES (cumulated over whole period [m3*100000]*/
      fprintf(r2file,"\n\n sum of calc discharge [100000*m3] \n ");

     for (iiopt=1;iiopt<=anzahlopt1;iiopt++)        /*cumulated m3/s --> 100000 m3*/
      for (jjopt=1;jjopt<=anzahlopt2;jjopt++)
	volumeopt[iiopt][jjopt] = volumeopt[iiopt][jjopt]*3600*timestep/100000; 

     for (iiopt=1;iiopt<=anzahlopt1;iiopt++)
      {   fprintf(r2file,"\n%7.2f |",(iiopt-1)*stepopt1+startopt1);
	  for (jjopt=1;jjopt<=anzahlopt2;jjopt++)
	    fprintf(r2file,"%8.3f", volumeopt[iiopt][jjopt]);
      }
    
      fprintf(r2file,"\n\n measured discharge volume [in 100000 m3] = %10.4f\n",volumemeas);

      fclose(r2file);
      r2file = NULL;            

  return;
 }


 


