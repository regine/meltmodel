/*
 * initial.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 */

#ifndef MELT_MOD_INITIAL_H_
#define MELT_MOD_INITIAL_H_

#include<stdio.h>

void readrestofline(FILE **infile);

void toobig_resout();

void closefile(FILE **datei, char *name);
void meandayreserve();
void meandaynull();
void meanallnull();
void startinputdata();
void startoutascii();
void startmeltstakes();
void startspecificmassbalance();
void opensnowfree();
void startarrayreserve();
void glacierrowcol();
void readclim();
void exitclimread();
void readdatesmassbal();
void areaelevationbelts();

#endif /* MELT_MOD_INITIAL_H_ */
