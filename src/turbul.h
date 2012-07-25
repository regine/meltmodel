/****************************************************************************/
/* FILE   turbul.h                                          */
/*         SPATIAL INTERPOLATION OF METEOROLOGICAL INPUT FACTORS TO GRID    */
/*         temperature, air pressure, vapour pressure, precipitation       */
/*         CALCULATION TURBULENT HEAT FLUXES                                */
/*         Part of the Hock Melt Model                        */
/*  Author: Lyman Gillispie                                                 */
/*  Created on: May 25, 2012                                                */
/****************************************************************************/

#ifndef MELT_MOD_TURBUL_H_
#define MELT_MOD_TURBUL_H_

/**********************************************************/
/* FUNCTION  tempgradient                                 */
/*   CALCULATION OF TEMPERATURE OF WHOLE GRID             */
/*   USING A LINEAR DECREASE OF TEMP WITH ELEVATION       */
/* constant lapse rate or monthly varying read from input.dat */
/* or variable lapse rate read from climate data file     */
/*   called from function tempinterpol (every time step   */
/*   climate station either outside or inside the calculated area */
/*  NEW 27 April 2010 */
/**********************************************************/
void tempgradient();

/**********************************************************/
/* FUNCTION  tempinterpol                                 */
/*   CALCULATION OF TEMPERATURE OF WHOLE GRID             */
/*   USING A LINEAR DECREASE OF TEMP WITH ELEVATION       */
/*   OR READING TEMPERATURE GRID FILES FOR EACH TIME STEP */
/*   called from main every time step                     */
/*   climate station either outside or inside the calculated area */
/*  last update 13 April 2010 RH */
/**********************************************************/
void tempinterpol();

/***************************************************************************/
/* FUNCTION   airpress                                                     */
/*   CALCULATION OF AIR PRESSURE DEPENDING ON ELEVATION a.s.l.             */
/*   taken from KONZELMANN et al. Global and Planetary Change, 1994, p.161 */
/* called from main every time step and each grid cell, not stored in array*/
/***************************************************************************/
void airpress();

/***************************************************************************/
/* FUNCTION   vappress                                                     */
/*     CALCULATION OF VAPOUR PRESSURE FROM RELATIVE HUMIDITY               */
/*     aus Weischet, p.138                                                 */
/* called from main every time step and each grid cell, not stored in array*/
/***************************************************************************/
void vappress();

/***************************************************************************/
/* FUNCTION   vappressicesnow                                              */
/*     CALCULATION OF VAPOUR PRESSURE OF ICE OR SNOW SURFACE               */
/*     needed if surface temp lowered by iteration                         */
/***************************************************************************/
void vappressicesnow();

/* ========== TURBULENT FLUXES ============================================ */

/****************************************************************************/
/* FUNCTION   sensescher                                                    */

/* CALCULATION OF SENSIBLE HEAT HEAT ACCORDING TO ESCHER-VETTER, 1980, p.14 */
/*     sens = transfer coefficient x air temperature
       trans coeff = 5.7 W/m2/K * sqrt(wind speed) /m/s)
       energy transferred to surface defined as positive (condensation)
       assumption: surface temp = 0 -> melting surface                      */
/****************************************************************************/
void sensescher();

/***************************************************************************/
/* FUNCTION   latescher                                                    */

/* CALCULATION OF LATENT HEAT HEAT ACCORDING TO ESCHER-VETTER, 1980, p.15  */
/*   as a function of transfer coefficient (wind dependent),
     vapour pressure, air pressure, latent heat of evaporation, cp
       assumption: surface temp = 0 -> melting surface -> e0=611 Pa
		   relative humidity constant over whole area              */
/***************************************************************************/
void latescher();

/***************************************************************************/
/* FUNCTION   roughnesslength                                              */
/* CALCULATION OF ROUGHNESS LENGTHS ACCORDING TO turbulmethod=2            */
/*  constant roughness lengths, different z0w and z0T                      */
/*  function called for each grid cell from function sensible              */
/*       only if methodturbul = 2 (no stability)                           */
/***************************************************************************/
void roughnesslength();

/***************************************************************************/
/* FUNCTION   roughnesslengthAndreas                                       */
/* CALCULATION OF ROUGHNESS LENGTHS ACCORDING TO Andreas 1997              */
/*                      surface renewal theory, see Munro 1990             */
/*  function of Reynolds number, then assumed constant for entire glacier  */
/*  function called for climate station grid from function iterationstation */
/***************************************************************************/
void roughnesslengthAndreas();

/***************************************************************************/
/* FUNCTION   moninobukhovlength                                             */
/* CALCULATION OF FRICTION VELOCITY AND STABILITY FUNCTIONS, Munro 1990    */
/*   stability function according to Beljaar and Holtslag, 1991            */
/*  function called for climate station grid from function iterationstation */
/***************************************************************************/
void moninobukhovlength();

/***************************************************************************/
/* FUNCTION   stabilityBeljaar                                             */
/* CALCULATION OF STABILITY FUNCTIONS                                      */
/*   stability function (stable) according to Beljaar and Holtslag, 1991   */
/*  function called for climate station grid from function sensiblestabilityiteration */
/***************************************************************************/
void stabilityBeljaar();

/***************************************************************************/
/* FUNCTION   stabilityunstable                                            */
/* CALCULATION OF STABILITY FUNCTION FOR UNSTABLE CASE                     */
/*                     according to Panofsky and Dutton, 1984              */
/*    called for climate station grid from function iterationstation       */
/***************************************************************************/
void stabilityunstable();

/***************************************************************************/
/* FUNCTION   sensiblestabilityiteration                                  */
/* CALCULATION OF SENSIBILE HEAT FLUX FOR CLIMATE STATION GRID             */
/*   AND STABILITY FUNCTIONS (assumed constant for rest of grid)           */
/*  function called once every time step for climate station grid          */
/*     from function iterationstation (if surftemp is iterated             */
/*         and from main (if turbulmethod=3=stability)                     */
/***************************************************************************/
void sensiblestabilityiteration();

/***************************************************************************/
/* FUNCTION   sensible                                                     */
/* CALCULATION OF SENSIBLE HEAT HEAT ACCORDING TO Hock and Holmgren, 1996  */
/*  constant roughness lengths, different z0w and z0T                      */
/*  or according to Andreas (1987)*/
/***************************************************************************/
void sensible();

/***************************************************************************/
/* FUNCTION   latent                                                       */
/* CALCULATION OF LATENT HEAT HEAT as in Hock and Holmgren, 1996           */
/*  constant roughness lengths, different zow and zoT                      */
/***************************************************************************/
void latent();

/* =============== PRECIPITATION ================================================ */


/***************************************************************************/
/* FUNCTION   readprecipindexmap                                                */
/*     READ PRECIPITATION INDEX MAP                                             */
/*     contains a value for each grid cell by which the precip at the AWS       */
/*     is multiplied by, to account for spatially stable precip patters         */
/*    called from main once before the time step loop if methodprecipinterpol=2 */
/***************************************************************************/
void readprecipindexmap();

/***************************************************************************/
/* FUNCTION   readprecipfromfile                                           */
/*     READ PRECIPITATION FROM FILE FOR EACH TIME STEP                     */
/*    called from main for each time step loop if methodprecipinterpol=3   */
/***************************************************************************/
void readprecipfromfile();

/***************************************************************************/
/* FUNCTION   precipinterpol                                               */
/*            INTERPOLATION OF PRECIPITIATION, DETERMINE RAIN OR SNOW      */
/*       snow is assumed below the threshold temperature minus 1 degree    */
/*	 and rain above T0 plus 1 degree, inbetween there is a mixtures    */
/*	 of snow and rain which is determined by linear interpolation      */
/*   3 methods how to extrapolate precip over the area                  */
/*    called from main for every time step and for each grid               */
/***************************************************************************/
void precipinterpol();

/***************************************************************************/
/* FUNCTION   precipnoetzlidip                                             */
/*            INTERPOLATION OF PRECIPITIATION, DETERMINE RAIN OR SNOW      */
/*            only rain, no snow, no mixture snow and rain                 */
/*    called from main for every time step and for each grid               */
/*    function used by Ch. Noetzli for his MSc thesis*/
/***************************************************************************/
/* LG: does anyone call precipnoetzlidip ?*/
void precipnoetzlidip();

/***************************************************************************/
/* FUNCTION   precipenergy                                                 */
/*            CALCULATION OF RAIN ENERGY,  called from main for each grid  */
/***************************************************************************/
void precipenergy();

/***************************************************************************/
/* FUNCTION   energybalance                                                */
/*            CALCULATION OF ENERGY AVAILABLE FOR MELT (ENERGY BALANCE)    */
/***************************************************************************/
void energybalance();

/***************************************************************************/
/* FUNCTION  negenergybalance                                              */
/*     store negative energy balance for next time step                    */
/*      take into account nocturnal cooling of the surface                 */
/*     called from main for every time step and each grid cell             */
/***************************************************************************/
void negenergybalance();

/***************************************************************************/
/* FUNCTION   waterequivalentmelt                                          */
/*            CONVERT ENERGY TO WATER EQUILAVENT MELT (in mm/time step)    */
/*     quantity to use for discharge calculation                           */
/***************************************************************************/
void waterequivalentmelt();

/***************************************************************************/
/* FUNCTION  waterequivalentabla                                           */
/*           CONVERT ENERGY TO WATER EQUILAVENT ABlation (in mm/time step) */
/*     quantity to compare with mass balance data                          */
/*     ABLA can be negativ = surface raising due to re-sublimation         */
/***************************************************************************/
void waterequivalentabla();

/***************************************************************************/
/* FUNCTION  snowcover                                                     */
/*     calculate water equivalent of snow cover after melt or snow fall    */
/*     called from main for every time step and each grid cell             */
/*         only energy balance model                                       */
/***************************************************************************/
void snowcover();

/***************************************************************************/
/* FUNCTION  massbalance                                                   */
/*     calculate mass balance (acc-abl) for winter and summer in cm        */
/*     called from main for every time step and each glacier grid cell     */
/***************************************************************************/
void massbalance();

/***************************************************************************/
/* FUNCTION  iterationstation                                              */
/*     calculate energy balance only for climate station grid and iterate  */
/*     surface temperature or compute it from longwave measurements        */
/*     called from main for every time step in case surftemp iterated      */
/***************************************************************************/
void iterationstation();


#endif /* TURBUL_H_ */
