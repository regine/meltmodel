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
/* =========================================================================== */
/*  FILE  disopt.h                                                            */
/*     functions to calculate discharge if discharge or precipitation          */
/*     parameters are optimized                                                */
/*     (not for optimization of melt parameters (ddfopt.c different function)  */
/*     called from main (meltmod.c and degree.c) every time step               */
/*  created 5.25.2012 */
/* =========================================================================== */

#ifndef MELT_MOD_DISOPT_H_
#define MELT_MOD_DISOPT_H_

/**************************************************************************/
/*  FUNCTION dischargeopt                                                 */
/*      DISCHARGE and PRECIPITATION CALCULATION FOR DIFFERENT VALUES OF   */
/*      2 OPTIMIATION PARAMETERS                                          */
/*      called from main every time step after melt grid has been calculated */
/**************************************************************************/
void dischargeopt();

/*** =================================================================== ***/
/***           FUNCTION  write r2-values-matrix to Output-file           ***/
/***                           called from main                          ***/
/*** =================================================================== ***/
void write2matriz();

#endif /* MELT_MOD_DISOPT_H_ */
