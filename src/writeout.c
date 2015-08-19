/***********************************************************************
 * Copyright 1996-2012 Regine Hock
 * This file is part of DEBAM and DETIM.
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
/**************************************************************************/
/*  FILE  writeout.c                                                      */
/*        WRITE OUTPUT TO OUTPUT-FILES                                    */
/*  Last update 5 Feb 2015 */
/**************************************************************************/

#include "writeout.h"

#include<stdlib.h>
#include<string.h>
#include<math.h>

/*include all .h files that have functions that are called in this file*/
#include "initial.h"
#include "grid.h"
#include "snowinput.h"
#include "turbul.h"

#include "variabex.h"      /* all global VARIABLES */

/**************************************************************************/
/*  FUNCTION  openoutfile                                                 */
/*            OPEN OUTPUT-FILES                                           */
/*            called from startwritehour, startwriteday and startwriteall */
/*            and many other functions                                    */
/**************************************************************************/

void openoutfile(FILE **datei,char *outpath,char *name)

/*The parameter list can not be empty here, because if it was a separate
  function would be needed for each file to be opened*/

{
    strcpy(dummy,outpath);
    strcat(dummy,name);             /*ADD PATH TO OUTPUT-NAME*/

    if ((*datei = fopen(dummy,"wb")) == NULL) {
        printf("\n ERROR opening output-file: %s\n",name);
        exit(20);
    }

    return;
}


/**************************************************************************/
/*  FUNCTION  openoutfileascii                                            */
/*            OPEN OUTPUT-FILES                                           */
/*            called from many functions                                  */
/**************************************************************************/

void openoutfileascii(FILE **datei,char *outpath,char *name)

/*The parameter list can not be empty here, because if it was a separate
  function would be needed for each file to be opened*/

{
    strcpy(dummy,outpath);
    strcat(dummy,name);             /*ADD PATH TO OUTPUT-NAME*/

    if ((*datei = fopen(dummy,"wt")) == NULL) {
        printf("\n ERROR opening output-file: %s\n",name);
        exit(20);
    }

    return;
}



/**************************************************************************/
/* FUNCTION  startwritehour()                                             */
/*     OUTPUT-FILE FOR EVERY TIME STEP                                    */
/*     CREATE GRID OUTPUT-FILENAME e.g. : sha+JD+time e.g. sha19012.bin   */
/*     OPEN FILE and WRITE FIRST 6 ROWS                                   */
/*     called from main every time step after grid loop                   */
/**************************************************************************/

void startwritehour()

{
    if (directfromfile !=1)   /*only written to file, if it doesn't exist yet*/

        /*if directfromfile=1, shading, correction factor, direct
          radiation uncorrected and corrected are not calculated in the program;
          therefore data can not be written to file. Corrected direct
          radiation is read from existing files prior created by shading.c */

    {
        if(shayes == 1) {
            sprintf(nameshade,"sha%03d%02d.bin",(int)jd,(int)zeit);
            openoutfile(&outshade,outpath,nameshade);
        }
        if(exkyes == 1) {
            sprintf(nameexkorr,"exk%03d%02d.bin",(int)jd,(int)zeit);
            openoutfile(&outexkorr,outpath,nameexkorr);
        }
        if(solyes == 1) {
            sprintf(namesolhor,"sol%03d%02d.bin",(int)jd,(int)zeit);
            openoutfile(&outsolhor,outpath,namesolhor);
        }
        if(diryes == 1) {
            sprintf(namedirect,"dir%03d%02d.bin",(int)jd,(int)zeit);
            openoutfile(&outdirect,outpath,namedirect);
        }
    }

    if(dir2yes == 1) {
        sprintf(namedirect2,"dis%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outdirect2,outpath,namedirect2);
    }
    if(difyes == 1) {
        sprintf(namediffus,"dif%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outdiffus,outpath,namediffus);
    }
    if(albyes == 1) {
        sprintf(namealbedoout,"alb%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outalbedo,outpath,namealbedoout);
    }
    if(gloyes == 1) {
        sprintf(nameglobal,"glo%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outglobal,outpath,nameglobal);
    }

    if(swbyes == 1) {
        sprintf(nameswbal,"swb%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outswbal,outpath,nameswbal);
    }
    if(linyes == 1) {
        sprintf(namelongin,"lin%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outlongin,outpath,namelongin);
    }
    if(loutyes == 1) {
        sprintf(namelongout,"lou%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outlongout,outpath,namelongout);
    }
    if(netyes == 1) {
        sprintf(namenetrad,"net%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outnetrad,outpath,namenetrad);
    }
    if(senyes == 1) {
        sprintf(namesensible,"sen%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outsensible,outpath,namesensible);
    }
    if(latyes == 1) {
        sprintf(namelatent,"lat%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outlatent,outpath,namelatent);
    }
    if(rainyes == 1) {
        sprintf(namerainen,"ren%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outrainen,outpath,namerainen);
    }
    if(balyes == 1) {
        sprintf(nameenbal,"bal%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outenbal,outpath,nameenbal);
    }

    if(melyes == 1) {
        sprintf(namemelt,"mel%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outmelt,outpath,namemelt);
    }
    if(ablyes == 1) {
        sprintf(nameabla,"abl%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outabla,outpath,nameabla);
    }

    if(surftempyes == 1) {
        sprintf(namesurftemp,"sut%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outsurftemp,outpath,namesurftemp);
    }

    /*snow model output ****************************************************/
    if(runoffyes == 1) {
        sprintf(namerunoff,"run%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outrunoff,outpath,namerunoff);
    }
    if(superyes == 1) {
        sprintf(namesuper,"sup%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outsuper,outpath,namesuper);
    }
    if(wateryes == 1) {
        sprintf(namewater,"wat%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outwater,outpath,namewater);
    }
    if(surfwateryes == 1) {
        sprintf(namesurfwater,"swt%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outsurfwater,outpath,namesurfwater);
    }
    if(slushyes == 1) {
        sprintf(nameslush,"slu%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outslush,outpath,nameslush);
    }
    if(coldsnowyes == 1) {
        sprintf(namecoldsnow,"cos%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outcoldsnow,outpath,namecoldsnow);
    }
    if(coldtotyes == 1) {
        sprintf(namecoldtot,"cot%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outcoldtot,outpath,namecoldtot);
    }
    if(precipyes == 1) {
        sprintf(namesnprecip,"sno%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outsnprecip,outpath,namesnprecip);
        sprintf(nameraprecip,"rai%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outraprecip,outpath,nameraprecip);
    }
    if(intaccumyes == 1) {
        sprintf(namemass,"mas%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outmass,outpath,namemass);
        sprintf(namecapwat,"wac%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outcapwat,outpath,namecapwat);
        sprintf(nameslwat,"was%03d%02d.bin",(int)jd,(int)zeit);
        openoutfile(&outslwat,outpath,nameslwat);
    }
    /**************************************************************************/

    /*only if degree day method; melt and surface can also be written
      to file for this method, this is controlled by melyes and surfyes = 1*/
    if(degreedaymethod == 1) {
        if(posyes == 1) {
            sprintf(namepos,"pos%03d%02d.bin",(int)jd,(int)zeit);
            openoutfile(&outpos,outpath,namepos);       /*pos temperatures*/
        }
        if(ddfyes == 1) {
            sprintf(nameddf,"ddf%03d%02d.bin",(int)jd,(int)zeit);
            openoutfile(&outddf,outpath,nameddf);
        }
    }

    return;
}


/****************************************************************************/
/* FUNCTION  startwriteday()                                                */
/*    OUTPUT-FILE FOR DAILY MEAN GRIDS                                      */
/*    CREATE OUTPUT-FILENAME e.g. : sha+julianday  e.g. sha190.bin AND OPEN */
/****************************************************************************/

void startwriteday()

{
    if (directfromfile !=1) {
        if(shayes == 1) {
            sprintf(nameshade,"sha%03d.bin",(int)jd);      /*CREATE FILENAME*/
            openoutfile(&outshade,outpath,nameshade);
        }             /*OPEN FILE*/
        if(exkyes == 1) {
            sprintf(nameexkorr,"exk%03d.bin",(int)jd);
            openoutfile(&outexkorr,outpath,nameexkorr);
        }
        if(solyes == 1) {
            sprintf(namesolhor,"sol%03d.bin",(int)jd);
            openoutfile(&outsolhor,outpath,namesolhor);
        }
    }

    /*direct radiation for every time step is read from files,
      but daily and period means can be calculated and written to file*/

    if(diryes == 1) {
        sprintf(namedirect,"dir%03d.bin",(int)jd);
        openoutfile(&outdirect,outpath,namedirect);
    }
    if(dir2yes == 1) {
        sprintf(namedirect2,"dis%03d.bin",(int)jd);
        openoutfile(&outdirect2,outpath,namedirect2);
    }
    if(difyes == 1) {
        sprintf(namediffus,"dif%03d.bin",(int)jd);
        openoutfile(&outdiffus,outpath,namediffus);
    }
    if(albyes == 1) {
        sprintf(namealbedoout,"alb%03d.bin",(int)jd);
        openoutfile(&outalbedo,outpath,namealbedoout);
    }
    if(gloyes == 1) {
        sprintf(nameglobal,"glo%03d.bin",(int)jd);
        openoutfile(&outglobal,outpath,nameglobal);
    }
    if(swbyes == 1) {
        sprintf(nameswbal,"swb%03d.bin",(int)jd);
        openoutfile(&outswbal,outpath,nameswbal);
    }
    if(linyes == 1) {
        sprintf(namelongin,"lin%03d.bin",(int)jd);
        openoutfile(&outlongin,outpath,namelongin);
    }
    if(loutyes == 1) {
        sprintf(namelongout,"lou%03d.bin",(int)jd);
        openoutfile(&outlongout,outpath,namelongout);
    }
    if(netyes == 1) {
        sprintf(namenetrad,"net%03d.bin",(int)jd);
        openoutfile(&outnetrad,outpath,namenetrad);
    }
    if(senyes == 1) {
        sprintf(namesensible,"sen%03d.bin",(int)jd);
        openoutfile(&outsensible,outpath,namesensible);
    }
    if(latyes == 1) {
        sprintf(namelatent,"lat%03d.bin",(int)jd);
        openoutfile(&outlatent,outpath,namelatent);
    }
    if(rainyes == 1) {
        sprintf(namerainen,"ren%03d.bin",(int)jd);
        openoutfile(&outrainen,outpath,namerainen);
    }
    if(balyes == 1) {
        sprintf(nameenbal,"bal%03d.bin",(int)jd);
        openoutfile(&outenbal,outpath,nameenbal);
    }
    if(melyes == 1) {
        sprintf(namemelt,"mel%03d.bin",(int)jd);
        openoutfile(&outmelt,outpath,namemelt);
    }
    if(ablyes == 1) {
        sprintf(nameabla,"abl%03d.bin",(int)jd);
        openoutfile(&outabla,outpath,nameabla);
    }
    if(surftempyes == 1) {
        sprintf(namesurftemp,"sut%03d.bin",(int)jd);
        openoutfile(&outsurftemp,outpath,namesurftemp);
    }

    /*snow model output ****************************************************/
    if(runoffyes == 1) {
        sprintf(namerunoff,"run%03d.bin",(int)jd);
        openoutfile(&outrunoff,outpath,namerunoff);
    }
    if(superyes == 1) {
        sprintf(namesuper,"sup%03d.bin",(int)jd);
        openoutfile(&outsuper,outpath,namesuper);
    }
    if(wateryes == 1) {
        sprintf(namewater,"wat%03d.bin",(int)jd);
        openoutfile(&outwater,outpath,namewater);
    }
    if(surfwateryes == 1) {
        sprintf(namesurfwater,"swt%03d.bin",(int)jd);
        openoutfile(&outsurfwater,outpath,namesurfwater);
    }
    if(slushyes == 1) {
        sprintf(nameslush,"slu%03d.bin",(int)jd);
        openoutfile(&outslush,outpath,nameslush);
    }
    if(coldsnowyes == 1) {
        sprintf(namecoldsnow,"cos%03d.bin",(int)jd);
        openoutfile(&outcoldsnow,outpath,namecoldsnow);
    }
    if(coldtotyes == 1) {
        sprintf(namecoldtot,"cot%03d.bin",(int)jd);
        openoutfile(&outcoldtot,outpath,namecoldtot);
    }
    if(precipyes == 1) {
        sprintf(namesnprecip,"sno%03d.bin",(int)jd);
        openoutfile(&outsnprecip,outpath,namesnprecip);
        sprintf(nameraprecip,"rai%03d.bin",(int)jd);
        openoutfile(&outraprecip,outpath,nameraprecip);
    }
    if(intaccumyes == 1) {
        sprintf(namemass,"mas%03d.bin",(int)jd);
        openoutfile(&outmass,outpath,namemass);
        sprintf(namecapwat,"wac%03d.bin",(int)jd);
        openoutfile(&outcapwat,outpath,namecapwat);
        sprintf(nameslwat,"was%03d.bin",(int)jd);
        openoutfile(&outslwat,outpath,nameslwat);
    }
    /**************************************************************************/

    if(degreedaymethod == 1) {
        if(posyes == 1) {
            sprintf(namepos,"pos%03d.bin",(int)jd);
            openoutfile(&outpos,outpath,namepos);
        }
        if(ddfyes == 1) {
            sprintf(nameddf,"ddf%03d.bin",(int)jd);
            openoutfile(&outddf,outpath,nameddf);
        }
    }


    /*DIVIDE EACH GRID VALUE BY NUMBER TIMESTEPS PER DAY TO GET THE DAILY MEAN
      AND RENAME THE ARRAY (SAME NAME AS USED IN writegridoutput) */

    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if (griddgmdrain[i][j] != nodata) {   /*grid within calculated area*/
                if (directfromfile !=1) {  /*direct radiation not read from file*/
                    if(shayes == 1)
                        SHADE[i][j]   = meanSHADEday[i][j]/(24/timestep);
                    if(exkyes == 1)
                        EXKORR[i][j]  = meanDIRECTday[i][j]/meanSOLHORday[i][j];
                    /*this is a weighted mean, instead of an arithmetic mean*/
                    if(solyes == 1)
                        strlsumme[i][j]  = meanSOLHORday[i][j]/(24/timestep);
                }

                if(diryes==1)  DIRECT[i][j]  = meanDIRECTday[i][j]/(24/timestep);
                if(dir2yes==1) DIRECT2[i][j] = meanDIRECT2day[i][j]/(24/timestep);
                if(difyes==1)  DIFFUS[i][j]  = meanDIFFUSday[i][j]/(24/timestep);
                if(gloyes==1)  GLOBAL[i][j]  = meanGLOBALday[i][j]/(24/timestep);
                if(albyes==1)  ALBEDO[i][j]  = meanALBEDOday[i][j]/(24/timestep);
                if(swbyes==1)  SWBAL[i][j]   = meanSWBALday[i][j]/(24/timestep);
                if(linyes==1)  LONGIN[i][j]  = meanLONGINday[i][j]/(24/timestep);
                if(loutyes==1) LONGOUT[i][j] = meanLONGOUTday[i][j]/(24/timestep);
                if(netyes==1)  NETRAD[i][j]  = meanNETRADday[i][j]/(24/timestep);
                if(senyes==1)  SENSIBLE[i][j]= meanSENSIBLEday[i][j]/(24/timestep);
                if(latyes==1)  LATENT[i][j]  = meanLATENTday[i][j]/(24/timestep);
                if(rainyes==1) rainenergy[i][j] = meanRAINENday[i][j]/(24/timestep);


                /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005  add wrt=======*/
                if(balyes==1)  wrtENBAL[i][j]   = meanENBALday[i][j]/(24/timestep);

                if(melyes==1) {
                    if(cumumeltyes == 0)       /*output in daily mean in mm/timestep*/
                        wrtMELT[i][j] = meanMELTday[i][j]/(24/timestep);   /*CHR added wrt*/
                    else                       /*output in cm or m per period*/
                        wrtMELT[i][j] = meanMELTday[i][j]/cm_or_m;   /*CHR added wrt*/
                }  /*if meltyes*/
                /*============================================================*/

                if(ablyes==1) {
                    if(cumumeltyes == 0)       /*output in daily mean in mm/timestep*/
                        wrtABLA[i][j]    = meanABLAday[i][j]/(24/timestep);  /*CHR added wrt*/
                    else                       /*output in cm or m per period*/
                        wrtABLA[i][j]    = meanABLAday[i][j]/cm_or_m;   /*CHR added wrt*/
                }  /*if meltyes*/

                if(runoffyes==1) {
                    if(cumumeltyes == 0)       /*output in daily mean in mm/timestep*/
                        RUNOFF[i][j] = meanrunoffday[i][j]/(24/timestep);
                    else                       /*output in cm or m per period*/
                        RUNOFF[i][j] = meanrunoffday[i][j]/cm_or_m;
                }  /*if meltyes*/
                if(superyes==1) {
                    tmpsuperice[i][j] = superice[i][j];
                    superice[i][j] = (meansupericeday[i][j]/(24/timestep))/densice;   /*cumulated, no mean*/
                }
                if(wateryes==1) watercontent[i][j] = meanwatercontentday[i][j]/(24/timestep);
                if(surfwateryes==1) wrtsurfacewater[i][j] = meansurfacewaterday[i][j]/(24/timestep);
                if(slushyes==1) slushthickness[i][j] = meanslushthicknessday[i][j]/(24/timestep);
                if(coldsnowyes==1) coldcontentsnow[i][j] = meancoldcontentsnowday[i][j]/(24/timestep);
                if(coldtotyes==1)  coldcontentice[i][j] = meancoldcontenticeday[i][j]/(24/timestep);
                if(intaccumyes==1)  sumMASS[i][j]  = sumMASS[i][j];
                if(intaccumyes==1)  capwatercontent[i][j]  = capwatercontent[i][j];
                if(intaccumyes==1)  slwatercontent[i][j]  = slwatercontent[i][j];

                /*============================================================*/

                if(surftempyes==1)  surftemp[i][j] = meanSURFTEMPday[i][j]/(24/timestep);

                if(degreedaymethod == 1) {
                    if(posyes == 1)
                        TEMPOS[i][j]  = meanTEMPOSday[i][j]/(24/timestep);
                    if(ddfyes == 1)
                        DDFCALC[i][j] = meanTEMPOSday[i][j] > 0. ?
                                        meanMELTday[i][j]/meanTEMPOSday[i][j]*(24/timestep) : 0.0;   /*mm/d/K*/
                }

            }  /*if inside area to be calculated*/

            else {    /*grid not to be calculated, no output modeled*/
                if (directfromfile !=1) {
                    if(shayes==1)  SHADE[i][j]   = nodata;
                    if(exkyes==1)  EXKORR[i][j]  = nodata;
                    if(solyes==1)  strlsumme[i][j]  = nodata;
                }

                if(diryes==1)  DIRECT[i][j]  = nodata;
                if(dir2yes==1)  DIRECT2[i][j] = nodata;
                if(difyes==1)  DIFFUS[i][j]  = nodata;
                if(albyes==1)  ALBEDO[i][j]  = nodata;
                if(gloyes==1)  GLOBAL[i][j]  = nodata;
                if(swbyes==1)  SWBAL[i][j]   = nodata;
                if(linyes==1)  LONGIN[i][j]  = nodata;
                if(loutyes==1) LONGOUT[i][j] = nodata;
                if(netyes==1)  NETRAD[i][j]  = nodata;
                if(senyes==1)  SENSIBLE[i][j]= nodata;
                if(latyes==1)  LATENT[i][j]  = nodata;
                if(rainyes==1) rainenergy[i][j] = nodata;
                /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005, add wrt =======*/
                if(balyes==1)  wrtENBAL[i][j]   = nodata;
                if(melyes==1)  wrtMELT[i][j]    = nodata;
                if(ablyes==1)  wrtABLA[i][j]    = nodata;

                if(runoffyes==1)  RUNOFF[i][j] = nodata;  /*wrt because of double*/
                if(superyes==1)   superice[i][j]  = nodata;     /*arrays are float, no wrt*/
                if(wateryes==1)   watercontent[i][j] = nodata;
                if(surfwateryes==1)   wrtsurfacewater[i][j] = nodata;
                if(slushyes==1)   slushthickness[i][j]   = nodata;
                if(coldsnowyes==1)  coldcontentsnow[i][j] = nodata;
                if(coldtotyes==1)   coldcontentice[i][j]  = nodata;
                if(precipyes==1)  sumSNOWprec[i][j] = nodata;
                if(precipyes==1)  sumRAINprec[i][j] = nodata;
                if(intaccumyes==1)  sumMASS[i][j] = nodata;
                if(intaccumyes==1)   capwatercontent[i][j] = nodata;
                if(intaccumyes==1)   slwatercontent[i][j] = nodata;
                /*============================================================*/
                if(surftempyes==1)  surftemp[i][j] = nodata;

                if(degreedaymethod == 1) {
                    if(posyes == 1)   TEMPOS[i][j]  = nodata;
                    if(ddfyes == 1)   DDFCALC[i][j] = nodata;
                }
            }  /*else outside glacier*/

        }  /*for*/

    return;
}



/**************************************************************************/
/* FUNCTION  startwriteall()                                              */
/*    GRID OUTPUT-FILE FOR WHOLE PERIOD                                   */
/*    CREATE OUTPUT-FILENAME e.g. : sha+julianday  e.g. sha190.bin        */
/*    OPEN FILE,   called from main after time loop at the end of main    */
/**************************************************************************/

void startwriteall()

{
    if (directfromfile !=1)

    {
        if(shayes == 1) {
            strcpy(nameshade,"shaall.bin");              /*CREATE FILENAME*/
            openoutfile(&outshade,outpath,nameshade);
        }        /*OPEN FILE*/
        if(exkyes == 1) {
            strcpy(nameexkorr,"exkall.bin");
            openoutfile(&outexkorr,outpath,nameexkorr);
        }
        if(solyes == 1) {
            strcpy(namesolhor,"solall.bin");
            openoutfile(&outsolhor,outpath,namesolhor);
        }
    }

    if(diryes == 1) {
        strcpy(namedirect,"dirall.bin");
        openoutfile(&outdirect,outpath,namedirect);
    }
    if(dir2yes == 1) {
        strcpy(namedirect2,"disall.bin");
        openoutfile(&outdirect2,outpath,namedirect2);
    }
    if(difyes == 1) {
        strcpy(namediffus,"difall.bin");
        openoutfile(&outdiffus,outpath,namediffus);
    }
    if(albyes == 1) {
        strcpy(namealbedoout,"alball.bin");
        openoutfile(&outalbedo,outpath,namealbedoout);
    }
    if(gloyes == 1) {
        strcpy(nameglobal,"gloall.bin");
        openoutfile(&outglobal,outpath,nameglobal);
    }
    if(swbyes == 1) {
        strcpy(nameswbal,"swball.bin");
        openoutfile(&outswbal,outpath,nameswbal);
    }
    if(linyes == 1) {
        strcpy(namelongin,"linall.bin");
        openoutfile(&outlongin,outpath,namelongin);
    }
    if(loutyes == 1) {
        strcpy(namelongout,"louall.bin");
        openoutfile(&outlongout,outpath,namelongout);
    }
    if(netyes == 1) {
        strcpy(namenetrad,"netall.bin");
        openoutfile(&outnetrad,outpath,namenetrad);
    }
    if(senyes == 1) {
        strcpy(namesensible,"senall.bin");
        openoutfile(&outsensible,outpath,namesensible);
    }
    if(latyes == 1) {
        strcpy(namelatent,"latall.bin");
        openoutfile(&outlatent,outpath,namelatent);
    }
    if(rainyes == 1) {
        strcpy(namerainen,"renall.bin");
        openoutfile(&outrainen,outpath,namerainen);
    }
    if(balyes == 1) {
        strcpy(nameenbal,"balall.bin");
        openoutfile(&outenbal,outpath,nameenbal);
    }
    if(melyes == 1) {
        strcpy(namemelt,"melall.bin");
        openoutfile(&outmelt,outpath,namemelt);
    }
    if(ablyes == 1) {
        strcpy(nameabla,"ablall.bin");
        openoutfile(&outabla,outpath,nameabla);
    }

    if(surftempyes == 1) {
        strcpy(namesurftemp,"sutall.bin");
        openoutfile(&outsurftemp,outpath,namesurftemp);
    }

    /*snow model output ****************************************************/
    if(runoffyes == 1) {
        sprintf(namerunoff,"runall.bin");
        openoutfile(&outrunoff,outpath,namerunoff);
    }
    if(superyes == 1) {
        sprintf(namesuper,"supall.bin");
        openoutfile(&outsuper,outpath,namesuper);
    }
    if(wateryes == 1) {
        sprintf(namewater,"watall.bin");
        openoutfile(&outwater,outpath,namewater);
    }
    if(surfwateryes == 1) {
        sprintf(namesurfwater,"swtall.bin");
        openoutfile(&outsurfwater,outpath,namesurfwater);
    }
    if(slushyes == 1) {
        sprintf(nameslush,"sluall.bin");
        openoutfile(&outslush,outpath,nameslush);
    }
    if(coldsnowyes == 1) {
        sprintf(namecoldsnow,"cosall.bin");
        openoutfile(&outcoldsnow,outpath,namecoldsnow);
    }
    if(coldtotyes == 1) {
        sprintf(namecoldtot,"cotall.bin");
        openoutfile(&outcoldtot,outpath,namecoldtot);
    }
    if(precipyes == 1) {
        strcpy(namesnprecip,"snoall.bin");
        openoutfile(&outsnprecip,outpath,namesnprecip);
        strcpy(nameraprecip,"raiall.bin");
        openoutfile(&outraprecip,outpath,nameraprecip);
    }
    if(intaccumyes == 1) {
        strcpy(namemass,"masall.bin");
        openoutfile(&outmass,outpath,namemass);
        strcpy(namecapwat,"wacall.bin");
        openoutfile(&outcapwat,outpath,namecapwat);
        strcpy(nameslwat,"wasall.bin");
        openoutfile(&outslwat,outpath,nameslwat);
    }
    /**************************************************************************/

    if(degreedaymethod == 1) {
        if(posyes == 1) {
            strcpy(namepos,"posall.bin");
            openoutfile(&outpos,outpath,namepos);
        }
        if(ddfyes == 1) {
            strcpy(nameddf,"ddfall.bin");
            openoutfile(&outddf,outpath,nameddf);
        }
    }


    /*DIVIDE EACH GRID VALUE BY NUMBER TIMESTEPS OF PERIOD TO GET THE MEAN
      AND RENAME THE ARRAY (SAME NAME AS USED IN writegridoutput) */

    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if (griddgmdrain[i][j] != nodata) {   /*grid is within calculated area*/
                if (directfromfile !=1) {
                    if(shayes==1)  SHADE[i][j]   = meanSHADEall[i][j]/nsteps;
                    if(exkyes==1)  EXKORR[i][j]  = meanDIRECTall[i][j]/meanSOLHORall[i][j];
                    if(solyes==1)  strlsumme[i][j]  = meanSOLHORall[i][j]/nsteps;
                }

                if(diryes==1)  DIRECT[i][j]   = meanDIRECTall[i][j]/nsteps;
                if(dir2yes==1)  DIRECT2[i][j] = meanDIRECT2all[i][j]/nsteps;
                if(difyes==1)  DIFFUS[i][j]   = meanDIFFUSall[i][j]/nsteps;
                if(albyes==1)  ALBEDO[i][j]   = meanALBEDOall[i][j]/nsteps;
                if(gloyes==1)  GLOBAL[i][j]   = meanGLOBALall[i][j]/nsteps;
                if(swbyes==1)  SWBAL[i][j]    = meanSWBALall[i][j]/nsteps;
                if(linyes==1)  LONGIN[i][j]   = meanLONGINall[i][j]/nsteps;
                if(loutyes==1) LONGOUT[i][j]  = meanLONGOUTall[i][j]/nsteps;
                if(netyes==1)  NETRAD[i][j]   = meanNETRADall[i][j]/nsteps;
                if(senyes==1)  SENSIBLE[i][j] = meanSENSIBLEall[i][j]/nsteps;
                if(latyes==1)  LATENT[i][j]   = meanLATENTall[i][j]/nsteps;
                if(rainyes==1)  rainenergy[i][j] = meanRAINENall[i][j]/nsteps;

                /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005,  change factinter=======*/
                if(balyes==1)  wrtENBAL[i][j] = meanENBALall[i][j]/nsteps;  /*CHR added wrt*/
                if(surftempyes==1)  surftemp[i][j] = meanSURFTEMPall[i][j]/nsteps;

                if(melyes==1) {
                    if(cumumeltyes == 0)
                        wrtMELT[i][j] = meanMELTall[i][j]/nsteps;   /*CHR added wrt*/
                    else
                        wrtMELT[i][j] = meanMELTall[i][j]/cm_or_m;   /*CHR added wrt*/
                }

                if(ablyes==1) {
                    if(cumumeltyes == 0)
                        wrtABLA[i][j]    = meanABLAall[i][j]/nsteps;    /*CHR added wrt*/
                    else
                        wrtABLA[i][j]    = meanABLAall[i][j]/cm_or_m;   /*CHR added wrt*/
                }

                if(runoffyes==1) {
                    if(cumumeltyes == 0)
                        RUNOFF[i][j] = meanrunoffall[i][j]/nsteps;
                    else
                        RUNOFF[i][j] = meanrunoffall[i][j]/cm_or_m;
                }
                if(superyes==1)    superice[i][j] = (meansupericeall[i][j]/nsteps)/densice;
                /*superimposed ice not mean but total*/
                if(wateryes==1)    watercontent[i][j] = meanwatercontentall[i][j]/nsteps;
                if(surfwateryes==1)    wrtsurfacewater[i][j] = meansurfacewaterall[i][j]/nsteps;
                if(slushyes == 1)  slushthickness[i][j]   = meanslushthicknessall[i][j]/nsteps;
                if(coldsnowyes==1) coldcontentsnow[i][j] = meancoldcontentsnowall[i][j]/nsteps;
                if(coldtotyes==1)  coldcontentice[i][j]  = meancoldcontenticeall[i][j]/nsteps;
                if(intaccumyes==1)  sumMASS[i][j]  = sumMASS[i][j];
                if(intaccumyes==1)  capwatercontent[i][j]  = capwatercontent[i][j];
                if(intaccumyes==1)  slwatercontent[i][j]  = slwatercontent[i][j];
                /**************************************************************************/

                if(degreedaymethod == 1) {
                    if(posyes == 1)
                        TEMPOS[i][j]  = meanTEMPOSall[i][j]/nsteps;
                    if(ddfyes == 1)
                        DDFCALC[i][j] = meanTEMPOSall[i][j] > 0. ?    /*refering to a day*/
                                        meanMELTall[i][j]/meanTEMPOSall[i][j]*(24/timestep) : 0.0;
                }

            }  /*if glacier*/
            else {       /*outside calculated area*/
                if (directfromfile !=1) {
                    if(shayes==1)  SHADE[i][j]   = nodata;
                    if(exkyes==1)  EXKORR[i][j]  = nodata;
                    if(solyes==1)  strlsumme[i][j]  = nodata;
                }

                if(diryes==1)   DIRECT[i][j]  = nodata;
                if(dir2yes==1)  DIRECT2[i][j] = nodata;
                if(difyes==1)   DIFFUS[i][j]  = nodata;
                if(albyes==1)   ALBEDO[i][j]  = nodata;
                if(gloyes==1)   GLOBAL[i][j]  = nodata;
                if(swbyes==1)   SWBAL[i][j]   = nodata;
                if(linyes==1)   LONGIN[i][j]  = nodata;
                if(loutyes==1)  LONGOUT[i][j] = nodata;
                if(netyes==1)   NETRAD[i][j]  = nodata;
                if(senyes==1)   SENSIBLE[i][j]= nodata;
                if(latyes==1)   LATENT[i][j]  = nodata;
                if(rainyes==1)  rainenergy[i][j] = nodata;

                /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
                if(balyes==1)   wrtENBAL[i][j]   = nodata;  /*CHR added wrt*/
                if(melyes==1)   wrtMELT[i][j]    = nodata;
                if(ablyes==1)   wrtABLA[i][j]    = nodata;

                if(runoffyes==1)   RUNOFF[i][j]  = nodata;
                if(superyes==1)    superice[i][j]   = nodata;
                if(wateryes==1)    watercontent[i][j] = nodata;
                if(surfwateryes==1)    wrtsurfacewater[i][j] = nodata;
                if(slushyes == 1)  slushthickness[i][j]   = nodata;
                if(coldsnowyes==1) coldcontentsnow[i][j] = nodata;
                if(coldtotyes==1)  coldcontentice[i][j]  = nodata;
                if(precipyes==1)  sumSNOWprec[i][j] = nodata;
                if(precipyes==1)  sumRAINprec[i][j] = nodata;
                if(intaccumyes==1)  sumMASS[i][j] = nodata;
                if(intaccumyes==1)  capwatercontent[i][j] = nodata;
                if(intaccumyes==1)  slwatercontent[i][j] = nodata;
                /*============================================================*/
                if(surftempyes==1)   surftemp[i][j] = nodata;

                if(degreedaymethod == 1) {
                    if(posyes == 1)    TEMPOS[i][j]  = nodata;
                    if(ddfyes == 1)    DDFCALC[i][j] = nodata;
                }
            }  /*else  outside glacier*/

        }  /*for*/

    return;
}


/*=======================================================================*/
/**** CALCULATION OF STATISTICS FOR SECOND 6 ROWS IN OUTPUTFILE       ****/
/*=======================================================================*/

void statist(float **matrix,float *x)

{

    x[10]= 0.0;     /*sum of values in grid*/
    x[7] = 0.0;          /*number of grids*/
    x[8] = 10000.0;      /*minimum, must be initialized high*/
    x[9] = -1000.0;      /*maximum*/

    for (i=1; i<=nrows; i++) {       /*ueber alle Reihen*/
        for (j=1; j<=ncols; j++) {    /*ueber alle Reihen*/
            if (matrix[i][j] != nodata) {   /*only area to be calcualated*/
                x[7] += 1.0;        /*number of grids*/

                if (matrix[i][j] > x[9])   x[9] = matrix[i][j];   /*maximum*/
                if (matrix[i][j] < x[8])   x[8] = matrix[i][j];   /*minimum*/
                x[10] += matrix[i][j];   /*sum*/
            }  /*endif*/

        }  /*for col*/
    }   /*for row*/

    if (x[7]!=0.0)     /*no division by zero*/
        x[11] = x[10]/x[7];      /*mean=sum/number*/
    else
        x[11] = nodata;

    x[12] = nodata;     /*standard deviation not calculated*/
    return;
}

/*=======================================================================*/
/**** CALCULATE STATISTICS FOR SECOND 6 ROWS IN OUTPUTFILE, double precision ****/
/*=======================================================================*/

void statistdouble(double **matrix,float *x)
{

    x[10]= 0.0;     /*sum of values in grid*/
    x[7] = 0.0;          /*number of grids*/
    x[8] = 10000.0;      /*minimum, must be initialized high*/
    x[9] = -1000.0;      /*maximum*/

    for (i=1; i<=nrows; i++) {       /*ueber alle Reihen*/
        for (j=1; j<=ncols; j++) {    /*ueber alle Reihen*/
            if (matrix[i][j] != nodata) {   /*only area to be calcualated*/
                x[7] += 1.0;        /*number of grids*/

                if (matrix[i][j] > x[9])   x[9] = matrix[i][j];   /*maximum*/
                if (matrix[i][j] < x[8])   x[8] = matrix[i][j];   /*minimum*/
                x[10] += matrix[i][j];   /*sum*/
            }  /*endif*/

        }  /*for col*/
    }   /*for row*/

    if (x[7]!=0.0)     /*no division by zero*/
        x[11] = x[10]/x[7];      /*mean=sum/number*/
    else
        x[11] = nodata;

    x[12] = nodata;     /*standard deviation not calculated*/
    return;
}


/**************************************************************************/
/* FUNCTION  writegridoutput()                                            */
/*           WRITE GRID DATA TO OUTPUT-FILE hourly, daily, period means   */
/*  called from main after grid loop (hourly,daily) after time loop (period) */
/**************************************************************************/

void writegridoutput()

{
    /* CALCULATE STATISTICS AND WRITE FIRST 12 ROW TO OUTPUT-FILE
       second 6 rows = statistics */

    /*no writing of shade and exkorr if direct radiation is read from file*/
    /* because shade and exkor files are not created in programm and may not exist*/

    if (directfromfile != 1)      /*only if direct rad is not read from file*/
        /* no writing of shade and exkorr if direct radiation is read from file*/
        /* because shade,exkor,solhor files are not created in programm */

    {
        if(shayes == 1) {
            statist(SHADE,x);
            fwrite(&(x[1]),sizeof(float),12,outshade);
        }
        if(exkyes == 1) {
            statist(EXKORR,x);
            fwrite(&(x[1]),sizeof(float),12,outexkorr);
        }
        if(solyes == 1) {
            statist(strlsumme,x);
            fwrite(&(x[1]),sizeof(float),12,outsolhor);
        }
        if(diryes == 1) {
            statist(DIRECT,x);
            fwrite(&(x[1]),sizeof(float),12,outdirect);
        }
    }

    /*if direct rad read from file, writing of direct rad only if output not hourly,
      because this already exists in this case; daily or period means can be calculated
      from the data of existing files and written to file */
    if((directfromfile == 1) && (do_out != 1)) {
        if(diryes == 1) {
            statist(DIRECT,x);
            fwrite(&(x[1]),sizeof(float),12,outdirect);
        }
    }

    if(dir2yes == 1) {
        statist(DIRECT2,x);
        fwrite(&(x[1]),sizeof(float),12,outdirect2);
    }
    if(difyes == 1) {
        statist(DIFFUS,x);
        fwrite(&(x[1]),sizeof(float),12,outdiffus);
    }
    if(albyes == 1) {
        statist(ALBEDO,x);
        fwrite(&(x[1]),sizeof(float),12,outalbedo);
    }
    if(gloyes == 1) {
        statist(GLOBAL,x);
        fwrite(&(x[1]),sizeof(float),12,outglobal);
    }
    if(swbyes == 1) {
        statist(SWBAL,x);
        fwrite(&(x[1]),sizeof(float),12,outswbal);
    }
    if(linyes == 1) {
        statist(LONGIN,x);
        fwrite(&(x[1]),sizeof(float),12,outlongin);
    }
    if(loutyes == 1) {
        statist(LONGOUT,x);
        fwrite(&(x[1]),sizeof(float),12,outlongout);
    }
    if(netyes == 1) {
        statist(NETRAD,x);
        fwrite(&(x[1]),sizeof(float),12,outnetrad);
    }
    if(senyes == 1) {
        statist(SENSIBLE,x);
        fwrite(&(x[1]),sizeof(float),12,outsensible);
    }
    if(latyes == 1) {
        statist(LATENT,x);
        fwrite(&(x[1]),sizeof(float),12,outlatent);
    }
    if(rainyes == 1) {
        statist(rainenergy,x);
        fwrite(&(x[1]),sizeof(float),12,outrainen);
    }

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    if(balyes == 1)
        /*CHR{ statist(ENBAL,x);
          fwrite(&(x[1]),sizeof(float),12,outenbal);  }*/
    {
        statistdouble(ENBAL,x);
        fwrite(&(x[1]),sizeof(float),12,outenbal);
    }
    if(melyes == 1)
        /*CHR{ statist(MELT,x);
          fwrite(&(x[1]),sizeof(float),12,outmelt);  }*/
    {
        statistdouble(MELT,x);
        fwrite(&(x[1]),sizeof(float),12,outmelt);
    }
    if(ablyes == 1)
        /*CHR  { statist(ABLA,x);
            fwrite(&(x[1]),sizeof(float),12,outabla);  }*/
    {
        statistdouble(ABLA,x);
        fwrite(&(x[1]),sizeof(float),12,outabla);
    }

    if(runoffyes == 1) {
        statist(RUNOFF,x);
        fwrite(&(x[1]),sizeof(float),12,outrunoff);
    }
    if(superyes == 1) {
        statist(superice,x);
        fwrite(&(x[1]),sizeof(float),12,outsuper);
    }
    if(wateryes == 1) {
        statistdouble(watercontent,x);
        fwrite(&(x[1]),sizeof(float),12,outwater);
    }
    if(surfwateryes == 1) {
        statistdouble(surfacewater,x);
        fwrite(&(x[1]),sizeof(float),12,outsurfwater);
    }
    if(slushyes == 1) {
        statist(slushthickness,x);
        fwrite(&(x[1]),sizeof(float),12,outslush);
    }
    if(coldsnowyes == 1) {
        statist(coldcontentsnow,x);
        fwrite(&(x[1]),sizeof(float),12,outcoldsnow);
    }
    if(coldtotyes == 1) {
        statist(coldcontentice,x);
        fwrite(&(x[1]),sizeof(float),12,outcoldtot);
    }

    if(precipyes == 1) {
        statist(sumSNOWprec,x);
        fwrite(&(x[1]),sizeof(float),12,outsnprecip);
        statist(sumRAINprec,x);
        fwrite(&(x[1]),sizeof(float),12,outraprecip);
    }
    if(intaccumyes == 1) {
        statist(sumMASS,x);
        fwrite(&(x[1]),sizeof(float),12,outmass);
        statist(capwatercontent,x);
        fwrite(&(x[1]),sizeof(float),12,outcapwat);
        statist(slwatercontent,x);
        fwrite(&(x[1]),sizeof(float),12,outslwat);
    }

    /*============================================================*/

    if(surftempyes == 1) {
        statist(surftemp,x);
        fwrite(&(x[1]),sizeof(float),12,outsurftemp);
    }


    if(degreedaymethod == 1) {
        if(posyes == 1) {
            statist(TEMPOS,x);
            fwrite(&(x[1]),sizeof(float),12,outpos);
        }
        if(ddfyes == 1) {
            statist(DDFCALC,x);
            fwrite(&(x[1]),sizeof(float),12,outddf);
        }
    }


    /* WRITE GRIDDATA TO FILE */

    for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
    {
        /*no writing of direct radiation read from file*/
        /* because shade and exkor files are not created in programm and may not exist*/

        if (directfromfile != 1) {    /*only if direct rad not read from file*/
            if(shayes == 1)
                fwrite(&(SHADE[i][1]),sizeof(float),ncols,outshade);    /*all columns*/
            if(exkyes == 1)
                fwrite(&(EXKORR[i][1]),sizeof(float),ncols,outexkorr);
            if(solyes == 1)
                fwrite(&(strlsumme[i][1]),sizeof(float),ncols,outsolhor);
            if(diryes == 1)
                fwrite(&(DIRECT[i][1]),sizeof(float),ncols,outdirect);
        }

        /*if direct rad read from file, writing only if output not hourly*/
        if((directfromfile == 1) && (do_out != 1)) {
            if(diryes == 1)
                fwrite(&(DIRECT[i][1]),sizeof(float),ncols,outdirect);
        }

        if(dir2yes == 1)
            fwrite(&(DIRECT2[i][1]),sizeof(float),ncols,outdirect2);
        if(difyes == 1)
            fwrite(&(DIFFUS[i][1]),sizeof(float),ncols,outdiffus);
        if(albyes == 1)
            fwrite(&(ALBEDO[i][1]),sizeof(float),ncols,outalbedo);
        if(gloyes == 1)
            fwrite(&(GLOBAL[i][1]),sizeof(float),ncols,outglobal);
        if(swbyes == 1)
            fwrite(&(SWBAL[i][1]),sizeof(float),ncols,outswbal);
        if(linyes == 1)
            fwrite(&(LONGIN[i][1]),sizeof(float),ncols,outlongin);
        if(loutyes == 1)
            fwrite(&(LONGOUT[i][1]),sizeof(float),ncols,outlongout);
        if(netyes == 1)
            fwrite(&(NETRAD[i][1]),sizeof(float),ncols,outnetrad);
        if(senyes == 1)
            fwrite(&(SENSIBLE[i][1]),sizeof(float),ncols,outsensible);
        if(latyes == 1)
            fwrite(&(LATENT[i][1]),sizeof(float),ncols,outlatent);
        if(rainyes == 1)
            fwrite(&(rainenergy[i][1]),sizeof(float),ncols,outrainen);

        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        if(balyes == 1)
            fwrite(&(wrtENBAL[i][1]),sizeof(float),ncols,outenbal); /*CHR added wrt*/
        if(melyes == 1)
            fwrite(&(wrtMELT[i][1]),sizeof(float),ncols,outmelt); /*CHR added wrt*/
        if(ablyes == 1)
            fwrite(&(wrtABLA[i][1]),sizeof(float),ncols,outabla); /*CHR added wrt*/

        if(runoffyes == 1)
            fwrite(&(RUNOFF[i][1]),sizeof(float),ncols,outrunoff);
        if(superyes == 1)
            fwrite(&(superice[i][1]),sizeof(float),ncols,outsuper);
        if(wateryes == 1)
            fwrite(&(watercontent[i][1]),sizeof(float),ncols,outwater);
        if(surfwateryes == 1)
            fwrite(&(wrtsurfacewater[i][1]),sizeof(float),ncols,outsurfwater);
        if(slushyes == 1)
            fwrite(&(slushthickness[i][1]),sizeof(float),ncols,outslush);
        if(coldsnowyes == 1)
            fwrite(&(coldcontentsnow[i][1]),sizeof(float),ncols,outcoldsnow);
        if(coldtotyes == 1)
            fwrite(&(coldcontentice[i][1]),sizeof(float),ncols,outcoldtot);

        if(precipyes == 1) {
            fwrite(&(sumSNOWprec[i][1]),sizeof(float),ncols,outsnprecip);
            fwrite(&(sumRAINprec[i][1]),sizeof(float),ncols,outraprecip);
        }
        if(intaccumyes == 1) {
            fwrite(&(sumMASS[i][1]),sizeof(float),ncols,outmass);
            fwrite(&(capwatercontent[i][1]),sizeof(float),ncols,outcapwat);
            fwrite(&(slwatercontent[i][1]),sizeof(float),ncols,outslwat);
        }

        /*============================================================*/
        if(surftempyes == 1)
            fwrite(&(surftemp[i][1]),sizeof(float),ncols,outsurftemp);


        if(degreedaymethod == 1) {
            if(posyes == 1)   fwrite(&(TEMPOS[i][1]),sizeof(float),ncols,outpos);
            if(ddfyes == 1)   fwrite(&(DDFCALC[i][1]),sizeof(float),ncols,outddf);
        }

    }  /*end for  next row*/

    /* CLOSE FILES, ONLY IF THEY EXIST, if shayes=0 etc  they don't exist */
    if(outshade)    closefile(&outshade,nameshade);
    if(outexkorr)   closefile(&outexkorr,nameexkorr);
    if(outsolhor)   closefile(&outsolhor,namesolhor);
    if(outdirect)   closefile(&outdirect,namedirect);
    if(outdirect2)  closefile(&outdirect2,namedirect2);
    if(outdiffus)   closefile(&outdiffus,namediffus);
    if(outalbedo)   closefile(&outalbedo,namealbedoout);
    if(outglobal)   closefile(&outglobal,nameglobal);
    if(outswbal)    closefile(&outswbal,nameswbal);
    if(outlongin)   closefile(&outlongin,namelongin);
    if(outlongout)  closefile(&outlongout,namelongout);
    if(outnetrad)   closefile(&outnetrad,namenetrad);
    if(outsensible) closefile(&outsensible,namesensible);
    if(outlatent)   closefile(&outlatent,namelatent);
    if(outrainen)   closefile(&outrainen,namerainen);
    if(outenbal)    closefile(&outenbal,nameenbal);
    if(outmelt)     closefile(&outmelt,namemelt);
    if(outabla)     closefile(&outabla,nameabla);
    if(outsurftemp)     closefile(&outsurftemp,namesurftemp);

    /*snow model*/
    if(outrunoff)    closefile(&outrunoff,namerunoff);
    if(outsuper)     closefile(&outsuper,namesuper);
    if(outwater)     closefile(&outwater,namewater);
    if(outslush)     closefile(&outslush,nameslush);
    if(outcoldsnow)  closefile(&outcoldsnow,namecoldsnow);
    if(outcoldtot)   closefile(&outcoldtot,namecoldtot);
    if(outsnprecip)  closefile(&outsnprecip,namesnprecip);
    if(outraprecip)  closefile(&outraprecip,nameraprecip);
    if(outmass)      closefile(&outmass,namemass);
    if(outcapwat)    closefile(&outcapwat,namecapwat);
    if(outslwat)     closefile(&outslwat,nameslwat);
    /************/

    if(outpos)      closefile(&outpos,namepos);
    if(outddf)      closefile(&outddf,nameddf);

    return;
}



/****************************************************************/
/* FUNCTION  sumday
             SUM UP OVER ALL TIMESTEPS OF DAY FOR EACH GRID     */
/****************************************************************/

void sumday()

{
    for (i=firstrow; i<=lastrow; i++)
        for (j=firstcol[i]; j<=lastcol[i]; j++) {
            if (directfromfile != 1) {
                if(shayes == 1)  meanSHADEday[i][j]   += SHADE[i][j];
                if(solyes == 1)  meanSOLHORday[i][j]  += strlsumme[i][j]/timestep;
            }

            if(diryes == 1)   meanDIRECTday[i][j]  += DIRECT[i][j];    /*clear-sky*/
            if(dir2yes == 1)  meanDIRECT2day[i][j] += DIRECT2[i][j];   /*actual*/
            if(difyes == 1)   meanDIFFUSday[i][j]  += DIFFUS[i][j];
            if(albyes == 1)   meanALBEDOday[i][j]  += ALBEDO[i][j];
            if(gloyes == 1)   meanGLOBALday[i][j]  += GLOBAL[i][j];
            if(swbyes == 1)   meanSWBALday[i][j]   += SWBAL[i][j];
            if(linyes == 1)   meanLONGINday[i][j]  += LONGIN[i][j];
            if(loutyes == 1)  meanLONGOUTday[i][j] += LONGOUT[i][j];
            if(netyes == 1)   meanNETRADday[i][j]  += NETRAD[i][j];
            if(senyes == 1)   meanSENSIBLEday[i][j]+= SENSIBLE[i][j];
            if(latyes == 1)   meanLATENTday[i][j]  += LATENT[i][j];
            if(rainyes == 1)  meanRAINENday[i][j]  += rainenergy[i][j];
            if(balyes == 1)   meanENBALday[i][j]   += ENBAL[i][j];
            if(melyes == 1)   meanMELTday[i][j]    += MELT[i][j];
            if(ablyes == 1)   meanABLAday[i][j]    += ABLA[i][j];
            if(surftempyes == 1)   meanSURFTEMPday[i][j]    += surftemp[i][j];

            /*snowmodel*/
            if(runoffyes == 1)   meanrunoffday[i][j]    += RUNOFF[i][j];
            if(superyes == 1)    meansupericeday[i][j]     += superice[i][j];
            if(wateryes == 1)    meanwatercontentday[i][j] += watercontent[i][j];
            if(surfwateryes == 1)    meansurfacewaterday[i][j] += surfacewater[i][j];
            if(slushyes == 1)    meanslushthicknessday[i][j]   += slushthickness[i][j];
            if(coldsnowyes == 1) meancoldcontentsnowday[i][j] += coldcontentsnow[i][j];
            if(coldtotyes == 1)  meancoldcontenticeday[i][j]  += coldcontentice[i][j];


            if((degreedaymethod == 1) && (posyes == 1))
                meanTEMPOSday[i][j]  += TEMPOS[i][j];

        }  /*FOR COLUMNS*/

    return;
}



/*****************************************************************/
/* FUNCTION  sumall
             SUM UP OVER ALL TIMESTEPS OF PERIOD OF CALCULATION  */
/*           for grid output over whole period                   */
/*****************************************************************/

void sumall()

{
    for (i=firstrow; i<=lastrow; i++)
        for (j=firstcol[i]; j<=lastcol[i]; j++) {
            if (directfromfile != 1) {
                if(shayes == 1)
                    meanSHADEall[i][j]   = meanSHADEall[i][j] + SHADE[i][j];
                if(solyes == 1)
                    meanSOLHORall[i][j]  += strlsumme[i][j]/timestep;
            }

            if(diryes == 1)  meanDIRECTall[i][j]  += DIRECT[i][j];
            if(dir2yes == 1) meanDIRECT2all[i][j] += DIRECT2[i][j];
            if(difyes == 1)  meanDIFFUSall[i][j]  += DIFFUS[i][j];
            if(albyes == 1)  meanALBEDOall[i][j]  += ALBEDO[i][j];
            if(gloyes == 1)  meanGLOBALall[i][j]  += GLOBAL[i][j];
            if(swbyes == 1)  meanSWBALall[i][j]   += SWBAL[i][j];
            if(linyes == 1)  meanLONGINall[i][j]  += LONGIN[i][j];
            if(loutyes == 1) meanLONGOUTall[i][j] += LONGOUT[i][j];
            if(netyes == 1)  meanNETRADall[i][j]  += NETRAD[i][j];
            if(senyes == 1)  meanSENSIBLEall[i][j]+= SENSIBLE[i][j];
            if(latyes == 1)  meanLATENTall[i][j]  += LATENT[i][j];
            if(rainyes == 1)  meanRAINENall[i][j]  += rainenergy[i][j];
            if(balyes == 1)  meanENBALall[i][j]   += ENBAL[i][j];
            if(melyes == 1)  meanMELTall[i][j]    += MELT[i][j];
            if(ablyes == 1)  meanABLAall[i][j]    += ABLA[i][j];
            if(surftempyes == 1)  meanSURFTEMPall[i][j] += surftemp[i][j];

            if(runoffyes == 1)   meanrunoffall[i][j]    += RUNOFF[i][j];
            if(superyes == 1)    meansupericeall[i][j]     += superice[i][j];
            if(wateryes == 1)    meanwatercontentall[i][j] += watercontent[i][j];
            if(surfwateryes == 1)    meansurfacewaterall[i][j] += surfacewater[i][j];
            if(slushyes == 1)    meanslushthicknessall[i][j]   += slushthickness[i][j];
            if(coldsnowyes == 1) meancoldcontentsnowall[i][j] += coldcontentsnow[i][j];
            if(coldtotyes == 1)  meancoldcontenticeall[i][j]  += coldcontentice[i][j];


            if((degreedaymethod == 1) && (posyes == 1))
                meanTEMPOSall[i][j]  += TEMPOS[i][j];

        }  /*FOR COLUMNS*/

    return;
}


/*****************************************************************/
/* FUNCTION  writesnoworsurfaceyes                               */
/*     check if snow cover or surface type should be written to  */
/*     file; 2 options: every x th day or for randomly  selected */
/*         called from main every time step                      */
/*****************************************************************/

void writesnoworsurfaceyes()

{
    write2fileyes = 0;    /*default no output*/

    if((surfyes == 1) || (snowyes == 1)) {  /*output every x th day*/
        if((zeit == 24) && (jd/daysnow == floor(jd/daysnow)))
            write2fileyes = 1;
    }

    if((surfyes == 2) || (snowyes == 2)) {  /*output */
        if( (zeit == 24) && (jd == jdsurface[newday]) ) {
            write2fileyes = 1;
            newday += 1;
        }
    }
    if(newday > numbersnowdaysout)  /*all days for one year written to output*/
        newday = 1;    /*set to first day for next year to be outputted*/

    return;
}


/*****************************************************************/
/* FUNCTION  writesurface                                        */
/*     write surface conditions to file once per day, (not daily */
/*     means but grid for time step) for validation of snow line */
/*       for both meltmod and degree, but only if initial snow   */
/*       cover used as input,  called from main                  */
/*****************************************************************/

void writesurface()

{
    sprintf(namesurf,"sur%03d%02d%04d.bin",(int)jd,(int)zeit,(int)year);
    openoutfile(&outsurf,outpath,namesurf);

    statist(surface,x);
    fwrite(&(x[1]),sizeof(float),12,outsurf);     /*FIRST 12 ROWS*/

    for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
        fwrite(&(surface[i][1]),sizeof(float),ncols,outsurf);

    if(outsurf)
        closefile(&outsurf,namesurf);

    return;
}


/*****************************************************************/
/* FUNCTION  writesnowcover                                      */
/*     write snow cover to file once per day, (not daily         */
/*     means but grid for time step) for validation of snow melt */
/*       for both meltmod and degree, but only if initial snow   */
/*       cover used as input                                     */
/*  called from main                                             */
/*****************************************************************/

void writesnowcover()

{
    sprintf(namewritesnow,"sno%03d%02d%04d.bin",(int)jd,(int)zeit,(int)year);
    openoutfile(&outwritesnow,outpath,namewritesnow);

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    /*CHR   statist(SNOW,x);
       fwrite(&(x[1]),sizeof(float),12,outwritesnow);   */  /*FIRST 12 ROWS*/
    statistdouble(SNOW,x);
    fwrite(&(x[1]),sizeof(float),12,outwritesnow);     /*FIRST 12 ROWS*/

    for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
        for (j=1; j<=ncols; j++) /*CHR added*/
            wrtSNOW[i][j] = SNOW[i][j];   /*CHR added*/

    for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
        fwrite(&(wrtSNOW[i][1]),sizeof(float),ncols,outwritesnow);
    /*============================================================*/

    if(outwritesnow)
        closefile(&outwritesnow,namewritesnow);

    return;
}


/*============= MASS BALANCE OUTPUT ==========================================*/
/*****************************************************************/
/* FUNCTION  writemassbalgrid                                    */
/*     write summer/winter and massbalance to grid file          */
/*     and also annual mass balances to ascii-file               */
/*     and mass balance profiles for each year                   */
/*  called every day at midnight (if zeit=24, in time step loop) */
/*     modified May 2006                                         */
/*****************************************************************/

void writemassbalgrid()

{
    float summerbaljd;    /*jd written to file name for summer balance file*/
    /*= jd of summerbal end from input.dat or each day for Dyurgerov method*/
    int winterprofile_yes=0, summerprofile_yes=0, massbalprofile_yes=0;
    float elevbelt;    /*lower limit of elevation band to be checked*/
    float factorcm=100;    /*mass balance is in cm and then divided by this factor, 100=in meters*/
    /* printf("\n    %.0f %.0f winter=%d summer=%d ",year,jd,itswinter,itssummer); */

 /*** =========== WINTER MASS BALANCE ===================== ***/
    if(winterbalyes == 1)
        if(jd == winterjdend) {   /*only once a year at end of winter*/
            for(i=1; i<=numberbelt; i++)
                winterbalprofile[i] = 0;   /*array with balance for each elevation band*/

            sprintf(namewinterbal,"winbal%04d_%03d.bin",(int)year,(int)winterjdend);
            openoutfile(&outwinterbal,outpath,namewinterbal);
            statist(WINTERBAL,x);
            fwrite(&(x[1]),sizeof(float),12,outwinterbal);     /*FIRST 12 ROWS*/
            for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
                fwrite(&(WINTERBAL[i][1]),sizeof(float),ncols,outwinterbal);

            if(outwinterbal)
                closefile(&outwinterbal,namewinterbal);
            outwinterbal = NULL;

      /*winter mass balance profiles;  must be computed before array is set to zero*/
            computemassbalprofile(WINTERBAL,winterbalprofile);
            winterprofile_yes = 1;     /*if 1 then it will be written to file below*/

            initializeglacier2zero_nodata(nrows, ncols, WINTERBAL);

      /*** WRITE AREA-AVERAGED WINTER MASS BALANCE TO FILE only if winter and summer is computed, seasonalmassbal.txt ***/
            if(summerbalyes == 1) {      /*x[11] is output from statist(), = mean value*/
                cumwinterbal += x[11]/factorcm;
                fprintf(outspecificmassbal,"%5.0f %4.0f %8.3f",year-1,year,x[11]/factorcm);
            }
            printf("\n Winter mass balance %5.0f %4.0f = %8.3f cm\n",year-1,year,x[11]);
        }

 /*** =========== SUMMER MASS BALANCE ===================== ***/
    if(summerbalyes == 1)
        if((jd == summerjdend) || (bs_eachday_yes == 1))
            if( (datesfromfileyes == 0) || ((datesfromfileyes == 1) && (year == nextyear)))
                /*second condition needed not to write to file twice a year if varying dates of summerend*/
            {
                if(bs_eachday_yes == 1)  /*to write to file summer balance for each day - for Dyurgerov method*/
                    summerbaljd = jd;    /*to put jd for each day into file name*/
                else
                    summerbaljd = summerjdend;   /*jd when summer ends put into summerbal file*/

                for(i=1; i<=numberbelt; i++)
                    summerbalprofile[i] = 0;

                if (methodsurftempglac == 4) {
                    sprintf(namesummerbalst,"sumbalst%04d_%03d.bin",(int)year,(int)summerbaljd);
                    openoutfile(&outsummerbalst,outpath,namesummerbalst);
                    statist(SUMMERBALST,x);
                    fwrite(&(x[1]),sizeof(float),12,outsummerbalst);     /*FIRST 12 ROWS*/
                    for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
                        fwrite(&(SUMMERBALST[i][1]),sizeof(float),ncols,outsummerbalst);
                    if(outsummerbalst)
                        closefile(&outsummerbalst,namesummerbalst);
                    outsummerbalst = NULL;

                    if(bs_eachday_yes != 1)    /*do not set to 0 in case every day is written to file*/
                        initializeglacier2zero_nodata(nrows, ncols, SUMMERBALST);

                    printf("\n Summer mass balance (stake method) %5.0f = %8.3f cm\n",year,x[11]);   /*mean of grid*/
                    fprintf(outcontrol,"\n Summer mass balance (stake method) %5.0f = %8.3f cm\n",year,x[11]);
                }

                sprintf(namesummerbal,"sumbal%04d_%03d.bin",(int)year,(int)summerbaljd);
                openoutfile(&outsummerbal,outpath,namesummerbal);
                statist(SUMMERBAL,x);    /*array cumulated in massbalance() in turbul.c*/
                fwrite(&(x[1]),sizeof(float),12,outsummerbal);     /*FIRST 12 ROWS*/
                for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
                    fwrite(&(SUMMERBAL[i][1]),sizeof(float),ncols,outsummerbal);
                if(outsummerbal)
                    closefile(&outsummerbal,namesummerbal);
                outsummerbal = NULL;

                /*mass balance profiles;  must be computed before array is set to zero*/
                computemassbalprofile(SUMMERBAL,summerbalprofile);
                summerprofile_yes = 1;     /*if 1 then it will be written to file below*/

                if(bs_eachday_yes != 1)    /*do not set to 0 in case every day is written to file*/
                    initializeglacier2zero_nodata(nrows, ncols, SUMMERBAL);   /*set to 0 for next year's balance*/

    /*** WRITE AREA-AVERAGED WINTER MASS BALANCE TO FILE only if winter and summer is computed, seasonalmassbal.txt ***/
                if(winterbalyes == 1) {    /*summerbalyes is alreday 1*/
                    cumsummerbal += x[11]/factorcm;     /*/x[11] is mean over glacier grid*/
                          /*cumulative annual area-averaged summer mass balances*/
                    fprintf(outspecificmassbal," %8.3f",x[11]/factorcm);
                }
                printf("\n Summer mass balance (ablation method) %5.0f = %8.3f cm\n",year,x[11]);
                fprintf(outcontrol,"\n Summer mass balance %5.0f = %8.3f cm\n",year,x[11]);
            }

 /*** =========== ANNUAL NET MASS BALANCE ===================== ***/
    if((winterbalyes == 1) && (summerbalyes == 1))
        if(jd == summerjdend)
            if( (datesfromfileyes == 0) || ((datesfromfileyes == 1) && (year == nextyear)))
                /*second condition needed not to write to file twice a year if varying dates of summerend*/
            {
                for(i=1; i<=numberbelt; i++)
                    massbalprofile[i] = 0;

                sprintf(namemassbal,"massbal%04d_%03d.bin",(int)year,(int)summerjdend);
                openoutfile(&outmassbal,outpath,namemassbal);
                statist(MASSBALcum,x);   /*here the glacier-wide specific balance x[11] is computed*/

		if(retreatyes > 1)  /*mean balance needed for retreat parameterization, x[11] may be overridden*/
		   specificglacwidebalance = x[11];

                fwrite(&(x[1]),sizeof(float),12,outmassbal);     /*FIRST 12 ROWS*/
                for (i=1; i<=nrows; i++)      /* for all rows (and columns in fwrite command) */
                    fwrite(&(MASSBALcum[i][1]),sizeof(float),ncols,outmassbal);
                if(outmassbal)
                    closefile(&outmassbal,namemassbal);
                outmassbal = NULL;

                /*mass balance profiles;  must be computed before array is set to zero*/
                computemassbalprofile(MASSBALcum,massbalprofile);
                massbalprofile_yes = 1;

                if (retreatyes <= 1)     /*massbal array needed for retreat parameterization, set to 0 later*/    
                  initializeglacier2zero_nodata(nrows, ncols, MASSBALcum);   /*cum annual net balance grid*/

            /*********** WRITE AREA-AVERAGED NET MASS BALANCE TO FILE EACH YEAR****************/
                printf(" Mass balance %5.0f/%4.0f = %8.3f cm\n",year-1,year,x[11]);
                fprintf(outcontrol," Mass balance %5.0f/%4.0f = %8.3f cm\n",year-1,year,x[11]);
                cumnetbal += x[11]/factorcm;    /*x[11] is mean, output from statist*/
                fprintf(outspecificmassbal," %8.3f %8.3f %8.3f %8.3f %5.0f %5.0f %5.0f",
                        x[11]/factorcm,cumwinterbal,cumsummerbal,cumnetbal,winterjdbeg,summerjdbeg,summerjdend);

                if(retreatyes == 0)  /*if area changes are not computed and therefore no area columns written into that file*/
                    fprintf(outspecificmassbal," \n");

                readdatesmassbalyes = 1;
                /*to allow allocating dates for next massbalance year in function massbalance*/

            }  /*endif*/


    /*====== WRITE MASS BALANCE PROFILES TO FILE - ONE FILE FOR EACH YEAR ======***/

    if((winterprofile_yes == 1) || (summerprofile_yes == 1) || (massbalprofile_yes == 1)) {
        if(dat_or_txt == 1)
            sprintf(namemassbalprofile,"massbalprofile%04d.dat",(int)year);
        else
            sprintf(namemassbalprofile,"massbalprofile%04d.txt",(int)year);

        openoutfile(&outmassbalprofile,outpath,namemassbalprofile);

        fprintf(outmassbalprofile,"no elevation(m)  pixels  area(m2)     bw(m)     bs    bn\n");
        fprintf(outmassbalprofile,"Mass balance profiles for %d-%d, elevation is middle of band\n",(int)year-1,(int)year);
        elevbelt = elevbeltmin;

        for (i=1; i<=numberbelt; i++) {    /* for all rows (and columns in fwrite command) */
            fprintf(outmassbalprofile,"%d %8.0f %.0f  %0.2f    ",i,elevbelt+(beltwidth/2),areabelt[i],areabelt[i]*gridsize*gridsize);
            fprintf(outmassbalprofile,"%12.3f %12.3f %12.3f\n",winterbalprofile[i]/factorcm,summerbalprofile[i]/factorcm,massbalprofile[i]/factorcm);
            elevbelt += beltwidth;
        }
        closefile(&outmassbalprofile,namemassbalprofile);

        if(outwinterbal)
            closefile(&outwinterbal,namewinterbal);
        outwinterbal = NULL;
    }  /*endif*/

    return;
}


/*****************************************************************/
/* FUNCTION  computemassbalprofile                               */
/*     compute the mass balance of each elevaton belt            */
/*  called from function writemassbalgrid() above for each variable (winter, summer, net) separately */
/*    new May 2006                                               */
/*****************************************************************/

void computemassbalprofile(float **matrixprofile,float *massbalbands)
{
    int jj=1, elevationfound = 0;
    int numberloop = 0;   /*to avoid endless loop in elevation belt loop*/
    float elevbelt;    /*lower limit of elevation band to be checked*/

    for (jj=1; jj<=numberbelt; jj++)
        massbalbands[jj] = 0;

    for (i=firstrow; i<=lastrow; i++)         /*for all grid cells of computed area*/
        for (j=firstcol[i]; j<=lastcol[i]; j++) {
            if (griddgmglac[i][j] != nodata) {
                elevationfound = 0;
                numberloop = 0;
                elevbelt = elevbeltmin;   /*start loop with lowest elevation, defined input.dat*/
                jj = 1;     /*index for each elevation belt profile data*/
                /*check for each grid cell elevation if it falls into lowest band, the next etc*/
                /*going through all bands upwards and each belt has its specific indice*/
                while(elevationfound == 0) {
                    numberloop += 1;
                    if((griddgmglac[i][j] >= elevbelt) && (griddgmglac[i][j] < (elevbelt+beltwidth) )) {
                        massbalbands[jj] += matrixprofile[i][j];    /*sum up values of same elevation belt*/
                        /*matrixprofile is WINTERBAL, SUMMERBAL or MASSBALcum*/
                        /*printf(" elevbelt arraymassbal %d %f %f\n",jj, elevbelt, matrixprofile[i][j]);*/
                        elevationfound = 1;
                    } else {
                        elevbelt += beltwidth;  /*check next elevation belt*/
                        jj += 1;
                    }
                    if(numberloop >= 2000) {
                        printf("\n\n exit function computemassbalprofile: check input.txt if defined elevation belts exit\n\n");
                        exit(2);
                    }
                }  /*endwhile*/
            }  /*endif*/
        }  /*endfor*/

    for(jj=1; jj<=numberbelt; jj++)
        massbalbands[jj]=massbalbands[jj]/areabelt[jj];    /*divide sum of massbal by area of elevation belt*/
    /*area determined in initial.c*/
    return;
}


/*****************************************************************/
/* FUNCTION  meanmassbalprofile                                  */
/*     compute the mean mass balance profile of all years        */
/*  years can be defined that are to be excluded from the mean   */
/*  adjust 2 variables below and give the years                  */
/*  called from main after all time steps have been calculation  */
/*    new May 2006                                               */
/*****************************************************************/

void meanmassbalprofile()

{
    char  nameprofile[FNAME_LEN],nameoutmeanprofile[FNAME_LEN];
    FILE  *meanprofile,*outmeanprofile;     /*file to open for each year,  output file*/
    int   jj=1,ii,icol;
    float rowprofile[8];    /*7 columns in profile file*/
    float col1[201],col2[201],col3[201],col4[201],colbw[201],colbs[201],colbn[201];
    float volumebw=0,volumebs=0,volumebn=0;
    int   nyears=0;   /*number of years for averaging mass balance profiles*/
    float npixels=0;     /*number of grid cells per elevation belt, for integration of bal*/
    int   excludeyearsyes = 0;    /*0=No, 1=some years are excluded in calculation of mean mb profiles*/
    /*&&&&&&&&&&&&&&&&&& ADJUST IF NEEDED &&&&&&&&&&&&&&&&&&&*/
    int   numberexcludeyears = 0;            /*years can be excluded from taken the mean*/
    int   excludeyears[2] = {1997,2000};    /*define which years &&&&&&&&&&&&&*/

    nyears = (int)yearend-(int)yearbeg;

    for(i=0; i<=200; i++) { /*up to 200 elevation bands, initialize balances for each band to zero*/
        col1[i] = 0.0;
        col2[i] = 0.0;
        col3[i]= 0.0;
        col4[i] = 0.0;
        colbw[i] = 0.0;
        colbs[i]= 0.0;
        colbn[i]=0.0;
    }

    for(i=(int)yearbeg+1; i<=(int)yearend; i++) { /*for each file (year)*/
        if(numberexcludeyears > 0) {     /*compute number of years to be excluded from taking the mean*/
            for(ii=0; ii<=numberexcludeyears; ii++) {
                if(i == excludeyears[ii]) {
                    excludeyearsyes = 1;   /*set this, to avoid that years are included in summing below*/
                    nyears = nyears-1;     /*one year less to take mean*/
                    fprintf(outcontrol,"Year %d excluded from computation of mean mass balance profile\n",excludeyears[ii]);
                    printf("Year %d excluded from computation of mean mass balance profile\n",excludeyears[ii]);
                }
            }  /*endfor*/
        }  /*endif*/

        if(excludeyearsyes == 0) {   /*if year not to be excluded add up values from this year*/
            /*** open mass balance profile file of one year*/
            if(dat_or_txt == 1)
                sprintf(nameprofile,"massbalprofile%04d.dat",(int)i);
            else
                sprintf(nameprofile,"massbalprofile%04d.txt",(int)i);

            strcpy(dummy,outpath);
            strcat(dummy,nameprofile);
            /*   printf("profiles file: %s\n",dummy);  */

            if ((meanprofile = fopen (dummy,"rt")) == NULL) {
                printf("\n ERROR : Mass balance profile file not found !!!\n %s \n\n",dummy);
                exit(4);
                fclose(outcontrol);
            }  /*ENDIF*/

            /*** read data of whole file*/
            readrestofline(&meanprofile);  /*READ ONE LINE*/
            readrestofline(&meanprofile);  /*READ ONE LINE*/

            for(jj=1; jj<=numberbelt; jj++) { /*for each elevation belt, i.e. for each row*/
                for (icol=1; icol<=7; icol++)     /*read one full row (all columns)*/
                    fscanf(meanprofile,"%f",rowprofile+icol);

                /*   printf("year=%d jj=%d ",i,jj);
                for (icol=1; icol<=7; icol++)
                  printf(" %.2f ",rowprofile[icol]);
                   printf("\n");
                */

                if(i==yearend) {  /*only needed once because this does not change from year to year*/
                    col1[jj] = rowprofile[1];     /*number*/
                    col2[jj] = rowprofile[2];     /*elevation*/
                    col3[jj] = rowprofile[3];     /*number pixels*/
                    col4[jj] = rowprofile[4];     /*area of elevation belt in m2*/
                }  /*endif*/

                colbw[jj] += rowprofile[5];     /*sum up bal from each file, bw*/
                colbs[jj] += rowprofile[6];     /*bs*/
                colbn[jj] += rowprofile[7];     /*bn*/

            }  /*endfor,  next row of same year*/
            closefile(&meanprofile,nameprofile);

        } /*endif,  excludeyearyes*/
    }  /*endfor,  next file/year*/


    /**** WRITE MEAN PROFILES TO OUTPUT *************/
    if(dat_or_txt == 1)
        sprintf(nameoutmeanprofile,"massbalprofilemean%04d%04d.dat",(int)yearbeg+1,(int)yearend);
    else
        sprintf(nameoutmeanprofile,"massbalprofilemean%04d%04d.txt",(int)yearbeg+1,(int)yearend);

    openoutfile(&outmeanprofile,outpath,nameoutmeanprofile);

    fprintf(outmeanprofile,"no  elevation(m)  pixels  area(m2)     bw(m)     bs    bn\n");
    fprintf(outmeanprofile,"Mean mass balance profiles for %d-%d, number of years=%d, %d years exluded from mean,  elevation is mean of band\n",
            (int)yearbeg+1,(int)yearend,nyears,numberexcludeyears);
    /*  printf("number of years for mass balance profiles = %d\n",nyears);  */

    for (jj=1; jj<=numberbelt; jj++) {    /* for all rows (and columns in fwrite command) */
        fprintf(outmeanprofile,"%.0f %8.0f  %0.2f  %0.2f  ",col1[jj],col2[jj],col3[jj],col4[jj]);
        fprintf(outmeanprofile,"   %12.3f %12.3f %12.3f\n",colbw[jj]/nyears,colbs[jj]/nyears,colbn[jj]/nyears);

        volumebw += colbw[jj]/nyears*col3[jj];    /*compute volumes of mean mass balances*/
        volumebs += colbs[jj]/nyears*col3[jj];    /*divide by nyears to get mean massbal of all years*/
        volumebn += colbn[jj]/nyears*col3[jj];
        npixels += col3[jj];
    }
    closefile(&outmeanprofile,nameoutmeanprofile);

    /**** COMPUTE AND PRINT MEAN MASS BALANCE OF ALL YEARS *************/
    printf("\nMEAN MASS BALANCE OVER %d YEARS (%.0f - %.0f)\n",nyears,yearbeg+1,yearend);
    fprintf(outcontrol,"\nMEAN MASS BALANCE OVER %d YEARS (%.0f - %.0f)\n",nyears,yearbeg+1,yearend);

    if (npixels > 0)
        printf("  bw = %.3f   bs = %.3f   bn = %.3f\n", volumebw/npixels, volumebs/npixels, volumebn/npixels);
    else
        printf("  bw = %.3f   bs = %.3f   bn = %.3f\n", volumebw, volumebs, volumebn);
    printf("  number of grid cells of all elevation belts = %.0f",npixels);

    printf("\n years= %d  numberexcludeyears (years excluded from computing mb profiles =%d\n",nyears,numberexcludeyears);

    return;
}



/*================ TIME SERIES OUTPUT FILES (ASCII) =============*/

/*****************************************************************/
/* FUNCTION  areameannull
             SPATIAL MEAN OVER WHOLE GLACIER SET TO ZERO
             Time series Ascii-file                             
             called from main for each time step at beginning   */
/*****************************************************************/

void areameannull()

{
    if(energymethod == 1) {
      areashade    = 0.;
      areaexkorr   = 0.;
      areasolhor   = 0.;
      areadirect   = 0.;
      areadirect2  = 0.;
      areadiffus   = 0.;
      areaalbedo   = 0.;
      areaglobal   = 0.;
      areaswbal    = 0.;
      arealongin   = 0.;
      arealongout  = 0.;
      areanetrad   = 0.;
      areasensible = 0.;
      arealatent   = 0.;
      areaiceheat  = 0.;
      arearain     = 0.;
      areaenbal    = 0.;
      areaabla     = 0.;
      areasurftemp = 0.;
    }
    if(degreedaymethod == 1) {
        areapos = 0.;
        areaddf = 0.;
    }
    
    areamelt = 0.;

  return;
}



/*****************************************************************/
/* FUNCTION  areasum
             SPATIAL MEAN OVER WHOLE GLACIER                     */
/*  called from main before end of grid loop (for each grid cell)*/
/*  summing up values of all grid cells for each times step      */
/*****************************************************************/

void areasum()

{
    if(energymethod ==1 ) {
        if (directfromfile != 1) {
            areashade    += SHADE[i][j];
            areaexkorr   += EXKORR[i][j];
            areasolhor   += strlsumme[i][j]/timestep;
        }

        areadirect   += DIRECT[i][j];

        if(methodglobal == 2) {
            areadirect2  += DIRECT2[i][j];
            areadiffus   += DIFFUS[i][j];
        }

        areaalbedo   += ALBEDO[i][j];
        areaglobal   += GLOBAL[i][j];
        areareflect  += GLOBAL[i][j]*ALBEDO[i][j];
        areaswbal    += SWBAL[i][j];
        arealongin   += LONGIN[i][j];

        if(methodsurftempglac >= 2)    /*ONLY IF VARIABLE, OTHERWISE VALUE OF CLIMATE STATION*/
            arealongout   += LONGOUT[i][j];

        areanetrad   += NETRAD[i][j];
        areasensible += SENSIBLE[i][j];
        arealatent   += LATENT[i][j];
    	areaiceheat  += ICEHEAT[i][j];
        arearain     += rainenergy[i][j];
        areaenbal    += ENBAL[i][j];
        areaabla     += ABLA[i][j];
        areasurftemp += surftemp[i][j];

    } /*if energy method*/

    if(degreedaymethod == 1) {
        areapos += TEMPOS[i][j];
        areaddf += DDFCALC[i][j];
    }

    areamelt += MELT[i][j];     /*only melt is calculated for both cases (detim/debam), not ABLA*/
    
    if(griddgmglac[i][j] != nodata)     /*New 10/2013: sum up only over glacier not possibly larger drainage basin*/
        areamassbal += MASSBALgrid[i][j];    /*massbal in cm*/

    return;
}


/* ======= TEMPORAL SERIES : ASCII-FILES ==============================*/
/******************************************************************/
/* FUNCTION  areameanwrite
             SPATIAL MEAN OVER WHOLE CALCULATED AREA areamean.txt,
             in case of mass balance only over glacier area       */
/*NOTE: ndrain is number pixels over drainage basin area*/
/*      nglac is number pixels over glacier area*/
/*  called from main for each time step after grid loop           */
/*  sum of grid cell values is done in grid cell loop (areasum)   */
/******************************************************************/

void areameanwrite()
{
    float arealongbal=0;

    if(energymethod == 1) {
        if (directfromfile != 1) {
            areashade    = areashade/ndrain;      /*CALCULATION OF SPATIAL MEAN*/
            areaexkorr   = areaexkorr/ndrain;
            areasolhor   = areasolhor/ndrain;
        }

        /*spatial means over all calculated grids*/
        areadirect   = areadirect/ndrain;  /*spatial mean of clear-sky direct radiation*/
        areadirect2  = areadirect2/ndrain;
        areadiffus   = areadiffus/ndrain;
        areaalbedo   = areaalbedo/ndrain;
        areaglobal   = areaglobal/ndrain;
        areareflect  = areareflect/ndrain;
        areaswbal    = areaswbal/ndrain;

        if(methodlongin == 1)      /*longwave incoming radiation spatially constant*/
            arealongin = LWin;      /*LWin calculated once every time step*/
        else
            arealongin   = arealongin/ndrain;

        arealongout  = arealongout/ndrain;
        areanetrad   = areanetrad/ndrain;
        areasensible = areasensible/ndrain;
        arealatent   = arealatent/ndrain;
        areaiceheat  = areaiceheat/ndrain;
        arearain     = arearain/ndrain;
        areaenbal    = areaenbal/ndrain;
        areaabla     = areaabla/ndrain;
        areasurftemp = areasurftemp/ndrain;

        if(methodsurftempglac == 1) {
            arealongbal = arealongin - LWout;
            arealongout = LWout;   /*to write to file below*/
        } else
            arealongbal = arealongin - arealongout;

    } /*if energy*/

    if(degreedaymethod == 1) {
        areapos     = areapos/ndrain;
        areaddf     = areaddf/ndrain;    /*refering to one day:  mm/day */
    }

    areamelt = areamelt/ndrain;   /*melt is integrated over drainage DEM*/
    areamassbal = areamassbal/nglac;  /*New 10/2013: glacierwide mass balance in cm for each time step*/
    areamassbalcum += areamassbal;    /*cumulated glacierwide balances in cm for e.g. GRACE comparison*/
        
    /*midnight, must be next day for continous series of real numbers for grapher plot*/
    jd2 = jd;
    if(timestep != 24)     /*not for daily timesteps*/
        if (jd == (int)jd)   jd2 = jd + 1;

    if(energymethod == 1) {
        fprintf(outarea,"%5.0f %6.2f %6.1f %5.2f %5.2f %8.2f %9.2f %8.2f",
                year,jd2,zeit,areashade,areaexkorr,areasolhor,areadirect,areadirect2);
        fprintf(outarea,"%9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f %9.2f",areadiffus,
                areaglobal,areareflect,areaalbedo,areaswbal,arealongin,arealongout,arealongbal,areanetrad);
        fprintf(outarea,"%9.2f %9.2f %9.2f %10.3f %10.3f %10.3f %10.3f %12.3f %9.2f",
                areasensible,arealatent,areaiceheat,arearain,areaenbal,areamelt,areaabla,areamassbal,areamassbalcum);
        fprintf(outarea,"%9.2f\n",areasurftemp);
    } /*endif*/

    if(degreedaymethod == 1)
        fprintf(outarea,"%5.0f %6.2f %6.1f %5.2f %5.2f %9.2f %9.2f %9.2f %10.3f %10.3f %10.3f %12.3f %9.2f\n",
                year,jd2,zeit,areashade,areaexkorr,areasolhor,areadirect,areamelt,areapos,temp,areaddf,areamassbal,areamassbalcum);

    areamassbal  = 0.;   /*glacierwide mass balance for each time step, must be set to 0*/

    return;
}



/*****************************************************************/
/* FUNCTION  stationoutput
          WRITE INDIVIDUAL GRID RESULTS TO ASCII-OUTPUT-FILE
          Output for every time step for whole period
          called from main every time step
          first line written in startoutasci (initial.c)        */
/*****************************************************************/

void stationoutput()

{
    int    ii,r,c;
    float  notexist=-99;     /*to produce missing value in file for shading,exkor and solhor etc*/
    /*all variables that are not computed in specific run*/
    float  dirhor,dirslope;
//    float  lwincoming,lwoutgoing,snowcover;
    float  lwoutgoing,snowcover;

    for (ii=1; ii<=outgridnumber; ii++) { /*for each station or location*/
        r = stnrow[ii];
        c= stncol[ii];    /*nur um Schreibarbeit zu sparen*/
        /*stnrow/col : doesn't have to the same as rowclim/colclim
          output is written for stnrow/stncol but measured data from rowclim/colclim is included*/


        /*midnight, must be next day for continous real numbers for grapher plot*/
        jd2 = jd;
        if(timestep != 24)     /*not for daily timesteps*/
            if (jd == (int)jd)   jd2 = jd + 1;

		if(methodsurftempglac == 4) jd2 = jd2-((0.5*timestep)/24.);
		/*CHR shifts time with half a full time step since the values represent the average over previous timestep*/
        /*--------------------------------------------------------------------------------------*/
        /*depending on whether direct radiation (DIRECT) is calculated by the program or
          read from existing files, in case of the climate station grid it refers to the
          actual slope in the latter case if refers to an assumed horizontal surface*/

        dirhor = directstationhoriz; /*direct rad if grid was horizontal, corrected for shading*/
        dirslope = DIRECT[r][c];     /*direct rad corrected for slope, aspect and shading*/

        if ((r==rowclim) && (c==colclim)) {
            if (directfromfile == 1)
                dirslope = 0.0;      /*for climate station grid slope was assumed to be horizontal
                              -> doesn't exist --> 0=missing value*/
        } else /*for other grids than climate station, direct for horiz. surface not available*/
            dirhor = 0.0;

        /*--------------------------------------------------------------------------------------*/
        if(energymethod == 1)

        {
            fprintf(outgrid[ii],"%.0f %6.2f %6.1f ",year,jd2,zeit);

            if(methodsurftempglac == 1)   /*LONGWAVE OUTCOMING RADIATION CONSTANT IN SPACE*/
                lwoutgoing = LWout;
            else                     /*VARIABLE IN SPACE, THEREFORE ARRAY*/
                lwoutgoing = LONGOUT[r][c];


            if(directfromfile == 0) {   /*shading etc calculated*/
                fprintf(outgrid[ii],"%6.2f",SHADE[r][c]);    /*column 4*/
                fprintf(outgrid[ii],"%6.2f",EXKORR[r][c]);
                fprintf(outgrid[ii],"%8.2f",strlsumme[r][c]/timestep);
            } else  /*DIRECT READ FROM FILES, ARRAYS DO NOT EXIT, WRITE 0 */
                fprintf(outgrid[ii],"%6.2f %6.2f %8.2f",notexist,notexist,notexist);

            fprintf(outgrid[ii],"%11.4f",DIRECT[r][c]);

            if(methodglobal == 1)      /*GLOBAL RADIATION INTERPOLATED*/
                fprintf(outgrid[ii],"%8.2f %8.2f",notexist,notexist);
            else {                     /*DIRECT AND DIFFUSE RAD SEPARATED*/
                fprintf(outgrid[ii],"%11.4f",DIRECT2[r][c]);
                fprintf(outgrid[ii],"%11.4f",DIFFUS[r][c]);
            }

            if ((snetfromobsyes == 1) && (calcgridyes == 2)) /*2011: added, if only climate station calculated and obs available*/
                fprintf(outgrid[ii],"%11.4f",glob);  /*reflected radiation*/
            else
                fprintf(outgrid[ii],"%11.4f",GLOBAL[r][c]);     /*column 10*/


            if ((snetfromobsyes == 1) && (calcgridyes == 2))   /*CHR added, if only climate station calculated and meas available then possible*/
                /*second condition added 1/2011*/
                fprintf(outgrid[ii],"%11.4f",ref);  /*reflected radiation*/
            else
                fprintf(outgrid[ii],"%11.4f",GLOBAL[r][c]*ALBEDO[r][c]);  /*reflected radiation*/

            fprintf(outgrid[ii],"%8.5f",ALBEDO[r][c]);
            fprintf(outgrid[ii],"%11.4f",SWBAL[r][c]);

            fprintf(outgrid[ii],"%11.4f",LONGIN[r][c]);
            fprintf(outgrid[ii],"%11.4f",lwoutgoing);
            fprintf(outgrid[ii],"%11.4f",LONGIN[r][c]-lwoutgoing);

            fprintf(outgrid[ii],"%11.4f",NETRAD[r][c]);     /*column 17*/
            fprintf(outgrid[ii],"%11.4f",SENSIBLE[r][c]);
            fprintf(outgrid[ii],"%11.4f",LATENT[r][c]);

            /*compute ice heat flux in case subsurface model is run,  2/2011*/
            /*convert Runoff in mm into energy*/
            /*        ICEHEAT[r][c] = RUNOFF[r][c]*Lf/(60*60*timestep) - rainenergy[r][c];  */

            fprintf(outgrid[ii],"%11.4f",ICEHEAT[r][c]);
            fprintf(outgrid[ii],"%11.4f",rainenergy[r][c]);
            fprintf(outgrid[ii],"%11.4f",ENBAL[r][c]);
            if (methodsurftempglac == 4) {
              fprintf(outgrid[ii],"%11.4f",meltenergy[r][c]);
            }
            fprintf(outgrid[ii],"%11.5f",RAIN[r][c]);
            fprintf(outgrid[ii],"%11.5f",MELT[r][c]);
            fprintf(outgrid[ii],"%11.5f",ABLA[r][c]);       /*column 24*/
            fprintf(outgrid[ii],"%11.3f",MASSBALgrid[r][c]);       /*NEW 10/2013*/
            fprintf(outgrid[ii],"%11.3f",MASSBALcum_all[r][c]);       /*NEW 10/2013*/

            fprintf(outgrid[ii],"%9.4f",surftemp[r][c]);   /*column 25*/

            /* Other variable*/
            if(directfromfile == 0)     /*shading etc calculated*/
                fprintf(outgrid[ii],"%7.2f",exkorstationhoriz);
            else    /*DIRECT READ FROM FILES, ARRAYS DO NOT EXIT, WRITE 0 */
                fprintf(outgrid[ii],"%7.2f",notexist);

            if(methodglobal == 1)      /*GLOBAL RADIATION INTERPOLATED*/
                fprintf(outgrid[ii],"%6.2f",ratio);
            else                       /*DIRECT AND DIFFUSE SEPARATED*/
                fprintf(outgrid[ii],"%6.2f",ratiodir2dir);

            if(directfromfile == 0)     /*shading etc calculated*/
                fprintf(outgrid[ii],"%8.2f",dirhor);
            else
                fprintf(outgrid[ii],"%8.2f",notexist);

            /*print snow water equivalent to file, only possible if run with initial snow w.eq.*/
            /*if not print a missing value, so that the number/order of columns is always the same*/
            if(methodinisnow == 2)
                snowcover = SNOW[r][c];
            else
                snowcover = notexist;
            fprintf(outgrid[ii],"%11.4f",snowcover);

            /*surface elevation change - convert w.eq. to surface lowering for comparison SR50 measurements*/
            i = r;
            j = c;
            if (methodsurftempglac != 4) {
                // (Last) parameter of snowlayermsnow and meltlayermice is only used when methodsurftempglac==4
                snowlayermsnow(i, j, -1);
                meltlayermice(-1.);
            }
            fprintf(outgrid[ii],"%11.5f",snowlayer[r][c]);

            /*output mass balance per time step*/
            if (methodsurftempglac != 4) {
                if (calcgridyes == 1) {   /*whole grid computed*/
                    i=r;    /*snowprec must be computed again, since not stored in array*/
                    j=c;
                    precipinterpol();
                }
                fprintf(outgrid[ii],"%10.5f",snowprec-ABLA[r][c]); /*CHR mass bal added*/
            }
            if (methodsurftempglac == 4) { /*CHR mass bal added*/
                fprintf(outgrid[ii],"%10.5f",MBsum[r][c]/10); /*CHR mass bal added, convert mm to cm*/
              }

            /*only if snow model is run*/
            if (methodsurftempglac == 4) {
                fprintf(outgrid[ii],"%11.5f",sumSNOWprec[r][c]);       /*mm we*/
                fprintf(outgrid[ii],"%11.5f",SUBLIMATION[r][c]);       /*mm we*/
                fprintf(outgrid[ii],"%10.5f",RUNOFF[r][c]);  /*in mm we*/
                fprintf(outgrid[ii],"%10.5f",watercontent[r][c]); /* in mm we */
                fprintf(outgrid[ii],"%15.5f",deltaWATER[r][c]); /* in mm we */
                fprintf(outgrid[ii],"%10.5f",deltaREFREEZE[r][c]); /* in mm we */
                fprintf(outgrid[ii],"%10.5f",superice[r][c]/densice); /* in m thickness*/
                fprintf(outgrid[ii],"%10.5f",slushthickness[r][c]); /*slush layer thickness in m snow */
                fprintf(outgrid[ii],"%10.5f",surfacewater[r][c]); /* in mm we */
                fprintf(outgrid[ii],"%12.3f",coldcontentsnow[r][c]);
                fprintf(outgrid[ii],"%12.3f",coldcontentice[r][c]);
            }

            if(outglobnet[ii] == 1) {       /*included measured data*/
                fprintf(outgrid[ii],"%8.2f",(glob));
                fprintf(outgrid[ii],"%8.2f",(glob-ref));   /*SWBAL measured*/
                fprintf(outgrid[ii],"%8.2f\n",net);
            } else
                fprintf(outgrid[ii],"\n");

        } /*if energymethod*/


        if(degreedaymethod == 1) {   /*TEMPERATURE INDEX METHOD*/
            if(directfromfile != 1) {  /*CLEAR-SKY RADIATION IS CALCLULATED*/
                fprintf(outgrid[ii],"%.0f %6.2f%6.1f %5.2f %5.2f %8.2f %8.2f %7.2f %7.2f %7.2f%7.2f  %10.3f  %12.3f\n",
                        year,jd2,zeit,SHADE[r][c],EXKORR[r][c],strlsumme[r][c]/timestep,
                        DIRECT[r][c],MELT[r][c],TEMPOS[r][c],temp,DDFCALC[r][c],MASSBALgrid[r][c],MASSBALcum_all[r][c]);
            } else   /*CLEAR-SKY RADIATION IS READ FROM FILES*/
                fprintf(outgrid[ii],"%.0f %6.2f%6.1f %5.2f %5.2f %5.2f %8.2f %8.2f %8.2f %8.2f %9.3f %10.3f %12.3f\n",
                        year,jd2,zeit,notexist,notexist,notexist,DIRECT[r][c],MELT[r][c],TEMPOS[r][c],temp,DDFCALC[r][c],MASSBALgrid[r][c],MASSBALcum_all[r][c]);
        }  /*endif degreeday*/

    }  /*for*/

    return;
}


/*******************************************************************/
/* FUNCTION  writemeltstakes                                       */
/*           WRITE 2 FILES: CUMULATIVE MELTING AND                 */
/*           CUMULATIVE MASS BALANCE OF SEVERAL LOCATIONS INTO ONE FILE  */
/*           called from main every time step after grid loop      */
/*******************************************************************/

void writemeltstakes()

{
    /* indexing is being handled be global variables, i'm assuming these were there before*/
    //int rowout,colout;    /*row and col to be written to file*/  /

    /*MASSBALANCE (SNOW PRECIP - ABLATION) OUTPUT IN CM*/
    /*midnight, must be next day for continous real numbers for grapher plot*/
    jd2 = jd;
    if(timestep != 24)     /*not for daily timesteps*/
        if (jd == (int)jd)   jd2 = jd + 1;

    fprintf(outmassbalstakes,"%.0f %7.2f %5.1f",year,jd2,zeit);    /*first 3 columns*/
    if (methodsurftempglac == 4)
        fprintf(outmassbalstakes2,"%.0f %7.2f %5.1f",year,jd2,zeit);    /*first 3 columns*/
    for(i=1; i<=maxmeltstakes; i++) {
        /*if (methodsurftempglac != 4)*/
              /*10/2013: changed from MASSBALcum to MASSBALcum_all, because was wrong*/
        fprintf(outmassbalstakes,"%10.3f",plusminus*MASSBALcum_all[meltoutrow[i]][meltoutcol[i]]);
        if (methodsurftempglac == 4)
            fprintf(outmassbalstakes2,"%10.3f",plusminus*MASSBALcumstake[meltoutrow[i]][meltoutcol[i]]);
    }
    fprintf(outmassbalstakes,"\n");     /*NEXT ROW = NEXT TIME STEP*/
    if (methodsurftempglac == 4)
        fprintf(outmassbalstakes2,"\n");     /*NEXT ROW = NEXT TIME STEP*/

    /*CUMULATIVE MELT IN cm, MELT IS DEFINED POSITIVE*/
    /*the array used for the grid output for the entire period are used because
      these sum up all all values over the entire period*/
    if(energymethod == 1) {     /*must be ABLA, because it includes mass loss sublimation*/
        fprintf(outmeltstakes,"%.0f %6.2f %5.1f",year,jd2,zeit);
        for(i=1; i<=maxmeltstakes; i++)
            /*     fprintf(outmeltstakes,"%6.2f",ABLA[meltoutrow[i]][meltoutcol[i]]); */
            fprintf(outmeltstakes,"%10.3f",meanABLAall[meltoutrow[i]][meltoutcol[i]]/10);
        /*convert from mm to cm for output*/
        fprintf(outmeltstakes,"\n");
    }


    if(degreedaymethod == 1) {  /*must be MELT, because ABLA doesn't exist*/
        fprintf(outmeltstakes,"%.0f %6.2f %5.1f",year,jd2,zeit);    /*first 3 columns*/
        for(i=1; i<=maxmeltstakes; i++)
            /*   fprintf(outmeltstakes,"%7.3f",MELT[meltoutrow[i]][meltoutcol[i]]);  */
            fprintf(outmeltstakes,"%10.3f",meanMELTall[meltoutrow[i]][meltoutcol[i]]/10);
        /*convert from mm to cm for output*/
        fprintf(outmeltstakes,"\n");     /*NEXT ROW = NEXT TIME STEP*/
    }

    return;
}


/************************************************************************/
/*  FUNCTION:  percentsnowfree                                          */
/*     goes through snow cover file and counts how may pixels snow free */
/*     writes one row to Ascii-file every midnight if snowfreeyes = 1   */
/*     can also be used if methodinisnow=1 !                            */
/************************************************************************/

void percentsnowfree() {
    int     i,j;
    float   allcells=0;   /*number of grid cells*/
    float   cellssnowfree=0;

    for (i=1; i<=nrows; i++) {       /*ueber alle Reihen*/
        for (j=1; j<=ncols; j++) {    /*ueber alle Reihen*/
            if (surface[i][j] != nodata) {   /*only area to be calculated*/
                allcells += 1.0;        /*number of grids*/
                if (surface[i][j] >= 2)     /*1=ice, 2=slush, 3=ice 4=rock*/
                    cellssnowfree += 1.0;
            }  /*endif*/

        }  /*for col*/
    }   /*for row*/

    fprintf(outsnowfree,"%5.0f %6.2f %4.0f   %10.0f %10.0f    %10.3f %10.3f\n",
            year,jd,zeit,allcells,cellssnowfree,cellssnowfree/allcells*100,
            100-cellssnowfree/allcells*100);

    return;
}


/************************************************************************/
/*  FUNCTION:  writeperformance                                         */
/*     computes and writes model performance to file                    */
/*     one line with various performance values, missing values if a value does not exist */
/*     called once at the end of main                                   */
/*     New 10/2013                                                      */
/************************************************************************/

void writeperformance() 
{  float missvalQ  =-9999;   /*missing value in case discharge matrix is not computed */
   float diffvolume=-9999;
   float r2value,r2lnvalue;   /*new variables because array only exists if discharge data is available*/

   if(calcgridyes == 1)  /*only if entire grid is calculated, not if only AWS station computed*/ 
    { 
     strcpy(dummy,outpath);
     strcat(dummy,"modelperformance.txt");

        if ((outperformance = fopen(dummy,"wt")) == NULL)  {
            printf("\n Error in opening output model performance file\n (File initial.c): %s\n\n",dummy);
            exit(4);
        }  /*ENDIF*/

        fprintf(outperformance,"R2 is Nash-Sutcliffe efficiency criterion for discharge Q (-infinity to 1); discharge volumes are in 100,000 m3\n"); 
        fprintf(outperformance,"nsteps = number modeled time steps, nstepsdis = number of time steps with valid discharge data - must be the same for using the volume difference as criterion for calibration\n");

      if (disyes != 1) {    /*if discharge r2 not computed, set to missing value*/
         r2value    = missvalQ;   r2lnvalue  = missvalQ;
         volumesim  = missvalQ;   volumemeas = missvalQ;
         diffvolume = missvalQ;   nstepsdis  = missvalQ;
      }
      else     /*discharge data exists and r2 could be calculated*/
      {  if(nsteps == nstepsdis)   /*volume difference only makes sense if number of time steps are the same*/
            diffvolume=volumesim-volumemeas;    /*differenct between total discharge volume over entire period*/
         r2value   = r2[1][1];     /*array only exists if discharge data is available, r2 computed even if nstepsdis < nsteps*/
         r2lnvalue = r2ln[1][1];
      }

      /*cumulative mass balance over entire modeling period written to output for comparison with geodetic longer-term balance, converted to m*/
     fprintf(outperformance,"Q_R2\t%.6f\n", r2value);
     fprintf(outperformance,"Q_lnR2\t%.6f\n", r2lnvalue);
     fprintf(outperformance,"Qvolumesim\t%.6f\n", volumesim);
     fprintf(outperformance,"Qvolumemeas\t%.6f\n", volumemeas);
     fprintf(outperformance,"Difference(sim-meas)\t%.6f\n", diffvolume);
     fprintf(outperformance,"nsteps\t%i\n", nsteps);
     fprintf(outperformance,"nstepsdis\t%i\n", nstepsdis);
     fprintf(outperformance, "totalglacierwidemassbalance(m)\t%.6f",areamassbalcum/100); 
     fclose(outperformance);
    }  /*endif*/
    
    return;
}


/********************************************************************/
/*  FUNCTION  writemodelmeaspointbalances                           */
/*       makes file with measured and modeled point balances (in m) */
/*          called from main once at the end                        */
/*   A file with measured point balances is read and a new file created which
     is identical but adds another column with the modeled balances for each
     stake observation. Modeled balances are computed from cumulated
     balance file "cummassbal.txt" which has the cumulated time series*/
/* New October 2013, R. Hock*/
/********************************************************************/

void writemodelmeaspointbalances() 
{
   int write_output_yes = 1;    /*controls whether or not output file with comparison is generated*/
      /*avoids another row in input.txt, if file exists compute performance, if not don't do anything*/
   FILE *outpointbal=NULL;   /*output file with modeled-measured point balances to be created here*/
   FILE *modelcummassbal=NULL;  /*model output file with modeled cumulative point balances*/
   FILE *measpointbal=NULL;     /*file with measured point balances */

   float **cummassbal;   /*array to read all data of modeled cummassbal.txt file*/
   float **measuredpointbal;  /*array to read measured point balance file*/
   float readvalue;    /*for reading numbers from file and put into array*/
   int numberdatapoints = 5000;   /*max number of rows in measured point balance file*/
   int n_col_meas=8;   /*number of columns in measured stake data file (measuredpointbalances.txt)*/
   int n_meas=0;    /*number of rows (stake locations) in measured file; computed by counting when reading file*/
   int rowmeas,rowmod,nnn;     /*indices to go through arrays*/
   float massbal_mod_start,massbal_mod_end;   /*modeled cumulative values for start and end date of corresponding measurements*/
   float datediff;   /*difference between start and end date, to check for errors in measured point balance file*/
   int startfound_yes=0,endfound_yes=0;
   int status;
   int getoutfunctionyes=0;

/*===================================================================*/
/*==== OPEN MEASURED POINT BALANCE FILE AND READ FILE INTO ARRAY ====*/
  /* Format: x,y-coord,elevation,B,startyear,startday,endyear,endday */
    strcpy(dummy,inpath);
    strcat(dummy,"measuredpointbalances.txt");   /*Mass balance output is in m*/
    if ((measpointbal = fopen(dummy,"rt")) == NULL)
     { write_output_yes = 0;
       printf("\n\n No point measurements to compare to modeled point balances\n");
       printf("    measuredpointbalances.txt could not be opened (function writemodelmeaspointbalances)\n");
     }

    if(maxmeltstakes == 0)
      write_output_yes = 0;   
 
 if(write_output_yes == 1)   /*only if measured point balance file exists, make output file*/   
 {	 	
/*=======================================================*/
/*===== OPEN OUTPUT FILE ================================*/
    strcpy(dummy,outpath);
    strcat(dummy,"pointbalances.txt");
        if ((outpointbal = fopen(dummy,"wt")) == NULL)  {
            printf("\n Error in opening output file\n (File initial.c): %s\n\n",dummy);
            exit(4);
        }  /*ENDIF*/
    fprintf(outpointbal,"X-coord \t Y-coord\t Elevation(m) MeasMassbal(m)  StartYear StartDay EndYear  EndDay \t ModeledMassbal(m)\n");
    fprintf(outpointbal,"First 8 columns are same as read from measuredpointbalances.txt, last columnn is computed from model output cummassbal.txt\n");

/*==============================================================*/
/*====== READ MEASURED POINT BALANCE FILE INTO ARRAY ===========*/ 
      readrestofline(&measpointbal);   /*read over 2 header lines*/
      readrestofline(&measpointbal);
           /*define 2-D array to read all measured data*/  
 /*---- INITIALIZE ARRAY WITH MEASURED DATA---------*/ 
      measuredpointbal = matrixreserv(1,numberdatapoints,1,n_col_meas);   /*file has n_col_meas columns*/
      for(i=1;i<=numberdatapoints;i++)    /*initialize array to zero*/
      {  for(j=1;j<=n_col_meas;j++)
			measuredpointbal[i][j] = -9999;
	  }		
			
 /*read measured data into array*/
      i=1; j=1; n_meas=0;     /*i=row, j=column, n_meas=number of rows (stakes)*/      
	  while((status=fscanf(measpointbal, "%f", &readvalue)) != EOF)   /*read one number after another*/
      {         
        measuredpointbal[i][j]=readvalue;   /*put numbers in array as they are*/

        if(j==n_col_meas)   /*make new line, start with col 1 again, measured file has n_col_meas columns*/
        { j=0;
          i=i+1;   /*next line*/
          n_meas+=1;    /*find total number of rows (data points) in measured file*/ 
          if(status!=EOF) readrestofline(&measpointbal);     /* READ LINE BREAK */
        }     
        j=j+1;   /*next column same row*/
      }  /*endwhile*/

 /* ---- check for each measured point balance if it is outside simulation period ----- */
 /*      if so, program does not exit but put a warning on screen and into modellog.txt */
     for(i=1;i<=n_meas;i++)    /*for each row of measured file, col5=startyear*/
     {  if(((measuredpointbal[i][5]*365+measuredpointbal[i][6]) < (yearbeg*365+jdbeg)) || ((measuredpointbal[i][5]*365+measuredpointbal[i][6]) > (yearend*365+jdend)))
        {  printf("\n==================== !!!! WARNING !!!!!=======================================================");
          printf("\n --- WARNING: start date of stake (row) %d in measuredpointbalance.txt is outside simulation period",i);   
          fprintf(outcontrol," \n --- WARNING: start date of stake (row) %d in measuredpointbalance.txt is outside simulation period",i);
 
        } 
        if(((measuredpointbal[i][7]*365+measuredpointbal[i][8]) < (yearbeg*365+jdbeg)) || ((measuredpointbal[i][7]*365+measuredpointbal[i][8]) > (yearend*365+jdend)))
        {  printf("\n==================== !!!! WARNING   !!!!!=====================================================");     
           printf("\n --- WARNING: end date of stake (row) %d in measuredpointbalance.txt is outside simulation period",i);

          fprintf(outcontrol," \n --- WARNING: start date of stake (row) %d in measuredpointbalance.txt is outside simulation period",i);
        }
     }
       	 
 /* ---- check in measured stake file for each row if end date is after start date -----------*/
     for(i=1;i<=numberdatapoints;i++)    /*for each row in measured file; col 5=startyear, col 6=startday*/
      {  if((measuredpointbal[i][5] != -9999) && (measuredpointbal[i][7] !=-9999))    /*possible since array has 5000 lines*/  
         { datediff = (measuredpointbal[i][7]*365+measuredpointbal[i][8]) - (measuredpointbal[i][5]*365+measuredpointbal[i][6]);
           if(datediff <=0)     /*datediff is differences in days*/
           { printf("\n\n ==================== !!!!!!!!!============================\n");
             printf("  ERROR in file measuredpointbalances.txt in row %d\n",i);
             printf("   The end date must be after the start date of the balance observations.\n   No point balance file written to output.\n");
 			 printf("==================== !!!!!!!!! ============================\n\n");
             getoutfunctionyes=1;     /*avoid exist but just don't do anything anymore in this function*/
           }
         }  /*endif*/
      }   /*endfor*/
 
    if(n_meas != maxmeltstakes)
     { printf("\n Number of stakes in measuredpointbalances.txt is larger/lower (=%d) than number of stakes computed in model run (%d)",n_meas,maxmeltstakes);
       printf(" (maxmeltstakes in input.txt). It must be equal. No point balance file written to output.\n\n");
       getoutfunctionyes=1;     /*avoid exist but just don't do anything anymore in this function*/
     }
     
  if(getoutfunctionyes==0)  /*only if number of stakes in measured file is equal to number of stakes written to output and no date errors*/
  {    
    printf("\n\nModeled POINT BALANCES written to file (pointbalances.txt):\n   Number of stakes in measured file = %d\n",n_meas);
    printf("   Number of point locations written to output (maxmeltstakes) = %d\n",maxmeltstakes);

/*===================================================================================================*/
/*====== OPEN MODELED CUMULATIVE MASS BALANCE FILE CREATED BY THE MODEL AND PUT DATA INTO ARRAY======*/
   strcpy(dummy,outpath);
        strcat(dummy,"cummassbal.txt");        

        if ((modelcummassbal = fopen(dummy,"rt")) == NULL) {
            printf("\n Error in opening output file 'cummassbal.txt'\n (File initial.c): %s\n\n",dummy);
            exit(4);
        }  /*ENDIF*/
   		readrestofline(&modelcummassbal);   /*read 2 header lines*/
   		readrestofline(&modelcummassbal);

  /*---- CREATE ARRAY FOR DATA OF MODELED CUMULATIVE POINT BALANCE FILES AND READ ALL DATA-----------*/
     /*nstep is number of time steps, i.e. rows for array, col+3 because of year, day, time columns*/
       cummassbal = matrixreserv(1,nsteps,1,maxmeltstakes+3);
    
 	    for(i=1;i<=nsteps;i++)   /*for each row*/
 		{  for(j=1;j<=maxmeltstakes+3;j++)     /*plus 3 because of 3 date columns*/
 		   {  fscanf(modelcummassbal, "%f", &readvalue);
              cummassbal[i][j]=readvalue;   /*put numbers in array as they are*/
           }  /*endfor*/   	
        }   /*end for each row*/
        
/*======================================================================================*/
/* ====== COMPUTE MODELED POINT BALANCE FOR TIME PERIOD OF MEASURED BALANCE ============*/
  /* go through each measured stake (row) and check the corresponding time series of modeled stake (column) for start and end date*/
  /* Note that each measured stake is in one row, while each modeled stake is a column*/
 
      for(rowmeas=1;rowmeas<=n_meas;rowmeas++)   /*for each row (stake) of the measured file, column 5 is start year, 6 start day*/
      { startfound_yes = 0; endfound_yes = 0;  /*set to zero for each stake (row of measured file)*/
        massbal_mod_start=-9999;  massbal_mod_end = -9999;  /*initialize in case dates do not exist in modeled file*/
    /*go through whole modeled time series of that stake and find start/end data of stakes point measurement*/
        for(rowmod=1;rowmod<=nsteps;rowmod++)    /*for each modeled time step (column) of that stake*/
        {  
          if(startfound_yes==0)   /*start date not found yet*/
          { 
           if((measuredpointbal[rowmeas][5] == cummassbal[rowmod][1]) && (measuredpointbal[rowmeas][6] == cummassbal[rowmod][2]))
             { massbal_mod_start = cummassbal[rowmod][rowmeas+3]/100;   /*Note: rowmeas corresponds to the column in the modeled file*/
                   /*divide by 100 to convert cm into m*/
      		   startfound_yes = 1;	   
   /*printf(" dates found: massbal start= %.2f stake number (rowmeas)= %d  %.0f %.0f\n",massbal_mod_start,rowmeas,cummassbal[rowmod][1],cummassbal[rowmod][2]);*/   		   
      		 }  /*endif*/
		  }  /*start day not found yet*/
		  
		  if(endfound_yes==0)   /*start date not found yet; col 7 (8) = end year (day) in measured file, in cummassbal.txt col 1 is always year*/
		  {	 if((measuredpointbal[rowmeas][7] == cummassbal[rowmod][1]) && (measuredpointbal[rowmeas][8] == cummassbal[rowmod][2]))
             { massbal_mod_end = cummassbal[rowmod][rowmeas+3]/100;   /*Note: rowmeas corresponds to the column in the modeled file*/
      		   endfound_yes = 1;
  /*printf("    massbal end = %.2f stake number (rowmeas)= %d %.0f %.0f\n",massbal_mod_end,rowmeas,cummassbal[rowmod][1],cummassbal[rowmod][2]);*/
             }  /*endif*/
  		  }  /*end day not found yet*/    
	    }  /*for rowmod: each modeled time step of same stake*/
	    
  /* ------ write to new output file, each stake is one row -------- */
        for(nnn=1;nnn<=n_col_meas;nnn++)    /*print all columns that were in measured data file*/
         { fprintf(outpointbal,"%.3f\t",measuredpointbal[rowmeas][nnn]);
         }      /*print the corresponding modeled balance*/
         fprintf(outpointbal,"  %.4f\n",massbal_mod_end-massbal_mod_start);

      }  /*end for rowmeas: each stake (row) of measured file*/
    
 /* ------ free maxtrices and close files ---------*/
      freematrix(cummassbal,1,nsteps,1,maxmeltstakes+3);
      if(modelcummassbal) {
        fclose(modelcummassbal);
        modelcummassbal = NULL;
      }
    
    }  /*endif getoutfunctionyes*/
    /*has been reserved even if no output created, therefore outside loop above*/
     freematrix(measuredpointbal,1,numberdatapoints,1,n_col_meas);
      if(outpointbal) {
        fclose(outpointbal);
        outpointbal = NULL;
      }
   
   }  /* if write_output_yes = 1 */
   
 return;
}
