/* This File is Part of the Hock Melt Model
 * melt_mod_globcor.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 */

#ifndef MELT_MOD_GLOBCOR_H_
#define MELT_MOD_GLOBCOR_H_

void jd2monat();
double deklination(int tag, int monat);
double gettimeequation(int tag,int monat);
double sonnenhoehe(int tag,int monat,double breite,double laenge,double stunde);
void getsuncoordinates(int tag,int monat,double breite,double laenge,double stunde,
                       double *sunazimuth,double *sunaltitude);

double getcosincidence(double aspect,double slope,double azimuth,double zenith);
void sunrise(int tag,int monat,double breite,double laenge,double *sr,double *ss);
void radiussunearth();
void schatten();
void getzenith();
void topofatmosphere();
void readdirect();



#endif /* GLOBCOR_H_ */
