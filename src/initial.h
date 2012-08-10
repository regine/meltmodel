/***********************************************************************
 * Copyright 2012 Regine Hock, Lyman Gillispie
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
/*
 * initial.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 */

#ifndef MELT_MOD_INITIAL_H_
#define MELT_MOD_INITIAL_H_

#include<stdio.h>

/*******************************************************/
/* READ REST OF LINE                                   */
/*******************************************************/
void readrestofline(FILE **infile);


void checkgridinputdata_ok();

/************************************************************/
/* EXIT PROGRAM IF TOO MANY LINES WRITTEN TO OUTPUT res.out */
/*  to avoid that harddisk is filled to limit               */
/*  called from mainly subsurf()                            */
/*  resoutlines are added every time a variable exceeds preset limits */
/*  e.g. if surftemperature would fall below given limit    */
/************************************************************/
void toobig_resout();

/***********************************************/
/*     CLOSE FILE                              */
/***********************************************/
void closefile(FILE **datei, char *name);

/*============================================================================*/
/*  FUNCTIONS FOR INITIALISATION OF OUTPUT FILES WITH COMPONENTS OF ENERGY BALANCE
    DAILY MEANS AND/OR MEANS FOR THE WHOLE PERIOD
       called at end of start and from main                                 ***/

/*************************************************************/
/* FUNCTION  meandayreserve
	     RESERVE STORAGE FOR GRID ARRAYS FOR DAILY MEANS */
/*************************************************************/
void meandayreserve();

/******************************************************/
/* FUNCTION  meandaynull
	     INITIALISATION OF ARRAYS FOR DAILY MEANS */
/*must be zero, because of summing up  */
/******************************************************/
void meandaynull();

/****************************************************************/
/* FUNCTION  meanallnull
	     RESERVE STORAGE FOR ARRAY
	     INITIALISATION OF ARRAYS FOR MEANS OF WHOLE PERIOD
	     called from start                                  */
/****************************************************************/
void meanallnull();

/****************************************************************************/
/*  FUNCTION  startinputdata                                                */
/*    OPEN AND READ GRID FILES DTM, drainage basin, glacier, slope, aspect  */
/*    OPEN FILE WITH CLIMATE DATA and READ UNTIL START                      */
/****************************************************************************/
void startinputdata();

/*========================= OUTPUT ===========================================*/


/*******************************************************************************/
/*  FUNCTION  startoutascii                                                    */
/*    OPEN ASCII-FILES FOR TIME SERIES OUTPUT (spatial means or station output)*/
/*   and WRITE FIRST 2 ROWS                                                    */
/*-----------------------------------------------------------------------------*/
void startoutascii();

/*******************************************************************************/
/*  FUNCTION  startmeltstakes
      OPEN FILE FOR ASCI-OUTPUT OF MELT FOR STAKE LOCATIONS
      OUTPUT OF CUMULATIVE MELTING AND CUMULATIVE MASS BALANCE                 */
/*******************************************************************************/
void startmeltstakes();

/*******************************************************************************/
/*  FUNCTION  startspecificmassbalance
      OPEN FILE FOR ASCI-OUTPUT OF SPECIFIC MASS BALANCE FOR EACH YEAR
      ONLY IF summer and winter mass balance COMPUTED AND ONLY USEFUL FOR
      MULTIYEAR RUNS;  CALLED FROM MAIN ONCE
      NEW: 3/2004  */
/*******************************************************************************/
void startspecificmassbalance();

/*******************************************************************************/
/*  FUNCTION  opensnowfree                                                     */
/*      OPEN FILE FOR TIME SERIES OF HOW MANY PIXELS ARE SNOW FREE EVERY DAY   */
/*******************************************************************************/
void opensnowfree();

/*******************************************************************************/
/*  FUNCTION  startarrayreserve
      RESERVE STORAGE AND INITIALIZE OUTPUT-ARRAYS FOR GRID MODEL RESULTS      */
/*******************************************************************************/
void startarrayreserve();

/*******************************************************************************/
/*  FUNCTION  glacierrowcol                                                    */
/*    FIND THE FIRST AND LAST ROW WITH NO MISSING VALUES IN THE DRAINAGE GRID  */
/*    AND FIND FOR EACH ROW WITH DRAINAGE BASIN GRIDS THE FIRST AND LAST COLUMN*/
/* to speed up the program, calculations are only done for basin grid cells    */
/*******************************************************************************/
void glacierrowcol();

/**************************************************************/
/*  FUNCTION  readclim                                        */
/*        READ ONE LINE OF CLIMATE DATA FROM FILE             */
/*          called from main every time step                  */
/**************************************************************/
void readclim();

/********************************************************************/
/*  FUNCTION  exitclimread                                          */
/*       EXITS PROGRAM IF READ INPUT DATA IS OUTSIDE LOGICAL VALUES */
/*          called from every time step                    */
/********************************************************************/
void exitclimread();

/********************************************************************/
/*  FUNCTION  readdatesmassbal                                      */
/*       reads the dates of mass balance measurements               */
/*          called from main once                                   */
/********************************************************************/
void readdatesmassbal();

/********************************************************************/
/*  FUNCTION  areaelevationbelts                                    */
/*       counts the number of grid cells for each elevation band    */
/*       called from main once,  May 2006                           */
/********************************************************************/
void areaelevationbelts();

#endif /* MELT_MOD_INITIAL_H_ */
