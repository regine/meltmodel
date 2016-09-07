/***********************************************************************
 * Copyright 2011 Regine Hock
 * This file is part of DeBAM and DETiM.
 * 
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with This software.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/
/******************************************************************/
/*   FILE retreat.c                                               */
/*     compute change in area/thickness at the end of each mass balance year*/
/* Nov 2011 (called scaling.c then); update 20 Aug 2015 */
/******************************************************************/

#include "retreat.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

/*include all .h files of the .c files that have functions that are called in this file*/
#include "initial.h"
#include "writeout.h"
#include "grid.h"

#include "variabex.h"      /* all global VARIABLES */


/***************************************************************/
/*** DETERMINE GLACIER AREA, count number of glacier gridcells */
/*** called from retreat functions below                       */
/***************************************************************/

int countglaciergridcells()  {
    /*Note that variable nglac refers to all drainage basin grid cells*/

   int nglaciercells = 0;
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if ((griddgmglac[i][j]) != nodata)     /*grid of glacier*/
                nglaciercells += 1;
        }
    return(nglaciercells);
}

/*********************************************************/
/*** WRITE NEW GLACIER DEM AND THICKNESS TO FILE      ****/
/*** called from retreat functions below              ****/
/*********************************************************/
   
void writeupdatedgrid2file(float **newgrid,char *name) {

    FILE   *outnewgrid=NULL;

    sprintf(dummy,"%04d.bin",(int)year);   /*create filename including year*/
    strcat(name,dummy);

    openoutfile(&outnewgrid,outpath,name);
    statist(newgrid,x);    /*grid has been updated, compute new statistics*/
    fwrite(&(x[1]),sizeof(float),12,outnewgrid);     /*FIRST 12 ROWS*/
    for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
        fwrite(&(newgrid[i][1]),sizeof(float),ncols,outnewgrid);

    if(outnewgrid)
        closefile(&outnewgrid,name);

    return;
}


/*********************************************************/
/*  V-A scaling to model glacier retreat                 */
/*       retreat modeled by removing lowest grid cells   */
/*       no advance, elevations unchanged                */
/*********************************************************/

void scaling() {
    int   nglaciercells=0;
    float volumechange=0,glacarea=0,glacareanew=0,areachange=0,areachange2=0;
    int   areachange_numbercells;   /*number of grid cells to be added or removed*/
    float minimum;
    int min_i,min_j,ii;
    char  namenewgridfile[FNAME_LEN];   /* file name of each years new DEM file*/

    nglaciercells=countglaciergridcells();    /*count number of glaciergridcells to compute area chnges*/

    /* --- DETERMINE GLACIER AREA, count number of glacier gridcells -----------*/
    glacarea = nglaciercells * gridsize * gridsize;    /*in m2*/

    /* --- CONVERT ANNUAL SPECIFIC MASS BALANCE INTO VOLUME -----------*/
    volumechange = x[11]/100/0.9*glacarea;   /*assuming 900 kg/m3;  x[11] is mean, comes from statist, writeout.c*/
    /*divide by 100 to convert cm into meter*/

    /* -----------------------------------------------*/
    /* --- SCALING 1 ---Arendt JGR 2006 differentiated form --*/
    /* -----------------------------------------------*/
    areachange = (volumechange/ (c_coefficient*gammaVA*(pow(glacarea,(gammaVA-1))) ));  /*m2, negative in case of retreat*/

    /* -----------------------------------------------*/
    /* --- SCALING 2 - just simple V-A scaling assuming delta V is prop to delta A --*/
    /* -----------------------------------------------*/
    areachange2 = pow( (fabs(volumechange)/c_coefficient),(1/gammaVA) );   /*in m2*/
    /*fabs=absolute value; has to be positive, otherwise NaN*/
    if(volumechange < 0)
        areachange2 = -areachange2;

    /* --- DETERMINE NUMBER OF GRID CELLS TO ADD OR REMOVE--------*/
    areachange_numbercells = (int)(areachange/(gridsize*gridsize));    /*negative in case of retreat, gridsize in m*/

    glacarea = glacarea/1000000;    /*convert m2 to km2*/
    areachange = areachange/1000000;
    areachange2 = areachange2/1000000;

    glacareanew = glacarea + areachange;    /*areachange is negative in case of retreat, in km2*/
    if(glacareanew < 0)
        glacareanew = 0;

    /* --- PRINT AREA AND AREA CHANGE TO MASS BALANCE FILE--------*/

    fprintf(outspecificmassbal,"           %.4f  %.4f   %.3f    %d    %.3f\n",glacarea,glacareanew,areachange,areachange_numbercells,areachange2);

    /* -----------------------------------------------*/
    /* --- REMOVE GRID CELLS FROM GLACIER DEM--------*/
    /* -----------------------------------------------*/

    if((areachange < 0) && (glacareanew > 0))     /*only for retreat, no advance*/
    {
        for (ii=1; ii<=  -areachange_numbercells; ii++) {       /*ueber alle Reihen*/
            minimum = 10000.0;      /*minimum, must be initialized high*/
            for (i=1; i<=nrows; i++) {
                for (j=1; j<=ncols; j++) {
                    if (griddgmglac[i][j] != nodata) {   /*only glacier*/
                        if (griddgmglac[i][j] < minimum) {
                            minimum = griddgmglac[i][j];   /*minimum*/
                            min_i = i;    /*store row, col of cell with minimum value*/
                            min_j = j;
                        }
                    }  /*endif*/
                }  /*for col*/
            }   /*for row*/

            griddgmglac[min_i][min_j] = nodata;
        }  /*for each grid cell to be removed*/
    }  /*endif retreat*/

     /* --- WRITE NEW GLACIER DEM AND THICKNESS GRIDS TO FILE ONCE PER YEAR -----------*/
    strcpy(namenewgridfile,"DEMglac");    /* to be included in filename, year added in writeupdatedgrid2file()*/
    writeupdatedgrid2file(griddgmglac,namenewgridfile);   /*function is above*/
           
}


/*********************************************************/
/*** Retreat parameterization Huss et al. 2010           */
/*   only retreat, separate advance parameterization is called from here ******/
/*   called once per year at end of mass-balance year    */
/*********************************************************/

/* 1.) delta H using function normalized DH - normalized elevation range
   2.) restrict to most negative surface mass balance
   3.) use local balance if thickness < 10 m (no ice flow)
   4.) redistribute excess volume equally for mass conservation  */
  
void retreat_Huss() {

  int   nglaciercells_start=0, nglaciercells_new=0;   /*number grid cells glacierized at start/end of MB year*/
  float glacarea_start=0,glacarea_new=0;   /*new is area after adjustment*/
  float deltaH_norm_sum = 0;        /*sum of normalized delta h values, Eq. (2)*/
  float icedens = 900, waterdens=1000;      /*ice density 900 kg/m3*/ 
  float deltaV_MBmodel, deltaV_Hussmodel;   /*needed to correct for mass conservation*/
     /*coefficients for normalized delta h - normalized elevation range*/
  float Huss_a=-.022, Huss_b=0.12, Huss_c=-0.02, Huss_gamma=6;  
  float factor_f;   /*factor f in Equ. (2), Huss et al. 2010*/
  float elevationchange;   /*factor f*DH; Equ. (3), Huss et al. 2010*/
  float elevationchangesum=0;     /*sum of all grid cell's elevation changes (m ice eq.)*/
  float elev_range_norm;   /*normalized elevation range, 0=top of glacier, 1=front*/
  float min_ba=0;      /*most negative annual mass balance value from glacier grid*/
  char  namenewgridfile[FNAME_LEN];   /* file name of each years new DEM/thickness file*/
  float excess_vol=0;    /*volume that needs to be added/subtracted to conserve mass (in m3 ice eq.*/

  /*--------- initialize delta h grid --------------*/
   /*grid DH initialized and storage reserved in intial.c, because needed every year which
     makes freeing storage difficult*/
  
  /*-------------------------------*/
  if ((i==100) && (j==100))
	 printf("%.4f %.4f\n",MASSBALcum[i][j],specificglacwidebalance);   /*in cm w.e.*/
	        
/*================================ GLACIER RETREAT ==============================*/
if(specificglacwidebalance < 0)      /*glacier retreat only when mass balance is negative*/
 { 
 /*most negative mass balance needed, since surface lowering is retricted not to exceed 
     that year's surface mass balance in ice eq. at terminus*/
   min_ba = x[8];   /*x still contains the annual balance (statist last called in writemassbalgrid())
                      but will be overwritten before needed, when statist is called for glacier grid*/
   
  /*--------- compute # glacier grid cells, area (in km2), min --------------*/
   nglaciercells_start=countglaciergridcells();  /*count # of glacier gridcells to compute area before area changes*/
   glacarea_start = (nglaciercells_start * gridsize * gridsize);    /*gridsize in m2*/
   deltaV_MBmodel = specificglacwidebalance/100*glacarea_start/(icedens/waterdens);  /*total V change (m3 ice equ.)*/
  
  /*--------- find max, min glacier elevation (changes each year) -----------*/
    statist(griddgmglac,x);  /*get statistics of glacier DEM for determining elevation range (in writeout.c)*/
	                         /* x[9] is maximum, x[8] is minimum elevation */
    
  /*--------- compute normalized elevation change grid -----------*/
    for (i=1; i<=nrows; i++)
    	for (j=1; j<=ncols; j++) {
            if ((griddgmglac[i][j] != nodata) && (THICK[i][j] > 10))      /*only if ice > 10 m*/
	         {  nglaciercells_start += 1;   /*count # of grid cells*/
      /*--- compute normalized elevation change (DH), Equ (1), Huss et al.--------*/
	  /* DH must be grid because f needs to be determined before h can be updated, i.e. values must be stored*/
                elev_range_norm = (x[9]-griddgmglac[i][j])/(x[9] - x[8]);  /*normalized elev range for 1 grid cell*/
	       	    DH[i][j] = (pow((elev_range_norm+Huss_a),Huss_gamma)) + Huss_b*(elev_range_norm+Huss_a) + Huss_c;
	            deltaH_norm_sum += DH[i][j];   /*sum up over glacier for Eq. 2; area for each grid cell is same*/
	    	  }  /*endif*/
	    }  /*endfor*/
	
  /*--------- compute factor f in Equ. (2); can only be done when DH has been calculated for whole grid ------*/
  /*    therefore looping through entire glacier grid twice (above and below)*/
	factor_f = specificglacwidebalance*100/(icedens*deltaH_norm_sum);   /*NOTE: glacier area cancels out; 100: cm-->m*/	    	     
	
  /*========= compute elevation change for each grid cell and new elevation, Equ. (3) ----------*/
	for (i=1; i<=nrows; i++)
    	for (j=1; j<=ncols; j++) {
            if (griddgmglac[i][j] != nodata)     /*only glacier area*/
	         {  if(THICK[i][j] > 10)    /*thicker ice: Huss parameterization applied*/
	              elevationchange = factor_f*DH[i][j];    /*Equation 3, Huss et al. 2010*/
	            else     /*thin ice, ice dynamics neglected, thickness change is mass balance*/
 	              elevationchange = MASSBALcum[i][j]/100/icedens;   /*annual spec. net mass bal, convert to m ice eq.*/
 	              
 	  /* --- surface lowering is retricted not to exceed that year's surface mass balance in ice eq. at terminus*/
 	            printf(" retreatHuss(): elevationchange= %.2f, min_ba = %.2f\n",elevationchange,min_ba);
 	            if(elevationchange < min_ba/icedens)
 	               elevationchange = min_ba/icedens;
 	               
	          	if (THICK[i][j] > elevationchange)    /*ice thick enough to thin all elevationchange*/	          	     
   	             { griddgmglac[i][j] += elevationchange;   /*adjust glacier DEM*/
   	               THICK[i][j]       += elevationchange;   /*adjust ice thickness*/
   	       /*adjust elevations for glacier also in other DEMs (whole region, drainagebasin) to make sure
             that any elevation dependent calculations have newly adjusted elevations and same as DEMglac*/
   	               griddgmdrain[i][j] = griddgmglac[i][j];
   	               griddgm[i][j]      = griddgmglac[i][j];  
				 }
   	            else   /*not enough ice to melt all elevationchange, grid cells becomes ice-free*/
   	             { elevationchange     = -THICK[i][j];   /*change can't be more than thickness; must be negative*/
		           griddgmglac[i][j]   = nodata;    /*glacier extent changed, no longer ice-covered*/
		           THICK[i][j]         = 0;
		           griddgmdrain[i][j] += elevationchange;   /*subtract remaining thickness from other grids*/
		           griddgm[i][j]       = griddgmdrain[i][j];   /*replace elevation for entire DEM*/
 	             }
 	             
 	             elevationchangesum += elevationchange;    /*sum up to adjust for mass conservation*/

    	 	 }  /*endif glacier grid*/
        }  /*endfor all grid cells*/
	    
/*--------- conserve mass: both volume changes must be the same; due to some additional conditions (see above)
  they may deviate; then redistribute the difference (excess volume) equally across the new glacier area so that the
  total volume change derived from the modeled mass balance is preserved  ------*/
       /*annual volume change based on sum of elevation changes per pixel of initial ice cover (m3 ice equ.)*/
        deltaV_Hussmodel = elevationchangesum*glacarea_start;   /*m3 ice equivalent (specific MB x area), is negative*/
		excess_vol       = deltaV_MBmodel - deltaV_Hussmodel;   /*m3 ice equivalent*/
		    /*if positive there is still more thinning needed to conserve mass, 
		      i.e. to obtain the glacierwide vol change from the mass balance calculation*/

   /*redistribute the difference equally over all new glacier grid cells*/
   /*NOTE: loop is only over the new (reduced) glacier area; newly unglacierized elevations are not changed further*/
        for (i=1; i<=nrows; i++)    
    	  for (j=1; j<=ncols; j++) {
    	    if (griddgmglac[i][j] != nodata)     /*only glacier area*/
			 {  nglaciercells_new   += 1;   /*number of glacier grid cells after geometric adjustments*/
			    griddgmglac[i][j]   -= excess_vol/glacarea_new;  /*adjust elevation change uniformly across the glacier*/
		        THICK[i][j]         -= excess_vol/glacarea_new;
		        griddgmdrain[i][j] += griddgmglac[i][j];   /*subtract remaining thickness from other grids*/
		        griddgm[i][j]       = griddgmglac[i][j];   /*replace elevation for entire DEM*/
			 }   /*if: only glacier area*/
		  }  /*for all rows/col*/


/*============= OUTPUT ==================================*/
    /* --- DETERMINE NEW GLACIER AREA, count number of glacier gridcells -----------*/
     glacarea_new = (nglaciercells_new * gridsize * gridsize)/1000000;    /*gridsizein m2; convert m2 to km2*/

    /* --- PRINT AREA AND AREA CHANGE TO ANNUAL MASS BALANCE TEXT FILE--------*/
    /* --- header is written in startspecificmassbalance() in initial.c--------*/
    fprintf(outspecificmassbal," %.4f %.4f %.3f\n",glacarea_start,glacarea_new,glacarea_new-glacarea_start);

    /* --- WRITE NEW GLACIER DEM AND THICKNESS GRIDS TO FILE ONCE PER YEAR -----------*/
    strcpy(namenewgridfile,"DEMglac");    /*to be included in filename, year added in writeupdatedgrid2file()*/
    writeupdatedgrid2file(griddgmglac,namenewgridfile);   /*function is above*/
	
	strcpy(namenewgridfile,"thickness");
    writeupdatedgrid2file(THICK,namenewgridfile);   /*function is above*/
    
  /*============= WRITE TO logfile =========================*/
    fprintf(outcontrol, "=== Huss parameterization: year=%.0f day=%.0f  excess_vol (m3) =   %.1f mean thickness of excess volume (initial area) = %.2f   factor_f= %.2f\n",year,jd2,excess_vol,excess_vol/glacarea_new,factor_f);

    printf("=== Huss parameterization: year=%.0f day=%.0f  excess_vol (m3) =   %.1f mean thickness of excess volume (initial area) = %.2f   factor_f= %.2f\n",year,jd2,excess_vol,excess_vol/glacarea_new,factor_f);

    
/*============= RE-INITIALIZATION ==================================*/
    /* --- set MASSBALcum to zero for next year; done in writeout.c otherwise */
         /*massbal array set to 0 otherwise in writeout.c */    
    initializeglacier2zero_nodata(nrows, ncols, MASSBALcum);   /*cum annual net balance grid*/
    /*compute # of grid cells per elevation band for new glacier DEM since # bands may have changes,
      needed for computation of mass balance profiles (computemassbalprofile() in writeout.c) */
    areaelevationbelts();    /*in initial.c*/

 }  /*endif neg mass balance --> retreat*/

 /* advance/thickening not yet implemented*/

    return;
}
