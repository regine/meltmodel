/***********************************************************************
 * Copyright 2012  Carleen Tijm-Reijmer, Regine Hock, Lyman Gillispie
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
/*  FILE  :  skintemperature.h                                                    */
/*    subsurface model                                                      */
/*    Authored: 5/25/2012                                                   */
/****************************************************************************/

#ifndef MELT_MOD_SKINTEMP_H_
#define MELT_MOD_SKINTEMP_H_

/****************************************************************************/
/* FUNCTION  surftempfrommodel                                                */
/*   this function calculates surface temperature of glacier by linear      */
/*   extrapolating temperature of upper two layers                          */
/****************************************************************************/
void surftempfrommodel();

/****************************************************************************/
/* FUNCTION  surftempskin                                                   */
/*   this function calculates surface temperature of glacier by             */
/*   using a skinlayer formulation                                          */
/*   a skin layer is a surface layer without heat capacity                  */
/*   the skinlayer temp is calculated by writing the surface EB in terms of  */
/*   it and deriving a formultion of Tskin                                  */
/*   longin and longout from previous timestep                              */
/****************************************************************************/
void surftempskin();

/****************************************************************************/
/* FUNCTION bisection (x1,x2,x_acc)  */
/****************************************************************************/
/* This function determines the surface temperature required to close the energy balance */
/****************************************************************************/
void bisection();

/*==============FUNCTION THAT WOULD BELONG TO TURBUL.C ====================== */

/****************************************************************************/
/* FUNCTION  turbfluxes                                                         */
/*   this function calculates the turbulent fluxes of heat and moisture     */
/****************************************************************************/
void turbfluxes();

/***************************************************************************/
/* FUNCTION   roughnesslengthAndreasmodified                               */
/* CALCULATION OF ROUGHNESS LENGTHS ACCORDING TO Andreas 1997              */
/*                      surface renewal theory, see Munro 1990             */
/*             modified as by Smeets et al 2006 in prep                    */
/*  function of Reynolds number, then assumed constant for entire glacier  */
/*  function called for climate station grid from function iterationstation */
/***************************************************************************/
void roughnesslengthAndreasmodified();

/****************************************************************************/
/* FUNCTION  spechum                                                        */
/*   this function calculates the specific humidity at surface and          */
/*   at observation level. At surface it is the saturation value            */
/****************************************************************************/
void spechum();

/***************************************************************************/
/* FUNCTION   stable                                                       */
/* CALCULATION OF STABILITY FUNCTIONS FOR STABLE CASE                      */
/*   stability function (stable) according to Beljaar and Holtslag, 1991   */
/***************************************************************************/
void stable();

/***************************************************************************/
/* FUNCTION   unstable                                                     */
/* CALCULATION OF STABILITY FUNCTION FOR UNSTABLE CASE                     */
/*                     according to Panofsky and Dutton, 1984              */
/***************************************************************************/
void unstable();

/***************************************************************************/
/* FUNCTION   sensiblenew                                                  */
/* CALCULATION OF SENSIBLE HEAT HEAT                                       */
/***************************************************************************/
void sensiblenew();

/***************************************************************************/
/* FUNCTION   latentnew                                                    */
/* CALCULATION OF LATENT HEAT HEAT                                         */
/***************************************************************************/
void latentnew();

/***************************************************************************/
/* FUNCTION   tsurfenergybalance                                                    */
/* calculates the energy balance as a function of the surface temperature (K)      */
/***************************************************************************/
void tsurfenergybalance(double tskin);

#endif /* SNOWMODEL_H_ */
