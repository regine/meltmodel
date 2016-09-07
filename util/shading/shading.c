/***********************************************************************
 * Copyright 1998, 2003, 2004, 2012 Regine Hock 
 *
 * This file is part of MeltMod.
 * 
 * MeltMod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * MeltMod is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MeltMod.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

/***********************************************************************/
/* PROGRAM  shading.c                                               */

/*   CALCULATION OF
       - SHADING
       - CORRECTIONFACTOR FOR CLEAR-SKY DIRECT RADIATION
           DUE TO SHADING, SLOPE AND EXPOSITION
       - CLEAR-SKY DIRECT RADIATION ON THE INCLINED SURFACE (SHADE CORRECTED)

       (mean of a time interval, how many calculation per time interval
        given by parameter split)
     if timestep = 1 und split = 1  ==> one calculation
                         split = 2  ==> weighted mean of 2 calculations per timestep
     INPUT:  DTM, slope, aspect  in standard format (see meltmod.c)
             Input time period to be calculated via keyboard
             area-specific input must be read from file shading.par

     OUTPUT-files :    _ = jd and time (created according to time period wanted)
     for every time step or for daily means
      sha_.bin   binary gridfiles: Shade-file  shade=1 sun=0
      exk_.bin       correction factor due to topographic effect
      dir_.bin       direct radiation
      zenith.dat  ASCII-file data of one grid cell :
         shade (1=shade or 0=sun)
         exkorr=correction factor for direct rad due to slope and aspect of grid cell)
         direct=direct radiation*exkorr (corrected for slope,aspect and shading)
         directhor= not corrected with exkorr,i.e. for horizontal surface
         zenith=zenith angle, azimuth=azimuth angle,  dekli=declination
         stuwi=Stundenwinkel (hour angle), rise=sunrise,  set=sunset

     all Outputfiles are optional (adjust in input_read)
     - The slope of the climate station grid can be set to zero
       (needed for energy balance model)                              */

/* smaller grid can be written to output, but computations are done for whole grid*/

/*   optimized shade search algorithm taken from Joerg Schulla, 1998 */
/*   Version 6/2003 Modification daily means  */
/*   Version 7/2003 Modification period means,  Regine Hock  */
/*   last changes 2/11/2004: error header output was wrong */
/***********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/*-------------------------------------------------------------*/
/*** VARIABLES                                               ***/
/*-------------------------------------------------------------*/
char   namedgm[101],namedgmslope[101],namedgmaspect[101];  /*filenames*/
char   inpath[101]="",outpath[101]="";    /*Path for Input/Output-files*/
char   nameshade[101],nameexkorr[101],namedirect[101];        /*OUTPUT-FILES*/
char   namesinglegrid[101]="";
char   dummy[101]="";
char  filenamein[101];       /*name of Steuerfile*/

FILE   *in;
FILE   *indgm=NULL,*indgmslope=NULL,*indgmaspect=NULL;    /*Inputfiles*/
FILE   *outshade=NULL,*outexkorr=NULL,*outdirect=NULL;    /*Binary grid files*/
FILE   *outsinglegrid=NULL;       /*ASCII-file for one grid cell*/

float  **griddgm;  /* 2 dim. Arrays elevation data */
float  **SLOPE,**ASP;           /* 2-D-Grids slope and aspect */
float  **SHAPE,**SHADE;    /* 2-D-Grids Shade Zeitpunkt, interval mean */
float  **EXKORR,**DIRECT;  /* 2-D-Grids correction factor, direct radiation*/
float  **SHADEday,**EXKORRday,**DIRECTday;  /* grids for daily means*/

double laenge, breite;                    /*longitude, latitude deg */
double reflongitude;       /*meridian time refers to*/
float  jdbeg,jdend, zeitbeg,zeitend;   /*period to be calculated*/
float  timestep;      /*time step of output files=averaging interval*/
float  *x=NULL;                      /* first 12 rows in grid files */
float  jd,zeit;                                  /*julian day, hours*/

int    ncols, nrows;           /*number of columns, rows*/
int    startrow,endrow,startcol,endcol,ncolsnew,wholegridyes;

float  xll,yll,cs,nodata;      /*left lower corner,cellsize,missing value*/
long   nrl,nrh,ncl,nch;        /* for Speicherreservierung Grid */

int    number=0; /*number of time steps calculated - if > 100days exist*/
/* to avoid endlos loop */

float  numberhour, numberhourend;     /*to find end of calculation in main*/
int    rowclim,colclim;   /*row and col of grid for output file of zenith angle etc*/
int    writeshade,writeexkorr,writedirect,writesinglegrid;  /*if output yes or no*/
int    dailyoutput;      /*if daily output yes or no*/
int    stationhorizyes;      /*if grid of climate station should be taken horizontal*/
float  daysdirect;  /*files produced only every daysdirect number of days*/
float  add2jd;     /*number to be added to jd of gridoutput, for calc over New years*/


/***** fuer Joergs shading routines****/
long   tag,monat,tagnr;
int    mon[13]= {31,31,28,31,30,31,30,31,31,30,31,30,31};               /*number of days in month*/
double stunde;
double dekli,timeeq,stuwi,cosz,sonnenstand;         /*Function sonnenhoehe*/
double cosOM;
double aspect,slope,azimuth,zenith;
float  InV,InVtag;   /* Intervallaenge in Min sowie Anzahl der Intervalle pro Tag*/

double  sunazimuthangle,sunaltitudeangle,sunzenithangle;
double  cosincidence;
double  pi  = 3.141592653;

double  timeequation[14];
/* gibt die Verschiebung der realen Sonnenzeit gegenueber der
   mittleren Sonnenzeit in min an (Elliptische Bahn) das Array steht
   an jeder Seite um eis ueber, d.h., es kann auch mit monat-1
   bzw. monat+1 gerechnet werden*/

float  timekorr; /*Zeitverschiebung der Zeitzone gegebueber den Zeitangaben der gemessenen Daten,
                    wenn die Daten in UTC angegeben werden, die Zeitzone aber MEZ ist, dann ist
                    timekorr=1.0 zu setzen usw.  */

float  hmax;     /* max elevation in indgm */
int    hmaxcalculated=0;     /* boolean meaning */

float  gewicht,sp,spa,spe;
float  cossunazim;
float  **Isenk;    /* potential clear sky incoming shortwave radiation */
float  **strlsumme;

float   se;
int    *indexx,*indexy; /* zwei Zeiger auf ein Array aus Integern  */
int	dim_index;       /*dimensioning av arrays above, max of nrows, ncols*/

double *sr,*ss;
float  faktor;
int    ii,jj,dx,dy,fx,fy,vx,vy; /* fuer die Geradengleichungen */
float  dz;

double sunr,suns;     /* Sonnenauf- und Sonnenuntergangszeiten in hh.hh (dezimal) */
int    exkorrindex,shapeindex;
int    nostrl;
int    cn,count;      /* nach je count Intervallen werden neue Werte berechnet */
int    split;
/* es werden jeweils split Teilintervalle gebildet. Fuer jedes dieser
  Teilintervalle wird die Mittelzeit genommen, fuer diese Mittelzeit wird dann
  die Expositionskorrektur gerechnet. Der Mittelwert aus allen Berechnungen
  wird als repraesentativ fuer das Intervall angenommen. Es werden nur Intervalle
  beruecksichtigt, an denen die Sonne ueber dem Horizont steht.
  Bei einem Zeitintervall von 24 Stunden kann split z.B. 24 sein, es werden dann
  nach jeweils count Rechenaufrufen (also count Tagen) fuer alle 24 Stunden
  die Korrekturwerte ermittelt un dann ain repraesentetiver Tageswert errechnet. */
int    i,j,k,m;
float  p;   /*air pressure*/

float dark_a,dark_e,inta,inte,split_temp,Anteil_dunkel,Anteil_hell;    /*neu 21.1.1997*/

float  slopealt;   /*original slope of climate station if set to 0, only for output file*/
float  topofatm = 0;    /*solar radiation at top of atmosphere*/
float  jd2,zeit2;       /*for creating gridfile name*/

float  radius2sun;

/****neu fuer Statistik ***/
float  numberexkor,numbershade;
float  maxexkor,maxshade,minexkor,minshade,sumexkor,sumshade,standabw;
float  mittelexkor,mittelshade;


/*******************************************************/
/* READ REST OF LINE                                   */
/*******************************************************/
void readrestofline(FILE **infile) {
    int c;
    while((c=getc(*infile)) != '\n')
        ;   /*empty command, just read, don´t do anything with c*/
    return;
}

/***********************************************/
/*   FUNCTION      input_read           ********/
/***********************************************/

#include <stdio.h>

void input_read()

/*READ FILENAMES AND VARIABLES*/
/*MUST BE ADJUSTED BY USER*/

{
    int inputok=0;   /*for check of user input 1=ok 0=not ok*/

    printf("\n====================================================================\n");
    printf("  PROGRAM  shading.c  : Calculation of shading, direct radiation ...\n");
    printf("====================================================================\n");

    /*    while(inputok == 0)

         { printf("\nfirst Julian day to be calculated  : ==>  ");
           scanf("%f",&jdbeg);
           printf("last Julian day to be calculated   : ==>  ");
           scanf("%f",&jdend);

          inputok = 1;
          if ((jdbeg > 366) || (jdbeg < 1))     inputok = 0;
          if ((jdend > 366) || (jdend < 1))     inputok = 0;
          if (jdbeg > jdend)     inputok = 0;
          if (inputok==0)
             printf("\n wrong input, try again, compute at least 2 days \n\n");

         }
    */

    /********READ INPUT DATA FROM NAMELIST FILE inpshade.dat *********************/
    /**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&**/

    strcpy(filenamein,"shading.par");    /* $$$$$$ NAME NAMELIST-FILE $$$$$ */

    if ((in = fopen (filenamein,"rt")) == NULL) {             /*OPEN FILE*/
        printf("\n Input-File 'shading.par' not found \n\n");
        printf("       File shading.c ");
        exit(1);
    }
    readrestofline(&in);
    fscanf(in,"%s",inpath);   readrestofline(&in);
    fscanf(in,"%s",outpath);  readrestofline(&in);
    printf("Input-Path  : %s\nOutput-Path : %s\n",inpath,outpath);

    fscanf(in,"%s",namedgm);        readrestofline(&in);
    fscanf(in,"%s",namedgmslope);   readrestofline(&in);
    fscanf(in,"%s",namedgmaspect);  readrestofline(&in);
    readrestofline(&in);

    fscanf(in,"%f",&jdbeg);  readrestofline(&in);
    fscanf(in,"%f",&jdend);  readrestofline(&in);
    readrestofline(&in);
    if ((jdbeg > 366) || (jdbeg < 1))     inputok = 0;
    if ((jdend > 366) || (jdend < 1))     inputok = 0;
    if (jdbeg > jdend)     inputok = 0;
    if (inputok==0)
        printf("\n wrong input: starting (%.1f) or ending (%.1f) day \n\n",jdbeg,jdend);

    fscanf(in,"%lf",&laenge);
    readrestofline(&in);
    fscanf(in,"%lf",&breite);
    readrestofline(&in);
    fscanf(in,"%lf",&reflongitude);
    readrestofline(&in);

    if ((laenge < -180) || (laenge > 180)) {
        printf("\n\n ERROR in input.dat: longitude = %.4f\n",laenge);
        exit(2);
    }
    if ((breite < -90) || (breite > 90)) {
        printf("\n\n ERROR in input.dat: latitude = %.4f\n",breite);
        exit(2);
    }
    if ((reflongitude < -180) || (reflongitude > 180)) {
        printf("\n\n input.dat: reference longitude = %.3f\n",reflongitude);
        exit(2);
    }

    fscanf(in,"%d",&rowclim);
    readrestofline(&in);
    fscanf(in,"%d",&colclim);
    readrestofline(&in);
    readrestofline(&in);

    fscanf(in,"%f",&timestep);
    readrestofline(&in);
    if ( ((int)24./timestep) != (floor((int)24./timestep)) ) {
        printf("\n\n input.dat: time step is wrong  n");
        printf("\n  dividing 24 by the time step must be an integer!\n");
        printf("    valid timesteps: e.g. 0.5,1,2,3,4,6,8,12\n\n");
        exit(2);
    }
    fscanf(in,"%d",&split);
    readrestofline(&in);
    fscanf(in,"%d",&dailyoutput);
    readrestofline(&in);
    if ((dailyoutput != 0) && (dailyoutput != 1) && (dailyoutput != 2)) {
        printf("\n ERROR in input file:\n");
        printf("   variable dailyoutput must be 0 (time step), 1 (daily) or 2 (period) \n\n");
        exit(2);
    }
    readrestofline(&in);

    fscanf(in,"%d",&writeshade);
    readrestofline(&in);
    fscanf(in,"%d",&writeexkorr);
    readrestofline(&in);
    fscanf(in,"%d",&writedirect);
    readrestofline(&in);
    fscanf(in,"%d",&writesinglegrid);
    readrestofline(&in);
    readrestofline(&in);

    fscanf(in,"%d",&wholegridyes);
    readrestofline(&in);
    fscanf(in,"%d%d",&startrow,&endrow);
    readrestofline(&in);
    fscanf(in,"%d%d",&startcol,&endcol);
    readrestofline(&in);
    readrestofline(&in);
    if ((wholegridyes != 0) && (wholegridyes != 1)) {
        printf("\n ERROR in input file:\n");
        printf("   variable wholegridyes (=%d) must be 0 or 1\n\n",wholegridyes);
        exit(2);
    }
    if (wholegridyes == 0)
        if ((startrow > endrow) || (startrow > endcol)) {
            printf("\n ERROR in input file:\n");
            printf("   variable startrow (%d) > endrow (%d) or startcol > endcol\n\n",startrow,endrow);
            exit(2);
        }

    fscanf(in,"%f",&daysdirect);
    readrestofline(&in);
    if(dailyoutput == 2)   /*period mean, computations every day*/
        daysdirect = 1;

    fscanf(in,"%f",&add2jd);
    readrestofline(&in);
    if(add2jd < 0)   /*period mean, computations every day*/
        printf("\n ERROR in input file: variable add2jd (%d) must be >=0\n\n",add2jd);

    fscanf(in,"%i",&stationhorizyes);
    readrestofline(&in);
    /*this is necessary, if the output of direct radiation is used as input
      for meltmod.c, because the ratio of glob/direct must refer to a horizontal
      surface, because the instrument is levelled horizontally,
      in this case the slope of the climate station grid is set to 0 */
    /******************************************************************************/
    fclose(in);

    zeitbeg = timestep;
    zeitend = 24;
    if(dailyoutput >= 1)
        timestep = 1;  /*for daily means computation at least every hour*/

    return;
}


/***************************************************/
/* FILE      screen.c                              */
/* FUNCTION  screen                                */
/* WRITE TO SCREEN *********************************/


void screen()

{
    printf("\n calculation jd = %5.1f  time = %5.2f  until",jdbeg,zeitbeg);
    printf("\n             jd = %5.1f  time = %5.2f \n",jdend,zeitend);

    printf("\n ******************************");
    printf("\n FILENAMES:\t%s  \n",namedgm);
    printf("\t\t%s\n \t\t%s \n",namedgmslope,namedgmaspect);
    printf("   longitude = %6.2lf \t latitude = %6.2lf \n",laenge,breite);
    printf("   timestep  = %4.1f\n",timestep);
    printf("   split     = %d\n",split);
    printf("   dailyoutput (0=timestep, 1=daily, 2=period) = %d\n",dailyoutput);
    if(wholegridyes == 0) {
        printf("  OUTPUT GRID SMALLER THAN INPUT FILES \n");
        printf("   start row = %6d  end row = %6d\n",startrow,endrow);
        printf("   start col = %6d  end col = %6d\n",startcol,endcol);
    }
    printf("   output every %4.0f th day\n",daysdirect);
    printf("   add %5.0f to jd\n",add2jd);
    if(stationhorizyes == 0)
        printf(" Station grids slope set to zero !\n");
    printf("\n ******************************\n");

    return;
}



/*===============================================================*/
/*****************************************************************/
/*  INITIALISATION FUNCTION :
       OPEN AND READ GRID FILES
       OPEN OUTPUT-FILES                                         */
/*****************************************************************/

/**************************************************************/
/* FUNCTION   matrixreserv                                    */
/* reserviert Speicher fuer eine Matrix m[nr..nrh][ncl..nch]  */
/*   aus Numerical Receipes in C                              */
/**************************************************************/

float **matrixreserv(long nrl, long nrh, long ncl, long nch)

{
    /*nrl = number of rows lower limit, nrh = high limit, c = columns*/
    long  i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
    float **m;         /*GRID FILE*/

    /*Speicher fuer die Zeiger auf die einzelnen Zeilen reservieren*/

    m = (float **)calloc((size_t)(nrow+1),sizeof(float*));
    if (!m) {
        printf("\n ERROR in storage reservation \n\n");
        exit(1);
    }
    m+=1;
    m-=nrl;

    /*Speicher fuer die einzelnen Zeilen reservieren und Zeiger auf sie setzen*/

    m[nrl]= (float *)calloc(nrow*ncol+1,sizeof(float));
    if (!m[nrl]) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }
    m[nrl] +=1;
    m[nrl] -= ncl;

    /*Zuweisen der Adressen der Zeilenzeiger mit dem Abstand von je ncol*sizeof(float) */

    for (i=nrl+1; i<=nrh; i++) m[i]=m[i-1]+ncol;

    return m;
}
/***************END SPEICHER RESERVIEREN  FUER MATRIX ********************/



/*==========================================================================*/
/****************************************************************************/
/*   FUNCTION  start                                                        */

/*    OPEN AND READ GRID FILES
          DIGITAL TERRAIN MODEL (ELEVATION OF ALL GRIDPOINTS)
          GRID WITH DRAINAGE BASIN, GLACIER, SLOPE, ASPECT
          OPEN FILE WITH CLIMATE DATA and READ UNTIL START                  */
/***************************************************************************/

void start()

{
    /*------------------------------------------------------------*/
    /***  OPEN INPUT-FILES                                      ***/
    /*------------------------------------------------------------*/

    /**** OPEN DIGITAL TERRAIN MODEL *******************/
    strcpy(dummy,inpath);
    strcat(dummy,namedgm);

    if ((indgm = fopen (dummy,"rb")) == NULL)  {
        printf("\n\n Digital terrain model file '%s' not found\n\n",dummy);
        exit(1);
    }  /*ENDIF*/


    /**** OPEN DTM gridslope FILE ************************/
    strcpy(dummy,inpath);
    strcat(dummy,namedgmslope);

    if ((indgmslope = fopen (dummy,"rb")) == NULL)  {
        printf("\n DTM slope file not found \n\n");
        exit(3);
    }  /*ENDIF*/


    /**** OPEN DTM gridaspect FILE ************************/
    strcpy(dummy,inpath);
    strcat(dummy,namedgmaspect);

    if ((indgmaspect = fopen (dummy,"rb")) == NULL)  {
        printf("\n DTM aspect file not found \n\n");
        exit(3);
    }  /*ENDIF*/


    /*---------------------------------------------------------------------------*/
    /* READ WHOLE DTM - GRID FILES                                               */
    /*---------------------------------------------------------------------------*/

    /******** DIGITAL TERRAIN MODEL **********************************/

    x = (float*)calloc(13,sizeof(float));      /*SPEICHERRESERVIERUNG fuer 12 rows*/

    /*first 12 rows must be read prior to storage reservation, because the number
      of rows and columns is not known, yet - other gridfiles first storage reservation
      then reading of whole file*/

    if ( (fread(&(x[1]),sizeof(float),12,indgm)) !=12 ) {    /*READ FIRST 12 ROWS*/
        printf("\n ERROR in file %s \n",namedgm);
        exit(5);
    }

    ncols  = (int)x[1];
    nrows  = (int)x[2];      /*number col, rows      */
    xll    = x[3];
    yll    = x[4];      /*left lower corner     */
    cs     = x[5];
    nodata = x[6];      /*cellsize, missing data*/

    nrl = 1;
    nrh = (long)nrows;      /*limit rows von 1 bis max number rows*/
    ncl = 1;
    nch = (long)ncols;      /*limit columns  "                    */

    printf(" \n ncols,nrows,xll,yll,cs,nodata %i %i %5.1f %5.1f %5.1f %5.1f\n",
           ncols,nrows,xll,yll,cs,nodata);

    if ((rowclim > nrows) || (colclim > ncols)) {
        printf("\n ERROR in input file:\n");
        printf("   variable rowclim or colclim larger than dimension of grid \n");
        printf("   nrows = %d  ncols = %d\n\n",nrows,ncols);
        exit(2);
    }

    griddgm=matrixreserv(1,(long)nrows,1,(long)ncols);      /*SPEICHERRESERVIERUNG*/

    /*CHECK IF GRID TO BE WRITTEN TO OUTPUT IS NOT LARGER THAN INPUT GRID*/
    if(wholegridyes == 0) { /*only reduced grid to output*/
        if ((startrow > nrows) || (endrow > nrows)) {
            printf("\n ERROR in input file:\n");
            printf("   variable startrow endrow > number of rows in grid\n\n");
            exit(2);
        }
        if ((startcol > ncols) || (endcol > ncols)) {
            printf("\n ERROR in input file:\n");
            printf("   variable startcol endcol > number of columns in grid\n\n");
            exit(2);
        }
    } /*endif size of output grid*/

    /*** READ GRID FILE : ELEVATIONS ***/
    if ((fread(&(griddgm[1][1]),sizeof(float),ncols*nrows,indgm)) != ncols*nrows)  {
        printf("\n ERROR in reading grid data \n");
        exit(6);
    }
    fclose(indgm);


    /******** READ DTM SLOPE *********************************************/

    SLOPE=matrixreserv(1,(long)nrows,1,(long)ncols);      /*SPEICHERRESERVIERUNG*/

    if ( (fread(&(x[1]),sizeof(float),12,indgmslope)) !=12 )  {
        printf("\n ERROR in file %s \n",namedgmslope);
        exit(11);
    }
    if ((fread(&(SLOPE[1][1]),sizeof(float),ncols*nrows,indgmslope)) != ncols*nrows)  {
        printf("\n ERROR in reading grid data %s \n",namedgmslope);
        exit(12);
    }
    fclose(indgmslope);

    /*** IF OUTPUT OF DIRECT RADIATION GRIDS ARE TO BE USED AS INPUT FOR
         meltmod.c THE GRID OF THE CLIMATE STATION MUST BE ASSUMED TO BE
         HORIZONTAL, BECAUSE RADIATION MEASUREMENTS ARE DONE HORIZONTALLY.
         THIS IS NEEDED FOR THE CALCULATION OF THE RATIO GLOB/DIRECT ***/
    slopealt = SLOPE[rowclim][colclim];   /*needed only for output file 1.row*/
    if (stationhorizyes == 0)
        SLOPE[rowclim][colclim] = 0.0;     /* !!!!!!!!!!!!!!!!!!! */


    /******** READ DTM ASPECT *********************************************/

    ASP=matrixreserv(1,(long)nrows,1,(long)ncols);      /*SPEICHERRESERVIERUNG*/

    if ( (fread(&(x[1]),sizeof(float),12,indgmaspect)) !=12 )  {
        printf("\n ERROR in file %s \n",namedgmaspect);
        exit(11);
    }
    if ((fread(&(ASP[1][1]),sizeof(float),ncols*nrows,indgmaspect)) != ncols*nrows)  {
        printf("\n ERROR in reading grid data %s \n",namedgmaspect);
        exit(12);
    }
    fclose(indgmaspect);

    return;
}


/*********************************************/
/* START FOR OUTPUT FILES - CALLED ONLY ONCE */
/*  STORAGE RESERVATION */
/*  OUTPUT ASCII-FILE FOR ONE GRID CELL */
/*   ZENITH ANGLE, AZIMUTH, SHADE, EXKORR, DIRECT, GLOBAL */
/*********************************************/

void startoutput()

{
    SHAPE  = matrixreserv(1,(long)nrows,1,(long)ncols);     /*STORAGE RESERVATION*/
    SHADE  = matrixreserv(1,(long)nrows,1,(long)ncols);
    EXKORR = matrixreserv(1,(long)nrows,1,(long)ncols);
    DIRECT = matrixreserv(1,(long)nrows,1,(long)ncols);
    Isenk  = matrixreserv(1,(long)nrows,1,(long)ncols);
    strlsumme = matrixreserv(1,(long)nrows,1,(long)ncols);

    if(dailyoutput >= 1) {    /*ARRAYS FOR DAILY SUMS*/
        SHADEday  = matrixreserv(1,(long)nrows,1,(long)ncols);
        EXKORRday = matrixreserv(1,(long)nrows,1,(long)ncols);
        DIRECTday = matrixreserv(1,(long)nrows,1,(long)ncols);
    }  /*endif*/

    if ((dailyoutput == 2) )  /*set to zero, needed here for period mean */
        /*for daily mean it is done in schatten() at 1 am every day*/
    {
        for (i=1; i<=nrows; i++)
            for (j=1; j<=ncols; j++) {
                EXKORRday[i][j] = 0.0;
                SHADEday[i][j]  = 0.0;
                DIRECTday[i][j] = 0.0;
            }  /*endfor*/
    }  /*endif*/


    if (writesinglegrid == 1) {  /*output ASCII-file*/
        strcpy(dummy,outpath);
        strcpy(namesinglegrid,"zenith.dat");
        strcat(dummy,namesinglegrid);

        if ((outsinglegrid = fopen(dummy,"wt")) == NULL) {
            printf("\n ERROR in zenith file");
            printf("\n   FUNCTION opensinglegrid, check if output path exists\n\n");
            exit(10);
        }
        /*WRITE FIRST 2 ROWS - COMMENTS*/
        fprintf(outsinglegrid,"File zenith.dat   data for row = %d  col = %d  (1=shade 0=sun)",
                rowclim,colclim);
        fprintf(outsinglegrid,"  slope = %5.2f, slope used = %5.2f ",slopealt,SLOPE[rowclim][colclim]);
        fprintf(outsinglegrid,"split = %3d\n",split);

        fprintf(outsinglegrid,"jd     time  shade  exkorr direct directhor topofatm  zenith   ");
        fprintf(outsinglegrid,"azimuth  dekli stuwi sunrise  set\n");

    } /*endif*/

    return;
}


/***********************************************************************/
/*   FUNCTION   opengridoutput */
/*              OPEN OUTPUT-GRID-FILES FOR SHADE DISTRIBUTION AND
                     CORRECTION FACTOR GLOBAL RADIATION
                called every time step, because file get new name      */
/***********************************************************************/

void opengridoutput()

{
    if (writeshade == 1) {          /*SHADE FILE*/
        if(dailyoutput < 2)   /*every time step or daily output*/
            sprintf(nameshade,"sha%03d%02d.bin",(int)(jd2),(int)zeit2);
        else
            sprintf(nameshade,"sha%03d_%03d.bin",(int)jdbeg,(int)jdend);

        strcpy(dummy,outpath);
        strcat(dummy,nameshade);

        if ((outshade = fopen(dummy,"wb")) == NULL) {
            printf("\n ERROR in opening shade file (check if output path exists)");
            printf("\n   FUNCTION opengridoutput.c\n\n");
            exit(10);
        }
    } /*endif*/

    if (writeexkorr == 1) {         /* CORRECTION FACTOR DIRECT RADIATION */
        if(dailyoutput < 2)   /*every time step or daily output*/
            sprintf(nameexkorr,"exk%03d%02d.bin",(int)(jd2),(int)zeit2);
        else
            sprintf(nameexkorr,"exk%03d_%03d.bin",(int)jdbeg,(int)jdend);

        strcpy(dummy,outpath);
        strcat(dummy,nameexkorr);

        if ((outexkorr = fopen(dummy,"wb")) == NULL) {
            printf("\n ERROR in opening correction factor file (check if output path exists)");
            printf("\n   FUNCTION opengridoutput.c\n\n");
            exit(10);
        }
    } /*endif*/

    if (writedirect == 1) {        /* DIRECT RADIATION FILE */
        if(dailyoutput < 2)   /*every time step or daily output*/
            sprintf(namedirect,"dir%03d%02d.bin",(int)(jd2),(int)zeit2);
        else
            sprintf(namedirect,"dir%03d_%03d.bin",(int)jdbeg,(int)jdend);

        strcpy(dummy,outpath);
        strcat(dummy,namedirect);

        if ((outdirect = fopen(dummy,"wb")) == NULL) {
            printf("\n ERROR in opening direct radiation file (check if output path exists)");
            printf("\n   FUNCTION opengridoutput.c\n\n");
            exit(10);
        }

    } /*endif*/

    return;
}



/**********************************************************************************/
/*   FILE :  globcor.c   of program meltmod.c                                   ***/
/*           BERECHNUNG DER ABSCHATTUNG und
                        KORREKTURFAKTOR fuer kurzwellige direkte Strahlung      ***/
/*                      Mittel fuer ein Zeitinterval (time step)                  */
/*   18.10.96 */
/**********************************************************************************/

/*   paar Solargeometriefunktionen und optimierteAbschattungs-Algorithmus von:
     Joerg Schulla, Stand      : 1.9.1995, Version    : 1.0
     Zweck      : Korrektur der direkten Sonnenstrahlung in Abhaengigkeit
                von Hangneigung, Exposition und ABSCHATTUNG!
                Abschattung mit Joergs Verfahren:
                Strahlverfolgung mit optimiertem Rastersuchverfahren
****************************************************************************/



/*==============================================================*/
/*** FUNCTION jd2monat                                        ***/
/*            CONVERT JULIAN DAY TO MONTH AND DAY               */
/*==============================================================*/

void jd2monat()

{
    if ((int)jd <=31)     {
        monat=1;
        tag=jd;
        return;
    }
    if ((int)jd <=59)     {
        monat=2;
        tag=jd-31;
        return;
    }
    if ((int)jd <=90)     {
        monat=3;
        tag=jd-60;
        return;
    }
    if ((int)jd <=120)    {
        monat=4;
        tag=jd-90;
        return;
    }
    if ((int)jd <=151)    {
        monat=5;
        tag=jd-120;
        return;
    }
    if ((int)jd <=181)    {
        monat=6;
        tag=jd-151;
        return;
    }
    if ((int)jd <=212)    {
        monat=7;
        tag=jd-181;
        return;
    }
    if ((int)jd <=243)    {
        monat=8;
        tag=jd-212;
        return;
    }
    if ((int)jd <=273)    {
        monat=9;
        tag=jd-243;
        return;
    }
    if ((int)jd <=304)    {
        monat=10;
        tag=jd-273;
        return;
    }
    if ((int)jd <=334)    {
        monat=11;
        tag=jd-304;
        return;
    } else {
        monat=12;
        tag=jd-334;
    }

    return;
}


/*=======================================================================*/
/************** DEKLINATION **********************************************/
/*=======================================================================*/

double deklination(long tag, long monat) {
    return -23.4 * cos( (jd+10)/58.09155423); /* 58.09 = 2*pi/365  */
}


/*=======================================================================*/
/************** REALE SONNENZEIT *****************************************/
/*=======================================================================*/

double gettimeequation(long tag,long monat)
/* gibt die Anzahl Stunden an, die zur aktuellen mittleren Sonnenzeit
   hinzuaddiert werden muessen, um auf die reale Sonnenzeit zu kommen
      called in getsuncoordinates    */
{
    return (double)(mon[(int)monat]-tag+1)/(double)mon[(int)monat]*(timeequation[(int)monat]/60.0) +
           (double)(tag-1)/(double)mon[(int)monat]*(timeequation[(int)monat+1]/60.0);
    /* einfache lineare Interpolation */
}


/*=======================================================================*/
/************** SONNENHOEHE   not used here ******************************/
/*=======================================================================*/

double sonnenhoehe(long tag,long monat,double breite,double laenge,double stunde) {
    timeeq = gettimeequation(tag,monat);    /*!!!!!!!!!!!!!!*/
    /*Deklination d. Sonne gegen d. Aequatorialebene haengt nur vom Tag ab (1.Naeherung)*/
    dekli  = deklination(tag,monat);        /*!!!!!!!!!!!!!!*/

    stunde = stunde-(reflongitude-laenge)/15+timeeq;
    stuwi  = 15*(12-stunde)/180*pi;
    cosz = sin(breite*pi/180)*sin(dekli*pi/180)+cos(breite*pi/180)*cos(dekli*pi/180)*cos(stuwi);
    sonnenstand = acos(cosz)*180/pi;
    if (cosz<0) sonnenstand = sonnenstand-90;
    else sonnenstand = 90-sonnenstand;
    return sonnenstand;
}


/*=======================================================================*/
/************** SUN COORDINATES          *********************************/
/*=======================================================================*/

void getsuncoordinates(long tag,long monat,double breite,double laenge,double stunde,
                       double *sunazimuth,double *sunaltitude)

/* liefert bei Eingabe der geographischen Koordinaten in Grad und des Tages, Monats und
    der Stunde die Koordinaten der Sonne azimuth und altitude zurueck (in RADIANT!) */
{
    /* neu */

    breite = fmod(breite,360.0);
    laenge = fmod(laenge,360.0);

    timeeq = gettimeequation(tag,monat);    /*!!!!!!!!!!!!!!*/
    dekli  = deklination(tag,monat);        /*!!!!!!!!!!!!!!*/
    /* Stunde bleibt nun immer zwischen 0 und 24 */
    stunde = fmod(stunde-(reflongitude-laenge)/15+timeeq+24.0,24.0);   /*time correction wegen Lage weg vom Zeitmeridian*/
    stuwi  = (12-stunde)/12 * pi;
    breite *= pi/180;      /*convert Grad to Rad*/
    dekli  *= pi/180;
    cosz   = sin(breite)*sin(dekli)+cos(breite)*cos(dekli)*cos(stuwi);   /*cosZenithangle*/
    cosOM  = (sin(dekli)*cos(breite) - cos(dekli)*sin(breite)*cos(stuwi))/sqrt(1-cosz*cosz);
    *sunaltitude = acos(cosz); /* sunaltitude ist eigentlich der zenithwinkel! */

    /**** alt ***/
    /* Sonnenstand ueber Horizont */
    /*  if (*sunaltitude <= pi/2) *sunaltitude = pi/2 - *sunaltitude;
    		    else *sunaltitude = -(*sunaltitude - pi/2);   */
    /* Sonnenstand unter Horizont */
    /*** NEU 7/1999 ***/
    *sunaltitude = pi/2.0 - *sunaltitude;   /* Sonnenstand ueber Horizont */
    if((cosOM>-1.0)&&(cosOM<1.0)) *sunazimuth=acos(cosOM);
    else *sunazimuth=pi;
    /******************/
    /*  alt
    *sunazimuth = acos(cosOM);   */

    if (stunde > 12) *sunazimuth = 2*pi - *sunazimuth;
    return;
}


/*=======================================================================*/
/************* ANGLE SUN - EBENENNORMALE  ********************************/
/*=======================================================================*/

double getcosincidence(double aspect,double slope,double azimuth,double zenith)

/* berechnet den cosinus des WInkels zwischen Sonnenvektor und Ebenennormale
   alle Angaben in Radiant, auch die Eingabedaten! */
{
    return cos(slope)*cos(zenith) + sin(slope)*sin(zenith)*cos(azimuth - aspect);
}


/*=======================================================================*/
/******      SUNRISE      ************************************************/
/*=======================================================================*/

void sunrise(long tag,long monat,double breite,double laenge,double *sr,double *ss)

{
    double z,dekli,timeeq;
    int    i;
    /* neu */

    timeeq = gettimeequation(tag,monat);   /*!!!!!!!!!!!!!!*/
    dekli  = deklination(tag,monat);       /*!!!!!!!!!!!!!!*/
    breite = fmod(breite,360.0);     /*neu 21.1.97*/
    laenge = fmod(laenge,360.0);     /*neu 21.1.97*/

    z = sin(breite*pi/180)/cos(breite*pi/180)*sin(dekli*pi/180)/cos(dekli*pi/180);
    if (fabs(z)<=1) {
        /* da es ansonsten keinen Sonnenuntergang gibt! */

        if (z!=0) *sr = 12/pi * acos(z);
        else *sr = 6.0;
        *ss = 24 - *sr;
        *sr = *sr + (reflongitude-laenge)/15-timeeq;
        *ss = *ss + (reflongitude-laenge)/15-timeeq;
    } else {
        *ss = -1;
        /*    *ss = 24.1; */
        *sr = -1.0;
    }
    return;
}


/*=======================================================================*/
/******      RADIUSSUNEARTH      *****************************************/
/*   calculates the squared ratio of mean distance between sun and earth */
/*   to the distance at a specific day                                   */
/*     from climatology lecture: Ohmura, p.33 */
/* new: 15.1.1997 */
/*=======================================================================*/

void radiussunearth() {
    float teta;

    teta = (2*pi*jd)/365;
    radius2sun = 1.000110+0.034221*cos(teta)+0.001280*sin(teta)+
                 0.000719*cos(2*teta)+0.000077*sin(2*teta);
    return;
}




/* ------------- HAUPTFUNKTION -- called in main -------------------------- */

void schatten()

{
    float  z;       /*elevation*/
    int    *indexx,*indexy;
    float  P0 = 101325.0;           /* standard pressure  [Pa] */

    /*** $$$  NEU OPTION : SCHATTENGRIDS DER SUBINTERVALLE AUF FILE SCHREIBEN 1=yes ***/
    int    subintfileyes=0;        /*** $$$ **********************/
    /*zum steuern, ob alle Schattengrids aller Subintervalle auf ein File
      geschrieben werden sollen, Name: sub__.__ mit Zeit als real*/

    FILE   *shadesubint=NULL;
    char   namesubint[61];
    float  faktorstation;  /*NEU NUR KONTROLL-SCREEN-OUTPUT $$$ */

    timekorr = 0.;        /*wenn Messdaten andere Zeit als MEZ not 0*/
    InV=timestep*60;      /*Intervallaenge in Minuten*/
    se = zeit;
    /*if time step=1 h, time=8, split=4 calculation at: 7h7.5min 7h22.5min etc*/


    jd2monat();           /*CONVERT JULIAN DAY TO MONTH AND DAY*/

    if(nrows >= ncols)         /*NEW 7/2003, for dimensioning of indexx,indexy-arrrays*/
        dim_index = nrows;
    else
        dim_index = ncols;

    /* Initialisieren des Arrays fuer die Abweichung der wahren Sonnenzeit von der Ortszeit
        in Abhaengigkeit vom Monat */
    /* gibt die Verschiebung der realen Sonnenzeit gegenueber der
            mittleren Sonnenzeit in min an (Elliptische Bahn) das Array steht
            an jeder Seite um eis ueber, d.h., es kann auch mit monat-1
            bzw. monat+1 gerechnet werden*/

    timeequation[0]  =  11.3;
    timeequation[1]  =  -3.2;     /*Januar*/
    timeequation[2]  = -13.6;
    timeequation[3]  = -12.6;
    timeequation[4]  =  -4.2;
    timeequation[5]  =   2.8;
    timeequation[6]  =   2.5;
    timeequation[7]  =  -3.5;
    timeequation[8]  =  -6.3;
    timeequation[9]  =  -0.3;
    timeequation[10] =  10.0;
    timeequation[11] =  16.4;
    timeequation[12] =  11.3;
    timeequation[13] =  -3.2;


    /* Suchen der groessten Hoehe, um den Schattensuchalgorithmus abbrechen zu koennen,
        wenn die Gerade ueber hmax steigt, und immer noch kein Schatten berechnet wurde */

    if (hmaxcalculated == 0) {   /*nur einmal berechnen - aendert sich nicht*/
        hmax=-1e10;
        for (i=1; i<=nrows; i++)
            for (j=1; j<=ncols; j++)
                if (griddgm[i][j]>hmax)   hmax=griddgm[i][j];
        hmaxcalculated = 1;
        printf("max elevation in DEM = %6.1f\n\n",hmax);
    }


    /* Ausfuehren der expositions-, gefaelle- und
       abschattungsabhaengigen Korrektur fuer die direkte Sonnenstrahlung */

    cn=1;
    gewicht=1.0/(float)split;

    /* es werden jeweils split Teilintervalle gebildet. Fuer jedes dieser
       Teilintervalle wird die Mittelzeit genommen, fuer diese Mittelzeit wird dann
       die Expositionskorrektur gerechnet. Der Mittelwert aus allen Berechnungen
       wird als repraesentativ fuer das Intervall angenommen. Es werden nur Intervalle
       beruecksichtigt, an denen die Sonne ueber dem Horizont steht.
       Bei einem Zeitintervall von 24 Stunden kann split z.B. 24 sein, es werden dann
       nach jeweils count Rechenaufrufen (also count Tagen) fuer alle 24 Stunden
       die Korrekturwerte ermittelt un dann ain repraesentetiver Tageswert errechnet. */

    /*-----------------------------------------*/
    /* Berechnen von sunazimuthangle, sunaltitudeangle, sunzenithangle fuer diese Stunde
       Angaben der Ergebnisse in rad, der Eingangsdaten aber in Grad! */

    /* SUNRISE AND SUNSET FOR THE DAY */
    sunrise(tag,monat,breite,laenge,&sunr,&suns);      /*!!!!!!!!!!*/

    /* Nullsetzen der Korrekturfaktoren erfolgt explizit, da die Zuweisung in der
       Schleife zur Korrekturfaktorermittlung incrementell erfolt (es wird dann fuer
       jedes Teilintervall eine um den Korrekturfaktor fuer dieses Zeitintervall korrigierte
       Strahlung addiert, so dass zum Intervallende genau <split> Teilintervall-Ergebnisse
       addiert sind. Der Wert in EXKORR entspricht dann dem Korrigierten Stundenwert der
       Strahlung (Korrekturfaktoren mit der astronomisch und topographisch moeglichen
       Strahlung gewichtet). Ganz am Ende wird dann durch eine separat ermittelte
       unkrrigierte Intervallsumme der Strahlung geteilt, um so den effektiven
       Korrekturfaktor zu erhalten */

    for (i=1; i<=nrows; i++)     /*CORRECTION FACTOR GRID INITIALISIEREN AUF NULL*/
        for (j=1; j<=ncols; j++) {
            EXKORR[i][j] =0.0;
            SHADE[i][j]  =0.0;     /*must be zero, because summing up of SHAPE*/
            DIRECT[i][j] =0.0;
            Isenk[i][j]  =0.0;    /*clear sky solar radiation, momentanwert*/
            strlsumme[i][j]=0.0;  /*clear sky solar rad Wh/m2: mean for time step interval*/
        }

    if ((dailyoutput == 1) && (zeit == zeitbeg)) { /*set to zero for first timestep of each day*/
        for (i=1; i<=nrows; i++)
            for (j=1; j<=ncols; j++) {
                EXKORRday[i][j] = 0.0;
                SHADEday[i][j]  = 0.0;
                DIRECTday[i][j] = 0.0;
            }  /*endfor*/
    }  /*endif*/

    /*SHADE = mean shade value for timestep, d.h. sum up 0 or 1 for each subinterval,
      divide by split for each time step*/

    topofatm = 0;      /*top of atmosphere radiation*/

    /* Da fuer mehrere Intervalle die Strahlung unterschiedlich gross sein kann,
       muessen die Korrekturfaktoren mit den jeweils astronomisch moegl. Strahlungen
       gewichtet werden. Es wird also das Integral der korrigierten maximal
       moeglichen Strahlungen errechnet, anschliessend durch das Integral der
       unkorrigierten maximal moeglichen Strahlung geteilt (me) und so
       der mittlere Korrekturfaktor erhalten. */

    /* Teilintervall am Anfang der Integration (Mitte des 1. der <split> Teilintervalle) */
    spa = se-InV/60.0*(1-0.5/(float)split)+timekorr;

    /* Teilintervall am Ende der Integration (Mitte des letzten der <split> Teilintervalle*/
    spe = se+0.01+timekorr;


    /* ------------------------------------------------------------------------------------ */
    /******* ZEIT-SCHLEIFE : UEBER ALLE ZEITUNTERINTERVALLE (je nach SPLIT) BERECHNUNG VON
                                 clear-sky direct radiation Isenk = height-dependent
                                 ABSCHATTUNG und CORRECTION FACTOR  ******/

    for (sp=(float)spa; sp<(float)spe; sp+=gewicht*(InV/60.0))


        /* sp ist die Stundennummer (Bruchteile erlaubt u. noetig) des aktuellen Teilintervalles,
           korrigiert mit der Zeitverschiebung der mittleren gegen die wahre Sonnenzeit;
           so ist spa bei einem Zeitintervall von 2 Stunden und split=4 (jede halbe Stunde
           neu berechnen) nach obiger Berechnung fuer se=10 (die an die Routine uebergebene
           Stundennummer [fuer das Ende des Intervalls])
           10 - 120/60 * (1 - 0.5/4) + korr = 10 - 2 * (1-0.25) = 10 - 2 + 0.5 = 8.5.
           Das erste Teiintervall ist also 0.5 Stunden lang und endet um 8:30.
           Als Increment wird jeweils die mit 1/<split> gewichtete Laenge eines
           Intervalles in Stunden (hier 0.25*120/60 = 0.5 addiert */
    {


        /*** $$$ NEU KONTROLL-OUTPUT, SCHATTEN DER SUBINTERVALLE */
        if (subintfileyes == 1) {
            sprintf(namesubint,"sub%4.2f",sp);
            strcpy(dummy,outpath);
            strcat(dummy,namesubint);

            if ((shadesubint = fopen(dummy,"wb")) == NULL) {
                printf("\n ERROR in opening shade file (subintervals)");
                exit(10);
            }
        } /*endif*/
        /*** $$$ ***********/

        /*  wenn der Beginn des aktuellen Intervalls vor Sonnenaufgang oder
           das Ende des aktuellen Berechnungsintervalles nach
           Sonnenuntergang liegt, wird keine Strahlung beruecksichtigt
           (nur vollstaendig "besonnte" Intervalle werden beruecksichtigt) */
        /* fuer alle Grids gleich */

        /* neu 21.1.1997, da Fehler in bestimmten Faellen noerdlich vom Polarkreis: */
        split_temp = InV/60.0/(float)split; /* Dauer eines Teilintervalles in h */
        inta = sp-split_temp/2.0;
        inte = sp+split_temp/2.0;
        sunr = fmod(sunr+24.0,24.0); /* Sonnenaufgang zwischen 0 ... 24 Uhr fixieren */
        suns = fmod(suns+24.0,24.0); /* Sonnenuntergang ebenfalls fixieren */
        /* Wie lange ist es am Anfang eines Teilintervalls noch dunkel und am Ende schon dunkel? */

        /* dark_a */
        if (sunr<inta)
            dark_a = 0.0;
        else if (sunr<inte)
            dark_a = sunr - inta;
        else /* Stunde komplett vor Sonnenaufgang */
            dark_a = 0.0;

        /* dark_e */
        if (suns>sunr) {
            /* der Normalfall */
            if ((suns>inte)&&(sunr<inte))
                dark_e = 0.0;
            else if ((suns>inta)&&(suns<inte))
                dark_e = inte - suns;
            else
                dark_e = split_temp;
        } else {
            /* der Fall, dass die Sonne erst nach Mitternacht gesetzl. Zeit untergeht */
            if (suns>inte)
                dark_e = 0.0;
            else if (suns>inta)
                dark_e = inte - suns;
            else
                dark_e = sunr > inte ? split_temp : 0.0;
        }

        Anteil_dunkel = (dark_a + dark_e)/split_temp;
        /* Wenn die Sonne im Intervall erst unter, dann aufgeht, muss zum Anfang und Ende
           des Intervalls das Komplement zu 1 von dark_a und dark_e gebildet und addiert werden
           Anteil_hell=(1-dark_a)+(1-dark_e) = 2-(dark_a+dark_e) = 2.0 - Anteil_dunkel
           was f’r Anteil_dunkel zu Anteil_dunkel = 1-Anteil_hell = 1 - 2 + Anteil_hell =
           Anteil_dunkel-1 fuehrt */

        if (Anteil_dunkel>1.0) Anteil_dunkel--;

        /* wenn die Sonne nur ganz kurz auftaucht, nuetzt es gar nichts!
           Da kann sie auch gleich untergetaucht bleiben */
        if (Anteil_dunkel > 0.95) Anteil_dunkel=1.0;
        Anteil_hell   = 1.0 - Anteil_dunkel;

        if ((Anteil_hell>0.5)&&((dark_a+dark_e)<=1))
            nostrl = 0;    /*day*/
        else
            nostrl = 1;    /*night*/

        /* wo steht die Sonne? calculate for every time step even if before sunrise*/
        getsuncoordinates(tag,monat,breite,laenge,sp,&sunazimuthangle,&sunaltitudeangle);
        sunzenithangle = pi/2 - sunaltitudeangle;

        /*---------------------------------------------------------------------------------*/
        /*      Berechnung der clear-sky solar radiation   for all grids                   */
        /*---------------------------------------------------------------------------------*/

        if (!nostrl) {           /*Sonne aufgegangen ueber horizontaler Ebene*/
            radiussunearth();    /*for every timestep calculate squared radius sun-earth*/

            /*NEU : Isenk hoehenabhaengig -> daher Berechnung fuer jedes Grid*/
            for (i=1; i<=nrows; i++)
                for (j=1; j<=ncols; j++) {
                    if (sunaltitudeangle>0.017) { /* 1 Grad */
                        /* Berechnung der bei Wolkenfreiem Himmel und klarer Atmosphaere auf einen
                           waagerechten Quadratmeter auftreffender Strahlungsenergie [Isenk] = Wh/m^2 */
                        /* cos(sunazimuthangle) wirkt hier auf den Extinktionskoeffizienten!  */

                        /*------------------------------------------------*/
                        p = exp(-0.0001184*griddgm[i][j]) * P0;           /* air pressure */
                        /*------------------------------------------------*/

                        /* Formel aus Oke S.345   Transmissivitaet = 0.8 estimated */
                        /*gewicht=1.0/(float)split,  InV=Intervalllaenge in Minuten=timestep*60 */
                        /*-!!!------------------------------------------------------------------------!!!-*/
                        Isenk[i][j] = gewicht*InV*1368.0*radius2sun/60.0 *
                                      pow(0.75,(p/P0)/cos(sunzenithangle));
                        /*-!!!------------------------------------------------------------------------!!!-*/

                    }  /*endif*/
                    else Isenk[i][j]=0.0; /* Nachts gibt es keine kurzwellige Einstrahlung */

                    strlsumme[i][j] += (Isenk[i][j]*cos(sunzenithangle));

                }  /*endfor  next grid*/

            /*-!!!------------------------------------------------------------------------!!!-*/
            /* ----SOLAR RADIATION AT TOP OF ATMOSPHERE integrated over subinterval Wh/m2---- */
            /*        NO ARRAY BECAUSE IT IS CONSTANT OVER THE WHOLE AREA  */
            topofatm += gewicht*InV*1368.0*radius2sun/60.0 * cos(sunzenithangle); /*!!!*/
            /*-!!!------------------------------------------------------------------------!!!-*/

        }  /*endif  !nostrl*/

        /*---------------------------------------------------------------------------------*/
        /*      Calculation of shading for all grids for one point of time                 */
        /*         algorithm by Joerg Schulla                                              */
        /*---------------------------------------------------------------------------------*/

        /* If the sun is below the horizon; shape array = 1, meaning the whole area in dark */
        /* If sun have risen above horizon shading calculations begins */
        for (i=1; i<=nrows; i++)
            for (j=1; j<=ncols; j++) {
                if (nostrl == 1)     /*before sunrise over horizontal surface (night) */
                    SHAPE[i][j]=1.0;      /*must be 1, because no shade calculation before sunrise*/
                else                 /*after sunrise -> shade calculation below*/
                    SHAPE[i][j]=0.0;      /*must be 0 if algorithm below used*/
            }


        if (!nostrl) {        /*Sonne aufgegangen ueber horizontaler Ebene*/
            /* Vorzeichen der Geradenanstiege in x- und y-Richtung */
            if (sunazimuthangle<=pi/2.0) {
                vx = 1;
                vy = 1;
            } else {
                if (sunazimuthangle<=pi) {
                    vx = 1;
                    vy =-1;
                } else {
                    if (sunazimuthangle<=pi*1.5) {
                        vx =-1;
                        vy =-1;
                    } else {
                        vx =-1;
                        vy = 1;
                    }
                }
            }
            /***********************************************************************************/
            /* If West-East direction smaller than 45 degrees azimuth */
            /* in x Richtung kleiner 45 Grad Neigung */
            if (fabs(cos(sunazimuthangle))<= (float)0.707106781)
                /* x-schritt ist immer 1, y-schritt wird sukzessive nachgefuehrt */
            {
                dy=(int)fabs((ncols*cos(sunazimuthangle)/sin(sunazimuthangle)));
                dx=ncols;

                dz=fabs((float)cs/sin(sunazimuthangle))*tan(sunaltitudeangle);
                /*        indexx = (int*)calloc(ncols+1,sizeof(int));
                        indexy = (int*)calloc(ncols+1,sizeof(int));    */

                /*NEW 7/2003, as probably underdimensioned and thus error for some applications*/
                indexx = (int*)calloc(dim_index+1,sizeof(int));
                indexy = (int*)calloc(dim_index+1,sizeof(int));

                /* Going through the grid column by column, keeping rows constant */
                /* If sun shines dominantly from West-East direction */
                if (vx>0)
                    /* von links nach rechts */
                {
                    /* alle Punkte im Grid durchlaufen i=1..nrows,j=1..ncols */
                    for (i=1; i<=nrows; i++) {
                        for (j=1; j<=ncols; j++) {
                            fx=dx/2;
                            ii=i;
                            jj=j;
                            z=griddgm[i][j];
                            k=1;
                            indexy[k]=ii;
                            indexx[k]=jj;
                            while ((ii<nrows)&&(ii>1)&&(jj<ncols)&&(z<hmax)&&(SHAPE[i][j]<0.5)) {
                                z+=dz;
                                jj+=vx;
                                fx-=dy;
                                if (fx<=0) {
                                    fx+=dx;
                                    ii-=vy;
                                }
                                k++;
                                indexy[k]=ii;
                                indexx[k]=jj;
                                if (griddgm[ii][jj]>z) {
                                    for (m=1; m<k; m++)
                                        SHAPE[indexy[m]][indexx[m]]=1.0;
                                }
                            }
                        }
                    }
                }
                /* If sun shines dominantly from East-West direction */
                else
                    /* von rechts nach links */
                {
                    /* alle Punkte im Grid durchlaufen i=1..nrows,j=ncols..1 */
                    for (i=1; i<=nrows; i++) {
                        for (j=ncols; j>=1; j--) {
                            fx=dx/2;
                            ii=i;
                            jj=j;
                            z=griddgm[i][j];
                            k=1;
                            indexy[k]=ii;
                            indexx[k]=jj;
                            while ((ii<nrows)&&(ii>1)&&(jj>1)&&(z<hmax)&&(SHAPE[i][j]<0.5)) {
                                z+=dz;
                                jj+=vx;
                                fx-=dy;
                                if (fx<=0) {
                                    fx+=dx;
                                    ii-=vy;
                                }
                                k++;
                                indexy[k]=ii;
                                indexx[k]=jj;
                                if (griddgm[ii][jj]>z) {
                                    for (m=1; m<k; m++)
                                        SHAPE[indexy[m]][indexx[m]]=1.0;
                                }
                            }
                        }
                    }
                }
                free(indexx);
                free(indexy);
            } else
                /* y-schritt ist immer 1, x-schritt wird nachgefuehrt */
            {
                dx=(int)fabs(nrows*sin(sunazimuthangle)/cos(sunazimuthangle));
                dy=nrows;
                dz=fabs((float)cs/cos(sunazimuthangle))*tan(sunaltitudeangle);

                /*    indexx = (int*)calloc(ncols+1,sizeof(int));
                      indexy = (int*)calloc(ncols+1,sizeof(int));   */

                /*NEW 7/2003, as probably underdimensioned and thus error for some applications*/
                indexx = (int*)calloc(dim_index+1,sizeof(int));
                indexy = (int*)calloc(dim_index+1,sizeof(int));

                if (vy>0)
                    /* von unten nach oben */
                {
                    /* alle Punkte im Grid durchlaufen i=nrows..1,j=1..ncols */
                    for (i=nrows; i>=1; i--) {
                        for (j=1; j<=ncols; j++) {
                            fy=dy/2;
                            ii=i;
                            jj=j;
                            z=griddgm[i][j];
                            k=1;
                            indexy[k]=ii;
                            indexx[k]=jj;
                            while ((ii>1)&&(jj>1)&&(jj<ncols)&&(z<hmax)&&(SHAPE[i][j]<0.5)) {
                                z+=dz;
                                ii-=vy;
                                fy-=dx;
                                if (fy<=0) {
                                    fy+=dy;
                                    jj+=vx;
                                }
                                k++;
                                indexy[k]=ii;
                                indexx[k]=jj;
                                if (griddgm[ii][jj]>z) {
                                    for (m=1; m<k; m++)
                                        SHAPE[indexy[m]][indexx[m]]=1.0;
                                }
                            }
                        }
                    }
                } else
                    /* von oben nach unten */
                {
                    /* alle Punkte im Grid durchlaufen i=1..nrows,j=1..ncols */
                    for (i=1; i<=nrows; i++) {
                        for (j=1; j<=ncols; j++) {
                            fy=dy/2;
                            ii=i;
                            jj=j;
                            z=griddgm[i][j];
                            k=1;
                            indexy[k]=ii;
                            indexx[k]=jj;
                            while ((ii<nrows)&&(jj>1)&&(jj<ncols)&&(z<hmax)&&(SHAPE[i][j]<0.5)) {
                                z+=dz;
                                ii-=vy;
                                fy-=dx;
                                if (fy<=0) {
                                    fy+=dy;
                                    jj+=vx;
                                }
                                k++;
                                indexy[k]=ii;
                                indexx[k]=jj;
                                if (griddgm[ii][jj]>z) {
                                    for (m=1; m<k; m++)
                                        SHAPE[indexy[m]][indexx[m]]=1.0;
                                }
                            }
                        }
                    }
                }
                free(indexx);
                free(indexy);

            }
        }

        /*-------------------------------------------------------------------------------------------*/
        /*      expositions- und neigungsbedingte Strahlunsgkorrektur    still same subinterval      */
        /*         correction factor = cos(angle of incidence)/cos(zenith)                           */ /*-------------------------------------------------------------------------------------------*/

        for (i=1; i<=nrows; i++) /* ueber alle Reihen */
            for (j=1; j<=ncols; j++) { /* Spalten des Grids */
                SHADE[i][j] += (SHAPE[i][j]);    /*fuer Mittelwertberechnung des ganzen time steps*/

                /*Aufsummieren des Schattenergebnisses fuer das split subinterval (=SHAPE ist 0 oder 1), nach Ende
                  aller split Teilintervalle teilen durch die Anzahl Teilintervalle => gibt mittlere Beschattung
                  waehrend des Zeitintervalls 0 oder 1 oder ein Wert zw. 0 und 1, d.h dann innerhalb dieses time
                  steps ist die Sonne unter oder aufgegangen. */


                if ((!nostrl)&&(SHAPE[i][j]<0.5)) {    /*AFTER SUNRISE AND NO SHADE*/
                    /*cos of angle of incidence */
                    faktor = getcosincidence(ASP[i][j]*pi/180.0,SLOPE[i][j]*pi/180.0,
                                             sunazimuthangle,sunzenithangle);

                    if (faktor<=0)
                        faktor=0.0;

                    /*Begrenzung ? ==> faktor/cos(sunzenithangle) > 5. => faktor = faktor*cosZ */
                    if(faktor/cos(sunzenithangle) > 5.)
                        faktor=5*cos(sunzenithangle);

                } else faktor=0;

                /***********FUER OUTPUT SHADE EACH SUBINTERVAL *****/
                if ((i==rowclim) && (j==colclim))
                    faktorstation = faktor;
                /***********/

                /* Korrektur fuer direkte Sonnenstrahlung:
                   wenn Sonne unter Horizont      : 0 (keine Korrektur)
                   wenn Sonne Flaeche nicht trifft: 0 (keine Korrektur)
                   wenn Flaeche im Schatten liegt : 0 (keine Korrektur)
                   sonst                          : echte Korrektur     */

                DIRECT[i][j] += (faktor*Isenk[i][j]);     /*!!!!!*/
            }

        /**** $$$  KONTROLL-OUTPUT SCHATTENFILES FUER JEDES SUBINTERVAL*****/
        if (subintfileyes == 1) {
            printf("\n\ttime=%5.2f  shape=%5.2f  zenith=%6.2lf faktor=%5.2f direct =%6.2f",
                   sp,SHAPE[rowclim][colclim],sunzenithangle*180/pi,faktorstation,
                   Isenk[rowclim][colclim]/timestep*split);

            if (shadesubint) {    /*FILE IS OPEN*/
                fwrite(&(x[1]),sizeof(float),12,shadesubint);   /*WRITE TO FILE*/
                for (i=1; i<=nrows; i++)        /*ueber alle Reihen*/
                    fwrite(&(SHAPE[i][1]),sizeof(float),ncols,shadesubint);

                fclose(shadesubint);
                shadesubint = NULL;     /*pointer freigeben*/
            }/*endif*/
        } /*endif fileyes*/

        /* $$$ *********/


    }   /* Schleife fuer die <split> Teil-(zeit-)intervalle                       ********/
    /* *******************************************  NEXT SPLIT TIME INTERVAL **************** */

    /* Strahlungssumme aller Teilintervalle wieder wegdividieren,
       also den effektiven Faktor, das eigentliche Ergebniss dieses Teilmodells,
       errechnen */

    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            EXKORR[i][j] = strlsumme[i][j] > 0.0 ? DIRECT[i][j]/strlsumme[i][j] : 0 ;
            SHADE[i][j]  = (SHADE[i][j])/split;     /*mittlere Beschattung des time steps*/
            DIRECT[i][j] = DIRECT[i][j]/timestep;
            /*direct radiation corrected Umrechnung von Wh/m2 in W/m2 */
        }

    if (dailyoutput >= 1) {  /*SUM UP ALL HOURLY VALUES OVER THE ENTIRE DAY*/
        for (i=1; i<=nrows; i++)
            for (j=1; j<=ncols; j++) {
                EXKORRday[i][j] += EXKORR[i][j];
                SHADEday[i][j]  += SHADE[i][j];
                DIRECTday[i][j] += DIRECT[i][j];
            }  /*endfor*/
    }  /*endif*/

    /* $$$ *********/
    if (subintfileyes == 1) {
        printf("\n\t  EXKORR = %6.2f strlsume= %6.2f, SHADE= %4.2f,   %d %d",
               EXKORR[rowclim][colclim],strlsumme[rowclim][colclim],
               SHADE[rowclim][colclim],rowclim,colclim);
    }
    /* $$$ *********/


    return;

}



/*=======================================================================*/
/**** CALCULATION OF STATISTICS FOR SECOND 6 FIGURES IN OUTPUTFILE    ****/
/*=======================================================================*/

void statist(float **matrix,float *x)

{
    int     i,j;

    x[10]= 0.0;     /*sum of values in grid*/
    x[7] = 0.0;          /*number of grids*/
    x[8] = 100000.0;      /*minimum*/
    x[9] = 0.0;          /*maximum*/

    for (i=1; i<=nrows; i++) {       /*ueber alle Reihen*/
        for (j=1; j<=ncols; j++) {    /*ueber alle Reihen*/
            if ((matrix[i][j] != nodata) && (i>=startrow) && (i<=endrow) && (j>=startcol) && (j<=endcol)) {
                x[7] += 1.0;        /*number of grid cells*/

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
    /* printf("\n number of grid cells in output grid = %.0f",x[7]); */

    return;
}



/*****************************************************************/
/*  FUNCTION  writeout                                           */
/*            write to output                                    */
/*   called either every time step, every day or only once at end of main for period mean */
/*	      and close file                                     */
/*****************************************************************/

void writeoutput()

{
    float meanfactor=1;

    /*FOR DAILY AND PERIOD OUTPUT, COPY VALUES SUMMED UP FOR ENTIRE DAY/PERIOD INTO SHADE/EXKORR/DIRECT-ARRAYS*/
    /* TO BE ABLE TO USE THE PART OF THE FUNCTION BELOW */

    /*INTEGRATED VALUES MUST BE DIVIDED BY 24 FOR DAILY MEANS AND LARGER FACTOR FOR PERIODS MEANS */

    if (dailyoutput == 1)  /*set to zero for first timestep of each day*/
        meanfactor = 24.0;
    if (dailyoutput == 2)
        meanfactor = 24*(jdend-jdbeg+1);

    if (dailyoutput >= 1) { /*set to zero for first timestep of each day*/
        if(writeshade==1) {
            for (i=1; i<=nrows; i++)
                for (j=1; j<=ncols; j++)
                    SHADE[i][j]  = SHADEday[i][j]/meanfactor;  /*COMPUTE DAILY MEANS, TIMESTEP MUST BE 1 h*/
        }
        if(writeexkorr==1) {
            for (i=1; i<=nrows; i++)
                for (j=1; j<=ncols; j++)
                    EXKORR[i][j] = EXKORRday[i][j]/meanfactor;
        }
        if(writedirect==1) {
            for (i=1; i<=nrows; i++)
                for (j=1; j<=ncols; j++)
                    DIRECT[i][j] = DIRECTday[i][j]/meanfactor;
        }
    }  /*endif dailyoutput*/



    /*********** CALCULATE STATISTICS AND WRITE TO FILE ************************************/

    if(writeshade==1)          /*binary grid output of shading*/
        if(outshade) {               /*check, if file is open*/
            statist(SHADE,x);
            fwrite(&(x[1]),sizeof(float),12,outshade);    /*first 12 ROWS IN OUTPUT*/
            for(i=startrow; i<=endrow; i++)
                fwrite(&(SHADE[i][startcol]),sizeof(float),ncolsnew,outshade);
            fclose(outshade);
            outshade=NULL;
        }

    if(writeexkorr==1)
        if(outexkorr) {               /*check, if file is open*/
            statist(EXKORR,x);
            fwrite(&(x[1]),sizeof(float),12,outexkorr);    /*first 12 ROWS IN OUTPUT*/
            for(i=startrow; i<=endrow; i++)
                fwrite(&(EXKORR[i][startcol]),sizeof(float),ncolsnew,outexkorr);
            fclose(outexkorr);
            outexkorr=NULL;
        }

    if(writedirect==1)      /*clear-sky direct radiation slope-corrected*/
        if(outdirect) {            /*check, if file is open*/
            statist(DIRECT,x);
            fwrite(&(x[1]),sizeof(float),12,outdirect);    /*first 12 ROWS IN OUTPUT*/
            for(i=startrow; i<=endrow; i++)
                fwrite(&(DIRECT[i][startcol]),sizeof(float),ncolsnew,outdirect);
            fclose(outdirect);
            outdirect=NULL;
        }

    /*   if(writedirect==1)
           if(outdirect)
           { statist(DIRECT,x);
             fwrite(&(x[1]),sizeof(float),12,outdirect);
             for(i=1;i<=nrows;i++)
               fwrite(&(DIRECT[i][1]),sizeof(float),ncols,outdirect);
             fclose(outdirect);
             outdirect=NULL;
           }            */


    if(writesinglegrid==1) {      /*Output Ascii-file every time step*/

        fprintf(outsinglegrid,"%5.2f %5.2f %5.2f %6.2f %8.2f  %6.2f %9.2f %7.1f %8.1f %6.1f %6.1f %6.2f %6.2f\n",
                jd+zeit/24,zeit,SHADE[rowclim][colclim],EXKORR[rowclim][colclim],DIRECT[rowclim][colclim],
                strlsumme[rowclim][colclim]/timestep,topofatm/timestep,sunzenithangle*180/pi,
                sunazimuthangle*180/pi,dekli*180/pi,stuwi*180/pi,sunr,suns);
    }

    return;
}



/*=====================================================================*/
/*  CLOSE FILES, FREE STORAGE ETC.*/
/*=====================================================================*/

/*************************************************************/
/* FUNCTION freematrix.c*/
/*   gibt reservierten Speicherplatz der Gridfiles frei ******/

void freematrix(float **m,long nrl, long nrh, long ncl, long nch)

{
    int i;
    /*Freigeben des Speichers auf die Array-Zeilen*/
    free((char *)(m[nrl]+ncl-1));

    /*Freigeben des Speichers auf das Pointer-Array*/
    free((char *)(m+nrl-1));

    return;
}


/*****************************************************************/

void closeall()

{
    freematrix(griddgm,nrl,nrh,ncl,nch);      /*Digital elevation model*/
    freematrix(SLOPE,nrl,nrh,ncl,nch);        /*slope grid*/
    freematrix(ASP,nrl,nrh,ncl,nch);          /*aspect grid*/
    freematrix(Isenk,nrl,nrh,ncl,nch);        /*direct radiation at point of time*/
    freematrix(strlsumme,nrl,nrh,ncl,nch);    /*sum of Isenk over subintervals*/

    freematrix(SHAPE,nrl,nrh,ncl,nch);        /*shade or sun at mean of subinterval*/
    freematrix(SHADE,nrl,nrh,ncl,nch);        /*weightes mean for time step*/
    freematrix(EXKORR,nrl,nrh,ncl,nch);       /*correction factor*/
    freematrix(DIRECT,nrl,nrh,ncl,nch);       /*direct radiation corrected for slope etc*/

    if(dailyoutput >= 1) {
        freematrix(SHADEday,nrl,nrh,ncl,nch);
        freematrix(EXKORRday,nrl,nrh,ncl,nch);     /*correction factor*/
        freematrix(DIRECTday,nrl,nrh,ncl,nch);     /*direct radiation corrected for slope etc*/
    }

    if(writesinglegrid==1) {
        fclose(outsinglegrid);
        outsinglegrid=NULL;
    }

    free(x);
    x=NULL;

    return;
}


/*=====================================================================*/
/**********    MAIN    *************************************************/
/*=====================================================================*/

int main()

{
    input_read();   /*** READ INPUT DATA ***/
    screen();       /*** WRITE CONTENT OF input_read TO SCREEN ***/
    start();        /*** OPEN AND READ 3 GRID FILES  ***/
    startoutput();  /*** OPEN OUTPUT FILES ***/

    jd = jdbeg;      /*start julian day*/
    zeit = zeitbeg;  /*start time interval*/
    numberhourend = jdend*24+zeitend;    /*to find the end in do-loop below*/

    /*RECALCULATE HEADER - NUMBER OF ROWS/COLS AND COORDINATES OF LOWER LEFT CORNER*/
    ncolsnew = endcol - startcol + 1;
    if(wholegridyes == 0) {
        x[1] = ncolsnew;                   /*number of columns*/
        x[2] = endrow - startrow + 1;      /*number of rows*/
        x[3] += (startcol-1)*x[5];
        x[4] += (nrows - endrow)*x[5];     /*y-coordinate*/
    } else { /*needed because for statistics so that whole grid is computed*/
        startrow = 1;
        startcol = 1;
        endrow = nrows;
        endcol = ncols;
    }

    while(numberhour <= numberhourend) {     /*for all timesteps*/
        jd2=jd;
        zeit2=zeit;

        if(dailyoutput == 0)     /*output for every timestep*/
            opengridoutput();   /*** OPEN FILES FOR CALC OF SHADE,EXKORR,DIRECT ***/

        schatten();         /*** initialze arrays, SHADE CALCULATIONS ***/

        if(dailyoutput == 0)     /*output for every timestep*/
            writeoutput();

        if(zeit == 24) {          /*next day*/
            printf("jd time  %4.1f %4.1f\n",jd,zeit);   /*SCREEN OUTPUT EVERY MIDNIGHT*/

            if(dailyoutput == 1) {
                opengridoutput();   /*** OPEN FILES FOR CALC OF SHADE,EXKORR,DIRECT ***/
                writeoutput();
            }

            zeit = 0.;
            jd = jd + daysdirect;  /*next day to be calculated*/
        }  /*endif*/

        zeit = zeit + timestep;   /*next time step*/
        numberhour = jd*24+zeit;

        /*control to avoid endless loop*/
        /*  number += 1;
            if(number > 10000)
              {  printf("\n Run through loop 10000 times = more than 100 days\n\n");
                 exit(1);
              }
        */
    }   /*endwhile*/

    if(dailyoutput == 2) {  /*period mean*/
        opengridoutput();   /*** OPEN FILES FOR CALC OF SHADE,EXKORR,DIRECT ***/
        writeoutput();
    }

    closeall();     /* CLOSE FILES, SPEICHERFREIGABE */

    printf(" \n outpath :   %s\n",outpath);
    printf("  split = %4d\n",split);
    printf("  output every %3.0f th day\n\n",daysdirect);

    return 0;
}
