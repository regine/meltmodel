/***********************************************************************
 * Copyright 2012 Regine Hock, Lyman Gillispie
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
/* This File is Part of the Hock Melt Model
 * tindex.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 */


/*================================================================*/
/* FILE tindex.h                                                  */
/* This file contains all functions which are needed              */
/* to calculate melt by temperature index methods and             */
/* discharge functions needed if melt parameters are optimized    */
/* Authored May/25, 2012 */
 /*================================================================*/

#ifndef MELT_MOD_TINDEX_H_
#define MELT_MOD_TINDEX_H_

/*****************************************************************/
/* FUNCTION  degreestart
      to avoid that grid output files are requested that can
      not be created if melt is not calculated by energy balance */
/*****************************************************************/
void degreestart();

/*****************************************************************/
/*  FUNCTION  temppos                                            */
/*****************************************************************/
void temppos();

/******************************************************************/
/*  FUNCTION  ddfcalculation                                      */
/*    calculate the degree day factor as the ratio between        */
/*      calculated melt and positive temperature                  */
/******************************************************************/
void ddfcalculation();

/******************************************************************/
/*  FUNCTION  degreedaymelt
     method 1
     reduced melt over debris on glacier possible
     calculating melt using a simple degree day method            */
/******************************************************************/
void degreedaymelt();

/******************************************************************/
/*  FUNCTION   dd_directmelt                                      */
/*     method 2                                                   */
/*     including direct radiation in melt calculation             */
/******************************************************************/
void dd_directmelt();

/******************************************************************/
/*  FUNCTION  dd_directglobal                                     */
/*     method 3                                                   */
/*     direct radiation term reduced under cloudier conditions    */
/******************************************************************/
void dd_directglobal();

/***************************************************************************/
/* FUNCTION  snowcoverdegree                                               */
/*     calculate water equivalent of snow cover after melt or snow fall    */
/*     called from main for every time step and each grid cell             */
/***************************************************************************/
void snowcoverdegree();

/*======================================================================*/
/*  FUNCTIONS FOR DISCHARGE CALCULATION                                 */
/******************************************************************/
/*  FUNCTION startdisddfopt                                       */
/*     DISCHARGE CALCULATION IF MELT PARAMETER OPTIMIZATION       */
/*     called once from main to initialize optimization run       */
/*     only optimization of melt parameters                       */
/*================================================================*/
void startdisddfopt();

/****************************************************************************/
/*   FUNCTION dischargeddfopt                                               */
/*         DISCHARGE CALCULATION                                            */
/*   called from main for every time step and every parameter constellation */
/****************************************************************************/
void dischargeddfopt();

/*======================================================================*/
/*  FUNCTIONS FOR COUPLING SNOW MODEL TO TEMP-INDEX METHOD              */
/******************************************************************/
/*  FUNCTION melt2energy                                          */
/*     convert temp-index determined melt into energy W/m2        */
/*     to be used for forcing in subsurf()                        */
/*================================================================*/
void melt2energy();


#endif /* TINDEX_H_ */
