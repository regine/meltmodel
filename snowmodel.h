/* This File is Part of the Hock Melt Model
 * snowmodel.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 */

#ifndef MELT_MOD_SNOWMODEL_H_
#define MELT_MOD_SNOWMODEL_H_

float *arrayreserv(long ndl, long ndh);
double *arrayreservdouble(long ndl, long ndh);
float ***tensorreserv(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
double ***tensorreservdouble(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void freearray(float *v, long ndl, long ndh);
void freearraydouble(double *v, long ndl, long ndh);
void freetensor(float ***t,long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void freetensordouble(double ***t,long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void initializeglacier2zero_nodata_tens(float ***glaciergrid);
void initializeglacier2zero_nodata_tensdouble(double ***glaciergrid);
void statistdouble(double **matrix,float *x);
void meandaysnownull();
void albedocalcdepth();
void albedosnowdepth();
void initgrid();
void changegrid();
void resetgridwinter();
void resetgridsummer();
void surftempfrommodel();
void surftempskin();
void bisection();
void iceconductivity();
void densification();
void snowmelt();
void refreezing();
void percolation();
void slushformation();
void irreducible_schneider();
void irreducible_coleou();
void subsurf();
void interpolate();
void turbfluxes();
void roughnesslengthAndreasmodified();
void spechum();
void stable();
void unstable();
void sensiblenew();
void latentnew();
void tsurfenergybalance(double tskin);

#endif /* SNOWMODEL_H_ */
