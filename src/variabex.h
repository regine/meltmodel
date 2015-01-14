/***********************************************************************
 * Copyright 1996-2012 Regine Hock
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

/*************************************************************/
/*  variabex.h                                               */
/*    Last update 30 October 2013 */
/*************************************************************/

#ifndef MELT_MOD_VARIAB_H_
#define MELT_MOD_VARIAB_H_

#define FNAME_LEN 80
#define PNAME_LEN 200
#define PFNAME_LEN 280

/*-------------------------------------------------------------*/
/*** VARIABLES                                               ***/
/*-------------------------------------------------------------*/

/*HIDDEN OPTIONS TO ADJUST ========*/
extern  float  surftempminimum;  /*surftemp cannot be lower than this*/
extern  int    dat_or_txt;   /*extension for areamean, grid asciifiles*/

extern  int    setmelt2zero;  /*melt is set to 0 if surftemp < 0*/
extern  int    surftempminmelt;  /*if setmelt2zero=1 melt is set to 0 if surftemp below this value*/
extern  float z2;     /*height of the temp/wind/humidity measurements for bulk aerodynamic method*/
extern  float  emissivitysurf;   /*surface emissivity for calc of longwave out from surf temp and vice versa*/
extern  int  snetfromobsyes;  /*0=Snet from albedo model*/
/* 1=Snet from obs Sin Srefl only possible if calcgridyes = 0 and obs available*/
extern  int    allradiationfromfile;  /*0=No, 1=Yes, take glob, refl, longin, longout from file, only if only climate station cell computed*/
extern  float snowscenario;  /*this amount is added to each grid cell of the initial snow cover*/

extern  int  skin_or_inter;	/*0=skin temperature formulation, 1=linear interpolation upper 2 snow levels*/
extern  int  tsurfextrapolation;   /*extrapolation to surface temp 1 = upper layer and surf, 2 = upper 2 layers and surf*/
extern  int  subsurf_print_surf;	/*surface properties if available yes (1) or not (1) printed in outputsubsurf*/

/*=== SUBSURFACE SNOW MODEL ===*/
extern  int init_layertemperature;	/*1=default,3=storglaciaren*/
extern  int init_snowlayerdensity;	/*1=default,3=storglaciaren*/
extern  int init_firnlayerdensity;		/*1=default,3=storglaciaren*/
/*extern  int init_snowlayermsnow;*/		/*1=default,3=storglaciaren*/
extern  int init_outputlines;		/*1=default,2=storglaciaren*/

/*----- INITIALIZATION OF SNOW TEMPERATURE PROFEILS - in case init_layertemperature = 2 */
/*------ define temperatures at turning point depths between gradients are derived from linear interpolation*/
extern  float  tempsurfaceAWS;     /*estimated start surface temperature*/
extern  float  tempturnpoint1AWS;    /*temp at first turning point below surface*/
extern  float  tempturnpoint2AWS;    /*temp at second turning point below surface*/

extern  float  depthtempturnpoint1;		/* depth of first turning point (where temp=tempgrad2)  */
extern  float  depthtempturnpoint2;		/* depth of second turning point (where temp=tempgrad3) */
extern  float  depthtemperate;		/* depth where ice is temperate (where temp=tempgrad3b  */

extern  float  templapserate1;  /* lapse rate for spatial extrapolation of surface temp, degrees/100 m */
/*negative values mean decrease in temperature with increasing elevation*/
extern  float  templapserate2;  /* lapse rate for spatial extrapolation of temp of 1. turning point */
/*next turning point is assumed spatially constant*/

/*============= INITIALIZATION OF SNOW DENSITY PROFEILS - in case init_snowlayerdensity = 2   =============*/
/*compute 3 density gradients between surface and depth of lowest firn layer*/
extern  float  denssurface;     /*density at surface,  kg/m3 */
extern  float  densturnpoint1;    /*density at first turning point below surface*/
extern  float  densturnpoint2;    /*density at second turning point below surface*/

extern  float  depthdensturnpoint1;		/* depth (m) of first turning point (where dens=densgrad2)  */
extern  float  depthdensturnpoint2;		/* depth of second turning point (where dens=densgrad3) */
extern  float  depthdensbottom;		/* depth where dens = dens at densturnpoint2  */
extern  float  densfirn;   /*maximum density allowed, density at depthbottom*/

/*============= INITIALIZATION OF FIRN DENSITY PROFEILS - in case init_firnlayerdensity = 2   =============*/
/* calculate firnlayer density by a linear gradient*/
extern  float firndensitytop;     /*firn density at firn-snow interface,  kg/m3*/
extern  float firndensitybottom;  /*firn density at firndepthbottom*/
extern  float firndepthbottom;     /*depth at which firndensitybottom is reached, in m*/
/*============ END OF PARAMETERS FOR INITIALIZATION OF PROFILES==============*/

extern  int  supericegrid;     /* 0 = all grids at normal vertical resolution, 1 = chosen grids at larger vertical resolution*/
extern  int  typealbedo;   /*snow albedo equation: 0=Oerlemans, 1=Oerlemans/Zuo, 2=douville */
extern  int  typeconduc;   /*5 functions to compute ice conductivity*/
extern  int  typedens;		/*3 functions to compute densification of the dry snowpack*/
extern  int  resoutlinesmax;   /*exit program, if more lines written to res.out*/
extern  int   bs_eachday_yes;   /*summer balance written to file every day - for
 Dyurgerov method*/

/*for function albedosnowdepth, snow albedo according to oerlemans and knap 98*/
extern float  snowstar;      /*Carleen=0.040 * 0.011;  */  /*characteristic snowdepth in m snow*/
extern float  timestarwet;   /*Carleen=9*21.9;*/	/*characteristic timescale wet surface in days*/
extern float  timestardry0;  /*Carleen=40 * 30;*/   /*characteristic timescale dry surface at 0C in days*/
extern float  timestardry10; /*Carleen=2000*characteristic timescale dry surface below -10C in days*/
/*for function slushformation, time scales according to Zuo and Oerlemans 1996 */
extern double  timec1;	/*time scale for runoff of surficial water on a steep surface (days)*/
extern double  timec2;		/*time scale for runoff of surficial water on a horizontal surface (days)*/
extern double  timec3;		/*time scale for runoff of surficial water on a surface with slope of 1 degree (days)*/
extern double  factc4;  		/*factor with which surface water runsoff faster compared to internal runoff*/

/*for function albedosnowpoly for zongo glacier, function by J.E. Sicart, see PhD thesis p.243 */
extern double Czgo ;   /* h per mm we, parametre albk3 de hock */
extern double nzgo ;       /* in days */
extern double ezgo ;        /* in mm we */

/*for parameterization of longwave radiation according to Konzelmann, function longinstationkonzel*/
extern float bKonzel;     /* value in Konzelmann et al.  b=0.484 */
extern float eocKonzel;   /* emissivity overcast in Konzelmann et al. eoc=0.952 */
extern  float pKonzel;    /* value in Konzelmann et al.  p=4 */

extern int    readsnowalbedo;         /*1=snow albedo read from climate station file*/
/*========== END OF HIDDEN OPTIONS ==================================================*/

extern  float  daysscreenoutput;   /*output to screen every X day*/
extern int  resoutlines; /*number of times unrealistic values are generated*/
extern int  calcgridyes;   /*compute whole grid or only climate station cell*/
extern int  ncolskurz;   /*for king george island, to cut columns in output grids*/

extern  int    energymethod;      /*is set to 1 in meltmod.c*/
extern  int    degreedaymethod;   /*set to 1 in degree.c*/
extern  int    notcalc;
extern  int    stoploop;   /*end condition for time loop*/

extern int    scalingyes;
extern float  gammaVA;    /*exponent in V-A scaling relationship*/
extern float  c_coefficient;    /*coefficient in V-A scaling relationship*/

/**** FILE NAMES ********/
extern  char   *namedgm,*namedgmdrain,*namedgmglac;  /* Filenames */
extern  char   *namedgmslope,*namedgmaspect,*nameklima;
extern  char   *namedgmskyview,*nameinitialsnow;
extern  char   *namedatesmassbal;
extern  char   inpath[FNAME_LEN],outpath[FNAME_LEN];    /*Path for Input/Output-files*/
//extern  char   inpath[101],outpath[101];    /*Path for Input/Output-files*/
extern  char   namealb[20][FNAME_LEN];             /*name of albedo-file of one day*/
extern char   namealbedo[FNAME_LEN];              /*variable for all albedo files */
extern char   namealbedoout[FNAME_LEN];           /*output file for grid output*/
extern char   nameshade[FNAME_LEN],nameexkorr[FNAME_LEN],namedirect[FNAME_LEN],namedirect2[FNAME_LEN]; /*OUTPUT-FILES*/
extern char   namesolhor[FNAME_LEN],nameglobal[FNAME_LEN],namediffus[FNAME_LEN],nameswbal[FNAME_LEN];
extern char   namelongin[FNAME_LEN],namelongout[FNAME_LEN],namenetrad[FNAME_LEN],namesensible[FNAME_LEN];
extern char   namelatent[FNAME_LEN],namerainen[FNAME_LEN],nameenbal[FNAME_LEN],namemelt[FNAME_LEN],nameabla[FNAME_LEN];
extern char   namesurf[FNAME_LEN],namesurftemp[FNAME_LEN],namewritesnow[FNAME_LEN];
extern char   namesnowfree[FNAME_LEN];
extern char   namewinterbal[FNAME_LEN],namesummerbal[FNAME_LEN],namemassbal[FNAME_LEN];
extern char   outgridname[20][FNAME_LEN],outareaname[FNAME_LEN],outspecificmassbalname[FNAME_LEN];
extern char   namemassbalprofile[FNAME_LEN];
extern char   dummy[PFNAME_LEN];
extern char   nametempgrid[FNAME_LEN];

/********* FILES **********/
extern  FILE  *indgm,*indgmdrain,*indgmglac;       /* input files */
extern  FILE   *indgmslope,*indgmaspect,*inklima;
extern  FILE   *inalbedo;
extern  FILE   *indgmskyview,*ininitialsnow;
extern  FILE   *indatesmassbal;
extern  FILE   *outshade,*outexkorr,*outdirect;     /*OUTPUT-FILES*/
extern  FILE   *outsolhor,*outglobal,*outswbal,*outnetrad;
extern  FILE   *outdirect2,*outdiffus,*outalbedo,*outlongin,*outlongout;
extern  FILE   *outsensible,*outlatent,*outrainen,*outenbal,*outmelt;
extern  FILE   *outabla,*outsurf,*outsurftemp,*outwritesnow;
extern  FILE   *outgrid[20], *outarea;
extern  FILE   *outsnowfree, *outspecificmassbal;
extern  FILE   *outsummerbal,*outwinterbal,*outmassbal;
extern  FILE   *outcontrol;    /*CONTROL FILE*/
extern  FILE   *inshade;    /*shading files, if shading read from files*/
extern  FILE   *indirect;
extern  FILE   *outmassbalprofile;
extern  FILE   *intempgrid;   /*temperature grid read from file*/
extern  FILE   *inprecipindexgrid;   /*precipitation index map read from file*/
extern  FILE   *inprecipreadgrid;   /*precipitation grid read from file*/
extern  FILE   *outperformance;     /*model performance (r2 etc) written to file*/

/* ********* ARRAYS **************************** */
extern  float  **griddgm, **griddgmdrain, **griddgmglac;  /* 2 dim. Arrays grid data */
extern  float  **SLOPE,**ASP;           /* 2-D-Grids slope and aspect */
extern  float  **SHAPE,**SHADE,**EXKORR;     /* 2-D-Grids Shade and correction factor*/
extern  float  **DIRECT,**DIRECT2;      /*clear-sky, direct separated from global rad*/
extern  float  **DIFFUS,**ALBEDO,**LONGIN,**LONGOUT;
extern  float  **surface;                  /*is grid firn,snow,ice? */
extern  float  **SKYVIEW;    /* **SNOW  converted to double below 2/2005*/;
extern  float  **GLOBAL,**SWBAL,**NETRAD;  /* 2-D-Arrays for radiation */
extern  float  **SENSIBLE,**LATENT;        /* 2-D-Array for turbulent fluxes */

/* extern  float  **ENBAL,**MELT,**ABLA;  made to doubles for snow model 2/2005*/

extern float  **precipreadgrid;    /*NEW 4/2010 to read precip grids from file for each time step*/
extern float  **precipindexmap;    /*NEW 4/2010 to read precip index map from file once*/
extern  float  **rainenergy;
extern  float  **ICEHEAT;
extern  float  **tempint;        /*INTERPOLATED AIR TEMPERATURE*/
extern  float  **surftemp;       /*SURFACE TEMPERATURE*/
extern  float  **NEGBAL;         /*NEGATIVE ENERGY BALANCE*/
extern  float  **ALBALT;         /*ALBEDO OF PRECEEDING TIME STEP*/
extern  float  **snowtofirn;     /*for snow albedo parameterization*/
extern  float  **numbdays;     /*number of day since snow fall, for snow albedo*/
extern  float  **ndbefsnow;    /*number of days before snow fall*/
extern  float  **ALBBEFSNOW;   /*snow albedo before snow fall*/

extern float  **MASSBALcum;   /*(cumulative) mass balance over one year*/
extern float  **MASSBALcum_all;   /*(cumulative) mass balance over whole calculation period*/
extern float  **MASSBALgrid;   /*grid with mass balance for each time step*/

extern  float  **WINTERBAL,**SUMMERBAL,**MASSBALcum;   /*mass balance*/
extern float  **MASSBALgrid;   /*grid with mass balance for each time step*/
extern  float  plusminus;

/*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
/*SNOW etc converted to double instead of float as before*/
extern  float  **wrtSNOW,**wrtENBAL,**wrtMELT,**wrtABLA,**RUNOFF; /*CHR added */
extern  double **SNOW,**ENBAL,**MELT,**ABLA;      /*ENERGY BALANCE, MELT mm*/
/*============================================================*/

extern  double **meanSHADEday,**meanSHADEall;    /*ARRAYS FOR DAILY AND PERIOD MEANS*/
extern  double **meanASPday,**meanASPall;
extern  double **meanEXKORRday,**meanEXKORRall;
extern  double **meanSOLHORday,**meanSOLHORall;  /*DIRECT RADIATION UNOBSTRUCTED HOR. SURFACE*/
extern  double **meanDIRECTday,**meanDIRECTall;  /*DIRECT RAD SLOPE/SHADE-CORRECTED*/
extern  double **meanDIRECT2day,**meanDIRECT2all;  /*DIRECT RAD CLOUD REDUCED*/
extern  double **meanDIFFUSday,**meanDIFFUSall;
extern  double **meanALBEDOday,**meanALBEDOall;
extern  double **meanGLOBALday,**meanGLOBALall;
extern  double **meanSWBALday,**meanSWBALall;    /*SHORT-WAVE RADIATION BALANCE*/
extern  double **meanLONGINday,**meanLONGINall;
extern  double **meanLONGOUTday,**meanLONGOUTall;
extern  double **meanNETRADday,**meanNETRADall;
extern  double **meanSENSIBLEday,**meanSENSIBLEall;
extern  double **meanLATENTday,**meanLATENTall;
extern  double **meanRAINENday,**meanRAINENall;
extern  double **meanENBALday,**meanENBALall;
extern  double **meanMELTday,**meanMELTall;
extern  double **meanABLAday,**meanABLAall;
extern  double **meanSURFTEMPday,**meanSURFTEMPall;   /*SURFACE TEMPERATURE*/

/* ********* WHICH GRID OUTPUT ******** */
extern  int    shayes,exkyes,solyes,diryes,dir2yes,difyes,albyes,gloyes;
extern  int    swbyes,linyes,loutyes,netyes,senyes,latyes,rainyes,balyes,melyes,ablyes,surfyes,posyes,ddfyes;
extern  int    surftempyes,surfyes,snowyes;
extern  int    snow2zeroeachyearyes,snowfreeyes;
extern  int    winterbalyes,summerbalyes;
extern  int    datesfromfileyes;

/* ********* WHICH METHOD USED FOR ENERGY BALANCE COMPONENTS ********* */
extern  int    methodinisnow;     /*read from file or generated internally*/
extern  int    methodsnowalbedo;  /*snow albedo constant or generated*/
extern  int    methodglobal;    /*direct and diffuse separated or not*/
extern  int    methodlonginstation;  /*from measurements or parameterization*/
extern  int    methodlongin;    /*longwave radiation constant or variable in space*/
extern  int    methodsurftempglac;    /*surface temp 0 or lowered by iteration*/
extern  int    methodturbul,methodiceheat,methodnegbal;

extern  double laenge, breite;                    /*longitude, latitude deg */
extern  double reflongitude;              /*longitude time refers to*/
extern  int    rowclim,colclim;           /* row and col of climate station */
extern  int    climoutsideyes;    /*climate station inside or outside grid*/
extern  float  heightclim;           /*elevation of climate station*/

extern  float  gridsize, timestep;
extern  float  jdbeg, jdend;                      /* julian day begin, end  */
extern  float  jdlasttimestep;   /*needed for albedocloud function*/

extern  float  winterjdbeg,winterjdend,summerjdbeg,summerjdend;
extern  float  yearmeasmassbal;     /*year in file with measured mass balances*/
extern  int    readdatesmassbalyes;   /*boolean variable to avoid allocating dates more than once*/
extern  float  cumwinterbal;    /*cumulative annual area-averaged mass balances*/
extern  float  cumsummerbal;
extern  float  cumnetbal;

extern  float  yearbeg, yearend;
extern  int    maxcol;                /*number of columns in climate file */
extern  int    formatclimdata;  /*format of climate data, meaning of midnight time*/

extern  int    coltemp, colhum, colwind;          /* column of parameter    */
extern  int    colglob, colref, colnet, colprec;  /* in climate input file  */
extern  int    collongin,collongout,colcloud,coltempgradvarying;

extern  char   rest[500];                 /* to ignore rest of row or whole row */
extern  float  jd,zeit,year;                                  /*julian day, hours*/
extern  float  jd2;
extern  float  temp,hum,wind,glob,ref,net,prec,cloud;               /*climate data*/

extern  int    methodtempinterpol;   /*method to distribute air temperature across area*/
extern  int    methodprecipinterpol;   /*method to distribute precipitation across area*/
extern  float  tempgrad,precgrad,precgradhigh,snowmultiplierglacier, snowmultiplierrock;   /*for temp,precip interpolation*/
extern float   precgradelev;      /*beyond this elevation 2. precipgradient*/
extern  float  tempscenario;     /*read temperature is changed by this amount*/
extern  float  precscenario;     /*read precipitation is changed by this percentage*/
extern int    monthtempgradyes,monthtempscenyes,monthprecipscenyes;
extern float  monthtempgrad[13],monthtempscen[13],monthprecipscen[13]; /*monthly changes*/
extern float  ERAtempshift,ERAwindshift;

extern  int    n_albfiles;       /*number of albedofiles*/
extern  float  jdstartalb[21];   /*first day for which the albedo file of same index is used*/
extern  int    ialb;           /*see startalbedo*/
extern  int    do_out;           /*controlling frequency of output files*/
extern  int    do_out_area;      /*if time series of spatial mean to file*/
extern int    disyes;                   /*=1 if discharge to be calculated */
extern int    nsteps;       /*number of time steps of whole period of calculations*/
extern int    outgridnumber;          /*number of grid outputfiles*/
extern int    stnrow[201],stncol[201];    /*row and column of grid to be written to output*/
extern int    firnreadyes;     /*if firn file has been read*/
extern float  p,e;                           /*air pressure, vapour pressure */
extern float  z0w,z0wice;          /*roughness length for ice*/
extern float  z0T,z0e;          /*roughness length for temp/vapour*/
extern float  PhiM,PhiH,PhiE;         /*stability functions wind, heat*/
extern float  frictionveloc;     /*friction velocity*/
extern float  MoninL;            /*Monin-Obukhov-length*/

extern float  *x;                      /* first 12 rows in grid files */

extern int    ncols, nrows;           /*number of columns, rows*/
extern float  xll,yll,cs,nodata;      /*left lower corner,cellsize,missing value*/

extern int    firstrow,lastrow;       /*first row which contains a glacier grids*/
extern int    *firstcol,*lastcol;     /*first,last columns which is glacierized in DTM*/

extern float  P0;           /* standard pressure  [Pa] */
extern float  startratio,ratio;        /*glob/calculated direct radiation*/
extern float  prozdiffuse;             /*percentage of diffuse radiation of direct radiation*/
extern float  trans;                   /*transmissivity for calculation of direct radiation*/
extern float  LWin;                    /*longwave radiation*/
extern float  LWinsky;                 /*sky radiation, if climate station was unobstructed*/
extern float  LWout;              /* assumption melting surface */
extern float  albsnow,albslush,albice,albrock,albfirn;  /*albedo*/
extern float  albmin;                  /*minimum snow albedo if albedo generated*/
extern float  snowalbincrease;     /*percent increase of snow alb per 100 m altitude*/

extern int    outglobnet[201];     /* measured glob and net written to station outputfile*/
extern float  topofatm;          /*top of atmosphere radiation*/
extern float  solarconst;          /*solar constant*/
extern float  ratiodir2dir;  /*ratio of clear-sky to actual direct*/
extern float  diffus0;         /*diffuse radiation of unobstructed sky*/
extern float  meanalb;         /*mean albedo of entire area*/
extern float  surftemplapserate;  /*decrease of surftemp with increasing elevation*/

extern float  albk1,albk2,albk3;   /*coefficients for snow albedo*/
extern float  ndstart;             /*number of days after snow fall, for snow albedo*/
extern float  albiceproz;          /*decrease of ice alb with elevation in % */
extern float  daysnow;     /*snow cover written to file if jd dividable by this value*/

extern double areashade,areaexkorr;    /*spatial means over whole glacier*/
extern double areasolhor,areadirect,areadirect2,areadiffus;
extern double areaalbedo,areaglobal,areareflect,areaswbal,arealongin,arealongout,areanetrad;
extern double areasensible,arealatent,areaiceheat,arearain,areaenbal,areamelt,areaabla;
extern double areasurftemp;
extern double areamassbal;    /*New 10/2013: glacierwide balance for each time step for time series output*/
extern double areamassbalcum;   /*New 10/2013: cumulated glacierwide balance e.g.for GRACE comparison*/

extern int    ndrain;                  /*number of drainage basin grid cells: area that is computed*/
extern int    nglac;                /*number of glacier grid cells, needed for mass balance*/

extern float  albedosnow[1000];        /*measured snow albedo - daily means*/
extern float  directstationhoriz;     /*direct radiation if climate station grid horizontal*/
extern int    shadefromfile;        /*1=shading is read from files*/
extern int    directfromfile;       /*1=direct radiation is read from files*/
extern char   pathdirectfile[PNAME_LEN];     /*path of shading/direct input files*/

extern float  daysdirect;             /*files only exist every number of days defined here*/
extern float  jddirect;               /*julian day of direct file prior to start day*/

extern float  albpercentagechange;    /*albedo modification according to clouds (Jonsell et al)*/
extern int    cumumeltyes;            /*1=cumulated melt written to file*/
extern float  cm_or_m;                /*unit of cumulated melt cm or m*/
extern float  iterstep;               /*iteration step for surface temp*/
extern float  iterationend;           /*set to ENBAL, as long as neg, iteration*/
extern int    iternumber;             /*number of iteration steps per grid cell*/
extern float  e0;                     /*saturation vapour pressure of ice, snow*/
extern float  lessmelt;               /*if energy balance negative and surf temp iterated*/

extern float  surftempstationalt;   /*surface temperature at climate station at start*/
extern int    slopestation;         /*if 0 = slope at station set to 0, if 1=actual slope*/
extern float  z0proz;        /*% increase of zo with decreasing elevation*/
extern float  z0min,z0max;     /*min and max z0w ice*/
extern float  dividerz0T;             /*z0Temp is z0wind divided by this value*/
extern float  dividerz0snow;          /*z0snow is z0ice divided by this value*/
extern int    method_z0Te;     /*according to Andreas=1, fixed ratio=2*/

extern int  itswinter,itssummer;
extern int  write2fileyes;  /*for surface grids and snow cover files*/
extern int  newday;   /*counter of jd to be written to output: surface or snow cover*/
extern int  numbersnowdaysout;  /*number of days to be written to output*/
extern float jdsurface[31];   /*jd to be written to output*/

extern float winterjdbegmeas[2050];
extern float summerjdbegmeas[2050];
extern float nextyear;  /*needed if multi-year mass bal is computed and dates differ*/
extern float elevbeltmin,beltwidth;   /*for mass balance profiles*/
extern int   numberbelt;    /*number of elevation belts for bn profiles*/
extern float areabelt[101];   /*area of each elevation belt*/
extern float winterbalprofile[101],summerbalprofile[101],massbalprofile[101];
/*max 100 elevation belts for profiles*/

extern float ratioglobToA,ratioglobToAold;   /*for cloud dependence albedo*/

/* ************ PRECIPITATION ****** */
extern float  T0;            /*threshold temperature for rain - snow*/
extern float  preccorr;
extern float  precip;        /*corrected andextern interpolated precipitation*/
extern float  rainprec,snowprec;
extern float  **RAIN;        /*grid of liquid precipitation*/

/* **** for discharge calculation ************* */

extern FILE   *firnfile;
extern FILE   *qcalcfile;       /*Output file of calculated discharge*/
extern FILE   *qmeasfile;       /*Input file of measured discharge*/
extern float   **FIRN;
extern double  **qfirnopt,**qsnowopt,**qiceopt,**qrockopt;  /*for optimization*/
extern double  **f2,**sumf0x,**sumf0x2;
extern double  **r2,**f02;
extern double  **f2ln,**sumf0xln,**sumf0x2ln;
extern double  **r2ln,**f02ln;
extern double  **volumeopt;

extern char   namedgmfirn[FNAME_LEN];
extern char   nameqcalc[FNAME_LEN];

extern float  dischkons;
extern float  meltwat;
extern float  totalwat;
extern float  sumk1,sumk2,sumk3,sumk4;
extern int    onlyglacieryes;    /* =1 if melt is not calculated for whole basin*/
extern float  glacierpart;         /*glacierized percentage*/
extern float  firnkons,snowkons,icekons,rockkons;
extern float  qground;
extern float  qfirnstart,qsnowstart,qicestart,qrockstart;  /*starting values*/
extern float  qfirnsim,qsnowsim,qicesim,qrocksim;        /*for simulation*/
extern double  qdischdiffsum;   /*cumulated difference meas - calc discharge*/
extern double volumesim,volumemeas;
extern float  jdstartr2diff;
extern int    disyesopt;
extern float  startopt1,startopt2;
extern float  stepopt1,stepopt2;
extern int    anzahlopt1,anzahlopt2;
extern char   optkA[30],optkB[30];

extern int     status;      /* Variable to control reading from file */

extern float   jddisch,yeardisch;    /*julian day of discharge input file*/
extern float   zeitdisch;
extern float   qdischber;
extern float   qdischgem;       /*measured discharge*/
extern float   qdischdiff;      /*calcualted minus measured discharge*/
extern float   qdischdiffln;

extern int     coldis;    /*column of discharge data in input file*/
extern float   nodis;            /*nodata value of discharge file)*/

extern char   namematrix[FNAME_LEN];      /*name of r2-file, optimization run*/
extern  FILE   *r2file;
extern int    nstepsdis;

/* **** for Joergs global radiation routines*** */
extern int hmaxcalculated;   /*fuer groesste Hoehe des DGM, damit nur 1x berechnet*/

extern int   tag,monat,tagnr;
extern int    mon[13];   /*number of days in month*/
extern double stunde;
extern double dekli,timeeq,stuwi,cosz,sonnenstand;         /*Function sonnenhoehe*/
extern double cosOM;
extern double aspect,slope,azimuth,zenith;
extern float  InV,InVtag;   /*extern intervallaenge in Min sowie Anzahl derextern intervalle pro Tag*/

extern double  sunazimuthangle,sunaltitudeangle,sunzenithangle;
extern double  cosincidence;
extern double  pi;

extern double  timeequation[14];
/* gibt die Verschiebung der realen Sonnenzeit gegenueber der
mittleren Sonnenzeit in min an (Elliptische Bahn) das Array steht
 an jeder Seite um eis ueber, d.h., es kann auch mit monat-1
 bzw. monat+1 gerechnet werden*/

extern float  timekorr; /*Zeitverschiebung der Zeitzone gegebueber den Zeitangaben der gemessenen Daten,
		    wenn die Daten in UTC angegeben werden, die Zeitzone aber MEZ ist, dann ist
		    timekorr=1.0 zu setzen usw.  */

extern float  hmax;     /* max elevation in indgm */
extern float  gewicht,sp,spa,spe;
extern float  cossunazim;

extern float  **Isenk;    /* potential clear sky solar radiation radiation Wh/m2*/
extern float  **Isenksum;
extern float  **strlsumme;         /*sum of Isenk over timeextern interval*/
extern float  se;         /*=zeit*/
extern double *sr,*ss;
extern float  faktor;
extern int    ii,jj,dx,dy,fx,fy,vx,vy; /* fuer die Geradengleichungen */
extern float  dz;

extern double sunr,suns;     /* Sonnenauf- und Sonnenuntergangszeiten in hh.hh (dezimal) */
extern int    exkorrindex,shapeindex;
extern int    nostrl;
extern int    cn,count;      /* nach je countextern intervallen werden neue Werte berechnet */
extern int    split;         /* number ofextern intervals per time step for shade calculations */
extern int    i,j;

extern float  exkorstationhoriz;  /*radiation correction factor if station was horizontal*/
extern float  radius2sun;         /*for calculation top of atmosphere and direct radiation*/
extern float dark_a,dark_e,inta,inte,split_temp,Anteil_dunkel,Anteil_hell;
/*neu 22.7.1997*/


/*=========================================================*/
/*FOR TEMPERATURE INDEX METHOD - needed in degree.c*/
/*=========================================================*/

extern int    ddmethod;         /*which temperature index method*/
extern  FILE   *outpos;     /*output grid file of pos temperatures*/
extern  FILE   *outddf;     /*output grid file of degree day factors*/
extern char   namepos[FNAME_LEN],nameddf[FNAME_LEN];   /*names of output grid files*/
extern float  **TEMPOS,**DDFCALC;        /*2-D arrays*/
extern double **meanTEMPOSday,**meanTEMPOSall;
extern double **meanDDFCALCday,**meanDDFCALCall;
extern double areapos,areaddf;

extern float  meltfactor;       /*for modified temp index method*/
extern float  DDFsnow,DDFice;   /*DDF for snow/ice surface*/
extern float  radfactorice,radfactorsnow;   /*different one for ice and snow*/
extern float  areatemp;         /*spatial mean of air temp*/
extern float  areatpos;         /*spatial mean of positive temp*/
extern float  DDFcalc;          /*melt devided by sum of pos temp*/
extern float  DDF;              /*DDFice or DDFsnow, needed for optimization*/
extern float  radfactor;  /*radfactorice or radfactorsnow, needed for optimization*/
extern int    ddfoptyes;      /*optimization of melt parameter*/
extern float  debrisfactor;     /*melt reduction factor for debris cover on glacier*/

extern int maxmeltstakes;     /*number of locations to be written to melt file*/
extern int meltoutrow[101];    /*row of location*/
extern int meltoutcol[101];    /*column of location*/
extern float  melt_xcoordinate[101];      /*x-coordinate of location*/
extern float  melt_ycoordinate[101];      /*y-coordinate of location*/
extern float  stn_xcoordinate[101];      /*x-coordinate of output station*/
extern float  stn_ycoordinate[101];      /*y-coordinate of output station*/

extern char outmeltstakename[PFNAME_LEN],outmassbalstakename[PFNAME_LEN];
extern FILE *outmeltstakes,*outmassbalstakes;
extern int  coordinatesyes;

extern float jdphoto;  /*for VERNAGTFERNER application*/

/*============================================================*/
/*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
extern double ***layerdepth;       /*3d matrix with layer depth (m)*/
extern double ***layerthickness;   /*3d matrix with layer thicknesses (m)*/
extern double ***layerdensity;     /*3D matrix density of the snow layer (kg/m3 = g/kg)*/
extern double ***layermass;        /*3D matrix mass (density*thickness) of the snow layer (mm)*/
extern double ***layertemperature; /*3D matrix temperature of the snow layer (C)*/
extern double ***layerrhocp;       /*3D matrix dens*cp of the snow layer*/
extern float  ***layerid;          /*3D matrix defines snow layer=1, firnlayer=2 icelayer=3*/
extern double ***layerwatercont;   /*3D matrix watercontent layer*/
extern double ***layerdeltawatercont;   /*3D matrix change in watercontent layer per time step*/
extern double ***layerrefreeze;    /*3D matrix total refrozen mass layer*/
extern double ***layerdeltarefreeze;    /*3D matrix per time step refrozen mass per layer*/
extern float  **layeramount;       /*number of layers*/
extern float  **layeramountcold;   /*number of layers used to calculate the cold content*/
extern double **snowlayersum;      /*new snow depth in mm we not yet thick enough for nwe layer*/
extern double **snowlayer;         /*snow depth in m snow*/
extern double **meltlayer;         /*melted layer in m ice*/
extern double **MELTsum;           /*sum of melted snow on interpolated time steps*/
extern double **ABLAsum;           /*sum of ablation on interpolated time steps*/
extern float  **RUNOFFsum;         /*sum of melted snow + rain on interpolated time steps*/
extern float  **MBsum;           /*sum of massbalance on interpolated time steps*/
extern double **deltaREFREEZE;     /*refrozen mass per (sub)time step*/
extern double **deltaREFREEZEsum;  /*refrozen mass summated over sub time steps*/
extern double **deltaWATER;        /*change in water content per (sub)time step*/
extern double **deltaWATERsum;     /*change in water content summated over sub time steps*/
extern float  **SNOWinit;          /*initial snow height*/
extern float  **MASSBALcumstake;   /*sum of lost snow height for stakes*/
extern float  **SUMMERBALST;       /*sum of lost snow height over summer for stakes*/
extern float  **SNOWswitch;		   /*snow still snow covered (0) or not (1) (firn or ice at surface)*/
extern float  **sumSNOWprec;       /*sum of snow precipitation */
extern float  **sumRAINprec;       /*sum of rain precipitation */
extern float  **sumMASS;           /*sum of mass snow/firn/ice layer */
extern float  **DIRECTold;         /*direct radiation previous time step in case of time interpolation*/
extern float  **DIRECTsum,**DIRECT2sum;      /*sumclear-sky, direct separated from global rad*/
extern float  **DIFFUSsum,**LONGINsum,**LONGOUTsum;
extern float  **GLOBALsum;		/* 2-D-Arrays for radiation */
extern float  **SENSIBLEsum,**LATENTsum;        /* 2-D-Array for turbulent fluxes */
extern float  **rainenergysum;
extern float  **ICEHEATsum;
extern double **SUBLIMATIONsum;  /*sum of mass condensated or sublimated*/ 
extern double **SUBLIMATION;		/*mass sublimated or condensated*/
extern float  **REFLECTsum;		/*Reflected solar radiation, used for averaging over subtime step*/
extern float  **superice;          /*layer thickness superimposed ice in m ice*/
extern float  **tmpsuperice;          /*layer thickness superimposed ice in m ice*/
extern double  **watercontent;      /*total water content in kg*/
extern double  **watercontentsum;   /*total water content in kg summed over subtimesteps*/
extern float  **capwatercontent;   /*total capilary water firn content layer in kg*/
extern float  **slwatercontent;    /*total water content firn layer in kg*/
extern float  **slushdepth;        /*depth of top slush layer in m snow*/
extern float  **slushthickness;    /*thickness of slush layer in m snow*/
extern double **surfacewater;      /*water amount on top snow/ice surface*/
extern float  **coldcontentsnow;   /*cold content snow layer, measure of temperate or not*/
extern float  **coldcontentice;    /*cold content total layer, measure of temperate or not*/
extern float  **meltenergy,**meltenergysum;    		/*total amount of energy put into melt*/
extern double *conduc;             /*layer conductivity*/
extern double *conducdtdz;         /*layer conductivitytimes temperature lapserate*/
extern double *layerenergy;    /*layer available energy to refreeze*/
extern int    ndepths;             /*maximum number of vertical layers*/
extern int    factinter;           /*factor for interpolation between ntwo time steps*/
extern int    factsubsurfout;      /*factor for subsurf output to file 1 = every hour, 24 = ones per day at midnight */
extern int    offsetsubsurfout;    /*offfsetfactor for subsurf output to make print at noon possible */
extern int    inter;               /* counter for interpolation*/
extern float  thicknessfirst;      /*layer thickness of first layer*/
extern float  thicknessdeep;       /*maximum layer thickness*/
extern float  factorthickness;     /*factor used to determine coefficient of thickness increase*/
extern float  depthdeep;           /*maximum depth model*/
extern double denswater;           /*water density kg/m3 */
extern double densice;             /*ice density kg/m3 */
extern float  diffdensice;         /*subtract from densice to prevent division by 0 */
extern double denssnow;            /*density of fresh snowfall*/
extern float  cpice;               /*specific heat capacity of ice Jkg-1K-1 (2097)*/
extern double rhocpice;            /*volumetric heat capacity of ice Jm-3K-1 densice * cpice */
extern double irrwatercont;        /*fraction of space irreducible filled with water*/
extern double sourceskin;          /*energy form skin layer temperature above 0*/
extern int  irrwatercontyes;     /*logical meaning 0 = constant irreducible water content 1 = density dependent according to Schneider and Jansson (2004) 2 = density dependent according to Coleou and Lesaffre (1998)*/
extern double freshsnowlayer;      /*freshsnowlayer thickness after melt calculation*/
extern int  percolationyes;      /*logical meaning 0 = dry 1 = meltwater percolation and refreezing*/
extern int  slushformationyes;     /*logical meaning 0 = not 1 = meltwater accumulation in layer only when percolationyes is 1*/
extern int  densificationyes;    /*logical meaning 0 = not 1 = densification of dry snow due to aging*/
extern int  wetstartyes;         /*logical measing 0 = dry start, 1 = wet start*/
extern int  runoffyes,superyes,wateryes,surfwateryes,slushyes,coldsnowyes,coldtotyes;   /*for grid output*/
extern int  precipyes;				/*logical meaning 0 = no sum precip output, 1 = yes sum precip output*/
extern int  intaccumyes;			/*logical meaning 0 = no, 1 = yes internal accumulation output */
extern int klinesmax;					/* number of profile lines defined in snowinput*/
extern float iline1[10],iline2[10],jline1[10],jline2[10];      /* start (1) and end (2) coordinates profile lines*/

extern double jdold,tempold,humold,globold,refold,netold,LWinold,LWoutold;
extern double gradtemp,gradhum,gradglob,gradref,gradnet,gradLWin,gradLWout;
extern double **graddirect;
extern float  timesteporig;
extern double sumrunoff,sumpercolation,summelt,sumrain;
extern float  **accyear;
extern float  q,q0;
extern double  ustar,thstar,qstar;
extern double  PhiM0,PhiH0,PhiE0;         /*stability functions wind, heat surface*/
extern double  Ch,Cq;         /*exchange coefficients heat and moisture surface*/
extern char   outsubsurflinename[FNAME_LEN];
extern FILE   *outsubsurf[50];
extern FILE   *outsubsurfline[11];
extern double source;   /*is ENBAL of considered grid cell*/

extern float taccur;
extern float tinterv;
extern double tsurf1,tsurf2;
extern double balancetsurf, tbisection;
extern int kspechum;
extern double tspechum;

/*** name grid output***/
extern char   namerunoff[FNAME_LEN],namesuper[FNAME_LEN],namewater[FNAME_LEN],namesurfwater[FNAME_LEN],nameslush[FNAME_LEN];
extern char   namecoldsnow[FNAME_LEN],namecoldtot[FNAME_LEN],namesnprecip[FNAME_LEN],nameraprecip[FNAME_LEN],namemass[FNAME_LEN];
extern char   namecapwat[FNAME_LEN],nameslwat[FNAME_LEN],namesummerbalst[FNAME_LEN];
extern char   outmassbalstakename2[FNAME_LEN];
extern FILE   *outrunoff,*outsuper,*outwater,*outsurfwater;    /*outgridfiles*/
extern FILE   *outslush,*outcoldsnow,*outcoldtot,*outsnprecip,*outraprecip;
extern FILE   *outmass,*outcapwat,*outslwat,*outsummerbalst;
extern FILE   *outmassbalstakes2;
extern double **meanrunoffday,**meanrunoffall;
extern double **meansupericeday,**meansupericeall;     /*superimposed ice*/
extern double **meanwatercontentday,**meanwatercontentall;     /*water content*/
extern double **meansurfacewaterday,**meansurfacewaterall;     /*surface water*/
extern float  **wrtsurfacewater;
extern double **meanslushthicknessday,**meanslushthicknessall;     /*slush*/
extern double **meancoldcontentsnowday,**meancoldcontentsnowall;     /*cold content snow*/
extern double **meancoldcontenticeday,**meancoldcontenticeall;     /*cold content all*/
/*============================================================*/

#endif // MELT_MOD_VARIAB_H_
