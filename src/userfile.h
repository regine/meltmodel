/****************************************************************************/
/* FILE   userfile.h                                                      */
/*    Function, which are specific to a certain application                 */
/*    not generally applicable                                              */
/*        Part of the Hock Melt Model */
/*  Author: Lyman Gillispie                                                 */
/*  Created on: May 25, 2012                                                     */
/****************************************************************************/

#ifndef MELT_MOD_USERFILE_H_
#define MELT_MOD_USERFILE_H_

/*************************************************************/
/* FUNCTION  readalbedo29 */
/*   this function is specific to application to Storglaciaren */
/*************************************************************/
void readalbedo29();

/*********************************************************************/
/*  FUNCTION:  iceheatStorglac                                       */
/*     ice heat flux decreases linearly as function of time          */
/*********************************************************************/
void iceheatStorglac();

/* LG: are any of the below called by anyone? */

/*************************************************************/
/* FUNCTION  readalbedo16 */
/*    for 1993*/
/*************************************************************/
void readalbedo16();

/*********************************************************************/
/*  FUNCTION : tempsurfacerock                                       */
/*      calculation of surface temperature of area outside glacier   */
/*      needed for longwave incoming acc. to Pluess, 1997            */
/*      Parameterization derived from Kaerkevagge data               */
/*      2 different functions fitted to account for daily hysteresis */
/*      needs global radiation for grid cell                         */
/*      called from main for each grid cell                          */
/*********************************************************************/
void tempsurfacerock();

/*********************************************************************/
/*  FUNCTION:  openphotovernagt                                      */
/*     open file with photodata (once) and read first date           */
/*********************************************************************/
void openphotovernagt();

/*********************************************************************/
/*  FUNCTION:  readphotovernagt                                      */
/*     read one days matrix and next date                            */
/*     for application VERNAGTFERNER                                 */
/*********************************************************************/
void readphotovernagt();

/******************      ZONGO      ********************************************/


/******************************************************************/
/* FUNCTION  albedosnownewzgo         (adapted to Zongo glacier)  */
/*           new function using days since snow fall and air temp */
/*    called from function albedocalczgo for each grid cell       */
/*    no sqrt(nd) BUT in the exponentiel                          */
/******************************************************************/
void albedosnownewzgo();

/******************************************************************/
/*  FUNCTION : longinstationbruntsvernagt                         */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION */
/*             FROM PARAMETERIZATION by Brunts                    */
/*	       specified for Vernagt, because climate station not */
/*	       in area to be calculated                           */
/******************************************************************/
void longinstationbruntsvernagt();

/**********************************************************/
/* FUNCTION  measuredlapserate                            */
/*   CALCULATION OF TEMPERATURE LAPSE RATE FROM DATA FROM 2 STATIONS */
/*   called from new main (lapserate.c) every time step
     before function 'tempinterpol' */
/*   the temperature of the 2. climate station must be in the
     column 'cloud'   */
/*  for application King George Island  1/2001            */
/* APRIL 2010, THIS OPTION IS IMPLEMENTED VIA INPUT.DAT*/
/**********************************************************/
void measuredlapserate();


#endif /* USERFILE_H_ */
