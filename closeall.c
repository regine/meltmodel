/*******************************************************************/
/*  FILE   closeall.c                                              */
/*              includes FUNCTION freematrix                       */
/*     CLOSING FILES, FREE STORAGE                                 */
/*  1.4.1997,  update 18 Nov 2011 */
/*******************************************************************/

#include "function.h"
#include "variabex.h"      /* all global VARIABLES */

/*******************************************************************/
/* FUNCTION freematrix.c*/
/*   gibt reservierten Speicherplatz der Gridfiles frei ******/
/*******************************************************************/

void freematrix(float **m,long nrl, long nrh, long ncl, long nch)

{
    /*Freigeben des Speichers auf die Array-Zeilen*/
    free((char *)(m[nrl]+ncl-1));

    /*Freigeben des Speichers auf das Pointer-Array*/
    free((char *)(m+nrl-1));

    return;
}


/*****************************************************************/
/* FUNCTION freematrixdouble.c*/
/*   gibt reservierten Speicherplatz der Gridfiles frei ******/
/*****************************************************************/

void freematrixdouble(double **m,long nrl, long nrh, long ncl, long nch)

{
    /*Freigeben des Speichers auf die Array-Zeilen*/
    free((char *)(m[nrl]+ncl-1));

    /*Freigeben des Speichers auf das Pointer-Array*/
    free((char *)(m+nrl-1));

    return;
}

/*****************************************************************/



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

    freematrix(griddgm,nrl,nrh,ncl,nch);
    freematrix(griddgmdrain,nrl,nrh,ncl,nch);     /*DRAINAGE BASIN*/
    freematrix(griddgmglac,nrl,nrh,ncl,nch);      /*GLACIATED AREA*/
    freematrix(SLOPE,nrl,nrh,ncl,nch);
    freematrix(ASP,nrl,nrh,ncl,nch);              /*ASPECT OF GRID*/

    freematrix(ALBEDO,nrl,nrh,ncl,nch);
    freematrix(surface,nrl,nrh,ncl,nch);
    freematrix(tempint,nrl,nrh,ncl,nch);          /*AIR TEMPERATURE*/

    if(methodprecipinterpol == 2)  /*precipitation index map*/
        freematrix(precipindexmap,nrl,nrh,ncl,nch);
    if(methodprecipinterpol == 3)  /*precipitation read from files for each time step*/
        freematrix(precipreadgrid,nrl,nrh,ncl,nch);

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    if(methodinisnow == 2)                         /*INITIAL SNOW COVER*/
        /*changed:      freematrix(SNOW,nrl,nrh,ncl,nch);   */
    {
        freematrix(wrtSNOW,nrl,nrh,ncl,nch);           /*INITIAL SNOW COVER, Also for temp index*/
        freematrixdouble(SNOW,nrl,nrh,ncl,nch);
    }
    /*============================================================*/

    if(methodglobal == 2)    /*direct and diffuse radiation split*/
        freematrix(SKYVIEW,nrl,nrh,ncl,nch);

    /*OUTPUT-ARRAYS*/
    if(directfromfile != 1) {
        freematrix(SHAPE,nrl,nrh,ncl,nch);           /*SHADE - SUN*/
        freematrix(SHADE,nrl,nrh,ncl,nch);           /*SHADE - SUN*/
        freematrix(Isenk,nrl,nrh,ncl,nch);
        freematrix(strlsumme,nrl,nrh,ncl,nch);
        freematrix(EXKORR,nrl,nrh,ncl,nch);          /*CORRECTION FACTOR FOR SHORT-WAVE RAD*/
    }
    freematrix(DIRECT,nrl,nrh,ncl,nch);
    if (methodsurftempglac == 4)
        freematrix(DIRECTold,nrl,nrh,ncl,nch);

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    /*CHR   freematrix(MELT,nrl,nrh,ncl,nch);  */
    freematrix(wrtMELT,nrl,nrh,ncl,nch);
    freematrixdouble(MELT,nrl,nrh,ncl,nch);
    if (methodsurftempglac == 4) {
        freematrix(RUNOFF,nrl,nrh,ncl,nch);
    }
    /*============================================================*/

    freematrix(RAIN,nrl,nrh,ncl,nch);            /*RAIN mm/timestep*/

    if(energymethod == 1) {
        freematrix(GLOBAL,nrl,nrh,ncl,nch);
        freematrix(SWBAL,nrl,nrh,ncl,nch);           /*SHORTWAVE RADIATION BALANCE*/
        freematrix(NETRAD,nrl,nrh,ncl,nch);          /*NET RADIATION*/
        freematrix(SENSIBLE,nrl,nrh,ncl,nch);
        freematrix(LATENT,nrl,nrh,ncl,nch);

        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        /*REMOVE     freematrix(ENBAL,nrl,nrh,ncl,nch);    */
        freematrix(wrtENBAL,nrl,nrh,ncl,nch);           /*ENERGY BALANCE*/
        freematrixdouble(ENBAL,nrl,nrh,ncl,nch);           /*ENERGY BALANCE*/
        /*============================================================*/

        freematrix(rainenergy,nrl,nrh,ncl,nch);
        freematrix(ICEHEAT,nrl,nrh,ncl,nch);

        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        /*CHR     freematrix(ABLA,nrl,nrh,ncl,nch);     */       /*MELT PLUS SUBLIMATION mm*/
        {
            freematrix(wrtABLA,nrl,nrh,ncl,nch);               /*MELT PLUS SUBLIMATION mm*/
            freematrixdouble(ABLA,nrl,nrh,ncl,nch);            /*MELT PLUS SUBLIMATION mm*/
        }
        /*============================================================*/

        freematrix(surftemp,nrl,nrh,ncl,nch);


        if(methodglobal == 2) {  /*direct and diffuse radiation separated*/
            freematrix(DIRECT2,nrl,nrh,ncl,nch);
            freematrix(DIFFUS,nrl,nrh,ncl,nch);
        }
        if(methodsnowalbedo >= 2)
            /*snow albedo function of days since snow fall*/
        {
            freematrix(numbdays,nrl,nrh,ncl,nch);
            freematrix(ALBALT,nrl,nrh,ncl,nch);
            freematrix(snowtofirn,nrl,nrh,ncl,nch);
            freematrix(ndbefsnow,nrl,nrh,ncl,nch);
            freematrix(ALBBEFSNOW,nrl,nrh,ncl,nch);
        }

        if(methodlongin == 2)    /*longwave incoming spatially variable*/
            freematrix(LONGIN,nrl,nrh,ncl,nch);
        if(methodsurftempglac >= 2)     /*surface temperature iterated*/
            freematrix(LONGOUT,nrl,nrh,ncl,nch);  /*longwave outcoming spatially variable*/

        if(methodnegbal == 2)    /*negative energy balances*/
            freematrix(NEGBAL,nrl,nrh,ncl,nch);
    }  /*endif energymethod*/

    /*================ new for snow model C. Tijm-Reijmer, 2/2005 ======*/
    if (methodsurftempglac == 4) { /*CHR added*/
        freetensordouble(layerdepth,nrl,nrh,ncl,nch,ndl,ndh);
        freetensordouble(layerthickness,nrl,nrh,ncl,nch,ndl,ndh);
        freetensordouble(layerdensity,nrl,nrh,ncl,nch,ndl,ndh);
        freetensordouble(layermass,nrl,nrh,ncl,nch,ndl,ndh);
        freetensordouble(layertemperature,nrl,nrh,ncl,nch,ndl,ndh);
        freetensordouble(layerrhocp,nrl,nrh,ncl,nch,ndl,ndh);
        freetensordouble(layerwatercont,nrl,nrh,ncl,nch,ndl,ndh);
        freetensordouble(layerrefreeze,nrl,nrh,ncl,nch,ndl,ndh);

        freetensor(layerid,nrl,nrh,ncl,nch,ndl,ndh);
        freematrix(layeramount,nrl,nrh,ncl,nch);
        freematrix(layeramountcold,nrl,nrh,ncl,nch);
        freematrix(superice,nrl,nrh,ncl,nch);
        freematrix(tmpsuperice,nrl,nrh,ncl,nch);
        freematrix(watercontent,nrl,nrh,ncl,nch);
        freematrix(slushdepth,nrl,nrh,ncl,nch);
        freematrix(slushthickness,nrl,nrh,ncl,nch);
        freematrix(coldcontentsnow,nrl,nrh,ncl,nch);
        freematrix(coldcontentice,nrl,nrh,ncl,nch);
        freematrixdouble(surfacewater,nrl,nrh,ncl,nch);

        freearraydouble(conduc,ndl,ndh);
        freearraydouble(conducdtdz,ndl,ndh);

        freematrixdouble(snowlayersum,nrl,nrh,ncl,nch);
        freematrixdouble(MELTsum,nrl,nrh,ncl,nch);
        freematrixdouble(ABLAsum,nrl,nrh,ncl,nch);
        freematrix(SNOWsum,nrl,nrh,ncl,nch);
        freematrix(SNOWinit,nrl,nrh,ncl,nch);
        freematrix(MASSBALcumstake,nrl,nrh,ncl,nch);
        freematrix(SUMMERBALST,nrl,nrh,ncl,nch);
        freematrix(sumSNOWprec,nrl,nrh,ncl,nch);
        freematrix(sumRAINprec,nrl,nrh,ncl,nch);
        freematrix(SNOWswitch,nrl,nrh,ncl,nch);

        freematrix(sumMASS,nrl,nrh,ncl,nch);
        freematrix(capwatercontent,nrl,nrh,ncl,nch);
        freematrix(slwatercontent,nrl,nrh,ncl,nch);

        freematrix(accyear,nrl,nrh,ncl,nch);

        if(runoffyes == 1) {
            freematrixdouble(meanrunoffday,nrl,nrh,ncl,nch);
            freematrixdouble(meanrunoffall,nrl,nrh,ncl,nch);
        }
        if(superyes == 1) {
            freematrixdouble(meansupericeday,nrl,nrh,ncl,nch);
            freematrixdouble(meansupericeall,nrl,nrh,ncl,nch);
        }
        if(wateryes == 1) {
            freematrixdouble(meanwatercontentday,nrl,nrh,ncl,nch);
            freematrixdouble(meanwatercontentall,nrl,nrh,ncl,nch);
        }
        if(surfwateryes == 1) {
            freematrix(wrtsurfacewater,nrl,nrh,ncl,nch);
            freematrixdouble(meansurfacewaterday,nrl,nrh,ncl,nch);
            freematrixdouble(meansurfacewaterall,nrl,nrh,ncl,nch);
        }
        if(slushyes == 1) {
            freematrixdouble(meanslushthicknessday,nrl,nrh,ncl,nch);
            freematrixdouble(meanslushthicknessall,nrl,nrh,ncl,nch);
        }
        if(coldsnowyes == 1) {
            freematrixdouble(meancoldcontentsnowday,nrl,nrh,ncl,nch);
            freematrixdouble(meancoldcontenticeday,nrl,nrh,ncl,nch);
        }
        if(coldtotyes == 1) {
            freematrixdouble(meancoldcontentsnowall,nrl,nrh,ncl,nch);
            freematrixdouble(meancoldcontenticeall,nrl,nrh,ncl,nch);
        }

    }  /*endif snow model run*/
    freematrixdouble(snowlayer,nrl,nrh,ncl,nch);
    freematrixdouble(meltlayer,nrl,nrh,ncl,nch);
    /*===================================================================*/


    if(degreedaymethod == 1) {
        if(posyes == 1)    freematrix(TEMPOS,nrl,nrh,ncl,nch);
        if(ddfyes == 1)    freematrix(DDFCALC,nrl,nrh,ncl,nch);
    }


    if ((do_out == 2) || (do_out == 4)) {         /*Arrays for mean Output*/
        if(shayes == 1)   freematrixdouble(meanSHADEday,nrl,nrh,ncl,nch);
        if(solyes == 1)   freematrixdouble(meanSOLHORday,nrl,nrh,ncl,nch);
        if(diryes == 1)   freematrixdouble(meanDIRECTday,nrl,nrh,ncl,nch);
        if(dir2yes == 1)  freematrixdouble(meanDIRECT2day,nrl,nrh,ncl,nch);
        if(difyes == 1)   freematrixdouble(meanDIFFUSday,nrl,nrh,ncl,nch);
        if(albyes == 1)   freematrixdouble(meanALBEDOday,nrl,nrh,ncl,nch);
        if(gloyes == 1)   freematrixdouble(meanGLOBALday,nrl,nrh,ncl,nch);
        if(swbyes == 1)   freematrixdouble(meanSWBALday,nrl,nrh,ncl,nch);
        if(linyes == 1)   freematrixdouble(meanLONGINday,nrl,nrh,ncl,nch);
        if(loutyes == 1)  freematrixdouble(meanLONGOUTday,nrl,nrh,ncl,nch);
        if(netyes == 1)   freematrixdouble(meanNETRADday,nrl,nrh,ncl,nch);
        if(senyes == 1)   freematrixdouble(meanSENSIBLEday,nrl,nrh,ncl,nch);
        if(latyes == 1)   freematrixdouble(meanLATENTday,nrl,nrh,ncl,nch);
        if(rainyes == 1)   freematrixdouble(meanRAINENday,nrl,nrh,ncl,nch);
        if(balyes == 1)   freematrixdouble(meanENBALday,nrl,nrh,ncl,nch);
        if(melyes == 1)   freematrixdouble(meanMELTday,nrl,nrh,ncl,nch);
        if(ablyes == 1)   freematrixdouble(meanABLAday,nrl,nrh,ncl,nch);

        if((degreedaymethod == 1) && (posyes == 1))
            freematrixdouble(meanTEMPOSday,nrl,nrh,ncl,nch);

    }  /*if*/

    if ((do_out == 3) || (do_out == 4)) {
        if(shayes == 1)   freematrixdouble(meanSHADEall,nrl,nrh,ncl,nch);
        if(solyes == 1)   freematrixdouble(meanSOLHORall,nrl,nrh,ncl,nch);
        if(diryes == 1)   freematrixdouble(meanDIRECTall,nrl,nrh,ncl,nch);
        if(dir2yes == 1)  freematrixdouble(meanDIRECT2all,nrl,nrh,ncl,nch);
        if(difyes == 1)   freematrixdouble(meanDIFFUSall,nrl,nrh,ncl,nch);
        if(albyes == 1)   freematrixdouble(meanALBEDOall,nrl,nrh,ncl,nch);
        if(gloyes == 1)   freematrixdouble(meanGLOBALall,nrl,nrh,ncl,nch);
        if(swbyes == 1)   freematrixdouble(meanSWBALall,nrl,nrh,ncl,nch);
        if(linyes == 1)   freematrixdouble(meanLONGINall,nrl,nrh,ncl,nch);
        if(loutyes == 1)  freematrixdouble(meanLONGOUTall,nrl,nrh,ncl,nch);
        if(netyes == 1)   freematrixdouble(meanNETRADall,nrl,nrh,ncl,nch);
        if(senyes == 1)   freematrixdouble(meanSENSIBLEall,nrl,nrh,ncl,nch);
        if(latyes == 1)   freematrixdouble(meanLATENTall,nrl,nrh,ncl,nch);
        if(rainyes == 1)  freematrixdouble(meanRAINENall,nrl,nrh,ncl,nch);
        if(balyes == 1)   freematrixdouble(meanENBALall,nrl,nrh,ncl,nch);
        if(melyes == 1)   freematrixdouble(meanMELTall,nrl,nrh,ncl,nch);
        if(ablyes == 1)   freematrixdouble(meanABLAall,nrl,nrh,ncl,nch);

        if((degreedaymethod == 1) && (posyes == 1))
            freematrixdouble(meanTEMPOSall,nrl,nrh,ncl,nch);

        if(winterbalyes == 1)   freematrix(WINTERBAL,nrl,nrh,ncl,nch);
        if(summerbalyes == 1)   freematrix(SUMMERBAL,nrl,nrh,ncl,nch);
        if( ((winterbalyes == 1) && (summerbalyes == 1)) || (maxmeltstakes > 0) )
            freematrix(MASSBALcum,nrl,nrh,ncl,nch);

    }  /*if*/

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
        freematrix(FIRN,nrl,nrh,ncl,nch);

    if (disyes >= 1) {   /*discharge calculations*/
        if (disyes == 1) {  /*discharge data available*/
            freematrixdouble(f2,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(sumf0x,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(sumf0x2,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(r2,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(f02,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);

            freematrixdouble(f2ln,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(sumf0xln,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(sumf0x2ln,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(r2ln,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(f02ln,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);

            fclose(qmeasfile);
        }

        if ((disyesopt == 1) || (ddfoptyes == 1)) { /*only optimization run*/
            nrl = 1;
            ncl = 1;     /*number of rows/columns lower limit*/
            freematrixdouble(qfirnopt,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(qsnowopt,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(qiceopt,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(qrockopt,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);
            freematrixdouble(volumeopt,nrl,(long)anzahlopt1,ncl,(long)anzahlopt2);

        } else                /*only simulation run*/
            if(qcalcfile)  fclose(qcalcfile);     /*Outputfile discharge*/

    } /*endif disyes=1*/

    return;

}

