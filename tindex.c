/*================================================================*/
/* FILE tindex.c                                                  */
/* This file contains all functions which are needed              */
/* to calculate melt by temperature index methods and             */
/* discharge functions needed if melt parameters are optimized    */
/*  14.2.1996;  update 7 July 2010 */
/*================================================================*/

#include "function.h"
#include "variabex.h"   /* all global VARIABLES, externally declared */


/*****************************************************************/
/* FUNCTION  degreestart
      to avoid that grid output files are requested that can
      not be created if melt is not calculated by energy balance */
/*****************************************************************/

void degreestart()

{
  dir2yes= 0;   difyes = 0;
  albyes = 0;   gloyes = 0;   
  swbyes = 0;   linyes = 0;   loutyes = 0;
  netyes = 0;   senyes = 0;
  latyes = 0;   balyes = 0;   rainyes = 0;
  ablyes = 0;   surftempyes = 0;

  methodglobal = 1;    /*to avoid array reservation for DIRECT2 and DIFFUS*/
  methodlongin = 1;    /*to avoid array reservation for LONGIN */
 
 return;
}


/*****************************************************************/
/*  FUNCTION  temppos                                            */
/*****************************************************************/


void temppos()

{
  if(tempint[i][j] > 0)
    TEMPOS[i][j] = tempint[i][j];
  else
    TEMPOS[i][j] = 0.;

}

/******************************************************************/
/*  FUNCTION  ddfcalculation                                      */
/*    calculate the degree day factor as the ratio between        */
/*      calculated melt and positive temperature                  */
/******************************************************************/

void ddfcalculation()

{
  if(tempint[i][j] > 0)
    DDFCALC[i][j] = MELT[i][j]/TEMPOS[i][j]*(24/timestep);
  else
    DDFCALC[i][j] = 0.;

}


/******************************************************************/
/*  FUNCTION  degreedaymelt
     method 1
     reduced melt over debris on glacier possible
     calculating melt using a simple degree day method            */
/******************************************************************/

void degreedaymelt()

{ int inti;
  inti=(int)surface[i][j];

  switch(inti)     /*MELT IS IN MM/TIMESTEP*/
   { case 1 :   /*snow surface*/
               MELT[i][j] = DDFsnow/24*timestep * tempint[i][j];
               break;        
     case 2 :   /*slush surface*/
               MELT[i][j] = DDFsnow/24*timestep * tempint[i][j];
               break;
     case 3 :   /*ice surface*/
               MELT[i][j] = DDFice/24*timestep * tempint[i][j];
               break;
     case 4 :  MELT[i][j] = 0;       /*rock surface*/
               break;
                /*melt reduced over debris cover on glacier*/
     case 5 :  MELT[i][j] = DDFice/24*timestep * debrisfactor * tempint[i][j];
               break;
                
     default : printf("\nError in surface file\n function ddmeltsnowice\n\n");
               printf("\n surf=%f  i=%d  j=%d \n",surface[i][j],i,j);
	       exit(10);    
   }  /*end switch*/


 if ((tempint[i][j] < 0.0) && (methodsurftempglac != 4))   /*snowmodel not run*/
       MELT[i][j] = 0.0;    
   /*melt must not be set to 0 if snow model is run, 
     negative energy needed for cooling surface*/

 return;
}


/******************************************************************/
/*  FUNCTION   dd_directmelt                                      */
/*     method 2                                                   */
/*     including direct radiation in melt calculation             */
/******************************************************************/

void  dd_directmelt()

{

 int inti;

  inti=(int)surface[i][j];

  switch(inti)
   { case 1 :   /*snow surface*/
               MELT[i][j] = (meltfactor/24 + radfactorsnow/1000*DIRECT[i][j])
                            *tempint[i][j]*timestep;
               break;        
     case 2 :   /*slush surface*/
               MELT[i][j] = (meltfactor/24 + radfactorsnow/1000*DIRECT[i][j])
                            *tempint[i][j]*timestep;
               break;
     case 3 :   /*ice surface*/
               MELT[i][j] = (meltfactor/24 + radfactorice/1000*DIRECT[i][j])
                            *tempint[i][j]*timestep;
               break;
     case 4 :  MELT[i][j] = 0;       /*rock surface*/
               break;
   /* MELT REDUCTION OVER DEBRIS ON GLACIERS*/
   /*NEW NOV 1999*/
     case 5 :  MELT[i][j] = (meltfactor/24 + radfactorice/1000*DIRECT[i][j])
                            *tempint[i][j]*debrisfactor*timestep;
	       break;
     default :  printf("\nError in surface file\n function ddradsnowicetemp\n\n");
	exit(10);    
   }  /*end switch*/

 if ((tempint[i][j] < 0.0) && (methodsurftempglac != 4))   /*snowmodel not run*/
       MELT[i][j] = 0.0;    
   /*melt must not be set to 0 if snow model is run, 
     negative energy needed for cooling surface*/

 return;
}

/******************************************************************/
/*  FUNCTION  dd_directglobal                                     */
/*     method 3                                                   */
/*     direct radiation term reduced under cloudier conditions    */
/******************************************************************/

void dd_directglobal()

{  
    /*ratio is the ratio of global and direct radiation at the climate station
      and it is considered as a measure of cloudiness; high cloudiness
      implies a small ratio and thus the radiation term below is reduced*/

 int inti;

  inti=(int)surface[i][j];

  switch(inti)
   { case 1 :   /*snow surface*/
               MELT[i][j] = (meltfactor/24 + radfactorsnow/1000*DIRECT[i][j]*ratio)
                            *tempint[i][j]*timestep;
               break;        
     case 2 :   /*slush or firn surface*/
               MELT[i][j] = (meltfactor/24 + radfactorsnow/1000*DIRECT[i][j]*ratio)
                            *tempint[i][j]*timestep;
               break;
     case 3 :   /*ice surface*/
               MELT[i][j] = (meltfactor/24 + radfactorice/1000*DIRECT[i][j]*ratio)
                            *tempint[i][j]*timestep;
               break;
     case 4 :  MELT[i][j] = 0;       /*rock surface*/
               break;

     case 5 :  MELT[i][j] = (meltfactor/24 + radfactorice/1000*DIRECT[i][j]*ratio)
                            *tempint[i][j]*debrisfactor*timestep;
	       break;

     default :  printf("\nError in surface file\n function ddradcloudreduction\n\n");
	exit(10);    
   }  /*end switch*/


 if ((tempint[i][j] < 0.0) && (methodsurftempglac != 4))   /*snowmodel not run*/
       MELT[i][j] = 0.0;    
   /*melt must not be set to 0 if snow model is run, 
     negative energy needed for cooling surface*/

 return;
}



/***************************************************************************/
/* FUNCTION  snowcoverdegree                                               */
/*     calculate water equivalent of snow cover after melt or snow fall    */
/*     called from main for every time step and each grid cell             */
/***************************************************************************/

void snowcoverdegree()
       /*must be a different function than for energy balance model, because
         the array abla is not defined for temp index model*/
       /* SNOW is in cm, melt und snowprec are in mm */
 {
     SNOW[i][j] = SNOW[i][j] - MELT[i][j]/10 + snowprec/10;
      if(SNOW[i][j] < 0)       /*snow cover has melted away*/
         SNOW[i][j] = 0;
    
  return;
 }




/*======================================================================*/
/*  FUNCTIONS FOR DISCHARGE CALCULATION                                 */
/******************************************************************/
/*  FUNCTION startdisddfopt                                       */
/*     DISCHARGE CALCULATION IF MELT PARAMETER OPTIMIZATION       */
/*     called once from main to initialize optimization run       */
/*     only optimization of melt parameters                       */
/*================================================================*/

void startdisddfopt()

{
     /*function are in discharg.c*/
   startarraysdis();     /*RESERVE STORAGE FOR ARRAYS FOR r2-SUMS*/
   startarraysopt();     /*ADDITIONAL ARRAYS ONLY FOR OPTIMIZATION*/
						      
 /*-------------------------------------------*/  
 /*** OPENING R2-MATRIX OUTPUTFILE          ***/
 /*-------------------------------------------*/

  strcpy(dummy,outpath);         
  strcat(dummy,namematrix);    

   if ((r2file = fopen(dummy,"wt")) == NULL)
    {   printf("\n ERROR in opening r2-matrix output file \n\n");
		   exit(4);
    }  /* END IF */

    /* write to matrix output file */
  fprintf(r2file,"\noutput path :  %s\n",outpath);
  fprintf(r2file," temperature index method : %3d\n",ddmethod);
  fprintf(r2file,"\n---------------------------------------------------\n");
  fprintf(r2file,"optimizationparameter no.1:  %s\n",optkA);
  fprintf(r2file,"optimizationparameter no.2:  %s\n",optkB);
  fprintf(r2file,"---------------------------------------------------\n");
  fprintf(r2file,"starting day=%4.0f     last day=%4.0f\n\n",jdbeg,jdend);

  fprintf(r2file,"temperature gradient              = %5.2f\n",tempgrad);
  fprintf(r2file,"treshtemp = %5.2f\n",T0);
  fprintf(r2file,"preccorr  = %5.2f  ",preccorr);
  fprintf(r2file,"    precgrad = %5.2f\n",precgrad);
  fprintf(r2file,"startratio (glob/direct at station) = %5.2f\n\n",startratio);

  if(disyes == 1)
   { fprintf(r2file,"start values for discharge of each area: \n");
     fprintf(r2file,"   qfirn =  %6.3f    qsnow = %6.3f    qice = %6.3f\n",
		     qfirnstart,qsnowstart,qicestart);
     fprintf(r2file,"recession constants for\n");
     fprintf(r2file,"   firn = %5.2f    snow = %5.2f    ice = %5.2f\n\n",
		 firnkons,snowkons,icekons);
   }

  fprintf(r2file,"if not optimised :\n");
  if(ddmethod == 1)
    fprintf(r2file,"  DDFice = %6.2f  DDFsnow = 6.2f\n",DDFice,DDFsnow);
  if(ddmethod >= 2)
    fprintf(r2file,"  meltfactor = %6.2f radfactorice = %6.2f radsnow = %6.2f\n\n",
          meltfactor,radfactorice,radfactorsnow);
 
  fprintf(r2file,"start optA=%4.1f    step optA =%5.2f  number optA = %3d\n",
	startopt1,stepopt1,anzahlopt1);
  fprintf(r2file,"start optB=%4.1f    step optB =%5.2f  number optB = %3d\n\n",
	startopt2,stepopt2,anzahlopt2);


  fprintf(r2file,"\n       optimization-parameter %s\n      ",optkB);
  for (jjopt=1;jjopt<=anzahlopt2;jjopt++)
      fprintf(r2file,"%7.2f",(jjopt-1)*stepopt2+startopt2);                                         fprintf(r2file,"\n______________________________________________________________\n");

 return;

}    /*END startddfopt()*/



/****************************************************************************/
/*   FUNCTION dischargeddfopt                                               */
/*         DISCHARGE CALCULATION                                            */
/*   called from main for every time step and every parameter constellation */
/****************************************************************************/

void dischargeddfopt()   

{
   for (i=firstrow;i<=lastrow;i++)            /* for each row */
       for (j=firstcol[i];j<=lastcol[i];j++)  /* for each col with drainage grids*/
	{         
	  if (griddgmglac[i][j] != nodata)       /*only glacierized area*/
	    {
	      	      	      
/*WENN MIT INITIAL SNOW GERECHNET WIRD, BRAUCHT MAN VERSCHIEDENE
  SNOW GRIDS FUER JEDE PARAMETER CONSTELATION */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

	      negativemelt(); 
	      meltrainvol();
	      whichreservoir();                                 
		       
	     }   /* END IF  GLACIERIZED GRID */
	}   /* END FOR   NEXT GRID POINT */


/*---------------------------------------------------------------------------*/
/***     CALCULATION OF DISCHARGE FOR ALL THREE AREAS [m3]  ***/
/*          only if time step = 1 hour !!! */
/*---------------------------------------------------------------------------*/

/*DISCHARGE OF EACH AREA (FIRN,SNOW,ICE) OF THE PREVIOUS STEP IS DIFFERENT FOR
  EACH PARAMETER CONSTELLATION, THEREFORE IT MUST BE AN ARRAY, IN ORDER NOT TO
  OVERWRITE THE PREVIOUS DAY VALUE OF THE FIRST PARAMETER CONSTELLATION WITH
  DISCHARGE OF ANOTHER PARAMETER SET */
/*convert discharge in m3/h for discharge formula below*/

	qfirnopt[iiopt][jjopt] = qfirnopt[iiopt][jjopt]*3600*timestep;  
	qsnowopt[iiopt][jjopt] = qsnowopt[iiopt][jjopt]*3600*timestep;  
	qiceopt[iiopt][jjopt]  = qiceopt[iiopt][jjopt]*3600*timestep;

      /**********************************************************************/          
	qfirnopt[iiopt][jjopt] = (qfirnopt[iiopt][jjopt]*exp(-(1/firnkons)) + 
				 sumk3 - sumk3*exp(-(1/firnkons)));
	qsnowopt[iiopt][jjopt] = (qsnowopt[iiopt][jjopt]*exp(-(1/snowkons)) + 
				 sumk2 - sumk2*exp(-(1/snowkons)));
	qiceopt[iiopt][jjopt]  = (qiceopt[iiopt][jjopt]*exp(-(1/icekons)) + 
				 sumk1 - sumk1*exp(-(1/icekons)));
      /**********************************************************************/          
/*CALCULATION OF AREAWATER/TIMESTEP [m3/s]*/
	qfirnopt[iiopt][jjopt] = qfirnopt[iiopt][jjopt]/(3600*timestep);
	qsnowopt[iiopt][jjopt] = qsnowopt[iiopt][jjopt]/(3600*timestep);
	qiceopt[iiopt][jjopt] = qiceopt[iiopt][jjopt]/(3600*timestep);

	  qdischber = (qfirnopt[iiopt][jjopt] + qsnowopt[iiopt][jjopt] + 
		      qiceopt[iiopt][jjopt]) + qground;
  
	  volumeopt[iiopt][jjopt] = volumeopt[iiopt][jjopt] + qdischber;
  
/*------------------------------------------------------------------------------*/
/*              r2-CRITERIUM FOR STATISTICS                                     */
/*------------------------------------------------------------------------------*/   
    sumr2();
    sumr2ln();      /*logarithmic r2*/

/*-------------------------------------------------------------------------------*/    
    
	sumk1 = 0.;     /*area sums for reservoirs of firn, snow, ice*/ 
	sumk2 = 0.;     /*must be set to 0 before next parameter set*/
	sumk3 = 0.;
     
  return;
}




/*======================================================================*/
/*  FUNCTIONS FOR COUPLING SNOW MODEL TO TEMP-INDEX METHOD              */
/******************************************************************/
/*  FUNCTION melt2energy                                          */
/*     convert temp-index determined melt into energy W/m2        */
/*     to be used for forcing in subsurf()                        */
/*================================================================*/

void melt2energy()

{  double source;
   float Lf=1;
   
   source = MELT[i][j]*Lf*factinter/(3600*timestep);

 return;
}




