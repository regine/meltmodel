/***********************************************************************
 * Copyright 2012 Carleen Tijm-Reijmer, Regine Hock, Lyman Gillispie
 * This file is part of MeltMod.
 * 
 * MeltMod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * MeltMod is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MeltMod.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/
/****************************************************************************/
/*  FILE  :  snowinput.h                                                    */
/*    subsurface model                                                      */
/* Authored 5/25/2012                                                       */
/****************************************************************************/

#ifndef MELT_MOD_SNOW_INPUT_H_
#define MELT_MOD_SNOW_INPUT_H_

/****************************************************************************/
/* FUNCTION  snowlayermsnow                                                 */
/*   this function calculates snow layer thickness in m of snow             */
/*  NOT initialisation, but for functions changegrid and stationoutput*/
/****************************************************************************/
void snowlayermsnow();

/****************************************************************************/
/* FUNCTION  meltlayermice                                                  */
/*   this function calculates thickness of melted layer in m ice        */
/*  NOT initialisation, but for functions changegrid and stationoutput*/
/****************************************************************************/
void meltlayermice();

/******************************************************************/
/* FUNCTION  choice_layertemperature                                           */
/*    called from initgrid in snowmodel.c     */
/*    choice to be set in variab.h */
/******************************************************************/
void choice_layertemperature();

/******************************************************************/
/* FUNCTION  choice_snowlayerdensity                                           */
/*    called from initgrid in snowmodel.c     */
/*    choice to be set in variab.h */
/******************************************************************/
void choice_snowlayerdensity();

/******************************************************************/
/* FUNCTION  choice_firnlayerdensity                              */
/*    called from initgrid in snowmodel.c                         */
/*    choice to be set in variab.h */
/******************************************************************/
void choice_firnlayerdensity();

/******************************************************************/
/* FUNCTION  choice_snowlayermsnow                                */
/*    called from initgrid in snowmodel.c                         */
/*    choice to be set in variab.h */
/*   options MUST correspond with options in choice_snowlayerdensity */
/******************************************************************/
void choice_snowlayermsnow();

/*******************************************************************************/
/* FUNCTION  defualt_layertemperature_3grad                                    */
/*   this function calculates a layer temperature                              */
/*   based on 3 temp gradients (1: between surface and first temp turning point*/
/*  2: between first and second turning point, 3: betwenn second turning point */
/*  and the depth at which the glacier is temperate                            */
/*  the depths at which the temperature gradient changes should be determined  */
/*  from data at simulation start or educated guesses                          */
/*******************************************************************************/
void default_layertemp3grad();

/****************************************************************************/
/* FUNCTION  default_layertemperature                                       */
/*   this function calculates a default layer temperature                   */
/*   based on annual averaged temperature and assumption of                 */
/*   temperate below 30 m, and spring start                                 */
/* FUNCTION NOT CALLED ANYWHERE BECAUSE DEFAULT REPLACED BY OTHER FUNCTION*/
/****************************************************************************/
void default_layertemperature();

/*============== SNOW LAYER DENSITY - 2 default options ====================================*/

/****************************************************************************/
/* FUNCTION  default_snowlayerdensity                                          */
/*   this function calculates a default snowlayer density   */
/*   using value for firn density from input.dat                        */
/*   called from function choice_snowlayerdensity()   */
/****************************************************************************/
void default_snowlayerdensity();

/****************************************************************************/
/* FUNCTION  default_snowlayerdensity3grad                                  */
/*   this function calculates snow layer density based on 3 gradients       */
/****************************************************************************/
void default_snowlayerdensity3grad();

/*============== FIRN LAYER DENSITY - 2 default options ======================================*/

/****************************************************************************/
/* FUNCTION  default_firnlayerdensity                                       */
/*   this function calculates a default firnlayer density                   */
/*   using value for firn density from input.dat                            */
/****************************************************************************/
void default_firnlayerdensity();

/****************************************************************************/
/* FUNCTION  default_firnlayerdensity1grad                                  */
/*   this function calculates a firnlayer density by a linear gradient      */
/*   called from choice_firnlayerdensity in snowinput.c                     */
/****************************************************************************/
void default_firnlayerdensity1grad();

/*============== CONVERSION SNOW LAYER DENSITY - 2 default options ======================*/
/*============== INVERTS DEFAULT SNOW LAYER DENSITY OPTIONS ======================*/

/****************************************************************************/
/* FUNCTION  default_snowlayermsnow                                         */
/*   this function calculates snow layer thickness in m of snow             */
/*   using value for firn density from input.dat                            */
/****************************************************************************/
void default_snowlayermsnow();

/****************************************************************************/
/* FUNCTION  default_snowlayermsnow3grad                                         */
/*   this function calculates snow layer thickness in m of snow             */
/*   using value for firn density from input.dat                            */
/****************************************************************************/
void default_snowlayermsnow3grad();

/*===========================================================================*/
/*===== USER FILES: ADD FUNCTIONS HERE FOR NEW APPLICATIONS =================*/
/*===========================================================================*/

/*===== FUNCTION TO INITIALIZE THE SNOW MODEL FOR STORGLACIAREN 1999 ===*/
/*denssnowwe[4] 0,1,2,3 (values given are for 1999 season, 0 allways empty) */
/*denscoeff[4] 0,1,2,3 (values given are for may 1999, 0 allways empty) NOT USE*/
/*tempcoeff[4] 0,1,2,3 (values given are for day 127, 1999, 24 h, 0 allways empty) */
/****************************************************************************/
/* FUNCTION  stor_layertemperature                                          */
/*   this function calculates layer temperature                             */
/*   based on measurements at Storglaciaren for may 1999                    */
/****************************************************************************/
void stor_layertemperature();

/****************************************************************************/
/* FUNCTION  stor_snowlayerdensity                                          */
/*   this function calculates snow layer density                            */
/*   based on density measurements at Storglaciaren                         */
/****************************************************************************/
void stor_snowlayerdensity();

/****************************************************************************/
/* FUNCTION  stor_firnlayerdensity                                          */
/*   this function calculates firn layer density                            */
/*   based on density measurements at Storglaciaren                         */
/****************************************************************************/
void stor_firnlayerdensity();

/****************************************************************************/
/* FUNCTION  stor_snowlayermsnow                                            */
/*   this function calculates snow layer thickness in m of snow             */
/*   based on density measurements at Storglaciaren                         */
/****************************************************************************/
void stor_snowlayermsnow();

/******************************************************************/
/* FUNCTION  choice_outputlines                                           */
/*    called from outputsubsurflines in snowinit.c     */
/*    choice to be set in variab.h */
/******************************************************************/
void choice_outputlines();

/******************************************************************/
/* FUNCTION  default_outputlines                                           */
/*    called from choice_outputlines in snowinit.c     */
/*    default is no output lines */
/******************************************************************/
void default_outputlines();

/******************************************************************/
/* FUNCTION  stor_outputlines                                           */
/*    called from choice_outputlines in snowinit.c     */
/*    default is no output lines */
/******************************************************************/
void stor_outputlines();



#endif /* SNOW_INPUT_H_ */
