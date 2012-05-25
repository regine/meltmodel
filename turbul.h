/****************************************************************************/
/* FILE   turbul.h                                          */
/*         SPATIAL INTERPOLATION OF METEOROLOGICAL INPUT FACTORS TO GRID    */
/*         temperature, air pressure, vapour pressure, precipitation       */
/*         CALCULATION TURBULENT HEAT FLUXES                                */
/*         Part of the Hock Melt Model                        */
/*  Author: Lyman Gillispie                                                 */
/*  Created on: May 25, 2012                                                */
/****************************************************************************/

#ifndef MELT_MOD_TURBUL_H_
#define MELT_MOD_TURBUL_H_

void tempgradient();
void tempinterpol();
void airpress();
void vappress();
void vappressicesnow();
void sensescher();
void latescher();
void roughnesslength();
void roughnesslengthAndreas();
void moninobukhovlength();
void stabilityBeljaar();
void stabilityunstable();
void sensiblestabilityiteration();
void sensible();
void latent();
void readprecipindexmap();
void readprecipfromfile();
void precipinterpol();
void precipnoetzlidip();
void precipenergy();
void energybalance();
void negenergybalance();
void waterequivalentmelt();
void waterequivalentabla();
void snowcover();
void massbalance();
void iterationstation();


#endif /* TURBUL_H_ */
