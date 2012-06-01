/*UPDATE Nov 2011*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void toobig_resout();
void initializeglacier2zero_nodata(float **glaciergrid);
void initializeglacier2zero_nodatadouble(double **glaciergrid);
void checkgridinputdata_ok();
void input_read();
void readrestofline(FILE **infile);
void freematrix(float **m,long nrl, long nrh, long ncl, long nch);
void freematrixdouble(double **m,long nrl, long nrh, long ncl, long nch);
void closeall();
void dischargeopt();
void write2matriz();
void opendiscalc();
void startarraysdis();
void startarraysopt();
void openr2matrizkopt();
void startdischarg();
void readdismeas();
void rainoutsideglac();
void negativemelt();
void meltrainvol();
void whichreservoir();
void sumr2();
void sumr2ln();
void writedisoutput();
void discharge();
void r2calc();
void r2calcln();
void tempinterpol();
void tempgradient();
void airpress();
void vappress();
void vappressicesnow();
void sensescher();
void latescher();
void sensible();
void latent();
void roughnesslength();
void roughnesslengthAndreas();
void roughnesslengthAndreasmodified();
void moninobukhovlength();
void sensiblestabilityiteration();

void precipinterpol();
void readprecipindexmap();   /*NEW 4/2010*/
void readprecipfromfile();   /*NEW 4/2010*/
void precipenergy();
void precipnoetzlidip();
void energybalance();
void waterequivalentmelt();
void waterequivalentabla();
void negenergybalance();
void snowcover();
void massbalance();
void areaelevationbelts();
void ratioglobal();
void albedoread();
void inialbedoalt();
void albedocalcconst();
void albedosnowmeas();
void albedocalc();
void albedosnownew();
void albedosnownewzgo();   /*JES*/
void globradratio();
void shortwavebalance();
void whichsurface();
void meanalbedo();
void albedosnowpoly();     /*for zongo, March 2006*/
void albedosnowclouds();   /*NEW 8/2004*/

void diffusunobstructed();
void interpoldiffus();
void adddirectdiffus();
void longwave_in1();
void longinskyunobstructed();
void longinpluess();
void longoutcalc();
void surftempfromlongout();
void radiationbalance();
void writesnowcover();
void writemassbalgrid();
void computemassbalprofile(float **matrixprofile,float *massbalbands);
void writemassbalprofile();
void meanmassbalprofile();

void writesnoworsurfaceyes();
void startspecificmassbalance();

void jd2monat();
double deklination(long tag, long monat);
double gettimeequation(long tag,long monat);
double sonnenhoehe(long tag,long monat,double breite,double laenge,double stunde);
void getsuncoordinates(long tag,long monat,double breite,double laenge,double stunde,double *sunazimuth,double *sunaltitude);
double getcosincidence(double aspect,double slope,double azimuth,double zenith);
void sunrise(long tag,long monat,double breite,double laenge,double *sr,double *ss);
void radiussunearth();
void schatten();
void getzenith();
void topofatmosphere();
void readdirect();
void openoutfile(FILE **datei,char *outpath,char *name);
void openoutfileascii(FILE **datei,char *outpath,char *name);
void startwritehour();
void startwriteday();
void startwriteall();
void statist(float **matrix,float *x);

void writegridoutput();
void sumday();
void sumall();
void areameannull();
void areasum();
void writesurface();
void areameanwrite();
void stationoutput();
float **matrixreserv(long nrl, long nrh, long ncl, long nch);
double **matrixreservdouble(long nrl, long nrh, long ncl, long nch);
void closefile(FILE **datei, char *name);
void meandayreserve();
void meandaynull();
void meanallnull();
void startinputdata();
void startoutascii();
void startarrayreserve();
void glacierrowcol();
void readclim();
void exitclimread();
void readalbedo29();
void readalbedo16();
void readdatesmassbal();

/*FUNCTIONS MORE SOPHISTICATED MODEL*/
void splitdiffusedirectcol();
void splitdiffusedirectkiruna();
void ratiodirectdirect();
void interpoldirect();
void albedocalc();
void longinstationnetradmeas();
void longinstationkonzel();
void cloudsfromratioglobTOASedlar();
void longinstationbrunts();
void longinstationbrutsaert();  /* by Brutsaert for clear sky, zongo for clouds, sicart PhD. p.260 */
void tempsurfacerock();
void iceheatStorglac();
void iterationstation();


/******* FUNCTION FOR TEMPERATURE INDEX METHOD, in tindex.c ***********/

void degreestart();
void temppos();
void ddfcalculation();
void degreedaymelt();
void dd_directmelt();
void dd_directglobal();
void startdisddfopt();
void dischargeddfopt();
void snowcoverdegree();
void melt2energy();      /*convert melt to energy for snow model*/


/****** OUTPUT MELT AT STAKES *****/
void startmeltstakes();
void writemeltstakes();

/*========================================*/
/*FUNCTIONS MORE SOPHISTICATED SNOWMODEL by Carleen Tijm-Reijmer, 2/2005*/
float *arrayreserv(long nrl, long nrh);
double *arrayreservdouble(long nrl, long nrh);
void freearray(float *v,long nrl, long nrh);
void freearraydouble(double *v,long nrl, long nrh);
float ***tensorreserv(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
double ***tensorreservdouble(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void freetensor(float ***t,long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void freetensordouble(double ***t,long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
void initializeglacier2zero_nodata_tens(float ***glaciergrid);
void initializeglacier2zero_nodata_tensdouble(double ***glaciergrid);
void meandaysnownull();
void albedocalcdepth();
void albedosnowdepth();
void initgrid();
void changegrid();
void resetgridwinter();
void resetgridsummer();
void surftempfrommodel();
void surftempskin();
void bisection();
void tsurfenergybalance();
void iceconductivity();
void subsurf();
void tempinterpolate();
void outputsubsurf();
void outputsubsurflines();
void statistdouble(double **matrix,float *x);
void densification();
void snowmelt();
void refreezing();
void percolation();
void slushformation();
void irreducible_schneider();
void irreducible_coleou();
void turbfluxes();
void spechum();
void stable();
void unstable();
void sensiblenew();
void latentnew();
void snowlayermsnow();
void meltlayermice();
/* Initialisation */
void choice_layertemperature();
void choice_snowlayerdensity();
void choice_firnlayerdensity();
void choice_snowlayermsnow();
void choice_outputlines();
/* Default initialization */
void default_layertemp3grad();   /*3 gradients,  NEW 4/2010*/
void default_layertemperature();  /*2 gradients, no longer used*/
void default_snowlayerdensity();  /*constant snow density, option 1*/
void default_snowlayerdensity3grad();  /*2 gradients, option 3*/

void default_firnlayerdensity();  /*constant firn density*/
void default_firnlayerdensity1grad();   /*1 linear gradient*/

void default_snowlayermsnow();
void default_snowlayermsnow3grad();

void default_outputlines();
void scaling();   /*V-A scaling*/

/* specific for initialization of Storglaciaren 1999*/
void stor_snowlayermsnow();
void stor_snowlayerdensity();
void stor_firnlayerdensity();
void stor_layertemperature();
void stor_outputlines();
/*========================================*/


/****** USERFILE *****/
void openphotovernagt();
void readphotovernagt();

void opensnowfree();
void percentsnowfree();
void measuredlapserate();

