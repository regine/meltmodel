/***********************************************************************
 * Copyright 2012  Carleen Tijm-Reijmer, Regine Hock, Lyman Gillispie
  * This file is part of DeBAM and DETiM.
 * 
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with This software.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/
/****************************************************************************/
/*  FILE  :  snowmodel.h                                                    */
/*    subsurface model                                                      */
/*    Authored: 5/25/2012                                                   */
/****************************************************************************/

#ifndef MELT_MOD_SNOWMODEL_H_
#define MELT_MOD_SNOWMODEL_H_

/*=======================================================================*/
/**** CALCULATION OF STATISTICS FOR SECOND 6 ROWS IN OUTPUTFILE       ****/
/*=======================================================================*/
void statistdouble(double **matrix,float *x);

/****************************************************************************/
/* FUNCTION  meandaysnownull
	     INITIALISATION OF ARRAYS FOR DAILY MEANS OF SNOW MODEL */
/*must be zero, because of summing up  */
/****************************************************************************/
void meandaysnownull();

/****************************************************************************/
/* FUNCTION  albedocalcdepth                                                */
/*    for method 4: internal generation of albedo                           */
/*    called from main every time step and for each grid cell               */
/****************************************************************************/
void albedocalcdepth();

/****************************************************************************/
/* FUNCTION  albedosnowdepth                                                */
/*   this function calculates snow albedo according to                      */
/*   either: Oerlemans and Knap, 1998, J. Glaciol.                          */
/*   or: Douville et al 1995.                                               */
/*   or: combination of Oerlemans and Knap, 1998, J. Glaciol. and           */
/*   Zuo and Oerlemans, 1996, J. Glaciol. taken from Bougamont et al 2005   */
/*   including a dependence on snowdepth following                          */
/*   Oerlemans and Knap, 1998  							    */
/*   including solar zenith angle dependency:                               */
/*   Segal et al., 1991 J. Atm. Sci., 48(8), 1024-1042.                     */
/****************************************************************************/
void albedosnowdepth();

/****************************************************************************/
/* FUNCTION  initgrid                                                       */
/*   create and initialises the grid for snow temperature calculation       */
/*   Possible snowlayer on top of ice or firn                               */
/*   maximum dept set with depthdeep                                        */
/*   thickness layer at depthgdeep set with thicknessdeep                   */
/*   depth positive downwards                                               */
/*   called from main once before time loop                                 */
/****************************************************************************/
void initgrid();

/****************************************************************************/
/* FUNCTION  changegrid                                                     */
/*   changes grid when layers become to thin or to thick                    */
/*   Possible snowlayer on top of ice or firn                               */
/*   maximum dept set with depthdeep                                        */
/*   depth positive downwards                                               */
/****************************************************************************/
void changegrid();

/****************************************************************************/
/* FUNCTION  resetgridwinter                                                     */
/* changes grid at end of mass balance year in order to make multiyear calculation of mb possible*/
/* layerid 1 is set to 2, snow becomes firn*/
/* SNOW set to 0*/
/****************************************************************************/
void resetgridwinter();

/****************************************************************************/
/* FUNCTION  resetgridsummer                                                     */
/* changes grid at start of summer mass balance season in order to make multiyear calculation of mb possible*/
/* SNOWinit set to SNOW*/
/****************************************************************************/
void resetgridsummer();

/****************************************************************************/
/* FUNCTION  iceconductivity                                                */
/*   this function calculates conductivity as a function of density         */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/
void iceconductivity(int i, int j, int k);

/****************************************************************************/
/* FUNCTION  densification                                                  */
/*   this function calculates densification  of a dry snowpack              */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/
void densification(int i, int j, int k);

/****************************************************************************/
/* FUNCTION  snowmelt                                                       */
/*   this function calculates melting of the snowpack if T > 0              */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/
void snowmelt(int i, int j, int k);

/****************************************************************************/
/* FUNCTION  refreezing                                                     */
/*   this function calculates refreezing of the melted water                */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/
void refreezing(int i, int j, int k);

/****************************************************************************/
/* FUNCTION  percolation                                                    */
/*   this function calculates percolation of the melted water               */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/
void percolation(int i, int j, int k);

/****************************************************************************/
/* FUNCTION  slushformation                                                 */
/*   this function calculates slush formation and surface water             */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/
void slushformation();

/****************************************************************************/
/* FUNCTION  irreducible_schneider                                          */
/*   this function calculates the irreducible water content according to    */
/*   measurements of Storglaciaren presented by Schneider and Jansson, 2004 */
/*   J. Glaciol., 50(168), 25-34                                            */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/
void irreducible_schneider(int i, int j, int k);

/****************************************************************************/
/* FUNCTION  irreducible_coleou                                             */
/*   this function calculates the irreducible water content according to    */
/*   Coleou and Lesaffre, 1998                                              */
/*   Ann. Glaciol., 26, 64-68                                               */
/*   called for each layer from subsurf() which is called for each grid cell*/
/****************************************************************************/
void irreducible_coleou(int i, int j, int k);

/****************************************************************************/
/* FUNCTION  subsurf                                                        */
/*   this function calculates snow temperature                              */
/*   called from main for each grid cell                                    */
/****************************************************************************/
void subsurf();

/****************************************************************************/
/* FUNCTION  interpolate                                                    */
/*   this function interpolates linearly the climate data for the subtimesteps */
/*   called from main for every subtimestep                                 */
/****************************************************************************/
void interpolate();

/****************************************************************************/
/* FUNCTION  outputsubsurf                                                  */
/*   this function prints out subsurf conditions for given location         */
/*   for each time step one row for each layer                              */
/*   called from main once before time step loop and in time loop for each grid cell */
/****************************************************************************/
void outputsubsurf();

/****************************************************************************/
/* FUNCTION  outputsubsurflines                                             */
/*   this function prints out subsurf conditions for given location         */
/*   for each time step one row for each layer                              */
/*   called from main once before time step loop and in time loop for each grid cell */
/****************************************************************************/
void outputsubsurflines();

#endif /* SNOWMODEL_H_ */
