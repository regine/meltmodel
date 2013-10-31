/**********************************************************************
 * Copyright 1995-2012 Regine Hock, Carleen Tijm-Reijmer
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
/* last update 13 June 2013*/
 
/*******************************************************************/
/*  FILE   closeall.c                                              */
/*                                                                 */
/*     CLOSING FILES, FREE STORAGE                                 */
/*******************************************************************/

#include "closeall.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "initial.h"
#include "grid.h"

#include "variabex.h"      /* all global VARIABLES */


void closeall()

{

    /*FILES*/

    fclose(inklima);
    inklima = NULL;
    fclose(outcontrol);
    outcontrol = NULL;
    if(outmeltstakes) {
        fclose(outmeltstakes);
        outmeltstakes = NULL;
    }
    if(outmassbalstakes) {
        fclose(outmassbalstakes);
        outmassbalstakes = NULL;
        if (methodsurftempglac == 4) {
            fclose(outmassbalstakes2);
            outmassbalstakes2 = NULL;
        }
    }
    if(outspecificmassbal) {
        fclose(outspecificmassbal);
        outspecificmassbal = NULL;
    }



    if(ddfoptyes == 0) {         /*no melt parameter optimation*/ {
            if(snowfreeyes == 1)   closefile(&outsnowfree,namesnowfree);
            if(do_out_area == 1) {     /*output requested by user*/
                fclose(outarea);
                outarea = NULL;
            }
        }
        if (outgridnumber > 0) {
            for (i=1; i<=outgridnumber; i++) {
                fclose(outgrid[i]);
                outgrid[i] = NULL;
            }
        } /*if*/
    }


    /*INPUT-ARRAYS*/

    freematrix(griddgm,1,nrows,1,ncols);
    freematrix(griddgmdrain,1,nrows,1,ncols);     /*DRAINAGE BASIN*/
    freematrix(griddgmglac,1,nrows,1,ncols);      /*GLACIATED AREA*/
    freematrix(SLOPE,1,nrows,1,ncols);
    freematrix(ASP,1,nrows,1,ncols);              /*ASPECT OF GRID*/

    freematrix(ALBEDO,1,nrows,1,ncols);
    freematrix(surface,1,nrows,1,ncols);
    freematrix(tempint,1,nrows,1,ncols);          /*AIR TEMPERATURE*/

    if(methodprecipinterpol == 2)  /*precipitation index map*/
        freematrix(precipindexmap,1,nrows,1,ncols);
    if(methodprecipinterpol == 3)  /*precipitation read from files for each time step*/
        freematrix(precipreadgrid,1,nrows,1,ncols);

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    if(methodinisnow == 2)                         /*INITIAL SNOW COVER*/
        /*changed:      freematrix(SNOW,1,nrows,1,ncols);   */
    {
        freematrix(wrtSNOW,1,nrows,1,ncols);           /*INITIAL SNOW COVER, Also for temp index*/
        freematrixdouble(SNOW,1,nrows,1,ncols);
    }
    /*============================================================*/

    if(methodglobal == 2)    /*direct and diffuse radiation split*/
        freematrix(SKYVIEW,1,nrows,1,ncols);

    /*OUTPUT-ARRAYS*/
    if(directfromfile != 1) {
        freematrix(SHAPE,1,nrows,1,ncols);           /*SHADE - SUN*/
        freematrix(SHADE,1,nrows,1,ncols);           /*SHADE - SUN*/
        freematrix(Isenk,1,nrows,1,ncols);
        freematrix(strlsumme,1,nrows,1,ncols);
        freematrix(EXKORR,1,nrows,1,ncols);          /*CORRECTION FACTOR FOR SHORT-WAVE RAD*/
    }
    freematrix(DIRECT,1,nrows,1,ncols);
    if (methodsurftempglac == 4)
        freematrix(DIRECTold,1,nrows,1,ncols);

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    /*CHR   freematrix(MELT,1,nrows,1,ncols);  */
    freematrix(wrtMELT,1,nrows,1,ncols);
    freematrixdouble(MELT,1,nrows,1,ncols);
    if (methodsurftempglac == 4) {
        freematrix(RUNOFF,1,nrows,1,ncols);
    }
    /*============================================================*/

    freematrix(RAIN,1,nrows,1,ncols);            /*RAIN mm/timestep*/

    if(energymethod == 1) {
        freematrix(GLOBAL,1,nrows,1,ncols);
        freematrix(SWBAL,1,nrows,1,ncols);           /*SHORTWAVE RADIATION BALANCE*/
        freematrix(LONGIN,1,nrows,1,ncols);
        freematrix(NETRAD,1,nrows,1,ncols);          /*NET RADIATION*/
        freematrix(SENSIBLE,1,nrows,1,ncols);
        freematrix(LATENT,1,nrows,1,ncols);

        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        /*REMOVE     freematrix(ENBAL,1,nrows,1,ncols);    */
        freematrix(wrtENBAL,1,nrows,1,ncols);           /*ENERGY BALANCE*/
        freematrixdouble(ENBAL,1,nrows,1,ncols);           /*ENERGY BALANCE*/
        /*============================================================*/

        freematrix(rainenergy,1,nrows,1,ncols);
        freematrix(ICEHEAT,1,nrows,1,ncols);

        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        /*CHR     freematrix(ABLA,1,nrows,1,ncols);     */       /*MELT PLUS SUBLIMATION mm*/
        {
            freematrix(wrtABLA,1,nrows,1,ncols);               /*MELT PLUS SUBLIMATION mm*/
            freematrixdouble(ABLA,1,nrows,1,ncols);            /*MELT PLUS SUBLIMATION mm*/
        }
        /*============================================================*/

        freematrix(surftemp,1,nrows,1,ncols);


        if(methodglobal == 2) {  /*direct and diffuse radiation separated*/
            freematrix(DIRECT2,1,nrows,1,ncols);
            freematrix(DIFFUS,1,nrows,1,ncols);
        }
        if(methodsnowalbedo >= 2)
            /*snow albedo function of days since snow fall*/
        {
            freematrix(numbdays,1,nrows,1,ncols);
            freematrix(ALBALT,1,nrows,1,ncols);
            freematrix(snowtofirn,1,nrows,1,ncols);
            freematrix(ndbefsnow,1,nrows,1,ncols);
            freematrix(ALBBEFSNOW,1,nrows,1,ncols);
        }

        if(methodsurftempglac >= 2)     /*surface temperature iterated*/
            freematrix(LONGOUT,1,nrows,1,ncols);  /*longwave outcoming spatially variable*/

        if(methodnegbal == 2)    /*negative energy balances*/
            freematrix(NEGBAL,1,nrows,1,ncols);
    }  /*endif energymethod*/

    /*================ new for snow model C. Tijm-Reijmer, 2/2005 ======*/
    if (methodsurftempglac == 4) { /*CHR added*/
        freetensordouble(layerdepth,1,nrows,1,ncols,1,ndepths);
        freetensordouble(layerthickness,1,nrows,1,ncols,1,ndepths);
        freetensordouble(layerdensity,1,nrows,1,ncols,1,ndepths);
        freetensordouble(layermass,1,nrows,1,ncols,1,ndepths);
        freetensordouble(layertemperature,1,nrows,1,ncols,1,ndepths);
        freetensordouble(layerrhocp,1,nrows,1,ncols,1,ndepths);
        freetensordouble(layerwatercont,1,nrows,1,ncols,1,ndepths);
        freetensordouble(layerrefreeze,1,nrows,1,ncols,1,ndepths);

        freetensor(layerid,1,nrows,1,ncols,1,ndepths);
        freematrix(layeramount,1,nrows,1,ncols);
        freematrix(layeramountcold,1,nrows,1,ncols);
        freematrix(superice,1,nrows,1,ncols);
        freematrix(tmpsuperice,1,nrows,1,ncols);
        freematrix(watercontent,1,nrows,1,ncols);
        freematrix(slushdepth,1,nrows,1,ncols);
        freematrix(slushthickness,1,nrows,1,ncols);
        freematrix(coldcontentsnow,1,nrows,1,ncols);
        freematrix(coldcontentice,1,nrows,1,ncols);
        freematrixdouble(surfacewater,1,nrows,1,ncols);

        freearraydouble(conduc,1,ndepths);
        freearraydouble(conducdtdz,1,ndepths);
        freearraydouble(layerenergy,1,ndepths);

        freematrixdouble(snowlayersum,1,nrows,1,ncols);
        freematrixdouble(MELTsum,1,nrows,1,ncols);
        freematrixdouble(ABLAsum,1,nrows,1,ncols);
        freematrix(SNOWsum,1,nrows,1,ncols);
        freematrix(SNOWinit,1,nrows,1,ncols);
        freematrix(MASSBALcumstake,1,nrows,1,ncols);
        freematrix(SUMMERBALST,1,nrows,1,ncols);
        freematrix(sumSNOWprec,1,nrows,1,ncols);
        freematrix(sumRAINprec,1,nrows,1,ncols);
        freematrix(SNOWswitch,1,nrows,1,ncols);

        freematrix(sumMASS,1,nrows,1,ncols);
        freematrix(capwatercontent,1,nrows,1,ncols);
        freematrix(slwatercontent,1,nrows,1,ncols);

        freematrix(accyear,1,nrows,1,ncols);

        if(runoffyes == 1) {
            freematrixdouble(meanrunoffday,1,nrows,1,ncols);
            freematrixdouble(meanrunoffall,1,nrows,1,ncols);
        }
        if(superyes == 1) {
            freematrixdouble(meansupericeday,1,nrows,1,ncols);
            freematrixdouble(meansupericeall,1,nrows,1,ncols);
        }
        if(wateryes == 1) {
            freematrixdouble(meanwatercontentday,1,nrows,1,ncols);
            freematrixdouble(meanwatercontentall,1,nrows,1,ncols);
        }
        if(surfwateryes == 1) {
            freematrix(wrtsurfacewater,1,nrows,1,ncols);
            freematrixdouble(meansurfacewaterday,1,nrows,1,ncols);
            freematrixdouble(meansurfacewaterall,1,nrows,1,ncols);
        }
        if(slushyes == 1) {
            freematrixdouble(meanslushthicknessday,1,nrows,1,ncols);
            freematrixdouble(meanslushthicknessall,1,nrows,1,ncols);
        }
        if(coldsnowyes == 1) {
            freematrixdouble(meancoldcontentsnowday,1,nrows,1,ncols);
            freematrixdouble(meancoldcontenticeday,1,nrows,1,ncols);
        }
        if(coldtotyes == 1) {
            freematrixdouble(meancoldcontentsnowall,1,nrows,1,ncols);
            freematrixdouble(meancoldcontenticeall,1,nrows,1,ncols);
        }

    }  /*endif snow model run*/
    freematrixdouble(snowlayer,1,nrows,1,ncols);
    freematrixdouble(meltlayer,1,nrows,1,ncols);
    /*===================================================================*/


    if(degreedaymethod == 1) {
        if(posyes == 1)    freematrix(TEMPOS,1,nrows,1,ncols);
        if(ddfyes == 1)    freematrix(DDFCALC,1,nrows,1,ncols);
    }


    if ((do_out == 2) || (do_out == 4)) {         /*Arrays for mean Output*/
        if(shayes == 1)   freematrixdouble(meanSHADEday,1,nrows,1,ncols);
        if(solyes == 1)   freematrixdouble(meanSOLHORday,1,nrows,1,ncols);
        if(diryes == 1)   freematrixdouble(meanDIRECTday,1,nrows,1,ncols);
        if(dir2yes == 1)  freematrixdouble(meanDIRECT2day,1,nrows,1,ncols);
        if(difyes == 1)   freematrixdouble(meanDIFFUSday,1,nrows,1,ncols);
        if(albyes == 1)   freematrixdouble(meanALBEDOday,1,nrows,1,ncols);
        if(gloyes == 1)   freematrixdouble(meanGLOBALday,1,nrows,1,ncols);
        if(swbyes == 1)   freematrixdouble(meanSWBALday,1,nrows,1,ncols);
        if(linyes == 1)   freematrixdouble(meanLONGINday,1,nrows,1,ncols);
        if(loutyes == 1)  freematrixdouble(meanLONGOUTday,1,nrows,1,ncols);
        if(netyes == 1)   freematrixdouble(meanNETRADday,1,nrows,1,ncols);
        if(senyes == 1)   freematrixdouble(meanSENSIBLEday,1,nrows,1,ncols);
        if(latyes == 1)   freematrixdouble(meanLATENTday,1,nrows,1,ncols);
        if(rainyes == 1)   freematrixdouble(meanRAINENday,1,nrows,1,ncols);
        if(balyes == 1)   freematrixdouble(meanENBALday,1,nrows,1,ncols);
        if(melyes == 1)   freematrixdouble(meanMELTday,1,nrows,1,ncols);
        if(ablyes == 1)   freematrixdouble(meanABLAday,1,nrows,1,ncols);

        if((degreedaymethod == 1) && (posyes == 1))
            freematrixdouble(meanTEMPOSday,1,nrows,1,ncols);

    }  /*if*/

    if ((do_out == 3) || (do_out == 4)) {
        if(shayes == 1)   freematrixdouble(meanSHADEall,1,nrows,1,ncols);
        if(solyes == 1)   freematrixdouble(meanSOLHORall,1,nrows,1,ncols);
        if(diryes == 1)   freematrixdouble(meanDIRECTall,1,nrows,1,ncols);
        if(dir2yes == 1)  freematrixdouble(meanDIRECT2all,1,nrows,1,ncols);
        if(difyes == 1)   freematrixdouble(meanDIFFUSall,1,nrows,1,ncols);
        if(albyes == 1)   freematrixdouble(meanALBEDOall,1,nrows,1,ncols);
        if(gloyes == 1)   freematrixdouble(meanGLOBALall,1,nrows,1,ncols);
        if(swbyes == 1)   freematrixdouble(meanSWBALall,1,nrows,1,ncols);
        if(linyes == 1)   freematrixdouble(meanLONGINall,1,nrows,1,ncols);
        if(loutyes == 1)  freematrixdouble(meanLONGOUTall,1,nrows,1,ncols);
        if(netyes == 1)   freematrixdouble(meanNETRADall,1,nrows,1,ncols);
        if(senyes == 1)   freematrixdouble(meanSENSIBLEall,1,nrows,1,ncols);
        if(latyes == 1)   freematrixdouble(meanLATENTall,1,nrows,1,ncols);
        if(rainyes == 1)  freematrixdouble(meanRAINENall,1,nrows,1,ncols);
        if(balyes == 1)   freematrixdouble(meanENBALall,1,nrows,1,ncols);
        if(melyes == 1)   freematrixdouble(meanMELTall,1,nrows,1,ncols);
        if(ablyes == 1)   freematrixdouble(meanABLAall,1,nrows,1,ncols);

        if((degreedaymethod == 1) && (posyes == 1))
            freematrixdouble(meanTEMPOSall,1,nrows,1,ncols);

        if(winterbalyes == 1)   freematrix(WINTERBAL,1,nrows,1,ncols);
        if(summerbalyes == 1)   freematrix(SUMMERBAL,1,nrows,1,ncols);

    }  /*if*/

        freematrix(MASSBALcum,1,nrows,1,ncols);
        freematrix(MASSBALcum_all,1,nrows,1,ncols);
        freematrix(MASSBALgrid,1,nrows,1,ncols);
        
    free(namedgm);
    free(namedgmdrain);
    free(namedgmglac);
    free(namedgmslope);
    free(namedgmaspect);
    free(namedgmskyview);
    free(nameinitialsnow);
    free(nameklima);
    free(namedatesmassbal);

    free(firstcol);
    free(lastcol);

    free(x);   /*first 12 rows in grid files*/

    if((methodinisnow == 2) || (disyes >= 1) || (methodsurftempglac == 4))
        freematrix(FIRN,1,nrows,1,ncols);

    if (disyes >= 1) {   /*discharge calculations*/
        if (disyes == 1) {  /*discharge data available*/
            freematrixdouble(f2,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(sumf0x,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(sumf0x2,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(r2,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(f02,1,anzahlopt1,1,anzahlopt2);

            freematrixdouble(f2ln,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(sumf0xln,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(sumf0x2ln,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(r2ln,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(f02ln,1,anzahlopt1,1,anzahlopt2);

            fclose(qmeasfile);
        }

        if ((disyesopt == 1) || (ddfoptyes == 1)) { /*only optimization run*/
            freematrixdouble(qfirnopt,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(qsnowopt,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(qiceopt,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(qrockopt,1,anzahlopt1,1,anzahlopt2);
            freematrixdouble(volumeopt,1,anzahlopt1,1,anzahlopt2);

        } else                /*only simulation run*/
            if(qcalcfile)  fclose(qcalcfile);     /*Outputfile discharge*/

    } /*endif disyes=1*/

    return;

}

