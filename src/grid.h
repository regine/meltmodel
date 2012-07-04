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

float **matrixreserv(int nrl, int nrh, int ncl, int nch);

/*============================================================*/
/**************************************************************/
/* FUNCTION   matrixreservdouble                              */
/* reserviert Speicher fuer eine Matrix m[nr..nrh][ncl..nch]  */
/*   fuer DOUBLE :  aus Numerical Receipes in C               */
/**************************************************************/

double **matrixreservdouble(int nrl, int nrh, int ncl, int nch);

/*******************************************************/
/* SET MARTIX-ARRAY TO ZERO                            */
/*******************************************************/
void freematrix(float **m,int nrl, int nrh, int ncl, int nch);
void freematrixdouble(double **m,int nrl, int nrh, int ncl, int nch);

void initializeglacier2zero_nodata(int nrows, int ncols, float **glaciergrid);
void initializeglacier2zero_nodatadouble(int nrows, int ncols, double **glaciergrid);

float *arrayreserv(int ndl, int ndh);
double *arrayreservdouble(int ndl, int ndh);
void freearray(float *v, int ndl, int ndh);
void freearraydouble(double *v, int ndl, int ndh);

float ***tensorreserv(int nrl, int nrh,
        int ncl, int nch,
        int ndl, int ndh);
double ***tensorreservdouble(int nrl, int nrh,
        int ncl, int nch,
        int ndl, int ndh);
void freetensor(float ***t,
        int nrl, int nrh,
        int ncl, int nch,
        int ndl, int ndh);
void freetensordouble(double ***t,
        int nrl, int nrh,
        int ncl, int nch,
        int ndl, int ndh);
/* LG: does anyone call  initializeglacier2zero_nodata_tens? */
void initializeglacier2zero_nodata_tens(int nrows, int ncols, int ndepths,
        float ***glaciergrid);
void initializeglacier2zero_nodata_tensdouble(int nrows, int ncols, int ndepths,
        double ***glaciergrid);



#endif /* GRID_H_ */
