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
/*==========================================================================*/
/*  FILE discharg.h                                                         */
/*     function startdischarg for initialization of discharge calculations  */
/*       (for both sim and optimization of discharge or precip parameters)  */
/*     function discharge for discharge calculation (only simulation run)   */
/*     function writedisoutput to print simulated discharge to output file  */
/*==========================================================================*/

#ifndef MELT_MOD_DISCHARG_H_
#define MELT_MOD_DISCHARG_H_

/**************************************************************************/
/*   FUNCTION  opendiscalc                                                */
/*        OPENING DISCHARGE OUTPUTFILE                                    */
/*        called once from startdischarg, only if simulation run          */
/**************************************************************************/
void opendiscalc();

/**************************************************************************/
/*  FUNCTION startarraysdis                                               */
/*      STORAGE RESERVATION (dynamical) AND INITIALIZATION                */
/*      needed for all discharge calculations (meltmod, degree, ddfopt)   */
/*      called once from startdischarg (below) and startddfopt            */
/*      called only if discharge data available (disyes = 1)              */
/**************************************************************************/
void startarraysdis();

/**************************************************************************/
/*  FUNCTION startarraysopt                                               */
/*     storage reservation and initialization of additional arrays        */
/*     needed for optimization run (both optimitization of k-values       */
/*     (meltmod, degree) and of melt parameters (ddfopt.c)                */
/**************************************************************************/
void startarraysopt();

/**************************************************************************/
/*  FUNCTION openr2matrizkopt                                             */
/*      OPEN R2-MATRIX OUTPUTFILE AND WRITE HEAD                          */
/*      only for optimization k-values                                    */
/**************************************************************************/
void openr2matrizkopt();

/**************************************************************************/
/*   FUNCTION  startdischarg                                              */
/*    called once from main of meltmod, degree                            */
/*       for both simulation and optimization of discharge parameters     */
/*       not applicable for ddfopt.c (different function)                 */
/*     calls functions to initialize discharge simulation                 */
/*       OPENS THE FILE OF FIRNAREA, DISCHARGE OUTPUTFILE,                */
/*       MEASURED DISCHARGE FILE                                          */
/**************************************************************************/
void startdischarg();

/**************************************************************************/
/*  FUNCTION  rainoutsideglac                                             */
/*    distributes rain falling outside the calculated area (e.g. glacier) */
/*    onto the glacier, in order to route rain, which falls outside the   */
/*    glacier through the glacier, this is only necessary, if the area    */
/*    calculated is smaller than the drainage basin                       */
/*      called from main every time step after grid computed              */
/**************************************************************************/
void rainoutsideglac();

/**************************************************************************/
/*  FUNCTION  negativemelt                                                */
/*      set melt water to zero, if energy balance is negative             */
/*      called from discharge (below)                                     */
/**************************************************************************/
void negativemelt();

/**************************************************************************/
/*  FUNCTION  meltrainvol                                                 */
/*    add melt and rain water and convert to volume                       */
/*    called from discharge (below) and dischargeopt for each drainage grid cell */
/**************************************************************************/
void meltrainvol();

/**************************************************************************/
/*  FUNCTION  whichreservoir                                              */
/*      in which reservoir (firn, snow, ice, rock) is the grid cell located */
/*      called from discharge (below) for each grid cell                  */
/*   rock is area outside glacier but not snow-covered, if snow-covered   */
/*   belongs to snow reservoir, only relevant if drainage basin larger than glacier */
/**************************************************************************/
void whichreservoir();

/*****************************************************************************/
/*  FUNCTION  sumr2                                                          */
/*     sum up sums required for r2 calculations (simulation and optimization */
/*      called from discharge() (below),    only if discharge data available */
/*****************************************************************************/
void sumr2();

/*****************************************************************************/
/*  FUNCTION  sumr2ln                                                        */
/*     sum up sums required for logarithmic r2 calculations                  */
/*      called from discharge() (below),    only if discharge data available */
/*****************************************************************************/
void sumr2ln();

/**************************************************************************/
/*  FUNCTION  writedisoutput                                              */
/*      write simulated discharge to file                                 */
/*      called from discharge() (below), only for simulation run          */
/**************************************************************************/
void writedisoutput();

/**************************************************************************/
/*   FUNCTION  discharge                                                  */
/*    called every time step from main of meltmod, degree                 */
/*    only simulation runs, no optimization of k-values or melt parameters */
/**************************************************************************/
void discharge();

/**************************************************************************/
/* FUNCTION r2cal CALCULATES THE r2-CRITERIUM                             */
/*     same function for simulation and optimization                      */
/**************************************************************************/
void r2calc();

/**************************************************************************/
/* FUNCTION r2calln CALCULATES THE logarithmic r2-CRITERIUM               */
/*     same function for simulation and optimization                      */
/**************************************************************************/
void r2calcln();



#endif /* MELT_MOD_DISCHARG_H_ */
