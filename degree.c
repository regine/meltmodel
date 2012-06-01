
/*********************************************************************/
/* PROGRAM  degree.c                                                 */
/*  DISTRIBUTED SNOW/ICE MELT MODEL BASED ON TEMPERATURE INDEX       */
/*  METHODS ENERGY BALANCE INCLUDING OPTIONAL DISCHARGE CALCULATIONS */
/*   5.3.1998, update 21 Sep 2011                                   */
/*********************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "closeall.h"
#include "discharg.h"
#include "disopt.h"
#include "globcor.h"
#include "initial.h"
#include "input.h"
#include "radiat.h"
#include "scaling.h"
#include "tindex.h"
#include "turbul.h"
/* all function heads, first line */
#include "writeout.h"

#include "variab.h"      /* all global VARIABLES */

/************* MAIN ************************************************/

int main()

{
    degreedaymethod = 1;
    ratio = startratio;

    /*----------------------------------------------------------------*/
    /*** READ DATA FROM CONTROLING INPUT FILE / INITIALISATION     ****/
    /*----------------------------------------------------------------*/

    input_read();      /*** READ INPUT DATA ***/
    degreestart();

    startinputdata();  /***OPEN AND READ GRID FILES AND CLIMATE DATA until start***/
    checkgridinputdata_ok(); /*check if grid input data ok*/
    startoutascii();   /***OPEN ASCII-OUTPUT FILES AND WRITE HEADS***/
    startspecificmassbalance(); /*OPEN FILE FOR SPECIFIC MASS BAL, MULTI-YEAR RUNS*/
    startarrayreserve();   /*RESERVE STORAGE FOR GRID-ARRAYS*/

    glacierrowcol();  /*FIND FIRST, LAST ROW AND COLUMN WHICH IS GLACIERIZED IN DTM*/
    readclim();       /*** READ CLIMATE INPUT FIRST ROW = ONE TIME STEP ****/

    if((winterbalyes == 1) || (summerbalyes == 1))
        areaelevationbelts();    /*number of grid cells per elevation band, for bn profiles*/

    if(maxmeltstakes > 0)
        startmeltstakes();    /*OUTPUT OF MELT OF SEVERAL LOCATIONS - COMPARISON WITH STAKES*/

    if(snowfreeyes == 1)    /*OPEN FILE WITH TIME SERIES WITH NUMBER OF SNOWFREE PIXELS PER DAY*/
        opensnowfree();

    if((datesfromfileyes == 1) &&  (winterbalyes == 1) && (summerbalyes == 1))
        readdatesmassbal();      /*READ DATES OF MASS BAL MEASUREMENTS FOR EACH YEAR*/

    if(methodprecipinterpol == 2)    /*read precipitation index map once (constant in time)*/
        readprecipindexmap();        /*in turbul.c under precipitation*/

    if (disyes >= 1)        /*DISCHARGE SIMULATION REQUESTED, 1=discharge data, 2=no data*/
        startdischarg();     /*INITIALIZE DISCHARGE SIMULATION*/


    /*=============FOR EVERY TIME STEP ===============================================*/

    do {

        nsteps += 1;         /*number of time steps of period calculated*/

        /******* ADJUST BY USER, HOW OFTEN OUTPUT JD-TIME TO SCREEN *******/

        if((zeit==24) && ((int)jd/daysscreenoutput == floor((int)jd/daysscreenoutput)))
            printf("\n  yr= %4.0f  jd = %4.2f   time =%3.0f",year,jd,zeit);

        /******* TEMPERATURE INTERPOLATION ******/
        tempinterpol(); /*** ELEVATION-DEPENDENT AIR TEMP INTERPOLATION ***/

        if(do_out_area == 1)
            areameannull();  /* SPATIAL MEANS OF MODEL OUTPUT SET TO ZERO */

        if(ddmethod > 1) {    /*DIRECT RAD NOT NEEDED FOR SIMPLE DEGREE DAY*/
            if (directfromfile == 0)
                schatten();      /*** CALCULATE SHADE, CORRECTION FACTOR, DIRECT GRIDs ***/
            else
                readdirect();    /* READ DIRECT RADIATION (slope corrected) FROM FILE */
        }

        if(methodinisnow == 1)  /*NEEDED FOR DISCHARGE CALCULATIONS FOR ANY ddmethod*/
            albedoread();   /* OPEN AND READ SPECIFIC ALBEDO FILE IF NEW ONE VALID FOR TIME STEP */
        else
            whichsurface();   /*VALUE FOR ARRAY surface (SNOW,FIRN,ICE,ROCK) ICE/SNOW DDF AND K-VALUES*/

        /*EXTENDED TEMP INDEX METHOD WITH DIRECT RAD AND CLOUD REDUCTION*/
        if(ddmethod == 3)
            ratioglobal();   /*CALC RATIO OF MEASURED GLOBAL RAD AND CALCULATED DIRECT RAD*/

        if(methodprecipinterpol == 3)   /*read precipitation grids from file for each time step*/
            readprecipfromfile();



        /*------- FOR EACH GRID POINT - only for drainage basin grids (defining the area to be computed)--*/

        for (i=firstrow; i<=lastrow; i++)           /*for all rows with area to be calculated*/
            for (j=firstcol[i]; j<=lastcol[i]; j++) {  /*for all columns*/

                if (griddgmdrain[i][j] != nodata) { /*only for area to be calculated defined by dgmdrain*/
                    if (griddgmdrain[i][j] != griddgm[i][j]) {
                        printf("\n\n ERROR elevation in DTM is not the same as in drainage grid\n");
                        printf("	 row  %d  col  %d      (function meltmod.c\n\n",i,j);
                        exit(12);
                    }

                    temppos();      /*** copying interpolated temp into positive temp-grid ***/
                    airpress();     /*** CALCULATION AIR PRESSURE AT GRID POINT         ***/
                    vappress();     /*** CALCULATION VAPOUR PRESSURE FROM REL. HUMIDITY ***/


                    /*==================================================================*/

                    switch(ddmethod)

                    {
                    case 1:
                        degreedaymelt();    /*CALCULATING MELT WITH DEGREE DAY*/
                        break;
                    case 2:
                        dd_directmelt();    /*INCLUDING DIRECT RADIATION (J.Glac. 1999)*/
                        break;
                    case 3:
                        dd_directglobal();  /*AS BUT INCLUDING GLOBAL RAD (J.GLAC 1999)*/
                        break;

                    default:
                        printf("\n no index method defined \n  degree.c\n");
                        exit(20);
                    }

                    /*==================================================================*/

                    ddfcalculation();    /*CALCULATE CORRESPONDING DEGREE DAY MELT/TEMPOS*/

                    /******* PRECIPITATION *********************/
                    precipinterpol();

                    if(methodinisnow == 2)      /*run with known initial snow cover*/
                        snowcoverdegree();       /*snow precip added to snow cover, to find k-values*/

                    if( (winterbalyes == 1) || (summerbalyes == 1) || (maxmeltstakes > 0))
                        if (griddgmglac[i][j] != nodata)   /*only for glacier, no matter if dgmdrain is larger*/
                            massbalance();

                    /*** SUMMING UP ALL VALUES OVER AREA - for spatial means ***/
                    if(do_out_area == 1)
                        areasum();

                } /*endif*/
            }  /*endfor next grid*/


        /***------------------ NEXT GRID CELL-------------------- ***/

        /*WHOLE GRIDS ARE CALCULATED FOR ONE TIME STEP -
           NOW CACULATE DISCHARGE AND WRITE TO OUTPUTFILE*/

        /********************* DISCHARGE ************************/

        if (disyes >= 1) {   /*DISCHARGE TO BE CALCULATED, measured file available (1) or not (2)*/
            if(onlyglacieryes == 1)  /*drainage basin larger than glacier*/
                rainoutsideglac();       /*rain from outside put onto the glacier, discharg.c*/

            if (disyesopt == 0)       /*simulation run*/
                discharge();    /*for every grid calculated discharge, sum to melt and sum discharge*/

            if (disyesopt == 1)     /*optimization run, disyes set to 1 in input.c*/
                dischargeopt();       /*calculated discharg, with optimal r2-criterium*/
        }

        /*** WRITE MODEL RESULTS TO FILE FOR INDIVIDUAL GRIDPOINTS FOR EVERY TIME STEP ***/
        if (outgridnumber > 0)     /*output file requested by user*/
            stationoutput();

        /*** WRITE MASS BALANCE FOR SEVERAL LOCATIONS TO ONE FILE ***/
        if(maxmeltstakes > 0)
            writemeltstakes();

        /*** WRITE GRID FILES ***/
        switch(do_out)   /*** WRITE ENERGY BALANCE GRID-OUTPUT-FILES ***/

        {
        case 0:
            break;
        case 1:     /*OUTPUT EVERY TIME STEP*/
            startwritehour();    /*open output-files for every time step*/
            writegridoutput();
            break;

        case 2:     /*OUTPUT ONLY EVERY DAY*/
            sumday();             /*sum up values for subdiurnal timesteps*/
            if (zeit == 24.0) {   /*last hour of day*/
                startwriteday();   /*open files for daily means*/
                writegridoutput(); /*write grid to output file*/
                meandaynull();     /*after writing initialize to zero*/
            }  /*if*/
            break;

        case 3:     /*OUTPUT ONLY FOR WHOLE PERIOD*/
            sumall();             /*sum up values for subdiurnal timesteps*/
            break;                /*mean for whole period : write at end*/

        case 4:     /*OUTPUT EVERY DAY AND FOR WHOLE PERIOD*/
            sumday();             /*sum up for daily means*/
            sumall();             /*sum up for period means*/
            /*must be done before writing daily means to files, because
              MELT-array will be overwritten by daily mean, discharge must
              also be calculated before*/
            if (zeit == 24) {     /*daily means, write period means at end*/
                startwriteday();   /*open files for daily means*/
                writegridoutput();
                meandaynull();     /*after writing initialize to zero*/
            }  /*if*/
        } /*switch*/


        /*CHECK IF SURFACE CONDITIONS OR SNOW COVER FILES SHOULD BE WRITTEN TO OUTPUT*/
        /*    NO TEMPORAL MEANS POSSIBLE - FOR VALIDATION OF SNOW LINE RETREAT */
        writesnoworsurfaceyes();

        /* WRITE GRID OF SURFACE CONDITIONS - ONLY FOR MIDNIGHT EVERY daysnow-th DAY */
        /*   OR FOR SELECTED DAYS SPECIFIED IN input.dat*/

        if((surfyes >= 1) && (write2fileyes == 1))
            writesurface();      /*open file and write to file*/
        if((snowyes >= 1) && (write2fileyes == 1))
            writesnowcover();      /*open file and write to file*/

        /*  WRITE TO TIME SERIES ASCII FILE EVERY MIDNIGHT HOW MANY PIXELS SNOWFREE*/
        if((snowfreeyes == 1) && (zeit == 24))
            percentsnowfree();

        /* WRITE TIME SERIES OF SPATIAL MEAN MODEL RESULTS TO OUTPUT FOR EVERY TIME STEP*/
        if (do_out_area == 1)
            areameanwrite();

        /*write winter/summer/mass balance grids at end of winter/summer*/
        if((winterbalyes == 1) || (summerbalyes == 1))
            if(zeit == 24)
                writemassbalgrid();

        /********** V-A scaling **********************************/
        if((jd == summerjdend) && (scalingyes == 1))
            if( (datesfromfileyes == 0) || ( (datesfromfileyes == 1) && (year == nextyear)))
                scaling();

        /**********/
        /*set snow array to zero at end of melt season for next mass balance year*/
        /*to avoid that snow constantly accumulates in accumulation area and therefore*/
        /*firn is never exposed; done each year at start of winter*/
        if( (methodinisnow == 2) && (snow2zeroeachyearyes == 1) && (jd == (winterjdbeg)) && (zeit == 24) )
            initializeglacier2zero_nodatadouble(SNOW);

        readclim();       /*** READ CLIMATE INPUT NEXT TIME STEP ****/

        if(timestep != 24) {  /*subdaily timesteps: midnight row is next julian day not to continue with*/
            if((jd > (jdend+1)) && (year == yearend))
                stoploop = 1;
        } else if((jd == jdend+1) && (year == yearend)) /*otherwise last julian day would not be run*/
            stoploop = 1;

    }  while (stoploop != 1);

    /*====================== NEXT TIME STEP =======================================*/

    /*OUTPUT OF MEAN COMPONENTS OF ENERGY BALANCE FOR WHOLE PERIOD OF CALCULATION*/

    if ((do_out == 3) || (do_out == 4)) {   /*mean of whole period*/
        startwriteall();   /*open files for whole period*/
        writegridoutput();
    }

    /********** WRITE MEAN MASS BALANCE PROFILE TO FILE********************************/
    if((winterbalyes == 1) || (summerbalyes == 1))
        meanmassbalprofile();     /*mean over all years*/

    /******************************************************************/

    if (disyes == 1) {    /*only if discharge data available*/
        r2calc();
        r2calcln();
        if (disyesopt == 1)   /*optimization run*/
            write2matriz();    /*write r2 matriz to file*/
    }

    closeall();     /* CLOSE FILES, SPEICHERFREIGABE */

    printf("\n\n number of glacier grids         %d\n",nglac);
    printf(" number of calculated time steps   %d\n\n",nsteps);
    printf(" number of timesteps of dischargedata  %d\n\n",nstepsdis);
    if(disyes ==1) {
        printf("  simulated discharge volume (100 000 m3) = %8.2f\n",volumesim);
        printf("  measured discharge volume               = %8.2f\n",volumemeas);
    }
    printf("\n output written to   %s\n\n",outpath);
    printf("********* PROGRAM RUN COMPLETED ************\n\n");

    return 0;
}

