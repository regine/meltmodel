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
