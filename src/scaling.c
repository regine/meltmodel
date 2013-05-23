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
/*******************************************************************/
/*   FILE          scaling.c                                       */
/*     compute change in area as a function of the volume change
       done at the end of each mass balance year*/
/* Nov 2011 */
/*******************************************************************/

#include "scaling.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "initial.h"
#include "writeout.h"

#include "variabex.h"      /* all global VARIABLES */


void scaling() {
    int   nglaciercells=0;
    float volumechange=0,glacarea=0,glacareanew=0,areachange=0,areachange2=0;
    int   areachange_numbercells;   /*number of grid cells to be added or removed*/
    float minimum;
    int min_i,min_j,ii;
    FILE   *outnewDEMglac=NULL;
    char   namenewDEMglac[100];

    /* --- DETERMINE GLACIER AREA, count number of glacier gridcells -----------*/
    /*Note that variable nglac refers to all drainage basin grid cells*/
    nglaciercells = 0;
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if ((griddgmglac[i][j]) != nodata)     /*grid of glacier*/
                nglaciercells += 1;
        }

    /* --- DETERMINE GLACIER AREA, count number of glacier gridcells -----------*/
    glacarea = nglaciercells * gridsize * gridsize;    /*in m2*/

    /* --- CONVERT ANNUAL SPECIFIC MASS BALANCE INTO VOLUME -----------*/
    volumechange = x[11]/100/0.9*glacarea;   /*assuming 900 kg/m3;  x[11] is mean, comes from statist, writeout.c*/
    /*divide by 100 to convert cm into meter*/

    /* -----------------------------------------------*/
    /* --- SCALING ---Arendt JGR 2006 and just simple V-A scaling assumin delta V is prop to delta A--------*/
    /* -----------------------------------------------*/
    areachange = (volumechange/ (c_coefficient*gammaVA*(pow(glacarea,(gammaVA-1))) ));  /*m2, negative in case of retreat*/

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

    fprintf(outspecificmassbal," %.4f %.4f %.3f %d %.3f\n",glacarea,glacareanew,areachange,areachange_numbercells,areachange2);

    /* -----------------------------------------------*/
    /* --- REMOVE GRID CELLES FROM GLACIER DEM--------*/
    /* -----------------------------------------------*/

    if((areachange < 0) && (glacareanew > 0))/*;*/     /*only for retreat, no advance*/
    {
        for (ii=1; ii<=  -areachange_numbercells; ii++) {       /*ueber alle Reihen*/
            minimum = 10000.0;      /*minimum, must be initialized high*/
            for (i=1; i<=nrows; i++) {
                for (j=1; j<=ncols; j++) {
                    if (griddgmglac[i][j] != nodata) {   /*only area to be calcualated*/
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

    /* -----------------------------------------------*/
    /* --- WRITE NEW GLACIER DEM TO FILE--------*/
    /* -----------------------------------------------*/
    sprintf(namenewDEMglac,"DEMglac%04d.bin",(int)year);
    openoutfile(&outnewDEMglac,outpath,namenewDEMglac);
    statist(griddgmglac,x);
    fwrite(&(x[1]),sizeof(float),12,outnewDEMglac);     /*FIRST 12 ROWS*/
    for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
        fwrite(&(griddgmglac[i][1]),sizeof(float),ncols,outnewDEMglac);

    if(outnewDEMglac)
        closefile(&outnewDEMglac,namenewDEMglac);

    return;
}
