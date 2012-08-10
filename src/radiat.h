/***********************************************************************
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
/* This File is Part of the Hock Melt Model
 * radiat.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 */

/****************************************************************************/
/*  FILE  :  radiat.h                                                       */
/*  CALCULATION OF GLOBAL RADIATION and                                     */
/*                  SHORT-WAVE RADIATION BALANCE AND NET RADIATION          */
/*    Authored 5/25/2012                                                    */
/****************************************************************************/

#ifndef MELT_MOD_RADIAT_H_
#define MELT_MOD_RADIAT_H_
/*================ GLOBAL RADIATION ======================================*/
/*********************************************************************/
/*  FUNCTION : ratioglobal                                           */
/*             for each time step, but constant for all grids        */
/*    needed if global radiation not split into direct and diffus    */
/*    (meltmod.c) and if temp index method 3 used (degree.c,ddfopt.c)*/
/*  updated 1 Jun 2006*/
/*********************************************************************/
void ratioglobal();

/******************************************************************/
/*  FUNCTION : globradratio
       CALCULATION OF SHORTWAVE RADIATION                         */
/******************************************************************/
void globradratio();

/*========================================================================*/
/*  EXTRAPOLATE DIRECT AND DIFFUSE RADIATION SEPARATELY                   */
/******************************************************************/
/*  FUNCTION : splitdiffusedirectcol                              */
/*             CALCULATION OF DIFFUSE RADIATION AT STATION        */
/*             empirical function according to Collares and Rabl  */
/*    called once every time step                                 */
/******************************************************************/
void splitdiffusedirectcol();

/******************************************************************/
/*  FUNCTION : splitdiffusedirectkiruna                           */
/*      CALCULATION OF DIFFUSE AND DIRECT RADIATION AT STATION    */
/*             empirical function based on Kiruna data            */
/*    called once every time step                                 */
/******************************************************************/
void splitdiffusedirectkiruna();

/******************************************************************/
/* FUNCTION : ratiodirectdirect                                   */
/*   CALCULATION OF RATIO OF DIRECT AND CLEAR-SKY DIRECT RADIATION*/
/*   AT THE CLIMATE STATION                                       */
/*      this ratio is used to interpolate direct radiation        */
/*      called once every time step from main                     */
/******************************************************************/
void ratiodirectdirect();

/******************************************************************/
/*  FUNCTION : interpoldirect                                     */
/*             INTPERPOLATE DIRECT RADIATION AS SEPARATED FROM    */
/*             GLOBAL RADIATION AT THE CLIMATE STATION            */
/*             interpolation according to Pluess (1997)           */
/*     called for each grid cell and every time step              */
/******************************************************************/
void interpoldirect();

/******************************************************************/
/*  FUNCTION : diffusunobstructed                                 */
/*             INTPERPOLATE DIFFUSE RADIATION AS SEPARATED FROM   */
/*             GLOBAL RADIATION AT THE CLIMATE STATION            */
/******************************************************************/
void diffusunobstructed();

/******************************************************************/
/*  FUNCTION : interpoldiffus
               INTPERPOLATE DIFFUSE RADIATION AS SEPARATED FROM
               GLOBAL RADIATION AT THE CLIMATE STATION            */
/******************************************************************/
void interpoldiffus();

/******************************************************************/
/*  FUNCTION : adddirectdiffus                                    */
/*             ADD DIRECT AND DIFFUSE RADIATION                   */
/*    called from main every time step and each grid cell         */
/* LG: do we need adddirectdifus()? it's used in one place and is one-line*/
/******************************************************************/
void adddirectdiffus();

/*================== ALBEDO ============================================================*/
/***********************************************************************************/
/*  FUNCTION  albedoread                                                           */
/*      DETERMINATION OF ALBEDO: ALBEDO IS READ FROM FILES VALID FOR FIXED PERIODS */
/*      ALLOCATION OF ALBEDOFILE, CHOOSE THE RIGHT ONE OPEN AND READ FILE          */
/***********************************************************************************/
void albedoread();

/****************************************************************************/
/*  FUNCTION : albedocalcconst                                              */
/*      TEMPORALLY CONSTANT VALUES ARE ALLOCATED TO DIFFERENT SURFACE TYPES */
/****************************************************************************/
void albedocalcconst();

/*******************************************************************/
/*  FUNCTION : albedosnowmeas                                      */
/*      TEMPORALLY CONSTANT VALUES ARE ALLOCATED TO SLUSH AND ICE  */
/*      BUT VARYING VALUES FOR THE SNOW SURFACE (daily mean values */
/*      read from file)                                            */
/*******************************************************************/
void albedosnowmeas();

/******************************************************************/
/*  FUNCTION  inialbedoalt                                        */
/*     initialize ALBALT (albedo array of previous time step)     */
/*     if albedo generated by model, used for snow albedo         */
/******************************************************************/
void inialbedoalt();

/******************************************************************/
/* FUNCTION  albedosnownew                                        */
/*           new function using days since snow fall and air temp */
/*    called from function albedocalc for each grid cell          */
/*  modification snow depth dependency if depthyes = 1            */
/*  by Carleen Tijm-Reijmer, 2/2005                               */
/******************************************************************/
void albedosnownew();

/******************************************************************/
/* FUNCTION  albedosnowclouds                                     */
/*     adjust albedo just computed with new albedo function  (albedosnownew) */
/*     including the effects of clouds according to Jonsell et al., 2003, J.Glaciol. */
/*     should only be used for hourly or similar time steps       */
/*    called from function albedosnownew for each grid cell       */
/******************************************************************/
void albedosnowclouds();

/******************************************************************/
/* FUNCTION  albedocalc                                           */
/*    for method 2: internal generation of albedo                 */
/*    called from main every time step and for each grid cell     */
/******************************************************************/
void albedocalc();

/******************************************************************/
/* FUNCTION : whichsurface                                        */
/*    allocate an integer to surface (of drainage DEM) depending  */
/*    on whether or not it is still snow-covered, firn, ice or rock*/
/*    purpose : to write to output for validation of snow line    */
/*              and used in albedocalc, discharge                 */
/*    called from main for every time step before grid cell loop  */
/*           and once from inialbedoalt                           */
/******************************************************************/
void whichsurface();

/******************************************************************/
/* FUNCTION : meanalbedo                                          */
/*    calculate spatial mean of ALBEDO over entire drainage basin */
/*    for diffuse radiation from surrounding terrain              */
/*    called once every time step                                 */
/******************************************************************/
void meanalbedo();

/******************************************************************/
/* FUNCTION  albedosnownewpoly                                     */
/*    13/03/2006, sicart PhD., Zongo Glacier                       */
/*    called from main     */
/******************************************************************/
/** if prec=0 snow depth: US army corps of engineers            ***/
/** if prec=0 snow thin: polynomial in power 3                  ***/
/** if prec=0 ice : linear decrease, not done yet               ***/
/** if prec>0 : albedo increase according to hock               ***/
/** all concerning "albedo is allowed only to drop to the value of ALBEDO BEFORE SNOWFALL
    after that the decay continues as if there were no snowfall"
	SUPPRESSED HERE!!! changed the 03 of march, 2001**/
/******************************************************************/
void albedosnowpoly();

/******************************************************************/
/*  FUNCTION : shortwavebalance                                   */
/******************************************************************/
void shortwavebalance();

/*========================================================================*/
/*   LONGWAVE RADIATION                                                   */
/******************************************************************/
/*  FUNCTION : longinstationnetradmeas                                  */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION */
/*             FROM MEASUREMENTS OF NET, GLOB, REFLECTED          */
/*             called from main once per time step                */
/******************************************************************/
void longinstationnetradmeas();

/******************************************************************/
/*  FUNCTION : longinstationkonzel                                */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION */
/*             FROM PARAMETERIZATION by Konzelmann et al. 1995    */
/******************************************************************/
void longinstationkonzel();

/******************************************************************/
/*  FUNCTION : cloudsfromratioglobTOASedlar                       */
/*     CALCULATION OF CLOUD AMOUNT FROM THE RATIO OF MEASURED     */
/*     GLOBAL RADIATION AND TOP OF ATMOSPHERE RADIATION           */
/*       coefficients derived on Storglaciaren, Sedlar and Hock, 2009 */
/*   update Nov 2006, updated 9/2009*/
/******************************************************************/
void cloudsfromratioglobTOASedlar();

/******************************************************************/
/*  FUNCTION : longinstationbrunts                                */
/*             CALCULATION LONGWAVE INCOMING RADIATION AT STATION */
/*             FROM PARAMETERIZATION by Brunts                    */
/******************************************************************/
void longinstationbrunts();

/******************************************************************/
/*  FUNCTION : longinstationbrutsaert                                       */
/*    CALCULATION LONGWAVE INCOMING RADIATION AT STATION                    */
/*    ROM PARAMETERIZATION by Brutsaert for clear sky, zongo for clouds     */
/*    cloud is read from climate input but the column was computed	priorly */
/*    from ratio global radiation/topofatmosphere, fitted to Zongo data     */
/*    CLOUD VARIES FROM 0 TO 1                                              */
/*    see Sicart, 2002, PhD thesis, p.260                                   */
/*    March 2006                                                            */
/******************************************************************/
void longinstationbrutsaert();

/***************************************************************************/
/*  FUNCTION : longinskyunobstructed                                       */
/*     sky longwave radiation obtained at climate station from rad data    */
/*     corrected as if the site was unobstructed by topography             */
/*     prior to interpolation over area  (based on Pluess, 1997)           */
/*     called from main once per timestep                                  */
/***************************************************************************/
void longinskyunobstructed();

/***************************************************************************/
/*  FUNCTION : longinpluess                                                */
/*     parameterization of longwave incoming rad according to Pluess, 1997 */
/*     called from main for each grid cell                                 */
/***************************************************************************/
void longinpluess();

/******************************************************************/
/*  FUNCTION : longoutcalc                                        */
/*             calculate longwave outgoing (Stephan Boltzmann)    */
/*             for each time step and each grid point             */
/* update 4/2010  include emissivities < 1   */
/******************************************************************/
void longoutcalc();

/*************************************************************************/
/*  FUNCTION :  surftempfromlongout                                      */
/*     calculate surface temp from longwave outgoing (Stephan Boltzmann) */
/*     for each time step and the climate station grid cell              */
/* update 4/2010  include emissivities < 1   */
/*************************************************************************/
void surftempfromlongout();

/*=================== RADIATION BALANCE ================================*/
/******************************************************************/
/*  FUNCTION : radiationbalance                                   */
/*             for each time step and each grid point             */
/*             longwave incoming assumed spatially constant       */
/******************************************************************/
void radiationbalance();

#endif /* RADIAT_H_ */
