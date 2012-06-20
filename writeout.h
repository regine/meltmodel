/****************************************************************************/
/* FILE   writeout.h                                                        */
/*        WRITE ENERGY BALANCE OUTPUT FOR EACH GRID TO OUTPUT-FILES         */
/*        Part of the Hock Melt Model                                       */
/*  Author: Lyman Gillispie                                                 */
/*  Created on: May 25, 2012                                                */
/****************************************************************************/

#ifndef MELT_MOD_WRITEOUT_H_
#define MELT_MOD_WRITEOUT_H_

#include<stdio.h>

void openoutfile(FILE **datei,char *outpath,char *name);
void openoutfileascii(FILE **datei,char *outpath,char *name);
void startwritehour();
void startwriteday();
void startwriteall();
void statist(float **matrix,float *x);
void writegridoutput();
void sumday();
void sumall();
void writesnoworsurfaceyes();
void writesurface();
void writesnowcover();
void writemassbalgrid();
void computemassbalprofile(float **matrixprofile,float *massbalbands);
void meanmassbalprofile();
void areameannull();
void areasum();
void areameanwrite();
void stationoutput();
void writemeltstakes();
void percentsnowfree();


#endif /* WRITEOUT_H_ */
