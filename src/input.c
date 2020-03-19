/***********************************************************************
 * Copyright 1997-2012 Regine Hock
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
/*********************************************************/
/*   FILE          input.c                        ********/
/*   FUNCTION      input_read                     ********/
/*   READ INPUT FROM CONTROLING INPUT FILE : 'input.txt' */
/*   FILE NAMES, GRIDSIZE ETC.                      ******/
/***  31.3.1997, Last update 2 Sep 2015  */
/*********************************************************/


#include "input.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "initial.h"

#include "variabex.h"      /* all global VARIABLES */


void input_read()

{

    FILE  *in=NULL;
    char  filenamein[FNAME_LEN];       /*name of Steuerfile*/
    char  outcontrolname[PFNAME_LEN];
    int   c;    /*to read file character by character and write to res.txt*/
    int methodsurftemglac_change = 0;

    namedgm       = (char*)calloc(FNAME_LEN,sizeof(char));     /*RESERVE STORAGE*/
    namedgmdrain  = (char*)calloc(FNAME_LEN,sizeof(char));
    namedgmglac   = (char*)calloc(FNAME_LEN,sizeof(char));
    nameklima     = (char*)calloc(FNAME_LEN,sizeof(char));
    namedgmslope  = (char*)calloc(FNAME_LEN,sizeof(char));
    namedgmaspect = (char*)calloc(FNAME_LEN,sizeof(char));
    namedgmskyview  = (char*)calloc(FNAME_LEN,sizeof(char));
    nameinitialsnow = (char*)calloc(FNAME_LEN,sizeof(char));
    namedgmthickness = (char*)calloc(FNAME_LEN,sizeof(char));
    namedatesmassbal= (char*)calloc(FNAME_LEN,sizeof(char));

    strcpy(filenamein,"input.txt");    /* $$$$$$ NAME NAMELIST-FILE $$$$$ */

    if ((in = fopen (filenamein,"rt")) == NULL) {             /*OPEN FILE*/
        printf("\n Input-File 'input.txt' not found \n\n");
        printf("       File input.c ");
        exit(1);
    }

    printf("\n\n\n\n\n\n ***********INPUTFILE : input.txt *****************\n");

    readrestofline(&in);
    readrestofline(&in);  /*read first 2 comment rows*/
    fscanf(in,"%d",&daysscreenoutput);
    readrestofline(&in);
    if (daysscreenoutput < 1) {
        printf("\n\ndaysscreenoutput=%d wrong in input.txt, must be >= 1\n\n",daysscreenoutput);
        exit(1);
    }

    /*output path must be read before file modellog.txt is opened */
    fscanf(in,"%s",inpath);		readrestofline(&in);
    fscanf(in,"%s",outpath);	readrestofline(&in);
    printf("inpath = %s\noutpath = %s\n",inpath,outpath);

    /* --------------------------------------------------------------*/
    /* OPEN CONTROL OUTPUT-FILE:   modellog.txt
         OUTPUT OF READ DATA AND INTERMEDIATE RESULTS IN ORDER TO CONTROL RUNS*/
    /* --------------------------------------------------------------*/

    strcpy(outcontrolname,outpath);
    strcpy(dummy,"modellog.txt");
    strcat(outcontrolname,dummy);
    printf("Name of model log file = %s\n",outcontrolname);

    if ((outcontrol = fopen(outcontrolname,"wt")) == NULL) {
        printf("\n\n ERROR opening controlling output-file: %s\n",outcontrolname);
        printf("   Output path specified in input.txt may not exist !\n\n");
        exit(20);
    }

    if(energymethod == 1)     fprintf(outcontrol,"ENERGY BALANCE MODEL RUN\n\n");
    if(degreedaymethod == 1)  fprintf(outcontrol,"TEMPERATURE INDEX MODEL RUN\n\n");

    fprintf(outcontrol,"OUTPUT VARIABLES IN input.txt ARE RESET IN CASES CERTAIN COMBINATIONS ARE NOT POSSIBLE\n");
    fprintf(outcontrol,"---- LIST OF SUCH CHANGES DONE BY THE PROGRAM: \n\n");

    /****** READ JULIANS DAYS TO BE CALCULATED ************/
    fscanf(in,"%f%f",&jdbeg,&yearbeg);		readrestofline(&in);
    fscanf(in,"%f%f",&jdend,&yearend);		readrestofline(&in);

    if (yearend < yearbeg) {
        printf("\n\n start or last year (yearbeg or yearend) wrong in input.txt\n\n");
        exit(1);
    }
    printf(" jdbeg=%4.0f yearbeg=%5.0f \t jdend=%4.0f yearend=%5.0f\n",jdbeg,yearbeg,jdend,yearend);

    /**** IF DISCHARGE TO BE CALCULATED *****************************/
    fscanf(in,"%d",&disyes);   readrestofline(&in);
    if ((disyes != 1) && (disyes != 0) && (disyes != 2)) {
        printf("\n\n  ERROR in input.txt: disyes=%d; must be 0, 1 or 2\n\n",disyes);
        exit(3);
    }
    if (disyes >= 1)
        printf("disyes=1:  DISCHARGE CALCULATION YES");
    else
        printf("disyes=0:  DISCHARGE CALCULATION NO");

    fscanf(in,"%d",&calcgridyes);
    readrestofline(&in);
    if ((calcgridyes != 1) && (calcgridyes != 2) ) {
        printf("\n\n ERROR in input.txt: calcgridyes=%3d must be 1 or 2 \n\n",calcgridyes);
        exit(3);
    }
    if (calcgridyes == 1) { /*compute whole grid*/
        snetfromobsyes = 0;  /*don't use AWS glob, ref for grid, but compute albedo, global*/
        fprintf(outcontrol,"  snetfromobsyes set to 0\n");
    }

    /******************* MODEL OUTPUT PARAMETERS ************************************/
    readrestofline(&in);
    fscanf(in,"%d",&maxmeltstakes);	 readrestofline(&in);
      printf("\n maxmeltstakes=%d\n",maxmeltstakes);
    fscanf(in,"%f",&plusminus);	  readrestofline(&in);
    if((plusminus != 1) && (plusminus != -1)) {
        printf("\n\n ERROR in inputdat variable: plusminus=%f) \n\n",plusminus);
        exit(3);
    }

    fscanf(in,"%d",&do_out);   readrestofline(&in);
    if((do_out != 0) && (do_out != 1) && (do_out != 2) && (do_out != 3)
            && (do_out !=4)) {
        printf("\n\n ERROR in %s (do_out=%d) \n\n",filenamein,do_out);
        exit(3);
    }

    switch(do_out) {   /*TEMPORAL AVERAGING OF GRID OUTPUT*/
    case 0 :
        printf("\ndo_out=%d:  output (whole grids) to files : NO\n",do_out);
        break;
    case 1 :
        printf("\ndo_out=%d:output (whole grid) to file : every time step\n",do_out);
        break;
    case 2 :
        printf("\ndo_out=%d:output (whole grid) to file : daily means\n",do_out);
        break;
    case 3 :
        printf("\ndo_out=%d:output (whole grid) to file only for whole period\n",do_out);
    }

    /*WHICH GRIDS TO FILE*/
    readrestofline(&in);    /*read header line*/
    fscanf(in,"%d %d %d %d %d %d %d %d %d %d %d",&shayes,&exkyes,&solyes,&diryes,
           &dir2yes,&difyes,&gloyes,&albyes,&swbyes,&linyes,&loutyes);
    readrestofline(&in);
    readrestofline(&in);    /*read header line*/
    fscanf(in,"%d %d %d %d %d %d %d %d %d %d",&netyes,&senyes,&latyes,&rainyes,
           &balyes,&melyes,&ablyes,&surftempyes,&posyes,&ddfyes);
    readrestofline(&in);

    printf("shayes exkyes solyes diryes dir2yes difyes gloyes albyes swbyes linyes loutyes\n");
    printf("%5d %5d %5d %6d %6d %6d %6d %6d %6d %6d %6d\n",
               shayes,exkyes,solyes,diryes,dir2yes,difyes,gloyes,albyes,swbyes,linyes,loutyes);
    printf("netyes senyes latyes rainyes balyes melyes ablyes surftempyes posyes ddfyes\n");
    printf("%5d %5d %5d %6d %6d %6d %7d %7d %9d %8d\n",
               netyes,senyes,latyes,rainyes,balyes,melyes,ablyes,surftempyes,posyes,ddfyes);

    if(ddfyes == 1) {  /*ddf can only be caclulated if postemp and melt array are produced*/
        posyes = 1;
        melyes = 1;
        fprintf(outcontrol,"ddfyes = 1  --> DDF to be put to output\n");
        fprintf(outcontrol," posyes and melyes set to 1\n");
    }

    fscanf(in,"%d",&surfyes);	readrestofline(&in);
    fscanf(in,"%d",&snowyes);	readrestofline(&in);
        if((surfyes > 2) || (snowyes > 2)) {
        printf("\n\n ERROR in input.txt: surfyes and snowyes must be 0, 1 or 2 !!! \n\n");
        exit(2);
    }
    
    fscanf(in,"%f",&daysnow);	          readrestofline(&in);
    fscanf(in,"%d",&numbersnowdaysout);	  readrestofline(&in);
    printf(" numbersnowdaysout = %5d\n",numbersnowdaysout);

/*read the jd to be written to output for surface type and snow cover*/
    if(numbersnowdaysout > 0)
	{ 
        for(i=1; i<=numbersnowdaysout; i++) {
            fscanf(in,"%f",&jdsurface[i]);
            printf(" jdsurface[%d] = %.0f\n",i,jdsurface[i]);
            if((jdsurface[i] < 0) || (jdsurface[i] > 1000)) {
                printf("\n\n ERROR in input.txt:  jdsurface must be 1-1000 !!! \n\n");
                exit(2);
            }
        }  /*endfor*/
        readrestofline(&in);
    }  /*endif*/

    readrestofline(&in);    /*read comment line "2.) MASS BALANCE OUTPUT"*/

    fscanf(in,"%d",&winterbalyes);  readrestofline(&in);  /*compute winter balance, yes or no*/
    fscanf(in,"%d",&summerbalyes);  readrestofline(&in);
    fscanf(in,"%f",&winterjdbeg);   readrestofline(&in);
    fscanf(in,"%f",&winterjdend);   readrestofline(&in);
    
    printf("    winterbalyes = %3d\n",winterbalyes);
    printf("    summerbalyes = %3d\n",summerbalyes);
    if ((winterbalyes != 0) && (winterbalyes != 1)) {
        printf("\n\n ERROR in input.txt: winterbalyes (=%d) must be 0 or 1 !!! \n\n",winterbalyes);
        exit(2);
    }
    if ((summerbalyes != 0) && (summerbalyes != 1)) {
        printf("\n\n ERROR in input.txt: summerbalyes (=%d) must be 0 or 1 !!! \n\n",summerbalyes);
        exit(2);
    }
    
    summerjdbeg = winterjdend+1;   /*summer balance starts day after winter balance*/
    summerjdend = winterjdbeg-1;   /*summer balance ends 1 day before winter balance starts*/
    if(winterjdbeg == 1)    {summerjdend   = 365;}   /*account for different year*/
	if(winterjdend == 365)  {summerjdbeg = 1;}

    fscanf(in,"%d",&datesfromfileyes);  readrestofline(&in);
    if ((datesfromfileyes != 0) && (datesfromfileyes != 1)) {
        printf("\n\n ERROR in input.txt:  datesfromfileyes (=%d) must be 0 or 1 !!! \n\n",datesfromfileyes);
        exit(2);
    }
    fscanf(in,"%s",namedatesmassbal);  readrestofline(&in);
    printf("\t\t%s\n",namedatesmassbal);

    fscanf(in,"%f",&beltwidth);   readrestofline(&in);
    printf(" beltwidth  = %.2f\n",beltwidth);

    fscanf(in,"%d",&snow2zeroeachyearyes);   readrestofline(&in);
    printf(" snowfrsnow2zeroeachyearyes = %5d\n",snow2zeroeachyearyes);
    if ((snow2zeroeachyearyes != 0) && (snow2zeroeachyearyes != 1)) {
        printf("\n\n ERROR in input.txt:  snow2zeroeachyearyes (=%d) must be 0 or 1 !!! \n\n",datesfromfileyes);
        exit(2);
    }

    fscanf(in,"%d",&snowfreeyes);   readrestofline(&in);
    printf(" snowfreeyes  = %5d\n",snowfreeyes);

    readrestofline(&in);   /*comment line*/
    fscanf(in,"%d",&cumumeltyes);   readrestofline(&in);
    fscanf(in,"%f",&cm_or_m);       readrestofline(&in);
    if ((cm_or_m != 10) && (cm_or_m != 1000)) {
        printf("\n\n ERROR in input.txt:  variable cm_or_m (=%.0f)!!! \n\n",cm_or_m);
        exit(2);
    }

    /*** TIME SERIES OF GLACIER MEAN ***/
    fscanf(in,"%d",&do_out_area);
    readrestofline(&in);

    if((do_out_area != 0) && (do_out_area != 1)) {
        printf("\n\n ERROR in %s (write output parameter 2) \n\n",filenamein);
        exit(3);
    }

    switch (do_out_area) {
    case 0:
        printf("output of time series area mean   : NO");
        break;
    case 1:
        printf("output of time series area mean   : YES");
    }

    /***OUTPUT OF INDIVIDUAL GRID RESULTS FOR EVERY TIME STEP*/

    fscanf(in,"%d",&outgridnumber);
    readrestofline(&in);
    printf("\nnumber of station output files    : %d\n",outgridnumber);
    readrestofline(&in);
    readrestofline(&in);

    if (outgridnumber > 0) {
        for (i=1; i<=outgridnumber; i++) {
            fscanf(in,"%s %f %f %d",&(outgridname[i][0]),&stn_xcoordinate[i],&stn_ycoordinate[i],
                   &outglobnet[i]);
            readrestofline(&in);
            /*variables read as float but must be transferred to integer*/
            /*if given as rows and columns float values are assigned to new integer variables below
              after coordinatesyes is read below*/
            /*if given as float coordinates, values are converted to row/col in initial.c*/

        } /*for*/
    }



    /****** METHODS RADIATION ************************************************/
    readrestofline(&in);    /*read comment line methods*/
    fscanf(in,"%d",&methodinisnow);
    readrestofline(&in);

    switch(methodinisnow) {
    case 1:
        printf("\nALBEDO / SURFACE TYPE READ FROM FILES\n");
        break;
    case 2:
        printf("\nINITIAL SNOW COVER NEEDED\n");
        break;
    default :
        printf("\n\n ERROR in input.txt: variable methodinisnow\n\n");
        exit(10);
        fclose(outcontrol);
    }

    fscanf(in,"%d",&methodsnowalbedo);
    readrestofline(&in);
    fscanf(in,"%d",&methodglobal);
    readrestofline(&in);
    fscanf(in,"%d",&methodlonginstation);
    readrestofline(&in);
    fscanf(in,"%d",&methodlongin);
    readrestofline(&in);
    fscanf(in,"%d",&methodsurftempglac);
    readrestofline(&in);
    readrestofline(&in);

    if((methodglobal > 2) || (methodlongin > 2)) {
        printf("\n\n ERROR in input.txt: methodglobal (=%d) and methodlongin (=%d) must be 1 or 2 !!! \n\n",methodglobal,methodlongin);
        exit(2);
    }
    
    if ((degreedaymethod == 1) && (methodsurftempglac == 4)) { /*snow model requested but degree day model run*/
        printf("snowmodel can not be run when degree day method-> therefore methodsurftempglac set to 1: \n");
        methodsurftempglac = 1;
        methodsurftemglac_change = 1;
    }

    fscanf(in,"%d",&methodturbul);
    readrestofline(&in);
    if((methodturbul < 1) || (methodturbul > 4)) {
        printf("\n\n ERROR in input.txt: methodturbul (=%d) must be 1, 2, 3 or 4 !!! \n\n",methodturbul);
        exit(2);
    }

    fscanf(in,"%d",&method_z0Te);
    readrestofline(&in);
    fscanf(in,"%d",&methodiceheat);
    readrestofline(&in);
    fscanf(in,"%d",&methodnegbal);
    readrestofline(&in);

    if(energymethod == 1) {
        switch(methodsnowalbedo) {
        case 1:
            printf("SNOW ALBEDO CONSTANT\n");
            fprintf(outcontrol,"SNOW ALBEDO CONSTANT albyes is set to 0\n");
            albyes=0;
            break;
        case 2:
            printf("SNOW ALBEDO GENERATED\n");
            break;
        case 3:
            printf("SNOW ALBEDO GENERATED INCLUDING CLOUD DEPENDENCE\n");
            break;
        case 4:
            printf("SNOW ALBEDO GENERATED AS A FUNCTION OF SNOWDEPTH\n");
            break;
        case 5:
            printf("SNOW ALBEDO GENERATED AS A FUNCTION OF SNOWDEPTH INCLUDING CLOUD DEPENDENCE\n");
            break;
        case 6:
            printf("SNOW ALBEDO ACCORDING TO SICART PHD P.243\n");
            break;
        default :
            printf("\n\n ERROR in input.txt: variable methodsnowalbedo\n\n");
            exit(10);
        }

        switch(methodglobal) {
        case 1:
            printf("GLOBAL RADIATION INTERPOLATED\n");
            fprintf(outcontrol,"GLOBAL RADIATION INTERPOLATED dir2yes and difyes set to 0\n");
            dir2yes = 0;
            difyes  = 0;
            /*SET TO 0, TO AVOID ARRAY STORAGE RESERVATION*/
            break;
        case 2:
            printf("DIRECT AND DIFFUSE RADIATION SEPARATED\n");
            break;
        default :
            printf("\n\n ERROR in input.txt: variable methodglobal\n\n");
            exit(10);
        }

        switch(methodlonginstation) {
        case 1:
            printf("LONGWAVE INCOMING RADIATION FROM MEAS OF NET, GLOB, REF\n");
            break;
        case 2:
            printf("LONGWAVE IN FROM DIRECT MEASUREMENTS\n");
            break;
        case 3:
            printf("LONGWAVE INCOMING RADIATION PAR. BY KONZELMANN\n");
            break;
        case 4:
            printf("LONGWAVE INCOMING RADIATION PAR. BY BRUNTS\n");
            break;
        case 5:
            printf("LONGWAVE INCOMING RADIATION PAR. BY BRUTSAERT\n");
            break;
        case 6:
            printf("LONGWAVE INCOMING RADIATION PAR. BY KONZELMANN BUT CLOUDS PARAMETERIZED\n");
            break;
        default :
            printf("\n\n ERROR in input.txt: variable methodlonginstation\n\n");
            exit(10);
        }

        switch(methodlongin) {
        case 1:
            printf("LONGWAVE INCOMING RADIATION CONSTANT IN SPACE\n");
            fprintf(outcontrol,"LONGWAVE INCOMING RAD CONSTANT IN SPACE linyes set to 0\n");
            linyes = 0;    /*!!!!!!!!!!*/
            /*SET TO 0, TO AVOID ARRAY STORAGE RESERVATION*/
            break;
        case 2:
            printf("LONGWAVE INCOMING RADIATION VARIABLE IN SPACE\n");
            break;
        default :
            printf("\n\n ERROR in input.txt: variable methodlongin\n\n");
            exit(10);
        }

        switch(methodsurftempglac) {
        case 1:
            printf("SURFACE TEMP OF GLACIER IS CONSTANT 0 degrees\n");
            fprintf(outcontrol,"SURFACE TEMP IS CONSTANT  loutyes set to 0\n");
            loutyes = 0;   /*to avoid output to grid file, not possible if longout constant*/
            break;
        case 2:
            printf("SURFACE TEMP ICE, SNOW LOWERED BY ITERATION IF ENBAL NEG \n");
            break;
        case 3:
            printf(" \nSURFACE TEMP FROM LONGOUT MEASUREMENTS AND (DECREASED WITH ELEVATION) \n");
            printf("     OPTION NOT YET OPERATIONAL\n\n");
            /*exit(3); jes*/
            break;
        case 4:
            printf("SURFACE TEMP ICE, SNOW CALCULATED WITH SNOW/ICE MODEL\n");
            methodiceheat=1;
            methodnegbal=1;
            fprintf(outcontrol,"methodsurftempglac = 4  --> snow model run\n");
            fprintf(outcontrol,"  methodiceheat and methodnegbal set to 0\n");
            break;
        default :
            printf("\n\n ERROR in input.txt: variable methodsurftempglac\n");
            exit(10);
        }

        switch(methodturbul) {
        case 1:
            printf("TURBULENCE ACCORDING TO ESCHER-VETTER\n");
            break;
        case 2:
            printf("TURBULENCE ACCORDING TO AMBACH - NO STABILITY\n");
            break;
        case 3:
            printf("TURBULENCE ACCORDING TO MUNRO (with stability)\n");
            break;
        case 4:
            printf("CHR TURBULENCE ACCORDING TO MUNRO (with stability)\n"); /*CHR added option*/
            break;
        default :
            printf("\n\n ERROR in input.txt: variable methodturbul\n\n");
            exit(10);
        }

        switch(methodiceheat) {
        case 1:
            printf("NO ICE HEAT FLUX\n");
            break;
        case 2:
            printf("ICE HEAT FLUX CALCULATED\n");
            break;
        default :
            printf("\n\n ERROR in input.txt: variable methodiceheat\n\n");
            exit(10);
        }

        switch(methodnegbal) {
        case 1:
            break;
        case 2:
            printf("NEGATIVE ENERGY BALANCES STORED\n");
            break;
        default :
            printf("\n\n ERROR in input.txt: variable methodnegbal\n\n");
            exit(10);
        }
    }  /*endif energy balance*/

    /******* SET VARIABLES FOR CERTAIN CASES *************/
    if((exkyes == 1) && (do_out >= 2)) {
        solyes = 1;
        diryes = 1;
        fprintf(outcontrol,"exkyes = 1, therefore solyes and diryes set to 0\n");
    }    /*this is necessary, because the mean exkor-factor is the ratio of
	    direct rad corrected and direct rad uncorrected*/

    if(methodinisnow == 1) {   /*prescribed surface characteristics maps*/
        surfyes = 0;           /*don't write this to file again*/
        snowyes = 0;           /*no snow accumulated, can not be written to file*/
        fprintf(outcontrol,"methodinisnow=1, therefore surfyes and snowyes set to 0\n");
    }

    if(methodglobal ==1) {    /*no separation into direct and diffuse radiation*/
        dir2yes = 0;
        difyes  = 0;
        fprintf(outcontrol,"methodglobal=1, therefore dir2yes and difyes set to 0\n");
    }
    if(methodlongin == 1) {   /*longwave irradiance spatially constant*/
        linyes   = 0;
        /*     surftempyes  = 0;   */
    }

    /******* GLACIER GEOMETRY CHANGES **********************************/
    readrestofline(&in);
    fscanf(in,"%d",&retreatyes);  	readrestofline(&in);
    fscanf(in,"%f",&gammaVA);     	readrestofline(&in);
    fscanf(in,"%f",&c_coefficient);	readrestofline(&in);

    switch(retreatyes) {
    case 0:
        printf("\nretreatyes=0 (CONSTANT AREA)\n");
        break;
    case 1:
        printf("\nretreatyes=1 (VOLUME - AREA SCALING)\n");
        winterbalyes=1;   summerbalyes=1;
        /*mass balance must be computed to have the volume change, and
          areachanges are written to mass balance file specificmassbal.txt*/
        fprintf(outcontrol," input.c in switch retreatyes: set winterbalyes=1 summerbalyes=1  \n");
        break;
    case 2:
        printf("\nretreatyes=2: GLACIER RETREAT USING HUSS PARAMETERIZATION\n");
        winterbalyes=1;   summerbalyes=1;
        fprintf(outcontrol," input.c in switch retreatyes: set winterbalyes=1 summerbalyes=1  \n");
        break;
    default :
      printf("\n\nERROR in input.txt: variable retreatyes=%d must be 0, 1 or 2\n\n",retreatyes);
        exit(10);
        fclose(outcontrol);
    }

    /******* READ FILE NAMES ***********************************/
    readrestofline(&in);
    fscanf(in,"%s",namedgm);		readrestofline(&in);
    fscanf(in,"%s",namedgmdrain);	readrestofline(&in);
    fscanf(in,"%s",namedgmglac);	readrestofline(&in);
    fscanf(in,"%s",namedgmslope);	readrestofline(&in);
    fscanf(in,"%s",namedgmaspect);	readrestofline(&in);
    fscanf(in,"%s",namedgmskyview);	readrestofline(&in);
    fscanf(in,"%s",namedgmfirn);	readrestofline(&in);
    fscanf(in,"%s",nameinitialsnow); 	readrestofline(&in);
    fscanf(in,"%s",namedgmthickness);	readrestofline(&in);
    fscanf(in,"%s",nameklima);			readrestofline(&in);

    printf("FILE NAMES:\t%s  \t%s \t%s \n",namedgm,namedgmdrain,namedgmglac);
    printf("\t\t%s \t%s \t%s \n",namedgmslope,namedgmaspect,nameklima);

    if ((strcmp(namedgm,namedgmdrain) == 0) || (strcmp(namedgm,namedgmglac) == 0)
            || (strcmp(namedgm,namedgmslope) == 0) || (strcmp(namedgm,namedgmaspect) == 0)
            || (strcmp(namedgm,namedgmskyview) == 0) || (strcmp(namedgm,namedgmfirn) == 0)
            || (strcmp(namedgmdrain,namedgmglac) == 0)) {
        printf("\n\n ERROR in file input.c, 2 filesnames are the same\n\n");
        exit(20);
    }

    /*-------------------------------------------------*/

    readrestofline(&in);               /*READ COMMENT LINE*/
    fscanf(in,"%lf",&laenge);	readrestofline(&in);
    fscanf(in,"%lf",&breite);	readrestofline(&in);
    fscanf(in,"%lf",&reflongitude);  readrestofline(&in);

    if ((laenge < -180) || (laenge > 180)) {
        printf("\n\n ERROR in input.txt: longitude (=%.3f) \n\n",laenge);
        exit(2);
    }
    if ((breite < -90) || (breite > 90)) {
        printf("\n\n ERROR in input.txt: latitude (=%.3f) \n\n",breite);
        exit(2);
    }
    if ((reflongitude < -180) || (reflongitude > 180)) {
        printf("\n\n input.txt: reference longitude n\n");
        exit(2);
    }

    printf(" \t longitude = %6.2f \t latitude = %6.2f \n",laenge,breite);

    fscanf(in,"%d",&rowclim);
    readrestofline(&in);
    fscanf(in,"%d",&colclim);
    readrestofline(&in);
    fscanf(in,"%d %f",&climoutsideyes,&heightclim);
    readrestofline(&in);
    if((climoutsideyes != 0) && (climoutsideyes != 1) && (climoutsideyes !=
            2)) {
        printf("\n\n ERROR in input.txt: variable: climoutsideyes) \n\n");
        exit(3);
    }

    /*    if((energymethod == 1) && (climoutsideyes == 1))
          {  printf("\n\n ERROR in input.txt: climoutsideyes=1 \n\n");
             printf("\n\n In energy balance modelling the climate station must be in the grid\n\n");
           	 exit(3);
          }
    */

    fscanf(in,"%f",&gridsize);
    readrestofline(&in);
    fscanf(in,"%f",&timestep);
    readrestofline(&in);

    if ((timestep !=1) && (timestep != 24)) {
        printf("\n timestep wrong in input.txt\n");
        printf("    must be 1 (hourly) or 24 (daily\n");
        printf("if other time steps wanted, finding climate start row must be adjusted in initial.c\n\n");
        exit(2);
    }
    if((timestep == 24) && (do_out == 2)) {   /*daily computation, no daily means can be computed*/
        do_out = 0;
        printf(" DAILY COMPUTATION: IF DAILY OUTPUT WANTED SET do_out=1 !\n");
        fprintf(outcontrol," daily time steps: do_out set to 0 (no daily output possible)\n");
    }
    if((timestep != 1) && (methodsnowalbedo >= 2) && (energymethod == 1)) {
        printf("\n snow albedo generated: only possible with hourly timesteps\n\n");
        exit(2);
    }

    readrestofline(&in);                  /*READ COMMENT LINE*/
    fscanf(in,"%d",&formatclimdata);
    readrestofline(&in);
    if((formatclimdata != 1) && (formatclimdata != 2) && (formatclimdata != 3)) {
        printf("\n ERROR in input.txt: formatclimdata must be 1, 2 or 3 \n\n");
        exit(2);
    }

    fscanf(in,"%d",&maxcol);    readrestofline(&in);
    fscanf(in,"%d",&coltemp);   readrestofline(&in);   /*columns climate data*/
    fscanf(in,"%d",&colhum);    readrestofline(&in);
    fscanf(in,"%d",&colwind);   readrestofline(&in);
    fscanf(in,"%d",&colglob);   readrestofline(&in);
    fscanf(in,"%d",&colref);    readrestofline(&in);
    fscanf(in,"%d",&colnet);    readrestofline(&in);
    fscanf(in,"%d",&collongin);    readrestofline(&in);
    fscanf(in,"%d",&collongout);   readrestofline(&in);
    fscanf(in,"%d",&colprec);   readrestofline(&in);
    fscanf(in,"%d",&colcloud);  readrestofline(&in);
    fscanf(in,"%d",&coltempgradvarying);   readrestofline(&in);
    fscanf(in,"%d",&coldis);    readrestofline(&in);

    readrestofline(&in);
    if(coltemp < 4) {
        printf("\n column with temperature (=%d) must be 4 or more; check 'coltemp' in input.txt\n\n",coltemp);
        exit(2);
    }
    printf(" number of columns in climate input file= %d\n",maxcol);

    fscanf(in,"%f",&ERAtempshift);
    readrestofline(&in);
    fscanf(in,"%f",&ERAwindshift);
    readrestofline(&in);
    printf(" ERAtempshift = %.2f\n",ERAtempshift);
    printf(" ERAwindshift = %.2f\n",ERAwindshift);
    readrestofline(&in);

    fscanf(in,"%d",&methodtempinterpol);
    readrestofline(&in);
    if ((methodtempinterpol == 3) && (timestep >1)) { /*temp data read from grid files*/
        printf("\n\n temp data can not be read from file if time step is not hourly\n check 'methodtempinterpol' \n\n");
        exit(2);
    }

    printf(" methodtempinterpol= %d\n",methodtempinterpol);

    fscanf(in,"%f",&tempgrad);       readrestofline(&in);
    fscanf(in,"%f",&tempscenario);   readrestofline(&in);
    fscanf(in,"%f",&precscenario);   readrestofline(&in);
    readrestofline(&in);

    fscanf(in,"%d",&monthtempgradyes);

    switch(methodtempinterpol) {
    case 1:
        printf("\n predescribed temperature lapse rates used\n");
        if(monthtempgradyes == 0)
            printf("constant temperature lapse rates used = %.2f\n\n",tempgrad);
        else
            printf("monthly variable lapse rates used\n\n");
        break;
    case 2:
        printf("\n temperature lapse rates computed from 2 AWS, variable in time\n");
        break;
    case 3:
        printf("\n air temperatures read from gridded files\n");
        break;
    default :
        printf("\n\n ERROR in input.txt: variable methodtempinterpol, must be 1,2 or 3 (printed from input.c)\n\n");
        exit(10);
    }

  /*--- read 12 values one for each month ---*/
    for (i=1; i<=12; i++)
        fscanf(in, "%f", monthtempgrad+i);
    readrestofline(&in);

    fscanf(in,"%d",&monthtempscenyes);
    for (i=1; i<=12; i++)
        fscanf(in, "%f", monthtempscen+i);
    readrestofline(&in);

    fscanf(in,"%d",&monthprecipscenyes);
    for (i=1; i<=12; i++)
        fscanf(in, "%f", monthprecipscen+i);
    readrestofline(&in);


    if (monthtempscenyes == 0)
        printf("CLIMATE SCENARIO: temp is increased by = %5.2f\n",tempscenario);
    else if(monthtempscenyes == 1)
        printf("\n monthly temperature scenario used\n\n");
    else {
        printf("\n ERROR input.txt: variable monthtempscenyes = %d\n",monthtempscenyes);
        printf("\n must be 0 or 1   (input.c) \n\n");
        exit(2);
    }

    if (monthprecipscenyes == 0)
        printf("CLIMATE SCENARIO: precipitation is increased by = %5.2f percent\n",precscenario);
    else if(monthprecipscenyes == 1)
        printf("\n monthly precipitation scenario used\n\n");
    else {
        printf("\n ERROR input.txt: variable monthprecipscenyes = %d\n",monthprecipscenyes);
        printf("\n must be 0 or 1  (input.c) \n\n");
        exit(2);
    }

    /*---- SURFACE TYPE FILES ----*/
    readrestofline(&in);
    fscanf(in,"%d",&n_albfiles);
    readrestofline(&in); /*number Alb-files*/

    if(n_albfiles > 0) {   /*SURFACE TYPE PRESCRIBED BY RASTER MAPS*/
        for (i=1; i<=n_albfiles; i++) {
            fscanf(in,"%f %s",&jdstartalb[i],&namealb[i][0]);
            readrestofline(&in);
        }
        jdstartalb[n_albfiles+1]=10000.;  /*damit last alb-file valid for rest of period*/

        if ((methodinisnow == 1) && (jdstartalb[1] > jdbeg)) {
            printf("\n There is no albedo-file for the first day\n\n");
            exit(2);
        }
    }
    /* print albedo file names to screen, 3 side by side*/
    /*   for (i=1;i<=n_albfiles;i++)
        {
          printf("    %s",namealb[i]);
          if ((i/3) == (float)i/3)
    	printf("\n");
        }
    */

    fscanf(in,"%f",&albsnow);
    readrestofline(&in);   /*ALBEDO-VALUES*/
    fscanf(in,"%f",&albslush);   readrestofline(&in);
    fscanf(in,"%f",&albice);     readrestofline(&in);
    fscanf(in,"%f",&albfirn);    readrestofline(&in);
    fscanf(in,"%f",&albrock);    readrestofline(&in);
    fscanf(in,"%f",&albmin);     readrestofline(&in);
    fscanf(in,"%f",&snowalbincrease);    readrestofline(&in);
    fscanf(in,"%f",&albiceproz);    readrestofline(&in);
    fscanf(in,"%f",&ndstart);       readrestofline(&in);

    if((albsnow > 1) || (albice > 1) || (albfirn > 1)) {
        printf("\n\n ERROR in input.txt: variable albsnow %.2f or albice %.2f or albfirn %.2f wrong (> 1)\n\n",
               albsnow,albice,albfirn);
        exit(3);
    }

    printf("  albedo  snow %4.2f   slush %4.2f   ice %4.2f\n",albsnow,albslush,albice);
    printf("min albedo = %8.2f\n",albmin);
    printf("no. of days since snowfall = %6.1f\n",ndstart);
    if(albmin > 0.95) {
        printf("\n\n ERROR in input.txt : variable albmin %.2f wrong (> 0.95)\n\n",albmin);
        exit(3);
    }

    /******** RADIATION **********************************************/
    readrestofline(&in);
    fscanf(in,"%d",&split);
    readrestofline(&in);

    fscanf(in,"%f",&prozdiffuse);
    readrestofline(&in);
    fscanf(in,"%f",&trans);
    readrestofline(&in);
    fscanf(in,"%f",&startratio);
    readrestofline(&in);
    /*variable ratio is set to startratio at begining of main program*/
    fscanf(in,"%f",&ratiodir2dir);
    readrestofline(&in);
    fscanf(in,"%f",&surftemplapserate);
    readrestofline(&in);

    if((prozdiffuse < 0) || (prozdiffuse > 50)) {
        printf("\n\n ERROR in input.txt : variable prozdiffuse wrong  (input.c)\n\n");
        exit(3);
    }
    if((trans < 0.3) || (trans > 1)) {
        printf("\n\n ERROR in input.txt : variable trans wrong %.2f  (input.c) \n\n",trans);
        exit(3);
    }

    if((startratio < 0) || (startratio > 1.5)) {
        printf("\n\n ERROR in input.txt : variable startratio wrong %.2f\n\n",startratio);
        exit(3);
    }
    if((ratiodir2dir < 0) || (ratiodir2dir > 1.5)) {
        printf("\n\n ERROR in input.txt : variable ratiodir2dir wrong %.2f\n\n",ratiodir2dir);
        exit(3);
    }

    if((surftemplapserate > 0) || (surftemplapserate < -2)) {
        printf("\n\n ERROR in input.txt : variable surftemplapserate wrong %.2f\n\n",surftemplapserate);
        exit(3);
    }
    fscanf(in,"%d",&directfromfile);
    readrestofline(&in);
    fscanf(in,"%s",pathdirectfile);
    readrestofline(&in);

    if(timestep == 24) {
        directfromfile = 1;   /*daily mean does not work with this program*/
        /*in shading.c computations every hour/split, then mean of those values*/
        printf("   DAILY SIMULATION: DIRECT RADIATION FILES MUST BE READ FROM FILES !\n\n");
    }

    fscanf(in,"%f",&daysdirect);
    readrestofline(&in);
    printf("  daysdirect= %5.0f\n",daysdirect);
    /*** IF DIRECT RAD FILES GENERATED, THIS IS DONE FOR EACH TIME STEP,
         HENCE DAYSDIRECT MUST BE ZERO ***/
    if((directfromfile == 0) && (daysdirect > 1))
        daysdirect = 0;

    if (directfromfile == 1) {
        printf("direct radiation read from files (no shade etc. calculations)\n");
        split = 1;
    }
    /*SPLIT MUST BE SET TO ONE, BECAUSE IT IS NEEDED IN function readdirect
      to calculate the zenith angle for the middle of the subinterval
      if it was not 1, it would not be the middle of the subinterval   */
    else
        printf("   number of SHADE calculations per time step (split) : %d\n",split);

    if (directfromfile == 1)    /*direct radiation is read from existing files*/
        /*shading, correction factor and direct horizontal radiation are not calculated
          and can thus not be written to grid output files */
    {
        shayes = 0;
        exkyes = 0;
        solyes = 0;
        fprintf(outcontrol,"directfromfile == 1  --> shayes and solyes set to 0\n");

        if(do_out == 1) {   /*output every timestep*/
            diryes = 0;       /*direct radiation already exists on file*/
            fprintf(outcontrol,"Sdirectfromfile = 1 and do_out== --> diryes set to 0\n");
        }
    }

    fscanf(in,"%d",&slopestation);
    readrestofline(&in);
    printf("slopestation  = %5d\n",slopestation);

    if ((slopestation != 0) && (slopestation != 1)) {
        printf("\n\n ERROR in input.txt : slopestation is wrong, must be 0 or 1 !\n\n ");
        exit(11);
    }

    /******* TURBULENCE ********************************/
    readrestofline(&in);
    fscanf(in,"%f",&iterstep);
    readrestofline(&in);  /*for iteration of surf temp*/
    fscanf(in,"%f",&z0wice);
    readrestofline(&in);  /*roughness length*/
    printf("roughness length for ice = %14.6f\n",z0wice);
    fscanf(in,"%f",&dividerz0T);
    readrestofline(&in);  /*divide z0w by this value to get z0T*/
    fscanf(in,"%f",&dividerz0snow);
    readrestofline(&in);  /*to get roughness length for snow*/
    fscanf(in,"%f",&z0proz);
    readrestofline(&in);
    fscanf(in,"%f",&z0min);
    readrestofline(&in);
    fscanf(in,"%f",&z0max);
    readrestofline(&in);

    if ((z0proz < 0) || (z0min < 0) ||  (z0min < 0)) {
        printf("\n\n ERROR in input.txt : z0proz,min,max must be > 0 !\n\n ");
        exit(11);
    }

    /**************** PRECIPITATION  ****************/

    readrestofline(&in);

    fscanf(in,"%d",&methodprecipinterpol);
    readrestofline(&in);
    if((methodprecipinterpol > 3) || (methodprecipinterpol < 1)) {
        printf("\n\n input.c: methodprecipinterpol must be 1, 2 or 3 but it is %d\n",methodprecipinterpol);
        printf("   check input.txt: wrong value or rows are not in right order or too few or too many !!!\n\n");
        exit(10);
    }

    fscanf(in,"%f",&precgrad);
    readrestofline(&in);

    printf("precipitation gradient lower part= %8.2f\n",precgrad);
    fscanf(in,"%f",&precgradhigh);
    printf("precipitation gradient upper part = %8.2f\n",precgradhigh);
    fscanf(in,"%f",&precgradelev);
    readrestofline(&in);
    fscanf(in,"%f",&preccorr);
    readrestofline(&in);
    printf("precipitation correction = %8.2f\n",preccorr);
    fscanf(in,"%f",&snowmultiplierglacier);
    readrestofline(&in);
    fscanf(in,"%f",&snowmultiplierrock);
    readrestofline(&in);

    fscanf(in,"%f",&T0);
    readrestofline(&in);
    fscanf(in,"%d",&onlyglacieryes);
    readrestofline(&in);
    fscanf(in,"%f",&glacierpart);
    readrestofline(&in);
    printf("percentage of glaciation = %8.2f\n",glacierpart);
    
      if((glacierpart < 0) || (glacierpart >100)) {
        printf(" \n variable glacierpart wrong in input.txt = %.0f\n",glacierpart);
        printf("    percentage glacierization must 0-100 (function input.c)\n\n");
        exit(4);
    }

    /**************** DISCHARGE *********************/

    readrestofline(&in);

    fscanf(in,"%s",nameqcalc);
    readrestofline(&in);
    fscanf(in,"%f",&nodis);    /*missing value in discharge file*/
    readrestofline(&in);

    /**************** DISCHARGE STORGAE CONSTANTS*********************/

    fscanf(in,"%f",&firnkons);
    readrestofline(&in);
    fscanf(in,"%f",&snowkons);
    readrestofline(&in);
    fscanf(in,"%f",&icekons);
    readrestofline(&in);
    fscanf(in,"%f",&rockkons);
    readrestofline(&in);

    if(disyes >= 1)
        printf("   recession constants firn: %4.1f snow: %4.1f ice: %4.1f\n",
               firnkons,snowkons,icekons);
               
    if((firnkons <= 0) || (firnkons >2000)) {
        printf(" \n variable firnkons wrong in input.txt = %.0f\n",firnkons);
        printf("    must > 0 or <2000 (function input.c)\n\n");
        exit(4);
    }

    readrestofline(&in);                /*STORAGE CONSTANTS*/
    fscanf(in,"%f",&qfirnstart);
    readrestofline(&in);
    fscanf(in,"%f",&qsnowstart);
    readrestofline(&in);
    fscanf(in,"%f",&qicestart);
    readrestofline(&in);
    fscanf(in,"%f",&qrockstart);
    readrestofline(&in);
    fscanf(in,"%f",&qground);
    readrestofline(&in);
    fscanf(in,"%f",&jdstartr2diff);
    readrestofline(&in);

    /*=====================================================================*/
    /*   15.) read variables of SNOW MODEL by C. Tijm-Reijmer 2/2005       */
    /*=====================================================================*/
    readrestofline(&in);
    fscanf(in,"%d",&percolationyes);
    readrestofline(&in);
    printf("percolationyes = %d\n",percolationyes);
    if((percolationyes != 0) && (percolationyes != 1)) {
        printf(" \n variable percolationyes wrong in input.txt = %d\n",percolationyes);
        printf("    must be 0 or 1 (function input.c)\n\n");
        exit(4);
    }


    fscanf(in,"%d",&slushformationyes);
    readrestofline(&in);
    if (percolationyes == 0) slushformationyes = 0;
    if((slushformationyes != 0) && (slushformationyes != 1)) {
        printf(" \n variable slushformationyes wrong in input.txt = %d\n",slushformationyes);
        printf("    must be 0 or 1 (function input.c)\n\n");
        exit(4);
    }

    fscanf(in,"%d",&densificationyes);
    readrestofline(&in);
    if((densificationyes != 0) && (densificationyes != 1)) {
        printf(" \n variable densificationyes wrong in input.txt =%d\n",densificationyes);
        printf("    must be 0 or 1 (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%d",&wetstartyes);
    readrestofline(&in);
    if((wetstartyes != 0) && (wetstartyes != 1)) {
        printf(" \n variable wetstartyes wrong in input.txt = %d\n",wetstartyes);
        printf("    must be 0 or 1 (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%d",&methodinitialverticalgrid);
    readrestofline(&in);
    if((methodinitialverticalgrid != 1) && (methodinitialverticalgrid != 2)) {
        printf(" \n variable methodinitialverticalgrid wrong in input.txt = %d\n",methodinitialverticalgrid);
        printf("    must be 1 or 2 (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%d",&ndepths);
    readrestofline(&in);
    fscanf(in,"%d",&factinter);
    readrestofline(&in);
    printf(" \n ndepths=%d   factinter=%d\n",ndepths,factinter);

    readrestofline(&in);
    /*-----------------------*/
    fscanf(in,"%f",&thicknessfirst);
    readrestofline(&in);
    if((thicknessfirst <=0) || (thicknessfirst >50)) {
        printf(" \n variable thicknessfirst wrong in input.txt =%.2f\n",thicknessfirst);
        printf("    must be between 0 and 50 (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%f",&thicknessdeep);
    readrestofline(&in);
    if((thicknessdeep <=0) || (thicknessdeep < thicknessfirst)) {
        printf(" \n variable thicknessdeep wrong in input.txt = %.2f\n",thicknessdeep);
        printf("    must be >0 and < thicknessfirst (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%f",&depthdeep);
    readrestofline(&in);
    if((depthdeep <=0) || (depthdeep < depthdeep)) {
        printf(" \n variable depthdeep wrong in input.txt = %.2f\n",depthdeep);
        printf("    must be >0 and < depthdeep (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%lf",&denssnow);
    readrestofline(&in);
    if(denssnow > 700) {
        printf(" \n variable denssnow=%.2lf wrong in input.txt\n",denssnow);
        printf(" \n should be: denssnow > 700  (input.c)\n\n");
        exit(4);
    }
    printf(" denssnow=%.2lf ",denssnow);

    fscanf(in,"%d",&irrwatercontyes);
    readrestofline(&in);
    if((irrwatercontyes != 0) && (irrwatercontyes != 1) && (irrwatercontyes != 2)) {
        printf(" \n variable irrwatercontyes wrong in input.txt =%d\n",irrwatercontyes);
        printf("    must be 0, 1 or 2 (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%lf",&irrwatercont);
    readrestofline(&in);
    if((irrwatercont <=0) || (irrwatercont > 0.5)) {
        printf(" \n variable depthdeep wrong in input.txt =%.2lf\n",irrwatercont);
        printf("    must be >0 and < 0.5 (function input.c)\n\n");
        exit(4);
    }
    printf("    irrwatercont=%.2lf\n",irrwatercont);

    /*---- Output ----------*/
    readrestofline(&in);
    fscanf(in,"%d",&factsubsurfout);
    readrestofline(&in);
    if((factsubsurfout < 1) || (factsubsurfout > 24)) {
        printf(" \n variable irrwatercontyes wrong in input.txt = %d\n",factsubsurfout);
        printf("    must be 0, 1 or 2 (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%d",&offsetsubsurfout);
    readrestofline(&in);

    readrestofline(&in);
    fscanf(in,"%d%d%d%d",&runoffyes,&superyes,&wateryes,&surfwateryes);
    fscanf(in,"%d%d%d",&slushyes,&coldsnowyes,&coldtotyes);
    readrestofline(&in);
    if((superyes != 0) && (superyes != 1)) {
        printf(" \n variable superyes wrong in input.txt = %d\n",superyes);
        printf("    must be 0 or 1 (function input.c)\n\n");
        exit(4);
    }
    if((coldtotyes != 0) && (coldtotyes != 1)) {
        printf(" \n variable coldtotyes wrong in input.txt = %d\n",coldtotyes);
        printf("    must be 0 or 1 (function input.c)\n\n");
        exit(4);
    }


    /*=====================================================================*/
    /*   17.) TEMPERATURE INDEX METHOD    */
    /*=====================================================================*/
    /* NEEDED IF MELT IS CALCULATED BY TEMPERATURE INDEX METHOD
         program degree.c, ddfopt.c */

    if(degreedaymethod == 1) {
        printf(" \n TEMPERATURE INDEX METHOD\n ");
        fprintf(outcontrol,"\n   TEMPERATURE INDEX METHOD\n");
    }

    readrestofline(&in);
    readrestofline(&in);
    readrestofline(&in);
    fscanf(in,"%d",&ddmethod);    readrestofline(&in);
	printf("ddmethod = %d\n",ddmethod);
    if((ddmethod < 1) || (ddmethod > 3)) {
        printf(" \n variable ddmethod wrong in input.txt = %d\n",ddmethod);
        printf("    must be 1, 2 or 3  (function input.c)\n\n");
        exit(4);
    }
    fscanf(in,"%f",&DDFice);    readrestofline(&in);
    fscanf(in,"%f",&DDFsnow);   readrestofline(&in);

    readrestofline(&in);
    fscanf(in,"%f",&meltfactor);      readrestofline(&in);
    fscanf(in,"%f",&radfactorice);    readrestofline(&in);
    fscanf(in,"%f",&radfactorsnow);   readrestofline(&in);
    fscanf(in,"%f",&debrisfactor);    readrestofline(&in);
    readrestofline(&in);     /* COMMENT STAKES */

    fscanf(in,"%d",&coordinatesyes);
    if((coordinatesyes < 1) || (coordinatesyes > 3)) {
        printf(" \nERROR in input.txt: coordinatesyes (=%d) must be 1, 2 or 3  (input.c)\n\n",coordinatesyes);
        exit(4);
    }
    printf("coordinatesyes = %d\n",coordinatesyes);
    readrestofline(&in);

    /*if station output locations are given in row/col variable need to be made integer*/
    if ((outgridnumber > 0) && (coordinatesyes == 3)) {
        for (i=1; i<=outgridnumber; i++) {
            stnrow[i] = (int)stn_xcoordinate[i];
            stncol[i] = (int)stn_ycoordinate[i];
        }	 /*endfor*/
    }  /*endif*/


/*==================================================================================*/
/*=== CHANGE PARAMETERS IF INCONSISTENT WITH OTHER PARAMETERS CHOSEN ABOVE =========*/

    if(maxmeltstakes > 0) {
        if((melyes == 0) || (ablyes == 0)) {
            melyes = 1;   /*because otherwise the cumulative arrays do not exist (meanMELTall)*/
            ablyes = 1;   /*and these are used for output for individual stakes*/
            fprintf(outcontrol,"melyes and ablyes set to 1 because maxmeltstakes > 0\n");
        }

        for(i=1; i<=maxmeltstakes; i++) {      /* number of stakes for melt output */
            if(coordinatesyes < 3) {  /*locations given in coordinates, float values*/
                fscanf(in,"%f",&melt_xcoordinate[i]);
                fscanf(in,"%f",&melt_ycoordinate[i]);
                readrestofline(&in);   /*coordinates are transformed to row/col in initial.c*/
            } else { /*locations given in row and column, integer values*/
                fscanf(in,"%d",&meltoutrow[i]);    /*integer values*/
                fscanf(in,"%d",&meltoutcol[i]);
                readrestofline(&in);
            }
        }  /*endfor*/

        /**** RESET VARIABLES IN CASE SELECTED COMBINATIONS ARE NOT POSSIBLE - WRITE TO modellog.txt*/

        if((do_out) < 2) {
            do_out = 3;   /*compute period mean of whole grid*/
            fprintf(outcontrol,"maxmeltstakes>0, do_out<2: do_out set to 3\n");
            fprintf(outcontrol,"variable meanMELTall is used to cumulate melt and mass balance\n");
            fprintf(outcontrol," if period mean is not computed, then this variable is not defined\n\n");
        }
        if((do_out) == 2) {   /*daily means wanted*/
            do_out = 4;   /*compute daily and also period means*/
            fprintf(outcontrol,"maxmeltstakes>0, do_out=2: do_out set to 4\n");
            fprintf(outcontrol,"variable meanMELTall is used to cumulate melt and mass balance\n");
            fprintf(outcontrol," if period mean is not computed, then this variable is not defined\n\n");
        }
        /*variable meanMELTall is used to cumulate melt and mass balance;
          if period mean is not computed, then this variable is not defined
          therefore period mean must be computed in this case;
          if time step is daily daily means are not possible*/
        /*note: do_out is set to 0 early in case of daily time steps*/
    }  /*endif maxmeltstakes*/

    printf("*** ALL ROWS READ FROM input.txt ***\n");

    /*if the climate station is outside the grid, it does not make sense to compute only
      the location of the climate station*/
    if((climoutsideyes == 1) && (calcgridyes == 2)) {
        fprintf(outcontrol," climate station is outside grid, whole area must be computed: \n");
        fprintf(outcontrol,"  calcgridyes set to 1\n");
        calcgridyes=1;
    }

    /*if only climate station grid cell computed remove grid output etc that does not make sense*/
    if(calcgridyes == 2) {
        maxmeltstakes=0;
        do_out=0;
        surfyes=0;
        snowyes=0;
        disyes=0;
        fprintf(outcontrol,"only climate station grid cell computed, therefore set: \n");
        fprintf(outcontrol,"  maxmeltstakes=0  do_out=0   surfyes=0  snowyes=0  disyes=0\n");

    }

    if(methodsurftempglac != 4) {   /*snow model is not run*/
        runoffyes=0;
        superyes=0;
        wateryes=0;
        surfwateryes=0;
        slushyes=0;
        coldsnowyes=0;
        coldtotyes=0;
        precipyes=0;
        intaccumyes=0;
        fprintf(outcontrol,"snow model is not run -> therefore set: \n");
        fprintf(outcontrol," runoffyes=0 superyes=0  wateryes=0 surfwateryes=0 slushyes=0 coldsnowyes=0 coldtotyes=0 precipyes=0 intaccumyes=0\n");
    }

    if ((methodtempinterpol == 2) && (monthtempgradyes == 1)) { /*timevariable lapse rate*/
        printf("\n\n variable lapse rate used (methodtempinterpol=%d) \n\n",methodtempinterpol);
        fprintf(outcontrol,"variable lapse rate used methodtempinterpol -> therefore monthtempgradyes set to 0: \n");
        exit(2);
    }

    if (methodsurftemglac_change ){  /*snow model requested but degree day model run*/
        fprintf(outcontrol,"snowmodel can not be run when degree day method-> therefore methodsurftempglac set to 1: \n");
    }


    fprintf(outcontrol,"---- END OF LIST OF SUCH CHANGES ---- \n");
    fprintf(outcontrol,"--------------------------------------------------- \n\n");

/*========== WRITE entire input.txt as it is TO control output file modellog.txt ===================*/
    rewind(in);    /*go back to beginning of input.txt file*/
    while((c=getc(in)) != EOF)
        putc(c,outcontrol);

    fprintf(outcontrol,"\n---- END OF INPUT-FILE ----------------------\n\n");
    printf("\n ************END OF INPUT-FILE **********************\n\n");

/*========== WRITE HIDDEN OPTIONS as defined in variab.h, write to modellog.txt  ===================*/

    fprintf(outcontrol,"======= HIDDEN OPTIONS, defined in variab.h =========\n");
    fprintf(outcontrol,"surftempminimum = %.2f\n",surftempminimum);
    fprintf(outcontrol,"calcgridyes (1= whole grid, 2=only AWS) = %d\n",calcgridyes);
    fprintf(outcontrol,"setmelt2zero (0=no, 1=yes, set to 0 if surftemp<0) = %d\n",setmelt2zero);
    fprintf(outcontrol,"surftempminmelt (if setmelt2zero=1 melt is set to 0 if surftemp below this value) = %f\n",surftempminmelt);

    fprintf(outcontrol,"z2 (instrument height (T, RH, wind) = %.2f\n",z2);
    fprintf(outcontrol,"emissivitysurf (surface emissivity) = %.3f\n",emissivitysurf);
    fprintf(outcontrol,"snetfromobsyes (0 from albedo, 1=shortwave from AWS data))= %d\n",snetfromobsyes);
    fprintf(outcontrol,"allradiationfromfile (0=no, 1=yes: all rad fluxes read from file, only for AWS grid cell)= %d\n",allradiationfromfile);
    fprintf(outcontrol,"snowscenario (add to initial snow cover) = %.2f\n",snowscenario);

    fprintf(outcontrol,"   OPTIONS SUBSURFACE SNOW MODEL\n");
    fprintf(outcontrol,"supericegrid = %d\n",supericegrid);
    fprintf(outcontrol,"typealbedo = %d\n",typealbedo);
    fprintf(outcontrol,"typeconduc = %d\n",typeconduc);
    fprintf(outcontrol,"skin_or_inter = %d\n",skin_or_inter);
    fprintf(outcontrol,"init_layertemperature (1=default, >1 = userspecific) = %d\n",init_layertemperature);
    fprintf(outcontrol,"init_snowlayerdensity (1=default, >1 = userspecific) = %d\n",init_snowlayerdensity);

    fprintf(outcontrol,"bs_eachday_yes = %d\n",bs_eachday_yes);

    fprintf(outcontrol,"   Options for ALBEDO in case of snow albedo Oerle and Knap1998:\n");
    fprintf(outcontrol," snowstar = %.3f\n",snowstar);
    fprintf(outcontrol," timestarwet = %.3f\n",timestarwet);
    fprintf(outcontrol," timestardry0 = %.3f\n",timestardry0);
    fprintf(outcontrol," timestardry10 = %.3f\n",timestardry10);
    fprintf(outcontrol,"=======end HIDDEN OPTIONS, defined in variab.h=======\n\n");

    fclose(in);
    in = NULL;
    
printf(" XXXXXXXXX input.c:  winterbalyes=%d summerbalyes=%d retreatyes=%d \n",winterbalyes,summerbalyes,retreatyes);

    
    return;

}

