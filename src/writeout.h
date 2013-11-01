/***********************************************************************
 * Copyright 2012 Regine Hock, Lyman Gillispie
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
/****************************************************************************/
/* FILE   writeout.h                                                        */
/*        WRITE ENERGY BALANCE OUTPUT FOR EACH GRID TO OUTPUT-FILES         */
/*        Part of the Hock Melt Model                                       */
/*  Author: Lyman Gillispie                                                 */
/*  Created on: May 25, 2012                                                */
/*  Last update: October 2013*/
/****************************************************************************/

#ifndef MELT_MOD_WRITEOUT_H_
#define MELT_MOD_WRITEOUT_H_

#include<stdio.h>

/**************************************************************************/
/*  FUNCTION  openoutfile                                                 */
/*            OPEN OUTPUT-FILES                                           */
/*            called from startwritehour, startwriteday and startwriteall */
/*            and many other functions                                    */
/**************************************************************************/
void openoutfile(FILE **datei,char *outpath,char *name);

/**************************************************************************/
/*  FUNCTION  openoutfileascii                                            */
/*            OPEN OUTPUT-FILES                                           */
/*            called from many functions                                  */
/**************************************************************************/
void openoutfileascii(FILE **datei,char *outpath,char *name);

/**************************************************************************/
/* FUNCTION  startwritehour()                                             */
/*     OUTPUT-FILE FOR EVERY TIME STEP                                    */
/*     CREATE OUTPUT-FILENAME e.g. : sha+julianday+time e.g. sha19012.bin */
/*     OPEN FILE and WRITE FIRST 6 ROWS                                   */
/*     called from main every time step after grid loop                   */
/**************************************************************************/
void startwritehour();

/****************************************************************************/
/* FUNCTION  startwriteday()                                                */
/*    OUTPUT-FILE FOR DAILY MEANS                                           */
/*    CREATE OUTPUT-FILENAME e.g. : sha+julianday  e.g. sha190.bin AND OPEN */
/****************************************************************************/
void startwriteday();

/**************************************************************************/
/* FUNCTION  startwriteall()                                              */
/*    OUTPUT-FILE FOR WHOLE PERIOD                                        */
/*    CREATE OUTPUT-FILENAME e.g. : sha+julianday  e.g. sha190.bin        */
/*    OPEN FILE,   called from main after time loop at the end of main    */
/**************************************************************************/
void startwriteall();

/*=======================================================================*/
/**** CALCULATION OF STATISTICS FOR SECOND 6 ROWS IN OUTPUTFILE       ****/
/*=======================================================================*/
void statist(float **matrix,float *x);

/**************************************************************************/
/* FUNCTION  writegridoutput()                                            */
/*           WRITE GRID DATA TO OUTPUT-FILE hourly, daily, period means   */
/*  called from main after grid loop (hourly,daily) after time loop (period) */
/**************************************************************************/
void writegridoutput();

/****************************************************************/
/* FUNCTION  sumday
             SUM UP OVER ALL TIMESTEPS OF DAY FOR EACH GRID     */
/****************************************************************/
void sumday();

/*****************************************************************/
/* FUNCTION  sumall
             SUM UP OVER ALL TIMESTEPS OF PERIOD OF CALCULATION  */
/*           for grid output over whole period                   */
/*****************************************************************/
void sumall();

/*****************************************************************/
/* FUNCTION  writesnoworsurfaceyes                               */
/*     check if snow cover or surface type should be written to  */
/*     file; 2 options: every x th day or for randomly  selected */
/*         called from main every time step                      */
/*****************************************************************/
void writesnoworsurfaceyes();

/*****************************************************************/
/* FUNCTION  writesurface                                        */
/*     write surface conditions to file once per day, (not daily */
/*     means but grid for time step) for validation of snow line */
/*       for both meltmod and degree, but only if initial snow   */
/*       cover used as input,  called from main                  */
/*****************************************************************/
void writesurface();

/*****************************************************************/
/* FUNCTION  writesnowcover                                      */
/*     write snow cover to file once per day, (not daily         */
/*     means but grid for time step) for validation of snow melt */
/*       for both meltmod and degree, but only if initial snow   */
/*       cover used as input                                     */
/*  called from main                                             */
/*****************************************************************/
void writesnowcover();

/*============= MASS BALANCE OUTPUT ==========================================*/
/*****************************************************************/
/* FUNCTION  writemassbalgrid                                    */
/*     write summer/winter and massbalance to grid file          */
/*     and also annual mass balances to ascii-file               */
/*     and mass balance profiles for each year                   */
/*  called every day at midnight (if zeit=24, in time step loop) */
/*     modified May 2006                                         */
/*****************************************************************/
void writemassbalgrid();

/*****************************************************************/
/* FUNCTION  computemassbalprofile                               */
/*     compute the mass balance of each elevaton belt            */
/*  called from function writemassbalgrid for each variable (winter, summer, net) separately */
/*    new May 2006                                               */
/*****************************************************************/
void computemassbalprofile(float **matrixprofile,float *massbalbands);

/*****************************************************************/
/* FUNCTION  meanmassbalprofile                                  */
/*     compute the mean mass balance profile of all years        */
/*  years can be defined that are to be excluded from the mean   */
/*  adjust 2 variables below and give the years                  */
/*  called from main after all time steps have been calculation  */
/*    new May 2006                                               */
/*****************************************************************/
void meanmassbalprofile();

/*================ TIME SERIES OUTPUT FILES (ASCII) =============*/

/*****************************************************************/
/* FUNCTION  areameannull
             SPATIAL MEAN OVER WHOLE GLACIER SET TO ZERO
             Time series Ascii-file                             */
/*****************************************************************/
void areameannull();

/*****************************************************************/
/* FUNCTION  areasum
             SPATIAL MEAN OVER WHOLE GLACIER                     */
/*           called from main within grid loop for each grid     */
/*****************************************************************/
void areasum();

/* ======= TEMPORAL SERIES : ASCII-FILES ==============================*/
/******************************************************************/
/* FUNCTION  areameanwrite
             SPATIAL MEAN OVER WHOLE CALCULATED AREA areamean.dat */
/*NOTE: nglac is number pixels over drainage basin area*/
/******************************************************************/
void areameanwrite();

/*****************************************************************/
/* FUNCTION  stationoutput
          WRITE INDIVIDUAL GRID RESULTS TO ASCII-OUTPUT-FILE
          Output for every time step for whole period
          called from main every time step
          first line written in startoutasci (initial.c)        */
/*****************************************************************/
void stationoutput();

/*******************************************************************/
/* FUNCTION  writemeltstakes                                       */
/*           WRITE 2 FILES: CUMULATIVE MELTING AND                 */
/*           CUMULATIVE ASSBALANCE OF SEVERAL LOCATIONS INTO ONE FILE  */
/*           called from main every time step after grid loop      */
/*******************************************************************/
void writemeltstakes();

/************************************************************************/
/*  FUNCTION:  percentsnowfree                                          */
/*     goes through snow cover file and counts how may pixels snow free */
/*     writes one row to Ascii-file every midnight if snowfreeyes = 1   */
/*     can also be used if methodinisnow=1 !                            */
/************************************************************************/
void percentsnowfree();


/************************************************************************/
/*  FUNCTION:  writeperformance                                         */
/*     computes and writes model performance to file                    */
/*     one line with various r2 values                                  */
/*     called once at the end of main                                   */
/*     New 10/2013                                                      */
/************************************************************************/

void writeperformance();



/********************************************************************/
/*  FUNCTION  writemodelmeaspointbalances                           */
/*       makes file with measured and modeled point balances        */
/*          called from main once at the end                        */
/*   A file with measured point balances is read and a new file created which
 is identical but adds another column with the modeled balances for each
 stake observation. Modeled balances are computed from cumulated
 balance file "cummassbal.txt" which has the cumulated time series*/
/* New October 2013*/
/********************************************************************/

void  writemodelmeaspointbalances();



#endif /* WRITEOUT_H_ */
