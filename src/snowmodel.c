/***********************************************************************
 * Copyright 2005,2012 Carleen Tijm-Reijmer, Regine Hock
 *
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
/****************************************************************************/
/*  FILE  :  snowmodel.c                                                    */
/*    subsurface model                                                      */
/*    entire file by Carleen Tijm-Reijmer 3/2005                            */
/* update 10 May 2005*/
/****************************************************************************/


#include "snowmodel.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "initial.h"
#include "grid.h"
#include "radiat.h"
#include "snowinput.h"
#include "skintemperature.h"
#include "turbul.h"

#include "variabex.h"      /* all global VARIABLES */


/*==============FUNCTION THAT WOULD BELONG TO CLOSEALL.C ====================== */

/****************************************************************************/
/* FUNCTION  meandaysnownull
	     INITIALISATION OF ARRAYS FOR DAILY MEANS OF SNOW MODEL */
/*must be zero, because of summing up  */
/****************************************************************************/
void meandaysnownull()

{
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if(runoffyes == 1)
                meanrunoffday[i][j] = 0.0;
            if(superyes == 1) {
                meansupericeday[i][j] = 0.0;
                superice[i][j] = tmpsuperice[i][j];
            }
            if(wateryes == 1)
                meanwatercontentday[i][j] = 0.0;
            if(surfwateryes == 1)
                meansurfacewaterday[i][j] = 0.0;
            if(slushyes == 1)
                meanslushthicknessday[i][j] = 0.0;
            if(coldsnowyes == 1)
                meancoldcontentsnowday[i][j] = 0.0;
            if(coldtotyes == 1)
                meancoldcontenticeday[i][j] = 0.0;
        }

    return;
}


/****************************************************************************/
/* FUNCTION  albedocalcdepth                                                */
/*    for method 4: internal generation of albedo                           */
/*    called from main every time step and for each grid cell               */
/****************************************************************************/

void albedocalcdepth() {
    int inti;

    inti=(int)surface[i][j];
    switch(inti)   /*** WRITE ENERGY BALANCE GRID-OUTPUT-FILES ***/

    {
    case 1:
        albedosnowdepth();       /*snow surface*/
        break;
    case 2:
        albedosnowdepth();       /*firn, same procedure as for snow*/
        break;
    case 3:        /*ice linear decrease with elevation*/
        /* ALBEDO[i][j] = albice-albice*
         ((griddgm[rowclim][colclim]-griddgm[i][j])*albiceproz/100/100);*/
        albedosnowdepth(); /*plus change due to water on surface*/
        break;
    case 4:
        ALBEDO[i][j] = albrock;
        break;
    case 5:
        ALBEDO[i][j] = albrock;
        break;

    }

    return;
}


/****************************************************************************/
/* FUNCTION  albedosnowdepth                                                */
/*   this function calculates snow albedo according to                      */
/*   either: (0) Oerlemans and Knap, 1998, J. Glaciol.                      */
/*   or: (2) Douville et al 1995.                                           */
/*   or: (1) combination of Oerlemans and Knap, 1998, J. Glaciol. and       */
/*   Zuo and Oerlemans, 1996, J. Glaciol. taken from Bougamont et al 2005   */
/*   including a dependence on snowdepth following                          */
/*   Oerlemans and Knap, 1998  							                    */
/*   including solar zenith angle dependency:                               */
/*   Segal et al., 1991 J. Atm. Sci., 48(8), 1024-1042.                     */
/****************************************************************************/

void albedosnowdepth() {
    float  albmax/*=0.90*/;      /*maximum snow albedo value*/
    float  albwat=0.15;    /*albedo of water*/
    float  albsup=0.70;
    /* L.G.: icestar is unused, can we delete? */
    //float  icestar=0.011; /*0.011;  */  /*characteristic ice depth in m ice*/
    float  wetstar=300;   /*characteristic depth scale surficial water in mm water*/
    float  firndens=400;
    float  constant;	/*=7;*/
    float  timestar;
    float  reducrate=0.008;  /*constant reduction ratio*/
    float  tauday=86400;     /*amount of seconds in one day*/
    float  efoldcoef=0.24;   /*exponential e-folding coefficient*/
    float  snowalb, minalb;
    float  deltat;
    float  snowrest,snowwet,surfwet,surfice/*,surfdens*/,firnrest;
    float  factor,coszen;
    int    snowslush;
    int    solzenyes=1;	/*yes(1) or no(0) correction snow albedo for low solar zenith angles*/

    albmax = albsnow;
    deltat = timestep*3600.;
    minalb=albmin;

    if (typealbedo == 1) constant = (timestardry10-timestardry0)/10.; /*gradient for time scale albedo decay between -10 and 0 C*/

    if ((methodsurftempglac != 4) || (percolationyes == 0)) {
        snowrest = SNOW[i][j]*10./firndens;
        if ((FIRN[i][j] > 0.) && (snowrest == 0.)) {
            snowrest = 0.001;
            firnrest = 2;
        }
        if (methodsurftempglac == 4) firnrest = layerid[i][j][1];
        snowwet = 0.;
        /* if (MELT[i][j] > 0.) snowwet = 1.;*/
        snowslush = 0;
        surfwet = 0.;
        /*  if (MELT[i][j] > 0.) surfwet = 5.;*/	/*10 mm water on ice if melt occurs*/
        surfice = 0.;
        /* surfdens = 100.;*/
    } 
    else {
        snowrest = snowlayer[i][j];
        if (snowlayer[i][j] <= 0.0) snowrest= (snowlayersum[i][j]/denssnow);
        /*snowrest = snowrest - superice[i][j]/densice;*/ //LG: what's going on here?
        if (snowrest < 0.) snowrest = 0.;
        /*if ((FIRN[i][j] > 0.) && (snowrest == 0.)) snowrest = 0.001;*/
        if ((layerid[i][j][1] == 2) && (snowrest == 0.)) snowrest = 0.001;
        snowwet = layerwatercont[i][j][1];
        snowslush = 0;
        if ((slushdepth[i][j] > -99.) && (slushdepth[i][j] < 0.10)) snowslush = 1;
        surfwet = surfacewater[i][j];
        surfice = superice[i][j];
        /* surfdens = layerdensity[i][j][1];*/
    }

    if(snowprec == 0) {        /*no snow fall, decrease in albedo*/
        numbdays[i][j] += timestep/24.0;    /*number of days in days*/

        minalb=albfirn;
        if (snowwet > 0.) minalb = albslush;
        if (snowslush > 0) minalb=albslush;

        if (snowrest != 0.) {

            if (typealbedo == 0) {
                /*    Oerlemans and Knap, 1998 */
                snowalb = minalb + (albmax - minalb) * exp((-numbdays[i][j])/timestarwet);
            } else if (typealbedo == 1) {
                /*    Oerlemans and Knap, 1998, Zuo and Oerlemans, 1996 */
                if (snowwet > 0.) {
                    timestar = timestarwet;
                } else {
                    if ((surftemp[i][j] <= 0.) && (surftemp[i][j] >= -10.)) {
                        timestar = fabs(surftemp[i][j])*constant+timestardry0;
                    } else {
                        timestar = timestardry10;
                    }
                }
                snowalb = ALBALT[i][j] - (ALBALT[i][j]-minalb)*(deltat/(timestar*tauday));
            } else if (typealbedo == 2) {
                /*   Douville et al, 1995 */
                if(surftemp[i][j] >= 0) {   /*tempint*/
                    snowalb = minalb + (ALBALT[i][j] - minalb)*exp(-efoldcoef*deltat/tauday);
                } else {
                    snowalb = ALBALT[i][j] - reducrate*(deltat/(tauday*(1+0.1*pow(surftemp[i][j],4))));
                }
            }

            /*Correction albedo for low solar zenith angles, Segal et al, 1991*/
            if (solzenyes == 1) {
                coszen = cos(sunzenithangle);
                if (cos(sunzenithangle) < cos(80.*pi/180.)) coszen = cos(80.*pi/180.);
                factor = 0.32*0.5*((3/(1+4*coszen))-1);
                if (factor < 0.) factor = 0.;
                snowalb = snowalb + factor;
                if (snowalb > albmax) snowalb = albmax;
            }

            minalb=albice;
            /* if ((FIRN[i][j] > 0.) && (surfdens < 815.)) minalb=albfirn;*/
            if (firnrest == 2) minalb=albfirn;
            if ((surfice > 0.) && (snowrest < 10*snowstar)) minalb=albsup;
            /* Snow depth dependent albedo*/
            ALBEDO[i][j] = snowalb + (minalb - snowalb) * exp(-snowrest/snowstar);
        } else {
            minalb=albice;
            if (surfice > 0.)
                /*ALBEDO[i][j] = albsup + (albice - albsup) * exp(-(surfice/densice)/icestar);*/
                ALBEDO[i][j] = albsup;
            else
                ALBEDO[i][j] = albice-albice*
                               ((griddgm[rowclim][colclim]-griddgm[i][j])*albiceproz/100/100);
            /* albedo is lowered when water on the surface */
            if(surfwet > 0.) minalb=albwat;
            if(surfwet > 0.) ALBEDO[i][j] = minalb + (ALBEDO[i][j] - minalb)*exp(-surfwet/wetstar);
        }

        if(ALBEDO[i][j] < minalb)          /*albedo cannot be lower than albmin/minalb*/
            ALBEDO[i][j] = minalb;
    }

    else { /*SNOW FALL, ALBEDO INCREASES */
        ALBEDO[i][j] = albmax;
        numbdays[i][j] = 0;
        /* albedo is lowered when water on the surface */
        /*if(surfwet > 0.) minalb=albwat;
        if(surfwet > 0.) ALBEDO[i][j] = albwat + (ALBEDO[i][j] - albwat)*exp(-surfwet/wetstar);*/
    }

    if((methodsnowalbedo == 5) && (topofatm > 20) && (inter == factinter) && (surface[i][j] != 3)) { /*modify albedo according to cloudiness*/
        /*only during day, thus minimum radiation*/
        albedosnowclouds();
    }

    if ((typealbedo == 1) || (typealbedo == 2)) {
        ALBALT[i][j] = ALBEDO[i][j];    /*store albedo for next time step*/
        /*must be a different array, because, ALBEDO-array may be overwritten,
          if means are written to output, see writeout.c*/
        if ((solzenyes == 1) && (snowprec == 0) && (snowrest != 0.))
            ALBALT[i][j] = ALBALT[i][j]-factor;
    }
    /*  fprintf(outcontrol," %f %f %f %f %f %f %f %f %f \n ",
           jd2,ALBEDO[i][j],albmin,minalb,z0e,z0T,z0w,snowrest,snowprec);*/

    return;
}


/*==============FUNCTION THAT WOULD BELONG TO SNOW MODEL ====================== */

/****************************************************************************/
/* FUNCTION  initgrid                                                       */
/*   create and initialises the grid for snow temperature calculation       */
/*   Possible snowlayer on top of ice or firn                               */
/*   maximum dept set with depthdeep                                        */
/*   thickness layer at depthgdeep set with thicknessdeep                   */
/*   depth positive downwards                                               */
/*   called from main once before time loop                                 */
/****************************************************************************/


void initgrid() {
    int     ik,kk,ir,jc;
    int k;
    double  depth;
    double  depthl;
    float   coeff,coeffupper,coefflower;
    int     lowerpart;            /*logical function 0 = upper layer (snow on firn or ice), */
    /*1 = lower layer of ice or firn below snow*/
    int     krows,kcols;
    float   depthdist;
    double  sum,diff;
    double  airvolumeice,minwatercont;
    /* L.G.: Lf is unused, can we delete? */
    //float   Lf = 334000.0;     /*latent heat of fusion  [J/kg].*/

    for (kk = 1; kk <= numbersnowdaysout; kk++) {
        if (jdbeg <= jdsurface[kk]) {
            newday = kk;
            kk = numbersnowdaysout;
        }
    }

    printf(" INITIALISE SUBSURFACE GRID %d \n",newday);

    /*allocate space*/
    layerdepth=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layerthickness=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layerdensity=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layermass=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layertemperature=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layerrhocp=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layerwatercont=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layerdeltawatercont=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layerrefreeze=tensorreservdouble(1,nrows,1,ncols,1,ndepths);
    layerdeltarefreeze=tensorreservdouble(1,nrows,1,ncols,1,ndepths);

    layerid=tensorreserv(1,nrows,1,ncols,1,ndepths);
    layeramount=matrixreserv(1,nrows,1,ncols);
    layeramountcold=matrixreserv(1,nrows,1,ncols);

    MELTsum=matrixreservdouble(1,nrows,1,ncols);
    ABLAsum=matrixreservdouble(1,nrows,1,ncols);
    RUNOFFsum=matrixreserv(1,nrows,1,ncols);
    MBsum=matrixreserv(1,nrows,1,ncols);
    deltaWATER=matrixreservdouble(1,nrows,1,ncols);
    deltaREFREEZE=matrixreservdouble(1,nrows,1,ncols);
    deltaWATERsum=matrixreservdouble(1,nrows,1,ncols);
    deltaREFREEZEsum=matrixreservdouble(1,nrows,1,ncols);

    SNOWinit=matrixreserv(1,nrows,1,ncols);
    MASSBALcumstake=matrixreserv(1,nrows,1,ncols);
    SUMMERBALST=matrixreserv(1,nrows,1,ncols);
    initializeglacier2zero_nodata(nrows, ncols, SUMMERBALST);
    SNOWswitch=matrixreserv(1,nrows,1,ncols);

    sumSNOWprec=matrixreserv(1,nrows,1,ncols);
    sumRAINprec=matrixreserv(1,nrows,1,ncols);

    sumMASS=matrixreserv(1,nrows,1,ncols);
    capwatercontent=matrixreserv(1,nrows,1,ncols);
    slwatercontent=matrixreserv(1,nrows,1,ncols);

    snowlayersum=matrixreservdouble(1,nrows,1,ncols);
    superice=matrixreserv(1,nrows,1,ncols);
    tmpsuperice=matrixreserv(1,nrows,1,ncols);
    watercontent=matrixreservdouble(1,nrows,1,ncols);
    watercontentsum=matrixreservdouble(1,nrows,1,ncols);
    slushdepth=matrixreserv(1,nrows,1,ncols);
    slushthickness=matrixreserv(1,nrows,1,ncols);
    surfacewater=matrixreservdouble(1,nrows,1,ncols);
    coldcontentsnow=matrixreserv(1,nrows,1,ncols);
    coldcontentice=matrixreserv(1,nrows,1,ncols);
    meltenergy=matrixreserv(1,nrows,1,ncols);
    meltenergysum=matrixreserv(1,nrows,1,ncols);

    accyear=matrixreserv(1,nrows,1,ncols);
    
    graddirect=matrixreservdouble(1,nrows,1,ncols);
    DIRECTsum=matrixreserv(1,nrows,1,ncols);
    if (methodglobal == 2) {
      DIRECT2sum=matrixreserv(1,nrows,1,ncols);
      DIFFUSsum=matrixreserv(1,nrows,1,ncols);
    }
    GLOBALsum=matrixreserv(1,nrows,1,ncols);
    REFLECTsum=matrixreserv(1,nrows,1,ncols);
    LONGINsum=matrixreserv(1,nrows,1,ncols);
    LONGOUTsum=matrixreserv(1,nrows,1,ncols);
    SENSIBLEsum=matrixreserv(1,nrows,1,ncols);
    LATENTsum=matrixreserv(1,nrows,1,ncols);
    ICEHEATsum=matrixreserv(1,nrows,1,ncols);
    rainenergysum=matrixreserv(1,nrows,1,ncols);

    SUBLIMATION=matrixreservdouble(1,nrows,1,ncols);
    SUBLIMATIONsum=matrixreservdouble(1,nrows,1,ncols);

    /*new daily / period mean grid output*/
    if(runoffyes == 1) {
        meanrunoffday = matrixreservdouble(1,nrows,1,ncols);
        meanrunoffall = matrixreservdouble(1,nrows,1,ncols);
    }
    if(superyes == 1) {
        meansupericeday = matrixreservdouble(1,nrows,1,ncols);
        meansupericeall = matrixreservdouble(1,nrows,1,ncols);
    }
    if(wateryes == 1) {
        meanwatercontentday=matrixreservdouble(1,nrows,1,ncols);
        meanwatercontentall=matrixreservdouble(1,nrows,1,ncols);
    }
    if(surfwateryes == 1) {
        wrtsurfacewater=matrixreserv(1,nrows,1,ncols);
        meansurfacewaterday=matrixreservdouble(1,nrows,1,ncols);
        meansurfacewaterall=matrixreservdouble(1,nrows,1,ncols);
    }
    if(slushyes == 1) {
        meanslushthicknessday  =matrixreservdouble(1,nrows,1,ncols);
        meanslushthicknessall  =matrixreservdouble(1,nrows,1,ncols);
    }
    if(coldsnowyes == 1) {
        meancoldcontentsnowday=matrixreservdouble(1,nrows,1,ncols);
        meancoldcontenticeday =matrixreservdouble(1,nrows,1,ncols);
    }
    if(coldtotyes == 1) {
        meancoldcontentsnowall=matrixreservdouble(1,nrows,1,ncols);
        meancoldcontenticeall =matrixreservdouble(1,nrows,1,ncols);
    }
    /*****************/

    conduc=arrayreservdouble(1,ndepths);
    conducdtdz=arrayreservdouble(1,ndepths);
    layerenergy=arrayreservdouble(1,ndepths);

    coeff = (factorthickness - thicknessfirst)/factorthickness;
    coeffupper = coeff;
    coefflower = coeff;
    rhocpice = densice * cpice;

    if(methodinisnow == 2)
        whichsurface();    /*for each surface (snow,firn,ice,rock) integer allocated*/
    sumrunoff = 0.;

    krows = nrows;
    kcols = ncols;
    if (calcgridyes == 2) {
        krows = 1;
        kcols = 1;
    }

    for (ir = 1; ir <= krows; ir++) {
        for (jc = 1; jc <= kcols; jc++) {
            i = ir;
            j = jc;
            if (calcgridyes == 2) {
                i = rowclim;
                j = colclim;
            }

            if (surface[i][j] != nodis) {
                coeffupper = coefflower;
                if ((supericegrid == 1) && (i == rowclim) && (j == colclim)) coeffupper = 0.;
                if (supericegrid == 2) coeffupper = 0.;
                coeff = coeffupper;

                choice_snowlayermsnow();

                SNOWinit[i][j] = SNOW[i][j];
                if (SNOW[i][j] > 0.) SNOWswitch[i][j] = 0.;

                lowerpart = 0;
                if ((snowlayer[i][j] == 0.0) && (FIRN[i][j] > 0.))
                    lowerpart = 1;
                else if (snowlayer[i][j] == 0.0) lowerpart = 2;
                sum=0.0;

                for (k = 1; k <= ndepths; k++) {
                    kk = k;
                    ik = k;

                    /* initialize subsurface grid, density profile, and layer ID*/
                    switch(methodinitialverticalgrid) {

                    /* with Carleen initialization */
                    case 1:
                    	/* initialise subsurface grid */
						if (lowerpart == 0) coeff = coeffupper;
						if (lowerpart != 0) coeff = coefflower;

						if (k == 1) {
							depth = 0;
							depthl = thicknessfirst + coeff*depth;
							layerthickness[i][j][1] = thicknessfirst;   /*thickness first layer (top)*/
							layerdepth[i][j][1] = 0.5*thicknessfirst;   /*depth of layer = half way of the layer*/
							depth = depth + depthl;
						} else {
							if ((depthl > snowlayer[i][j] ) && (lowerpart == 0)) {
								depthl = snowlayer[i][j];
								layerthickness[i][j][k] = depthl - depth;
								layerdepth[i][j][k] = depth + 0.5*layerthickness[i][j][k];
								if (FIRN[i][j] > 0) lowerpart = 1;
								if (FIRN[i][j] <= 0) lowerpart = 2;
							} else {
								if ((depthl > FIRN[i][j]+snowlayer[i][j] ) && (lowerpart == 1)) {
									depthl = FIRN[i][j]+snowlayer[i][j];
									layerthickness[i][j][k] = depthl - depth;
									layerdepth[i][j][k] = depth + 0.5*layerthickness[i][j][k];
									lowerpart = 2;
								} else {
									depthl = (1 + coeff) * depth + thicknessfirst;
									layerthickness[i][j][k] = depthl - depth;
									if (layerthickness[i][j][k] > thicknessdeep) {
										layerthickness[i][j][k] = thicknessdeep;
										depthl = depth + layerthickness[i][j][k];
									}
									if (lowerpart == 0) {
										depthdist = snowlayer[i][j] - depthl;
										if (depthdist < 0.75*layerthickness[i][j][k]) {
											depthl = snowlayer[i][j];
											if (FIRN[i][j] > 0) lowerpart = 1;
											if (FIRN[i][j] <= 0) lowerpart = 2;
										}
									} else if (lowerpart == 1) {
										depthdist = FIRN[i][j]+snowlayer[i][j] - depthl;
										if (depthdist < 0.75*layerthickness[i][j][k]) {
											depthl = FIRN[i][j]+snowlayer[i][j];
											lowerpart = 2;
										}
									}
									layerthickness[i][j][k] = depthl - depth;
									layerdepth[i][j][k] = depth + 0.5*layerthickness[i][j][k];
								}
								depth = depthl;
								if (depthl > depthdeep)
									ik = ndepths;
							}
						}
						/*Initialize and density profile and layer ID */
						if (layerdepth[i][j][k] <= snowlayer[i][j]) {
							choice_snowlayerdensity(i, j, k);
							layerid[i][j][k] = 1;
							if (layerdensity[i][j][k] > densice-diffdensice)
								layerdensity[i][j][k] = densice;
						} else {
							if (FIRN[i][j] > 0.) { /*firn area*/
								choice_firnlayerdensity(i, j, k);
								layerid[i][j][k] = 2;
								/*  if ((layerdepth[i][j][k]-0.5*layerthickness[i][j][k]) > FIRN[i][j]) */
								if ((layerdepth[i][j][k]) > FIRN[i][j]+snowlayer[i][j]) {
									layerdensity[i][j][k] = densice;
									layerid[i][j][k] = 3;
								}
								if (layerdensity[i][j][k] > densice-diffdensice) {
									layerdensity[i][j][k] = densice;
									layerid[i][j][k] = 3;
								}
								if (layerdensity[i][j][k] < denssnow) {
									layerdensity[i][j][k] = denssnow;
								}
							}
							if (FIRN[i][j] == 0.) {
								layerdensity[i][j][k] = densice;
								layerid[i][j][k] = 3;
							}
						}
						break;

					/* F.Covi 2/27/19, SNOW INIT UPDATE */
                    /* initialize subsurface grid, density profile, and layer ID from file*/
                    case 2:
                    	layerdepth[i][j][k] = layerdepthinit[k];
						layerthickness[i][j][k] = layerthicknessinit[k];
						layerdensity[i][j][k] = layerdensityinit[k];
						layerid[i][j][k] = layeridinit[k];
						break;
                    }

                    /*mass*/
                    layermass[i][j][k] = layerdensity[i][j][k] * layerthickness[i][j][k];
                    /*redefine mass density and watercontent in case start is wet*/
                    /*Note that after the density is recalculated more moisture can be added to the layer*/
                    if (wetstartyes == 1) { /*snow is wet at start*/
                        if (irrwatercontyes == 1) irreducible_schneider(i, j, k);
                        if (irrwatercontyes == 2) irreducible_coleou(i, j, k);
                        airvolumeice = ((densice - layerdensity[i][j][k])/densice)*layerthickness[i][j][k];
                        minwatercont = airvolumeice*irrwatercont*denswater;
                        layerwatercont[i][j][k] = minwatercont;
                        layermass[i][j][k] = layermass[i][j][k] - layerwatercont[i][j][k];
                        layerdensity[i][j][k] = layermass[i][j][k] / layerthickness[i][j][k];
                    } else {
                        layerwatercont[i][j][k] = 0.;
                    }

                    /*check snow layer mass*/
                    if (layerid[i][j][k] == 1)
                        sum = sum + layermass[i][j][k] + layerwatercont[i][j][k];
                    if ((k > 1) && (layerid[i][j][k-1] == 1) && (layerid[i][j][k] != 1)) {
                        switch(methodinitialverticalgrid) {
                            /* with Carleen initialization */
                            case 1:
                                diff = SNOW[i][j]*10. - sum;
                                break;
                            /* F.Covi 7/16/19, SNOW INIT UPDATE */
                            /* with initialization from file redefine SNOW from read snow layers and densities */
                            case 2:
                                SNOW[i][j] = sum/10.;
                                diff = SNOW[i][j]*10. - sum;
                                break;
                        }

                        if (fabs(diff) > 0.000001) {
                            fprintf(outcontrol," i %d j %d k %d Diff %f snow %f sum %f mass %f water %f thick %f dens %f id %f \n",
                                    i,j,k-1,diff,SNOW[i][j]*10.,sum,layermass[i][j][k-1],layerwatercont[i][j][k-1],
                                    layerthickness[i][j][k-1],layerdensity[i][j][k-1],layerid[i][j][k-1]);
                            printf("\n i %d j %d k %d Diff %f snow %f sum %f mass %f water %f \n thick %f dens %f id %f \n",
                                   i,j,k-1,diff,SNOW[i][j]*10.,sum,layermass[i][j][k-1],layerwatercont[i][j][k-1],
                                   layerthickness[i][j][k-1],layerdensity[i][j][k-1],layerid[i][j][k-1]);
                        }
                        layermass[i][j][k-1] = layermass[i][j][k-1] + diff;
                        layerdensity[i][j][k-1] = layermass[i][j][k-1]/layerthickness[i][j][k-1];
                    }
                    /* initialize temperature profile */
                    switch(methodinitialverticalgrid) {

                    case 1:
                	    choice_layertemperature(i, j, k);
                	    break;

                    case 2:
                	    layertemperature[i][j][k] = layertemperatureinit[k];
                	    break;
                    }
                    /*heat capacity*/
                    layerrhocp[i][j][k] = rhocpice * layerdensity[i][j][k] / densice;

                    if ((layermass[i][j][k] > 20000.) || (layermass[i][j][k] < 0.) ||
                            (layerwatercont[i][j][k] < 0) || (layerthickness[i][j][k] < 0.) ||
                            (layerdensity[i][j][k] < denssnow-1.) || (layerdensity[i][j][k] > denswater*1.1) ||
                            (layertemperature[i][j][k] > 0.0) || (layertemperature[i][j][k] <= surftempminimum)) {
                        fprintf(outcontrol,"Z jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                                jd2,k,i,j,layerthickness[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],
                                layerdensity[i][j][k],layertemperature[i][j][k]);
                        resoutlines += 1;
                        if(resoutlines > resoutlinesmax) toobig_resout();
                    }

                    if (ik == ndepths) k = ik;

                } /* end loop over layers */
                layeramount[i][j] = kk;
                layeramountcold[i][j] = kk;

                if ((layerdepth[i][j][kk] + layerthickness[i][j][kk]) < depthdeep) {
                    printf("\n Not enough layers to reach deep! %d %d %f %f %d %d \n",
                           i,j,layerdepth[i][j][kk] + layerthickness[i][j][kk],depthdeep,ndepths,kk);
                    printf("  Set larger value in input.dat\n\n");
                    fclose(outcontrol);
                    exit(3);
                }

            } /* end if surface*/
        } /* end loop over columns */
    } /* end loop over rows */




    return;
}


/****************************************************************************/
/* FUNCTION  changegrid                                                     */
/*   changes grid when layers become to thin or to thick                    */
/*   Possible snowlayer on top of ice or firn                               */
/*   maximum dept set with depthdeep                                        */
/*   depth positive downwards                                               */
/****************************************************************************/

void changegrid() {
    float  factbounl=0.5;
    float  factbounh=2.0;
    float  absbounl,bounl,bounh;
    double coeff,coeffupper,coefflower;
    double depthadd;
    double dummydepth,dummythickness;
    int    fusionyes,skipyes,splityes,fusion2yes;
    double testabla, dummyabla;
    double *thicknessnew,*massnew,*depthnew;
    double *temperaturenew,*densitynew,*watercontnew;
    double *refreezenew,*deltarefreezenew,*deltawatercontnew;
    float  *idnew;
    int k;
    int    newk,newklast;
    double freshsnowlayerthickness,freshsnowlayerthicknessold,freshsnowlayerthicknessnew,extrathickness;
    double oldtoplayerthickness,oldtoplayermass,oldtoplayerdensity;
    double sum,sumwater,diff;
    double energywater;      /*energy released or necessary to convert layerwater to ice or vv*/
    double energytemperature;/*energy necessary to raise layertemperature to melting level */
    /* L.G.: energydensity is unused, can we delete? */
    //double energydensity;    /*energy necessary to convert water into ice enough to raise layerdensity to density ice*/
    double energy;           /*actual energy used (smallest from the above three)*/
    double masschangelayer;
    double energytemperatureold;
    double densfirst;
    float  Lf = 334000.0;     /*latent heat of fusion  [J/kg] */
    /* L.G.: airvolumeice and  minwatercont are unused, can we delete? */
    // double airvolumeice;     /*volume in ice available to store water in*/
    //double minwatercont;     /*if water present this is the minimum amount always present*/
    idnew=arrayreserv(1,ndepths);
    thicknessnew=arrayreservdouble(1,ndepths);
    massnew=arrayreservdouble(1,ndepths);
    refreezenew=arrayreservdouble(1,ndepths);
    deltarefreezenew=arrayreservdouble(1,ndepths);
    depthnew=arrayreservdouble(1,ndepths);
    temperaturenew=arrayreservdouble(1,ndepths);
    densitynew=arrayreservdouble(1,ndepths);
    watercontnew=arrayreservdouble(1,ndepths);
    deltawatercontnew=arrayreservdouble(1,ndepths);

    jd2=jd;
    if ((zeit == 24) && (inter == factinter))
        jd2 = (int)jd + 1;

    coeff = (factorthickness - thicknessfirst)/factorthickness;
    coeffupper = coeff;
    coefflower = coeff;
    if ((supericegrid == 1) && (i == rowclim) && (j == colclim)) coeffupper = 0.0;
    if (supericegrid == 2) coeffupper = 0.0;
    bounl = factbounl * thicknessfirst; /* factbounl * thicknessfirst; */
    absbounl = bounl;                   /* smallest layer possible; comment by F.Covi 7/16/19 */
    bounh = factbounh * thicknessfirst;

    dummydepth = 0.0;
    dummyabla = 0.0;
    newk = 0;
    testabla = 1;
    densfirst = layerdensity[i][j][1];
    
    /* ACCOUNT FOR SUBLIMATION AT THE SURFACE; comment by F.Covi 7/16/19 */
    /*MELT already taken into account in the layer size only sublimation to correct for*/
    /* Set surface ablation given only by SUBLIMATION; comment by F.Covi 7/16/19 */
    if (percolationyes == 1) {
        testabla=0;
        dummyabla = -SUBLIMATION[i][j];	/*ABLA[i][j]-MELT[i][j];*/
    }  /*note NO factor 1000 necessary because density is in g/kg*/
    if (percolationyes == 0) {
        freshsnowlayer = snowprec - sumrunoff; /*MELT[i][j];*/
        testabla = 0;
        /*if ((ABLA[i][j] >= 0.0) && (freshsnowlayer >= 0.))
        { dummyabla = ABLA[i][j] - MELT[i][j];}
        if ((ABLA[i][j] >= 0.0) && (freshsnowlayer < 0.))
        { dummyabla = ABLA[i][j] - (MELT[i][j] + freshsnowlayer);
         freshsnowlayer = 0.;}
        if ((ABLA[i][j] < 0.0) && (freshsnowlayer >= 0.))
        { dummyabla = 0.;}
        if ((ABLA[i][j] < 0.0) && (freshsnowlayer < 0.))
        { dummyabla = MELT[i][j] - (MELT[i][j] + freshsnowlayer);
         freshsnowlayer = 0.;}*/
        if (freshsnowlayer >= 0.) {
            dummyabla = -SUBLIMATION[i][j];	/*ABLA[i][j]-MELT[i][j];*/
        }
        if (freshsnowlayer < 0.) {
            dummyabla = -SUBLIMATION[i][j]-freshsnowlayer;  /*ABLA[i][j] - (MELT[i][j] + freshsnowlayer);*/
            freshsnowlayer = 0.;
        }
    }
    /* Apply surface ablation given by SUBLIMATION to affected layers mass; comment by F.Covi 7/16/19 */
    k=0;
    while (testabla == 0) {
        k=k+1;
        layermass[i][j][k] = layermass[i][j][k] - dummyabla;
        if (dummyabla >= 0.)
            layerthickness[i][j][k] = layerthickness[i][j][k] - dummyabla/layerdensity[i][j][k];
        else
            layerthickness[i][j][k] = layerthickness[i][j][k] - dummyabla/densice;
        if (layerthickness[i][j][k] < 0) {
            dummyabla = -layerthickness[i][j][k]*layerdensity[i][j][k];
            layerthickness[i][j][k] = 0.;
            layermass[i][j][k] = 0.;
            layerrefreeze[i][j][k] = 0.;
            testabla = 0.;
        } else
            testabla = 1;
    }

    /* ACCOUNT for SNOW ACCUMULATION; comment by F.Covi 7/16/19 */
    /*ABLA and snowprec are in mm we, layerthickness in m snow*/
    /*freshsnowlayer is amount of snowprec not melted*/
    if (freshsnowlayer > 0) {
        snowlayersum[i][j] += freshsnowlayer;
        layerthickness[i][j][1] = layerthickness[i][j][1] + (freshsnowlayer/denssnow);
        layermass[i][j][1] = layermass[i][j][1] + freshsnowlayer;
        layerdensity[i][j][1] = layermass[i][j][1]/layerthickness[i][j][1];
    }

    /* LOOP over OLD LAYERS; comment by F.Covi 7/16/19 */
    newk = 1;    /* loop counter for new layers; comment by F.Covi 7/16/19 */
    for (k = 1; k <= (int)layeramount[i][j]; k++) {
        if (layerid[i][j][k] == 1) coeff = coeffupper;
        if (layerid[i][j][k] != 1) coeff = coefflower;
        dummythickness = thicknessfirst + coeff*dummydepth;
        if (dummythickness > thicknessdeep) dummythickness = thicknessdeep;
        bounl = factbounl * dummythickness;
        bounh = factbounh * dummythickness;
        if (k == 1)
            bounh = 1.5 * dummythickness;
        skipyes = 0;  /* if = 1 skip next layer (in case of layers fusion); comment by F.Covi 7/16/19 */

        /* First check if this layer is the last snow layer*/
        fusionyes=0;
        if (layerthickness[i][j][k] > absbounl)
            if (layerid[i][j][k] != layerid[i][j][k+1])
                fusionyes=1;
        fusion2yes=0;		/*1 = if layer too small add layer to layer above, in case of snow ice transition, or layer numbers become larger than max possible*/
        if ((k > 1) && (layerthickness[i][j][k] < absbounl)) {
            if ((layerid[i][j][k] == 1) && (layerid[i][j][k+1] != 1))
                fusion2yes=1;
        }
        if ((k <= (int)layeramount[i][j]) && (newk > ndepths)) {
            fusionyes=1;
            fusion2yes=1;
            if ((idnew[newk-1] == 1) && (layerid[i][j][k] != 1)) fusion2yes = 2;
        }

        /* Then check if a fresh layer of snow is added*/
        splityes = 0;
        if ((k == 1) && (snowlayersum[i][j] > 0.) && (layerthickness[i][j][1] >= thicknessfirst) &&
                (layerid[i][j][1] > 1))
            splityes = 1;

        /* Fusion of two layers */
        /* 1) all layers but not the deepest one; comment by F.Covi 7/16/19 */
        if ((layerthickness[i][j][k] < bounl) && (k != (int)layeramount[i][j]) && (fusionyes == 0) &&
                (fusion2yes == 0)) {
            skipyes = 1;
            thicknessnew[newk] = layerthickness[i][j][k] + layerthickness[i][j][k+1];
            /* if new layer is thicker than bounh, create two layers instead; comment by F.Covi 7/16/19 */
            if (thicknessnew[newk] > bounh) {
                thicknessnew[newk] = bounh/factbounh;
                thicknessnew[newk+1] = layerthickness[i][j][k] + layerthickness[i][j][k+1] - thicknessnew[newk];
                massnew[newk+1] = (thicknessnew[newk+1]/layerthickness[i][j][k+1])*layermass[i][j][k+1];
                massnew[newk] = layermass[i][j][k] + layermass[i][j][k+1] - massnew[newk+1];
                refreezenew[newk+1] = (thicknessnew[newk+1]/layerthickness[i][j][k+1])*layerrefreeze[i][j][k+1];
                refreezenew[newk] = layerrefreeze[i][j][k] + layerrefreeze[i][j][k+1] - refreezenew[newk+1];
                deltarefreezenew[newk+1] = (thicknessnew[newk+1]/layerthickness[i][j][k+1])*layerdeltarefreeze[i][j][k+1];
                deltarefreezenew[newk] = layerdeltarefreeze[i][j][k] + layerdeltarefreeze[i][j][k+1] - deltarefreezenew[newk+1];
                watercontnew[newk+1] = (thicknessnew[newk+1]/layerthickness[i][j][k+1])*layerwatercont[i][j][k+1];
                watercontnew[newk] = layerwatercont[i][j][k] + layerwatercont[i][j][k+1] - watercontnew[newk+1];
                deltawatercontnew[newk+1] = (thicknessnew[newk+1]/layerthickness[i][j][k+1])*layerdeltawatercont[i][j][k+1];
                deltawatercontnew[newk] = layerdeltawatercont[i][j][k] + layerdeltawatercont[i][j][k+1] - deltawatercontnew[newk+1];
                if (watercontnew[newk] < 0) watercontnew[newk] = 0.; /*Precision problem*/
                densitynew[newk] = massnew[newk]/thicknessnew[newk];
                densitynew[newk+1] = massnew[newk+1]/thicknessnew[newk+1];
                temperaturenew[newk+1] = layertemperature[i][j][k+1];
                temperaturenew[newk] = (layertemperature[i][j][k]*layermass[i][j][k] +
                                        layertemperature[i][j][k+1]*(layermass[i][j][k+1]-massnew[newk+1])) /
                                       (massnew[newk]);
                if ((watercontnew[newk+1] > 0.0) && (temperaturenew[newk+1] < 0.0)) {
                    energywater = watercontnew[newk+1]*Lf;
                    energytemperature = fabs(temperaturenew[newk+1])*massnew[newk+1]*cpice;
                    energy = energywater;
                    if (energy > energytemperature) energy = energytemperature;
                    masschangelayer = energy/Lf;
                    energytemperatureold = temperaturenew[newk+1]*cpice*massnew[newk+1];
                    watercontnew[newk+1] = watercontnew[newk+1] - masschangelayer;
                    deltawatercontnew[newk+1] = deltawatercontnew[newk+1] - masschangelayer;
                    refreezenew[newk+1] = refreezenew[newk+1] + masschangelayer;
                    deltarefreezenew[newk+1] = deltarefreezenew[newk+1] + masschangelayer;
                    if (watercontnew[newk+1] < 0) watercontnew[newk+1] = 0.; /*Precision problem*/
                    massnew[newk+1] = massnew[newk+1] + masschangelayer;
                    densitynew[newk+1] = massnew[newk+1]/thicknessnew[newk+1];
                    temperaturenew[newk+1] = (energytemperatureold + energy)/(massnew[newk+1]*cpice);
                }

                if ((massnew[newk] > 20000.) || (massnew[newk] < 0.) ||
                        (watercontnew[newk] < 0) || (thicknessnew[newk] < bounl) ||
                        (densitynew[newk] < denssnow-1.) || (densitynew[newk] > denswater*1.1) ||
                        (temperaturenew[newk] > 0.) || (temperaturenew[newk] <= surftempminimum))
                    fprintf(outcontrol,"E jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk],massnew[newk],watercontnew[newk],
                            densitynew[newk],temperaturenew[newk]);
                if ((massnew[newk+1] > 20000.) || (massnew[newk+1] < 0.) ||
                        (watercontnew[newk+1] < 0) || (thicknessnew[newk+1] < bounl) ||
                        (densitynew[newk+1] < denssnow-1.) || (densitynew[newk+1] > denswater*1.1) ||
                        (temperaturenew[newk+1] > 0.) || (temperaturenew[newk+1] <= surftempminimum))
                    fprintf(outcontrol,"F jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk+1],massnew[newk+1],watercontnew[newk+1],
                            densitynew[newk+1],temperaturenew[newk+1]);

                idnew[newk] = layerid[i][j][k];
                idnew[newk+1] = layerid[i][j][k+1];
                if ((k == 1) && (layerid[i][j][k+1] > 1)) {
                    idnew[newk] = idnew[newk+1];
                    SNOW[i][j] = 0; /*all snow has melted*/
                    SNOWinit[i][j] = SNOWinit[i][j] - layermass[i][j][1]/10. - layerwatercont[i][j][1]/10.- surfacewater[i][j]/10.;
                }
                depthnew[newk] = dummydepth + 0.5*thicknessnew[newk];
                dummydepth = depthnew[newk] + 0.5*thicknessnew[newk];
                depthnew[newk+1] = dummydepth + 0.5*thicknessnew[newk+1];
                dummydepth = depthnew[newk+1] + 0.5*thicknessnew[newk+1];
                newk = newk + 2;
            /* if new layer is thinner than bounh, just merge two layers; comment by F.Covi 7/16/19 */
            } else {
                massnew[newk] = layermass[i][j][k] + layermass[i][j][k+1];
                refreezenew[newk] = layerrefreeze[i][j][k] + layerrefreeze[i][j][k+1];
                deltarefreezenew[newk] = layerdeltarefreeze[i][j][k] + layerdeltarefreeze[i][j][k+1];
                watercontnew[newk] = layerwatercont[i][j][k] + layerwatercont[i][j][k+1];
                deltawatercontnew[newk] = layerdeltawatercont[i][j][k] + layerdeltawatercont[i][j][k+1];
                densitynew[newk] = massnew[newk]/thicknessnew[newk];
                temperaturenew[newk] = (layertemperature[i][j][k]*layermass[i][j][k] +
                                        layertemperature[i][j][k+1]*layermass[i][j][k+1]) /
                                       (layermass[i][j][k] + layermass[i][j][k+1]);
                if ((massnew[newk] > 20000.) || (massnew[newk] < 0.) ||
                        (watercontnew[newk] < 0) || (thicknessnew[newk] < bounl) ||
                        (densitynew[newk] < denssnow-1.) || (densitynew[newk] > denswater*1.1) ||
                        (temperaturenew[newk] > 0.) || (temperaturenew[newk] <= surftempminimum))
                    fprintf(outcontrol,"G jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk],massnew[newk],watercontnew[newk],
                            densitynew[newk],temperaturenew[newk]);

                depthnew[newk] = dummydepth + 0.5*thicknessnew[newk];
                idnew[newk] = layerid[i][j][k+1];
                dummydepth = depthnew[newk] + 0.5*thicknessnew[newk];
                newk = newk + 1;
            }
        }
        /* 2) deepest layer; comment by F.Covi 7/16/19 */
        if ((layerthickness[i][j][k] < bounl) && (k == (int)layeramount[i][j]) && (fusionyes == 0) &&
                (fusion2yes == 0)) {
            thicknessnew[newk-1] = layerthickness[i][j][k] + thicknessnew[newk-1];
            massnew[newk-1] = layermass[i][j][k] + massnew[newk-1];
            refreezenew[newk-1] = layerrefreeze[i][j][k] + refreezenew[newk-1];
            deltarefreezenew[newk-1] = layerdeltarefreeze[i][j][k] + deltarefreezenew[newk-1];
            watercontnew[newk-1] = layerwatercont[i][j][k] + watercontnew[newk-1];
            deltawatercontnew[newk-1] = layerdeltawatercont[i][j][k] + deltawatercontnew[newk-1];
            densitynew[newk-1] = massnew[newk-1]/thicknessnew[newk-1];
            temperaturenew[newk-1] = (layertemperature[i][j][k]*layermass[i][j][k] +
                                      temperaturenew[newk-1]*massnew[newk-1]) /
                                     (layermass[i][j][k] + massnew[newk-1]);
            depthnew[newk-1] = depthnew[newk-1] + 0.5*layerthickness[i][j][k];
            /*idnew[newk-1] = idnew[k-1];*/
            dummydepth = depthnew[newk-1] + 0.5*thicknessnew[newk-1];
            if ((massnew[newk-1] > 20000.) || (massnew[newk-1] < 0.) ||
                    (watercontnew[newk-1] < 0) || (thicknessnew[newk-1] < bounl) ||
                    (densitynew[newk-1] < denssnow-1.) || (densitynew[newk-1] > denswater*1.1) ||
                    (temperaturenew[newk-1] > 0.) || (temperaturenew[newk-1] <= surftempminimum))
                fprintf(outcontrol,"H jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                        jd2,newk,i,j,thicknessnew[newk-1],massnew[newk-1],watercontnew[newk-1],
                        densitynew[newk-1],temperaturenew[newk-1]);
        }

        /* special cases; comment by F.Covi 7/16/19 */
        if (fusion2yes != 0) {
            /* fusion with layer above; comment by F.Covi 7/16/19 */
            if (fusion2yes == 1) {
                thicknessnew[newk-1] = layerthickness[i][j][k] + thicknessnew[newk-1];
                massnew[newk-1] = layermass[i][j][k] + massnew[newk-1];
                refreezenew[newk-1] = layerrefreeze[i][j][k] + refreezenew[newk-1];
                deltarefreezenew[newk-1] = layerdeltarefreeze[i][j][k] + deltarefreezenew[newk-1];
                watercontnew[newk-1] = layerwatercont[i][j][k] + watercontnew[newk-1];
                deltawatercontnew[newk-1] = layerdeltawatercont[i][j][k] + deltawatercontnew[newk-1];
                densitynew[newk-1] = massnew[newk-1]/thicknessnew[newk-1];
                temperaturenew[newk-1] = (layertemperature[i][j][k]*layermass[i][j][k] +
                                          temperaturenew[newk-1]*massnew[newk-1]) /
                                         (layermass[i][j][k] + massnew[newk-1]);
                depthnew[newk-1] = depthnew[newk-1] + 0.5*layerthickness[i][j][k];
                /*idnew[newk-1] = idnew[k-1];*/
                dummydepth = depthnew[newk-1] + 0.5*thicknessnew[newk-1];
                /*newk = newk - 1;*/
                if ((massnew[newk-1] > 20000.) || (massnew[newk-1] < 0.) ||
                        (watercontnew[newk-1] < 0) || (thicknessnew[newk-1] < bounl) ||
                        (densitynew[newk-1] < denssnow-1.) || (densitynew[newk-1] > denswater*1.1) ||
                        (temperaturenew[newk-1] > 0.) || (temperaturenew[newk-1] <= surftempminimum))
                    fprintf(outcontrol,"I jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk-1],massnew[newk-1],watercontnew[newk-1],
                            densitynew[newk-1],temperaturenew[newk-1]);
                fusionyes = 0;
            /* fusion of the two layers above; comment by F.Covi 7/16/19 */
            } else if (fusion2yes == 2) {
                thicknessnew[newk-2] = thicknessnew[newk-2] + thicknessnew[newk-1];
                massnew[newk-2] = massnew[newk-2] + massnew[newk-1];
                refreezenew[newk-2] = refreezenew[newk-2] + refreezenew[newk-1];
                deltarefreezenew[newk-2] = deltarefreezenew[newk-2] + deltarefreezenew[newk-1];
                watercontnew[newk-2] = watercontnew[newk-2] + watercontnew[newk-1];
                deltawatercontnew[newk-2] = deltawatercontnew[newk-2] + deltawatercontnew[newk-1];
                densitynew[newk-2] = massnew[newk-2]/thicknessnew[newk-2];
                temperaturenew[newk-2] = (temperaturenew[newk-2]*massnew[newk-2] +
                                          temperaturenew[newk-1]*massnew[newk-1]) /
                                         (massnew[newk-2] + massnew[newk-1]);
                depthnew[newk-2] = depthnew[newk-2] + 0.5*thicknessnew[newk-1];
                /*idnew[newk-1] = idnew[k-1];*/
                dummydepth = depthnew[newk-2] + 0.5*thicknessnew[newk-2];
                /*newk = newk - 1;*/
                if ((massnew[newk-2] > 20000.) || (massnew[newk-2] < 0.) ||
                        (watercontnew[newk-2] < 0) || (thicknessnew[newk-2] < bounl) ||
                        (densitynew[newk-2] < denssnow-1.) || (densitynew[newk-2] > denswater*1.1) ||
                        (temperaturenew[newk-2] > 0.) || (temperaturenew[newk-2] <= surftempminimum))
                    fprintf(outcontrol,"I1 jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk-2],massnew[newk-2],watercontnew[newk-2],
                            densitynew[newk-2],temperaturenew[newk-2]);
                thicknessnew[newk-1] = layerthickness[i][j][k];
                massnew[newk-1] = layermass[i][j][k];
                refreezenew[newk-1] = layerrefreeze[i][j][k];
                deltarefreezenew[newk-1] = layerdeltarefreeze[i][j][k];
                if (massnew[newk-1] < 0.) massnew[newk-1] = 0.; /*precision problem*/
                watercontnew[newk-1] = layerwatercont[i][j][k-1];
                deltawatercontnew[newk-1] = layerdeltawatercont[i][j][k-1];
                if (watercontnew[newk-1] < 0.) watercontnew[newk-1] = 0.; /*precision problem*/
                temperaturenew[newk-1] = layertemperature[i][j][k];
                densitynew[newk-1] = layerdensity[i][j][k];
                idnew[newk-1] = layerid[i][j][k];
                depthnew[newk-1] = dummydepth + 0.5 * thicknessnew[newk-1];
                dummydepth = depthnew[newk-1] + 0.5 * thicknessnew[newk-1];
                if ((massnew[newk-1] > 20000.) || (massnew[newk-1] < 0.) ||
                        (watercontnew[newk-1] < 0) || (thicknessnew[newk-1] < bounl) ||
                        (densitynew[newk-1] < denssnow-1.) || (densitynew[newk-1] > denswater*1.1) ||
                        (temperaturenew[newk-1] > 0.) || (temperaturenew[newk-1] <= surftempminimum))
                    fprintf(outcontrol,"I2 jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk-1],massnew[newk-1],watercontnew[newk-1],
                            densitynew[newk-1],temperaturenew[newk-1]);
                fusionyes = 0;
            }
        }

        /* Splitting of 1 layer in two */
        if (((layerthickness[i][j][k] > bounh) && (newk+1 <= ndepths)) || (splityes == 1)) {
            if (k == 1) {  /*creating extra layer of new freshly fallen snow if enough snow has fallen*/
                freshsnowlayerthickness = freshsnowlayer/denssnow;
                freshsnowlayerthicknessold = 0.;
                extrathickness = 0.;
                oldtoplayerthickness = layerthickness[i][j][1] - freshsnowlayerthickness;
                oldtoplayermass = layermass[i][j][1] - freshsnowlayer;
                oldtoplayerdensity = oldtoplayermass / oldtoplayerthickness;

                if (snowlayersum[i][j] == freshsnowlayer) snowlayersum[i][j] = 0.;
                if (snowlayersum[i][j]-freshsnowlayer > 0.) {
                    freshsnowlayerthicknessold = (snowlayersum[i][j]-freshsnowlayer)/ oldtoplayerdensity;
                    oldtoplayerthickness = oldtoplayerthickness - freshsnowlayerthicknessold;
                    oldtoplayermass = layermass[i][j][1] - snowlayersum[i][j];
                    if (oldtoplayermass < 0.) {
                        freshsnowlayerthicknessold = layerthickness[i][j][1] - freshsnowlayerthickness;
                        oldtoplayerthickness = 0.;
                        oldtoplayermass = layermass[i][j][1];
                        snowlayersum[i][j] = snowlayersum[i][j] - layermass[i][j][1];
                    } else
                        snowlayersum[i][j] = 0.;
                    extrathickness = (freshsnowlayerthicknessold*oldtoplayerdensity/denssnow - freshsnowlayerthicknessold);
                }

                freshsnowlayerthicknessnew = freshsnowlayerthickness +
                                             freshsnowlayerthicknessold*oldtoplayerdensity/denssnow;
                thicknessnew[1] = freshsnowlayerthicknessnew;
                if (thicknessnew[1] < bounl) {
                    snowlayersum[i][j] = thicknessnew[1]*denssnow;
                    thicknessnew[1] = bounl;
                }
                thicknessnew[2] = layerthickness[i][j][1] - thicknessnew[1] + extrathickness;
                if (thicknessnew[2] >= bounl) {
                    densitynew[2] = oldtoplayerdensity;
                    massnew[2] = thicknessnew[2] * densitynew[2];
                    massnew[1] = layermass[i][j][1] - massnew[2];
                    densitynew[1] = massnew[1] / thicknessnew[1];
                } else {
                    thicknessnew[2] = bounl;
                    thicknessnew[1] = layerthickness[i][j][1] + extrathickness - thicknessnew[2];
                    massnew[1] = thicknessnew[1] * denssnow;
                    massnew[2] = layermass[i][j][1] - massnew[1];
                    densitynew[1] = denssnow;
                    densitynew[2] = massnew[2]/thicknessnew[2];
                }
                refreezenew[1]= ((oldtoplayermass-massnew[2])/oldtoplayermass)*layerrefreeze[i][j][1];
                refreezenew[2]= layerrefreeze[i][j][1] - refreezenew[1];
                deltarefreezenew[1]= ((oldtoplayermass-massnew[2])/oldtoplayermass)*layerdeltarefreeze[i][j][1];
                deltarefreezenew[2]= layerdeltarefreeze[i][j][1] - deltarefreezenew[1];
                watercontnew[1] = (thicknessnew[1]/(layerthickness[i][j][1]+extrathickness))*layerwatercont[i][j][1];
                watercontnew[2] = (thicknessnew[2]/(layerthickness[i][j][1]+extrathickness))*layerwatercont[i][j][1];
                deltawatercontnew[1] = (thicknessnew[1]/(layerthickness[i][j][1]+extrathickness))*layerdeltawatercont[i][j][1];
                deltawatercontnew[2] = (thicknessnew[2]/(layerthickness[i][j][1]+extrathickness))*layerdeltawatercont[i][j][1];
                temperaturenew[1] = layertemperature[i][j][1];
                temperaturenew[2] = layertemperature[i][j][1];

                if ((massnew[1] > 20000.) || (massnew[1] < 0.) ||
                        (watercontnew[1] < 0) || (thicknessnew[1] < bounl) ||
                        (densitynew[1] < denssnow-1.) || (densitynew[1] > denswater*1.1) ||
                        (temperaturenew[1] > 0.) || (temperaturenew[1] <= surftempminimum))
                    fprintf(outcontrol,"J jd %f k 1 i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,i,j,thicknessnew[1],massnew[1],watercontnew[1],
                            densitynew[1],temperaturenew[1]);
                if ((massnew[2] > 20000.) || (massnew[2] < 0.) ||
                        (watercontnew[2] < 0) || (thicknessnew[2] < bounl) ||
                        (densitynew[2] < denssnow-1.) || (densitynew[2] > denswater*1.1) ||
                        (temperaturenew[1] > 0.) || (temperaturenew[1] <= surftempminimum))
                    fprintf(outcontrol,"K jd %f k 2 i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,i,j,thicknessnew[2],massnew[2],watercontnew[2],
                            densitynew[2],temperaturenew[2]);

                depthnew[1] = 0.5*thicknessnew[1];
                depthnew[2] = thicknessnew[1] + 0.5*thicknessnew[2];

                idnew[1] = 1;
                if ((idnew[1] == 1) && (layerid[i][j][1] != 1)) {
                    SNOW[i][j] = (massnew[1] + watercontnew[1])/10. + surfacewater[i][j]/10.;
                }
                if ((thicknessnew[1] > freshsnowlayerthicknessnew) && (densitynew[1] > (denssnow+100.))) {
                    idnew[1] = layerid[i][j][1];
                    if (idnew[1] > 1) SNOW[i][j] = 0.; /*freshsnowlayer is not thick enough to create new layer*/
                }
                idnew[2] = layerid[i][j][1];
                dummydepth = depthnew[2] + 0.5*thicknessnew[2];
                newk = 3;
                fusionyes = 0;
            } else {
                thicknessnew[newk] = 0.5 * layerthickness[i][j][k];
                massnew[newk] = 0.5 * layermass[i][j][k];
                refreezenew[newk] = 0.5 * layerrefreeze[i][j][k];
                deltarefreezenew[newk] = 0.5 * layerdeltarefreeze[i][j][k];
                watercontnew[newk] = 0.5 * layerwatercont[i][j][k];
                deltawatercontnew[newk] = 0.5 * layerdeltawatercont[i][j][k];
                temperaturenew[newk] = layertemperature[i][j][k];
                densitynew[newk] = layerdensity[i][j][k];
                if ((massnew[newk] > 20000.) || (massnew[newk] < 0.) ||
                        (watercontnew[newk] < 0) || (thicknessnew[newk] < bounl) ||
                        (densitynew[newk] < denssnow-1.) || (densitynew[newk] > denswater*1.1) ||
                        (temperaturenew[newk] > 0.) || (temperaturenew[newk] <= surftempminimum))
                    fprintf(outcontrol,"L jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk],massnew[newk],watercontnew[newk],
                            densitynew[newk],temperaturenew[newk]);
                depthnew[newk] = dummydepth + 0.5 * thicknessnew[newk];
                idnew[newk] = layerid[i][j][k];
                dummydepth = depthnew[newk] + 0.5 * thicknessnew[newk];
                newk = newk + 1;
                thicknessnew[newk] = 0.5 * layerthickness[i][j][k];
                massnew[newk] = 0.5 * layermass[i][j][k];
                refreezenew[newk] = 0.5 * layerrefreeze[i][j][k];
                deltarefreezenew[newk] = 0.5 * layerdeltarefreeze[i][j][k];
                watercontnew[newk] = 0.5 * layerwatercont[i][j][k];
                deltawatercontnew[newk] = 0.5 * layerdeltawatercont[i][j][k];
                temperaturenew[newk] = layertemperature[i][j][k];
                densitynew[newk] = layerdensity[i][j][k];
                if ((massnew[newk] > 20000.) || (massnew[newk] < 0.) ||
                        (watercontnew[newk] < 0) || (thicknessnew[newk] < bounl) ||
                        (densitynew[newk] < denssnow-1.) || (densitynew[newk] > denswater*1.1) ||
                        (temperaturenew[newk] > 0.) || (temperaturenew[newk] <= surftempminimum))
                    fprintf(outcontrol,"M jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk],massnew[newk],watercontnew[newk],
                            densitynew[newk],temperaturenew[newk]);
                depthnew[newk] = dummydepth + 0.5 * thicknessnew[newk];
                idnew[newk] = layerid[i][j][k];
                dummydepth = depthnew[newk] + 0.5 * thicknessnew[newk];
                newk = newk + 1;
                fusionyes = 0;
            }
        } /*End splitting loop*/


        /* Nothing to do */
        if (((layerthickness[i][j][k] >= bounl) && (layerthickness[i][j][k] <= bounh) &&
                (splityes == 0) && (fusion2yes == 0)) || ((fusionyes == 1) && (fusion2yes == 0))) {
            thicknessnew[newk] = layerthickness[i][j][k];
            massnew[newk] = layermass[i][j][k];
            refreezenew[newk] = layerrefreeze[i][j][k];
            deltarefreezenew[newk] = layerdeltarefreeze[i][j][k];
            if (massnew[newk] < 0.) massnew[newk] = 0.; /*precision problem*/
            watercontnew[newk] = layerwatercont[i][j][k];
            deltawatercontnew[newk] = layerdeltawatercont[i][j][k];
            if (watercontnew[newk] < 0.) watercontnew[newk] = 0.; /*precision problem*/
            temperaturenew[newk] = layertemperature[i][j][k];
            densitynew[newk] = layerdensity[i][j][k];
            if (fusionyes == 0) {
                if ((massnew[newk] > 20000.) || (massnew[newk] < 0.) ||
                        (watercontnew[newk] < 0) || (thicknessnew[newk] < bounl) ||
                        (densitynew[newk] < denssnow-1.) || (densitynew[newk] > denswater*1.1) ||
                        (temperaturenew[newk] > 0.) || (temperaturenew[newk] <= surftempminimum))
                    fprintf(outcontrol,"N jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,newk,i,j,thicknessnew[newk],massnew[newk],watercontnew[newk],
                            densitynew[newk],temperaturenew[newk]);
            }
            idnew[newk] = layerid[i][j][k];
            depthnew[newk] = dummydepth + 0.5 * thicknessnew[newk];
            dummydepth = depthnew[newk] + 0.5 * thicknessnew[newk];
            newk = newk + 1;
        }

        /* in case of fusion, skip next layer*/
        if (skipyes == 1)
            k=k+1;

        newklast = newk - 1;
    } /*End loop over old layers */

    if (newklast > ndepths) {
        printf("\n 1 Number of layers exceeds maximum number: %d, new %d, last %d, i %d, j %d\n",ndepths,newk,newklast,i,j);
        printf(" function changegrid in snowmodel.c\n\n");
        fclose(outcontrol);
        exit(3);
    }


    k=1;
    /* loop to assign new grid */
    sum = 0.;
    sumwater = 0.;
    newk = newklast;
    layeramountcold[i][j] = layeramountcold[i][j] + newk-layeramount[i][j];
    layeramount[i][j] = newk;

    for (k = 1; k <= newklast; k++) {
        layerthickness[i][j][k] = thicknessnew[k];
        layermass[i][j][k] = massnew[k];
        layerrefreeze[i][j][k] = refreezenew[k];
        layerdeltarefreeze[i][j][k] = deltarefreezenew[k];
        if (layerrefreeze[i][j][k] < 0.) layerrefreeze[i][j][k] = 0.;
        layerwatercont[i][j][k] = watercontnew[k];
        layerdeltawatercont[i][j][k] = deltawatercontnew[k];
        layerdensity[i][j][k] = densitynew[k];
        layerdepth[i][j][k] = depthnew[k];
        layertemperature[i][j][k] = temperaturenew[k];
        layerid[i][j][k] = idnew[k];
        if ((k == 1) && (layerid[i][j][k] != 1)) SNOW[i][j] = 0;
        layerrhocp[i][j][k] = rhocpice * layerdensity[i][j][k] / densice;
        if ((layermass[i][j][k] > 20000.) || (layermass[i][j][k] < 0.) ||
                (layerwatercont[i][j][k] < 0) || (layerthickness[i][j][k] < 0.) ||
                (layerdensity[i][j][k] < denssnow-1.) || (layerdensity[i][j][k] > denswater*1.1) ||
                (layertemperature[i][j][k] > 0.0) || (layertemperature[i][j][k] <= surftempminimum))
            fprintf(outcontrol,"O jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f \n",
                    jd2,k,i,j,layerthickness[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],
                    layerdensity[i][j][k],layertemperature[i][j][k]);

        if (layerid[i][j][k] == 1) {
            sum = sum + layermass[i][j][k];
            sumwater = sumwater + layerwatercont[i][j][k];
        }
        if ((layerid[i][j][k-1] == 1) && (layerid[i][j][k] != 1) && (k>1)) {
            diff = SNOW[i][j]*10. - sum - sumwater - surfacewater[i][j];
            if (fabs(diff) > 0.000001) {
                fprintf(outcontrol,"jd %.3f int %d i %d j %d k %d Diff %.8f snow %f sumt %f \n",
                        jd2,inter,i,j,k-1,diff,SNOW[i][j]*10.,(sum+sumwater+surfacewater[i][j]));
                fprintf(outcontrol,"melt %f abla %f runoff %f subl %f sumsn %f sumw %f surfw %f \n corrmelt+ %f corrpercol- %f corrain+ %f meltrunoff %f \n precsn %f precwat %f frsn %f \n mass %f thick %f dens %f water %f id %f T %f id %f\n",
                        MELT[i][j],ABLA[i][j],RUNOFF[i][j],SUBLIMATION[i][j],sum,sumwater,surfacewater[i][j],
                        summelt,sumpercolation,sumrain,sumrunoff,
                        snowprec,rainprec,freshsnowlayer,layermass[i][j][k-1],layerthickness[i][j][k-1],
                        layerdensity[i][j][k-1],layerwatercont[i][j][k-1],layerid[i][j][k-1],layertemperature[i][j][k-1],layerid[i][j][k-1]);

                printf("\n %f diff SNOW - (sum+sumwater+surfacewater) (%f) too big, something wrong !\n",jd2,diff);
                printf("  function changegrid in snowmodel.c\n\n");
                fclose(outcontrol);
                exit(3);
            }
            layermass[i][j][k-1] = layermass[i][j][k-1] + diff;
            layerdensity[i][j][k-1] = layermass[i][j][k-1]/layerthickness[i][j][k-1];
            snowlayermsnow(i, j, k);
        }
    }
    k=1;
    meltlayermice(densfirst);
    

    if (dummydepth < depthdeep) {
        newk = newk + 1;
        if (newk <= ndepths ) {
            layerthickness[i][j][newk] = layerthickness[i][j][newklast];
            layermass[i][j][newk] = layermass[i][j][newklast];
            layerrefreeze[i][j][newk] = 0.0;
            layerdeltarefreeze[i][j][newk] = 0.0;
            layerwatercont[i][j][newk] = 0.0;
            layerdeltawatercont[i][j][newk] = 0.0;
            layerdensity[i][j][newk] = layerdensity[i][j][newklast];
            layerdepth[i][j][newk] = layerdepth[i][j][newklast] + 0.5*(layerthickness[i][j][newklast] + layerthickness[i][j][newklast]);
            layertemperature[i][j][newk] = layertemperature[i][j][newklast];
            layerid[i][j][newk] = layerid[i][j][newklast];
            layerrhocp[i][j][newk] = layerrhocp[i][j][newklast];
            if ((layermass[i][j][newk] > 20000.) || (layermass[i][j][newk] < 0.) ||
                    (layerwatercont[i][j][newk] < 0) || (layerthickness[i][j][newk] < 0.) ||
                    (layerdensity[i][j][newk] < denssnow-1.) || (layerdensity[i][j][newk] > denswater*1.1) ||
                    (layertemperature[i][j][newk] > 0.0) || (layertemperature[i][j][newk] <= surftempminimum)) {
                fprintf(outcontrol,"P jd %.3f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                        jd2,newk,i,j,layerthickness[i][j][newk],layermass[i][j][newk],layerwatercont[i][j][newk],
                        layerdensity[i][j][newk],layertemperature[i][j][newk]);
                resoutlines += 1;
                if(resoutlines > resoutlinesmax)
                    toobig_resout();
            }
            if (layerid[i][j][newk] == 1)
                sum=sum+layermass[i][j][newk]/1000.;
            layeramount[i][j] = newk;
        } else {
            newk = newk -1;
            depthadd = 1.5*(depthdeep-dummydepth);
            layerdepth[i][j][newk] = layerdepth[i][j][newklast] + 0.5*depthadd;
            layerthickness[i][j][newk] = layerthickness[i][j][newklast] + depthadd;
            layermass[i][j][newk] = layerdensity[i][j][newklast]*layerthickness[i][j][newk];
//      layerrefreeze[i][j][newk] = layerrefreeze[i][j][newklast];		no change
//      layerwatercont[i][j][newk] = layerwatercont[i][j][newklast];	no change
//      layerdensity[i][j][newk] = layerdensity[i][j][newklast];		no change
//      layertemperature[i][j][newk] = layertemperature[i][j][newklast];	no change
//      layerid[i][j][newk] = layerid[i][j][newklast];	no change
//      layerrhocp[i][j][newk] = layerrhocp[i][j][newklast];	no change
            if ((layermass[i][j][newk] > 20000.) || (layermass[i][j][newk] < 0.) ||
                    (layerwatercont[i][j][newk] < 0) || (layerthickness[i][j][newk] < 0.) ||
                    (layerdensity[i][j][newk] < denssnow-1.) || (layerdensity[i][j][newk] > denswater*1.1) ||
                    (layertemperature[i][j][newk] > 0.0) || (layertemperature[i][j][newk] <= surftempminimum)) {
                fprintf(outcontrol,"Q jd %.3f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                        jd2,newk,i,j,layerthickness[i][j][newk],layermass[i][j][newk],layerwatercont[i][j][newk],
                        layerdensity[i][j][newk],layertemperature[i][j][newk]);
                resoutlines += 1;
                if(resoutlines > resoutlinesmax)
                    toobig_resout();
            }
            if (layerid[i][j][newk] == 1)
                sum=sum+0.5*layermass[i][j][newk]/1000.;
            layeramount[i][j] = newk;
        }
    }

    if (newk > ndepths) {
        printf("\n 2 Number of layers exceeds maximum number: %d, new %d, i %d, j %d, %f %f\n",ndepths,newk,i,j,dummydepth,depthdeep);
        printf(" function changegrid in snowmodel.c\n\n");
        fclose(outcontrol);
        exit(3);
    }

    freearray(idnew,1,ndepths);
    freearraydouble(thicknessnew,1,ndepths);
    freearraydouble(massnew,1,ndepths);
    freearraydouble(refreezenew,1,ndepths);
    freearraydouble(deltarefreezenew,1,ndepths);
    freearraydouble(depthnew,1,ndepths);
    freearraydouble(temperaturenew,1,ndepths);
    freearraydouble(densitynew,1,ndepths);
    freearraydouble(watercontnew,1,ndepths);
    freearraydouble(deltawatercontnew,1,ndepths);

    return;
}

/****************************************************************************/
/* FUNCTION  resetgridwinter                                                     */
/* changes grid at end of mass balance year in order to make multiyear calculation of mb possible*/
/* layerid 1 is set to 2, snow becomes firn*/
/* SNOW set to 0*/
/* Modified by Tobben August 2013                                         */
/****************************************************************************/

void resetgridwinter() {
    int     k,ir,jc;
    int     krows,kcols;
	
	krows = nrows;
    kcols = ncols;
// start new algo TOBBEN
// snow depth (sum of all snow layerthickness[i][j][k] ) is added in meltlayer[][] before all snow is converted to firn.
    if (calcgridyes == 2) {
        krows = 1;
        kcols = 1;
    }
    for (ir = 1; ir <= krows; ir++) {
        for (jc = 1; jc <= kcols; jc++) {
            i = ir;
            j = jc;
            if (calcgridyes == 2) {
                i = rowclim;
                j = colclim;
            }
            if (surface[i][j] != nodis) {
                for (k = 1; k <= (int)layeramount[i][j]; k++) {
                    if (layerid[i][j][k] == 1){ 
												meltlayer[i][j] = meltlayer[i][j] + layerthickness[i][j][k];
                		}    
                }
                
            }
        }
    }
		// end new algo TOBBEN
    initializeglacier2zero_nodatadouble(nrows, ncols, SNOW);
    initializeglacier2zero_nodata_tensdouble(nrows, ncols, ndepths,layerrefreeze);
    if (runoffyes == 1)
        initializeglacier2zero_nodatadouble(nrows, ncols, meanrunoffall);
    if (superyes == 1)
        initializeglacier2zero_nodatadouble(nrows, ncols, meansupericeall);
    if (wateryes == 1)
        initializeglacier2zero_nodatadouble(nrows, ncols, meanwatercontentall);
    if (surfwateryes == 1)
        initializeglacier2zero_nodatadouble(nrows, ncols, meansurfacewaterall);
    if (slushyes == 1)
        initializeglacier2zero_nodatadouble(nrows, ncols, meanslushthicknessall);
    if (coldtotyes == 1) {
        initializeglacier2zero_nodatadouble(nrows, ncols, meancoldcontentsnowall);
        initializeglacier2zero_nodatadouble(nrows, ncols, meancoldcontenticeall);
    }

    krows = nrows;
    kcols = ncols;
    if (calcgridyes == 2) {
        krows = 1;
        kcols = 1;
    }
    for (ir = 1; ir <= krows; ir++) {
        for (jc = 1; jc <= kcols; jc++) {
            i = ir;
            j = jc;
            if (calcgridyes == 2) {
                i = rowclim;
                j = colclim;
            }
            if (surface[i][j] != nodis) {
                for (k = 1; k <= (int)layeramount[i][j]; k++) {
                    if (layerid[i][j][k] == 1) layerid[i][j][k] = 2;
                    if (layerid[i][j][k] == 3 && layerdensity[i][j][k] <= 825.) layerid[i][j][k] = 2;
                    if (layerid[i][j][k] == 2 && layerdensity[i][j][k] > 825.) layerid[i][j][k] = 3;
                }
                changegrid();
            }
        }
    }

    return;
}
/****************************************************************************/
/* FUNCTION  resetgridsummer                                                     */
/* changes grid at start of summer mass balance season in order to make multiyear calculation of mb possible*/
/* SNOWinit set to SNOW*/
/****************************************************************************/

void resetgridsummer() {
    int     ir,jc;
    int     krows,kcols;

    krows = nrows;
    kcols = ncols;
    if (calcgridyes == 2) {
        krows = 1;
        kcols = 1;
    }
    for (ir = 1; ir <= krows; ir++) {
        for (jc = 1; jc <= kcols; jc++) {
            i = ir;
            j = jc;
            if (calcgridyes == 2) {
                i = rowclim;
                j = colclim;
            }
            if (surface[i][j] != nodis) {
                SNOWinit[i][j] = SNOW[i][j];
                accyear[i][j] = 1.0*SNOW[i][j]/100.; /*annual accumulation*/ /*Greuell set to 0.15*/
            }
        }
    }

    return;
}

/****************************************************************************/
/* FUNCTION  outputsubsurflines                                             */
/*   this function prints out subsurf conditions for given location         */
/*   for each time step one row for each layer                              */
/*   called from main once before time step loop and in time loop for each grid cell */
/****************************************************************************/

void outputsubsurflines() {

    float   a[11],b[11]; /*y = a x + b*/
    float   calc[11],diff[11];
    //float   i1[11],j1[11],i2[11],j2[11];  /* L.G.: i1, i2, j1, j2 are unused, can we delete? */
    int     kk,ki,kj;
    int     printyes;
    int     of,cf;
    char dummysubsurf[FNAME_LEN];

    of = 0;
    cf = 0;
    if ((i == rowclim) && (j == colclim)) {
        if ((methodsurftempglac == 2) || ( (methodsurftempglac == 4) && (griddgmglac[i][j] != nodata)) )
            of = 1;
    }
    if ((i == firstrow) && (j == firstcol[firstrow]) && (of == 0)) {
        if ((methodsurftempglac == 2) || ( (methodsurftempglac == 4) && (griddgmglac[i][j] != nodata)) )
            of = 0;
        else
            of = 1;
    }
    if ((i == lastrow) && (j == lastcol[lastrow])) cf = 1;

    choice_outputlines();

    if (klinesmax > 0) {

        if (of == 1) {
            for (kk=1; kk <= klinesmax; kk++) {
                strcpy(dummysubsurf,"subsurfline");   /*add in front of outgridname*/
                if (dat_or_txt == 1)
                    sprintf(outsubsurflinename,"%1d%03d%04d.dat",kk,(int)jdsurface[newday],(int)year);
                if (dat_or_txt == 2)
                    sprintf(outsubsurflinename,"%1d%03d%04d.txt",kk,(int)jdsurface[newday],(int)year);
                strcpy(dummy,outpath);
                strcat(dummy,dummysubsurf);
                strcat(dummy,outsubsurflinename);
                printf("\nsnowmodel output: %f %d %d %s",jd2,i,j,dummy);

                if ((outsubsurfline[kk] = fopen(dummy,"wt")) == NULL) {
                    printf("\n Error in opening outsubsurfline file \n (file snowinput.c) %s \n",dummy);
                    exit(4);
                } /*ENDIF*/
            } /*ENDFOR*/
        }

        for (kk = 1; kk <= klinesmax; kk++) {
            printyes = 0;

            a[kk] = (iline1[kk] - iline2[kk])/(jline1[kk] - jline2[kk]);
            b[kk] = iline1[kk] - a[kk]*jline1[kk];

            if (kk == 1) {
                calc[kk] = a[kk]*j + b[kk];
                diff[kk] = fabs(calc[kk]-i);
                if (diff[kk] < 0.50) printyes = kk;
                ki = i;
                kj = j;
            }
            if (kk > 1) {
                calc[kk] = (i - b[kk])/a[kk];
                diff[kk] = fabs(calc[kk]-j);
                if (diff[kk] < 0.50) printyes = kk;
                ki = j;
                kj = i;
            }

            if (printyes != 0) {
                int k = 1;
                surftempfrommodel();
                //First Layer
                fprintf(outsubsurfline[kk]," %3d %3d %2d 0.000000 0.000000 %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f \n",
                        ki,kj,k,surftemp[i][j],layerdensity[i][j][1],layermass[i][j][1],
                        layerwatercont[i][j][1],layerrefreeze[i][j][1],snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][1]);

                for (k = 2; k <= (int)layeramount[i][j]; k++) {   /*for each layer*/
                    fprintf(outsubsurfline[kk]," %3d %3d %2d %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f \n",
                            ki,kj,k,layerdepth[i][j][k],layerthickness[i][j][k],layertemperature[i][j][k],
                            layerdensity[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],layerrefreeze[i][j][k],
                            snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][k]);
                }   /*endfor  each layer*/

            }/*end printyes*/
        }/*end kk loop, different lines*/

        if (cf == 1) {
            for (kk=1; kk <= klinesmax; kk++) {
                fclose(outsubsurfline[kk]);
            } /*ENDFOR*/
        }

    } /*ENDIF klinesmax loop*/

    return;
}

/****************************************************************************/
/* FUNCTION  iceconductivity                                                */
/*   this function calculates conductivity as a function of density         */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/

void iceconductivity(int i, int j, int k) {
    float density;
    /*** which function used defined by variable typeconduc set in variab.h*/
    /* type of equation used to calculate the conductivity */
    /* 1 = Von Dussen, presented in Mellor, J. Glaciol, 19(81), 15-66, 1977 */
    /*                 or Sturm, J. Glaciol., 43(143), 26-41, 1997 */
    /* 2 = Sturm, J. Glaciol., 43(143), 26-41, 1997 */
    /* 3 = Douville et al., Clim. Dynamics, 12, 21-35, 1995 */
    /* 4 = Jansson, presented in Sturm, J. Glaciol., 43(143), 26-41, 1997 */
    /* 5 = Ostin & Andersson, Int. J. Heat Mass Transfer, 34(4-5), 1009-1017, 1991 */
    /*                 or presented in Sturm, J. Glaciol., 43(143), 26-41, 1997 */

    /* for (k = 1; k <= (int)layeramount[i][j]; k++) */
    density = layerdensity[i][j][k];
    /* density = layerdensity[i][j][k]+(layerwatercont[i][j][k]/layerthickness[i][j][k]);*/
    {
        if ( typeconduc == 1)
            conduc[k] = 0.21e-01 + 0.42e-03 * density + 0.22e-08 * pow(density,3);
        if ( typeconduc == 2)
            conduc[k] = 0.138 - 1.01e-3 * density + 3.233e-6 * pow(density,2);
        if ( typeconduc == 3)
            conduc[k] = 2.2 * pow((density/densice),1.88);
        if ( typeconduc == 4)
            conduc[k] = 0.02093 + 0.7953e-3 * density + 2.512e-12 * pow(density,4);
        if ( typeconduc == 5)
            conduc[k] = -0.00871 + 0.439e-3 * density + 1.05e-6 * pow(density,2);
        /*  conduc[k]=conduc[k]/2;*/
    }

    return;
}

/****************************************************************************/
/* FUNCTION  densification                                                  */
/*   this function calculates densification  of a dry snowpack              */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/

void densification(int i, int j, int k) {
    /*  int  typedens=3;		*/
    float tauday=86400.;     /*amount of seconds in one day*/
    float tauyear=31536000.; /*amount of seconds in one year*/
    float efoldcoef=0.24;   /*exponential e-folding coefficient*/
    float deltat;
    float universal = 8.3144;  /*universal gas constant in J/K/mol */
    float tkel = 273.16;     /*0 point of water in K*/
    double factk, factpow, factk0, factE, factk0G ;
    float Ra,Rv,tgrad;
    float beta = 2.0;		/*was 18*/
    float Deff = 0.000110;
    float p0 = 610.5;
    float lc = 2838000;
    float rv = 461.51;
    float J1,J2;

    /* type of equation used to calculate the densification */
    /* 1 = Herron & Langway, J. Glaciol., 25(93), 373-385 */
    /* 2 = Douville et al., Clim. Dynamics, 12, 21-35, 1995 */
    /* 3 = Herron & Langway, adapted by Li & Zwally, Ann Glaciol., 38, 309-313, 2004*/

    deltat = timestep*3600.;
    if (k == 1) {
        if ((nsteps == 1) && (inter == 1))
            accyear[i][j] = 1.0*SNOW[i][j]/100.; /*annual accumulation*/ /*Greuell set to 0.15*/
    }

    /* Herron & Langway*/
    if ( typedens == 1) {
        if (layerdensity[i][j][k] < 550.) {
            factk = 11.*exp(-10160./(universal * (layertemperature[i][j][k]+tkel)));
            factpow = 1.;
        } else if ((layerdensity[i][j][k] >= 550.) && (layerdensity[i][j][k] < densice-diffdensice)) {
            factk = 575.*exp(-21400./(universal * (layertemperature[i][j][k]+tkel)));
            factpow = 0.5;
        } else {
            factk = 0.;
            factpow = 1.;
        }
        layerdensity[i][j][k] = layerdensity[i][j][k] + (factk * pow(accyear[i][j],factpow) *
                                (densice - layerdensity[i][j][k]) *(deltat/tauyear));
    }

    /* Douville*/
    if ( typedens == 2) {
        layerdensity[i][j][k] = densice + ((layerdensity[i][j][k] - densice) *
                                           exp(-efoldcoef*deltat/tauday));
    }

    /* Herron & Langway, adapted by Li & Zwally*/
    if ( typedens == 3) {
        if ((layerdensity[i][j][k] < densice-diffdensice)) {
            if (layertemperature[i][j][k] < -0.132) {
                factk0G = 8.36*pow(fabs(layertemperature[i][j][k]),-2.061) ;
                factE = 883.8*pow(fabs(layertemperature[i][j][k]),-0.885) ;
            } else {
                factk0G = 542.88;
                factE = 5304.5;
            }
            factk0 = beta*factk0G;
            factk = factk0*exp(-factE/(universal * (layertemperature[i][j][k]+tkel)));
            factpow = 1.;
        } else {
            factk = 0.;
            factpow = 1.;
        }

        if (k == 1)
            tgrad = (surftemp[i][j] - layertemperature[i][j][k])/layerdepth[i][j][k];
        if (k > 1)
            tgrad = (layertemperature[i][j][k]-layertemperature[i][j][k-1]) /
                    (layerdepth[i][j][k]-layerdepth[i][j][k-1]);

        Ra = (factk * pow(accyear[i][j],factpow) * ((densice - layerdensity[i][j][k])/densice));

        if (k == 1) {
            J1 = -Deff*(p0/pow(rv,2))*((lc-rv*(surftemp[i][j]+tkel))/pow((surftemp[i][j]+tkel),3))*
                 exp((lc/rv)*((1/tkel)-(1/(surftemp[i][j]+tkel)))) * tgrad;
            J2 = -Deff*(p0/pow(rv,2))*((lc-rv*(layertemperature[i][j][k]+tkel))/pow((layertemperature[i][j][k]+tkel),3))*
                 exp((lc/rv)*((1/tkel)-(1/(layertemperature[i][j][k]+tkel)))) * tgrad;
            Rv = -(J2-J1)/ layerdepth[i][j][k];
        }
        if (k > 1) {
            J1 = -Deff*(p0/pow(rv,2))*((lc-rv*(layertemperature[i][j][k-1]+tkel))/pow((layertemperature[i][j][k-1]+tkel),3))*
                 exp((lc/rv)*((1/tkel)-(1/(layertemperature[i][j][k-1]+tkel)))) * tgrad;
            J2 = -Deff*(p0/pow(rv,2))*((lc-rv*(layertemperature[i][j][k]+tkel))/pow((layertemperature[i][j][k]+tkel),3))*
                 exp((lc/rv)*((1/tkel)-(1/(layertemperature[i][j][k]+tkel)))) * tgrad;
            Rv = -(J2-J1)/ (layerdepth[i][j][k]-layerdepth[i][j][k-1]);
        }

        layerdensity[i][j][k] = layerdensity[i][j][k] + (Ra + Rv) * (deltat/tauyear) ;
        /*   if ((i==43) && (j==25) && (k < 20)) fprintf(outcontrol,"%f %f %f %f %f %f %f \n",jd2,Ra,Rv,factk,factk0G,factE,accyear[i][j]);
        */
    }


    layerthickness[i][j][k] = layermass[i][j][k]/layerdensity[i][j][k];

    return;
}

/****************************************************************************/
/* FUNCTION  snowmelt                                                       */
/*   this function calculates melting of the snowpack if T > 0              */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/

void snowmelt(int i, int j, int k) {
    double  meltedlayer;
    double  meltedsurflayer;
    float  deltat;
    float  Lf = 334000.0;     /*latent heat of fusion  [J/kg] */
    float  depth;
    int idone;

    deltat = timestep*3600;

    /*depth = snowlayer[i][j];*/
    depth = FIRN[i][j]+snowlayer[i][j];
    if (depth < 0 ) depth = 0.;
    /*if (FIRN[i][j] > 0.)
      depth = layerdepth[i][j][(int)layeramount[i][j]] +
              0.5*layerthickness[i][j][(int)layeramount[i][j]];*/
              
    meltedsurflayer = 0.;
    meltedlayer = 0.;
    if ((skin_or_inter == 0) && (k == 1)) meltedsurflayer = source*deltat/Lf;
    if (layertemperature[i][j][k] > 0.) meltedlayer = layerenergy[k]/Lf;
/*    meltedlayer = layertemperature[i][j][k]*layermass[i][j][k]*cpice/Lf + meltedsurflayer;*/
    meltedlayer = meltedlayer + meltedsurflayer;
    meltenergy[i][j] = meltenergy[i][j] + meltedlayer*Lf/deltat;

    layerwatercont[i][j][k] = layerwatercont[i][j][k] + meltedlayer;

    /*first melt the fresh snowlayer*/
    if ((k == 1) && (snowprec > 0.)) {
        freshsnowlayer = snowprec - meltedlayer;
        if (freshsnowlayer < 0) {
            meltedlayer = -freshsnowlayer;
            freshsnowlayer = 0;
        } else
            meltedlayer = 0.;
    }
    if ((k == 1) && (meltedlayer > 0.) && (snowlayersum[i][j] > 0.)) {
        snowlayersum[i][j] = snowlayersum[i][j] - meltedlayer;
        if (snowlayersum[i][j] < 0.) snowlayersum[i][j] = 0.;
    }
    /*calculate thickness of superimposed ice layer, independent of snow/ice thickness*/
    /*This does not affect meltedlayer*/
    if ( /*(k!= 1) &&*/ (meltedlayer > 0) && (superice[i][j] > 0.) &&
                        ((depth-(superice[i][j]/densice)) < layerdepth[i][j][k]+0.5*layerthickness[i][j][k]) &&
                        ((depth-(superice[i][j]/densice)) > layerdepth[i][j][k]-0.5*layerthickness[i][j][k]) ) {
        superice[i][j] = superice[i][j] - meltedlayer;
        if (superice[i][j] < 0.) superice[i][j] = 0.;
    }
    idone = 0;
    if ( (k ==1) && (meltedlayer > 0) && (superice[i][j] > 0.) &&
            ((depth-(superice[i][j]/densice)) < 0) ) {
        superice[i][j] = superice[i][j] - meltedlayer;
        if (superice[i][j] < 0.) superice[i][j] = 0.;
        idone = 1;
    }
    if ((k == 1) && (meltedlayer > 0) && (superice[i][j] > 0.) &&
            (layerdensity[i][j][1] > densice-diffdensice) && (idone == 0)) {
        superice[i][j] = superice[i][j] - meltedlayer;
        if (superice[i][j] < 0.) superice[i][j] = 0.;
    }
    /*End superimposed ice calculations*/
    /*then melt the first snow/firn/ice layer*/
    layerrefreeze[i][j][k] = layerrefreeze[i][j][k] - (meltedlayer/layermass[i][j][k])*layerrefreeze[i][j][k];
    if (layerrefreeze[i][j][k] < 0.) layerrefreeze[i][j][k] = 0.;
    layermass[i][j][k] = layermass[i][j][k] - meltedlayer;
    layerthickness[i][j][k] = layermass[i][j][k]/layerdensity[i][j][k];
    if (layermass[i][j][k] < 0.) {
        layermass[i][j][k+1] = layermass[i][j][k+1] + layermass[i][j][k];
        layermass[i][j][k] = 0.;
        layerthickness[i][j][k] = 0.;
    }
    if (layertemperature[i][j][k] > 0.) layertemperature[i][j][k] = 0.0;

    return;
}

/****************************************************************************/
/* FUNCTION  refreezing                                                     */
/*   this function calculates refreezing of the melted water                */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/

void refreezing(int i, int j, int k) {
    float   Lf = 334000.0;     /*latent heat of fusion  [J/kg] */
    double  masschangelayer;
    double  energywater;      /*energy released or necessary to convert layerwater to ice or vv*/
    double  energytemperature;/*energy necessary to raise layertemperature to melting level */
    double  energydensity;    /*energy necessary to convert water into ice enough to raise layerdensity to density ice*/
    double  energy;           /*actual energy used (smallest from the above three)*/
/*    double  energytempleft;*/   /*energy left necessary to raise temperature to melting point after refreezing*/

    /*if (FIRN[i][j] > 0.) superice[i][j] = 0.;*/

    layerdeltarefreeze[i][j][k] = 0.;
    if (layerdensity[i][j][k] < densice-diffdensice) {
        energywater = layerwatercont[i][j][k]*Lf;
  /*      energytemperature = fabs(layertemperature[i][j][k])*layermass[i][j][k]*cpice; */
        energytemperature = fabs(layerenergy[k]);
        energydensity = (densice - layerdensity[i][j][k])*layerthickness[i][j][k]*Lf;
        energy = energywater;
        if (energy > energytemperature) energy = energytemperature;
        if (energy > energydensity) energy = energydensity;
        masschangelayer = energy/Lf;
        layerwatercont[i][j][k] = layerwatercont[i][j][k] - masschangelayer;
        if (layerwatercont[i][j][k] < 0) layerwatercont[i][j][k] = 0.; /*precision problem*/
        layermass[i][j][k] = layermass[i][j][k] + masschangelayer;
        layerrefreeze[i][j][k] += masschangelayer;
        layerdeltarefreeze[i][j][k] = masschangelayer;
        /*if (layerid[i][j][k] == 2) sumMASS[i][j] += masschangelayer;*/

        if ((slushdepth[i][j] > 0.) && (slushdepth[i][j] < (layerdepth[i][j][k]+0.5*layerthickness[i][j][k]))) {
            superice[i][j] = superice[i][j] + masschangelayer;
            /*if (layerid[i][j][k] == 1) superice[i][j] = superice[i][j] + densice*masschangelayer/(densice - layerdensity[i][j][k]);
            if (layerid[i][j][k] != 1) superice[i][j] = superice[i][j] + densice*masschangelayer/(densice - denssnow);*/
        }

        layerdensity[i][j][k] = layermass[i][j][k]/layerthickness[i][j][k];
/*        energytempleft = energytemperature - energy;
        if (energytempleft < 0) energytempleft = 0.;*/ /*precision*/
  /*      layertemperature[i][j][k] = -1.0*energytempleft/(layermass[i][j][k]*cpice);*/
//         layertemperature[i][j][k] = layertemperature[i][j][k] + Lf*energy/(layermass[i][j][k]*cpice);
         /* This formulation looks suspect to me, try the following, F.C. 8/13/19 */
        layertemperature[i][j][k] = layertemperature[i][j][k] + energy/(layermass[i][j][k]*cpice);

        if (layertemperature[i][j][k] > 0.) layertemperature[i][j][k] = 0.; /*precision problem ??? F.C.*/
    }
    if ((k == 1) && (layertemperature[i][j][k] < 0.0) && (layerwatercont[i][j][k] > 0.0)) {
        energywater = layerwatercont[i][j][k]*Lf;
        energytemperature = fabs(layertemperature[i][j][k])*1.0*layermass[i][j][k]*cpice;
        if (skin_or_inter == 1) energytemperature = fabs(layertemperature[i][j][k])*1.5*layermass[i][j][k]*cpice;
        energy = energywater;
        if (energy > energytemperature) energy = energytemperature;
        masschangelayer = energy/Lf;
        layerwatercont[i][j][k] = layerwatercont[i][j][k] - masschangelayer;
        if (layerwatercont[i][j][k] < 0.) layerwatercont[i][j][k] = 0.; /*precision problem*/
        layermass[i][j][k] = layermass[i][j][k] + masschangelayer;
        layerthickness[i][j][k] = layerthickness[i][j][k] + masschangelayer/densice;
        layerdensity[i][j][k] = layermass[i][j][k]/layerthickness[i][j][k];
/*        energytempleft = energytemperature - energy;
        if (energytempleft < 0) energytempleft = 0.; *//*precision*/
/*        layertemperature[i][j][k] = -1.0*energytempleft/(layermass[i][j][k]*cpice);*/
         layertemperature[i][j][k] = layertemperature[i][j][k] + Lf*energy/(layermass[i][j][k]*cpice); 
         if (layertemperature[i][j][k] > 0.) layertemperature[i][j][k] = 0.;
        superice[i][j] = superice[i][j]+masschangelayer;
        layerdeltarefreeze[i][j][k] += masschangelayer;
        surfacewater[i][j] = 0.;
    }

    return;
}

/****************************************************************************/
/* FUNCTION  percolation                                                    */
/*   this function calculates percolation of the melted water               */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/

void percolation(int i, int j, int k) {
    double  waterchangelayer;
    double  airvolumeice;     /*volume in ice available to store water in*/
    double  minwatercont;     /*if water present this is the minimum amount always present*/
    /* L.G.: maxwatercont is unused, can we delete? */
    // double  maxwatercont;     /*maximum possible water content before runoff occurs*/
    /*sumpercolation is melted snow that percolates into firn layer */
    /*summelt is melted ice, does contribute to melt but is not allowed to impact SNOW*/

    airvolumeice = ((densice - layerdensity[i][j][k])/densice)*layerthickness[i][j][k];
    minwatercont = airvolumeice*irrwatercont*denswater;
    if (layerdensity[i][j][k] >= densice) minwatercont = 0.0;
    if (layerwatercont[i][j][k] > minwatercont) {
        waterchangelayer = layerwatercont[i][j][k] - minwatercont;
        layerwatercont[i][j][k] = layerwatercont[i][j][k] - waterchangelayer;
        if (layerwatercont[i][j][k] < 0) layerwatercont[i][j][k] = 0.; /*precision problem*/

        if (k != (int)layeramount[i][j]) {  /*for any layer but not the deepest*/
            layerwatercont[i][j][k+1] = layerwatercont[i][j][k+1] + waterchangelayer;

            if ((layerid[i][j][k] == 1) && (layerid[i][j][k+1] != 1))  /*1=snow, 2=firn, 3=ice*/
                sumpercolation = waterchangelayer;
        } else { /*lowest layer*/
            sumrunoff = sumrunoff + waterchangelayer;
            if (layerid[i][j][k] != 1)    /*firn and ice*/
                summelt = summelt + waterchangelayer;
        }
        if (summelt == sumpercolation) {
            summelt = 0.;
            sumpercolation = 0.;
        }
    }

    return;
}

/****************************************************************************/
/* FUNCTION  slushformation                                                 */
/*   this function calculates slush formation and surface water             */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/

void slushformation() {
//    int     kmax,kk;
    int     kk;
    /*  double  timec1,timec2,timec3,factc4; */
    double  tanslope;
    double  c1wat,c2wat,c3wat;
    double  timestar;
    double  timefactsurf,timefactint;
    double  slushlayer,watersurplus;
    double  airvolumeice,maxwatercont;
    double  watercontold, refreezeold;
//    double  watercontold,layerwet;
    double  layerwet;
    double  depth;
    int k;

//  timec1 = 0.05;	time scale for runoff of surficial water on a steep surface (days)*/
//  timec2 = 20.;		/*time scale for runoff of surficial water on a horizontal surface (days)*/
//  timec3 = 2.;		/*time scale for runoff of surficial water on a surface with slope of 1 degree (days)*/
//  factc4 = 10.;  		/*factor with which surface water runsoff faster compared to internal runoff*/
    tanslope = tan(pi*SLOPE[i][j]/180.);
    c1wat = timec1;
    c2wat = timec2 - timec1;
    c3wat = -log((timec3-c1wat)/c2wat) / tan(pi/180.0);
    timestar = c1wat+c2wat*exp(-c3wat*tanslope);	/*timescale in days*/
    timefactsurf = (timestep/24.)/timestar;		/*timescale per timestep for surface runoff*/
    if (timefactsurf > 1.) timefactsurf = 1.;	/*1 = immediate runoff*/
    timefactint = timefactsurf/factc4;		/*timescale per timestep for runoff in snow, slower than on surface*/

    slushlayer = 0.0;
    depth = layerdepth[i][j][(int)layeramount[i][j]] + 0.5*layerthickness[i][j][(int)layeramount[i][j]];
    
    surfacewater[i][j] = 0.;
//    printf("\n %f %f %f ",jd2,surfacewaterold,sumrunoff);

    if (sumrunoff >= 0.) {
//        kmax = (int)layeramount[i][j];
        watersurplus = sumrunoff*(1-timefactint);
        sumrunoff = sumrunoff*timefactint;
        if (sumrunoff <= summelt) summelt = sumrunoff;

        for (k=(int)layeramount[i][j]; k > 0; k--) {
            airvolumeice = ((densice - layerdensity[i][j][k])/densice)*layerthickness[i][j][k];
            maxwatercont = airvolumeice*denswater;
            if (layerdensity[i][j][k] > densice-diffdensice) {
                maxwatercont = 0.;
                depth = layerdepth[i][j][k] - 0.5*layerthickness[i][j][k];
            }

            watercontold = layerwatercont[i][j][k];
            layerwatercont[i][j][k] = layerwatercont[i][j][k] + watersurplus;
            if ((layerid[i][j][k] == 1) && ( layerid[i][j][k+1] != 1) && (k != layeramount[i][j])) {
                if ((summelt > 0.) && (sumpercolation == 0))
                    sumpercolation = sumpercolation - watersurplus;
                else if (sumpercolation > 0.)
                    sumpercolation = sumpercolation - watersurplus;
            }

            if ((layertemperature[i][j][k] < 0.) && (maxwatercont > 0.)) { /*maxwatercont = 0.0;*/
                refreezeold = layerdeltarefreeze[i][j][k];
                refreezing(i, j, k);
                layerdeltarefreeze[i][j][k] = layerdeltarefreeze[i][j][k] + refreezeold;
                deltaREFREEZE[i][j] = deltaREFREEZE[i][j] + layerdeltarefreeze[i][j][k];
                /* fprintf(outcontrol,"\n extra refreezing %f %d %d %d %f",jd2,i,j,k,layerid[i][j][k]);*/
            }

            kk = k;
            if (layerwatercont[i][j][k] > maxwatercont) {
                if ((layerdensity[i][j][k] < densice-diffdensice) || (layerid[i][j][k] != 3)) {
                    slushlayer = slushlayer+layerthickness[i][j][k];
                    depth = layerdepth[i][j][k] - 0.5*layerthickness[i][j][k];
                }
                watersurplus = layerwatercont[i][j][k] - maxwatercont;
                layerwatercont[i][j][k] = maxwatercont;
                if (k == 1) {
                    surfacewater[i][j] = watersurplus*(1-timefactsurf);
                    slushthickness[i][j] = slushlayer; /*thickness of slush layer*/
                    slushdepth[i][j] = 0.; /*depth of slush layer top in m snow/ice*/
                    if (layerid[i][j][1] == 3) {
                        slushthickness[i][j] = 0.;
                        slushdepth[i][j] = -99.;
                    }
                    sumrunoff = sumrunoff + watersurplus*timefactsurf;
                    if (layerid[i][j][k] != 1) summelt = sumrunoff;
                    kk = 0;
                }
            } else {
                surfacewater[i][j] = 0.;
                watersurplus = 0;
                depth = layerdepth[i][j][k] - 0.5*layerthickness[i][j][k];
                /*if (watercontold < maxwatercont)
                { layerwet = ((layerwatercont[i][j][k] - watercontold)/(maxwatercont - watercontold)) * layerthickness[i][j][k];
                }
                else
                {*/
                layerwet = (layerwatercont[i][j][k]/maxwatercont)*layerthickness[i][j][k];
                /*}*/
                slushthickness[i][j] = slushlayer + layerwet;
                slushdepth[i][j] = depth + (layerthickness[i][j][k] - layerwet);
                kk = 0;
            }
            layerdeltawatercont[i][j][k] = layerdeltawatercont[i][j][k] + layerwatercont[i][j][k] - watercontold;
            deltaWATER[i][j] = deltaWATER[i][j] + layerwatercont[i][j][k] - watercontold;
            k = kk;
        }/*END layerloop*/
    }

    return;
}

/****************************************************************************/
/* FUNCTION  irreducible_schneider                                          */
/*   this function calculates the irreducible water content according to    */
/*   measurements of Storglaciaren presented by Schneider and Jansson, 2004 */
/*   J. Glaciol., 50(168), 25-34                                            */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/

void irreducible_schneider(int i, int j, int k) {
    double porosity;
    /* L.G.: gravwatercont is unused, can we delete it? */
    //double gravwatercont;  /*gravimetric water content*/
    double dencap;		/* density of capillary water (kg/m3)*/
    double denpor;		/* density of water when all pores are filled completely (kg/m3)*/
    double dencol;		/* density of water when maximum amount according to Coleou is filled (kg/m3)*/
    double irrwater;	/*irreducible water content in % of mass according to schneider
                          (mass of water devided by sum of masses of water and snow) */

    /*New description*/
    if (layerdensity[i][j][k] > densice - diffdensice) {
        dencap = 0.;
        irrwatercont = 0.;
    } else {
        porosity = (densice - layerdensity[i][j][k])/densice;
        irrwater = 0.0143*exp(3.3*porosity);
        dencol = irrwater/(1. - irrwater)*layerdensity[i][j][k];
        denpor = porosity*denswater;
        dencap = dencol;
        if (dencap > denpor) dencap = denpor;

        irrwatercont = dencap/(porosity*denswater);
        /* irrwatercont = irrwater;  /* F.Covi test, 8/8/19 */
    }
    /*irrwatercont = irrwatercont / 2.;*/

    return;
}

/****************************************************************************/
/* FUNCTION  irreducible_coleou                                             */
/*   this function calculates the irreducible water content according to    */
/*   Coleou and Lesaffre, 1998                                              */
/*   Ann. Glaciol., 26, 64-68                                               */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/

void irreducible_coleou(int i, int j, int k) {
    double porosity;
    double irrwater;	/*irreducible water content in % of mass according to coleou
                          (mass of water devided by sum of masses of water and snow) */
    double dencap;		/* density of capillary water (kg/m3)*/
    double denpor;		/* density of water when all pores are filled completely (kg/m3)*/
    double dencol;		/* density of water when maximum amount according to Coleou is filled (kg/m3)*/

    if (layerdensity[i][j][k] > densice - diffdensice) {
        dencap = 0.;
        irrwatercont = 0.;
    } else {
        porosity = (densice - layerdensity[i][j][k])/densice;
        irrwater = ((0.057*porosity)/(1.0-porosity)) + 0.017;
        dencol = irrwater/(1. - irrwater)*layerdensity[i][j][k];
        denpor = porosity*denswater;
        dencap = dencol;
        if (dencap > denpor) dencap = denpor;

        irrwatercont = dencap/(porosity*denswater);
    }

    return;
}

/****************************************************************************/
/* FUNCTION  subsurf                                                        */
/*   this function calculates snow temperature                              */
/*   called from main for each grid cell                                    */
/****************************************************************************/

void subsurf() {
    float   deltat;
    float   stcrit;
    double  dzl;
    double  factG,factGa,factGb,term1,term2;
//    double  watercontold;
    int     factsource=1;   /*fraction of enbal heating/cooling the first layer was 0.65 */
    float   Lf = 334000.0;     /*latent heat of fusion  [J/kg] */
    int k;
    double *layerwatercontold;
    double  surfacewaterold;  
    
    layerwatercontold=arrayreservdouble(1,ndepths);

    jd2=jd;
    if ((zeit == 24) && (inter == factinter))
        jd2 = (int)jd + 1;

    deltat = timestep*3600;

    /* Assume no radiation penetration*/
/*    source = 0.;*/
    tsurfenergybalance(surftemp[i][j]);
    source = balancetsurf;		/*this includes all fluxes and is based on Tsurf of this time step*/
    if (skin_or_inter == 1) source = -1.*ENBAL[i][j]; /*this includes energy from rain excludes ICEHEAT*/

    /*printf(" recalculate subsurface temperatures. row  %d  col  %d  \n\n",i,j);*/
    surfacewaterold = surfacewater[i][j];
    for (k=1; k <= (int)layeramount[i][j]; k++) {   /*for each layer first temperature calculation*/
        layerwatercontold[k] = layerwatercont[i][j][k];
        iceconductivity(i, j, k);
        layerenergy[k] = 0.;
        if (k == 1) {
        conducdtdz[k] = conduc[k] * (layertemperature[i][j][k] - surftemp[i][j])/layerthickness[i][j][k];	 /*Newly added*/
     }
        else /*if (k > 1)*/ {
/*            conducdtdz[k-1] = -0.5 * (conduc[k-1] + conduc[k]) *
                              (layertemperature[i][j][k]-layertemperature[i][j][k-1]) /
                              (layerdepth[i][j][k]-layerdepth[i][j][k-1]); */ /*OLD FORMULATION*/
            /* NOTE: minus sign  old formulation because sign convention flux is opposite to grid of subsurface */
            dzl = 0.5*(layerthickness[i][j][k] + layerthickness[i][j][k-1]);
            conducdtdz[k] = (0.5/dzl)*(conduc[k]*layerthickness[i][j][k] + conduc[k-1]*layerthickness[i][j][k-1])*
                                     (layertemperature[i][j][k]-layertemperature[i][j][k-1]) / dzl;         
        }

    }  /*endfor next layer*/

//    if (skin_or_inter == 0) {
      if (tsurfextrapolation == 1) {
        factG = conducdtdz[1];  }
      else if (tsurfextrapolation == 2) {
        factGa = conduc[1]*(surftemp[i][j]-layertemperature[i][j][1])/layerthickness[i][j][1];
        term1 = ((conduc[1]*layerthickness[i][j][1]+conduc[2]*layerthickness[i][j][2])/
                 (layerthickness[i][j][1]+layerthickness[i][j][2]));
        term2 = ((layertemperature[i][j][1]-layertemperature[i][j][2])/
                 (0.5*(layerthickness[i][j][1]+layerthickness[i][j][2])));
        factGb = term1*term2;
        factG = -(layerthickness[i][j][1]*(2.*factGa - factGb) + layerthickness[i][j][2]*factGa)/
                (layerthickness[i][j][1]+layerthickness[i][j][2]);
      }
//    }

    /*boundary condition, no heat flux to or from lower layers*/
/*    conducdtdz[(int)layeramount[i][j]] = 0.0; */ /* with above formulation now redundant new boundary condition set below*/
             
    for (k=1; k <= (int)layeramount[i][j]-1; k++) {

        /* First calculate temperature */
      if (skin_or_inter == 1 && k == 1 ) {
         /* First calculate temperature first layer*/
        layertemperature[i][j][k] = layertemperature[i][j][k] + 
                               (deltat/(0.5*(layerrhocp[i][j][k]+layerrhocp[i][j][k+1])))*
                               ((conducdtdz[k+1] - source* factsource)/layerthickness[i][j][k]); 
        layerenergy[k] = layerrhocp[i][j][k]*1.0*layerthickness[i][j][k]*layertemperature[i][j][k];		/*negative value for T below 0degC*/
        if ((layertemperature[i][j][k] > 0.0) && (percolationyes == 0)) layertemperature[i][j][k] = 0.0;
         if (layertemperature[i][j][k] < surftempminimum) {
           fprintf(outcontrol,"1 jd %f i %d j %d k 1 temp %f \n",jd2,i,j,layertemperature[i][j][1]);
           layertemperature[i][j][k] = surftempminimum;
           resoutlines += 1;
           if(resoutlines > resoutlinesmax) toobig_resout();
          }
         }
        else {
           layertemperature[i][j][k] = layertemperature[i][j][k] + 
                               (deltat/(0.5*(layerrhocp[i][j][k]+layerrhocp[i][j][k+1])))*
                               ((conducdtdz[k+1]-conducdtdz[k])/layerthickness[i][j][k]); 
            if (k == 1) {
              layerenergy[k] = layerrhocp[i][j][k]*1.5*layerthickness[i][j][k]*layertemperature[i][j][k];}
            else {
              layerenergy[k] = layerrhocp[i][j][k]*layerthickness[i][j][k]*layertemperature[i][j][k];}
            if ((layertemperature[i][j][k] > 0.0) && (percolationyes == 0)) layertemperature[i][j][k] = 0.0;
            if (layertemperature[i][j][k] < surftempminimum) {
                fprintf(outcontrol,"2 jd %f i %d j %d k %d temp %f \n",jd2,i,j,k,layertemperature[i][j][k]);
                layertemperature[i][j][k] = surftempminimum;
                resoutlines += 1;
                if(resoutlines > resoutlinesmax) toobig_resout();
            }
        }  /*endif*/
    }  /* end do over layers*/ 
        layertemperature[i][j][(int)layeramount[i][j]] = layertemperature[i][j][(int)layeramount[i][j] -1];
        layerenergy[(int)layeramount[i][j]] = layerenergy[(int)layeramount[i][j] -1];
        /* new boundary condition formulation, no heat flux to or from lower layers */
        /*  temperature profile re-calculated done*/
        if (skin_or_inter == 1) source = -source;
        if (surftemp[i][j] < 0.) source = 0;	/*source is now only melt energy, not energy used for heating*/
        if (surftemp[i][j] >= 0. && source < 0.) {
        if (skin_or_inter == 0) fprintf(outcontrol,"Tskin == 0., but source < 0.: %f %d %d %f %f \n",jd2,i,j,surftemp[i][j],source);
        source = 0.;}

    for (k=1; k <= (int)layeramount[i][j]; k++) {   /*for each layer now snow content calculation*/

        if (percolationyes == 1) {
            /* melting of the snow/firn/ice layer */
            if (k == 1) {
                freshsnowlayer = snowprec;
                sumrain = rainprec;
                layerwatercont[i][j][k] = layerwatercont[i][j][k] + sumrain;
                meltenergy[i][j] = 0.;
            }
            if (skin_or_inter == 0) {/* skin layer formulation*/
              if ((k == 1 && surftemp[i][j] >= 0.) || (layertemperature[i][j][k] > 0.0))
               snowmelt(i, j, k);
            }
            else { /*skin or inter == 1 (extrapolation of upperlayers)*/
              if (layertemperature[i][j][k] > 0.0) snowmelt(i, j, k);}

/*            if ((skin_or_inter == 0 && layertemperature[i][j][k] >= 0.0) || 
                (skin_or_inter == 1 && layertemperature[i][j][k] > 0.0 )) {
                snowmelt(i, j, k);
                 }*/
            if ((layermass[i][j][k] > 20000.) || (layermass[i][j][k] < 0.) ||
                    (layerwatercont[i][j][k] < 0) || (layerthickness[i][j][k] < 0.) ||
                    (layerdensity[i][j][k] < denssnow-1.) || (layerdensity[i][j][k] > denswater*1.1) ||
                    (layertemperature[i][j][k] > 0.0) || (layertemperature[i][j][k] <= surftempminimum)) {
                fprintf(outcontrol,"A jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                        jd2,k,i,j,layerthickness[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],
                        layerdensity[i][j][k],layertemperature[i][j][k]);
                resoutlines += 1;
                if(resoutlines > resoutlinesmax) toobig_resout();
            }

            /* refreezing of the meltwater that percolated */
            /* with refreezing the temperature of the layer will rise*/
            /* so much water will refreeze until the layertemperature is 0*/
            /* the rest of the water will than percolate leaving a minimal amount of water behind*/
            /*Define some output gridded fields*/
            if (k == 1) {
                layerwatercont[i][j][1] = layerwatercont[i][j][1] + surfacewater[i][j];
            }
            layerdeltarefreeze[i][j][k] = 0.;
            if ((layerwatercont[i][j][k] > 0.0) && (layertemperature[i][j][k] < 0.)) {
              refreezing(i, j, k);
                if ((layermass[i][j][k] > 20000.) || (layermass[i][j][k] < 0.) ||
                        (layerwatercont[i][j][k] < 0) || (layerthickness[i][j][k] < 0.) ||
                        (layerdensity[i][j][k] < denssnow-1.) || (layerdensity[i][j][k] > denswater*1.1) ||
                        (layertemperature[i][j][k] > 0.0) || (layertemperature[i][j][k] <= surftempminimum)) {
                    fprintf(outcontrol,"B jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,k,i,j,layerthickness[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],
                            layerdensity[i][j][k],layertemperature[i][j][k]);
                    resoutlines += 1;
                    if(resoutlines > resoutlinesmax)
                        toobig_resout();
                }
            }

            if (k == 1) sumMASS[i][j] = 0.;
            if (layerid[i][j][k] == 2) sumMASS[i][j] += layerrefreeze[i][j][k];

            /* water then can percolate*/
            if (k == 1) {
                sumrunoff = 0.0;
                sumpercolation = 0.0;
                summelt = 0.0;
            }
            if (layerwatercont[i][j][k] > 0.0) {
                if (irrwatercontyes == 1) irreducible_schneider(i, j, k);
                if (irrwatercontyes == 2) irreducible_coleou(i, j, k);
                percolation(i, j, k);
                if ((layermass[i][j][k] > 20000.) || (layermass[i][j][k] < 0.) ||
                        (layerwatercont[i][j][k] < 0) || (layerthickness[i][j][k] < 0.) ||
                        (layerdensity[i][j][k] < denssnow-1.) || (layerdensity[i][j][k] > denswater*1.1) ||
                        (layertemperature[i][j][k] > 0.0) || (layertemperature[i][j][k] <= surftempminimum)) {
                    fprintf(outcontrol,"C jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                            jd2,k,i,j,layerthickness[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],
                            layerdensity[i][j][k],layertemperature[i][j][k]);
                    resoutlines += 1;
                    if(resoutlines > resoutlinesmax)
                        toobig_resout();
                }  /*endif resout*/
            }
        }/* END if percolation is allowed */

        /*Define some output gridded fields*/
        if (k == 1) {
//            watercontold = watercontent[i][j];
            watercontent[i][j] = 0.;
            deltaWATER[i][j] = 0.;
            deltaREFREEZE[i][j] = 0.;
            capwatercontent[i][j] = 0.;
            slwatercontent[i][j] = 0.;
            coldcontentsnow[i][j] = 0.;
            coldcontentice[i][j] = 0.;
        }
        watercontent[i][j] += layerwatercont[i][j][k];
        layerdeltawatercont[i][j][k] = layerwatercont[i][j][k] - layerwatercontold[k]; /*pos = gain of water*/
        deltaWATER[i][j] += layerdeltawatercont[i][j][k];
//        if (k == (int)layeramount[i][j])
//        printf("\n %f %d %f %f %f %f %f %f %f ",jd2,k,deltaWATER[i][j],watercontent[i][j],watercontold,watercontold2,layerdeltawatercont[i][j][k],layerwatercont[i][j][k],layerwatercontold[k]);
        
        deltaREFREEZE[i][j] += layerdeltarefreeze[i][j][k];
        if ((k <= layeramountcold[i][j]) && (layerid[i][j][k] != 1)) {
            capwatercontent[i][j] += layerwatercont[i][j][k];
            slwatercontent[i][j] += layerwatercont[i][j][k];
        }

        if (layerid[i][j][k] == 1)
            coldcontentsnow[i][j] += fabs(layertemperature[i][j][k])*layermass[i][j][k]*cpice/deltat;
        if ((k <= layeramountcold[i][j]) && (layerid[i][j][k] != 1))
            coldcontentice[i][j] += fabs(layertemperature[i][j][k])*layermass[i][j][k]*cpice/deltat;

        /* if ((k <= layeramountcold[i][j]) && (layerid[i][j][k] != 1) && (layerdensity[i][j][k] < densice))
         { if (jd2 < 243.0)
             coldcontentice[i][j] += fabs(layertemperature[i][j][k])*layermass[i][j][k]*cpice/deltat;
           if (jd2 >= 243.0)
            { if (irrwatercontyes == 1) irreducible_schneider();
              coldcontentice[i][j] += irrwatercont*(1-layerdensity[i][j][k]/densice)*layerthickness[i][j][k];
            }
         } */

        /* finally densification of the dry snowpack*/
        if (densificationyes == 1) {
            densification(i, j, k);
            if ((layermass[i][j][k] > 20000.) || (layermass[i][j][k] < 0.) ||
                    (layerwatercont[i][j][k] < 0) || (layerthickness[i][j][k] < 0.) ||
                    (layerdensity[i][j][k] < denssnow-1.) || (layerdensity[i][j][k] > denswater*1.1) ||
                    (layertemperature[i][j][k] > 0.0) || (layertemperature[i][j][k] <= surftempminimum)) {
                fprintf(outcontrol,"D jd %f k %d i %d j %d thickness %.3f mass %.3f water %.3f density %.3f temperature %.3f\n",
                        jd2,k,i,j,layerthickness[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],
                        layerdensity[i][j][k],layertemperature[i][j][k]);
                resoutlines += 1;
                if(resoutlines > resoutlinesmax)
                    toobig_resout();
            }  /*endif*/
        } /* END if densification is allowed */

    }/* END layer loop */

    /*possibility of slush formation, amount depends on slope, no lateral movement possible*/
    /*   if ((slushformationyes == 1) && (layerdensity[i][j][(int)layeramount[i][j]]))*/
    slushdepth[i][j] = -99.;
    slushthickness[i][j] = 0.;
//    printf("\n A %f %f %f %f %f %f ",jd2,surfacewater[i][j],layerwatercont[i][j][1],layerdensity[i][j][1],sumrunoff,watercontent[i][j]);
//    if ((slushformationyes == 1) && (/*watercontent[i][j] +*/ sumrunoff > 0.) /* &&
//       (layerdensity[i][j][(int)layeramount[i][j]] >= densice-diffdensice)*/ )
//        /*Allways allow slush to build up, even if lowest layer has density smaller than ice */
    if (slushformationyes == 1)
    {
        watercontent[i][j] = watercontent[i][j] + sumrunoff;
        slwatercontent[i][j] = slwatercontent[i][j] + sumrunoff;
        slushformation();
        watercontent[i][j] = watercontent[i][j] - sumrunoff;
        slwatercontent[i][j] = slwatercontent[i][j] - sumrunoff;
    }
    deltaWATER[i][j] = deltaWATER[i][j] + surfacewater[i][j] - surfacewaterold;

    /* sumpercolation is melted snow that percolates into firn layer*/
    /* not mass in SNOW layer any more but not lost through melt either*/
    /* therefore SNOW corrected for this loss*/
    /* summelt is melted ice, does contribute to melt but is not allowed to impact SNOW*/
    /* sumrain is rain water added to the water content of the snow/firn layer*/
    /* must also be added to SNOW*/
    /* sumrunoff at this point is all water leaving the snowpack */
    if (percolationyes == 1) {
        RUNOFF[i][j] = sumrunoff;
        if (sumrain > 0.) {
            if (sumrunoff >= sumrain) {
                sumrunoff = sumrunoff - sumrain;
                sumrain = 0.;
            } else {
                sumrain = sumrain - sumrunoff;
                sumrunoff = 0.;
            }
        }
//        MELT[i][j] = sumrunoff;	/* = meltwater partition in RUNOFF */
        MELT[i][j] = meltenergy[i][j]*deltat/Lf;
        /*     if (SNOW[i][j] > 0.) fprintf(outcontrol," %f %d %d %f %f %f %f %f %f %f \n",
                       jd2,i,j,SNOW[i][j],RUNOFF[i][j],sumrunoff,sumrain,rainprec,summelt,sumpercolation);*/
        SNOW[i][j] = SNOW[i][j] + summelt/10. - sumpercolation/10. + sumrain/10.;
//        RAIN[i][j] = RAIN[i][j] - sumrain;
    }
    /* RUNOFF = all water leaving the snowpack -- Keep as is*/
    /* MELT = water leaving the snow pack due to melt not rain -- change into actual amount of melt taken place*/
    /* RAIN = water in the snow pack that was original rain -- change into actual amount of water entering column*/

    /* modified by F.Covi 7/12/19, ground heat flux UPDATE*/
//    if (skin_or_inter == 0) {
//        ICEHEAT[i][j] = -factG ;
//        energybalance();
//        }
//    else {
//        ICEHEAT[i][j] = ENBAL[i][j] - meltenergy[i][j];//source - MELT[i][j]*Lf/deltat;
//        energybalance();
//    }
    if (skin_or_inter == 1) {
        ICEHEAT[i][j] = ENBAL[i][j] - meltenergy[i][j];//source - MELT[i][j]*Lf/deltat;
        energybalance();
    }



    /* Check stability of the solution with criterium: D*dt/dx2 < 0.25 */
    stcrit=(conduc[1]/layerrhocp[i][j][1])*deltat/pow(layerthickness[i][j][1],2);
    if ((nsteps == 1) && (i == rowclim) && (j == colclim) && (stcrit >= 0.25) && (inter == 1)) {
        printf("\n Numerical instability in subsurface %f %f %f \n",stcrit,0.25*deltat/stcrit,deltat);
        printf("%f %f %f \n",conduc[1],layerrhocp[i][j][1],layerthickness[i][j][1]);
        /*fprintf(outcontrol,"\n Numerical instability in subsurface %f %f %f \n",stcrit,0.25*deltat/stcrit,deltat);*/
    }
    freearraydouble(layerwatercontold,1,ndepths);
    
    return;
}

/****************************************************************************/
/* FUNCTION  interpolate                                                    */
/*   this function interpolates linearly the climate data for the subtimesteps */
/*   called from main for every subtimestep                                 */
/****************************************************************************/

void interpolate() {
    if (inter == 1) { /*first subtimestep*/
        /* change timestep appropriately*/
        /*variable timestep is maniputated but set to original value before next timestep*/
        timestep = timestep/factinter;
        if (zeit != 24) jd = (int)jd+zeit/24.;
        if (zeit == 24) jd = (int)jd;
        jdold = (int)jd + (zeit-1.)/24.;
        if (nsteps == 1) {
            printf("\n Timestep of %.1f hours divided in %d steps to obtain timestep %.3f (=%.1f min) \n",
                   timesteporig,factinter,timestep,timestep*60.);
            fprintf(outcontrol,"\n Timestep of %f hours divided in %d steps to obtain timestep %f (= %f min) \n\n",
                    timesteporig,factinter,timestep,timestep*60.);
            /*timestep divided in factinter steps for stability reasons*/
        }
    }  /*endif inter*/

    for (i = 1; i < nrows; i++) {
        for (j = 1; j < ncols; j++) {
            if (DIRECT[i][j] >= 0.) {
                if ((nsteps == 1) && (inter == 1))
                    DIRECTold[i][j] = DIRECT[i][j];
                if (inter == 1)
                    graddirect[i][j] = (DIRECT[i][j] - DIRECTold[i][j])/factinter;
                DIRECT[i][j] = DIRECTold[i][j] + inter*graddirect[i][j];
                if (inter == factinter)
                    DIRECTold[i][j] = DIRECT[i][j];
            }
        }
    }

    /* initialise old climate variables */
    if ((nsteps == 1) && (inter == 1)) {
        /*variables always needed*/
        tempold = temp;              /*temperature*/
        humold = hum;                /*humidity*/
        /* wind is kept constant between different measurements*/
        globold = glob;              /*global radiation, incoming shortwave*/
        /* precipitation is divided in equal parts */

        /* variables needed depending on parameterisation */
        refold = ref;                /*reflected shortwave*/
        if(methodlonginstation == 1)
            netold = net;              /*net radiation */
        if(methodlonginstation == 2) /*longwave incoming radiation measurements used*/
            LWinold = LWin;            /*long wave incoming radiation*/
        /*if(methodlonginstation == 3) cloud is kept constant between different measurements*/
    }

    if (inter == 1)   /*first time through loop, determin how much to increment each variable*/
        /*for each subtimestep*/
    {
        gradtemp = (temp - tempold)/factinter;
        gradhum = (hum - humold)/factinter;
        gradglob = (glob-globold)/factinter;
        prec = prec/factinter;
        gradref = (ref-refold)/factinter;
        if(methodlonginstation == 1)
            gradnet = (net-netold)/factinter;
        if(methodlonginstation == 2)
            gradLWin = (LWin-LWinold)/factinter;
    }

    jd = jdold + inter*timestep/24.;
    jd2 = jd;
    temp = tempold + inter*gradtemp;
    hum  = humold + inter*gradhum;
    glob = globold + inter*gradglob;
    ref  = refold + inter*gradref;
    if(methodlonginstation == 1)
        net = netold + inter*gradnet;
    if(methodlonginstation == 2) { /*longwave incoming radiation measurements used*/
        LWin = LWinold + inter*gradLWin;
        LONGIN[rowclim][colclim] = LWin;
    }
    if (wind < 0.1) wind = 0.1;

    if (inter == factinter) {  /*last sub timestep*/
        /*store values before reading new row of data from file*/
        if (zeit == 24)
            jd = (int)jd -1;
        tempold = temp;              /*temperature*/
        humold = hum;                /*humidity*/
        globold = glob;              /*global radiation, incoming shortwave*/
        refold = ref;                /*reflected shortwave*/
        if(methodlonginstation == 1)
            netold = net;              /*net radiation */
        if(methodlonginstation == 2)
            LWinold = LWin;            /*long wave incoming radiation*/
    }
    /*printf("\n TIME %d %f %f %f",nsteps,jd,jd2,jdold,zeit);*/

    return;
}

/****************************************************************************/
/* FUNCTION  subtimestepsummation                                           */
/*   this function summates the mass and energy balance components          */
/*   for the subtime steps in the subsurface model runs                     */
/****************************************************************************/

void subtimestepsummation() {

   if (inter == 1) { /*only first subtime step, initialise to zero*/
       MELTsum[i][j] = 0.;
       ABLAsum[i][j] = 0.;
       SUBLIMATIONsum[i][j] = 0.;
       RUNOFFsum[i][j] = 0.;
//       watercontentsum[i][j] = 0.;
       deltaWATERsum[i][j] = 0.;
       deltaREFREEZEsum[i][j] = 0.;
       sumSNOWprec[i][j] = 0.;
       MBsum[i][j] = 0.;
	   sumRAINprec[i][j] = 0.;
       DIRECTsum[i][j] = 0.;
       if (methodglobal == 2) {
       	  DIRECT2sum[i][j] = 0.;
       	  DIFFUSsum[i][j] = 0.;
       }
       GLOBALsum[i][j] = 0.;
       REFLECTsum[i][j] = 0.;
       LONGINsum[i][j] = 0.;
       LONGOUTsum[i][j] = 0.;
       SENSIBLEsum[i][j] = 0.;
       LATENTsum[i][j] = 0.;
       ICEHEATsum[i][j] = 0.;
       rainenergysum[i][j] = 0.;
	   meltenergysum[i][j] = 0.;
   }

/*Sum over subtime steps*/
   MELTsum[i][j] += MELT[i][j]; /* in mm*/
   ABLAsum[i][j] += ABLA[i][j];
   SUBLIMATIONsum[i][j] += SUBLIMATION[i][j];
   RUNOFFsum[i][j] += RUNOFF[i][j];
//   watercontentsum[i][j] += watercontent[i][j];
   deltaWATERsum[i][j] += deltaWATER[i][j];
   deltaREFREEZEsum[i][j] += deltaREFREEZE[i][j];
   MBsum[i][j] += snowprec-ABLA[i][j]+sumrain;
   sumSNOWprec[i][j] += snowprec;
   sumRAINprec[i][j] += RAIN[i][j];
   DIRECTsum[i][j] += DIRECT[i][j];
   if (methodglobal == 2) {
      DIRECT2sum[i][j] += DIRECT2[i][j];
	  DIFFUSsum[i][j] += DIFFUS[i][j];
   }
   if ((snetfromobsyes == 1) && (calcgridyes == 2)) {
      GLOBALsum[i][j] += glob;
      REFLECTsum[i][j] += ref;
   } else {
      GLOBALsum[i][j] += GLOBAL[i][j];
      REFLECTsum[i][j] += GLOBAL[i][j]*ALBEDO[i][j];
   }
   LONGINsum[i][j] += LONGIN[i][j];
   LONGOUTsum[i][j] += LONGOUT[i][j];
   SENSIBLEsum[i][j] += SENSIBLE[i][j];
   LATENTsum[i][j] += LATENT[i][j];
   ICEHEATsum[i][j] += ICEHEAT[i][j];
   rainenergysum[i][j] += rainenergy[i][j];
   meltenergysum[i][j] += meltenergy[i][j];
   
/*final step put back in original array*/
   if (inter == factinter) {
      MELT[i][j] = MELTsum[i][j];
      ABLA[i][j] = ABLAsum[i][j];
      SUBLIMATION[i][j] = SUBLIMATIONsum[i][j];
      RUNOFF[i][j] = RUNOFFsum[i][j];
//      watercontent[i][j] = watercontentsum[i][j]/factinter;
      deltaWATER[i][j] = deltaWATERsum[i][j];
      deltaREFREEZE[i][j] = deltaREFREEZEsum[i][j];
	  RAIN[i][j] = sumRAINprec[i][j];
      DIRECT[i][j] = DIRECTsum[i][j]/factinter;	/* is ok here, in old DIRECT is stored in DIRECTold, new is read at start next timestep*/
      if (methodglobal == 2) {
         DIRECT2[i][j] = DIRECT2sum[i][j]/factinter;
     	 DIFFUS[i][j] = DIFFUSsum[i][j]/factinter;
   	  }
	  if ((snetfromobsyes == 1) && (calcgridyes == 2)) {
   		 glob = GLOBALsum[i][j]/factinter;	/* is ok here, in old glob is stored in globold, new is read at start next timestep*/
   		 ref = REFLECTsum[i][j]/factinter;	/* is ok here, in old ref is stored in refold, new is read at start next timestep*/
//		 ALBEDO[i][j] = ref / glob ; 
   		 SWBAL[i][j] = glob - ref ;  
   	  } else {
   		 GLOBAL[i][j] = GLOBALsum[i][j]/factinter;
   		 /* Carleen fix to run model distributed (comment and change by F. Covi - 05/2020) */
   		 /* ALBEDO[i][j] = REFLECTsum[i][j]/GLOBALsum[i][j]; */
   		 if (GLOBALsum[i][j] > 0.) { ALBEDO[i][j] = REFLECTsum[i][j]/GLOBALsum[i][j]; }
   		 SWBAL[i][j] = GLOBAL[i][j]*(1-ALBEDO[i][j]);
   	  }
   	  LONGIN[i][j] = LONGINsum[i][j]/factinter;
   	  LONGOUT[i][j] = LONGOUTsum[i][j]/factinter;
   	  SENSIBLE[i][j] = SENSIBLEsum[i][j]/factinter;
   	  LATENT[i][j] = LATENTsum[i][j]/factinter;
   	  ICEHEAT[i][j] = ICEHEATsum[i][j]/factinter;
   	  rainenergy[i][j] = rainenergysum[i][j]/factinter;
	  meltenergy[i][j] = meltenergysum[i][j]/factinter;
   	  NETRAD[i][j] = SWBAL[i][j] + LONGIN[i][j] - LONGOUT[i][j];
   	  ENBAL[i][j] = NETRAD[i][j] + SENSIBLE[i][j] + LATENT[i][j] + rainenergy[i][j] - ICEHEAT[i][j];

//   tmp = RAIN[i][j] + MELT[i][j] - RUNOFF[i][j] - deltaREFREEZE[i][j];
//   tmp = RAIN[i][j] + MELT[i][j] - ABLA[i][j] - deltaREFREEZE[i][j] + SUBLIMATION[i][j];
//   if ( deltaWATER[i][j]-tmp > 0.) printf("\n %f %f %f %f ",jd2,deltaWATER[i][j],tmp,deltaWATER[i][j]-tmp);

   }

   return;
}

/****************************************************************************/
/* FUNCTION  outputsubsurf                                                  */
/*   this function prints out subsurf conditions for given location         */
/*   for each time step one row for each layer                              */
/*   called from main once before time step loop and in time loop for each grid cell */
/****************************************************************************/

void outputsubsurf() {
    int kk=1;
    int k;
    //int ki,kj; /* L.G.: ki and kj are unused, can we delete? */

    jd2=jd;
    if ((zeit == 24) && (inter == factinter))
        jd2 = (int)jd + 1;

    for (kk=1; kk <= outgridnumber; kk++) { /*for each point to be written to file*/
        if (nsteps == 0) {
            i = stnrow[kk];     /*row of grid point, read from input.dat*/
            j = stncol[kk];

            fprintf(outsubsurf[kk]," Site i=%d j=%d height=%.2f \n",i,j,griddgm[i][j]);
            fprintf(outsubsurf[kk]," year time nr depth(m) thickness(m) temp(C) dens(g/kg) mass(kg/m2) "); 
            fprintf(outsubsurf[kk]," water(kg/m2) deltarewater(kg/m2/dt) refreeze(kg/m2) deltarefreeze(kg/m2/dt) dsnow(msnow) dsnow(mmwe) id irrwc \n");

            fprintf(outcontrol,"Writing subsurface profiles of location i %d j %d \n",i,j);
        }  /*endif*/

        if ((i == stnrow[kk]) && (j == stncol[kk])) {  /*for grid cell to be outputted, read from input.dat*/
            for (k = 1; k <= (int)layeramount[i][j]; k++) {   /*for each layer*/
                if (irrwatercontyes == 1) irreducible_schneider(i, j, k);
                if (irrwatercontyes == 2) irreducible_coleou(i, j, k);

/* Plot the initial state*/
                if ((k == 1) && (nsteps == 0) && (subsurf_print_surf == 1)) {
                    surftempfrommodel();
                    fprintf(outsubsurf[kk]," %.0f %d.000000 %2d 0.000000 0.000000 %.4f %.4f %.4f %.4f 0.000000 %.4f 0.000000 %.4f %.4f %.0f %.3f \n",
                            year,(int)jd2,k,surftemp[i][j],layerdensity[i][j][1],layermass[i][j][1],
                            layerwatercont[i][j][1],layerrefreeze[i][j][1],snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][1],irrwatercont);
                }

                if (nsteps == 0) {
                    fprintf(outsubsurf[kk]," %.0f %d.000000 %2d %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f %.3f \n",
                            year,(int)jd2,k,layerdepth[i][j][k],layerthickness[i][j][k],layertemperature[i][j][k],
                            layerdensity[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],layerdeltawatercont[i][j][k],
                            layerrefreeze[i][j][k],layerdeltarefreeze[i][j][k],
                            snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][k],irrwatercont);
                }
/* Plot the modeled profiles other time steps*/
                if ((k == 1) && (nsteps > 0)  && (subsurf_print_surf == 1)) {
                    surftempfrommodel();
                    fprintf(outsubsurf[kk]," %.0f %f %2d 0.000000 0.000000 %.4f %.4f %.4f %.4f 0.000000 %.4f 0.000000 %.4f %.4f %.0f %.3f \n",
                            year,jd2,k,surftemp[i][j],layerdensity[i][j][1],layermass[i][j][1],
                            layerwatercont[i][j][1],layerrefreeze[i][j][1],snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][1],irrwatercont);
                }

                if (nsteps > 0) {
                    fprintf(outsubsurf[kk]," %.0f %f %2d %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.0f %.3f \n",
                            year,jd2,k,layerdepth[i][j][k],layerthickness[i][j][k],layertemperature[i][j][k],
                            layerdensity[i][j][k],layermass[i][j][k],layerwatercont[i][j][k],layerdeltawatercont[i][j][k],
                            layerrefreeze[i][j][k],layerdeltarefreeze[i][j][k],
                            snowlayer[i][j],SNOW[i][j]*10.,layerid[i][j][k],irrwatercont);
                }
            }   /*endfor  each layer*/
        }  /*endif right grid cell*/
    }  /*endfor*/

    return;
}

