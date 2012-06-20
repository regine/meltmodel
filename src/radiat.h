/* This File is Part of the Hock Melt Model
 * radiat.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 */

#ifndef MELT_MOD_RADIAT_H_
#define MELT_MOD_RADIAT_H_

void ratioglobal();
void globradratio();
void splitdiffusedirectcol();
void splitdiffusedirectkiruna();
void ratiodirectdirect();
void interpoldirect();
void diffusunobstructed();
void interpoldiffus();
/*
 * LG: do we need adddirectdifus()? it's used in one place and is one-line
 */
void adddirectdiffus();
void albedoread();
void albedocalcconst();
void albedosnowmeas();
void inialbedoalt();
void albedosnownew();
void albedosnowclouds();
void albedocalc();
void whichsurface();
void meanalbedo();
void albedosnowpoly();
void shortwavebalance();
void longinstationnetradmeas();
void longinstationkonzel();
void cloudsfromratioglobTOASedlar();
void longinstationbrunts();
void longinstationbrutsaert();
void longinskyunobstructed();
void longinpluess();
void longoutcalc();
void surftempfromlongout();
void radiationbalance();

#endif /* RADIAT_H_ */
