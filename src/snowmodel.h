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
/* FUNCTION  surftempfrommodel                                                */
/*   this function calculates surface temperature of glacier by linear      */
/*   extrapolating temperature of upper two layers                          */
/****************************************************************************/
void surftempfrommodel();

/****************************************************************************/
/* FUNCTION  surftempskin                                                   */
/*   this function calculates surface temperature of glacier by             */
/*   using a skinlayer formulation                                          */
/*   a skin layer is a surface layer without heat capacity                  */
/*   the skinlayer temp is calculated by writing the surface EB in terms of  */
/*   it and deriving a formultion of Tskin                                  */
/*   longin and longout from previous timestep                              */
/****************************************************************************/
void surftempskin();

/****************************************************************************/
/* FUNCTION bisection (x1,x2,x_acc)  */
/****************************************************************************/
/* This function determines the surface temperature required to close the energy balance */
/****************************************************************************/
void bisection();

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

/*==============FUNCTION THAT WOULD BELONG TO TURBUL.C ====================== */

/****************************************************************************/
/* FUNCTION  turbfluxes                                                         */
/*   this function calculates the turbulent fluxes of heat and moisture     */
/****************************************************************************/
void turbfluxes();

/***************************************************************************/
/* FUNCTION   roughnesslengthAndreasmodified                               */
/* CALCULATION OF ROUGHNESS LENGTHS ACCORDING TO Andreas 1997              */
/*                      surface renewal theory, see Munro 1990             */
/*             modified as by Smeets et al 2006 in prep                    */
/*  function of Reynolds number, then assumed constant for entire glacier  */
/*  function called for climate station grid from function iterationstation */
/***************************************************************************/
void roughnesslengthAndreasmodified();

/****************************************************************************/
/* FUNCTION  spechum                                                        */
/*   this function calculates the specific humidity at surface and          */
/*   at observation level. At surface it is the saturation value            */
/****************************************************************************/
void spechum();

/***************************************************************************/
/* FUNCTION   stable                                                       */
/* CALCULATION OF STABILITY FUNCTIONS FOR STABLE CASE                      */
/*   stability function (stable) according to Beljaar and Holtslag, 1991   */
/***************************************************************************/
void stable();

/***************************************************************************/
/* FUNCTION   unstable                                                     */
/* CALCULATION OF STABILITY FUNCTION FOR UNSTABLE CASE                     */
/*                     according to Panofsky and Dutton, 1984              */
/***************************************************************************/
void unstable();

/***************************************************************************/
/* FUNCTION   sensiblenew                                                  */
/* CALCULATION OF SENSIBLE HEAT HEAT                                       */
/***************************************************************************/
void sensiblenew();

/***************************************************************************/
/* FUNCTION   latentnew                                                    */
/* CALCULATION OF LATENT HEAT HEAT                                         */
/***************************************************************************/
void latentnew();

/***************************************************************************/
/* FUNCTION   tsurfenergybalance                                                    */
/* calculates the energy balance as a function of the surface temperature (K)      */
/***************************************************************************/
void tsurfenergybalance(double tskin);

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
