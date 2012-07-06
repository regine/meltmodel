/****************************************************************************/
/* FILE   turbul.c                                                          */
/*         SPATIAL INTERPOLATION OF METEOROLOGICAL INPUT FACTORS TO GRID    */
/*          temperature, air pressure, vapour pressure, precipitation       */
/*         CALCULATION TURBULENT HEAT FLUXES                                */
/*  29.9.1997, last update 28 April 2010 */
/****************************************************************************/


#include "turbul.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "initial.h"
#include "grid.h"
#include "radiat.h"
#include "snowinput.h"
#include "snowmodel.h"
#include "userfile.h"
#include "writeout.h"

#include "variabex.h"      /* all global VARIABLES */


/**********************************************************/
/* FUNCTION  tempgradient                                 */
/*   CALCULATION OF TEMPERATURE OF WHOLE GRID             */
/*   USING A LINEAR DECREASE OF TEMP WITH ELEVATION       */
/* constant lapse rate or monthly varying read from input.dat */
/* or variable lapse rate read from climate data file     */
/*   called from function tempinterpol (every time step   */
/*   climate station either outside or inside the calculated area */
/*  NEW 27 April 2010 */
/**********************************************************/

void tempgradient() {
    float lapserate;    /*either constant or monthly variable*/

    if(monthtempgradyes == 0)
        lapserate = tempgrad;
    else
        lapserate = monthtempgrad[monat];    /*different lapse for each month*/

    for (i=firstrow; i<=lastrow; i++) /*only rows and col to be calculated*/
        for (j=firstcol[i]; j<=lastcol[i]; j++) {
            if (griddgmdrain[i][j] != nodata) { /*only for area to be calculated*/
                switch(climoutsideyes) { /*climate data not in area to be calculated*/
                case 0 :
                    tempint[i][j] = temp + (griddgm[i][j]-griddgm[rowclim][colclim])/100 * lapserate;
                    break;
                case 1 :
                    tempint[i][j] = temp + (griddgm[i][j]-heightclim)/100 * lapserate;
                    break;
                case 2 :
                    tempint[i][j] = temp + (griddgm[i][j]-heightclim)/100 * lapserate;
                    break;     /*fixed elevation for temp interpol, but climate station elev for precip*/
                }  /*endswitch*/
            }  /*endif area to be calculated*/
        }  /*endfor*/
    return;
}


/**********************************************************/
/* FUNCTION  tempinterpol                                 */
/*   CALCULATION OF TEMPERATURE OF WHOLE GRID             */
/*   USING A LINEAR DECREASE OF TEMP WITH ELEVATION       */
/*   OR READING TEMPERATURE GRID FILES FOR EACH TIME STEP */
/*   called from main every time step                     */
/*   climate station either outside or inside the calculated area */
/*  last update 13 April 2010 RH */
/**********************************************************/

void tempinterpol() {
    /* L.G.: lapserate is unused, can we delete it? */
    // float lapserate;    /*either constant or monthly variable*/

    /*T from files has already been adjusted for bias between ERA and glacier and
      for climate scenario; this is done in initial.c, readclim()*/

    switch(methodtempinterpol) {
    case 1:
        tempgradient();
        break;   /*constant or monthly variable lapse rate from input.dat*/
    case 2:
        tempgradient();
        break;   /*each time step variable lapse rates from climate file*/
    case 3:           /*read temperatures from existing grid*/
        strcpy(dummy,pathdirectfile);
        sprintf(nametempgrid,"temp%04d%03d%02d.bin",(int)year,(int)jd,(int)zeit);
        strcat(dummy,nametempgrid);

        if ((intempgrid = fopen (dummy,"rb")) == NULL)  {
            printf("\n temperature grid file not found: %s\n\n",dummy);
            exit(4);
        }  /*ENDIF*/

        if ( (fread(&(x[1]),sizeof(float),12,intempgrid)) !=12 )  {            /*FIRST 12 ROWS*/
            printf("\n ERROR in file %s \n",nametempgrid);
            exit(9);
        }
        if ((fread(&(tempint[1][1]),sizeof(float),ncols*nrows,intempgrid)) != ncols*nrows)  {
            printf("\n ERROR in reading temperature file %s  jd=%4.1f,zeit=%4.1f\n",nametempgrid,jd,zeit);
            exit(10);
        }
        closefile(&intempgrid,nametempgrid);

        /* printf("in tempinterpol(): tempint = %.2f row=%d col=%d jd=%4.1f,zeit=%4.1f\n ",tempint[rowclim][colclim],rowclim,colclim,jd,zeit); */

        break;
    }  /*end switch*/
    return;
}


/***************************************************************************/
/* FUNCTION   airpress                                                     */
/*   CALCULATION OF AIR PRESSURE DEPENDING ON ELEVATION a.s.l.             */
/*   taken from KONZELMANN et al. Global and Planetary Change, 1994, p.161 */
/* called from main every time step and each grid cell, not stored in array*/
/***************************************************************************/

void airpress()

{
    /* elevation in m, P0 is in Pascal */
    /*------------------------------------------------*/
    p = exp(-0.0001184*griddgm[i][j]) * P0;
    /*------------------------------------------------*/

    return;
}


/***************************************************************************/
/* FUNCTION   vappress                                                     */
/*     CALCULATION OF VAPOUR PRESSURE FROM RELATIVE HUMIDITY               */
/*     aus Weischet, p.138                                                 */
/* called from main every time step and each grid cell, not stored in array*/
/***************************************************************************/

void vappress() {
    float  Esat;      /* saturation vapour pressure */

    /*Esat is in hPa, convert to Pascal below;   temp in Celcius*/
    /*----------------------------------------------------------------------*/
    Esat = 6.1078 * exp(17.08085*tempint[i][j]/(234.175+tempint[i][j]));
    /*----------------------------------------------------------------------*/

    e = Esat*hum;  /* e=in Pascal: e=Esat*hum/100 is in hPa, convert to Pa: *100 */

    return;
}


/***************************************************************************/
/* FUNCTION   vappressicesnow                                              */
/*     CALCULATION OF VAPOUR PRESSURE OF ICE OR SNOW SURFACE               */
/*     needed if surface temp lowered by iteration                         */
/***************************************************************************/

void vappressicesnow() {
    /*e0 in hPa;   temperature in Celcius*/
    /*------------------------------------------------------------------------*/
    e0 = 6.1078 * exp(17.08085*surftemp[i][j]/(234.175+surftemp[i][j]));
    /*------------------------------------------------------------------------*/
    e0 = e0*100;    /*in Pascal*/

    return;
}

/* ========== TURBULENT FLUXES ============================================ */

/****************************************************************************/
/* FUNCTION   sensescher                                                    */

/* CALCULATION OF SENSIBLE HEAT HEAT ACCORDING TO ESCHER-VETTER, 1980, p.14 */
/*     sens = transfer coefficient x air temperature
       trans coeff = 5.7 W/m2/K * sqrt(wind speed) /m/s)
       energy transferred to surface defined as positive (condensation)
       assumption: surface temp = 0 -> melting surface                      */
/****************************************************************************/

void sensescher()

{
    float transcoeff = 5.7*sqrt(wind);
    SENSIBLE[i][j] = transcoeff * tempint[i][j];      /* in W/m2 */

    return;
}


/***************************************************************************/
/* FUNCTION   latescher                                                    */

/* CALCULATION OF LATENT HEAT HEAT ACCORDING TO ESCHER-VETTER, 1980, p.15  */
/*   as a function of transfer coefficient (wind dependent),
     vapour pressure, air pressure, latent heat of evaporation, cp
       assumption: surface temp = 0 -> melting surface -> e0=611 Pa
		   relative humidity constant over whole area              */
/***************************************************************************/

void latescher()

{
    float   transcoeff = 5.7*sqrt(wind);    /* transfer coefficient */
    float   Lv = 2514000.0;   /* latent heat of evaporation  [J/kg] */
    float   cp = 1010.0;      /* specific heat capacity  [J/kg/K]   */

    /*------------------------------------------------------------*/
    LATENT[i][j] = transcoeff *0.623 * Lv/ (p*cp) * (e-e0);
    /*------------------------------------------------------------*/
    /* in W/m2*/

    return;
}


/***************************************************************************/
/* FUNCTION   roughnesslength                                              */
/* CALCULATION OF ROUGHNESS LENGTHS ACCORDING TO turbulmethod=2            */
/*  constant roughness lengths, different z0w and z0T                      */
/*  function called for each grid cell from function sensible              */
/*       only if methodturbul = 2 (no stability)                           */
/***************************************************************************/

void roughnesslength() {
    if(surface[i][j] == 1)           /*snow surface*/
        z0w=z0wice/dividerz0snow;
    else                             /*ice*/
        z0w=z0wice;

    /*** increase of roughness length with decreasing elevation if z0proz > 0 ***/
    if(z0proz > 0.0) {
        z0w = z0w+z0w*((griddgm[rowclim][colclim]-griddgm[i][j])*z0proz/100/100);
        if(z0w < z0min)
            z0w = z0min;
        if(z0w > z0max)
            z0w = z0max;
    }

    z0T = z0w/dividerz0T;
    z0e = z0T;

    return;
}

/***************************************************************************/
/* FUNCTION   roughnesslengthAndreas                                       */
/* CALCULATION OF ROUGHNESS LENGTHS ACCORDING TO Andreas 1997              */
/*                      surface renewal theory, see Munro 1990             */
/*  function of Reynolds number, then assumed constant for entire glacier  */
/*  function called for climate station grid from function iterationstation */
/***************************************************************************/

void roughnesslengthAndreas() {
    /*   float  b0T,b1T,b2T, b0e,b1e,b2e;*/

    /* float viscos = 0.000015;   kinematic viscosity of air m2/s  for 0 degrees */
    float viscos = 0.0000139;     /*kinematic viscosity of air m2/s for 5 degrees*/

    /*change z0 as fixed ratio in case of snow surface*/
    if(surface[i][j] == 1)           /*snow surface*/
        z0w=z0wice/dividerz0snow;         /*z0w can be different for ice and snow*/
    else                             /*ice*/
        z0w=z0wice;

    float Re = frictionveloc*z0w/viscos; /*Reynoldnumber*/

    /*formulated as in Munro; is the same as below for rough case*/
    /*   z0T = exp(log(z0w)+0.317-0.565*log(Re)-0.183* pow((log(Re)),2) );
         z0e = exp(log(z0w)+0.396-0.512*log(Re)-0.180* pow((log(Re)),2) ); */

    /*coefficients according to Van as et al., BoundLayerMetin press*/
    /*   if(Re <= 0.135) */   /*smooth regime*/
    /*   { b0T = 1.250;  b1T = 0.;  b2T = 0.;
         b0e = 1.610;  b1e = 0.;  b2e = 0.;
       }
       else
       { if(Re < 2.5)*/   /*transition*/
    /*      {  b0T = 0.149;  b1T = -0.550;  b2T = 0.;
             b0e = 0.351;  b1e = -0.628;  b2e = 0.;
          }
          else   */       /*rough*/
    /*      {  b0T = 0.317;  b1T = -0.565;  b2T = -0.183;
             b0e = 0.396;  b1e = -0.512;  b2e = -0.180;
          }
       }

       z0T = z0w*exp( b0T+b1T*log(Re) +  b2T*pow(log(Re),2) );
       z0e = z0w*exp( b0e+b1e*log(Re) +  b2e*pow(log(Re),2) );*/

    /*coefficients according to Andreas 1987, Boundary Layer Meteor. 38, 159-184 */
    if (Re <= 0.135) { /*Smooth regime*/
        z0T = z0w*exp(1.250);
        z0e = z0w*exp(1.610);
    } else if ((Re > 0.135) && (Re < 2.5)) { /*Transitional regime*/
        z0T = z0w*exp(0.149-0.550*log(Re));
        z0e = z0w*exp(0.351-0.628*log(Re));
    } else if (Re >= 2.5) { /*Rough regime */
        z0T = z0w*exp(0.317-0.565*log(Re)-0.183* pow((log(Re)),2) );
        z0e = z0w*exp(0.396-0.512*log(Re)-0.180* pow((log(Re)),2) );
    }

    return;
}


/***************************************************************************/
/* FUNCTION   moninobukhovlength                                             */
/* CALCULATION OF FRICTION VELOCITY AND STABILITY FUNCTIONS, Munro 1990    */
/*   stability function according to Beljaar and Holtslag, 1991            */
/*  function called for climate station grid from function iterationstation */
/***************************************************************************/

void moninobukhovlength() {
    /* L.G.: Tgeo is unused, can we delete it? */
    //float  Tgeo;         /*geometric mean of air temperature*/
    float  dens0 = 1.29;     /*air densitiy at standard pressure  in kg/m3*/
    float  k     = 0.40;     /*Karmans constant*/
    float  cp    = 1010;     /*heat capacity      in J/kg/K*/
    float  g     = 9.81;     /*gravitational constant*/
    float  MoninL_lowerlimit = 0.3;
    /*   float  Rich; */

    /*COMPUTE FRICTION VELOCITY ****/
    frictionveloc = k*wind/(log(z2/z0w)-PhiM);

    /*COMPUTE MEAN AIR TEMPERATURE FOR MONIN-OBUKHOV LENGTH COMPUTATION ****/
    /*Tgeo is absolute temp of the air between surface and measurement height */
    /*   geometric mean */
    /* Tgeo = pow( ((surftemp[i][j]+273.15)*(tempint[i][j]+273.15)),0.5 ); */

    /*COMPUTE MONIN-OBUKHOV LENGTH COMPUTATION ****/
    /*function only called if sensible heat flux not equal 0*/
    /*    MoninL = dens0*p/P0*cp*(pow(frictionveloc,3))*Tgeo/(k*g*SENSIBLE[i][j]); */

    MoninL = dens0*p/P0*cp*(pow(frictionveloc,3))*(tempint[i][j]+273.15)/(k*g*SENSIBLE[i][j]);

    /*   Rich = 9.81/(temp+273.15)*2*temp/(wind*wind);  bulk Richardson number */

    /*avoid crashing of program in case of too high stability*/
    if ((MoninL < MoninL_lowerlimit) && (MoninL > 0)) {
        MoninL = MoninL_lowerlimit;
        /*printf(" after MoninL = %f\n",MoninL);  */
    }

    /*    printf("  u* =%.3f L=%f ",frictionveloc,MoninL);  */

    return;
}


/***************************************************************************/
/* FUNCTION   stabilityBeljaar                                             */
/* CALCULATION OF STABILITY FUNCTIONS                                      */
/*   stability function (stable) according to Beljaar and Holtslag, 1991   */
/*  function called for climate station grid from function sensiblestabilityiteration */
/***************************************************************************/

void stabilityBeljaar() {
    float  aa,b,c,d;

    aa=1;
    b=0.667;
    c=5;
    d=0.35;

    if (SENSIBLE[i][j] != 0.) {
        PhiM= -((aa*z2/MoninL) + b*(z2/MoninL-c/d)*exp(-d*z2/MoninL) + b*c/d);
        PhiH= -((pow((1+2/3*aa*z2/MoninL),1.5)) + b*(z2/MoninL-c/d)*exp(-d*z2/MoninL) + b*c/d-1);
    } else {
        printf("\n exit function stabilityBeljaar, Monin L = 0\n\n");
        exit(6);
    }

    return;
}


/***************************************************************************/
/* FUNCTION   stabilityunstable                                            */
/* CALCULATION OF STABILITY FUNCTION FOR UNSTABLE CASE                     */
/*                     according to Panofsky and Dutton, 1984              */
/*    called for climate station grid from function iterationstation       */
/***************************************************************************/

void stabilityunstable() {
    float  xx;

    xx=pow((1-16*z2/MoninL),0.25);
    PhiM=log((1+pow(xx,2))/2 * pow(((1+xx)/2),2)) - 2*atan(xx)+pi/2;
    PhiH=2*log(0.5*(1+ (pow( (1-16*z2/MoninL),0.5) )));

    return;
}


/***************************************************************************/
/* FUNCTION   sensiblestabilityiteration                                  */
/* CALCULATION OF SENSIBILE HEAT FLUX FOR CLIMATE STATION GRID             */
/*   AND STABILITY FUNCTIONS (assumed constant for rest of grid)           */
/*  function called once every time step for climate station grid          */
/*     from function iterationstation (if surftemp is iterated             */
/*         and from main (if turbulmethod=3=stability)                     */
/***************************************************************************/

void sensiblestabilityiteration() {
    float sensible_lastiteration;   /*sensible heat flux previous iteration*/
    float difference;  /*diff between sensible heat flux and value of previous iteration*/
    float maxdiff=999;     /*if difference larger than this, continue interation*/
    int   iterationsteps=0;
    int   getoutloop = 0;    /*iterations as long as it is 0, set to 1 when z/L found*/

    z0w=z0wice;           /*start values for first iteration*/
    z0T=z0w/dividerz0T;

    PhiM=0;
    PhiH=0;      /*first run with neutral stability*/
    sensible_lastiteration = -9999;
    difference = 9999;    /*must be >maxdiff to allow first run through while-loop*/
    MoninL = 1000;        /* >0 to allow first run through while-loop*/


    /*printf("\nJD = %.0f %.0f temp=%.2f wind=%.2f surfT=%.2f\n ",jd,zeit,temp,wind,surftemp[i][j]);*/

    while(getoutloop == 0) {   /*iterative determination of z/L and stab functions*/
        /*by the procedure L can become 0 and the program crashes (see funct moninobukhovlength)*/
        /*to avoid that if L < 0.3, iteration should stop by setting getoutloop to 1*/
        if(MoninL == 0.3)
            getoutloop = 1;

        sensible();     /*first guess of sensible heat flux with neutral stability*/

        /*   printf("sensible neutral %f\n",SENSIBLE[i][j]);  */

        /*printf("   z/L = %.5f  z0w=%.5f z0T=%f PhiM=%f PhiH=%f sens=%.2f iter=%d rich=%f\n",
        2/MoninL,z0w,z0T,PhiM,PhiH,SENSIBLE[i][j],iterationsteps,Rich);*/

        if(SENSIBLE[i][j] == 0)      /*stability computation not needed*/
            getoutloop = 1;   /*to stop next run through while-loop*/
        else {
            moninobukhovlength();      /*computes friction vecolity and L*/

            if(method_z0Te == 1)       /*method to compute z0T and z0e*/
                roughnesslength();       /*z0T and z0e fixed ratio to z0w*/
            /* else */
            if(method_z0Te == 2)
                roughnesslengthAndreas();
            /* z0T,e according to Andreas, 1987, needs friction velocity*/
            if(method_z0Te == 3)       /*method to compute z0T and z0e*/
                roughnesslengthAndreasmodified();

            if(MoninL > 0)              /*stable stratification*/
                stabilityBeljaar();     /*computes stability functions, needs L*/
            else
                stabilityunstable();    /*compute stability function for unstable case*/

            /***** DETERMINE ITERATION STOP CONDITION *****/
            difference = SENSIBLE[i][j] - sensible_lastiteration;
            if(difference < 0)
                difference = (-1)*difference;    /*take absolute difference*/
            maxdiff = SENSIBLE[i][j]/100*1;     /* 1% of the sensible heat flux*/
            if(maxdiff < 0)
                maxdiff = (-1)*maxdiff;
            if(difference < maxdiff)
                getoutloop = 1;

            sensible_lastiteration = SENSIBLE[i][j];

        }  /*endif else  sensible not 0*/

        /*in case the iteration does not converge the loop is exited and the sensible heat flux*/
        /*is computed without stability correction; case occurred only once for 1993/1994 Storglaciären*/
        iterationsteps += 1;
        if(iterationsteps > 100) {
            fprintf(outcontrol," function sensiblestabilityiteration %.0f %.0f temp=%.2f surftemp=%.2f L=%f  \n",
                    jd,zeit,temp,surftemp[i][j],MoninL);
            PhiM = 0;
            PhiH = 0;
            MoninL=1000;
            roughnesslength();     /*use fixed ratio z0T/z0w*/
            sensible();     /*compute sens heat without stability correction*/
            getoutloop = 1;
        }

    }  /*endwhile*/

    if(SENSIBLE[i][j] != 0) {  /*stability computation and recalc of sens not needed*/
        if( ((z2/MoninL) < 0.03) && ((z2/MoninL) >=0) ) {
            PhiM = 0;
            PhiH = 0;
        }

        sensible();  /*final calculation of sens heat flux with "right" stability correction*/
    }  /*endif*/


    /*printf("\nafter iter:  z/L=%.3f temp=%.1f wind=%.3f z0w=%.5f z0T=%.5f PhiM=%f PhiH=%f sens=%.2f iter=%d\n",
    2/MoninL,temp,wind,z0w,z0T,PhiM,PhiH,SENSIBLE[i][j],iterationsteps);  */

    PhiE = PhiH;    /*for computation of latent heat flux*/

    return;
}

/***************************************************************************/
/* FUNCTION   sensible                                                     */
/* CALCULATION OF SENSIBLE HEAT HEAT ACCORDING TO Hock and Holmgren, 1996  */
/*  constant roughness lengths, different z0w and z0T                      */
/*  or according to Andreas (1987)*/
/***************************************************************************/

void sensible()

{
    float  dens0 = 1.29;     /*air densitiy at standard pressure  in kg/m3*/
    float  cp    = 1010;     /*heat capacity      in J/kg/K*/
    float  k     = 0.40;     /*Karmans constant*/
    float  dragcoeff;

    if(methodturbul == 2) {   /*no stability*/
        if(method_z0Te == 1)
            roughnesslength();    /*allows spatial variation with gradient*/
        else {    /*zoTe according to Andreas (1987)*/
            frictionveloc = k*wind/(log(z2/z0w)-PhiM);
            if (method_z0Te == 2)
                roughnesslengthAndreas();   /*no spatial variation but with time*/
            if(method_z0Te == 3)       /*method to compute z0T and z0e*/
                roughnesslengthAndreasmodified();
        }
    }
    /*if stability considered (methodturbul=3) roughness lengths have already been
      computed (call from sensiblestabilityiteration), must be done there because
      first iteration step is done with roughness lengths according to fixed ratio*/
    /*roughness lengths only computed for climate station grid and assumed constant in space*/
    /*PHiM, PHiH, PhiE are set to 0 in variab.h for case of turbulmethod=2*/

    /*-----------------------------------------------------------------------------*/
    dragcoeff = k*k * p/(P0 * (log(z2/z0w)-PhiM)*(log(z2/z0T)-PhiH) );
    SENSIBLE[i][j] = dragcoeff * dens0*cp * wind*(tempint[i][j]-surftemp[i][j]);
    /*-----------------------------------------------------------------------------*/

    return;
}


/***************************************************************************/
/* FUNCTION   latent                                                       */
/* CALCULATION OF LATENT HEAT HEAT as in Hock and Holmgren, 1996           */
/*  constant roughness lengths, different zow and zoT                      */
/***************************************************************************/

void latent()

{
    float  dens0 = 1.29;     /*air densitiy at standard pressure  in kg/m3*/
    float  k     = 0.40;     /*Karmans constant*/
    float  Lv    = 2514000;  /*latent heat of evaporation  in J/kg*/
    float  Ls    = 2849000;  /*latent hate of sublimation  in J/kg*/
    float  L;      /* Lv or Ls depending on surface temperature */
    float  dragcoeff;   /*stability function*/


    /***LATENT HEAT OF EVAPORATION OR SUBLIMATION see Greuel and Konzelmann 1994***/
    L = Ls;                           /*sublimation*/
    if (((e-e0) > 0.) && (surftemp[i][j] == 0.))
        L = Lv;                /*condensation as liquid water if melting surface*/

    /*------------------------------------------------------------*/
    dragcoeff = k*k * 0.623/(P0 * (log(z2/z0w)-PhiM)*(log(z2/z0e)-PhiE) );
    LATENT[i][j] = dragcoeff * dens0*L * wind*(e-e0);
    /*------------------------------------------------------------*/

    /*e0 computed in vappressicesnow: called from iterationstation*/

    return;
}


/* =============== PRECIPITATION ================================================ */


/***************************************************************************/
/* FUNCTION   readprecipindexmap                                                */
/*     READ PRECIPITATION INDEX MAP                                             */
/*     contains a value for each grid cell by which the precip at the AWS       */
/*     is multiplied by, to account for spatially stable precip patters         */
/*    called from main once before the time step loop if methodprecipinterpol=2 */
/***************************************************************************/

void readprecipindexmap() {
    /*** OPEN INDEX  FILE ****/
    strcpy(dummy,inpath);
    strcat(dummy,"precipindexmap.bin");
    if ((inprecipindexgrid = fopen (dummy,"rt")) == NULL)  {
        printf("\n ERROR : Climate data file not found !!!\n %s \n\n",dummy);
        fclose(inprecipindexgrid);
        exit(4);
    }

    precipindexmap=matrixreserv(1,nrows,1,ncols);     /*SPEICHERRESERVIERUNG*/

    if ( (fread(&(x[1]),sizeof(float),12,inprecipindexgrid)) !=12 )  {   /*READ FIRST 12 ROWS*/
        printf("\n ERROR in file %s \n (File initial.c)\n",dummy);
        fclose(outcontrol);
        exit(7);
    }
    if ((fread(&(precipindexmap[1][1]),sizeof(float),ncols*nrows,inprecipindexgrid)) != ncols*nrows) {
        printf("\n ERROR in reading grid data %s \n",dummy);
        fclose(inprecipindexgrid);
        exit(8);
    }
    closefile(&inprecipindexgrid,dummy);

    printf("    Gridfile %s opened and read \n",dummy);

    return;
}



/***************************************************************************/
/* FUNCTION   readprecipfromfile                                           */
/*     READ PRECIPITATION FROM FILE FOR EACH TIME STEP                     */
/*    called from main for each time step loop if methodprecipinterpol=3   */
/***************************************************************************/

void readprecipfromfile() {
    char   nameprecipgrid[80];

    strcpy(dummy,pathdirectfile);
    sprintf(nameprecipgrid,"precip%04d%03d%02d.bin",(int)year,(int)jd,(int)zeit);

    /*&&&&&&&*/
    /*   printf("precipgridfiles = %s  jd=%f  time=%.2f",dummy,jd,zeit);  */

    strcat(dummy,nameprecipgrid);
    if ((inprecipreadgrid = fopen (dummy,"rb")) == NULL)  {
        printf("\n temperature grid file not found: %s\n\n",dummy);
        exit(4);
    }  /*ENDIF*/

    if ( (fread(&(x[1]),sizeof(float),12,inprecipreadgrid)) !=12 )  {            /*FIRST 12 ROWS*/
        printf("\n ERROR in file %s \n",nameprecipgrid);
        exit(9);
    }
    if ((fread(&(precipreadgrid[1][1]),sizeof(float),ncols*nrows,inprecipreadgrid)) != ncols*nrows)  {
        printf("\n ERROR in reading temperature file %s  jd=%4.1f,zeit=%4.1f\n",nameprecipgrid,jd,zeit);
        exit(10);
    }
    closefile(&inprecipreadgrid,nameprecipgrid);
    /*   printf("tempint = %.2f  ",precipreadgrid[rowclim][colclim]);  */

    closefile(&intempgrid,nameprecipgrid);

    return;
}



/***************************************************************************/
/* FUNCTION   precipinterpol                                               */
/*            INTERPOLATION OF PRECIPITIATION, DETERMINE RAIN OR SNOW      */
/*       snow is assumed below the threshold temperature minus 1 degree    */
/*	 and rain above T0 plus 1 degree, inbetween there is a mixtures    */
/*	 of snow and rain which is determined by linear interpolation      */
/*   3 methods how to extrapolate precip over the area                  */
/*    called from main for every time step and for each grid               */
/***************************************************************************/

void precipinterpol()

{
    float halfrange=1;   /*mixed rain and snow in temp range of 2x halfrange*/
    /*here within a temperature range of 2 degrees*/
    float referenceelevation;  /*elevation of reference height*/


    if(prec > 0) {    /*precipitation falls in timestep considered*/
        switch(climoutsideyes) {  /*climate station outside (1) or inside grid (0)*/
        case 0 :
            referenceelevation = griddgm[rowclim][colclim];    /*climate station elevation*/
            break;
        case 1 :
            referenceelevation = heightclim;  /*fixed elevation from which extrapolation*/
            break;
        case 2 :
            referenceelevation = griddgm[rowclim][colclim];
            break;       /*use this for precip, but temp from fixed height*/
        }


        /*&&&&&&&& ------ FOR ANNALS OF GLACIOLOGY CAMBRIDGE PAPER -------- &&&&&&*/
        /*   referenceelevation = 1280;    */
        /*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&6*/


        switch(methodprecipinterpol) {    /*NEW APRIL 2010*/
        case 1:   /*precipitation gradient*/
            /*PRECIP CORRECTION AND INTERPOLATION (in mm/timestep) */
            precip = prec+prec*preccorr/100.0;   /*CORRECTION DUE TO GAUGE UNDERTAKE*/

            /* beyond certain elevation (maxprecelev) different prec gradient */
            if(griddgm[i][j] <= precgradelev)    /*HEIGHT-DEPENDENT PRECIPITATION GRADIENT*/
                precip = precip - ( (referenceelevation-griddgm[i][j])*precip*precgrad/100/100);
            else {
                /*first bring precipitation of the elevation where new gradient applies*/
                precip = precip - ( (referenceelevation-precgradelev)*precip*precgrad/100/100);
                /*then apply new gradient from this elevation*/
                precip = precip - ( (precgradelev-griddgm[i][j])*precip*precgradhigh/100/100);
            }

            break;

        case 2:   /*multiply precip read from AWS file by value from index map*/
            precip = prec * precipindexmap[i][j];
            break;

        case 3:   /*read precipitation from file for each time step*/
            precip = precipreadgrid[i][j];
            break;

        }  /*end switch methodprecipinterpol*/

        /*DETERMINATION IF RAIN OR SNOW - IMPORTANT FOR WATER FOR DISCHARGE AND ALBEDO*/
        if(tempint[i][j] <= (T0-halfrange)) {  /*ONLY SNOW*/
            rainprec = 0.0;           /* NO RAIN */
            snowprec = precip;        /* ALL PRECIP FALLS AS SNOW */
        } else { /*RAIN OR MIXTURE*/
            if (tempint[i][j] >= (T0+halfrange)) {  /* ONLY RAIN */
                rainprec = precip;
                snowprec = 0;       /* NO SNOW */
            } else { /*MIXTURE OF RAIN AND SNOW*/
                snowprec = ((T0+halfrange)-tempint[i][j])/2 * precip;
                rainprec = precip - snowprec;
            }
        } /*endelse*/

        if (rainprec < 0)     /*can happen when interpolating grid cells below station*/
            rainprec = 0;
        if (snowprec < 0)
            snowprec = 0;

        /*add more snow: snow assumed to have larger error; different factors for glacier and outside glacier possible*/
        if (griddgmdrain[i][j] == griddgmglac[i][j])      /*on glacier*/
            snowprec = snowprec*snowmultiplierglacier;
        else
            snowprec = snowprec*snowmultiplierrock;    /*grid cell outside glacier but in drainage basin*/


        RAIN[i][j] = rainprec;   /*in mm as in climate input file*/
        /*RAIN is stored in array because it is needed for discharge routines*/
        /*snow prec not needed unless simulation with initial snow cover, then it is
          added to SNOW array in function snowcover*/

    } /*end if precip*/
    else {   /*no precipitation*/
        RAIN[i][j] = 0;
        snowprec = 0;
        rainprec = 0.;
        precip = 0;   /*NEW July 2010*/
    }

    return;
}


/***************************************************************************/
/* FUNCTION   precipnoetzlidip                                             */
/*            INTERPOLATION OF PRECIPITIATION, DETERMINE RAIN OR SNOW      */
/*            only rain, no snow, no mixture snow and rain                 */
/*    called from main for every time step and for each grid               */
/*    function used by Ch. Noetzli for his MSc thesis*/
/***************************************************************************/

void precipnoetzlidip() {
    if(prec > T0) {    /*precipitation falls in timestep considered*/
        /*PRECIP CORRECTION AND INTERPOLATION (in mm/timestep) */
        precip = prec+prec*preccorr;   /*CORRECTION DUE TO GAUGE UNDERTAKE*/
        precip = precip - ((griddgm[rowclim][colclim]-griddgm[i][j])*precip*
                           precgrad/100/100);    /*HEIGHT-DEPENDENT PRECIPITATION GRADIENT*/

        rainprec = precip;
        snowprec = 0;

        RAIN[i][j] = rainprec;   /*in mm as in climate input file*/
        /*RAIN is stored in array because it is needed for discharge routines*/
        /*snow prec not needed unless albedo is generated internally, then it is
          added to SNOW array in function snowcover*/
        snowprec = 0;
    } else { /*no precipitation*/
        RAIN[i][j] = 0;
        snowprec = 0;
    }

    return;
}


/***************************************************************************/
/* FUNCTION   precipenergy                                                 */
/*            CALCULATION OF RAIN ENERGY,  called from main for each grid  */
/***************************************************************************/

void precipenergy()

{
    /*rain in mm/timestep,  rain temp assumed to be air temp
      should be inside iteration loop for surface temperature, neglected
      because error is considered small */

    float  cw=4180;    /*specific heat of water  4.18x1000 J/kg/K*/
    float  Ts=0;         /*temperature surface*/

    /*------------------------------------------------------------------*/
    rainenergy[i][j] = cw * RAIN[i][j] /(3600*timestep) * (tempint[i][j]-Ts);
    /*------------------------------------------------------------------*/
    /* in W/m2 */

    /*  if ((i == rowclim) && (j == colclim))
         fprintf(outcontrol,"%5.2f %5.0f %5.0f %5.1f%5.1f %8.4f\n",
      jd,zeit,tempint[i][j],prec,flprec,rainenergy[i][j]);  */

    return;
}


/***************************************************************************/
/* FUNCTION   energybalance                                                */
/*            CALCULATION OF ENERGY AVAILABLE FOR MELT (ENERGY BALANCE)    */
/***************************************************************************/

void energybalance()

{
    /*ice heat is initialized to zero, thus no problem if not used*/
    /*-------------------------------------------------------------------------------*/
    ENBAL[i][j] = NETRAD[i][j] + SENSIBLE[i][j] + LATENT[i][j] + rainenergy[i][j]
                  - ICEHEAT[i][j];
    /*-------------------------------------------------------------------------------*/
    /* in W/m2 */

    /*surftemp from longwave outgoing measurements and surftemp < 0*/
    /*set energy balance to zero in case surftemp < user specified surf temp (variab.h))*/
    if((methodsurftempglac == 3) && (setmelt2zero == 1) && (surftemp[i][j] < surftempminmelt) && (ENBAL[i][j] >0))
        ENBAL[i][j] = 0;     /*avoid melt when surface temperature is below zero*/

    return;
}


/***************************************************************************/
/* FUNCTION  negenergybalance                                              */
/*     store negative energy balance for next time step                    */
/*      take into account nocturnal cooling of the surface                 */
/*     called from main for every time step and each grid cell             */
/***************************************************************************/

void negenergybalance()

{
    float factornegbal=1.0;   /*ONLY PART OF NEGBAL NEEDS TO BE COMPENSATED*/
    /*0.1 = only 10%, 1= all*/

    /*NEGBAL is initialized to 0 in initial.c*/
    if(ENBAL[i][j] < 0)      /*no energy for melt*/
        NEGBAL[i][j] += ENBAL[i][j]*factornegbal; /*NEGBAL turns negative if ENBAL neg*/
    else {
        if(ENBAL[i][j] > fabs(NEGBAL[i][j])) { /*surface back to zero and some melt*/
            lessmelt = NEGBAL[i][j];
            NEGBAL[i][j] = 0;
        } else {  /*still not enough energy to eliminate cold surface, no melt*/
            NEGBAL[i][j] += ENBAL[i][j];
            lessmelt = NEGBAL[i][j];
        }
    } /*end else energy for melt*/

    return;
}


/***************************************************************************/
/* FUNCTION   waterequivalentmelt                                          */
/*            CONVERT ENERGY TO WATER EQUILAVENT MELT (in mm/time step)    */
/*     quantity to use for discharge calculation                           */
/***************************************************************************/

void waterequivalentmelt()

{
    float   Lf = 334000.0;     /*latent heat of fusion  [J/kg] */

    if(surface[i][j] != 4) {   /*only melt if no rock surface*/
        if(ENBAL[i][j] > 0)
            /*-------------------------------------------------------------------*/
            MELT[i][j]  = (ENBAL[i][j]+lessmelt)/Lf * 60*60*timestep;
        /*-------------------------------------------------------------------*/
        else     /*no energy available for melt*/
            MELT[i][j] = 0;

        /*compute water from condensation, if latent positiv and surface temp is zero*/
        /* removed Aug 2004*/
        /*     if((LATENT[i][j] > 0.0) && (surftemp[i][j] >= 0.0))
                condens = LATENT[i][j]/Lf *3600*timestep;
             MELT[i][j] += condens;    */
        /*add water from condensation to melt for discharge*/

        if(MELT[i][j] < 0)     /*can happen because of lessmelt*/
            MELT[i][j] = 0.;     /*MELT is in mm/timestep*/
    } else   /*rock outside glacier, no melt*/
        MELT[i][j] = 0.;

    return;
}


/***************************************************************************/
/* FUNCTION  waterequivalentabla                                           */
/*           CONVERT ENERGY TO WATER EQUILAVENT ABlation (in mm/time step) */
/*     quantity to compare with mass balance data                          */
/*     ABLA can be negativ = surface raising due to re-sublimation         */
/***************************************************************************/

void waterequivalentabla()

{
    float   Ls = 2849000;      /*latent rate of sublimation  in J/kg*/
    /* L.G.: Lf is unused, can we delete it? */
    //float   Lf = 334000.0;     /*latent heat of fusion  [J/kg] */
    float   meltsublimation;   /*ablation or mass gain by sublimation*/

    if(surface[i][j] != 4) {   /*only melt if no rock surface*/
        meltsublimation = 0;
        if (LATENT[i][j] < 0.)   /* SUBLIMATION INDEPENDENT IF SURFTEMP POS OR NEG*/
            meltsublimation = (LATENT[i][j])/Ls * 60*60*timestep;  /*ABLATION INCREASED*/

        else {    /*CONDENSATION OR RE-SUBLIMATION*/
            if(surftemp[i][j] < 0.)       /*RE-SUBLIMATION -> ABLATION DECREASED*/
                meltsublimation = (LATENT[i][j])/Ls * 60*60*timestep;
        }

        /*meltsublimation is negative in case of sublimation (solid to vapour phase*/
        /*then ablation is increased*/

        /*--------------mm/timestep------------------------------------------*/
        ABLA[i][j] = MELT[i][j] -  meltsublimation;
        /*-------------------------------------------------------------------*/

    } else
        ABLA[i][j] = 0;

    return;
}


/***************************************************************************/
/* FUNCTION  snowcover                                                     */
/*     calculate water equivalent of snow cover after melt or snow fall    */
/*     called from main for every time step and each grid cell             */
/*         only energy balance model                                       */
/***************************************************************************/

void snowcover()

{
    /* SNOW is in cm !!!  ABLA and snowprec in mm !!! */

    if(ABLA[i][j] > 0)     /*ablation*/
        SNOW[i][j] = SNOW[i][j] - (ABLA[i][j]/10) + (snowprec/10);
    else       /*ABLA < 0 => means higher resublimation than melt*/
        /*must not be added to SNOW not to disturb snow albedo generation*/
        /*   SNOW[i][j] = SNOW[i][j] + (snowprec/10); */
        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        if (methodsurftempglac == 4)
            /*SNOW[i][j] = SNOW[i][j] - (MELT[i][j]/10) + (snowprec/10);*/ /*CHR else problem with mass conservation*/
            SNOW[i][j] = SNOW[i][j] - (ABLA[i][j]/10) + (snowprec/10);
        else
            SNOW[i][j] = SNOW[i][j] + (snowprec/10);
    /*============================================================*/

    if(SNOW[i][j] < 0)       /*snow cover has melted away*/
        SNOW[i][j] = 0;

    return;
}


/***************************************************************************/
/* FUNCTION  massbalance                                                   */
/*     calculate mass balance (acc-abl) for winter and summer in cm        */
/*     called from main for every time step and each glacier grid cell     */
/***************************************************************************/

void massbalance()

{
    float massbal;

    /*in case of subdaily timesteps winter/summer starts one time step too
      early, (midnight value)-> not corrected because considered negligible*/

    if(readdatesmassbalyes == 1)    /*to avoid that dates are allocated every time step*/
        /*only needed once every mass balance year, set to 1 at end of massbal year in writeout.c*/
    {
        if((datesfromfileyes == 1) &&  (winterbalyes == 1) && (summerbalyes == 1)) {
            nextyear = year + 1;
            /*variable needed to use it to write bn to file only at end of massbalance year
            if not used, then condition jd=summerjdend is fulfilled twice a year if the end of summer
            is later in the following mass balance year*/
            winterjdbeg = winterjdbegmeas[(int)year];
            summerjdbeg = summerjdbegmeas[(int)nextyear];    /*must be day of next year*/
            winterjdend = summerjdbeg - 1;
            summerjdend = winterjdbegmeas[(int)nextyear]-1;
            readdatesmassbalyes = 0;
        }
    }

    /*works only if model run starts in summer, preferably at winterjdbeg*/
    if (methodsurftempglac == 4) {
        if(jdold == winterjdbeg) {
            itswinter = 1;
            itssummer = 0;
        }
        if(jdold == summerjdbeg) {
            itswinter = 0;
            itssummer = 1;
        }
    } else { /*no subsurface snow model*/
        if(jd == winterjdbeg) {
            itswinter = 1;
            itssummer = 0;
        }
        if(jd == summerjdbeg) {
            itswinter = 0;
            itssummer = 1;
        }
    }

    /* massbal is in cm !!!  ABLA and snowprec in mm !!! */
    /* massbal is grid cell mass balance, will be overriden for next grid cell*/

    if(energymethod == 1)     /*array ablation only exists for energy balance*/
        massbal = snowprec/10 - ABLA[i][j]/10;
    if(degreedaymethod == 1)
        massbal = snowprec/10 - MELT[i][j]/10;
    if (methodsurftempglac == 4)
        massbal += sumrain/10;

    if((itswinter == 1) && (winterbalyes == 1))
        WINTERBAL[i][j] +=  massbal;

    if((itssummer == 1) && (summerbalyes == 1)) {
        SUMMERBAL[i][j] +=  massbal;

        if (methodsurftempglac == 4) {
            if ((SNOW[i][j] >= 0.)  && (SNOWswitch[i][j] == 0.))
                SUMMERBALST[i][j] = -(SNOWinit[i][j] - SNOW[i][j]);
            else
                SUMMERBALST[i][j] += massbal;

            if ((winterbalyes != 1) || (maxmeltstakes == 0))
                if (SNOW[i][j] == 0.) SNOWswitch[i][j] = 1.;

            /*       if ((i == rowclim) && (j == colclim)) fprintf(outcontrol,"%f %f %f %f %f \n",
                     jd2,SUMMERBAL[i][j],SUMMERBALST[i][j],SUMMERBAL[i][j]-SUMMERBALST[i][j],massbal);*/
        }
    }

    /*     if((i==rowclim) && (j==colclim))
            printf("jd=%.1f   massbal = %.2f Winterbal = %.2f SUMMERBAL=%.2f  itswinter=%d\n",jd,massbal,WINTERBAL[i][j],SUMMERBAL[i][j],itswinter);
    */

    /* massbalance is cumulated mass balance over whole period of computation, i.e.
       not necessarily balance year if jdbeg is before start day winterbalance */
    if( ((winterbalyes == 1) && (summerbalyes == 1)) || (maxmeltstakes > 0) ) {
        MASSBALcum[i][j] +=  massbal;


        /*MASSBAL arrays set to zero in writemassbalgrid() after winter/summer end*/
        if (methodsurftempglac == 4) {
            if ((SNOW[i][j] >= 0.)  && (SNOWswitch[i][j] == 0.))
                MASSBALcumstake[i][j] = -(SNOWinit[i][j] - SNOW[i][j]);
            else
                MASSBALcumstake[i][j] += massbal;

            if (SNOW[i][j] == 0.) SNOWswitch[i][j] = 1.;
        }
    }

    return;
}



/***************************************************************************/
/* FUNCTION  iterationstation                                              */
/*     calculate energy balance only for climate station grid and iterate  */
/*     surface temperature or compute it from longwave measurements        */
/*     called from main for every time step in case surftemp iterated      */
/***************************************************************************/

void iterationstation()

{
    int    iternumberstation=0;

    i=rowclim;
    j=colclim;

    /********* GLOBAL RADIATION **********************/
    if(methodglobal==1)     /*no separation into direct and diffus*/
        globradratio();           /*calculation of global radiation*/
    if(methodglobal==2) {   /*separate interpolation of direct and diffuse radiation*/
        interpoldirect();
        interpoldiffus();
        adddirectdiffus();
    }

    /********** PRECIPITATION *****needed before albedo in method 2*********/
    precipinterpol();
    precipenergy();

    /*  printf("  precip %5.2f RAIN %5.2f DIR %5.1f\n",LWin,LONGIN[54][102],DIRECT[54][102]);
      printf("   glob %5.2f ref %5.2f  net %5.2f LWout %5.2f\n",glob,ref,net,LWout); } */

    /********* ALBEDO **********************/
    if(readsnowalbedo==0) { /*no use of albedo measurements*/
        switch(methodsnowalbedo) {
        case 1:
            albedocalcconst();    /*constant albedo for snow/slush/ice*/
            break;
        case 2:
            albedocalc();  /*albedo generated as function of T, snow fall*/
            break;
        case 3:
            albedocalc();  /*as 2 but incl. cloud dependence*/
            break;
        case 4:
            albedocalcdepth();  /*similar to 2 but depending on snowdepth*/
            break;
        case 5:
            albedocalcdepth();  /*as 4 but incl. cloud dependence*/
            break;
        case 6:
            albedosnowpoly();  /*modified version of oerlemans and knap, sicart PhD. p.243*/
            break;
        }  /*end case*/
    }  /*endif*/
    else     /*measured albedo data read from file*/
        albedosnowmeas();   /*use measured daily means of snow albedo*/

    shortwavebalance();    /*SHORTWAVE RADIATION BALANCE*/

    /********* SURFACE TEMPERATURE **********************/

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    if(methodsurftempglac == 4)   /*CHR: LONGIN AND LONGOUT FROM MEASUREMENTS*/
        iterationend = 10;           /*SURFTEMP HAS BEEN CALCULATED BEFORE*/
    /*TO AVOID ITERATION, TEMP IS COMPUTED USING THE MEASUREMENTS, */
    /*NOT THE ITERATION PROCEDURE, AFTER ONE LOOP, THE LOOP SHOULD BE ENDED*/
    else {
        surftemp[i][j] = 0;    /*iteration start = 0 degrees*/
        iterationend = -1;      /*CHR: to get iteration in other cases*/
    }
    /*============================================================*/

    /*before iteration must be reset to 0, strictly speaking, only if lowered via
    iteration, or if surftemp written to output (because array is overwritten for
    output means, at start in initial.c set to 0 for area calculated*/

    airpress();    /*** CALCULATION AIR PRESSURE AT GRID POINT FOR TURB FLUXES ***/
    vappress();    /*** CALCULATION VAPOUR PRESSURE FROM REL. HUMIDITY  ***/

    /**** ======== ITERATION LOOP FOR SURFACE TEMP ICE, SNOW ============================ */
    /**** longwave outgoing, turbulent fluxes and longwave incoming (if acc. Pluess) are
          affected by surface temperature and thus calculated inside iteration loop
          effect on rain energy neglected ***/
    do {
        /************* TURBULENT FLUXES *********************/
        vappressicesnow();       /*saturation vapour pressure of ice, snow surface*/

        switch(methodturbul) {
        case 1:
            sensescher();
            latescher();
            break;
        case 2:
            sensible();     /*no stability correction, Phi=0*/
            latent();
            break;
        case 3:
            sensiblestabilityiteration();
            /*sensible() called from there after computing stability functions*/
            latent();
            break;
        case 4:
            turbfluxes();  /*as 3 but different way: Carleen Tijm-Reijmer, 2/2005*/
            break;
        }

        /********* LONGWAVE RADIATION **********************/
        if(methodlongin == 2)      /*LONGWAVE INCOMING RADIATION VARIABLE IN SPACE*/
            longinpluess();

        /*  printf("   glob %5.2f ref %5.2f  net %5.2f LWout %5.2f\n",glob,ref,net,LWout); } */

        if(methodsurftempglac >= 2)   /*LONGWAVE OUTGOING RADIATION NOT ASSUMED CONSTANT 316.5 (MELTING)*/
            longoutcalc();

        /********* RADIATION BALANCE **********************/
        radiationbalance();

        /********* ICE HEAT FLUX **************************/
        if(methodiceheat == 2)      /*if 1 no heat flux*/
            iceheatStorglac();


        /********* ENERGY BALANCE *************************/
        if (methodsurftempglac == 4)
            ICEHEAT[i][j] = 0.;
        energybalance();

        /*  if((i==rowclim) &&(j==colclim))
            printf(" Ts=%3.1f lin=%5.1f lou=%5.1f NET=%5.1f sen=%4.1f ENB= %4.1f\n",
            surftemp[i][j],LONGIN[i][j],LONGOUT[i][j],NETRAD[i][j],SENSIBLE[i][j],ENBAL[i][j]); */

        if((methodnegbal == 2) && (iternumberstation == 0))
            negenergybalance();      /*STORE NEGATIVE ENERGY BALANCE*/
        /*iternumber must to 0 to ensure that in case of iteration, the first
          negative ENBAL is stored  before it is brought to zero by lowering the surface
          temperature in the iteration procedure (the function is called only once for every
          time step and each grid cell), iternumber is increased when surface temp lowered,
          after iteration loop energy balance can not be negative any more*/

        if (iterationend < 0 ) { /*CHR only if iteration is wanted*/
            if(ENBAL[i][j] < 0) {  /*iteration wanted*/
                if(surface[i][j] != 4)  /*glacier or snow on rock*/
                    surftemp[i][j] -= iterstep;      /*decrease temperature*/
                iternumberstation += 1;          /*count number of iteration step*/
            }  /*endif*/

            iterationend = ENBAL[i][j];  /*trick to determine iterationend, if pos no iteration*/
            if(iternumberstation > 1000) {   /*to avoid endless loops*/
                printf("\n\nTOO MANY ITERATIONS STATION jd=%4.1f %5.1f row %5d col %5d\n\n",jd,zeit,i,j);
                exit(20);
            }
        }  /*endif iterationend*/


        /**** CALCULATE NEW VALUES OF LONGWAVE OUTGOING AT STATION AND LONGWAVE INCOMING ***/
        LWout = LONGOUT[i][j];
        if(methodlonginstation == 1)   /*IF LONGWAVE FROM NET, GLOB, REF MEASUREMENTS*/
            longinstationnetradmeas();   /*LONGWAVE IN MUST BE ADJUSTED AS OUTGOING HAS CHANGES*/
        surftempstationalt = surftemp[i][j];

        if(methodlongin == 2)
            longinskyunobstructed();

        if(surftemp[i][j] < surftempminimum)     /*stop iteration to avoid surface temp too low*/
            iterationend = 10;


    } while(iterationend < 0);
    /**** ======== ITERATION LOOP FOR SURFACE TEMP ICE, SNOW end: next temp ===== */

    /* printf("   LWin %5.2f LONGIN %5.2f Ts %5.1f\n",LWin,LONGIN[54][102],surftemp[54][102]); */

    if(iternumberstation > 0) {
        ENBAL[i][j] = 0;   /*set to 0, so that it is not pos after iteration is over*/
        iternumberstation = 0;     /*set to zero for next grid cell*/
        iterationend = 10;  /*set positive, to avoid iteration if ENBAL pos*/
    }

    /****** WATER EQUIVALENT MELT/ABLATION ******/
    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    /*before only waterequi-functions called*/
    if ((methodsurftempglac != 4) || ((methodsurftempglac ==4 ) && (percolationyes == 0)) ) { /*CHR added*/
        waterequivalentmelt();     /*** WATER EQUIVALENT MELT ***/
        waterequivalentabla();     /*** WATER EQUIVALENT ABLATION ***/
        RUNOFF[i][j] = MELT[i][j] + rainprec;
    } else {
        MELT[i][j] = 0.;
        RUNOFF[i][j] = 0.;
    }

    if (methodsurftempglac == 4) {
        if (inter == 1) {
            MELTsum[i][j] = 0.;
            ABLAsum[i][j] = 0.;
            RUNOFFsum[i][j] = 0.;
            SNOWsum[i][j] = 0.;
        }
        subsurf(); /*chr calculate new surface temperature field*/
        waterequivalentabla();
        /*  if (percolationyes == 1)*/
        {
            ABLAsum[i][j] += ABLA[i][j];
            MELTsum[i][j] += MELT[i][j];
            RUNOFFsum[i][j] += RUNOFF[i][j];
            SNOWsum[i][j] += snowprec;
            sumSNOWprec[i][j] += snowprec;
            sumRAINprec[i][j] += rainprec;
        }
    }
    /*============================================================*/

    if(methodinisnow == 2)
        snowcover();

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    if (methodsurftempglac == 4) {
        changegrid();
        if ((inter == factinter) &&
                ((int)((offsetsubsurfout+zeit)/factsubsurfout) == (offsetsubsurfout+zeit)/factsubsurfout))
            outputsubsurf();
        if ((inter == factinter) && ((int)jd2 == (int)jdsurface[newday]) &&
                ((int)zeit == offsetsubsurfout) && (calcgridyes == 1))
            outputsubsurflines();
    }
    /*============================================================*/

    if( (winterbalyes == 1) || (summerbalyes == 1) || (maxmeltstakes > 0))
        if (griddgmglac[i][j] != nodata)   /*only for glacier, no matter if dgmdrain is larger*/
            massbalance();

    /*============================================================*/

    /*      if ((methodsurftempglac == 4) && (percolationyes == 1) && (inter == factinter)) */
    if ((methodsurftempglac == 4) && (inter == factinter)) {
        MELT[i][j] = MELTsum[i][j];
        ABLA[i][j] = ABLAsum[i][j];
        RUNOFF[i][j] = RUNOFFsum[i][j];
    }

    /********* OUTPUT ****/
    if((do_out_area == 1) && (inter == factinter))  /*CHR added */
        areasum();     /*** SUMMING UP ALL VALUES OVER AREA - for spatial means ***/

    return;
}
