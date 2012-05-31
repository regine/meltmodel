/********************************************************************************/
/*   FILE :  globcor.c                                                        ***/
/*           CALCULATE MEAN OVER ONE TIME STEP FOR ALL GRIDS OF DTM             */
/*              - SHADING                                                       */
/*              - CORRECTION FACTOR FOR DIRECT RADIATION                      ***/
/*              - DIRECT RADIATION ON SLOPED SURFACE                            */
/* 6.2.97,  last update 17/11/2006 */
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


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "globcor.h"
#include "initial.h"

#include "variabex.h"      /* all global VARIABLES */



/*==============================================================*/
/*** FUNCTION jd2monat                                        ***/
/*            CONVERT JULIAN DAY TO MONTH AND DAY               */
/*      leap years neglected !!!                                */
/*==============================================================*/

void jd2monat()

 {    
    if ((int)jd <=31)     { monat=1;  tag=jd;      return; }
    if ((int)jd <=59)     { monat=2;  tag=jd-31;   return; }
    if ((int)jd <=90)     { monat=3;  tag=jd-60;   return; }
    if ((int)jd <=120)    { monat=4;  tag=jd-90;   return; }
    if ((int)jd <=151)    { monat=5;  tag=jd-120;  return; }
    if ((int)jd <=181)    { monat=6;  tag=jd-151;  return; }
    if ((int)jd <=212)    { monat=7;  tag=jd-181;  return; }
    if ((int)jd <=243)    { monat=8;  tag=jd-212;  return; } 
    if ((int)jd <=273)    { monat=9;  tag=jd-243;  return; }
    if ((int)jd <=304)    { monat=10;  tag=jd-273;  return; }
    if ((int)jd <=334)    { monat=11;  tag=jd-304;  return; }
       else 
	 {   monat=12;  
	     tag=jd-334; 
	 }

    return;
 }  


/*=======================================================================*/
/************** DEKLINATION **********************************************/
/*=======================================================================*/

/*  Maybe could be made inline? Does it get clled a lot?*/
double deklination(long tag, long monat)
{
  return -23.4 * cos( (jd+10)/58.09155423); /* 58.09 = 2*pi/365  */
      /*add 10 days, because the minimum is Dec. 21*/
}


/*=======================================================================*/
/************** REALE SONNENZEIT *****************************************/
/*=======================================================================*/

double gettimeequation(long tag,long monat)
/* gibt die Anzahl Stunden an, die zur aktuellen mittleren Sonnenzeit
   hinzuaddiert werden muessen, um auf die reale Sonnenzeit zu kommen */
{
   return (double)(mon[(int)monat]-tag+1)/(double)mon[(int)monat]*(timeequation[(int)monat]/60.0) +
	  (double)(tag-1)/(double)mon[(int)monat]*(timeequation[(int)monat+1]/60.0);
    /* einfache lineare Interpolation */
}


/*=======================================================================*/
/************** SONNENHOEHE     ******************************************/
/*=======================================================================*/

double sonnenhoehe(long tag,long monat,double breite,double laenge,double stunde)
{
  timeeq = gettimeequation(tag,monat);    /*!!!!!!!!!!!!!!*/
/*Deklination d. Sonne gegen d. Aequatorialebene haengt nur vom Tag ab (1.Naeherung)*/
  dekli  = deklination(tag,monat);        /*!!!!!!!!!!!!!!*/

  stunde = stunde-(reflongitude-laenge)/15+timeeq;
  stuwi  = 15*(12-stunde)/180*pi;
  cosz=sin(breite*pi/180)*sin(dekli*pi/180)+cos(breite*pi/180)*cos(dekli*pi/180)*cos(stuwi);
  sonnenstand = acos(cosz)*180/pi;
  if (cosz<0) sonnenstand = sonnenstand-90; else sonnenstand = 90-sonnenstand;
  return sonnenstand;
}


/*=======================================================================*/
/************** SUN COORDINATES          *********************************/
/*=======================================================================*/

void getsuncoordinates(long tag,long monat,double breite,double laenge,double stunde,
		       double *sunazimuth,double *sunaltitude)

   /* liefert bei Eingabe der geographischen Koordinaten in Grad und des Tages, Monats und
      der Stunde die Koordinaten der Sonne azimuth und altitude zurueck (in RADIANT!) */
{
  timeeq = gettimeequation(tag,monat);    /*!!!!!!!!!!!!!!*/
  dekli  = deklination(tag,monat);        /*!!!!!!!!!!!!!!*/
  stunde = stunde-(reflongitude-laenge)/15+timeeq;  /*time correct. wegen Lage weg vom Zeitmeridian*/
  stuwi  = (12-stunde)/12 * pi;
  breite *= pi/180;      /*convert Grad to Rad*/
  dekli  *= pi/180;
  cosz   = sin(breite)*sin(dekli)+cos(breite)*cos(dekli)*cos(stuwi);   /*cosZenithangle*/
 /*cos azimuth,    sqrt(1-cosz cosz) = sinz*/
  cosOM  = (sin(dekli)*cos(breite) - cos(dekli)*sin(breite)*cos(stuwi))/sqrt(1-cosz*cosz);
  *sunaltitude = acos(cosz); /* sunaltitude ist eigentlich der zenithwinkel! */
  if (*sunaltitude <= pi/2) *sunaltitude = pi/2 - *sunaltitude; 
	    /* Sonnenstand ueber Horizont */
  else   *sunaltitude = -(*sunaltitude - pi/2); /* Sonnenstand unter Horizont */
  *sunazimuth = acos(cosOM);
  if (stunde > 12) *sunazimuth = 2*pi - *sunazimuth;
  return;
}


/*=======================================================================*/
/************* ANGLE SUN - EBENENNORMALE  ********************************/
/*=======================================================================*/

double getcosincidence(double aspect,double slope,double azimuth,double zenith)

  /* berechnet den cosinus des WInkels zwischen Sonnenvektor und Ebenennormale
     alle Angaben in Radiant, auch die Eingabedaten! */
{
 return cos(slope)*cos(zenith) + sin(slope)*sin(zenith)*cos(azimuth - aspect);
}


/*=======================================================================*/
/******      SUNRISE      ************************************************/
/*=======================================================================*/

void sunrise(long tag,long monat,double breite,double laenge,double *sr,double *ss)

{
 double z,dekli,timeeq;
/* neu */

  timeeq = gettimeequation(tag,monat);   /*!!!!!!!!!!!!!!*/
  dekli  = deklination(tag,monat);       /*!!!!!!!!!!!!!!*/
  breite = fmod(breite,360.0);     /*neu 22.7.97*/
  laenge = fmod(laenge,360.0);     /*neu 22.7.97*/

  z = sin(breite*pi/180)/cos(breite*pi/180)*sin(dekli*pi/180)/cos(dekli*pi/180);
  if (fabs(z)<=1)
  { /* da es ansonsten keinen Sonnenuntergang gibt! */

    if (z!=0) *sr = 12/pi * acos(z); else *sr = 6.0;
    *ss = 24 - *sr;
    *sr = *sr + (reflongitude-laenge)/15-timeeq;
    *ss = *ss + (reflongitude-laenge)/15-timeeq;
  }
  else
  {
     *ss = -1; 
/*    *ss = 24.1;   vor 21.1.97 */
    *sr = -1.0;
  }
  return;
}



/*=======================================================================*/
/******      RADIUSSUNEARTH      *****************************************/
/*   calculates the ratio of mean distance between sun and earth to the  */
/*   the distance at a specific day                                      */
/*     from climatology lecture: Ohmura, p.33                            */
/* new: 15.1.1997 */
/*=======================================================================*/

void radiussunearth()
{
  float teta;

   teta = (2*pi*jd)/365;
   radius2sun = 1.000110+0.034221*cos(teta)+0.001280*sin(teta)+
		0.000719*cos(2*teta)+0.000077*sin(2*teta);

 return;
}


/* ------------- HAUPTFUNKTION -- called in main -------------------------- */

void schatten()

{ 
  float  z;       /*elevation*/
  int    *indexx,*indexy;
  float  faktorstationhoriz = 0;  /*for correction factor assuming the climate station */
				  /* grid to be horizontal */
  float  slopehoriz = 0;     /* slope of the climate station for horizontal calculation*/

 /*** $$$  NEU OPTION : SCHATTENGRIDS DER SUBINTERVALLE AUF FILE SCHREIBEN 1=yes ***/
   int    subintfileyes=0;        /*** $$$ **********************/
     /*zum steuern, ob alle Schattengrids aller Subintervalle auf ein File
       geschrieben werden sollen, Name: sub__.__ mit Zeit als real*/

   FILE   *shadesubint=NULL;
   char   namesubint[21];
   float  faktorstation;  /*NEU NUR KONTROLL-SCREEN-OUTPUT $$$ */

  exkorstationhoriz  = 0.0;
  directstationhoriz = 0.0;  /*must be set to 0 every timestep before subinterval loop*/

  timekorr = 0.;        /*wenn Messdaten andere Zeit als MEZ not 0*/
  InV=timestep*60;      /*Intervallaenge in Minuten*/
  se = zeit;        
    /*if time step=1 h, time=8, split=4 calculation at: 7h7.5min 7h22.5min etc*/


  jd2monat();           /*CONVERT JULIAN DAY TO MONTH AND DAY*/

 /* Initialisieren des Arrays fuer die Abweichung der wahren Sonnenzeit von der Ortszeit
     in Abhaengigkeit vom Monat */
 /* gibt die Verschiebung der realen Sonnenzeit gegenueber der
	 mittleren Sonnenzeit in min an (Elliptische Bahn) das Array steht
	 an jeder Seite um eis ueber, d.h., es kann auch mit monat-1
	 bzw. monat+1 gerechnet werden*/

  /* LG: timeequation gets re-initialized everytime schatten() or getzenith is called
   * could we just fix in in place as a global const somewhere? It already lives in the global
   * namespace, and we could  save some cycles by not re-initilizing the values every time
   */

  timeequation[0]  =  11.3;
  timeequation[1]  =  -3.2;     /*Januar*/
  timeequation[2]  = -13.6;
  timeequation[3]  = -12.6;
  timeequation[4]  =  -4.2;
  timeequation[5]  =   2.8;
  timeequation[6]  =   2.5;
  timeequation[7]  =  -3.5;
  timeequation[8]  =  -6.3;
  timeequation[9]  =  -0.3;
  timeequation[10] =  10.0;
  timeequation[11] =  16.4;
  timeequation[12] =  11.3;
  timeequation[13] =  -3.2;


 /* Suchen der groessten Hoehe, um den Schattensuchalgorithmus abbrechen zu koennen,
     wenn die Gerade ueber hmax steigt, und immer noch kein Schatten berechnet wurde */

 if (hmaxcalculated == 0)     /*nur einmal berechnen - aendert sich nicht*/
  {
    hmax=-1e10;
     for (i=1;i<=nrows;i++)
     for (j=1;j<=ncols;j++)
	if (griddgm[i][j]>hmax)   hmax=griddgm[i][j];

    hmaxcalculated = 1;
  }  

       
/* Ausfuehren der expositions-, gefaelle- und 
   abschattungsabhaengigen Korrektur fuer die direkte Sonnenstrahlung */

  cn=1;
  gewicht=1.0/(float)split;

 /* es werden jeweils split Teilintervalle gebildet. Fuer jedes dieser           
      Teilintervalle wird die Mittelzeit genommen, fuer diese Mittelzeit wird dann 
      die Expositionskorrektur gerechnet. Der Mittelwert aus allen Berechnungen 
      wird als repraesentativ fuer das Intervall angenommen. Es werden nur Intervalle
      beruecksichtigt, an denen die Sonne ueber dem Horizont steht.          
      Bei einem Zeitintervall von 24 Stunden kann split z.B. 24 sein, es werden dann 
      nach jeweils count Rechenaufrufen (also count Tagen) fuer alle 24 Stunden
      die Korrekturwerte ermittelt un dann ein repraesentetiver Tageswert errechnet. */

 /*-----------------------------------------*/
  /* Berechnen von sunazimuthangle, sunaltitudeangle, sunzenithangle fuer diese Stunde
     Angaben der Ergebnisse in rad, der Eingangsdaten aber in Grad! */

  /* SUNRISE AND SUNSET FOR THE DAY */
  sunrise(tag,monat,breite,laenge,&sunr,&suns);      /*!!!!!!!!!!*/

  /* Nullsetzen der Korrekturfaktoren erfolgt explizit, da die Zuweisung in der 
     Schleife zur Korrekturfaktorermittlung incrementell erfolt (es wird dann fuer 
     jedes Teilintervall eine um den Korrekturfaktor fuer dieses Zeitintervall korrigierte
     Strahlung addiert, so dass zum Intervallende genau <split> Teilintervall-Ergebnisse
     addiert sind. Der Wert in EXKORR entspricht dann dem Korrigierten Stundenwert der 
     Strahlung (Korrekturfaktoren mit der astronomisch und topographisch moeglichen 
     Strahlung gewichtet). Ganz am Ende wird dann durch eine separat ermittelte 
     unkrrigierte Intervallsumme der STrahlung geteilt, um so den effektiven 
     Korrekturfaktor zu erhalten */

  for (i=1;i<=nrows;i++)       /*CORRECTION FACTOR GRID INITIALISIEREN AUF NULL*/
    for (j=1;j<=ncols;j++)        
      {  EXKORR[i][j]=0.0; 
	 SHADE[i][j]=0.0;      /*must be zero, because summing up of SHAPE*/
	 Isenk[i][j]=0.0;      /*clear sky solar radiation, momentanwert*/
	 strlsumme[i][j]=0.0;  /*Isenk*cosZ summed up (refers to horizontal surface)*/
		      /*clear sky solar radiation Wh/m2: mean for time step interval*/
	 DIRECT[i][j]=0.0;
      }               

  /*SHADE = mean shade value for timestep, d.h. sum up 0 or 1 for each subinterval,
    divide by split for each time step*/

  /* Da fuer mehrere Intervalle die Strahlung unterschiedlich gross sein kann,
     muessen die Korrekturfaktoren mit den jeweils astronomisch moegl. Strahlungen
     gewichtet werden. Es wird also das Integral der korrigierten maximal
     moeglichen Strahlungen errechnet, anschliessend durch das Integral der
     unkorrigierten maximal moeglichen Strahlung geteilt (me) und so
     der mittlere Korrekturfaktor erhalten. */

  /* Teilintervall am Anfang der Integration (Mitte des 1. der <split> Teilintervalle) */
	spa = se-InV/60.0*(1-0.5/(float)split)+timekorr;

  /* Teilintervall am Ende der Integration (Mitte des letzten der <split> Teilintervalle*/
	spe = se+0.01+timekorr;


/* ------------------------------------------------------------------------------------ */
/******* ZEIT-SCHLEIFE : UEBER ALLE ZEITUNTERINTERVALLE (je nach SPLIT) BERECHNUNG VON
			     clear-sky direct radiation Isenk = height-dependent
			     ABSCHATTUNG und CORRECTION FACTOR  ******/

  for (sp=(float)spa;sp<(float)spe;sp+=gewicht*(InV/60.0))

  /* sp ist die Stundennummer (Bruchteile erlaubt und noetig) des aktuellen Teilintervalles,
     korrigiert mit der Zeitverschiebung der mittleren gegen die wahre Sonnenzeit;
     so ist spa bei einem Zeitintervall von 2 Stunden und split=4 (jede halbe Stunde neu 
     berechnen) nach obiger Berechnung fuer se=10 (die an die Routine uebergebene 
     Stundennummer [fuer das Ende des Intervalls]) 
     10 - 120/60 * (1 - 0.5/4) + korr = 10 - 2 * (1-0.25) = 10 - 2 + 0.5 = 8.5.
     Das erste Teiintervall ist also 0.5 Stunden lang und endet um 8:30.
     Als Increment wird jeweils die mit 1/<split> gewichtete Laenge eines 
     Intervalles in Stunden (hier 0.25*120/60 = 0.5 addiert */

  {

/*** $$$ NEU KONTROLL-OUTPUT, SCHATTEN DER SUBINTERVALLE */
 if (subintfileyes == 1)
   { 
      sprintf(namesubint,"sun%4.2f",sp);
      strcpy(dummy,outpath);
      strcat(dummy,namesubint);
   
      if ((shadesubint = fopen(dummy,"wb")) == NULL)
	{ printf("\n ERROR in opening shade file (subintervals)");
	  printf("\n       file globcor.c\n\n");
	  exit(10);
	}
   } /*endif*/

/*** $$$ ***********/

    /* wenn der Beginn des aktuellen Intervalls vor Sonnenaufgang oder 
       das Ende des aktuellen Berechnungsintervalles nach
       Sonnenuntergang liegt, wird keine Strahlung beruecksichtigt 
       (nur vollstaendig "besonnte" Intervalle werden beruecksichtigt) */
    /* fuer alle Grids gleich */

/* neu Modifikation vom 21.1.1997 in shading.c hier 22.7.1997 eingebaut, 
   da Fehler in bestimmten Faellen noerdlich vom Polarkreis: */
    split_temp = InV/60.0/(float)split; /* Dauer eines Teilintervalles in h */
    inta = sp-split_temp/2.0;
    inte = sp+split_temp/2.0;
    sunr = fmod(sunr+24.0,24.0); /* Sonnenaufgang zwischen 0 ... 24 Uhr fixieren */
    suns = fmod(suns+24.0,24.0); /* Sonnenuntergang ebenfalls fixieren */
    /* Wie lange ist es am Anfang eines Teilintervalls noch dunkel und am Ende schon dunkel? */

    /* dark_a */
    if (sunr<inta)
      dark_a = 0.0;
    else
      if (sunr<inte)
        dark_a = sunr - inta;
      else /* Stunde komplett vor Sonnenaufgang */
        dark_a = 0.0;

    /* dark_e */
    if (suns>sunr)
    { /* der Normalfall */
      if ((suns>inte)&&(sunr<inte))
        dark_e = 0.0;
      else
        if ((suns>inta)&&(suns<inte))
          dark_e = inte - suns;
        else
          dark_e = split_temp;
    }
    else
    { /* der Fall, dass die Sonne erst nach Mitternacht gesetzl. Zeit untergeht */
      if (suns>inte)
        dark_e = 0.0;
      else
        if (suns>inta)
          dark_e = inte - suns;
        else
          dark_e = sunr > inte ? split_temp : 0.0;
    }

    Anteil_dunkel = (dark_a + dark_e)/split_temp;
    /* Wenn die Sonne im Intervall erst unter, dann aufgeht, muss zum Anfang und Ende
       des Intervalls das Komplement zu 1 von dark_a und dark_e gebildet und addiert werden
       Anteil_hell=(1-dark_a)+(1-dark_e) = 2-(dark_a+dark_e) = 2.0 - Anteil_dunkel
       was f’r Anteil_dunkel zu Anteil_dunkel = 1-Anteil_hell = 1 - 2 + Anteil_hell = 
       Anteil_dunkel-1 fuehrt */

    if (Anteil_dunkel>1.0) Anteil_dunkel--;

    /* wenn die Sonne nur ganz kurz auftaucht, nuetzt es gar nichts!
       Da kann sie auch gleich untergetaucht bleiben */
    if (Anteil_dunkel > 0.95) Anteil_dunkel=1.0;
    Anteil_hell   = 1.0 - Anteil_dunkel;

    if ((Anteil_hell>0.5)&&((dark_a+dark_e)<=1))
/*    if ( (sp-InV/60.0*0.5/(float)split <= sunr) || (sp > suns) ) nostrl = 1 */
      nostrl = 0;    /*day*/ 
    else
      nostrl = 1;    /*night*/
       
/*---------------------------------------------------------------------------------*/
/*      Berechnung der clear-sky solar radiation   for all grids                   */
/*---------------------------------------------------------------------------------*/
 
  if (!nostrl)             /*Sonne aufgegangen ueber horizontaler Ebene*/
    {    
      /* wo steht die Sonne? */
      getsuncoordinates(tag,monat,breite,laenge,sp,&sunazimuthangle,&sunaltitudeangle);
      sunzenithangle = pi/2 - sunaltitudeangle;

      radiussunearth();    /*ratio of mean radius to earth to tru radius to earth*/

  /*NEU : Isenk hoehenabhaengig -> daher Berechnung fuer jedes Grid*/    
    for (i=1;i<=nrows;i++)        
     for (j=1;j<=ncols;j++)
     {
      if (sunaltitudeangle>0.017) /* 0.017=1 Grad */
       {
	/* Berechnung der bei Wolkenfreiem Himmel und klarer Atmosphaere auf einen
	   waagerechten Quadratmeter auftreffender Strahlungsenergie [Isenk] = Wh/m^2 */
	/* cos(sunazimuthangle) wirkt hier auf den Extinktionskoeffizienten!  */
	 
	   /*------------------------------------------------*/
		p = exp(-0.0001184*griddgm[i][j]) * P0;           /* air pressure */
	   /*------------------------------------------------*/

	   /* Formel aus Oke S.345  transmissivity read from input.dat */
	   /*gewicht=1.0/(float)split,  InV=Intervalllaenge in Minuten=timestep*60 */
 /*-!!!------------------------------------------------------------------------!!!-*/
	Isenk[i][j] = gewicht*InV*1368.0*radius2sun/60.0 * 
		      pow(trans,(p/P0)/cos(sunzenithangle));
 /*-!!!------------------------------------------------------------------------!!!-*/
 /** beim Teilen durch cos(sunzenithangle) : braucht nicht begrenzt werden **/

       }  /*endif*/
       else Isenk[i][j]=0.0; /* no direct radiation at night */

       strlsumme[i][j] += (Isenk[i][j]*cos(sunzenithangle));  
	     /*direct rad uncorrected over timeinterval referred to horizontal surface*/

      }  /*endfor  next grid*/

    }  /*endif  !nostrl*/

/*---------------------------------------------------------------------------------*/
/*      Berechnung der Abschattung   for all grids for one point of time           */
/*         algorithm by Joerg Schulla                                              */
/*---------------------------------------------------------------------------------*/
 
   for (i=1;i<=nrows;i++)      
    for (j=1;j<=ncols;j++)
      {  if (nostrl == 1)     /*before sunrise over horizontal surface (night) */      
	  SHAPE[i][j]=1.0;      /*must be 1, because no shade calculation before sunrise*/
	 else                 /*after sunrise -> shade calculation below*/
	  SHAPE[i][j]=0.0;      /*must be 0 if algorithm below used*/
      }


if (shadefromfile != 1)          /*shading calculated here*/
 {
    if (!nostrl)          /*Sonne aufgegangen ueber horizontaler Ebene*/
    {
      /* Vorzeichen der Geradenanstiege in x- und y-Richtung */
      if (sunazimuthangle<=pi/2.0)
      {
	 vx = 1;
	 vy = 1;
      }
      else
      {
	if (sunazimuthangle<=pi)
	{
	  vx = 1;
	  vy =-1;
	}
	else
	{
	  if (sunazimuthangle<=pi*1.5)
	    {
	      vx =-1;
	      vy =-1;
	    }
	    else
	    {
	      vx =-1;
	      vy = 1;
	    }
	  }
	}

	 /* in x Richtung kleiner 45 Grad Neigung */
      if (fabs(cos(sunazimuthangle))<= (float)0.707106781)
       /* x-schritt ist immer 1, y-schritt wird sukzessive nachgefuehrt */
      {
	dy=(int)fabs((ncols*cos(sunazimuthangle)/sin(sunazimuthangle)));
	dx=ncols;

	dz=fabs((float)cs/sin(sunazimuthangle))*tan(sunaltitudeangle);
	indexx = (int*)calloc(ncols+1,sizeof(int));
	indexy = (int*)calloc(ncols+1,sizeof(int));

	if (vx>0)
	/* von links nach rechts */
	{
	  /* alle Punkte im Grid durchlaufen i=1..nrows,j=1..ncols */
	  for (i=1;i<=nrows;i++)
	  {
	    for (j=1;j<=ncols;j++)
	    {
		fx=dx/2;
		int ii=i;
		int jj=j;
		z=griddgm[i][j];
		k=1;
		indexy[k]=ii;
		indexx[k]=jj;
		while ((ii<nrows)&&(ii>1)&&(jj<ncols)&&(z<hmax)&&(SHAPE[i][j]<0.5))
		{
		  z+=dz;
		  jj+=vx;
		  fx-=dy;
		  if (fx<=0)
		  {
		    fx+=dx;
		    ii-=vy;
		  }
		  k++;
		  indexy[k]=ii;
		  indexx[k]=jj;
		  if (griddgm[ii][jj]>z)
		  {
		    for (m=1;m<k;m++)
		    SHAPE[indexy[m]][indexx[m]]=1.0;
		  }
	      }                 
	    }
	  }
	}
	else
	/* von rechts nach links */
	{
	  /* alle Punkte im Grid durchlaufen i=1..nrows,j=ncols..1 */
	  for (i=1;i<=nrows;i++)
	  {
	    for (j=ncols;j>=1;j--)
	    {             
		fx=dx/2;
		int ii=i;
		int jj=j;
		z=griddgm[i][j];
		k=1;
		indexy[k]=ii;
		indexx[k]=jj;
		while ((ii<nrows)&&(ii>1)&&(jj>1)&&(z<hmax)&&(SHAPE[i][j]<0.5))
		{
		  z+=dz;
		  jj+=vx;
		  fx-=dy;
		  if (fx<=0)
		  {
		    fx+=dx;
		    ii-=vy;
		  }
		  k++;
		  indexy[k]=ii;
		  indexx[k]=jj;
		  if (griddgm[ii][jj]>z)
		  {
		    for (m=1;m<k;m++)
		    SHAPE[indexy[m]][indexx[m]]=1.0;
		  }
		}              
	    }
	  }
	}
	free(indexx);
	free(indexy);
      }
      else
      /* y-schritt ist immer 1, x-schritt wird nachgefuehrt */
      {
	dx=(int)fabs(nrows*sin(sunazimuthangle)/cos(sunazimuthangle));
	dy=nrows;
	dz=fabs((float)cs/cos(sunazimuthangle))*tan(sunaltitudeangle);
	indexx = (int*)calloc(ncols+1,sizeof(int));
	indexy = (int*)calloc(ncols+1,sizeof(int));

	if (vy>0)
      /* von unten nach oben */
	{
	  /* alle Punkte im Grid durchlaufen i=nrows..1,j=1..ncols */
	  for (i=nrows;i>=1;i--)
	  {
	    for (j=1;j<=ncols;j++)
	    {
		fy=dy/2;
		int ii=i;
		int jj=j;
		z=griddgm[i][j];
		k=1;
		indexy[k]=ii;
		indexx[k]=jj;
		while ((ii>1)&&(jj>1)&&(jj<ncols)&&(z<hmax)&&(SHAPE[i][j]<0.5))
		{
		  z+=dz;
		  ii-=vy;
		  fy-=dx;
		  if (fy<=0)
		  {
		    fy+=dy;
		    jj+=vx;
		  }
		  k++;
		  indexy[k]=ii;
		  indexx[k]=jj;
		  if (griddgm[ii][jj]>z)
		  {
		    for (m=1;m<k;m++)
		    SHAPE[indexy[m]][indexx[m]]=1.0;
		  }
		}
	    }
	  }
	}
	else
	/* von oben nach unten */
	{
	  /* alle Punkte im Grid durchlaufen i=1..nrows,j=1..ncols */
	  for (i=1;i<=nrows;i++)
	  {
	    for (j=1;j<=ncols;j++)
	    {
			      fy=dy/2;
		int ii=i;
		int jj=j;
		z=griddgm[i][j];
		k=1;
		indexy[k]=ii;
		indexx[k]=jj;
		while ((ii<nrows)&&(jj>1)&&(jj<ncols)&&(z<hmax)&&(SHAPE[i][j]<0.5))
		{
		  z+=dz;
		  ii-=vy;
		  fy-=dx;
		  if (fy<=0)
		  {
		    fy+=dy;
		    jj+=vx;
		  }
		  k++;
		  indexy[k]=ii;
		  indexx[k]=jj;
		  if (griddgm[ii][jj]>z)
		  {
		    for (m=1;m<k;m++)
		    SHAPE[indexy[m]][indexx[m]]=1.0;
		  }
		}              
	    }
	  }
	}
	free(indexx);
	free(indexy);

      }
    }

}   /*endif shadefromfile ==1 */

else     /*shading read from existing files*/
{  
 strcpy(dummy,pathdirectfile);
 sprintf(nameshade,"sha%03d%02d.bin",(int)jd,(int)zeit);
 strcat(dummy,nameshade);
 if ((inshade = fopen (dummy,"rb")) == NULL)  {
       printf("\n shade file not found \n\n");
	exit(4);
    }  /*ENDIF*/

 if ( (fread(&(x[1]),sizeof(float),12,inshade)) !=12 )  {            /*FIRST 12 ROWS*/
       printf("\n ERROR in file %s \n",nameshade);
       exit(9);  
 }
 if ((fread(&(SHAPE[1][1]),sizeof(float),ncols*nrows,inshade)) != ncols*nrows)  {
     printf("\n ERROR in reading shade file %s  jd=%4.1f\n",nameshade,jd);
     exit(10);
 } 
  closefile(&inshade,nameshade);

}  /*end else  shading from files*/

/*------------------------------------------------------------------------------------*/
/*   expositions- und neigungsbedingte Strahlunsgkorrektur    still same subinterval  */
/*------------------------------------------------------------------------------------*/

    for (i=1;i<=nrows;i++)   /* ueber alle Reihen */
      for (j=1;j<=ncols;j++) /* Spalten des Grids */
      {
	SHADE[i][j] += (SHAPE[i][j]);  /*fuer Mittelwertberechnung des ganzen time steps*/
       
/*Aufsummieren des Schattenergebnisses fuer das split subinterval (=SHAPE ist 0 oder 1), 
  nach Ende aller split Teilintervalle teilen durch die Anzahl Teilintervalle => gibt
  mittlere Beschattung waehrend des Zeitintervalls 0 oder 1 oder ein Wert zw. 0 und 1, 
  d.h dann innerhalb dieses time steps ist die Sonne unter oder aufgegangen. */

	if ((!nostrl)&&(SHAPE[i][j]<0.5))      /*AFTER SUNRISE AND NO SHADE*/
	 {
	      /*faktor=cos of angle of incidence*/
	   faktor = getcosincidence(ASP[i][j]*pi/180.0,SLOPE[i][j]*pi/180.0,
		    sunazimuthangle,sunzenithangle);

	   if (faktor<=0)
	     faktor=0.0;

/*if((i==54) && (j==104))
  printf("\n   costheta=%8.5f  Z=%10.4f  cosZ=%10.5f   exk=%5.2f ",    
     faktor,sunzenithangle,cos(sunzenithangle),
     faktor/cos(sunzenithangle));  */

      /*Begrenzung ? ==> faktor/cos(sunzenithangle) > 5. => faktor = 5*cosZ */
	   if(faktor/cos(sunzenithangle) > 5.)   
	     { faktor=5*cos(sunzenithangle);

	       /* if((i==54) && (j==104))
		     printf("\n     groesser5 costheta 5.2%",faktor); */

	     } 
	 }
	 else  faktor=0;       /*BEFORE SUNRISE OR IN SHADE*/
 

/***********FUER OUTPUT SHADE EACH SUBINTERVAL *****/
if ((i==rowclim) && (j==colclim))
    faktorstation = faktor;
   /***********/

	  /* Korrektur fuer direkte Sonnenstrahlung:
	     wenn Sonne unter Horizont      : 0 (keine Korrektur)
	     wenn Sonne Flaeche nicht trifft: 0 (keine Korrektur)
	  */

	  DIRECT[i][j] += (faktor*Isenk[i][j]);

  /*Begrenzung ?   
	faktor/cos(sunzenithangle) > 5.   ->  faktor*cosZ*Isenk   */

      }  /*endfor  next grid*/


/*for the grid of the climate station, a horizontal slope must be taken for calculation of
  the direct radiation, because the pyranometer was installed horizontally. Thus, the 
  ratio of global radiation and direct radiation must refer to a horizontal plane. 
  Therefore, for the grid of the climate station the factor is recalculated using a slope 
  zero. The slope of the grid is not set to zero a priori, because the output files of 
  exkorr and direct radiation should refer to the actual slope. Thus, new variables are 
  needed to preserve the actual slope*/

      if ((!nostrl)&&(SHAPE[rowclim][colclim]<0.5))      /*AFTER SUNRISE AND NO SHADE*/
       {
	 faktorstationhoriz = getcosincidence(ASP[rowclim][colclim]*pi/180.0,
	       slopehoriz,sunazimuthangle,sunzenithangle);    /*slopehoriz=0*/

	 if (faktorstationhoriz<=0)
	   faktorstationhoriz=0.0;
       }
       else  faktorstationhoriz=0;       /*BEFORE SUNRISE OR IN SHADE*/

	directstationhoriz += (faktorstationhoriz*Isenk[rowclim][colclim]);

/**** $$$  KONTROLL-OUTPUT SCHATTENFILES FUER JEDES SUBINTERVAL*****/
  if (subintfileyes == 1)
   { printf("\n\ttime=%5.2f  shape=%5.2f  zenith=%6.2811f faktor=%5.2f direct =%6.2f",
       sp,SHAPE[rowclim][colclim],sunzenithangle*180/pi,faktorstation,
       Isenk[rowclim][colclim]/timestep*split);

    if (shadesubint)      /*FILE IS OPEN*/
     { fwrite(&(x[1]),sizeof(float),12,shadesubint);   /*WRITE TO FILE*/
	for (i=1;i<=nrows;i++)          /*ueber alle Reihen*/
	  fwrite(&(SHAPE[i][1]),sizeof(float),ncols,shadesubint); 

       fclose(shadesubint);
       shadesubint = NULL;     /*pointer freigeben*/
     }/*endif*/
   } /*endif fileyes*/

/* $$$ *********/


     }   /* Schleife fuer die <split> Teil-(zeit-)intervalle                   ********/
/* *******************************************  NEXT SPLIT TIME INTERVAL ************ */

  /* Strahlungssumme aller Teilintervalle wieder wegdividieren,
     also den effektiven Faktor, das eigentliche Ergebniss dieses Teilmodells, 
     errechnen */

  for (i=1;i<=nrows;i++)
    for (j=1;j<=ncols;j++)           
      {  SHADE[i][j]  = SHADE[i][j]/split;     /*mittlere Beschattung des time steps*/
	 EXKORR[i][j] = strlsumme[i][j] > 0.0 ?  DIRECT[i][j]/strlsumme[i][j] : 0.;
	    /*correction factor is the ratio of corrected rad to uncorrected radiation*/
	 DIRECT[i][j] = DIRECT[i][j]/timestep;     /*solar rad in W/m2 */

      }

/*climate station grid must be assumed to be horizontal for calculation of ratio glob/direct */
     exkorstationhoriz = strlsumme[rowclim][colclim] > 0.0 ? 
	   directstationhoriz/strlsumme[rowclim][colclim] : 0 ;
     directstationhoriz = (directstationhoriz/timestep);

/* $$$ *********/
  if (subintfileyes == 1)
   {   printf("\n\t  EXKORR = %6.2f strlsume= %6.2f, SHADE= %4.2f,   %d %d",
	   EXKORR[rowclim][colclim],strlsumme[rowclim][colclim],
	   SHADE[rowclim][colclim],rowclim,colclim);
   }
/* $$$ *********/

 return;

}


/*===========================================================================*/
/* FUNCTION getzenith   
       - calculating zenith angle for the middle of time interval,
	 because it is needed in radiat.c (ratio glob/direct)                */
/*===========================================================================*/

void getzenith()

{ 
  timekorr = 0.;        /*wenn Messdaten andere Zeit als MEZ not 0*/
  sunaltitudeangle = 0;

  jd2monat();           /*CONVERT JULIAN DAY TO MONTH AND DAY*/

 /* gibt die Verschiebung der realen Sonnenzeit gegenueber der
	 mittleren Sonnenzeit in min an (Elliptische Bahn) das Array steht
	 an jeder Seite um eins ueber, d.h., es kann auch mit monat-1
	 bzw. monat+1 gerechnet werden*/

  /* LG: timeequation gets re-initialized everytime schatten() or getzenith is called
     * could we just fix in in place as a global const somewhere? It already lives in the global
     * namespace, and we could  save some cycles by not re-initilizing the values every time.
     * Both of these get called all of the time, so it makes sense to do this
     */

  timeequation[0]  =  11.3;
  timeequation[1]  =  -3.2;     /*Januar*/
  timeequation[2]  = -13.6;
  timeequation[3]  = -12.6;
  timeequation[4]  =  -4.2;
  timeequation[5]  =   2.8;
  timeequation[6]  =   2.5;
  timeequation[7]  =  -3.5;
  timeequation[8]  =  -6.3;
  timeequation[9]  =  -0.3;
  timeequation[10] =  10.0;
  timeequation[11] =  16.4;
  timeequation[12] =  11.3;
  timeequation[13] =  -3.2;

   /*-----------------------------------------*/
  /* Berechnen von sunazimuthangle, sunaltitudeangle, sunzenithangle fuer diese Stunde
     Angaben der Ergebnisse in rad, der Eingangsdaten aber in Grad! */

  /* SUNRISE AND SUNSET FOR THE DAY */
    sunrise(tag,monat,breite,laenge,&sunr,&suns);      /*!!!!!!!!!!*/

  /* Teilintervall am Anfang der Integration (Mitte des timestep) */
      sp = zeit - timestep/2;

  /* GET SUN ZENITH ANGLE */
      getsuncoordinates(tag,monat,breite,laenge,sp,&sunazimuthangle,&sunaltitudeangle);
      sunzenithangle = pi/2 - sunaltitudeangle;

 return;

}


/*===========================================================================*/
/*  FUNCTION topofatmosphere
      calculation of solar radiation at top of atmosphere
      needed to separate diffuse radiation from global radiation             */
/*           called from main,    1/1997, updated 5/2006                     */
/*===========================================================================*/

void topofatmosphere()

{  float topofatmsum=0;
   int   kk;
   
   if(timestep < 24)      /*not daily time step*/
    {  getzenith();   /*get sunzenithangle for middle of time interval*/
       radiussunearth();
       topofatm = solarconst*radius2sun*cos(sunzenithangle);
       if(topofatm < 0)
          topofatm = 0.0;
    }
   else    /*daily time step, compute mean of half-hourly values*/
    {  timestep = 1;
       zeit = 0.5;
       for(kk=1;kk<=48;kk++)    /*half-hourly time steps*/
       {  zeit += 0.5;          /*next half-hour*/
          getzenith();   /*get sunzenithangle for middle of time interval*/
          radiussunearth();
          topofatm = solarconst*radius2sun*cos(sunzenithangle);
	  if(topofatm < 0)
             topofatm = 0.0;	  
	  topofatmsum += topofatm;
       }
       topofatm = topofatmsum/48;
       timestep = 24;    /*set back to initial value for daily time steps*/
       zeit     = 24;      
    }   /*endelse*/   

 return;
}



/*===========================================================================*/
/*  FUNCTION readdirect
      reading direct radiation (slope corrected) from existing files)
      instead of calculating it by function schatten (is less time consuming)
       - creating file names and reading data from files  
	     called from main,    9/1996                                     */
/*===========================================================================*/

void readdirect()

{ 
  jddirect = jd;   /*new variable to allow same direct files to be read over
                     a series of days - to save disk space*/

 if((int)jd/daysdirect != floor((int)jd/daysdirect))
    jddirect =  (floor((int)jd/daysdirect)) * daysdirect;

 strcpy(dummy,pathdirectfile);
 sprintf(namedirect,"dir%03d%02d.bin",(int)jddirect,(int)zeit);

/* fprintf(outcontrol,"%s jd= %7.2f jddirect = %8.2f\n",namedirect,jd,jddirect); */

 strcat(dummy,namedirect);

 if ((indirect = fopen (dummy,"rb")) == NULL)  {
       printf("\n globcor.c : direct radiation file not found : %s\n\n",dummy);
       exit(4);
 }  /*ENDIF*/

 if ( (fread(&(x[1]),sizeof(float),12,indirect)) !=12 )  {     /*FIRST 12 FIGURES*/
       printf("\n globcor.c:  ERROR in reading first 12 numbers in radiation file\n %s \n",dummy);
       exit(9);  
 }

 if ((fread(&(DIRECT[1][1]),sizeof(float),ncols*nrows,indirect)) != ncols*nrows)  {
     printf("\n globcor.c : ERROR in reading rad data in direct rad file\n %s\n",dummy);
     printf("               elevation DEM: ncols= %d nrows= %d\n",ncols,nrows);
     printf("               radiation file: ncols= %d nrows= %d\n",(int)x[1],(int)x[2]);
     printf("  Number of rows and columns must be identical in DEM and radiation files\n\n");
     exit(10);
 } 
  closefile(&indirect,namedirect);

 directstationhoriz = DIRECT[rowclim][colclim];
    /*used in radiat.c, must be a different name because in globcor.c
      both are different (DIRECT refers to sloped grid of climate station)*/

 return;

}


