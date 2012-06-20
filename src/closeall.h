/* This file is part of the Hock Melt Model
 *
 * closeall.h
 *
 *  Created on: May 25, 2012
 *      Author: Lyman Gillispie
 */

#ifndef MELT_MOD_CLOSEALL_H_
#define MELT_MOD_CLOSEALL_H_

void freematrix(float **m,long nrl, long nrh, long ncl, long nch);
void freematrixdouble(double **m,long nrl, long nrh, long ncl, long nch);
void closeall();

#endif /* MELT_MOD_CLOSEALL_H_ */
