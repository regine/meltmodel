
/*==========================================================================*/
/*  FILE discharg.c                                                         */
/*     function startdischarg for initialization of discharge calculations  */
/*       (for both sim and optimization of discharge or precip parameters)  */
/*     function discharge for discharge calculation (only simulation run)   */
/*     function writedisoutput to print simulated discharge to output file  */
/*  31.3.1997,  update 18 Nov 2011 */
/*==========================================================================*/

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

/*=========================================================================*/
/* FUNCTION TO INITIALIZE DISCHARGE SIMULATION OR OPTIMIZATION OF K-VALUES */
/*     called from startdischarg (below)  */




/**************************************************************************/
/*   FUNCTION  opendiscalc                                                */
/*        OPENING DISCHARGE OUTPUTFILE                                    */
/*        called once from startdischarg, only if simulation run          */
/**************************************************************************/

void opendiscalc()

 {
   strcpy(dummy,outpath);         /* definition of outpath */
   strcat(dummy,nameqcalc);    /* copies name of outputfile to path */

     if ((qcalcfile = fopen(dummy,"wt")) == NULL)
	 {  printf("\n ERROR in opening discharge output file \n\n");
	    exit(4);
	 }  /* END IF */
      
  fprintf(qcalcfile," year  jd      time   qmeas     qcalc   qfirn    qsnow     qice  qrock");
  fprintf(qcalcfile," qground  cumvolmeas cumvolcalc cumdiffc-m\n");

 return;

 }


/**************************************************************************/
/*  FUNCTION startarraysdis                                               */
/*      STORAGE RESERVATION (dynamical) AND INITIALIZATION                */
/*      needed for all discharge calculations (meltmod, degree, ddfopt)   */
/*      called once from startdischarg (below) and startddfopt            */
/*      called only if discharge data available (disyes = 1)              */
/**************************************************************************/

void startarraysdis()

 {
   /*for simulation runs variable as arrays not needed (only one sum each), but
     here used for simplicity to avoid new variables (arrays needed for optimization) */
 
      /*** plus one probably not needed */
   f2      = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   sumf0x  = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   sumf0x2 = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   r2      = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   f02     = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);

      /*** plus one probably not needed */
   f2ln      = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   sumf0xln  = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   sumf0x2ln = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   r2ln      = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   f02ln     = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);

     /*in case of simulation only one parameter constellation*/
   if((disyesopt==0) && (ddfoptyes==0))
    {  anzahlopt1 = 1;
       anzahlopt2 = 1;
    }

   for (iiopt=1;iiopt<=anzahlopt1;iiopt++) 
     for (jjopt=1;jjopt<=anzahlopt2;jjopt++) 
       {    /*for simumlation run no arrays needed, but for simplicity
	      no new variables declared */
	  f2[iiopt][jjopt] = 0.0;      
	  sumf0x[iiopt][jjopt] = 0.0;
	  sumf0x2[iiopt][jjopt] = 0.0;    
	  f02[iiopt][jjopt] = 0.0;
	  r2[iiopt][jjopt] = 0.0;

	  f2ln[iiopt][jjopt] = 0.0;      
	  sumf0xln[iiopt][jjopt] = 0.0;
	  sumf0x2ln[iiopt][jjopt] = 0.0;    
	  f02ln[iiopt][jjopt] = 0.0;
	  r2ln[iiopt][jjopt] = 0.0;
	}
      
    /*must be set to 1, because in case of simulation run it is 2, after the
      loop and 1 is needed in r2calc*/
  iiopt=1;
  jjopt=1;

  return;
 }


/**************************************************************************/
/*  FUNCTION startarraysopt                                               */
/*     storage reservation and initialization of additional arrays        */
/*     needed for optimization run (both optimitization of k-values       */
/*     (meltmod, degree) and of melt parameters (ddfopt.c)                */
/**************************************************************************/

void startarraysopt()

 {
      /*for every parameter constellation the previous day discharge */
      /* of the 3 reservoirs must be known for the next time step */
   qfirnopt= matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   qsnowopt= matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);
   qiceopt = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);  
   qrockopt = matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);  
   volumeopt= matrixreservdouble(1,(long)anzahlopt1,1,(long)anzahlopt2);

   for (iiopt=1;iiopt<=anzahlopt1;iiopt++) 
    for (jjopt=1;jjopt<=anzahlopt2;jjopt++)  
     {
       qfirnopt[iiopt][jjopt] = (double)qfirnstart;
       qsnowopt[iiopt][jjopt] = (double)qsnowstart;
       qiceopt[iiopt][jjopt] = (double)qicestart;  
       qrockopt[iiopt][jjopt] = (double)qrockstart;  

	volumeopt[iiopt][jjopt] = 0.0; 
	   /*for simulation run different variable names because
	     no arrays needed*/
      }  /*end for*/
	      
  return;
 }


/**************************************************************************/
/*  FUNCTION openr2matrizkopt                                             */
/*      OPEN R2-MATRIX OUTPUTFILE AND WRITE HEAD                          */
/*      only for optimization k-values                                    */
/**************************************************************************/

void openr2matrizkopt()

 {
    strcpy(dummy,outpath);         /* definition of outpath */
    strcat(dummy,namematrix);    /* copies name of outputfile to path */

      if ((r2file = fopen(dummy,"wt")) == NULL)
	{  printf("\n ERROR in opening r2-matrix output file \n\n");
	   exit(4);
	}  /* END IF */

     /* write k-values of the first row */
  fprintf(r2file,"\noptimizationparameter no.1:  %s\n",optkA);
  fprintf(r2file,"optimizationparameter no.2:  %s\n\n",optkB);
  fprintf(r2file,"simulation day : %4.0f - %4.0f\n",jdbeg,jdend);

  fprintf(r2file,"name of firndatafile: %s\n\n",namedgmfirn);
  fprintf(r2file,"difference between simulation and r2-optimization: %4.1f\n", 
		  jdstartr2diff);
  fprintf(r2file,"Threshold temperature rain - snow = %5.2f\n",T0);
  fprintf(r2file,"precipitation-correction          = %5.2f\n",preccorr);
  fprintf(r2file,"precipitation gradient            = %5.2f\n",precgrad);
  fprintf(r2file,"temperature gradient              = %5.2f\n",tempgrad);
  fprintf(r2file,"startratio (glob/direct at station) = %5.2f\n\n",startratio);

  fprintf(r2file,"\nqground= %5.2f\t\n",qground);
   
  fprintf(r2file,"\nrecession constant firn : %5.1f\n",firnkons);
  fprintf(r2file,"recession constant snow : %5.1f\n",snowkons);
  fprintf(r2file,"recession constant ice  : %5.1f\n\n",icekons);
  fprintf(r2file,"recession constant rock  : %5.1f\n\n",rockkons);

  fprintf(r2file,"startvalues for simulation (m3/s):\n\t qfirnstart =%6.2f\n",qfirnstart);
  fprintf(r2file,"\t qsnowstart =%7.2f\n\t qicestart =%7.2f\n\t qrockstart=%7.2f\n\n",qsnowstart,qicestart,qrockstart);  
  
  fprintf(r2file,"\n       optimization-parameter %s\n      ",optkB);
  for (jjopt=1;jjopt<=anzahlopt2;jjopt++)
      fprintf(r2file,"%9.2f",(jjopt-1)*stepopt2+startopt2);                                   
      fprintf(r2file,"\n___________________________________________________________________");

  return;
 }



/*===============================================================================*/
/**************************************************************************/
/*   FUNCTION  startdischarg                                              */
/*    called once from main of meltmod, degree                            */
/*       for both simulation and optimization of discharge parameters     */
/*       not applicable for ddfopt.c (different function)                 */
/*     calls functions to initialize discharge simulation                 */
/*       OPENS THE FILE OF FIRNAREA, DISCHARGE OUTPUTFILE,                */
/*       MEASURED DISCHARGE FILE                                          */
/**************************************************************************/  

void startdischarg()

{
 if(disyes == 1)        /*only if measured data file available*/    
   startarraysdis();     /*RESERVE STORAGE FOR ARRAYS FOR r2-SUMS*/  
  
 volumemeas = 0.0;     /*CUMULATED MEASURED DISCHARGE VOLUME*/

 if(disyesopt == 0)    /*SIMULATION RUN, NO OPTIMIZATION OF k-VALUES*/
  { 
     opendiscalc();    /*OPEN DISCHARG OUTPUT FILE*/

      /*Initialisation of the different q's to the starting values*/
      /*in case of k-optimization these variables must be arrays*/
     qfirnsim = qfirnstart;   /*discharge firn area of previous time step*/
     qsnowsim = qsnowstart;
     qicesim = qicestart;
     qrocksim = qrockstart;   /*area outside glacier if not snow-covered*/

     volumesim  = 0.0;     /*Initialisation of discharge summation */

  }
  else                 /*OPTIMIZATION OF k-VALUES*/
  {
    startarraysopt();      /*ADDITIONAL ARRAYS NEEDED ONLY FOR OPTIMIZAITON */
    openr2matrizkopt();    /*OPEN FILE WITH MATRIZ OF R2-VALUES*/
  }

 return;
}



/*===============================================================================*/
/*  CALCULATION OF DISCHARGE  */
/*      called from function discharge (below) every time step */


/**************************************************************************/
/*  FUNCTION  rainoutsideglac                                             */
/*    distributes rain falling outside the calculated area (e.g. glacier) */
/*    onto the glacier, in order to route rain, which falls outside the   */
/*    glacier through the glacier, this is only necessary, if the area    */
/*    calculated is smaller than the drainage basin                       */
/*      called from main every time step after grid computed              */
/**************************************************************************/

void rainoutsideglac()

 {    
  for (i=firstrow;i<=lastrow;i++)          /* for each row */
    for (j=firstcol[i];j<=lastcol[i];j++)  /* for each col with drain grids*/         
     {  if (griddgmglac[i][j] != nodata)       /*only calculated area*/
   /*-----------------------------------------------------------------------*/
   RAIN[i][j] = RAIN[i][j] + RAIN[i][j]/glacierpart*(100.0-glacierpart);         
 /*-----------------------------------------------------------------------*/      
       /* correction caused by the fact, that rain falls also outside the glacier
	 -> water is spread over the glacier for discharge routing */
     }
     
  return;
 }


/**************************************************************************/
/*  FUNCTION  negativemelt                                                */
/*      set melt water to zero, if energy balance is negative             */
/*      called from discharge (below)                                     */
/**************************************************************************/

void negativemelt()
 {   
    if ((MELT[i][j]) <= 0.)
       meltwat = 0.;            /* methode Oerlemans */     
    else
       meltwat = (MELT[i][j]);

  return;
 }


/**************************************************************************/
/*  FUNCTION  meltrainvol                                                 */
/*    add melt and rain water and convert to volume                       */
/*    called from discharge (below) and dischargeopt for each drainage grid cell */
/**************************************************************************/

void meltrainvol()
 {
    /*------------------------------------------------------------------*/
	totalwat = pow(gridsize,2) * (meltwat + RAIN[i][j])/1000;
	   /*totalwat is "waterproduction" in [m3] per timestep for one grid cell (drainage basin)*/
    /*------------------------------------------------------------------*/
 }


/**************************************************************************/
/*  FUNCTION  whichreservoir                                              */
/*      in which reservoir (firn, snow, ice, rock) is the grid cell located */
/*      called from discharge (below) for each grid cell                  */
/*   rock is area outside glacier but not snow-covered, if snow-covered   */
/*   belongs to snow reservoir, only relevant if drainage basin larger than glacier */
/**************************************************************************/

void whichreservoir()

 {   int  inti;

   /*-------------------------------------------------------*/
   /* SUM UP OF VOLUME OF RESPECTIVE RESERVOIR              */
   /*-------------------------------------------------------*/    
								    
   if ((FIRN[i][j] != nodata) && (FIRN[i][j] > 0))
     /*firn is any number > 0*/
     /*area inside glacier, 0 = rest of glacier outside firnarea*/
     /*firn file missing value outside glacier*/
       sumk3 = sumk3 + totalwat;     /*firn area*/
		  
   else      /*outside firnarea*/
    { inti=(int)surface[i][j];     /*surface type allocated in whichsurface() in radiat.c*/

         switch(inti)
            {  case 1:  sumk2 = sumk2 + totalwat;    /*snow surface*/
                        break;
               case 2:  sumk2 = sumk2 + totalwat;    /*slush surface*/
          /*surface 2 can have 2 meanings: slush or firn. It is firn if albedo*/
          /* is generated internally, but case 2 here is only called if*/
          /* 2 means slush (see paper Hock+Noetzli, 1997*/
                        break;
               case 3:  sumk1 = sumk1 + totalwat;    /*ice surface*/
                        break;
               case 4:  sumk4 = sumk4 + totalwat;    /*rock treated as ice surface*/                
                  /*rock is gridcell outside glacier but not snow-covered*/
                  /*NEW 2011: treated as separate reservoir with own storage constant*/
			            break;
	           case 5:  sumk1 = sumk1 + totalwat;    /*debris treated as ice surface*/
            }   /*end case*/
    }  /*endelse*/
      	
  return;
 }


/*****************************************************************************/
/*  FUNCTION  sumr2                                                          */
/*     sum up sums required for r2 calculations (simulation and optimization */
/*      called from discharge() (below),    only if discharge data available */
/*****************************************************************************/

void sumr2()

 {
      /*SUMATION FOR r2 ONLY IF DISCHARGE DATA AVAILABLE,
	IT CAN BE STARTED AFTER A CERTAIN NUMBER OF DAYS, BECAUSE MIGHT
	BE HIGH AT THE BEGINING DUE TO UNKNOWN START VALUES*/    
    
    if (qdischgem != nodis) 
     {  
	  qdischdiff =   qdischber - qdischgem;    /*calculated minus measured*/

       /*CUMULATED DISCHARGE DIFFERENCE VOLUME ONLY IF SIMULATION RUN, FOR OUTPUT*/
	 if((disyesopt==0) && (ddfoptyes==0))
	   qdischdiffsum = qdischdiffsum + qdischdiff;
	       /*initialized in variab.h*/

       /*ARRAYS NOT NEEDED FOR SIMULATION, NEW VARIABLE AVOIDED */      

       /* Sum differences (meas-calc) squared */
	   f2[iiopt][jjopt] = f2[iiopt][jjopt] + pow(qdischdiff,2);  
   
       /* Sum measured discharge */
	   sumf0x[iiopt][jjopt] = sumf0x[iiopt][jjopt] + qdischgem;

       /*Sum measured squared */
	   sumf0x2[iiopt][jjopt] = sumf0x2[iiopt][jjopt] + pow(qdischgem,2);
      
             /*  printf("sumf0x2=%5.2f\n",sumf0x2[iiopt][jjopt]);  */

      } /*END IF r2diff-calculation*/    
 
  return;
 } 


/*****************************************************************************/
/*  FUNCTION  sumr2ln                                                        */
/*     sum up sums required for logarithmic r2 calculations                  */
/*      called from discharge() (below),    only if discharge data available */
/*****************************************************************************/

void sumr2ln()

 {
    if(qdischgem != nodis)		  /* March 2006*/

     {  
	  qdischdiffln = log(qdischber) - log(qdischgem);    /*calculated minus measured*/

       /*ARRAYS NOT NEEDED FOR SIMULATION, NEW VARIABLE AVOIDED */      
       /* Sum differences (meas-calc) squared */
	   f2ln[iiopt][jjopt] = f2ln[iiopt][jjopt] + pow(qdischdiffln,2);  
   
       /* Sum measured discharge */
	   sumf0xln[iiopt][jjopt] = sumf0xln[iiopt][jjopt] + log(qdischgem);

       /*Sum measured squared */
	   sumf0x2ln[iiopt][jjopt] = sumf0x2ln[iiopt][jjopt] + pow(log(qdischgem),2);
   
	  /* printf("sumf0x2ln=%5.2f\n",sumf0x2ln[iiopt][jjopt]); */

      } /*END IF r2diff-calculation*/    
 
  return;
 } 



/**************************************************************************/
/*  FUNCTION  writedisoutput                                              */
/*      write simulated discharge to file                                 */
/*      called from discharge() (below), only for simulation run          */
/**************************************************************************/

void writedisoutput()

 {
    float diffvolsimmeas;
    
    if(disyes == 2) 
      qdischgem = nodis;
    if(qdischgem != nodis)
      diffvolsimmeas = volumesim-volumemeas;
         /*THIS QUANTITY ONLY MAKES SENSE IF THE PERIOD OF MELT CALC COINCIDES
           WITH THE PERIOD WITH VALID MEASURED DISCHARGE DATA*/
    else
      diffvolsimmeas = nodis;

if(qdischgem == nodis)
  {   
      volumemeas     = 0;
      diffvolsimmeas = 0;
   }						     

 /*for Grapher, midnight transformed to next day to get a continous time series
   write jd2 to output */
	jd2 = jd;
	if(timestep != 24)     /*not for daily timesteps*/
	   if (jd == (int)jd)  jd2 = jd + 1;

 /* WRITE ONE LINE TO OUTPUT FILE */
   fprintf(qcalcfile,"%5.0f %6.2f %6.1f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f", 
      year,jd2,zeit,qdischgem,qdischber,qfirnsim,qsnowsim,qicesim,qrocksim,qground);
   fprintf(qcalcfile," %.4f %.4f %.4f\n",
      volumemeas,volumesim,diffvolsimmeas);

  return;
 }


/*===============================================================================*/
/**************************************************************************/
/*   FUNCTION  discharge                                                  */
/*    called every time step from main of meltmod, degree                 */
/*    only simulation runs, no optimization of k-values or melt parameters */
/**************************************************************************/

void discharge()                  /*called each time step from main */

{

  for (i=firstrow;i<=lastrow;i++)          /* for each row */
    for (j=firstcol[i];j<=lastcol[i];j++)  /* for each col with drain grids*/
     {         
       if (griddgmdrain[i][j] != nodata)       /*only calculated area*/
  	   {
     
    /*melt cannot be negative for temp index method, but function is needed to
      rename the variable MELT into meltwat, which is needed in next function*/
	     negativemelt();      /*set melt to zero, if energy balance negative*/

	     meltrainvol();       /*add melt and rain water*/
	     whichreservoir();    /*sum up water volume for each reservoir*/
		       
	    }  /* END IF  DRAIN GRID */
      }    /* END FOR   NEXT GRID CELL */

/*---------------------------------------------------------------------------*/
/***     CALCULATION OF DISCHARGE OF EACH RESERVOIR AND TOTAL DISCHARGE      */
/*---------------------------------------------------------------------------*/
   /*convert discharge in m3/timestep for discharge formula below*/
	qfirnsim = qfirnsim*3600*timestep;
	qsnowsim = qsnowsim*3600*timestep;  
	qicesim = qicesim*3600*timestep;
	qrocksim = qrocksim*3600*timestep;    /*NEW Nov 2011*/

	qfirnsim = (qfirnsim*exp(-(timestep/firnkons)) + sumk3 - sumk3*exp(-(timestep/firnkons)));
	qsnowsim = (qsnowsim*exp(-(timestep/snowkons)) + sumk2 - sumk2*exp(-(timestep/snowkons)));
	qicesim  = (qicesim*exp(-(timestep/icekons)) + sumk1 - sumk1*exp(-(timestep/icekons)));
	qrocksim  = (qrocksim*exp(-(timestep/rockkons)) + sumk4 - sumk4*exp(-(timestep/rockkons)));

	qfirnsim = qfirnsim/(3600*timestep);  /*CALCULATION OF AREAWATER/TIMESTEP*/
	qsnowsim = qsnowsim/(3600*timestep);  /*[m3/s]*/
	qicesim = qicesim/(3600*timestep);
    qrocksim = qrocksim/(3600*timestep);

      /*  printf("\n qfirn= %f",qfirnsim);printf("\nsumk3= %f",sumk3);
	printf("\n qsnow= %f",qsnowsim);printf("\nsumk2= %f",sumk2);
	printf("\n qfirn= %f",qfirnsim);printf("\nsumk1= %f",sumk1);   */

	  qdischber = (qfirnsim + qsnowsim + qicesim + qrocksim) + qground;     /* in m3/s */

  /* printf("\n sim sumk3 %f ",sumk3);   printf("\n sim sumk1 %f ",sumk1);
     printf("\n sim abfl %f ",qdischber); */
		   
     /*SUMMATION OF CUMULATIVE DISCHARGE VOLUMES [m3*100000] for writing to output*/    
	  volumesim = volumesim + qdischber*timestep*3600/100000; 
/*-------------------------------------------------------------------------------*/    
    if(disyes == 1)      /*only if discharge data available, i.e. if not 2*/
     {  sumr2();
        sumr2ln();
     } 
     writedisoutput();   /*write data of one time step to output file*/

/*-------------------------------------------------------------------------------*/    
    
	sumk1 = 0.;     /*area sums of total water of firn,snow,ice reservoirs */ 
	sumk2 = 0.;     /* must be set to 0 after each timestep */
	sumk3 = 0.;
    sumk4 = 0.;

  return;
 }



/**************************************************************************/
/* FUNCTION r2cal CALCULATES THE r2-CRITERIUM                             */
/*     same function for simulation and optimization                      */
/**************************************************************************/

void r2calc()
	
 {
   for (iiopt=1;iiopt<=anzahlopt1;iiopt++)
     for (jjopt=1;jjopt<=anzahlopt2;jjopt++)
      {
	    f02[iiopt][jjopt] = (sumf0x2[iiopt][jjopt]-pow(sumf0x[iiopt][jjopt],2)/nstepsdis);

	   if (f02[iiopt][jjopt] == 0)
	     printf ("\n\n function r2calc: ERROR in calculation of f02 i=%d j=%d nstepsdis=%d -->division by 0!!\n",iiopt,jjopt,nstepsdis);
	   else
	    r2[iiopt][jjopt] = (f02[iiopt][jjopt]-f2[iiopt][jjopt])/f02[iiopt][jjopt];                

      }  /*end for  next parameter constellation in case of optimization*/

     printf("\n \t\t r2 for discharge =%8.4f",r2[1][1]);
     fprintf(outcontrol,"\n r2  = %8.4f",r2[1][1]);

  return;
}


/**************************************************************************/
/* FUNCTION r2calln CALCULATES THE logarithmic r2-CRITERIUM               */
/*     same function for simulation and optimization                      */
/**************************************************************************/

void r2calcln()
	
 {
   for (iiopt=1;iiopt<=anzahlopt1;iiopt++)
     for (jjopt=1;jjopt<=anzahlopt2;jjopt++)
      {
   f02ln[iiopt][jjopt] = (sumf0x2ln[iiopt][jjopt]-pow(sumf0xln[iiopt][jjopt],2)/nstepsdis);
   
	if (f02ln[iiopt][jjopt] == 0)
	  printf ("\n\n function r2calcln: ERROR in calculation of f02 (ln) i=%d j=%d --> division by 0!!\n",iiopt,jjopt);
	else
	  r2ln[iiopt][jjopt] = (f02ln[iiopt][jjopt]-f2ln[iiopt][jjopt])/f02ln[iiopt][jjopt];                  
       }  /*end for  next parameter constelation*/

     printf("\n \t\t logarithmic r2 = %8.4f",r2ln[1][1]);
     fprintf(outcontrol,"\n logarithmic r2 = %8.4f",r2ln[1][1]);

  return;
}

