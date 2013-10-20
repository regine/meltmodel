/***********************************************************************
 * Copyright 2005, 2010, 2012 Carleen Tijm-Reijmer, Regine Hock
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
/*  FILE  :  skintemperature.c                                                    */
/*    subsurface model                                                      */
/*    entire file by Carleen Tijm-Reijmer 12/2012                            */
/****************************************************************************/
/*===== FUNCTIONS TO CALCULATE SURFACETEMPERATURE ===*/
/**/

#include "skintemperature.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "initial.h"
#include "turbul.h"
#include "variabex.h"      /* all global VARIABLES */

/****************************************************************************/
/* FUNCTION  surftempfrommodel                                                */
/*   this function calculates surface temperature of glacier by linear      */
/*   extrapolating temperature of upper two layers                          */
/****************************************************************************/

void surftempfrommodel() {
    float  tgrad;
    float  tsurf;

    jd2=jd;
    if ((zeit == 24) && (inter == factinter))
        jd2 = (int)jd + 1;

    /*  printf(" calculate surftemp from model. row  %d  col  %d  \n\n",i,j);*/
    /* tgrad = (layertemperature[i][j][2] - layertemperature[i][j][1]) / (layerthickness[i][j][1]+layerthickness[i][j][2]); */
    tgrad = (layertemperature[i][j][2] - layertemperature[i][j][1]) / (layerdepth[i][j][2] - layerdepth[i][j][1]);


    /* tsurf = layertemperature[i][j][1] - tgrad * layerthickness[i][j][1]; */
    tsurf = layertemperature[i][j][1] - tgrad * layerdepth[i][j][1];

    /* Surface temperature of melting snow cannot exceed 0.0 C or 273.16 K */
    if (tsurf > 0.0) {
        tsurf = 0.0;
    }
    if (tsurf < surftempminimum) {
        fprintf(outcontrol,"0 jd=%.3f i=%d j=%d k 1 temp %.3f grad %.3f \n",jd2,i,j,tsurf,tgrad);
        tsurf = surftempminimum;
        resoutlines += 1;
        if(resoutlines > resoutlinesmax)
            toobig_resout();    /*exit program*/
    }  /*endif*/

    surftemp[i][j] = tsurf;

    return;
}

/****************************************************************************/
/* FUNCTION  surftempskin                                                   */
/*   this function calculates surface temperature of glacier by             */
/*   using a skinlayer formulation                                          */
/*   a skin layer is a surface layer without heat capacity                  */
/*   the skinlayer temp is calculated by writing the surface EB in terms of  */
/*   it and deriving a formultion of Tskin                                  */
/*   longin and longout from previous timestep                              */
/****************************************************************************/

void surftempskin() {
    int tskiniter;
    int tskinitermax;
/*    float tkel = 273.16;   */  /*0 point of water in K*/
    double dtsurf,dtsurf1;
    double tsurf,tsurfold,tsurfold1;

    turbfluxes();

/*    printf("A %f %f  ",jd2,surftemp[i][j]);*/
    tsurf = surftemp[i][j];		/* in deg C*/
    tsurf1 = surftemp[i][j] - tinterv;
    tsurf2 = surftemp[i][j] + tinterv;
    dtsurf = 2.*taccur;
    dtsurf1 = 2.*taccur;
    tsurfold = tsurf1;

    tskiniter = 0;
    tskinitermax = 40;

    while ((dtsurf > taccur) && (dtsurf1 > 0.)) {
        tskiniter = tskiniter+1;
        
        tsurfold1 = tsurfold;
        tsurfold = tsurf;

        bisection();
        tsurf = tbisection;

        if (tsurf >= 0.)
            tsurf = 0.;
        tspechum = tsurf;
        kspechum = 2;
        spechum();

		surftemp[i][j] = tsurf;
        turbfluxes();

//        tsurfenergybalance(tsurf);
        if (tsurf >= 0.)
            tsurfenergybalance(0.);
        sourceskin = balancetsurf;
        if (sourceskin < 0.)
            sourceskin = 0.;

        dtsurf = fabs(tsurf - tsurfold);
        dtsurf1 = fabs(tsurf - tsurfold1);
        
        if ( tskiniter >= tskinitermax) { /* no solution found */
        fprintf(outcontrol," function surftempskin: more than %d iterations necessary %f %f %f %f %f \n",
                tskinitermax,taccur,dtsurf,tsurf,tsurfold,sourceskin);
        tsurf = 0.5 * (tsurf + tsurfold);
        dtsurf = 0.;
        }
        
    }

    if ((dtsurf > taccur) && (dtsurf1 == 0.) && (tskiniter > tskinitermax)) { /* no solution found */
      fprintf(outcontrol," function surftempskin: nosolution found, varies between: %d %f %f %f %f \n",
                tskiniter,taccur,dtsurf,tsurf,tsurfold);
      tsurf = 0.5 * (tsurf + tsurfold);
    }

    surftemp[i][j] = tsurf;

    return;
}

/****************************************************************************/
/* FUNCTION bisection (x1,x2,x_acc)  */
/****************************************************************************/
/* This function determines the surface temperature required to close the energy balance */
/****************************************************************************/

void bisection() {
    int kmax = 40;
    int k;
    double dtsurf,ff,fmid,tmid,rtb;

    tsurfenergybalance(tsurf2);
    fmid = balancetsurf;
    tsurfenergybalance(tsurf1);
    ff = balancetsurf;

    if ((ff*fmid) >= 0.0) {
        printf("\n Error: Root must be bracketed for bisection! day: %f T1: %f T2:%f Bal1: %f Bal2: %f\n",
                jd2,tsurf1,tsurf2,ff,fmid);
        exit(4);
    }

    if ( ff < 0.0) {
        rtb=tsurf1;
        dtsurf=tsurf2-tsurf1;
    } else {
        rtb=tsurf2;
        dtsurf=tsurf1-tsurf2;
    }

    for (k = 1; k <= kmax; k++) {
        dtsurf = dtsurf * 0.5;
        tmid = rtb+dtsurf;
        tsurfenergybalance(tmid);
        fmid = balancetsurf;

        if (fmid <= 0.0)
            rtb = tmid;
        if ((fabs(dtsurf) < taccur) || (fmid == 0.0))
            k = kmax+1;
    }

    if (k == kmax)
        printf("\n Warning: maximum number of bisections!! %f %f %f \n",jd2,tmid,fmid);

    tbisection = rtb;

    return;
}

/*==============FUNCTION THAT WOULD BELONG TO TURBUL.C ====================== */

/***************************************************************************/
/* FUNCTION   tsurfenergybalance                                                    */
/* calculates the energy balance as a function of the surface temperature (K)      */
/***************************************************************************/

void tsurfenergybalance(double tskin) {
    float rd     = 287.05;	  /*gas constant of dry air*/
    float tkel   = 273.16;
    float cp	    = 1005.;	/*specific heat air at constant pressure J K-1 kg-1 */
    float g      = 9.81;		/*gravitational constant*/
    float sigma=0.0000000567; /* Stefan Boltzman constant*/
    float emis = 1.0;   /*Emissivity snow*/
    float Ls     = 2849000.0;   /*latent hate of sublimation  in J/kg*/
    float Lv     = 2514000.0;   /* latent heat of evaporation  [J/kg] */
    float LL;
    double sumdivs;
    float densair;
    double factS,factL,factH,factLE,factG,factGa,factGb,factRain;
    double term1,term2;

    sumdivs = 0.;		/* for later to include radiation penetration*/

    if (tskin < 0.0) LL=Ls;
    if (tskin >= 0.0) LL=Lv;

    airpress();
    densair = p / (rd*(tempint[i][j]+tkel));

    tspechum = tskin;
    kspechum = 2;
    spechum();

    /* net radiation */
    factS = SWBAL[i][j] - sumdivs;
    factL = LONGIN[i][j] -sigma*emis*pow((tskin+tkel),4);
    /* sensible heat flux*/
    factH = Ch*densair*cp*wind*(tempint[i][j] - tskin + z2*g/cp);
    /* latent heat flux */
    factLE = Cq*densair*LL*wind*(q-q0);
    /* energy from rain */
    factRain = rainenergy[i][j]; 
/*    factRain = 0.;	*//* neglect rain energy */

    /* subsurface energy flux */
    if (tsurfextrapolation == 1) {
        factG = -(conduc[1]*(tskin-layertemperature[i][j][1]))/layerthickness[i][j][1];
     /*  factG = -(conduc[1]*(tskin-layertemperature[i][j][1]))/(0.5*layerthickness[i][j][1]); */
    } else if (tsurfextrapolation == 2) {
        factGa = conduc[1]*(tskin-layertemperature[i][j][1])/layerthickness[i][j][1];
        /*  factGa = -(conduc[1]*(tskin-layertemperature[i][j][1]))/(0.5*layerthickness[i][j][1]); */
        term1 = ((conduc[1]*layerthickness[i][j][1]+conduc[2]*layerthickness[i][j][2])/
                 (layerthickness[i][j][1]+layerthickness[i][j][2]));
        term2 = ((layertemperature[i][j][1]-layertemperature[i][j][2])/
                 (0.5*(layerthickness[i][j][1]+layerthickness[i][j][2])));
        factGb = term1*term2;
        factG = -(layerthickness[i][j][1]*(2.*factGa - factGb) + layerthickness[i][j][2]*factGa)/
                (layerthickness[i][j][1]+layerthickness[i][j][2]);
    }

    if (skin_or_inter == 1) factG = 0.;

   balancetsurf = factS + factL + factH + factLE + factG +factRain;

    return;
}

/****************************************************************************/
/* FUNCTION  turbfluxes                                                         */
/*   this function calculates the turbulent fluxes of heat and moisture     */
/****************************************************************************/

void turbfluxes() {
    int   iter;
    int   crit;	/*criterium for stable or unstable conditions*/
    int   itermax = 40;		/* max number of iterations*/
    float Tkel = 273.16;
    float karman = 0.40;			/*Karmans constant*/
    float g      = 9.81;			/*gravitational constant*/
    float cp = 1005.;				/*specific heat air at constant pressure J K-1 kg-1  */
    float theta,tsurf;
    double Lmo,Lmo_old;
    float Lmocrit = 0.01;		/* accuracy of final MO-length (relative) */
    float Phimin, z0tmin, z0emin;		/* lower limits of stability correction and roughness length */
    int nostabil = 1; /*logical yes (1) or no (0) stability correction */
    int nosurfstabil = 1; /*logical yes (1) or no (0) stability correction at surface level*/

    jd2=jd;
    if ((zeit == 24) && (inter == factinter))
        jd2 = (int)jd + 1;

    airpress();
    tspechum = tempint[i][j];
    kspechum = 1;
    spechum();
    tspechum = surftemp[i][j];
    kspechum = 2;
    spechum();

    /*First step, calculate u*, th*, q* under neutral conditions*/
    roughnesslength();
    ustar = (karman * (wind - 0.0 )) / log(z2/z0w);
    frictionveloc = ustar;
    if(method_z0Te == 2)       /*method to compute z0T and z0e*/
        roughnesslengthAndreas();
    if(method_z0Te == 3)       /*method to compute z0T and z0e*/
        roughnesslengthAndreasmodified();

    theta = tempint[i][j] + Tkel + z2*g/cp;
    tsurf = surftemp[i][j]+ Tkel;

    thstar = (karman * (theta - tsurf)) / log(z2/z0T);
    qstar = (karman * (q - q0)) / log(z2/z0e);

    Lmo = 10.E4;
    Lmo_old = 1.E4;
    Phimin = -2.;
    z0tmin=1.0E-10;
    z0emin=1.0E-10;
    iter = 0;
    PhiM = 0.;
    PhiH = 0.;
    PhiE = 0.;
    PhiM0 = 0.;
    PhiH0 = 0.;
    PhiE0 = 0.;

    /*printf("Turbulent fluxes %f \n",jd2);*/

    if (theta > tsurf) crit = 1;		/* stable conditions */
    if (theta <= tsurf) crit = -1;		/* unstable conditions */

    if (nostabil == 1) {
        while ((fabs((Lmo_old-Lmo)/Lmo_old) > Lmocrit) && (iter < itermax)) {
           iter = iter + 1;
/*        printf("B %d \n",iter);*/

            MoninL = Lmo;
            /*Now add stability and iterate */
            if (crit < 0.) {
                unstable(); /* only small change with respect to stabilityunstable()*/
                /*  fprintf(outcontrol,"Unstable %d %f %f %f %f %f %f %f %f %f \n",
                          iter,MoninL,PhiM,PhiH,temp,surftemp[i][j],wind,z0w,z0T,z0e);*/
            }
            if (crit > 0.) {
                stable(); /* only small change with respect to stabilityBeljaar()*/
                /*   fprintf(outcontrol,"Stable %d %f %f %f %f %f %f %f %f %f \n",
                           iter,MoninL,PhiM,PhiH,temp,surftemp[i][j],wind,z0w,z0T,z0e);*/
            }
  
            if (crit == 1) { /* limit stability correction */
              if (Lmo == 0.) iter = iter + itermax;
              if (PhiM < Phimin) PhiM = Phimin;
              if (PhiH < Phimin) PhiH = Phimin;
              if (PhiE < Phimin) PhiE = Phimin;
              if (PhiM0 < Phimin) PhiM0 = Phimin;
              if (PhiH0 < Phimin) PhiH0 = Phimin;
              if (PhiE0 < Phimin) PhiE0 = Phimin;
            }

            /*Recalculate the u*, th* and q* */
            ustar = (karman * (wind - 0.0 )) / ( log(z2/z0w) - PhiM + PhiM0);
            if (nosurfstabil == 0)
                ustar = (karman * (wind - 0.0 )) / ( log(z2/z0w) - PhiM );
            if (crit == 1 && ustar < 0.) ustar = (karman * (wind - 0.0 )) / ( log(z2/z0w));
                
            /*Calculate roughness length for heat and moisture, necessary to calculate th* and q* */
            frictionveloc = ustar;
            if(method_z0Te == 1)       /*method to compute z0T and z0e*/
                roughnesslength();       /*z0T and z0e fixed ratio to z0w*/
            if(method_z0Te == 2)
                roughnesslengthAndreas(); /* z0T,e according to Andreas, 1987, needs friction velocity*/
            if(method_z0Te == 3)       /*method to compute z0T and z0e*/
                roughnesslengthAndreasmodified();
                
            if (z0T < z0tmin) z0T = z0tmin;
            if (z0e < z0emin) z0e = z0emin;

            thstar = (karman * (theta - tsurf)) / ( log(z2/z0T) - PhiH + PhiH0);
            qstar = (karman * (q - q0)) / ( log(z2/z0e) - PhiE + PhiE0);
            if (nosurfstabil == 0) {
                thstar = (karman * (theta - tsurf)) / ( log(z2/z0T) - PhiH );
                qstar = (karman * (q - q0)) / ( log(z2/z0e) - PhiE );
            }

            Lmo_old = Lmo;
            /* Calculate the Monin Obukhov length including stability*/
            Lmo = pow(ustar,2.) / ( (karman*g/(tempint[i][j]+Tkel)) *
                                        (thstar + 0.62 * (tempint[i][j]+Tkel) * qstar ) );
            crit = 1;		/* stable */
            if (Lmo < 0.) crit = -1;			/*Unstable */
        }  /* iteration loop */
    } /*end yes stability correction*/

    MoninL = Lmo;

    if (iter > itermax) {
      Ch = 0.;
      Cq = 0.;
      ustar = 0.; /* results in H and LE being 0 */
      thstar = 0.;
      qstar = 0.;
      PhiM = 0.;
      PhiH = 0.;
      PhiE = 0.;
    }
    else if (Lmo == 0.) {
      Ch = 0.;
      Cq = 0.;
      ustar = 0.; /* results in H and LE being 0 */
      thstar = 0.;
      qstar = 0.;
      PhiM = 0.;
      PhiH = 0.;
      PhiE = 0.;
    }
    else {
      Ch = pow(karman,2)/((log(z2/z0w) - PhiM + PhiM0)*(log(z2/z0T) - PhiH + PhiH0));
      Cq = pow(karman,2)/((log(z2/z0w) - PhiM + PhiM0)*(log(z2/z0e) - PhiE + PhiE0));
    }

    /* Finally calculate the fluxes */
    sensiblenew();
    latentnew();

    if (iter > itermax)
        fprintf(outcontrol," function turbfluxes %.2f %.0f %d temp=%.2f surftemp=%.2f wind=%.2f L=%f  \n",
                jd2,zeit,iter,tempint[i][j],surftemp[i][j],wind,MoninL);
/*   printf(" %f %d %f %f %f %f %f %f \n ",jd2,iter,Ch,Cq,SENSIBLE[i][j],LATENT[i][j],tempint[i][j],surftemp[i][j]);*/

    return;
}

/***************************************************************************/
/* FUNCTION   roughnesslengthAndreasmodified                               */
/* CALCULATION OF ROUGHNESS LENGTHS ACCORDING TO Andreas 1997              */
/*                      surface renewal theory, see Munro 1990             */
/*             modified as by Smeets et al 2006 in prep                    */
/*  function of Reynolds number, then assumed constant for entire glacier  */
/*  function called for climate station grid from function iterationstation */
/***************************************************************************/

void roughnesslengthAndreasmodified() {
    float  Re;     /*Reynoldnumber*/
    float  viscos;
    /*   float  b0T,b1T,b2T, b0e,b1e,b2e;*/

    /* viscos = 0.000015;   kinematic viscosity of air m2/s  for 0 degrees */
    viscos = 0.0000139;     /*kinematic viscosity of air m2/s for 5 degrees*/

    /*change z0 as fixed ratio in case of snow surface*/
    if (surface[i][j] == 1)           /*snow surface*/
        z0w=z0wice/dividerz0snow;         /*z0w can be different for ice and snow*/
    else {                           /*ice*/
        if ((methodsurftempglac == 4) && (snowlayersum[i][j] > 0.))
            z0w=z0wice/dividerz0snow;
        else
            z0w=z0wice;
    }

    Re = frictionveloc*z0w/viscos;

    /*coefficients according to Andreas 1987, Boundary Layer Meteor. 38, 159-184 */
    /*coefficients according to Smeets 2006 */
    if (surface[i][j] != 3) {
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
    } else {
        /*  if (Re <= 0.135)*/ /*Smooth regime*/
        if (Re <= 11.0) { /*Smooth regime*/
            z0T = z0w*exp(1.250);
            z0e = z0w*exp(1.610);
        }
        /*  else if ((Re > 0.135) && (Re < 2.5))*/ /*Transitional regime*/
        /*  else if ((Re > 0.135) && (Re < 162))*/ /*Transitional regime*/
        /* { z0T = z0w*exp(1.250);
             z0e = z0w*exp(1.610);}*/
        /*  else if (Re >= 2.5)*/ /*Rough regime */
        /* else if (Re >= 162)*/ /*Rough regime */
        else if (Re > 11.0) { /*Rough regime */
            z0T = z0w*exp(3.5-0.7*log(Re)-0.1* pow((log(Re)),2) );
            z0e = z0T;
        }
    }

    return;
}

/****************************************************************************/
/* FUNCTION  spechum                                                        */
/*   this function calculates the specific humidity at surface and          */
/*   at observation level. At surface it is the saturation value            */
/****************************************************************************/

void spechum() {
    int kk = kspechum;
    float  Tkel = 273.16;
    float  rd=287.05;         /*gas constant of dry air*/
    float  rv=461.51;         /*gas constant of moist air*/
    float eps = rd/rv;
    float  tk;
    float tklvl = Tkel + 2.;
    float  fact1,fact2a,fact2b,fact3,fact4;
    float  Ls = 2849000.0;   /*latent heat of sublimation  in J/kg*/
    float  Lv = 2514000.0;   /* latent heat of evaporation  [J/kg] */
    /* L.G.: LL is unused, can we delete it? */
    // float  LL;
    float  beta=2317;         /*constant for calculation es J K-1 kg-1*/
    float  es0 = 610.78;      /*water vapour pressure at melting point hPa*/
    float  esat;


    /* saturation vapor pressure */
    /* above 2deg with respect to water, below with respect to ice*/
    /*  for (kk=1; kk <= 2; kk++)
      { if (kk == 1) tk = tempint[i][j] + Tkel;
        if (kk == 2) tk = surftemp[i][j] + Tkel; */
    tk = tspechum + Tkel;

    fact1 = 1./rv;
    fact2a = (Lv + beta * Tkel);
    fact2b = (Ls + beta * Tkel);
    fact3 = ( (1./Tkel) - (1./tk) );
    fact4 = beta * log(tk/Tkel);

    if (tk > tklvl)		/* with respect to water (Lv)*/
        esat = es0 * exp(fact1 * ((fact2a*fact3)-fact4) );
    if (tk <= tklvl)		/* with respect to ice (Ls)*/
        esat = es0 * exp(fact1 * ((fact2b*fact3)-fact4) );

    /* vapor pressure*/
    if (kk == 1) e = esat*hum/100.; /*in Pa*/
    if (kk == 2) e0 = esat; /*in Pa*/

    /* specific humidity */
    if (kk == 1) q = (eps*e) / (p-(1.-eps)*esat);
    if (kk == 2) q0 = (eps*e0) / (p-(1.-eps)*esat);
    /*}*/

    return;
}
/***************************************************************************/
/* FUNCTION   stable                                                       */
/* CALCULATION OF STABILITY FUNCTIONS FOR STABLE CASE                      */
/*   stability function (stable) according to Beljaar and Holtslag, 1991   */
/***************************************************************************/

void stable() {
    float  aa,b,c,d;

    aa=1;
    b=0.667;
    c=5;
    d=0.35;

    PhiM= -( (aa*(z2/MoninL)) + b*((z2/MoninL)-(c/d))*exp(-d*(z2/MoninL)) + (b*c/d) );
    PhiH= -( (pow((1+ (2/3)*aa*(z2/MoninL)),1.5)) + b*((z2/MoninL)-(c/d))*exp(-d*(z2/MoninL)) +
             (b*c/d) -1 );
    PhiE=PhiH;

    PhiM0= -((aa*z0w/MoninL) + b*(z0w/MoninL-c/d)*exp(-d*z0w/MoninL) + b*c/d);
    PhiH0= -((pow((1+2/3*aa*z0T/MoninL),1.5)) + b*(z0T/MoninL-c/d)*exp(-d*z0T/MoninL) + b*c/d-1);
    PhiE0= -((pow((1+2/3*aa*z0e/MoninL),1.5)) + b*(z0e/MoninL-c/d)*exp(-d*z0e/MoninL) + b*c/d-1);

    /*   PhiM = -pow(( 1 + 6.25* z2 / MoninL ),0.8);
       PhiH = -pow(( 1 + 9.375* z2 / MoninL ),0.8);	*/

    return;
}

/***************************************************************************/
/* FUNCTION   unstable                                                     */
/* CALCULATION OF STABILITY FUNCTION FOR UNSTABLE CASE                     */
/*                     according to Panofsky and Dutton, 1984              */
/***************************************************************************/

void unstable() {
    float  xx;

    xx=pow((1-16*z2/MoninL),0.25);
    PhiM=log((1+pow(xx,2))/2 * pow(((1+xx)/2),2)) - 2*atan(xx)+pi/2;
    PhiH=2*log(0.5*(1+ (pow( (1-16*z2/MoninL),0.5) )));
    PhiE=PhiH;

    xx=pow((1-16*z0w/MoninL),0.25);
    PhiM0=log((1+pow(xx,2))/2 * pow(((1+xx)/2),2)) - 2*atan(xx)+pi/2;
    PhiH0=2*log(0.5*(1+ (pow( (1-16*z0T/MoninL),0.5) )));
    PhiE0=2*log(0.5*(1+ (pow( (1-16*z0e/MoninL),0.5) )));

    return;
}

/***************************************************************************/
/* FUNCTION   sensiblenew                                                  */
/* CALCULATION OF SENSIBLE HEAT HEAT                                       */
/***************************************************************************/

void sensiblenew() {
    float rd     = 287.05;	  /*gas constant of dry air*/
    float densair;
    float Tkel = 273.16;
    float cp = 1005.;				/*specific heat air at constant pressure J K-1 kg-1 */

    airpress();
    densair = p / (rd*(tempint[i][j]+Tkel));

    SENSIBLE[i][j] = densair*cp*thstar*ustar;

    return;
}

/***************************************************************************/
/* FUNCTION   latentnew                                                    */
/* CALCULATION OF LATENT HEAT HEAT                                         */
/***************************************************************************/

void latentnew() {
    float Ls     = 2849000.0;   /*latent hate of sublimation  in J/kg*/
    float Lv     = 2514000.0;   /* latent heat of evaporation  [J/kg] */
    float LL;
    float rd     = 287.05;	  /*gas constant of dry air*/
    float densair;
    float Tkel = 273.16;

    if (surftemp[i][j] < 0.0) LL=Ls;
    if (surftemp[i][j] >= 0.0) LL=Lv;

    airpress();
    densair = p / (rd*(tempint[i][j]+Tkel));

    LATENT[i][j] = densair*LL*qstar*ustar;

    return;
}

/***************************************************************************/
