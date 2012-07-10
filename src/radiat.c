/****************************************************************************/
/*  FILE  :  radiat.c                                                       */
/*  CALCULATION OF GLOBAL RADIATION and                                     */
/*                  SHORT-WAVE RADIATION BALANCE AND NET RADIATION          */
/*    Mar  1998,  last update: 5 Jan 2011                                   */
/****************************************************************************/

#include "radiat.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "globcor.h"

#include "variabex.h"      /* all global VARIABLES */

/*================ GLOBAL RADIATION ======================================*/
/*********************************************************************/
/*  FUNCTION : ratioglobal                                           */
/*             for each time step, but constant for all grids        */
/*    needed if global radiation not split into direct and diffus    */
/*    (meltmod.c) and if temp index method 3 used (degree.c,ddfopt.c)*/
/*  updated 1 Jun 2006*/
/*********************************************************************/

void ratioglobal()

{
    /* DISTRIBUTION OF RADIATION TO EACH GRID USING THE RATIO :
      MEASURED GLOBAL RADIATION DEVIDED BY CALCULATED DIRECT RADIATION
      CORRECTED DUE TO EFFECTS OF SLOPE, ASPECT AND SHADING
      at climate station (=grid rowclim colclim)
      directstationhoriz = direct radiation at climate station if the grid was horizontal
      DIRECT = calculated solar radiation for sloped surface (see globcor)
               strlsumme/timestep * exkorstationhoriz             */

    float rationeu;

    if(timestep < 24)   /*not for daily timestep because this would only give midnight value*/
        getzenith();   /*GET THE ZENITH ANGLE FOR THE MIDDLE OF THE TIME INTERVAL*/
    /*ALSO NEEDED IN globradratio*/

    if (DIRECT[rowclim][colclim] != 0.) {        /*station in sun*/
        if ( ((sunzenithangle*180/pi <= 70.0) && (timestep < 24)) || ((timestep == 24) && (glob > 10)) )  /*if higher, ratio becomes inaccurate*/
            /*if daily timestep compute in any case since sunangle not relevant*/
        {
            rationeu = glob/directstationhoriz;
            if (rationeu <= 1.3)
                /*new variable essential, because old ratio must be safed*/
                ratio = rationeu;
        }
    }
    /*if station shaded: Shade=0 -> Exkorr=0 -> DIRECT=0
      after station is shaded the last ratio (station at least partially in sun)
      is used for all following time steps until the station is in the sun again
      and a new ratio can be calculated
      a new ratio is only calculated if station in sun and if the sun is not too
      low (zenith 70), otherwise the value in ratio is the last one calculated */
    return;
}



/******************************************************************/
/*  FUNCTION : globradratio
       CALCULATION OF SHORTWAVE RADIATION                         */
/******************************************************************/

void globradratio()

{
    float  dirradhor=0.0;

    /* DISTRIBUTION OF RADIATION TO EACH GRID USING THE RATIO OF MEASURED
       GLOBAL RADIATION AND CALCULATED CLEAR-SKY DIRECT RADIATION
       ratio calculated in function ratioglobal, is constant over area
       ASSUMPTION : UNIFORM CLOUD DISTRIBUTION OVER GRID    */

    if (DIRECT[i][j] > 0.0)    /*GRID IN SUN UNDER CLEAR SKY CONDITIONS*/
        /*IF THE STATION IS SHADED THE LAST RATIO UNDER SUNNY CONDITIONS
          REMAINS IN RATIO (this case is taken care of in ratioglobal) */
    {
        /*--------------------------------------------------------------*/
        GLOBAL[i][j]  = DIRECT[i][j]*ratio;
        /*--------------------------------------------------------------*/

        if ((i==rowclim) && (j==colclim))
            GLOBAL[i][j] = directstationhoriz*ratio;

        /*if direct radiation is calculated, i.e. not read from file, DIRECT
          at the climate station refers to the actual slope; directstationhoriz
          contains direct rad at climate station, if this grid cell was horizontal,
          which is needed here. In the case direct radiation is read from files
          it would not be necessary, because in that case direct refers to
          a horizontal slope */

    } else       /*GRID IN SHADE UNDER CLEAR SKY CONDITIONS*/

    {
        if ((DIRECT[rowclim][colclim]) <= 0.0)     /*STATION GRID SHADED */

            /*MEASURED GLOBAL RADIATION IS DIFFUSE BECAUSE THE STATION IS
              IN SHADE -> SAME DIFFUSE RADIATION AT SHADED GRID */
            /*------------------------*/
            GLOBAL[i][j] = glob;
        /*------------------------*/

        else {        /* GRID IN SHADE AND STATION GRID IN SUN*/
            /*THERE IS ONLY DIFFUSE RADIATION. IT IS ASSUMED TO BE A FIXED
             PERCENTAGE OF CALCULATED CLEAR-SKY DIRECT RADIATION (horizontal surface)
             THE PERCENTAGE (prozdiff) IS READ FROM INPUT.DAT,
             dirradhor not DIRECT because DIRECT is zero if grid is shaded
             dirradhor is the same as Isenk in globcor: clear-sky direct radiation
             if the grid was unobstructed and horizontal, here is it not a weighted
             mean over a number of subinterval, but only calculated for the middle
             of the subinterval */

            /*------------------------------------------------*/
            p = exp(-0.0001184*griddgm[i][j]) * P0;           /* air pressure */

            /*NEU 3/2000: IF NOT DONE --> INFINITE NEGATIVE VALUES CAN OCCURR*/
            if (sunaltitudeangle>0.017) { /* 0.017=1 Grad */
                dirradhor = 1368.0 * pow(trans,(p/P0)/cos(sunzenithangle));
                dirradhor *= cos(sunzenithangle);
            } else
                dirradhor = 0;

            /*--------------------------------------------------------------*/
            GLOBAL[i][j]  = dirradhor*(prozdiffuse/100);
            /*--------------------------------------------------------------*/

        }   /*endelse*/

    }  /*endelse*/

    return;
}



/*========================================================================*/
/*  EXTRAPOLATE DIRECT AND DIFFUSE RADIATION SEPARATELY                   */
/******************************************************************/
/*  FUNCTION : splitdiffusedirectcol                              */
/*             CALCULATION OF DIFFUSE RADIATION AT STATION        */
/*             empirical function according to Collares and Rabl  */
/*    called once every time step                                 */
/******************************************************************/

void splitdiffusedirectcol()

{
    float kt;   /*ratio of global radiation to top of atmosphere rad*/
    float ratiodifglob;    /*ratio of diffuse to global radiation*/

    if(topofatm > 0.0) {
        kt=glob/topofatm;

        /*PARAMETERIZATION BY COLLARES-PEREIRA AND RABL (1979)
          SEE PLUESS, 1997, ZGS, p.39*/
        if(kt <=0.17)
            ratiodifglob = 0.99;
        else
            ratiodifglob = 1.188-2.272*kt+9.473*pow(kt,2)-21.856*pow(kt,3)+14.648*pow(kt,4);

        DIFFUS[rowclim][colclim] = glob*ratiodifglob;
        DIRECT2[rowclim][colclim] = glob - DIFFUS[rowclim][colclim];

        if(DIRECT2[rowclim][colclim] < 0.)
            DIRECT2[rowclim][colclim] = 0.0;
    } /*endif topofatm*/

    return;
}


/******************************************************************/
/*  FUNCTION : splitdiffusedirectkiruna                           */
/*      CALCULATION OF DIFFUSE AND DIRECT RADIATION AT STATION    */
/*             empirical function based on Kiruna data            */
/*    called once every time step                                 */
/******************************************************************/

void splitdiffusedirectkiruna()

{
    float  ratiodifglob;   /*ratio diffuse to global radiation*/
    float  a0,a1,a2,a3;    /*coefficients of fitted polynom*/
    float  x;

    a0=0.929;
    a1=1.13445;
    a2=-5.11134;
    a3=3.10614;    /*based on all daily data*/
    /*     a0=0.9043;  a1=1.1119;  a2=-4.0421;   a3=1.9025;   only daily data of May*/

    if(topofatm > 0.0) {    /*after sunrise, before sunset*/
        x=glob/topofatm;

        if(x < 0.15)            /*only diffuse radiation*/
            ratiodifglob = 1;
        else {
            if(x < 0.8)
                ratiodifglob = a0 + a1*x + a2*x*x + a3*pow(x,3);   /*polynom*/
            else
                ratiodifglob = 0.15;  /*high ratio -> clear-sky --> 15% diffuse*/
        }

        DIFFUS[rowclim][colclim] = glob*ratiodifglob;
        DIRECT2[rowclim][colclim] = glob - DIFFUS[rowclim][colclim];

        /*if the station is shaded, glob rad is only diffuse, but this case doesn't
          have to be considered explicitely, because it is automatically
          captured in the method; in this case the ratio will be very small
          and thus the percentage diffuse radiation close to 1 */

        if(DIRECT2[rowclim][colclim] < 0.)
            DIRECT2[rowclim][colclim] = 0.0;
    } /*endif topofatm*/
    else {
        DIFFUS[rowclim][colclim] = 0;      /*during night*/
        DIRECT2[rowclim][colclim] = 0;
    }

    return;
}


/******************************************************************/
/* FUNCTION : ratiodirectdirect                                   */
/*   CALCULATION OF RATIO OF DIRECT AND CLEAR-SKY DIRECT RADIATION*/
/*   AT THE CLIMATE STATION                                       */
/*      this ratio is used to interpolate direct radiation        */
/*      called once every time step from main                     */
/******************************************************************/

void ratiodirectdirect()

{
    if(timestep < 24)   /*not for daily timestep because this would only give midnight value*/
        getzenith();   /*GET THE ZENITH ANGLE FOR THE MIDDLE OF THE TIME INTERVAL*/
    /*ALSO NEEDED IN interpoldirect*/

    if (DIRECT[rowclim][colclim] != 0.) {  /*station in sun under clear conditions*/
        if ( ((sunzenithangle*180/pi <= 70.0) && (timestep < 24)) || ((timestep == 24) && (glob > 10)) )  /*if higher, ratio becomes inaccurate*/
            /*if daily timestep compute in any case since sunangle not relevant*/
            ratiodir2dir = DIRECT2[rowclim][colclim]/DIRECT[rowclim][colclim];
    }
    /*if climate station is shaded, the old ratio will be used*/

    return;
}


/******************************************************************/
/*  FUNCTION : interpoldirect                                     */
/*             INTPERPOLATE DIRECT RADIATION AS SEPARATED FROM    */
/*             GLOBAL RADIATION AT THE CLIMATE STATION            */
/*             interpolation according to Pluess (1997)           */
/*     called for each grid cell and every time step              */
/******************************************************************/

void interpoldirect()

{
    /*DIRECT is clear-sky direct radiation (no clouds !)
      DIRECT2 is direct radiation (including the effects of clouds)
      derived from empirical function, ratiodir2dir is at climate station */

    if(topofatm > 0.0) {    /*during day*/
        if(DIRECT[i][j] == 0)    /*grid cell is shaded*/
            DIRECT2[i][j] = 0;
        else
            DIRECT2[i][j] = DIRECT[i][j]*ratiodir2dir;
    } else
        DIRECT2[i][j] = 0;


    return;
}

/******************************************************************/
/*  FUNCTION : diffusunobstructed                                 */
/*             INTPERPOLATE DIFFUSE RADIATION AS SEPARATED FROM   */
/*             GLOBAL RADIATION AT THE CLIMATE STATION            */
/******************************************************************/

void diffusunobstructed()

{
    /*SKYVIEW is 1 for unobstructed sky, mean albedo is of preceding
      time step, because it is not yet calculated */

    if(SKYVIEW[i][j] != 0)
        diffus0 = (DIFFUS[rowclim][colclim] - meanalb*glob*
                   (1-SKYVIEW[rowclim][colclim])) / SKYVIEW[rowclim][colclim];
    else {
        printf("\n\n ERROR function diffusunobstructed\n");
        printf(" row  %d  col  %d  \n\n",i,j);
        exit(12);
    }

    if(diffus0 < 0)
        diffus0 = 0;

    return;
}


/******************************************************************/
/*  FUNCTION : interpoldiffus
               INTPERPOLATE DIFFUSE RADIATION AS SEPARATED FROM
               GLOBAL RADIATION AT THE CLIMATE STATION            */
/******************************************************************/

void interpoldiffus()

{
    if(topofatm > 0)
        DIFFUS[i][j] = SKYVIEW[i][j]*diffus0 + meanalb*glob*(1-SKYVIEW[i][j]);
    else
        DIFFUS[i][j] = 0;

    return;
}


/******************************************************************/
/*  FUNCTION : adddirectdiffus                                    */
/*             ADD DIRECT AND DIFFUSE RADIATION                   */
/*    called from main every time step and each grid cell         */
/******************************************************************/

void adddirectdiffus()

{
    GLOBAL[i][j] = DIRECT2[i][j] + DIFFUS[i][j];

    return;
}



/*================== ALBEDO ============================================================*/
/***********************************************************************************/
/*  FUNCTION  albedoread                                                           */
/*      DETERMINATION OF ALBEDO: ALBEDO IS READ FROM FILES VALID FOR FIXED PERIODS */
/*      ALLOCATION OF ALBEDOFILE, CHOOSE THE RIGHT ONE OPEN AND READ FILE          */
/***********************************************************************************/

void albedoread()

{
    if (jd >= jdstartalb[ialb]) {  /*FILES READ ONLY ONCE*/
        /* Ueberlesen von Files, die noch vor dem ersten zu verwendetem File im Inputfile stehen
           d.h. finden des ersten richtigen Files beim ersten Durchgang */
        if (ialb == 1)
            while (jd >= jdstartalb[ialb+1])
                ialb += 1;

        strcpy(dummy,inpath);
        strcpy(namealbedo,namealb[ialb]);                   /*ALLOCATE NEW FILE-NAME*/
        strcat(dummy,namealbedo);

        if ((inalbedo = fopen(dummy,"rb")) == NULL) {    /*OPEN NEW ALBEDO-FILE*/
            printf("\n\n Error: methodinisnow == %d, ",methodinisnow);
            printf("but Albedo-File  %s  not found. \n\n",namealb[ialb]);
            exit(20);
        }

        if (inalbedo) {
            if ((fread(&(x[1]),sizeof(float),12,inalbedo)) !=12) {  /*READ FIRST 12 ROWS*/
                printf("\n\n ERROR in file  %s  \n  file radiat.c\n\n",namealb[ialb]);
                exit(30);
            }
            /*READ WHOLE FILE*/
            if ((fread(&(surface[1][1]),sizeof(float),ncols*nrows,inalbedo)) != ncols*nrows) {
                printf("\n\n ERROR in reading grid data  %s \n  file radiat.c\n\n",
                       namealb[ialb]);
                exit(40);
            }

            if ((fclose(inalbedo)) == EOF) {
                printf("\n\n Surface-File (Albedo) %s  not closed  \n\n",namealb[ialb]);
                exit(20);
            }

            inalbedo = NULL;
        } /*endif inalbedo*/
        else {
            printf("\n\n ERROR in file radiat.c\n\n");
            exit(20);
        }

        fprintf(outcontrol,"\n FUNCTION albedoread  in radiat.c :\n");
        fprintf(outcontrol,"albedo-file read: %s   jd=%4.1f  time=%4.2f\n",
                namealb[ialb],jd,zeit);

        ialb++;      /*NEXT ALBEDONAME FROM input.dat*/

    } /*endif jdstartalb*/

    return;
}



/****************************************************************************/
/*  FUNCTION : albedocalcconst                                              */
/*      TEMPORALLY CONSTANT VALUES ARE ALLOCATED TO DIFFERENT SURFACE TYPES */
/****************************************************************************/

void albedocalcconst()

{
    if ((surface[i][j]) == 1.)        /*GRID CELL IS SNOW-COVERED*/
        (ALBEDO[i][j]) = albsnow;

    /*NOTE: VALUE 2 FOR SURFACE HAS DIFFERENT MEANINGS DEPENDING ON methodinisnow*/
    if ((surface[i][j]) == 2.) {
        if(methodinisnow == 1)        /*surface maps with option slush*/
            (ALBEDO[i][j]) = albslush;
        else     /*computation with initial snow cover*/
            (ALBEDO[i][j]) = albfirn;   /*2=firn without snow*/
    }
    if ((surface[i][j]) == 3.)
        (ALBEDO[i][j]) = albice;      /*GRID CELL IS ICE-EXPOSED*/
    if ((surface[i][j] == 4.) || (surface[i][j] == 5.))
        (ALBEDO[i][j]) = albrock;

    if ( ((ALBEDO[i][j]) < 0) || ((ALBEDO[i][j]) >1) ) {
        printf("\n ERROR in Albedo-array (fct. albedocalcconst) %s : \n Albedo=%5.1f\n",namealbedo,ALBEDO[i][j]);
        exit(10);
    }

    return;
}


/*******************************************************************/
/*  FUNCTION : albedosnowmeas                                      */
/*      TEMPORALLY CONSTANT VALUES ARE ALLOCATED TO SLUSH AND ICE  */
/*      BUT VARYING VALUES FOR THE SNOW SURFACE (daily mean values */
/*      read from file)                                            */
/*******************************************************************/

void albedosnowmeas()

{
    int jdint;

    /*ZUORDNEN ALBEDO-WERTE FUER SURFACE CONDITION WERTE*/

    jdint = (int)jd;

    if ((surface[i][j]) == 1.)
        (ALBEDO[i][j]) = albedosnow[jdint];

    if ((surface[i][j]) == 2.) {  /*array has different meanings for inisnow options*/
        if(methodinisnow == 1)         /*surface maps with option slush*/
            (ALBEDO[i][j]) = albslush;
        else     /*computation with initial snow cover, 2=firn area without snow*/
            (ALBEDO[i][j]) = albedosnow[jdint]; /*2=firn without snow, treated as snow*/
    }

    if ((surface[i][j]) == 3.)
        (ALBEDO[i][j]) = albice;
    if ((surface[i][j]) == 4.)
        (ALBEDO[i][j]) = albrock;

    if ( ((ALBEDO[i][j]) < 0) || ((ALBEDO[i][j]) >1) ) {
        printf("\n ERROR in Albedo-file %s : \n %5.1f\n",namealbedo,ALBEDO[i][j]);
        exit(10);
    }

    return;
}


/******************************************************************/
/*  FUNCTION  inialbedoalt                                        */
/*     initialize ALBALT (albedo array of previous time step)     */
/*     if albedo generated by model, used for snow albedo         */
/******************************************************************/

void inialbedoalt()

{
    int   inti;
    float snowalbincreaseabs;   /*albedo=albedo AWS + this amount*/
    float maxsnowincrease = 0.07;    /*maximum absolute albedo increase*/
    /*initial snow albedo can be increased with increasing elevation*/
    /*but the absolute increase is limited to this value*/
    /*as well as the absolute decrease for cells lower than the AWS*/

    if(methodinisnow == 2)
        whichsurface();    /*for each surface (snow,firn,ice,rock) integer allocated*/

    for (i=firstrow; i<=lastrow; i++)
        for (j=firstcol[i]; j<=lastcol[i]; j++) {
            if(griddgmdrain[i][j] != nodata) {  /*WITHIN AREA TO BE CALCULATED*/
                inti=(int)surface[i][j];
                switch(inti) {
                case 1:      /*snow surface,  snowalbincrease is in percent*/
                    snowalbincreaseabs =
                        (griddgm[i][j]-griddgm[rowclim][colclim])/100 * snowalbincrease/100*albsnow;

                    if(snowalbincreaseabs > maxsnowincrease)
                        snowalbincreaseabs = maxsnowincrease;
                    /*for cells lower than the AWS*/
                    if((snowalbincreaseabs < 0) && (snowalbincreaseabs < maxsnowincrease*(-1)))
                        snowalbincreaseabs = maxsnowincrease*(-1);
                    ALBALT[i][j] = albsnow + snowalbincreaseabs;
                    break;
                case 2:
                    ALBALT[i][j] = albfirn;   /*firn surface, no snow on top*/
                    break;
                case 3:
                    ALBALT[i][j] = albice;    /*ice surface*/
                    break;
                case 4:
                    ALBALT[i][j] = albrock;   /*rock surface*/
                    break;
                case 5:
                    ALBALT[i][j] = albrock;   /*debris surface*/
                    break;
                default:
                    printf("\nwrong type of surface\n function inialbedoalt\n");

                }  /*end case*/
            } /*endif*/

        }  /*end for*/

    return;
}



/******************************************************************/
/* FUNCTION  albedosnownew                                        */
/*           new function using days since snow fall and air temp */
/*    called from function albedocalc for each grid cell          */
/*  modification snow depth dependency if depthyes = 1            */
/*  by Carleen Tijm-Reijmer, 2/2005                               */
/******************************************************************/

void albedosnownew() {
    /*    float  albk1=0.005, albk2=-1.1, albk3=0.02;   */

    float  albk1=0.005, albk2=-1.05, albk3=0.02;
    float  albmax=0.9;      /*maximum snow albedo value*/
    float  tempfactor,daydecay;

    float  snowstar=1.1;    /*chr characteristic snowdepth in cm*/
    float  snowalb,minalb;  /*chr*/
    float  depthyes;        /*chr*/

    depthyes=1;     /*chr 1=yes, 0=no depth dependency */

    if(snowprec == 0) {        /*no snow fall, decrease in albedo*/
        numbdays[i][j] += timestep/24.0;    /*number of days in days*/

        if(tempint[i][j] > 0)
            tempfactor = log(tempint[i][j]+1);   /*faster decay with higher temp*/
        else
            tempfactor = 0.1;    /*no temp induced decay, only number of days*/

        daydecay = albk1*tempfactor*exp(albk2*sqrt(numbdays[i][j]))/factinter; /*CHR factinter added as quick fix*/
        if(daydecay > albk1)     /*to avoid values > 1, if exponent approaches 0*/
            daydecay = albk1;

        if (depthyes == 0) {
            ALBEDO[i][j] = ALBALT[i][j] - daydecay;    /*albedo decay*/
        } else { /*chr*/
            minalb=albice;
            if(FIRN[i][j] > 0)   minalb=albmin;
            snowalb = ALBALT[i][j] - daydecay;    /*albedo decay*/
            ALBEDO[i][j] = snowalb + (minalb - snowalb) * exp(-SNOW[i][j]/snowstar);
        }

        /* albedo drop by 0.05 if snow has melted and firn below */
        /***** NOTE: re-programed 10/2000, as SNOW array does not exist if photos used ***/
        /*    if((snowtofirn[i][j] == 0) && (SNOW[i][j] == 0))  */
        /***** NOTE: re-programed 10/2000, as SNOW array does not exist if photos used ***/


        if((snowtofirn[i][j] == 0) && (surface[i][j] == 2)) {
            ALBEDO[i][j] = ALBALT[i][j] - 0.05;
            snowtofirn[i][j] = 1;
            /*array controls that this albedo drop only occurs once, boolean meaning*/
        }

        if(ALBEDO[i][j] < albmin)          /*albedo cannot be lower than albmin*/
            ALBEDO[i][j] = albmin;


        /*to avoid that albedo drops to fast, if hours of snow fall and no snow alternate;
          albedo is allowed only to drop to the value of ALBEDO BEFORE SNOW fall; after
          that the decay continues as if there were no snow fall, ALBBEFSNOW initialized to 2*/
        if((ALBEDO[i][j] < ALBBEFSNOW[i][j]) && (ALBBEFSNOW[i][j] != 2)) {
            ALBEDO[i][j]   = ALBBEFSNOW[i][j];
            numbdays[i][j] = ndbefsnow[i][j];
            ALBBEFSNOW[i][j] = 2;
            /*set to 0 to make sure, that this loop is done only if there was snow fall
              during the previous time step. If it was not to 0, albedo would not at all be
              allowed to fall below ALBBEFSNOW which would be wrong*/
        }

    } else { /*SNOW FALL, ALBEDO INCREASES */
        /*first time step with snowfall, albedo decay has occurred before*/
        /*numbdays should only increase, not decrease, if 2. condition was not employed
          ndbefsnow could be very small (after 1 hour of snow fall) and subsequent decay
          would be very fast and thus too rapid*/
        if((numbdays[i][j] > 0) && (ALBALT[i][j] < ALBBEFSNOW[i][j])) {
            ALBBEFSNOW[i][j] = ALBALT[i][j];    /*store albedo of previous time step*/
            ndbefsnow[i][j]  = numbdays[i][j];
        }

        ALBEDO[i][j] = ALBALT[i][j] + albk3 * snowprec;   /*albedo increased*/
        if(ALBEDO[i][j] > albmax)        /*upper limit of albedo*/
            ALBEDO[i][j] = albmax;
        numbdays[i][j] = 0;
    }

    if ((methodsnowalbedo == 3) && (topofatm > 20) && (inter == factinter))  /*modify albedo according to cloudiness*/
        /*only during day, thus minimum radiation*/
        albedosnowclouds();

    return;
}


/******************************************************************/
/* FUNCTION  albedosnowclouds                                     */
/*     adjust albedo just computed with new albedo function  (albedosnownew) */
/*     including the effects of clouds according to Jonsell et al., 2003, J.Glaciol. */
/*     should only be used for hourly or similar time steps       */
/*    called from function albedosnownew for each grid cell       */
/******************************************************************/

void albedosnowclouds() {
    float  albmax=0.9;      /*maximum snow albedo value*/
    int    newtimestepyes = 0;   /*1=new time step*/

    /*to decide if function is called first time for the time step
      needed so that ratioglobToA-ratioglobToAold is only computed once per timestep
      jdlasttimestep is set to -1 for first time step in variab.h*/
    if(jd == jdlasttimestep)  /*first pixel to be calculated for each time step*/
        newtimestepyes = 0;  /*same time step as previous one, only new grid cell*/
    else
        newtimestepyes = 1;  /*next time step, compute new ratioglobToA*/

    if(newtimestepyes == 1)   /*ratioglobToA assumed constant for each grid cell*/
        ratioglobToA = glob/topofatm;    /*function only called if topofatm > 20*/

    /*to produce a 0 percent change for the very first time step*/
    if(jdlasttimestep == -1)    /*is the case only for the very first time step*/
        ratioglobToAold = ratioglobToA;

    /*** according to Jonsell et al., 2003, J.Glaciol. p. 66) */
    /*   offset 0.2 from equation in paper deliberately neglected */
    if(newtimestepyes == 1) {  /*only needed for each time step; constant for each grid cell*/
        albpercentagechange = (-20.7*(ratioglobToA-ratioglobToAold));

        if(albpercentagechange > 20)    /*set max % change to 20%*/
            albpercentagechange = 20;
        if(albpercentagechange < -20)    /*set min % change to -20%*/
            albpercentagechange = -20;

        /*printf(" jd=%.2f ratioglobToA=%.4f ratioglobToAold=%.4f alb>%f\n",jd,ratioglobToA,ratioglobToAold,albpercentagechange);*/
    }

    /*increase the albedopercentagechange in case the albedo needs to be increased (clouds)*/
    /*this an attempt to avoid the continuous drop in albedo due to clouds*/
    /*but this lead to way to high albedo, does not work*/
    /*absolute albedo increase = ALBEDO[i][j]*albpercentagechange/100;  */
    /* if(albpercentagechange > 0)
       ALBEDO[i][j]=ALBEDO[i][j]+(ALBEDO[i][j]+ALBEDO[i][j]*albpercentagechange/100)*albpercentagechange/100;
     else  */

    ALBEDO[i][j]=ALBEDO[i][j]+ALBEDO[i][j]*albpercentagechange/100;


    if(ALBEDO[i][j] < albmin)        /*albedo cannot be lower than albmin*/
        ALBEDO[i][j] = albmin;

    if(ALBEDO[i][j] > albmax)        /*upper limit of albedo*/
        ALBEDO[i][j] = albmax;

    /*must only be done once per time step, otherwise the albpercentagechange is always 0*/
    if(newtimestepyes == 1)
        ratioglobToAold = ratioglobToA;  /*store value for next hour*/
    /*ratioglobToAold is initialized to 0.5 in variab.h for first time step but this
      is not taken anyway because for the first time step the program equals them, see above*/

    jdlasttimestep = jd;   /*to identify next time step*/


    return;
}


/******************************************************************/
/* FUNCTION  albedocalc                                           */
/*    for method 2: internal generation of albedo                 */
/*    called from main every time step and for each grid cell     */
/******************************************************************/

void albedocalc() {
    int inti;

    inti=(int)surface[i][j];
    switch(inti)   /*** WRITE ENERGY BALANCE GRID-OUTPUT-FILES ***/

    {
    case 1:
        albedosnownew();       /*snow surface*/
        break;
    case 2:
        albedosnownew();       /*firn, same procedure as for snow*/
        break;
    case 3:   /*ice linear decrease with elevation*/
        ALBEDO[i][j] = albice-albice*
                       ((griddgm[rowclim][colclim]-griddgm[i][j])*albiceproz/100/100);
        break;
    case 4:
        ALBEDO[i][j] = albrock;
        break;
    case 5:
        ALBEDO[i][j] = albrock;
        break;

    }

    ALBALT[i][j] = ALBEDO[i][j];    /*store albedo for next time step*/
    /*must be a different array, because, ALBEDO-array may be overwritten,
      if means are written to output, see writeout.c*/

    /*stake 29 Storglaciaren*/
    /* if((i==50)&&(j==58))
     {  printf(" %2.0f %6.3f A=%6.3f ABS=%6.3f nd=%4.2f %4.2f rain=%4.2f sno=%3.2f",
         surface[i][j],ALBALT[i][j],ALBEDO[i][j],ALBBEFSNOW[i][j],numbdays[i][j],
         ndbefsnow[i][j],rainprec,snowprec);
     } */

    return;
}


/******************************************************************/
/* FUNCTION : whichsurface                                        */
/*    allocate an integer to surface (of drainage DEM) depending  */
/*    on whether or not it is still snow-covered, firn, ice or rock*/
/*    purpose : to write to output for validation of snow line    */
/*              and used in albedocalc, discharge                 */
/*    called from main for every time step before grid cell loop  */
/*           and once from inialbedoalt                           */
/******************************************************************/

/*for albedo calc: integers refer to actual surface type, if snow cover in firn
  area --> it is classified as snow. OBS: in discharge routing the firn area
  belongs to firn reservoir no matter if it is snow-covered or not*/

void whichsurface()

{
    /*only for area calculated, rest remains missing value*/

    for (i=firstrow; i<=lastrow; i++)
        for (j=firstcol[i]; j<=lastcol[i]; j++) {
            if(griddgmdrain[i][j] != nodata) {  /*WITHIN AREA TO BE CALCULATED*/
                if(SNOW[i][j] > 0)
                    surface[i][j] = 1;        /*SNOW SURFACE ALSO IN FIRN AREA AND OUTSIDE GLACIER*/
                else {    /*NO SNOW COVER LEFT*/
                    if(griddgmglac[i][j] == nodata)   /*WITHIN AREA TO BE CALCULATED BUT OUTSIDE GLACER*/
                        surface[i][j] = 4;     /*ROCK, OUTSIDE GLACIER*/
                    else {  /* GLACIER AREA */
                        if(FIRN[i][j] == 0)
                            surface[i][j] = 3;     /*ICE AREA, NO DEBRIS --> ICE BELOW SNOW*/
                        else {
                            if(FIRN [i][j] == -1)   /*ICE AREA DEBRIS COVER*/
                                surface[i][j] = 5;
                            else    /*any other value than 0 or 5*/
                                surface[i][j] = 2;   /*FIRN SURFACE: FIRN AREA NO WINTER/NEW SNOW*/
                        }
                    }

                }  /*endelse snow cover*/
            }  /*endif dgmdrain*/
        }  /*endfor*/

    return;
}


/******************************************************************/
/* FUNCTION : meanalbedo                                          */
/*    calculate spatial mean of ALBEDO over entire drainage basin */
/*    for diffuse radiation from surrounding terrain              */
/*    called once every time step                                 */
/******************************************************************/

void meanalbedo()

{
    int     numbergrids=0;
    double  summe=0.0;

    for (i=firstrow; i<=lastrow; i++)
        for (j=firstcol[i]; j<=lastcol[i]; j++) {
            if(ALBEDO[i][j] != nodata) {
                numbergrids += 1;
                summe += ALBEDO[i][j];
            } /*endif*/
        }  /*endfor*/

    if(numbergrids != 0.0)
        meanalb = summe/numbergrids;
    else {
        printf("\n ERROR in function meanalbedo\n\n");
        exit(20);
    }

    return;
}



/******************************************************************/
/* FUNCTION  albedosnownewpoly                                     */
/*    13/03/2006, sicart PhD., Zongo Glacier                       */
/*    called from main     */
/******************************************************************/
/** if prec=0 snow depth: US army corps of engineers            ***/
/** if prec=0 snow thin: polynomial in power 3                  ***/
/** if prec=0 ice : linear decrease, not done yet               ***/
/** if prec>0 : albedo increase according to hock               ***/
/** all concerning "albedo is allowed only to drop to the value of ALBEDO BEFORE SNOWFALL
    after that the decay continues as if there were no snowfall"
	SUPPRESSED HERE!!! changed the 03 of march, 2001**/
/******************************************************************/


void albedosnowpoly() {
    /* Czgo = albk3 de hock, nzgo et ezgo doubles déclarés et initialisés dans variab.h */

    double afirn,anewsnow=0.90,aice;
    double alb1,alb2,snow;
    double albmax=0.90;      /*maximum snow albedo value*/
    //double tempfactor=1; /* L.G.: tempfactor is unused, can we delete? */

    afirn=albfirn;   /* from input.dat */
    aice=albice;     /* from input.dat */

    if(snowprec == 0) {        /*no snow fall, decrease in albedo*/
        numbdays[i][j] += timestep/24.0;    /*number of days in days*/

        /*   if(tempint[i][j] > 0)
             tempfactor = log(tempint[i][j]+1);   /faster decay with higher temp/
           else
             tempfactor = 0.1;    /no temp induced decay, only number of days/  tempfactor fixed to 1*/

        snow = SNOW[i][j];     /* SNOW is in cm !!!  ABLA and snowprec in mm !!! */

        alb1 = afirn + (anewsnow-afirn) * exp(-1*numbdays[i][j]/nzgo);

        if ((FIRN[i][j]!=0)&(FIRN[i][j]!=-1))
            alb2 = alb1;   /* firn area */
        else
            alb2= alb1 + (aice-alb1) * pow(1+10*snow/ezgo,-3);  /*snow in cm we, ee in mm we*/

        ALBEDO[i][j] = (float) alb2;

        /* albedo drop by 0.05 if snow has melted and firn below */
        /***** NOTE: re-programed 10/2000, as SNOW array does not exist if photos used ***/
        /*    if((snowtofirn[i][j] == 0) && (SNOW[i][j] == 0))  */
        /***** NOTE: re-programed 10/2000, as SNOW array does not exist if photos used ***/

        if((snowtofirn[i][j] == 0) && (surface[i][j] == 2)) {
            ALBEDO[i][j] = ALBALT[i][j] - 0.05;
            snowtofirn[i][j] = 1;
            /*array controls that this albedo drop only occurs once, boolean meaning*/
        }


        if(ALBEDO[i][j] < albmin)          /*albedo cannot be lower than albmin*/
            ALBEDO[i][j] = albmin;

    } else { /*SNOW FALL, ALBEDO INCREASES */
        ALBEDO[i][j] = ALBALT[i][j] + Czgo * snowprec;   /*albedo increased*/
        if(ALBEDO[i][j] > albmax)        /*upper limit of albedo*/
            ALBEDO[i][j] = albmax;
        numbdays[i][j] = 0;
    }

    return;
}


/******************************************************************/
/*  FUNCTION : shortwavebalance                                   */
/******************************************************************/

void shortwavebalance()

{
    /*  if (snetfromobsyes == 1) CHR added, if only climate station calculated and meas available then possible
      {  SWBAL[i][j] = GLOBAL[i][j] - ref;
         if (SWBAL[i][j] < 0.)
         {  SWBAL[i][j] = 0.;
            ref = GLOBAL[i][j];
         }
      }
       appears wrong therefore changed to: (Jan 2011)*/

    /*global and ref to be taken from measurements, no albedo parameterization*/
    /*only when calculations only for AWS*/
    if ((snetfromobsyes == 1) && (calcgridyes == 2))
        SWBAL[i][j] = glob - ref;
    else
        /*--------------------------------------------------------*/
        SWBAL[i][j] = GLOBAL[i][j]*(1-ALBEDO[i][j]);
    /*--------------------------------------------------------*/

    return;
}


/*========================================================================*/
/*   LONGWAVE RADIATION                                                   */
/******************************************************************/
/*  FUNCTION : longinstationnetradmeas                                  */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION */
/*             FROM MEASUREMENTS OF NET, GLOB, REFLECTED          */
/*             called from main once per time step                */
/******************************************************************/

void longinstationnetradmeas()

{
    /*LWout is initialized to startlongout in input.c*/
    LWin = net - glob + ref + LWout;

    if(LWin < 150) {
        fprintf(outcontrol,"Longin < 150:  %.2f  jd=%.2f \n",LWin,jd);
        LWin = 150;   /*avoid unrealistic value*/
    }

    return;
}



/******************************************************************/
/*  FUNCTION : longinstationkonzel                                */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION */
/*             FROM PARAMETERIZATION by Konzelmann et al. 1995    */
/******************************************************************/

void longinstationkonzel()

{
    float  sigma=0.0000000567;

    /*compute cloud amount from global to TOA radiation*/
    /*otherwise cloud amount is read from file*/
    if(methodlonginstation == 6)
        cloudsfromratioglobTOASedlar();

    /*bKonzel, eocKonzel and pKonzel defined as hidden variables in variab.h*/
    LWin = ( (0.23 + bKonzel*pow((e/(temp+273.15)),1/8)) *
             (1-pow(cloud/8,pKonzel)) + eocKonzel*pow((cloud/8),pKonzel)) * sigma*pow((temp+273.15),4);

    return;
}



/******************************************************************/
/*  FUNCTION : cloudsfromratioglobTOASedlar                       */
/*     CALCULATION OF CLOUD AMOUNT FROM THE RATIO OF MEASURED     */
/*     GLOBAL RADIATION AND TOP OF ATMOSPHERE RADIATION           */
/*       coefficients derived on Storglaciaren, Sedlar and Hock, 2009 */
/*   update Nov 2006, updated 9/2009*/
/******************************************************************/

void cloudsfromratioglobTOASedlar()

{
    float  a3,a2,a1,a0;    /*coefficients for Storglaciären*/
    float  xx=0.5;         /*arbitrarystart value for Glob/Toa in case Toa = 0, in winter, night*/

    if(timestep < 24) {
        a3= -3.564;        /*hourly coefficients for Storglaciären*/
        a2= 1.634;
        a1= -0.2324;
        a0= 1;
    }
    if(timestep == 24) { /*different coefficients for daily values*/
        a3= -7.934;        /*hourly coefficients for Storglaciären*/
        a2= 4.968;
        a1= -1.069;
        a0= 1.058;
    }

    /*after sunrise, before sunset and if weather station not shaded*/
    /*does not work during night, then last value is kept until next morning*/
    if( (topofatm > 15.0) && (DIRECT[rowclim][colclim]) > 15.0) {
        xx=glob/topofatm;
        cloud = a3*pow(xx,3) + a2*pow(xx,2) + a1*xx + a0;
        cloud = cloud*8;     /*cloud cover when read from file is 0-8*/

        if(cloud < 0)  cloud = 0;
        if(cloud > 8)  cloud = 8;
    } else {
        if(timestep == 24)
            cloud = 4;   /*arbitrary assumption for winter period = 4/8 clouds*/
        /*better than assuming that winter is equal to last day of day light*/
    }

    return;
}



/******************************************************************/
/*  FUNCTION : longinstationbrunts                                */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION */
/*             FROM PARAMETERIZATION by Brunts                    */
/******************************************************************/

void longinstationbrunts()

{
    float  sigma=0.0000000567;

    /* Not possible to call vappress() directly in case the climate station
       is not in area calculated */

    e = 6.1078 * exp(17.08085*temp/(234.175+temp)) * hum;

    LWin = sigma * pow((temp + 273.15),4)*(0.52 + 0.065 * sqrt(e/100));

    return;
}


/******************************************************************/
/*  FUNCTION : longinstationbrutsaert                                              */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION                  */
/*             FROM PARAMETERIZATION by Brutsaert for clear sky, zongo for clouds  */
/*        cloud is read from climate input but the column was computed	priorly	   */
/*        from ratio global radiation/topofatmosphere, fitted to Zongo data        */
/*        CLOUD VARIES FROM 0 TO 1                                                 */
/*        see Sicart, 2002, PhD thesis, p.260                                      */
/*     March 2006                                                                  */
/******************************************************************/

void longinstationbrutsaert()

{
    float  sigma=0.0000000567;
    float  coeff_a=0.4,coeff_b=1;	 /*PhD. sicart coefficients for zongo p.260   */

    float  Esatjes,ejes;      /* saturation vapour pressure */

    /*Esat is in hPa, convert to Pascal below;   temp in Celcius*/
    /*----------------------------------------------------------------------*/
    Esatjes = 6.1078 * exp(17.08085*temp/(234.175+temp));  /* jes copie de vappress */
    /*----------------------------------------------------------------------*/

    ejes = Esatjes*hum/100;  /* e in hPa !!!! jes */

    LWin = (1.24 * pow(ejes/(temp+273.15),1./7.)) *
           (1 + coeff_a * pow(cloud,coeff_b)) * sigma * pow((temp+273.15),4); /* fonction jes */

    return;
}


/***************************************************************************/
/*  FUNCTION : longinskyunobstructed                                       */
/*     sky longwave radiation obtained at climate station from rad data    */
/*     corrected as if the site was unobstructed by topography             */
/*     prior to interpolation over area  (based on Pluess, 1997)           */
/*     called from main once per timestep                                  */
/***************************************************************************/

void longinskyunobstructed()

{
    float  Lb=100.2;   /* emitted radiance of a 0 C blackbody W/m2/sr */
    float  a=0.77;
    float  b=0.54;

    /*surftempstationalt is initialized with 0 (variab.h), then temp of previous timestep*/
    LWinsky = (LWin - (1-SKYVIEW[rowclim][colclim]) *
               pi*(Lb+a*temp+b*surftempstationalt)) /
              SKYVIEW[rowclim][colclim];

    return;
}


/***************************************************************************/
/*  FUNCTION : longinpluess                                                */
/*     parameterization of longwave incoming rad according to Pluess, 1997 */
/*     called from main for each grid cell                                 */
/***************************************************************************/

void longinpluess()

{
    float  Lb=100.2;   /* emitted radiance of a 0 C blackbody W/m2/sr */
    float  a=0.77;
    float  b=0.54;

    /*SKY RADIATION PLUS TERRAIN RADIATION*/
    /*----------------------------------------------------------------------*/
    LONGIN[i][j] = SKYVIEW[i][j] * LWinsky +
                   (1-SKYVIEW[i][j]) * pi*(Lb + a*tempint[i][j] + b*surftemp[i][j]);
    /*----------------------------------------------------------------------*/

    return;
}


/******************************************************************/
/*  FUNCTION : longoutcalc                                        */
/*             calculate longwave outgoing (Stephan Boltzmann)    */
/*             for each time step and each grid point             */
/* update 4/2010  include emissivities < 1   */
/******************************************************************/

void longoutcalc()

{
    float  sigma=0.0000000567;
    float  longwave_in;
    /*extra variable because it can be array if spatially variable or not (if invariable)*/

    if(methodlongin == 1)       /*LONGWAVE INCOMING RADIATION CONSTANT IN SPACE*/
        longwave_in = LWin;
    else     /*2= LONGWAVE INCOMING RADIATION VARIABLE IN SPACE*/
        longwave_in = LONGIN[i][j];

    /*emissivity defined in variab.h*/
    /*      LONGOUT[i][j] = sigma*pow((surftemp[i][j]+273.15),4);   */

    LONGOUT[i][j] = sigma*emissivitysurf*pow((surftemp[i][j]+273.15),4)+(1-emissivitysurf)*longwave_in;

    return;
}


/*************************************************************************/
/*  FUNCTION :  surftempfromlongout                                      */
/*     calculate surface temp from longwave outgoing (Stephan Boltzmann) */
/*     for each time step and the climate station grid cell              */
/* update 4/2010  include emissivities < 1   */
/*************************************************************************/

void  surftempfromlongout()

{
    float  sigma=0.0000000567;
    float  longwave_in;
    /*extra variable because it can be array if spatially variable or not (if invariable)*/

    if(methodlongin == 1)       /*LONGWAVE INCOMING RADIATION CONSTANT IN SPACE*/
        longwave_in = LWin;
    else   /*2= LONGWAVE INCOMING RADIATION VARIABLE IN SPACE*/
        longwave_in = LONGIN[rowclim][colclim];

    /*-273.15 --> convert surface temperature to Celcius*/
    /****** surftemp[rowclim][colclim] = pow((LWout/sigma),0.25) -273.15;     only valid if emissivity=1*/

    surftemp[rowclim][colclim] = pow(((LWout-(1-emissivitysurf)*longwave_in)/(sigma*emissivitysurf)),0.25) -273.15;

    if(surftemp[rowclim][colclim] > 0)
        surftemp[rowclim][colclim] = 0;

    /*  printf(" surftempfromlongout: jd=%.0f time=%.0f surftemp=%.2f LWout=%.2f\n",jd,zeit,surftemp[rowclim][colclim],LWout);  */

    return;
}



/*=================== RADIATION BALANCE ================================*/
/******************************************************************/
/*  FUNCTION : radiationbalance                                   */
/*             for each time step and each grid point             */
/*             longwave incoming assumed spatially constant       */
/******************************************************************/

void radiationbalance()

{
    /* different equations because longwave variable is 1-dem or 2-dimensional
       depending on whether it is constant or variable in space */

    if(methodlongin == 1) {     /*LONGWAVE INCOMING RADIATION CONSTANT IN SPACE*/
        if(methodsurftempglac == 1)   /*LONGWAVE OUTGOING RAD CONSTANT IN SPACE = MELTING*/
            NETRAD[i][j] = SWBAL[i][j] + LWin - LWout;
        else                /*LONGWAVE OUTGOING RADIATION VARIABLE IN SPACE*/
            NETRAD[i][j] = SWBAL[i][j] + LWin - LONGOUT[i][j];
    } else {         /*LONGWAVE INCOMING RADIATION VARIABLE IN SPACE*/
        if(methodsurftempglac == 1)   /*LONGWAVE OUTGOING RAD CONSTANT IN SPACE = MELTING*/
            NETRAD[i][j] = SWBAL[i][j] + LONGIN[i][j] - LWout;
        else                /*LONG OUT VARIABLE IN SPACE OR TAKEN FROM MEASUREMENTS*/
            NETRAD[i][j] = SWBAL[i][j] + LONGIN[i][j] - LONGOUT[i][j];
    }

    /*take all radiation data from file, only if only station grid cell computed*/
    if((allradiationfromfile == 1) && (methodsurftempglac == 3) && (calcgridyes == 2))
        NETRAD[i][j] = glob - ref + LWin - LWout;

    return;
}

