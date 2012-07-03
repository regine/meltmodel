/*
 * grid.h
 * This File is a part of meltmod
 *
 *  Created on: Jul 3, 2012
 *      Author: cminor
 */

#ifndef MELT_MOD_GRID_H_
#define MELT_MOD GRID_H_

/*============================================================*/
/**************************************************************/
/* FUNCTION   matrixreserv                                    */
/* reserviert Speicher fuer eine float Matrix m[nr..nrh][ncl..nch]  */
/*   aus Numerical Receipes in C, page 944                    */
/**************************************************************/

float **matrixreserv(long nrl, long nrh, long ncl, long nch);

/*============================================================*/
/**************************************************************/
/* FUNCTION   matrixreservdouble                              */
/* reserviert Speicher fuer eine Matrix m[nr..nrh][ncl..nch]  */
/*   fuer DOUBLE :  aus Numerical Receipes in C               */
/**************************************************************/

double **matrixreservdouble(long nrl, long nrh, long ncl, long nch);

/*******************************************************/
/* SET MARTIX-ARRAY TO ZERO                            */
/*******************************************************/

void initializeglacier2zero_nodata(float **glaciergrid);

void initializeglacier2zero_nodatadouble(double **glaciergrid);

void checkgridinputdata_ok();



#endif /* GRID_H_ */
