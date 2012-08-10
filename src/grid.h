/***********************************************************************
 * Copyright 2012 Regine Hock, Carleen Tijm-Reijmer, Lyman Gillispie
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
/******************************************************************************
 * grid.h
 * This File is a part of the Hock Melt Model
 *
 *  Contains functions for allocating and deallocating matricies/tensors
 *
 *  Created on: Jul 3, 2012
 *      Author: Lyman Gillispie
 *****************************************************************************/

#ifndef MELT_MOD_GRID_H_
#define MELT_MOD GRID_H_

/**************************************************************/
/* FUNCTION   matrixreserv                                    */
/* reserviert Speicher fuer eine float Matrix m[nr..nrh][ncl..nch]  */
/*   aus Numerical Receipes in C, page 944                    */
/**************************************************************/

float **matrixreserv(int nrl, int nrh, int ncl, int nch);

/**************************************************************/
/* FUNCTION   matrixreservdouble                              */
/* reserviert Speicher fuer eine Matrix m[nr..nrh][ncl..nch]  */
/*   fuer DOUBLE :  aus Numerical Receipes in C               */
/**************************************************************/

double **matrixreservdouble(int nrl, int nrh, int ncl, int nch);

/*******************************************************************/
/* FUNCTION freematrix.c*/
/*   gibt reservierten Speicherplatz der Gridfiles frei ******/
/*******************************************************************/
void freematrix(float **m,int nrl, int nrh, int ncl, int nch);

/*****************************************************************/
/* FUNCTION freematrixdouble.c*/
/*   gibt reservierten Speicherplatz der Gridfiles frei     ******/
/*****************************************************************/
void freematrixdouble(double **m,int nrl, int nrh, int ncl, int nch);

/*******************************************************/
/* SET MARTIX-ARRAY TO ZERO                            */
/*******************************************************/
void initializeglacier2zero_nodata(int nrows, int ncols, float **glaciergrid);

/*******************************************************/
/* SET DOUBLE MARTIX-ARRAY TO ZERO                     */
/*******************************************************/
void initializeglacier2zero_nodatadouble(int nrows, int ncols, 
        double **glaciergrid);

/****************************************************************************/
/* FUNCTION   arrayreserv                                                   */
/* reserves space for a 1d matrix t[ndl..ndh]                               */
/* from Numerical Receipes in C: vector()                                   */
/****************************************************************************/
float *arrayreserv(int ndl, int ndh);

/****************************************************************************/
/* FUNCTION   arrayreservdouble                                             */
/* reserves space for a 1d matrix t[ndl..ndh]                               */
/* from Numerical Receipes in C: dvector()                                  */
/****************************************************************************/
double *arrayreservdouble(int ndl, int ndh);

/****************************************************************************/
/* FUNCTION freearray.c                                                     */
/*   gives reserved space of 1d matrix free                                 */
/*   from Numerical Receipes in C: free_vetor()                             */
/****************************************************************************/
void freearray(float *v, int ndl, int ndh);

/****************************************************************************/
/* FUNCTION freearray.c                                                     */
/*   gives reserved space of 1d matrix free                                 */
/*   from Numerical Receipes in C: free_vetor()                             */
/****************************************************************************/
void freearraydouble(double *v, int ndl, int ndh);

/****************************************************************************/
/* FUNCTION   tensorreserv                                                  */
/* reserves space for a 3d matrix t[nr..nrh][ncl..nch][ndl..ndh]            */
/* from Numerical Receipes in C: f3tensor()                                 */
/****************************************************************************/
float ***tensorreserv(int nrl, int nrh,
        int ncl, int nch,
        int ndl, int ndh);

/****************************************************************************/
/* FUNCTION   tensorreservdouble                                            */
/* reserves space for a 3d matrix t[nr..nrh][ncl..nch][ndl..ndh]            */
/* from Numerical Receipes in C: f3tensor()                                 */
/****************************************************************************/
double ***tensorreservdouble(int nrl, int nrh,
        int ncl, int nch,
        int ndl, int ndh);

/****************************************************************************/
/* FUNCTION freetensor.c                                                    */
/*   gives reserved space of 3d matrix free                                 */
/*   from Numerical Receipes in C: free_f3tensor()                          */
/****************************************************************************/
void freetensor(float ***t,
        int nrl, int nrh,
        int ncl, int nch,
        int ndl, int ndh);

/****************************************************************************/
/* FUNCTION freetensordouble.c                                               */
/*   gives reserved space of 3d matrix free                                 */
/*   from Numerical Receipes in C: free_f3tensor()                          */
/****************************************************************************/
void freetensordouble(double ***t,
        int nrl, int nrh,
        int ncl, int nch,
        int ndl, int ndh);

/****************************************************************************/
/* SET TENSOR-ARRAY TO ZERO                                                 */
/****************************************************************************/
void initializeglacier2zero_nodata_tens(int nrows, int ncols, int ndepths,
        float ***glaciergrid);

/****************************************************************************/
/* SET TENSOR-ARRAY TO ZERO                                                 */
/****************************************************************************/
void initializeglacier2zero_nodata_tensdouble(int nrows, int ncols, int ndepths,
        double ***glaciergrid);



#endif /* GRID_H_ */
