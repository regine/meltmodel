/* **********************************************
 * This File is Part of the Hock Melt Model
 * snowmodel.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 ************************************************/

#ifndef MELT_MOD_SNOWMODEL_H_
#define MELT_MOD_SNOWMODEL_H_


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
void outputsubsurf();
void outputsubsurflines();

#endif /* SNOWMODEL_H_ */
