/***********************************************************************
 *  Copyright 1997-2012 Regine Hock
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
/*   FILE          initial.c                                       */
/*   FUNCTION      startinputdata,startoutascii,startarrayreserve, */
/*                 glacierrowcol,readlim matrixreserv etc.         */
/*  INITIALISATION FUNCTION :
       OPEN AND READ GRID FILES, RESERVE STORAGE OF ALL 2D-ARRAYS
       OPEN FILE WITH CLIMATE DATA and READ UNTIL START
       OPEN OUTPUT-FILES                                           */
/*  1.12.1997, last update 4 November 2013*/
/*******************************************************************/

#include "initial.h"

#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "closeall.h"
#include "globcor.h"
#include "grid.h"
#include "writeout.h"

#include "variabex.h"      /* all global VARIABLES */


/*******************************************************/
/* READ REST OF LINE                                   */
/*******************************************************/
void readrestofline(FILE **infile) {
    int c;
    int countchar=0;
    int maxcount = 10000;   /*to avoid endless loop in case line breaks are missing though they appear in text editor*/

    while((c=getc(*infile)) != '\n') {
        countchar+=1;
        if(countchar == maxcount) {
            printf("\n\n ERROR in function readrestofline (initial.c):\n Too many character (%d), check if line breaks missing\n",maxcount);
            printf("  *** PROGRAM EXITED - CHECK input.txt AND CLIMATE DATA INPUT ***\n\n");
            exit(20);
        }
    }

    /*	; empty command, just read, don´t do anything with variable c*/
    return;
}

void checkgridinputdata_ok() {
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if((griddgmglac[i][j] != nodata) && (griddgmdrain[i][j] == nodata)) {
                printf("\nERROR in drainage basin / glacier input grid data\n");
                printf(" Not all glacier grid cells are part of the drainage basin\n");
                printf(" adjust your glacier grid so that the entire glacier is part of the drainage basin grid\n\n");
                fclose(outcontrol);
                exit(3);
            }
            /*CURRENTLY SNOW MODEL CAN ONLY BE RUN FOR THE GLACIER AND NOT FOR AREA OUTSIDE*/
            if((methodsurftempglac == 4) && ((griddgmglac[i][j] == nodata) && (griddgmdrain[i][j] != nodata))) {
                printf("\nERROR in drainage basin / glacier input grid data\n");
                printf(" snow model is run: in this case glacier and drainage basin grids must be same\n");
                printf(" copy glacier grid under a new name into drainage grid\n\n");
                fclose(outcontrol);
                exit(3);
            }
        }  /*endfor*/
    return;
}




/************************************************************/
/* EXIT PROGRAM IF TOO MANY LINES WRITTEN TO OUTPUT modellog.txt */
/*  to avoid that harddisk is filled to limit               */
/*  called from mainly subsurf()                            */
/*  resoutlines are added every time a variable exceeds preset limits */
/*  e.g. if surftemperature would fall below given limit    */
/************************************************************/

void toobig_resout() {
    if(resoutlines > resoutlinesmax) {
        printf("\n modellog.txt too big, unrealistic values reached > %d times\n",resoutlinesmax);
        printf("  check output file modellog.txt\n\n");
        fclose(outcontrol);
        exit(3);
    }
    return;
}



/***********************************************/
/*     CLOSE FILE                              */
/***********************************************/

void closefile(FILE **datei, char *name)

{
    if (fclose(*datei) == EOF) {
        printf("\n\n ERROR  closing file  %s\n",name);
        printf("\n  function closefile, file initial.c \n\n");
        exit(10);
    }
    *datei = NULL;

    return;
}



/*============================================================================*/
/*  FUNCTIONS FOR INITIALISATION OF OUTPUT FILES WITH COMPONENTS OF ENERGY BALANCE
    DAILY MEANS AND/OR MEANS FOR THE WHOLE PERIOD
       called at end of start and from main                                 ***/

/*************************************************************/
/* FUNCTION  meandayreserve
	     RESERVE STORAGE FOR GRID ARRAYS FOR DAILY MEANS */
/*************************************************************/

void meandayreserve()

/*The array is set to zero in a separate function, because, storage is only reserved
  once, whereas the array is set to zero after each day; in case of means for the whole
  period, storage is only reserved once and initialization is done only once, thus this
  is combined in the same function (meanallnull)*/

{
    /*in case of degree day shayes ... balyes are set to zero,
      therefore an if-loop (if energymethod==1) is not necessary*/

    if(shayes == 1)
        meanSHADEday  = matrixreservdouble(1,nrows,1,ncols);
    /*meanEXK not needed, because it't ratio of DIRECT and SOLHOR*/
    if(solyes == 1)                                /*DIR ON UNOBSTRUCTED HORIZ*/
        meanSOLHORday = matrixreservdouble(1,nrows,1,ncols);
    if(diryes == 1)
        meanDIRECTday   = matrixreservdouble(1,nrows,1,ncols);
    if(dir2yes == 1)               /*ACTUAL DIRECT RADIATION : Global - diffus*/
        meanDIRECT2day = matrixreservdouble(1,nrows,1,ncols);
    if(difyes == 1)                /*DIFFUSE RADIATION*/
        meanDIFFUSday   = matrixreservdouble(1,nrows,1,ncols);
    if(albyes == 1)                /*DIFFUSE RADIATION*/
        meanALBEDOday   = matrixreservdouble(1,nrows,1,ncols);
    if(gloyes == 1)
        meanGLOBALday   = matrixreservdouble(1,nrows,1,ncols);   /*GLOBAL RADIATION  */
    if(swbyes == 1)
        meanSWBALday    = matrixreservdouble(1,nrows,1,ncols);   /*SHORT-WAVE BALANCE*/
    if(linyes == 1)                /*LONWAVE INCOMING RADIATION*/
        meanLONGINday   = matrixreservdouble(1,nrows,1,ncols);
    if(loutyes == 1)                /*LONWAVE INCOMING RADIATION*/
        meanLONGOUTday  = matrixreservdouble(1,nrows,1,ncols);
    if(netyes == 1)
        meanNETRADday   = matrixreservdouble(1,nrows,1,ncols);   /*NET RADIATION     */
    if(senyes == 1)
        meanSENSIBLEday = matrixreservdouble(1,nrows,1,ncols);
    if(latyes == 1)
        meanLATENTday   = matrixreservdouble(1,nrows,1,ncols);
    if(rainyes == 1)
        meanRAINENday   = matrixreservdouble(1,nrows,1,ncols);
    if(balyes == 1)
        meanENBALday    = matrixreservdouble(1,nrows,1,ncols);
    if(melyes == 1)                               /*MELT WATER EQUIVALENT*/
        meanMELTday     = matrixreservdouble(1,nrows,1,ncols);
    if(ablyes == 1)                               /*ABLATION WATER EQUIVALENT*/
        meanABLAday     = matrixreservdouble(1,nrows,1,ncols);

    if(surftempyes == 1)                               /*SURFACE TEMPERATURES*/
        meanSURFTEMPday     = matrixreservdouble(1,nrows,1,ncols);

    if((degreedaymethod == 1) && (posyes == 1))
        meanTEMPOSday     = matrixreservdouble(1,nrows,1,ncols);

    return;
}



/******************************************************/
/* FUNCTION  meandaynull
	     INITIALISATION OF ARRAYS FOR DAILY MEANS */
/*must be zero, because of summing up  */
/******************************************************/

void meandaynull()

{
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if(shayes == 1)
                meanSHADEday[i][j]   = 0.0;
            if(solyes == 1)       /*DIRECT RAD ON UNOBSTRUCTED HORIZ. SURFACE*/
                meanSOLHORday[i][j]  = 0.0;
            if(diryes == 1)
                meanDIRECTday[i][j]  = 0.0;  /*SLOPE CORRECTED CLEAR-SKY*/
            if(dir2yes == 1)
                meanDIRECT2day[i][j]  = 0.0;  /*SLOPE CORRECTED*/
            if(difyes == 1)
                meanDIFFUSday[i][j]  = 0.0;  /*SLOPE CORRECTED*/
            if(albyes == 1)
                meanALBEDOday[i][j]  = 0.0;
            if(gloyes == 1)
                meanGLOBALday[i][j]  = 0.0;
            if(swbyes == 1)
                meanSWBALday[i][j]   = 0.0;  /*SHORTWAVE RADIATION BALANCE*/
            if(linyes == 1)
                meanLONGINday[i][j]  = 0.0; /*LONGWAVE INCOMING*/
            if(loutyes == 1)
                meanLONGOUTday[i][j] = 0.0; /*LONGWAVE OUTGOING*/
            if(netyes == 1)
                meanNETRADday[i][j]  = 0.0;
            if(senyes == 1)
                meanSENSIBLEday[i][j]= 0.0;
            if(latyes == 1)
                meanLATENTday[i][j]  = 0.0;
            if(rainyes == 1)
                meanRAINENday[i][j]  = 0.0;
            if(balyes == 1)
                meanENBALday[i][j]   = 0.0;  /*ENERGY AVAILABLE FOR MELT*/
            if(melyes == 1)
                meanMELTday[i][j]    = 0.0;  /*WATER EQUIVALENT MELT*/
            if(ablyes == 1)
                meanABLAday[i][j]    = 0.0;  /*WATER EQUIVALENT ABLATION*/
            if(surftempyes == 1)
                meanSURFTEMPday[i][j]    = 0.0;  /*WATER EQUIVALENT ABLATION*/

            if((degreedaymethod == 1) && (posyes == 1))
                meanTEMPOSday[i][j]  = 0.0;
        }

    return;
}



/****************************************************************/
/* FUNCTION  meanallnull
	     RESERVE STORAGE FOR ARRAY
	     INITIALISATION OF ARRAYS FOR MEANS OF WHOLE PERIOD
	     called from start                                  */
/****************************************************************/

void meanallnull()

{
    if(shayes == 1)
        meanSHADEall    = matrixreservdouble(1,nrows,1,ncols);
    if(solyes == 1)
        meanSOLHORall   = matrixreservdouble(1,nrows,1,ncols);
    if(diryes == 1)
        meanDIRECTall   = matrixreservdouble(1,nrows,1,ncols);
    if(dir2yes == 1)
        meanDIRECT2all   = matrixreservdouble(1,nrows,1,ncols);
    if(difyes == 1)
        meanDIFFUSall   = matrixreservdouble(1,nrows,1,ncols);
    if(albyes == 1)
        meanALBEDOall   = matrixreservdouble(1,nrows,1,ncols);
    if(gloyes == 1)
        meanGLOBALall   = matrixreservdouble(1,nrows,1,ncols);
    if(swbyes == 1)
        meanSWBALall    = matrixreservdouble(1,nrows,1,ncols);
    if(linyes == 1)
        meanLONGINall   = matrixreservdouble(1,nrows,1,ncols);
    if(loutyes == 1)
        meanLONGOUTall  = matrixreservdouble(1,nrows,1,ncols);
    if(netyes == 1)
        meanNETRADall   = matrixreservdouble(1,nrows,1,ncols);
    if(senyes == 1)
        meanSENSIBLEall = matrixreservdouble(1,nrows,1,ncols);
    if(latyes == 1)
        meanLATENTall   = matrixreservdouble(1,nrows,1,ncols);
    if(rainyes == 1)
        meanRAINENall   = matrixreservdouble(1,nrows,1,ncols);
    if(balyes == 1)
        meanENBALall    = matrixreservdouble(1,nrows,1,ncols);
    if(melyes == 1)
        meanMELTall     = matrixreservdouble(1,nrows,1,ncols);
    if(ablyes == 1)
        meanABLAall     = matrixreservdouble(1,nrows,1,ncols);

    if(surftempyes == 1)
        meanSURFTEMPall     = matrixreservdouble(1,nrows,1,ncols);

    if((degreedaymethod == 1) && (posyes == 1))
        meanTEMPOSall = matrixreservdouble(1,nrows,1,ncols);

    if(surftempyes == 1)
        meanSURFTEMPall     = matrixreservdouble(1,nrows,1,ncols);

    /*INITIALISIEREN TO ZERO, not to nodata, because values are summed up*/

    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if(shayes == 1)    meanSHADEall[i][j]   = 0.0;
            if(solyes == 1)    meanSOLHORall[i][j]  = 0.0;
            if(diryes == 1)    meanDIRECTall[i][j]  = 0.0;
            if(dir2yes == 1)   meanDIRECT2all[i][j] = 0.0;
            if(difyes == 1)    meanDIFFUSall[i][j]  = 0.0;
            if(albyes == 1)    meanALBEDOall[i][j]  = 0.0;
            if(gloyes == 1)    meanGLOBALall[i][j]  = 0.0;
            if(swbyes == 1)    meanSWBALall[i][j]   = 0.0;
            if(linyes == 1)    meanLONGINall[i][j]  = 0.0;
            if(loutyes == 1)   meanLONGOUTall[i][j] = 0.0;
            if(netyes == 1)    meanNETRADall[i][j]  = 0.0;
            if(senyes == 1)    meanSENSIBLEall[i][j]= 0.0;
            if(latyes == 1)    meanLATENTall[i][j]  = 0.0;
            if(rainyes == 1)   meanRAINENall[i][j]  = 0.0;
            if(balyes == 1)    meanENBALall[i][j]   = 0.0;
            if(melyes == 1)    meanMELTall[i][j]    = 0.0;
            if(ablyes == 1)    meanABLAall[i][j]    = 0.0;
            if(surftempyes == 1)    meanSURFTEMPall[i][j]    = 0.0;

            if((degreedaymethod == 1) && (posyes == 1))
                meanTEMPOSall[i][j] = 0.0;

        }  /*FOR COLUMNS*/

    return;
}



/*==========================================================================*/
/****************************************************************************/
/*  FUNCTION  startinputdata                                                */
/*    OPEN AND READ GRID FILES DTM, drainage basin, glacier, slope, aspect  */
/*    OPEN FILE WITH CLIMATE DATA and READ UNTIL START                      */
/****************************************************************************/

void startinputdata()

{
    int  status;
    float **TMP;
    float  xcoorddummy, ycoorddummy;    /*for conversion coordinates into rows and columns*/
    int timeok=0;    /*needed to find start row in climate input file, 9/2009*/

    /*------------------------------------------------------------*/
    /***  OPEN INPUT-FILES                                      ***/
    /*------------------------------------------------------------*/

    /**** OPEN DIGITAL TERRAIN MODEL *******************/
    strcpy(dummy,inpath);    /*Path, wo file steht zum Namen kopieren*/
    strcat(dummy,namedgm);
    if ((indgm = fopen (dummy,"rb")) == NULL)  {
        printf("\n\n ERROR : Digital terrain model file '%s' not found !!!\n\n",
               namedgm);
        exit(1);
        fclose(outcontrol);
    }  /*ENDIF*/

    /**** OPEN DTM drainage basin FILE *****************/
    strcpy(dummy,inpath);
    strcat(dummy,namedgmdrain);
    if ((indgmdrain = fopen (dummy,"rb")) == NULL)  {
        printf("\n ERROR : DTM drainage basin file not found !!!\n\n");
        exit(2);
        fclose(outcontrol);
    }  /*ENDIF*/

    /**** OPEN DTM glacier FILE ************************/
    strcpy(dummy,inpath);
    strcat(dummy,namedgmglac);
    if ((indgmglac = fopen (dummy,"rb")) == NULL)  {
        printf("\n ERROR : DTM glacier file not found !!!\n\n");
        exit(3);
        fclose(outcontrol);
    }  /*ENDIF*/

    /*IF DIRECT RADIATION IS READ FROM FILES,SLOPE AND ASPECT NOT NEEDED
      FOR CALCULATIONS, BUT THE VALUES OF THE CLIMATE STATION GRID ARE
      WRITTEN TO COMMENT LINE IN ASCII OUTPUT FILES */

    /**** OPEN DTM gridslope FILE ************************/
    strcpy(dummy,inpath);
    strcat(dummy,namedgmslope);
    if ((indgmslope = fopen (dummy,"rb")) == NULL)  {
        printf("\n ERROR : DTM slope file not found !!!\n\n");
        exit(3);
        fclose(outcontrol);
    }  /*ENDIF*/


    /**** OPEN DTM gridaspect FILE ************************/
    strcpy(dummy,inpath);
    strcat(dummy,namedgmaspect);

    if ((indgmaspect = fopen (dummy,"rb")) == NULL)  {
        printf("\n ERROR : DTM aspect file not found !!!\n\n");
        exit(3);
        fclose(outcontrol);
    }  /*ENDIF*/


    /**** OPEN DTM skyview factor FILE ************************/
    if((methodglobal == 2) || (methodlongin == 2)) { /*only if direct and diffuse treated separately*/
        strcpy(dummy,inpath);
        strcat(dummy,namedgmskyview);

        if ((indgmskyview = fopen (dummy,"rb")) == NULL)  {
            printf("\n ERROR : DTM skyviewfactor file not found !!!\n\n");
            exit(3);
        }  /*ENDIF*/
    } /*endif methodglobal*/


    /**** OPEN DTM initial snow cover FILE ************************/
    if(methodinisnow == 2) {  /*start with initial snow cover (energy or temp index)*/
        strcpy(dummy,inpath);
        strcat(dummy,nameinitialsnow);

        if ((ininitialsnow = fopen (dummy,"rb")) == NULL)  {
            printf("\n ERROR : DTM initial snow file not found !!!\n\n");
            exit(3);
            fclose(outcontrol);
        }  /*ENDIF*/
    }  /*method*/

    /****************** CLIMATE *********************************/
    /*** OPEN CLIMATE DATA FILE ****/

    strcpy(dummy,inpath);
    strcat(dummy,nameklima);
    if ((inklima = fopen (dummy,"rt")) == NULL)  {
        printf("\n ERROR : Climate data file not found !!!\n %s \n\n",dummy);
        exit(4);
        fclose(outcontrol);
    }  /*ENDIF*/

    /*---------------------------------------------------------------------------*/
    /* READ WHOLE DTM - GRID FILES                                               */
    /*---------------------------------------------------------------------------*/

    /******** DIGITAL TERRAIN MODEL **********************************/

    x = (float*)calloc(13,sizeof(float));      /*STORAGE RESERVATION fuer 12 rows*/

    if ( (fread(&(x[1]),sizeof(float),12,indgm)) !=12 ) {    /*READ FIRST 12 ROWS*/
        printf("\n ERROR in file %s \n (File initial.c)\n",namedgm);
        exit(5);
    }

    ncols  = (int)x[1];
    nrows  = (int)x[2];      /*number col, rows      */
    xll    = x[3];
    yll    = x[4];      /*left lower corner     */
    cs     = x[5];
    nodata = x[6];      /*cellsize, missing data*/


    printf(" \n ncols,nrows,xll,yll,cellsize,nodata %d %d %5.1f %5.1f %5.1f %5.1f\n",
           ncols,nrows,xll,yll,cs,nodata);

    griddgm=matrixreserv(1,nrows,1,ncols);      /*SPEICHERRESERVIERUNG*/

    /*** READ GRID FILE : ELEVATIONS ***/
    if ((fread(&(griddgm[1][1]),sizeof(float),ncols*nrows,indgm)) != ncols*nrows)  {
        printf("\n ERROR in reading grid data \n");
        exit(6);
        fclose(outcontrol);
    }
    closefile(&indgm,namedgm);

    printf("    Gridfile %s opened and read \n",namedgm);

    if((rowclim > nrows)|| (colclim > ncols)) {
        printf("\n ERROR in input.txt\n");
        printf("  climate station grid cell is outside model domain !\n");
        printf("  adjust parameter rowclim (=%d) or/and colclim (=%d) !\n\n",rowclim,colclim);
        exit(6);
        fclose(outcontrol);
    }

    /******** READ DTM DRAINAGE BASIN ***********************************/
    griddgmdrain=matrixreserv(1,nrows,1,ncols);     /*SPEICHERRESERVIERUNG*/

    if ( (fread(&(x[1]),sizeof(float),12,indgmdrain)) !=12 )  {   /*READ FIRST 12 ROWS*/
        printf("\n ERROR in file %s \n (File initial.c)\n",namedgmdrain);
        exit(7);
        fclose(outcontrol);
    }
    if ((fread(&(griddgmdrain[1][1]),sizeof(float),ncols*nrows,indgmdrain)) != ncols*nrows) {
        printf("\n ERROR in reading grid data %s \n",namedgmdrain);
        exit(8);
        fclose(outcontrol);
    }
    closefile(&indgmdrain,namedgmdrain);

    printf("    Gridfile %s opened and read \n",namedgmdrain);


    /******** READ DTM GLACIER *******************************************/
    griddgmglac=matrixreserv(1,nrows,1,ncols);      /*SPEICHERRESERVIERUNG*/

    if ( (fread(&(x[1]),sizeof(float),12,indgmglac)) !=12 )  {            /*FIRST 12 ROWS*/
        printf("\n ERROR in file %s \n (File initial.c)\n",namedgmglac);
        exit(9);
    }
    if ((fread(&(griddgmglac[1][1]),sizeof(float),ncols*nrows,indgmglac)) != ncols*nrows)  {
        printf("\n ERROR in reading grid data %s \n",namedgmglac);
        exit(10);
    }
    closefile(&indgmglac,namedgmglac);

    printf("    Gridfile glacier: %s opened and read \n",namedgmglac);


    /******** READ DTM SLOPE *********************************************/
    SLOPE=matrixreserv(1,nrows,1,ncols);      /*SPEICHERRESERVIERUNG*/

    if ( (fread(&(x[1]),sizeof(float),12,indgmslope)) !=12 )  {
        printf("\n ERROR in file %s \n (File initial.c)\n",namedgmslope);
        exit(11);
        fclose(outcontrol);
    }
    if ((fread(&(SLOPE[1][1]),sizeof(float),ncols*nrows,indgmslope)) != ncols*nrows)  {
        printf("\n ERROR in reading grid data %s \n",namedgmslope);
        exit(12);
    }
    closefile(&indgmslope,namedgmslope);

    printf("    Gridfile slope: %s opened and read \n",namedgmslope);

    /*for calculation of direct radiation files, for files to be read in subsequent runs*/
    /*slope of climate station must be zero, because global radiation is measured horizontally*/
    if(slopestation == 0)
        SLOPE[rowclim][colclim] = 0.0;


    /*SLOPE of individual grid cells set to 0, for comparison of sim and meas global radiation*/
    /*   SLOPE[58][134] = 0.0;
       SLOPE[50][58] = 0.0;       */

    /******** READ DTM ASPECT *********************************************/
    ASP=matrixreserv(1,nrows,1,ncols);      /*SPEICHERRESERVIERUNG*/

    if ( (fread(&(x[1]),sizeof(float),12,indgmaspect)) !=12 )  {
        printf("\n ERROR in file %s \n (File initial.c)\n",namedgmaspect);
        exit(11);
    }

    if ((fread(&(ASP[1][1]),sizeof(float),ncols*nrows,indgmaspect)) != ncols*nrows)  {
        printf("\n ERROR in reading grid data %s \n",namedgmaspect);
        exit(12);
    }
    closefile(&indgmaspect,namedgmaspect);

    printf("    Gridfile aspect: %s opened and read \n",namedgmaspect);

    /******** READ DTM SKYVIEW FACTOR **************************************/
    if((methodglobal == 2) || (methodlongin == 2)) {
        SKYVIEW=matrixreserv(1,nrows,1,ncols);      /*SPEICHERRESERVIERUNG*/

        if ( (fread(&(x[1]),sizeof(float),12,indgmskyview)) !=12 )  {
            printf("\n ERROR in file %s \n (File initial.c)\n",namedgmskyview);
            exit(11);
        }

        if ((fread(&(SKYVIEW[1][1]),sizeof(float),ncols*nrows,indgmskyview)) != ncols*nrows)  {
            printf("\n ERROR in reading grid data %s \n",namedgmskyview);
            exit(12);
        }

        closefile(&indgmskyview,namedgmskyview);
    } /*endif methodglobal*/


    /**************************************************************************/
    /*    OPEN AND READ FILE WITH FIRN AREA ONLY                              */
    /*    needed 1. to determine if recession constant of firn is to be used  */
    /*      called once from startdischarg (below) and startddfopt (tindex.c) */
    /*    2. to determine if there is firn below snow (calculation of albedo) */
    /*      called from main (if methodalbedo = 2)                            */
    /**************************************************************************/

    if((methodinisnow == 2) || (disyes >= 1) || (methodsurftempglac == 4))
        /*READ FILE DELIMITING THE FIRN AREA*/     strcpy(dummy,inpath);
    {
        strcat(dummy,namedgmfirn);    /* copies name of outputfile to path */

        if ((firnfile = fopen(dummy,"rb")) == NULL) {   /*OPEN FIRNFILE*/
            printf("\n ERROR in opening firnfile\n\n");
            exit(8);
        }  /*END IF*/

        if ((fread(&(x[1]),sizeof(float),12,firnfile)) != 12) {
            printf("\n ERROR in firnfile\n\n");
            exit(8);
        }  /*END IF*/

        FIRN = matrixreserv(1,nrows,1,ncols); /*STORAGE RESERVATION*/

        if ((fread(&(FIRN[1][1]),sizeof(float),ncols*nrows,firnfile)) != ncols*nrows) {
            printf("\n ERROR in reading the firnfile: %s\n\n",dummy);
            exit(8);                                 /*READING THE FIRNFILE*/
        }  /*END IF*/

        fclose(firnfile);
    }  /*endif*/


    /******** READ DTM INITIAL SNOW COVER **************************************/
    if(methodinisnow == 2)
        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        /*CHR  { SNOW=matrixreserv(1,nrows,1,ncols); */ { /*RESERVE STORAGE FOR ARRAY*/
        SNOW=matrixreservdouble(1,nrows,1,ncols);    /*RESERVE STORAGE FOR ARRAY*/
        wrtSNOW=matrixreserv(1,nrows,1,ncols);
        TMP=matrixreserv(1,nrows,1,ncols);    /*CHR added RESERVE STORAGE FOR ARRAY*/
        /*============================================================*/

        if ( (fread(&(x[1]),sizeof(float),12,ininitialsnow)) !=12 )  {
            printf("\n ERROR in file %s \n (File initial.c)\n",nameinitialsnow);
            exit(11);
        }
        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        if ((fread(&(TMP[1][1]),sizeof(float),ncols*nrows,ininitialsnow)) != ncols*nrows)  { /*CHR change SNOW into TMP*/
            printf("\n ERROR in reading grid data %s \n",nameinitialsnow);
            exit(12);
            /*============================================================*/

            printf("    Gridfile %s opened and read \n",nameinitialsnow);
        }  /*method*/
        closefile(&ininitialsnow,nameinitialsnow);

        /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
        for (i=1; i<=nrows; i++) { /*CHR added*/
            for (j=1; j<=ncols; j++) {
                SNOW[i][j] = TMP[i][j];
            }
        }
        freematrix(TMP,1,nrows,1,ncols);
        /*============================================================*/


        /* TEST FOR MANIPULATION OF INITIAL SNOW COVER FOR CLIMATE SCENARIOS  1993 -15cm*/
        /*   snowscenario is added to initial snow cover, defined in variab.h (hidden option) */
        for (i=1; i<=nrows; i++)
            for (j=1; j<=ncols; j++) {
                if(griddgmglac[i][j] != nodata) {
                    SNOW[i][j] = SNOW[i][j] + snowscenario;
                    if(SNOW[i][j] < 0.)
                        SNOW[i][j] = 0.0;
                }
            }

    }  /*endif method*/

    printf("  All gridfiles opened and read !\n");
    printf("  Grid cell of climate station: elevation= %.2f  slope= %.2f  aspect= %.2f\n",griddgm[rowclim][colclim],SLOPE[rowclim][colclim],ASP[rowclim][colclim]);


    /*------------------------------------------------------------*/
    /*** READ CLIMATE DATA UNTIL STARTING DAY (jdbeg)           ***/
    /*------------------------------------------------------------*/
    /*Gelesen wird bis einschl. der Zeile vor dem eigentlichen Startzeitpunkt*/

    readrestofline(&inklima);
    readrestofline(&inklima);   /*first 2 rows*/

    if(timestep == 24) {   /*daily timestep*/
        timeok = 1;    /*needed for hourly timesteps; for daily the value in time does not matter*/
        if(jdbeg == 1) {
            jdbeg = 365;     /*preceeding timestep of 1 Jan is jd 365*/
            yearbeg = yearbeg - 1;   /*only read till day before the year to be calculated*/
        } else
            jdbeg = jdbeg - 1;
        /*needed, otherwise the starting day is found and one more row is read
        before the loop is exited, i.e. the starting day would not be computed*/
    }


    /*hourly timestep: different cases depending on midnight format*/
    if(timestep == 1) {
        if (formatclimdata == 3)
            /*midnight value of preceeding day is given JD of the day before the one to be calculated, e.g. 203, 24; day to be calculated is 204 hr 1*/
        {
            if (jdbeg == 1)
            {
                jdbeg = 365;
            } else {
            jdbeg = jdbeg - 1;
            }
        }
        if((formatclimdata == 1) || (formatclimdata == 2))
            /*day is day to be calculated, e.g. 204, 0 or  204, 24, day to be calculated is 204 hr 1*/
            timeok = 1;   /*time does not matter because first time jdbeg is found that is the right row (row before the one to be computed*/
    }  /*endif, hourly timestep*/

    /*read line by line until row before row to be included in calculations is found*/
    do {
        status = fscanf(inklima,"%f%f%f",&year,&jd,&zeit);   /*READ JULIAN DAY*/
        if (status == EOF) {
            printf("\n\n start julian day not found on climate file\n");
            printf(" check control file 'input.txt'\n (File initial.c)\n\n");
            fclose(inklima);
            exit(1);
        }  /*endif*/

        readrestofline(&inklima);             /*READ REST OF LINE*/


        if((zeit==24) && ((int)jd/daysscreenoutput == floor((int)jd/daysscreenoutput)))
            printf("reading climate data to find start row: year = %.2f day %.2f time %.2f\n",year,jd,zeit);


        if((timestep == 1) && (formatclimdata == 3) && ( zeit == 24))
            timeok = 1;   /*midnight value found*/

    } while ((jd != jdbeg) || (year != yearbeg) || (timeok != 1));
    /*last value read before leaving loop is midnight value, next row calculations start*/

    printf("\n STARTING DAY FOUND : year=%5.0f  day=%5.2f  time=%5.2f\n",year,jd,zeit);
    if(timestep == 24)     /*daily timestep*/
        printf(" NEXT ROW = JULIAN DAY %3.0f is first day to be computed\n",jd+1);

    /****** CONVERT COORDINATES FOR STAKE AND STATION OUTPUT LOCATIONS INTO ROW AND COLUMN
      CANNOT BE DONE EARLIER BECAUSE GRID HEADER MUST HAVE BEEN READ FIRST*/

    if(coordinatesyes < 3) {  /*not given in rows and cols yet*/

        for(i=1; i<=maxmeltstakes; i++) {
            if(coordinatesyes == 1) {    /*grid cell represents lower left corner of grid cell*/
                xcoorddummy = melt_xcoordinate[i];     /*for columns*/
                ycoorddummy = melt_ycoordinate[i];     /*for rows*/
            }
            /*move coordinates half a grid cell up and to the right in order to refer to a system
             where coordinates are center of grid cell; moving in order to use same equations below*/
            if(coordinatesyes == 2) {    /*grid cell represents center of grid cell*/
                xcoorddummy = melt_xcoordinate[i] + 0.5*cs;
                ycoorddummy = melt_ycoordinate[i] + 0.5*cs;
            }

            meltoutrow[i] = (int)( nrows - ((ycoorddummy - yll)/cs) ) + 1;    /*(int) removes decimals, no rounding*/
            /*if the y-coordinate of the stake cooincides with a grid y-coordinate*/
            if((ycoorddummy/cs) == floor(ycoorddummy/cs))
                meltoutrow[i] -= 1;    /*otherwise the stake will be moved to grid cell below although it hit the grid cell corner*/

            meltoutcol[i] = (int)( (xcoorddummy - xll)/cs ) + 1;

            if((meltoutrow[i] > nrows) || (meltoutcol[i] > ncols) || (meltoutrow[i] < 1) || (meltoutcol[i] < 1)) {
                printf("\nERROR: Coordinates for ablation stakes (see end of input.txt) are not compatible with DEM coordinates\n");
                printf("Make sure that they are from your study sites or turn off the option 'maxmeltstakes' (set to 0)\n");
                printf(" y-coord=%.3f x-coord=%.3f read from input.dat \n nrows=%d ncols=%d \n meltoutrow[i]=%d  meltoutcol[i]=%d  --> values must be between 1 and nrows/ncols\n\n",
                       melt_ycoordinate[i],melt_xcoordinate[i],nrows,ncols,meltoutrow[i],meltoutcol[i]);

                exit(20);
            }

            fprintf(outcontrol,"stakes: y-coord=%.3f x-coord=%.3f  row=%d col=%d dem=%.0f slope=%.3f firn=%.3f snow=%.0f\n",
                    melt_ycoordinate[i],melt_xcoordinate[i],meltoutrow[i],meltoutcol[i],
                    griddgm[meltoutrow[i]][meltoutcol[i]],SLOPE[meltoutrow[i]][meltoutcol[i]],
                    FIRN[meltoutrow[i]][meltoutcol[i]],SNOW[meltoutrow[i]][meltoutcol[i]]);
        }   /*end for, all stakes*/

        fprintf(outcontrol," Grid cell reference system = %d\n",coordinatesyes);
        fprintf(outcontrol," nrows=%d ncols=%d cs=%f\n",nrows,ncols,cs);
        fprintf(outcontrol," xll=%f yll=%f\n\n",xll,yll);

        /******  SAME THING FOR STATION OUTPUT LOCATIONS*/
        if(outgridnumber > 0) {
            for(i=1; i<=outgridnumber; i++) {  /*for each station to be written to output*/
                if(coordinatesyes == 1) {    /*grid cell represents lower left corner of grid cell*/
                    xcoorddummy = stn_xcoordinate[i];     /*for columns*/
                    ycoorddummy = stn_ycoordinate[i];     /*for rows*/
                }
                /*move coordinates half a grid cell up and to the right in order to refer to a system
                 where coordinates are center of grid cell; moving in order to use same equations below*/
                if(coordinatesyes == 2) {    /*grid cell represents center of grid cell*/
                    xcoorddummy = stn_xcoordinate[i] + 0.5*cs;
                    ycoorddummy = stn_ycoordinate[i] + 0.5*cs;
                }

                stnrow[i] = (int)( nrows - ((ycoorddummy - yll)/cs) ) + 1;    /*(int) removes decimals, no rounding*/
                /*if the y-coordinate of the stake cooincides with a grid y-coordinate*/
                if((ycoorddummy/cs) == floor(ycoorddummy/cs))
                    stnrow[i] -= 1;    /*otherwise the stake will be moved to grid cell below although it hit the grid cell corner*/

                stncol[i] = (int)( (xcoorddummy - xll)/cs ) + 1;

                if((stnrow[i] > nrows) || (stncol[i] > ncols) || (stnrow[i] < 1) || (stncol[i] < 1)) {
                    printf("\nERROR: Coordinates for station output locations (see outgridnumber in input.txt) are not compatible with DEM coordinates\n");
                    printf("Make sure that they are from your study sites and in the coordinate system as given by variable 'coordinatesyes' in input.txt\n");
                    printf(" y-coord=%.3f x-coord=%.3f read from input.txt \n nrows=%d ncols=%d \n stnrow[i]=%d  stncol[i]=%d  --> values must be between 1 and nrows/ncols\n\n",
                           stn_ycoordinate[i],stn_xcoordinate[i],nrows,ncols,stnrow[i],stncol[i]);
                    exit(20);
                }

                fprintf(outcontrol,"station output: y-coord=%.3f x-coord=%.3f  row=%d col=%d dem=%.0f slope=%.3f firn=%.3f snow=%.0f\n",
                        stn_ycoordinate[i],stn_xcoordinate[i],stnrow[i],stncol[i],
                        griddgm[stnrow[i]][stncol[i]],SLOPE[stnrow[i]][stncol[i]],
                        FIRN[stnrow[i]][stncol[i]],SNOW[stnrow[i]][stncol[i]]);
            }   /*end for, all stations*/
        }  /*end if, write grid cell output to file*/

    }  /*endif coordinatesyes*/

    return;
}



/*========================= OUTPUT =================================================*/
/*******************************************************************************/
/*  FUNCTION  startoutascii                                                    */
/*    OPEN ASCII-FILES FOR TIME SERIES OUTPUT (spatial means or station output)*/
/*   and WRITE FIRST 2 ROWS                                                    */
/*-----------------------------------------------------------------------------*/

void startoutascii()

{
    /*LG: kk is unused, can we delete it?*/
    //int kk; /*CHR in aid of more than 1 subsurfline file */
    char dummysubsurf[FNAME_LEN];

    /*-----------------------------------------------------------------------------*/
    /***OPEN ASCII-OUTPUTFILES FOR MODEL RESULTS OF SPATIAL MEAN and WRITE HEAD    */
    /*-----------------------------------------------------------------------------*/

    if(do_out_area == 1) {       /*output requested by user*/
        if(dat_or_txt == 1)
            strcpy(outareaname,"areamean.dat");
        else
            strcpy(outareaname,"areamean.txt");  /*better for GMT visualization, Carleen*/
        strcpy(dummy,outpath);
        strcat(dummy,outareaname);

        if ((outarea = fopen(dummy,"wt")) == NULL)  {
            printf("\n Error in opening mean output file \n (File initial.c)\n\n");
            exit(4);
        }  /*ENDIF*/

        /*in txt-file header lines are in different order than in .dat files*/
        if(dat_or_txt == 2)     /*first file name and variable in second line*/
            fprintf(outarea,"%s (Time series of spatial means integrated over area of DEM 2, and for mass balance DEM 3 (glacier)\n",outareaname);

        if(energymethod == 1) {   /*is set in meltmod.c, not in input.txt*/
            fprintf(outarea," year jd    time   shade exkorr solhor dirclearsky direct2 diffus  global ");
            fprintf(outarea," reflect albedo SWbal longin Lout LWbal netrad sensible  latent ground");
            fprintf(outarea," rain    enbal        melt  abla     massbal(cm) massbalcum(cm) surftemp\n");
        }

        if(degreedaymethod == 1) { /*is set in degree.c*/
            fprintf(outarea," year jd     time  shade exkorr  solhor dirclearsky");
            fprintf(outarea,"      melt  meantemp tempstation DDFcalc     massbal(cm) massbalcum(cm)\n");
        }

        if(dat_or_txt == 1)    /*variables first line, filename in second line*/
            fprintf(outarea,"%s (Time series of spatial means integrated over area of DEM 2, and for mass balance DEM 3 (glacier)\n",outareaname);

    } /*endif output areamean*/


    /*------------------------------------------------------------------------------*/
    /***OPEN ASCII-OUTPUTFILES FOR SNOW MODEL RESULTS OF INDIVUAL GRTDPOINTS, WRITE HEAD */
    /*------------------------------------------------------------------------------*/
    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    /*filenames consist of path+subsurf+name for individual model output given in input.txt*/
    if ((methodsurftempglac == 4) && (outgridnumber > 0)) {
        int k;
        strcpy(dummysubsurf,"subsurf");   /*add in front of outgridname*/
        for (k=1; k <= outgridnumber; k++) {
            strcpy(dummy,outpath);
            strcat(dummy,dummysubsurf);
            strcat(dummy,outgridname[k]);
            printf("snowmodel output:  %s\n",dummy);

            if ((outsubsurf[k] = fopen(dummy,"wt")) == NULL) {
                printf("\n Error in opening outsubsurf file \n (file initial.c) %s \n",dummy);
                exit(4);
            } /*ENDIF*/
        } /*ENDFOR*/
    }  /*ENDIF*/

    /*============================================================*/


    /*------------------------------------------------------------------------------*/
    /***OPEN ASCII-OUTPUTFILES FOR MODEL RESULTS OF INDIVUAL GRTDPOINTS, WRITE HEAD */
    /*------------------------------------------------------------------------------*/

    if (outgridnumber > 0) {               /*station output files requested by user*/
        for (i=1; i<=outgridnumber; i++) {
            strcpy(dummy,outpath);
            strcat(dummy,outgridname[i]);

            if ((outgrid[i] = fopen (dummy,"wt")) == NULL)  {
                printf("\n Error in opening grid output files \n (File initial.c)\n\n");
                exit(4);
            }  /*ENDIF*/

            if(dat_or_txt == 2) {  /*for .txt GMT-file, Carleen*/
                fprintf(outgrid[i],"%s  Output time series for one grid cell: elevation = %5.0f m   slope = %5.1f  aspect = %5.1f",
                        outgridname[i],
                        (griddgm[stnrow[i]][stncol[i]]),
                        (SLOPE[stnrow[i]][stncol[i]]),
                        (ASP[stnrow[i]][stncol[i]]));
                fprintf(outgrid[i],"   row = %5d  col = %5d\n",stnrow[i],stncol[i]);
            } /*endif*/

            if(energymethod == 1) {
                fprintf(outgrid[i],"year JD   time shade exkorr solhor dirclearsky direct2 ");
                fprintf(outgrid[i],"  diffus global reflect albedo  SWbal longin Lout LWbal ");
                fprintf(outgrid[i],"  netrad sensible latent ground rain    enbal   melten ");
                fprintf(outgrid[i],"  rain(mm) melt(mm) abla(mm) massbal(cm) massbalcum(cm)  ");
                fprintf(outgrid[i],"  surftemp exkhor ratio dirhor snow SR50mod  MBsum(cm)");

                /******** FOLLOWING COLUMNS ONLY IN CERTAIN CASES ****/
                if (methodsurftempglac == 4)
                    fprintf(outgrid[i]," snowfall subl runoff water deltawater deltarefreeze super slush surfwat coldsn coldtot ");

                if (outglobnet[i] == 1)  /*output with measured climate data of climate station */
                    fprintf(outgrid[i],"globmeas SWbmeas netmeas");
                fprintf(outgrid[i],"\n");
            } /*endif energymethod*/

            /* TEMPERATURE INDEX METHODS*/
            if(degreedaymethod ==1) {
                fprintf(outgrid[i]," year JD    time shade exkorr solhor dirclearsky melt");
                fprintf(outgrid[i]," tempinterpol tempstation DDFcalc massbal(cm) massbalcum(cm)\n ");
            }  /*endif degreedaymethod*/

            /*WRITE SECOND ROW FOR BOTH ENERGY OR DEGREE DAY FILES*/
            if(dat_or_txt == 1) {  /*not for .txt GMT-file, Carleen*/
                fprintf(outgrid[i],"%s Output time series for one grid cell:  elevation = %5.0f m   slope = %5.1f  aspect = %5.1f",outgridname[i],
                        (griddgm[stnrow[i]][stncol[i]]),(SLOPE[stnrow[i]][stncol[i]]),(ASP[stnrow[i]][stncol[i]]));
                fprintf(outgrid[i],"   row = %5d  col = %5d\n",stnrow[i],stncol[i]);
            } /*endif*/

        }  /*for  next file*/

    } /*if outgridnumber*/
    
    return;
}


/*******************************************************************************/
/*  FUNCTION  startmeltstakes
      OPEN FILE FOR ASCI-OUTPUT OF MELT FOR STAKE LOCATIONS
      OUTPUT OF CUMULATIVE MELTING AND CUMULATIVE MASS BALANCE                 */
/*******************************************************************************/

void startmeltstakes()

{
    /********** OUTPUT CUMULATIVE MELTING **********/
    if(dat_or_txt == 1)
        strcpy(outmeltstakename,"cumablation.dat");
    else
        strcpy(outmeltstakename,"cumablation.txt");  /*for GMT visualization, Carleen*/

    strcpy(dummy,outpath);
    strcat(dummy,outmeltstakename);

    if ((outmeltstakes = fopen (dummy,"wt")) == NULL)  {
        printf("\n Error in opening meltstake file \n (File initial.c)\n\n");
        exit(4);
    }  /*ENDIF*/

    fprintf(outmeltstakes,"Cumulative ablation in cm, S=stake number (S1 has coordinates of first row at end of input.txt, S2 the second etc) \n");      /*WRITE FIRST ROW*/
    fprintf(outmeltstakes,"Year  JD time ");      /*WRITE SECOND ROW*/
    for(i=1; i<=maxmeltstakes; i++)     /*WRITE NUMBER FOR ALL STAKES*/
        fprintf(outmeltstakes,"        s%d",i);
    fprintf(outmeltstakes,"\n");

    /********** OUTPUT CUMULATIVE MASS BALANCE **********/
    if(dat_or_txt == 1)
        strcpy(outmassbalstakename,"cummassbal.dat");
    else
        strcpy(outmassbalstakename,"cummassbal.txt");   /*for GMT visualization, Carleen*/

    strcpy(dummy,outpath);
    strcat(dummy,outmassbalstakename);

    if ((outmassbalstakes = fopen (dummy,"wt")) == NULL)  {
        printf("\n Error in opening mass balance stake file \n (File initial.c)\n\n");
        exit(4);
    }  /*ENDIF*/

    fprintf(outmassbalstakes,"Cumulative mass balance in cm, S=stake number (S1 has coordinates of first row at end of input.txt, S2 the second etc) \n");      /*WRITE FIRST ROW*/
    fprintf(outmassbalstakes,"Year  JD time ");      /*WRITE SECOND ROW*/
    /*    fprintf(outmassbalstakes,"Year  JD time (cm)");  */    /*WRITE FIRST ROW*/
    for(i=1; i<=maxmeltstakes; i++)     /*WRITE NUMBER FOR ALL STAKES*/
        fprintf(outmassbalstakes,"        s%d",i);
    fprintf(outmassbalstakes,"\n");

    if (methodsurftempglac == 4) {
        if(dat_or_txt == 1)
            strcpy(outmassbalstakename2,"cummassbalst.dat");
        else
            strcpy(outmassbalstakename2,"cummassbalst.txt");

        strcpy(dummy,outpath);
        strcat(dummy,outmassbalstakename2);

        if ((outmassbalstakes2 = fopen (dummy,"wt")) == NULL)  {
            printf("\n Error in opening mass balance stake file stake method \n (File initial.c)\n\n");
            exit(4);
        }  /*ENDIF*/

        fprintf(outmassbalstakes2,"Values in cm \n");      /*WRITE FIRST ROW*/
        fprintf(outmassbalstakes2,"Year  JD time ");      /*WRITE SECOND ROW*/
        /*    fprintf(outmassbalstakes,"Year  JD time (cm)");  */    /*WRITE FIRST ROW*/
        for(i=1; i<=maxmeltstakes; i++)     /*WRITE NUMBER FOR ALL STAKES*/
            fprintf(outmassbalstakes2,"        s%d",i);
        fprintf(outmassbalstakes2,"\n");
    }

    return;
}


/*******************************************************************************/
/*  FUNCTION  startspecificmassbalance
      OPEN FILE FOR ASCI-OUTPUT OF SPECIFIC MASS BALANCE FOR EACH YEAR
      ONLY IF summer and winter mass balance COMPUTED AND ONLY USEFUL FOR
      MULTIYEAR RUNS;  CALLED FROM MAIN ONCE
      NEW: 3/2004  */
/*******************************************************************************/

void startspecificmassbalance()

{
    if(dat_or_txt == 1)
        strcpy(outspecificmassbalname,"seasonalmassbal.dat");
    else    /*for GMT visualization, Carleen*/
        strcpy(outspecificmassbalname,"seasonalmassbal.txt");

    strcpy(dummy,outpath);
    strcat(dummy,outspecificmassbalname);

    if ((winterbalyes == 1) && (summerbalyes == 1)) {
        if ((outspecificmassbal = fopen (dummy,"wt")) == NULL)  {
            printf("\n Error in opening mass balance output file \n (File initial.c)\n\n");
            exit(4);
        }  /*ENDIF*/

        fprintf(outspecificmassbal,"Year1 Year2       bw(m)   bs      bn  cumbw(m)   cumbs    cumbn  winterjdbeg summerjdbeg summerjdend");
        if(scalingyes == 1)
            fprintf(outspecificmassbal," glacareaYear1(km2) glacareaYear2 areachange numbercells areachange2\n");
        else
            fprintf(outspecificmassbal,"\n");
    }
    return;
}


/*******************************************************************************/
/*  FUNCTION  opensnowfree                                                     */
/*      OPEN FILE FOR TIME SERIES OF HOW MANY PIXELS ARE SNOW FREE EVERY DAY   */
/*******************************************************************************/

void opensnowfree() {
    if(dat_or_txt == 1)
        strcpy(namesnowfree,"snowfree.dat");
    else      /*for GMT visualization, Carleen*/
        strcpy(namesnowfree,"snowfree.txt");

    openoutfile(&outsnowfree,outpath,namesnowfree);

    fprintf(outsnowfree," Year JD  time   allgridcells   snowfree ");
    fprintf(outsnowfree," percentsnowfree   percentsnowcovered \n");

    return;
}



/*******************************************************************************/
/*  FUNCTION  startarrayreserve
      RESERVE STORAGE AND INITIALIZE OUTPUT-ARRAYS FOR GRID MODEL RESULTS      */
/*******************************************************************************/

void startarrayreserve()

{
    tempint = matrixreserv(1,nrows,1,ncols);   /*AIR TEMPERATURE*/
    surface = matrixreserv(1,nrows,1,ncols);   /*classification: snow,ice...*/
    ALBEDO  = matrixreserv(1,nrows,1,ncols);
    /*needed also for temperature index methods, if discharge calculated
      to determine which recession constant to be used (k-values)*/
    RAIN = matrixreserv(1,nrows,1,ncols);

    if(methodprecipinterpol == 2)   /*read precipitation grids from file*/
        precipindexmap = matrixreserv(1,nrows,1,ncols);   /*AIR TEMPERATURE*/
    if(methodprecipinterpol == 3)   /*read precipitation grids from file*/
        precipreadgrid = matrixreserv(1,nrows,1,ncols);   /*AIR TEMPERATURE*/

    /*SHAPE = Beschattung von einem bestimmten split Teilinterval, Original von Joerg
      SHADE =  mittlere Beschattung des ganzen Zeitintervals (time step)
      wird gespeichert und kann als Output ausgegeben werden */

    if(directfromfile != 1) { /*not needed if direct read from file*/
        SHAPE  = matrixreserv(1,nrows,1,ncols);      /*SCHATTEN-GRID     */
        SHADE  = matrixreserv(1,nrows,1,ncols);      /*SCHATTEN-GRID     */
        Isenk  = matrixreserv(1,nrows,1,ncols);
        EXKORR = matrixreserv(1,nrows,1,ncols);      /*CORRECTION FACTOR */
        strlsumme = matrixreserv(1,nrows,1,ncols);
    }

    DIRECT = matrixreserv(1,nrows,1,ncols);        /*DIRECT SOLAR RAD  */
    if (methodsurftempglac == 4)
        DIRECTold = matrixreserv(1,nrows,1,ncols);   /*CHR for time interpolation*/

    /*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
    /*CHR   MELT   = matrixreserv(1,nrows,1,ncols);*/
    MELT   = matrixreservdouble(1,nrows,1,ncols);
    wrtMELT = matrixreserv(1,nrows,1,ncols);
    snowlayer=matrixreservdouble(1,nrows,1,ncols);
    meltlayer=matrixreservdouble(1,nrows,1,ncols);

    /*CHR     ENBAL    = matrixreserv(1,nrows,1,ncols);*/
    if((energymethod == 1)  || (methodsurftempglac == 4)) { /*also degree-day method*/
        wrtENBAL = matrixreserv(1,nrows,1,ncols);
        ENBAL    = matrixreservdouble(1,nrows,1,ncols);
        /*CHR     ABLA       = matrixreserv(1,nrows,1,ncols);*/
        wrtABLA    = matrixreserv(1,nrows,1,ncols);
        ABLA       = matrixreservdouble(1,nrows,1,ncols);
        RUNOFF     = matrixreserv(1,nrows,1,ncols);/*melt + rain*/
    }
    /*============================================================*/


    if(energymethod == 1) {  /*not needed if degree day method used*/
        GLOBAL   = matrixreserv(1,nrows,1,ncols);    /*GLOBAL RADIATION  */
        SWBAL    = matrixreserv(1,nrows,1,ncols);    /*SHORT-WAVE BALANCE*/
        LONGIN   = matrixreserv(1,nrows,1,ncols);
        NETRAD   = matrixreserv(1,nrows,1,ncols);    /*NET RADIATION     */
        SENSIBLE = matrixreserv(1,nrows,1,ncols);
        LATENT   = matrixreserv(1,nrows,1,ncols);

        rainenergy = matrixreserv(1,nrows,1,ncols);
        surftemp = matrixreserv(1,nrows,1,ncols);
        /*needed if longwave out variable or surface temperature iterated*/
        /*in any case needed in function sensible and latent*/
        ICEHEAT  = matrixreserv(1,nrows,1,ncols);

        if(methodglobal==2) {    /*direct and diffuse radiation separated*/
            DIRECT2 = matrixreserv(1,nrows,1,ncols);   /*DIRECT SOLAR RAD */
            DIFFUS  = matrixreserv(1,nrows,1,ncols);   /*DIFFUSE SOLAR RAD*/
        }

        if((methodsnowalbedo == 2) || (methodsnowalbedo >= 3)) {
            ALBALT     = matrixreserv(1,nrows,1,ncols);
            snowtofirn = matrixreserv(1,nrows,1,ncols);
            numbdays   = matrixreserv(1,nrows,1,ncols);
            ndbefsnow  = matrixreserv(1,nrows,1,ncols);
            ALBBEFSNOW = matrixreserv(1,nrows,1,ncols);
        }

        /*Surface temperature is iterated, surface not assumed melting, affects
         longwave outgoing radiation, is variable in space*/
        if(methodsurftempglac >= 2)     /*longout spatially variable*/
            LONGOUT  = matrixreserv(1,nrows,1,ncols);
        if(methodnegbal==2)      /*negative energy balances stored*/
            NEGBAL   = matrixreserv(1,nrows,1,ncols);

    } /*endif energy*/

    if(winterbalyes == 1) {
        WINTERBAL  = matrixreserv(1,nrows,1,ncols);
        initializeglacier2zero_nodata(nrows, ncols, WINTERBAL);   /*function is in grid.c*/
    }
    if(summerbalyes == 1) {
        SUMMERBAL  = matrixreserv(1,nrows,1,ncols);
        initializeglacier2zero_nodata(nrows, ncols, SUMMERBAL);
    }

    MASSBALcum = matrixreserv(1,nrows,1,ncols);    /*cum mass balance only over one mass-balance year*/
    initializeglacier2zero_nodata(nrows, ncols, MASSBALcum);
    
    MASSBALcum_all = matrixreserv(1,nrows,1,ncols);    /*cum mass bal over entire simulation period*/
    initializeglacier2zero_nodata(nrows, ncols, MASSBALcum_all);

    MASSBALgrid = matrixreserv(1,nrows,1,ncols);     /*New 10/2013: mass balance grid for each time step*/
    initializeglacier2zero_nodata(nrows, ncols, MASSBALgrid);

    if(degreedaymethod == 1) {
        TEMPOS  = matrixreserv(1,nrows,1,ncols);
        DDFCALC = matrixreserv(1,nrows,1,ncols);
    }


    /*INITIALISIEREN AUF MISSING VALUE*/
    /*nicht Null, da Berechnungen nur fuer einen Teil gemacht werden, aber
      das Outputfile ueber die ganze Grosse des DGM. Bei Initialisierung auf Null
      waeren Nuller im Outputfile am Rand */
    /*SHAPE UND EXKORR WERDEN IN FUNCTION schatten INITIALISIERT*/

    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            tempint[i][j] = nodata;
            DIRECT[i][j]  = 0.;   /*needed for both, energy balance and degree day*/
            MELT[i][j]    = nodata;
            surface[i][j] = nodata;    /*snow,ice ...*/
            RAIN[i][j]    = nodata;    /*rain mm/timestep*/
            if (methodsurftempglac == 4) RUNOFF[i][j] = nodata;

            if(griddgmdrain[i][j] == nodata)    /*outside area to be calculated*/
                ALBEDO[i][j]  = nodata;
            else
                ALBEDO[i][j]  = 0.5;  /*start value needed for mean albedo if albedo not generated*/
            /*but direct and diffuse interpolated separately*/

            if(energymethod ==1) {
                GLOBAL[i][j]  = nodata;
                SWBAL[i][j]   = nodata;
				LONGIN[i][j]  = nodata;
                NETRAD[i][j]  = nodata;
                SENSIBLE[i][j]= nodata;
                LATENT[i][j]  = nodata;
                ENBAL[i][j]   = nodata;
                rainenergy[i][j] = nodata;
                ICEHEAT[i][j] = 0;    /*must be zero, because is subtracted from enbal*/
                /*even if not used*/
                ABLA[i][j]       = nodata;

                if(griddgmdrain[i][j] == nodata)   /*outside area to be calculated*/
                    surftemp[i][j] = nodata;
                else
                    surftemp[i][j] = 0;  /*must be zero for start: e.g. turbulent fluxes*/

                if((i == rowclim) && (j == colclim))
                    surftemp[i][j] = 0;   /*for case if climate station outside glacier*/

                if(methodglobal==2) {    /*direct and diffuse radiation separated*/
                    DIRECT2[i][j] = nodata;
                    DIFFUS[i][j]  = nodata;
                }

                if(methodsnowalbedo>=2) {  /*snow albedo generated*/
                    numbdays[i][j] = ndstart;  /*number of days since snowfall at start day*/
                    ALBALT[i][j]   = nodata;
                    ALBBEFSNOW[i][j]  = 2;     /*snow albedo before snow fall*/
                    ndbefsnow[i][j]   = nodata;  /*number of days before snow fall*/

                    if(methodinisnow == 2) { /*initial snow cover*/
                        if(SNOW[i][j] == 0){  /*no snow*/
                            snowtofirn[i][j] = 0;   /*for snow albedo to drop from snow to firn*/
                        }else {
                            snowtofirn[i][j] = 1;   /*to avoid snow-to-firn-drop in albedo*/
                        }
                    }
                }

                if(methodsurftempglac>=2) {  /*surface temp computed for each grid cell*/
                    if (griddgmdrain[i][j] != nodata)    /*CHR to have nodata outside*/
                        LONGOUT[i][j] = LWout;    /*longwave outgoing variable in space*/
                    else
                        LONGOUT[i][j] = nodata;
                }
                /*value needed to calculate long incoming as a residual, because long out is calculated
                later, --> preceeding time step is used and needs to be initialized for 1. time step*/

                if(methodnegbal==2)      /*negative energy balances stored*/
                    NEGBAL[i][j] = 0;          /*must be zero, because 0 is start value*/

            } /*endif energy*/

            if(degreedaymethod == 1) {
                TEMPOS[i][j]  = nodata;
                DDFCALC[i][j] = nodata;
            }
        } /*endfor next grid cell*/

    /*GRIDOUT FOR EACH GRID*/
    switch(do_out) { /* INITIALISATION FOR ARRAYS WITH DAILY OR PERIOD MEANS */
    case 0:
        break;
    case 1:
        break;               /*Outputfile for every timestep*/
    case 2:
        meandayreserve();    /*Outputfile with daily means*/
        meandaynull();       /*set to zero*/
        break;
    case 3:
        meanallnull();       /*Outputfile with means for whole period*/
        break;
    case 4:
        meandayreserve();    /*daily means and means for whole period*/
        meandaynull();
        meanallnull();
    } /*switch*/

    return;
}           /*end arrayreserve*/



/*******************************************************************************/
/*  FUNCTION  glacierrowcol                                                    */
/*    FIND THE FIRST AND LAST ROW WITH NO MISSING VALUES IN THE DRAINAGE GRID  */
/*    AND FIND FOR EACH ROW WITH DRAINAGE BASIN GRIDS THE FIRST AND LAST COLUMN*/
/* to speed up the program, calculations are only done for basin grid cells    */
/* also count number of grid cell of DEM 2 and 3                               */
/* called once from main at the beginning before time loop                     */
/*******************************************************************************/

void glacierrowcol()

{
    int  firstrowfound=0,firstcolfound=0;

    firstcol = (int*)calloc(nrows+1,sizeof(int));   /*dynamisch Speicher reserv.*/
    lastcol  = (int*)calloc(nrows+1,sizeof(int));

    ndrain = 0;   nglac = 0;
    firstrow=0;
    lastrow=0;
    for (i=1; i<=nrows; i++) {
        firstcol[i] = 0;
        lastcol[i]  = 0;
    } /*for*/


    /*** FIND FIRST AND LAST COLUMN OF EACH ROW WHICH IS GLACIER GRID ***/
    for (i=1; i<=nrows; i++) {
        firstcolfound = 0;                  /*new for every row*/

        for (j=1; j<=ncols; j++) {
            if ((griddgmdrain[i][j]) != nodata) {   /*grid of drainage basin*/
                 ndrain += 1;              /*count number of drainage basin grid cells*/
                 
                 if ((griddgmglac[i][j]) != nodata)   /*grid of glacier*/
                   nglac += 1;      /*count number of glacierized cells for mass balance calculation, 10/2013*/
                   
                if (firstcolfound == 0) {       /*no drainage cell found before in this row*/
                    firstcol[i] = j;             /*first grid cell found for this row*/
                    firstcolfound = 1;           /*now find last columns*/
                    lastcol[i] = j;              /*wenn nur 1 grid in row valid value*/
                } else {
                    lastcol[i] = j;
                }
            } /*if*/

        } /*for next column*/

    } /*for rows*/


    /*** FIND FIRST AND LAST ROW AND WHICH CONTAINS GLACIER GRID ***/
    for (i=1; i<=nrows; i++) {
        if (firstcol[i] != 0) {             /*row contains glacier grids*/
            if (firstrowfound == 0) {       /*first row which contains glacier grids*/
                firstrow = i;
                firstrowfound = 1;
                lastrow = i;
            } /*if*/
            else {         /*row contain glacier grids and first row has been found*/
                lastrow = i;
            }
        } /*if*/
    } /*for*/


    /* OUTPUT TO CONTROLLING OUTPUT FILE modellog.txt ******/

    fprintf(outcontrol,"\n--- FUNCTION glacierrowcol, in initial.c : ---\n");
    fprintf(outcontrol,"first row containing grid cell to be calculated :%5d\n",firstrow);
    fprintf(outcontrol,"last row containing grid cell to be calculated   :%5d\n\n",lastrow);

    printf("\nfirst row containing grid cell to be calculated :%5d\n",firstrow);
    printf("last row containing grid cell to be calculated  :%5d\n\n",lastrow);

    /* for (i=firstrow;i<=lastrow;i++)
       {  fprintf(outcontrol,"row=%4d   first col = %5d  last col = %5d\n",
    	     i,firstcol[i],lastcol[i]);
       }
    */
    return;

}


/**************************************************************/
/*  FUNCTION  readclim                                        */
/*        READ ONE LINE OF CLIMATE DATA FROM FILE             */
/*          called from main every time step                  */
/**************************************************************/

void readclim()

{
    float  rowklima[51];        /*Array of one row of climate data*/
    int    status;

    for (i=0; i<=51; i++)
        rowklima[i]=0.0;

    /***READ ONE ROW***/
    for (i=1; i<=maxcol; i++) {
        status = fscanf(inklima, "%f", rowklima+i);

        if (status == EOF)  {
            printf("\n************** !!!  ERROR  !!! ************************");
            printf("\n End of file in climate file before last julian day to be computed\n");
            exit(2);
        }   /*ENDIF*/

    } /*ENDFOR*/

    readrestofline(&inklima);  /*READ REST OF LINE*/

    /*ASSIGN WHICH CLIMATE PARAMETER IN WHICH COLUMN*/

    year = rowklima[1];
    jd   = rowklima[2];

    if ((year < 1500) || (year > 3000)) {
        printf("\n************** !!!  ERROR in climate data file !!! ************************");
        printf("\n The first column (value=%.1f)is  outside the range 1500 to 3000; it should be the year \n",year);
        exit(2);
    }

    if(timestep == 24)    /*daily time step*/
        zeit = 24;         /*needed for creating consistent filenames for any time step*/
    else
        zeit = rowklima[3];

    temp = rowklima[coltemp];
    hum  = rowklima[colhum];
    wind = rowklima[colwind];
    glob = rowklima[colglob];
    ref  = rowklima[colref];
    net  = rowklima[colnet];
    prec = rowklima[colprec];
    cloud = rowklima[colcloud];
    qdischgem = rowklima[coldis];    /*NEW  7/2010*/

    if(methodtempinterpol == 2) {  /*time variant lapse rate read from file*/
        tempgrad = rowklima[coltempgradvarying];
        if((tempgrad < -2) || (tempgrad > 2)) {
            printf("\n initial.c: temperature lapse rate is unrealistic (must be between -2 and 2): %.2f\n\n",tempgrad);
            exitclimread();
        }
    }

    /****correct for bias between ERA and temperature at climate station/center of glacier****/
    temp += ERAtempshift;    /*adjust ERA data to elevation of climate station*/

    /* MANIPULATION OF AIR TEMPERATURE AND PRECIPITATION FOR CLIMATE SCENARIOS */
    if((monthtempgradyes == 1) || (monthtempscenyes == 1) || (monthprecipscenyes == 1))
        jd2monat();   /*converts julian day into which month it is*/

    if(monthtempgradyes == 1)    /*monthly variable temperature scenario*/
        temp += monthtempscen[monat];
    else
        temp += tempscenario;  /*constant temperature scenario for whole period*/

    if(monthtempgradyes == 1)    /*monthly variable precipitation scenario*/
        prec += prec/100*monthprecipscen[monat];   /*precipitation increased*/
    else
        prec += prec/100*precscenario;   /*precipitation increased*/

    if(prec < 0)
        prec = 0;


    /*CLIMATE DATA CONTROL*/
    if((temp < -60) || (temp > 50)) {
        printf("\n ERROR air temperature < -60 or > 50\n");
        exitclimread();
    }
    if((prec < 0) || (prec > 300)) {
        printf("\n ERROR precipitation < 0 or > 300\n");
        exitclimread();
    }

    if((disyes == 1) && (qdischgem != nodis))
        if((qdischgem <= 0) || (qdischgem > 2000)) {
            printf("\n ERROR measured discharge data (climate data file) year= %.0f day =%.0f:\n Q = %10.3f  Should be > 0 and < 2000 m3/s\n (initial.c)",jd,zeit,qdischgem);
            exitclimread();
        }

    if(energymethod == 1) {
        if((hum < 0) || (hum > 100)) {
            printf("\n ERROR relative humidity < 0 or > 100  %.0f jd=%8.2f %6.0f %8.2f\n", year,jd,zeit,hum);
            exitclimread();
        }
        if((wind < 0) || (wind > 200)) {
            printf("\n ERROR wind speed < 0 or > 90  %.0f jd=%8.2f %6.0f %8.2f\n", year,jd,zeit,wind);
            exitclimread();
        }
        if((glob < 0) || (glob > 1500)) {
            printf("\n ERROR global radiation < 0 or > 1500  %.0f jd=%8.2f %6.0f %8.2f\n", year,jd,zeit,glob);
            exitclimread();
        }


        /*&&&&&&&& ---------- FOR ANNALS OF GLACIOLOGY CAMBRIDGE PAPER -------- &&&&&6*/
        /*   if(glob > 300)   glob = 300;   */
        /*&&&&&&&&&&&&&6*/


        if(((ref < 0) || (ref > 1500)) &&  (methodlonginstation == 1) ) {
            printf("\n ERROR reflected radiation < 0 or > 1500  %.0f jd=%8.2f %6.0f %8.2f\n", year,jd,zeit,ref);
            exitclimread();
            if((ref > glob)) {
                printf("\n ERROR reflected rad %.2f > global radiation %.2f   yr=%.0f jd=%8.2f %6.0f\n",ref,glob,year,jd,zeit);
                exitclimread();
            }
        }

        if(methodlonginstation == 1) {
            if((net < -400) || (net > 1000)) {
                printf("\n ERROR net rad < -400 or > 1000  %.0f  jd=%8.2f %6.0f %8.2f\n", year,jd,zeit,net);
                exitclimread();
            }
        }
        if(methodlonginstation == 3) {
            if((cloud < 0) || (cloud > 10)) {
                printf("\n ERROR cloud < 0 or > 10  %.0f jd=%8.2f %6.0f %8.2f\n", year,jd,zeit,cloud);
                exitclimread();
            }
        }

        if(methodlonginstation == 2) {     /*take from measurements*/
            LWin  = rowklima[collongin];
            if((LWin < 50) || (LWin > 900)) {   /*CLIMATE DATA CONTROL*/
                printf("\n ERROR measured longwave incoming rad < 50 or > 900  %.0fjd=%.2f\n",year,jd);
                exitclimread();
            }
        }
        /*if method=1, the melting surface value is allocated in variab.h*/

        if(methodsurftempglac == 3) { /* use longwave outgoing measurements*/
            LWout = rowklima[collongout];
            if((LWout < 50) || (LWout > 330)) {
                printf("\n ERROR measured longwave outgoing rad < 50 or > 330  %.0f jd=%.2f time=%.0f\n",year,jd,zeit);
                exitclimread();
            }
        }

        /****correct for bias between ERA and wind speed at climate station/center of glacier****/
        wind += ERAwindshift;
        if(wind < 0)  wind = 0;

    } /*if energy method*/


    /*new: 3/2005: IF MIDNIGHT DATA NOT GIVEN AS 0 WITH NEW DAY*/
    /*SHOULD BE:   203.96  23,  204 0, etc.*/
    if(timestep != 24) {  /*only if not daily time step, this line added 11/2011*/
        if((formatclimdata == 2) && (zeit == 24))
            zeit = 0;
        if((formatclimdata == 3) && (zeit == 24)) {
            zeit = 0;
            jd = jd+1;
        }
    }


    /*then reformated again as done before*/

    /*IF THE JD ARE NOT PROVIDED AS REAL NUMBER, CONVERT TO REAL NUMBERS*/
    /*MUST BE DONE, OTHERWISE WRONG JD WRITTEN TO ASCII-FILES: JD=JD+1 IN writeout.c*/
    if(timestep != 24) {  /*not for daily timesteps*/
        if((jd == (int)jd) && (zeit !=0))
            jd = (int)jd + zeit/24.;

        /* REFORMAT MIDNIGHT TIME VALUE : e.g. jd=190 zeit=Null ==> jd=189 time=24 */
        if (zeit == 0.) {     /*Originalloggerdata : midnight = 0*/
            jd = jd-1.;     /*to find the proper direct radiation file*/
            zeit = 24.;
        }
    } else  /*in case of daily time steps time must always be 24*/
        zeit = 24;

    return;
}


/********************************************************************/
/*  FUNCTION  exitclimread                                          */
/*       EXITS PROGRAM IF READ INPUT DATA IS OUTSIDE LOGICAL VALUES */
/*          called from every time step                    */
/********************************************************************/

void  exitclimread() {
    printf("\n  input climate/discharge data beyond logical limits\n");
    printf("  wrong column may have been assigned to climate variable in question\n");
    printf("  check in input.txt\n\n");
    fclose(inklima);
    inklima = NULL;
    fclose(outcontrol);
    outcontrol = NULL;
    if(outmeltstakes) {
        fclose(outmeltstakes);
        outmeltstakes = NULL;
    }
    exit(5);
    return;
}


/********************************************************************/
/*  FUNCTION  readdatesmassbal                                      */
/*       reads the dates of mass balance measurements               */
/*          called from main once                                   */
/********************************************************************/

void  readdatesmassbal() {
    strcpy(dummy,inpath);
    strcat(dummy,namedatesmassbal);
    if ((indatesmassbal = fopen(dummy,"rt")) == NULL)  {
        printf("\n ERROR : file datesmassbal not found: %s !!!\n\n",dummy);
        exit(2);
        fclose(indatesmassbal);
    }  /*ENDIF*/

    readrestofline(&indatesmassbal);
    readrestofline(&indatesmassbal);

    /*READ ALL DATA OF FILE*/
    while(fscanf(indatesmassbal, "%f", &yearmeasmassbal) != EOF) {
        fscanf(indatesmassbal, "%f %f", &summerjdbegmeas[(int)yearmeasmassbal],&winterjdbegmeas[(int)yearmeasmassbal]);
        printf("mass bal meas  %5.0f %6.1f %6.1f\n",
               yearmeasmassbal,summerjdbegmeas[(int)yearmeasmassbal],winterjdbegmeas[(int)yearmeasmassbal]);
    }
    if(indatesmassbal) {
        fclose(indatesmassbal);
        indatesmassbal = NULL;
    }

    return;
}


/********************************************************************/
/*  FUNCTION  areaelevationbelts                                    */
/*       counts the number of grid cells for each elevation band    */
/*       called from main once,  May 2006                           */
/********************************************************************/

void areaelevationbelts()

{
    int jj, elevationfound;
    int numberloop = 0;   /*to avoid endless loop in elevation belt loop*/
    int numbergridcells = 0;
    float elevbelt;    /*lower limit of elevation band to be computed*/
    float elevbeltmax;

    for (jj=1; jj<=numberbelt; jj++)
        areabelt[jj] = 0;    /*number of grid cells per elevation band*/

    /*DETERMINE BELT BOUNDARIES FROM MINIMUM AND MAXIMUM ELEVATION AND BAND WIDTH*/
    /* elevations refer to lower boundary of the band*/
    statist(griddgmglac,x);   /*compute statistics of grid to get min/max elevation*/
    elevbeltmin = (int)(x[8]/beltwidth)*beltwidth;
    elevbeltmax = (int)(x[9]+beltwidth)/beltwidth*beltwidth;
    numberbelt = (elevbeltmax-elevbeltmin)/beltwidth;
    printf("  elevbeltmin= %.1f elevbeltmax %.1f numberbelt %d  max z %.1f min z %.1f\n",elevbeltmin,elevbeltmax,numberbelt,x[9],x[8]);

    /*compute number of pixels in each elevation band*/
    for (i=firstrow; i<=lastrow; i++)         /*for all grid cells of computed area*/
        for (j=firstcol[i]; j<=lastcol[i]; j++) {
            if (griddgmglac[i][j] != nodata) {
                elevationfound = 0;
                numberloop = 0;
                elevbelt = elevbeltmin;   /*start loop with lowest elevation, defined input.txt*/
                jj = 1;     /*index for each elevation belt profile data*/
                numbergridcells += 1;

                /*check for each grid cell elevation if it falls into lowest band, the next etc*/
                /*going through all bands upwards and each belt has its specific indice*/
                while(elevationfound == 0) {
                    numberloop += 1;
                    if((griddgmglac[i][j] >= elevbelt) && (griddgmglac[i][j] < (elevbelt+beltwidth) )) {
                        areabelt[jj] += 1;    /*number of gridd cells of same elevation belt*/
                        elevationfound = 1;
                    } else {
                        elevbelt += beltwidth;  /*check if grid cells belongs to next elevation belt*/
                        jj += 1;
                    }

                    if(numberloop >= 2000) {
                        printf("\n\n exit function computemassbalprofile: check input.txt if defined elevation belts exit\n\n");
                        exit(2);
                    }
                }  /*endwhile, elevation belt for grid cell elevation found*/
            }  /*endif*/
        }  /*endfor*/

    printf("\n function areaelevationbelts: number of glacier grid cells = %d\n",numbergridcells);
    numbergridcells = 0;  /*just for test if sum is correct, i.e. all elevations captured by one band*/
    for (jj=1; jj<=numberbelt; jj++)
        numbergridcells += areabelt[jj];   /*areabelt is number of grid cells per elevation band, not area*/
    printf(" sum of all grid cells of all elevation belts              = %d\n",numbergridcells);


    return;
}

