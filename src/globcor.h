/********************************************************************************/
/*   FILE :  globcor.c                                                        ***/
/*           CALCULATE MEAN OVER ONE TIME STEP FOR ALL GRIDS OF DTM             */
/*              - SHADING                                                       */
/*              - CORRECTION FACTOR FOR DIRECT RADIATION                      ***/
/*              - DIRECT RADIATION ON SLOPED SURFACE                            */
/* Authored 5/25/2012 */
/********************************************************************************/

/* Functions of soloar geometry and shading-algorithm taken from
   WASim model by Joerg Schulla, Stand  1.9.1995, Version 1.0
   Verfahren  : nach T.R. Oke: Boudary Layer Climates, Appendix A:
        Radiation/Solar Geometry,
        Abschattung mit Algorithmus von Joerg:
        Strahlverfolgung mit optimiertem Rastersuchverfahren

 !!!!! MODIFIZIERUNG 9/1995   Regine Hock:   !!!!!!!!!!!!!!!
     -Umschreiben von C++ in C
     -Files teilmod.cc expokorr.cc und teilmod.h integriert in ein File
     -andere clear sky solar radiation calculation: hoehenabhaengig
     -Berechnung einer mittleren Beschattung fuer das Zeitintervall
        als Mittel aus den Subintervallen
     -Moeglichkeit der Ausgabe der Schattengrids der Subintervalle
     -different treatment of cosZ                                       */
/****************************************************************************/

#ifndef MELT_MOD_GLOBCOR_H_
#define MELT_MOD_GLOBCOR_H_


/*==============================================================*/
/*** FUNCTION jd2monat                                        ***/
/*            CONVERT JULIAN DAY TO MONTH AND DAY               */
/*      leap years neglected !!!                                */
/*==============================================================*/
void jd2monat();

/*=======================================================================*/
/************** DEKLINATION **********************************************/
/*=======================================================================*/
double deklination(int tag, int monat);

/*=======================================================================*/
/************** REALE SONNENZEIT *****************************************/
/*=======================================================================*/
double gettimeequation(int tag,int monat);

/*=======================================================================*/
/************** SONNENHOEHE     ******************************************/
/*=======================================================================*/
double sonnenhoehe(int tag,int monat,double breite,double laenge,double stunde);

/*=======================================================================*/
/************** SUN COORDINATES          *********************************/
/*=======================================================================*/
void getsuncoordinates(int tag,int monat,double breite,double laenge,double stunde,
                       double *sunazimuth,double *sunaltitude);

/*=======================================================================*/
/************* ANGLE SUN - EBENENNORMALE  ********************************/
/*=======================================================================*/
double getcosincidence(double aspect,double slope,double azimuth,double zenith);

/*=======================================================================*/
/******      SUNRISE      ************************************************/
/*=======================================================================*/
void sunrise(int tag,int monat,double breite,double laenge,double *sr,double *ss);

/*=======================================================================*/
/******      RADIUSSUNEARTH      *****************************************/
/*   calculates the ratio of mean distance between sun and earth to the  */
/*   the distance at a specific day                                      */
/*     from climatology lecture: Ohmura, p.33                            */
/* new: 15.1.1997 */
/*=======================================================================*/
void radiussunearth();

/* ------------- HAUPTFUNKTION -- called in main -------------------------- */
void schatten();

/*===========================================================================*/
/* FUNCTION getzenith
       - calculating zenith angle for the middle of time interval,
	 because it is needed in radiat.c (ratio glob/direct)                */
/*===========================================================================*/
void getzenith();

/*===========================================================================*/
/*  FUNCTION topofatmosphere
      calculation of solar radiation at top of atmosphere
      needed to separate diffuse radiation from global radiation             */
/*           called from main,    1/1997, updated 5/2006                     */
/*===========================================================================*/
void topofatmosphere();

/*===========================================================================*/
/*  FUNCTION readdirect
      reading direct radiation (slope corrected) from existing files)
      instead of calculating it by function schatten (is less time consuming)
       - creating file names and reading data from files
	     called from main,    9/1996                                     */
/*===========================================================================*/
void readdirect();



#endif /* GLOBCOR_H_ */
