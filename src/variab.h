
/*************************************************************/
/*  variab.h                                                 */
/*    update 18 Nov 2011 */
/*************************************************************/

/*-------------------------------------------------------------*/
/*** VARIABLES                                               ***/
/*-------------------------------------------------------------*/

/*HIDDEN OPTIONS TO ADJUST ========*/

float  surftempminimum = -30;  /*surftemp cannot be lower than this*/
int    dat_or_txt = 2;   /*1=dat, 2=txt, extension for areamean, grid asciifiles*/
int    setmelt2zero = 0;  /*melt is set to 0 if surftemp < surtempminmelt; yes=1, no=0;  April 2010*/
/*only if methodsurtempglac=3 */
int    surftempminmelt = -0.5;  /*if setmelt2zero=1 melt is set to 0 if surftemp below this value*/
float  z2=2;     /*height of the temp/wind/humidity measurements for bulk aerodynamic method*/
float  emissivitysurf=1;   /*surface emissivity for calc of longwave out from surf temp and for computing longwave outgoing (radiat.c)*/
int    snetfromobsyes = 1;     /*0=Shorwavenet from albedo, 1 = Snet from obs Sin Srefl only possible if calcgridyes=2 and obs available*/
/*to force energy balance with shortwave measurements in case only climate station computed*/
/*in order to exclude shortwave(albeodo) simulations from explaining deviations obs-simul*/
int    allradiationfromfile=0;  /*0=No, 1=Yes, take glob, refl, longin, longout from file, only if just climate station cell computed*/
/*(calgridyes in input.dat = 2, and only if methodsurftempglac = 3*/
/* also snetfromobsyes must be set to 1 in that case */
float snowscenario = 0;  /*default should be 0; this amount is added to each grid cell of the initial snow cover*/

/*=== SUBSURFACE SNOW MODEL ===*/
/*----- NEEDS TO BE ADJUSTED BY USER TO EACH CASE (glacier and simulation start condition ----------------------------------*/
/*----- choose default options for initialization of vertical profiles of temp and density*/
int init_layertemperature=1;	/*1=default 3 gradients, (2=does not exist), 3=storglaciaren*/
int init_snowlayerdensity=2;	/*1=default constant density, 2=default 3 gradients, 3=storglaciaren*/
int init_firnlayerdensity=1;		/*1=default constant density, 2=default 1 linear gradient,  3=storglaciaren*/
/*int init_snowlayermsnow=1;*/		/*1=default constant density, 2= default 3 linear gradients,  3=storglaciaren*/
int init_outputlines=1;		/*1=default,3=storglaciaren*/

/*============= INITIALIZATION OF SNOW TEMPERATURE PROFEILS - in case init_layertemperature = 2  ======================== */
/*temperatures at the turning points, 3 gradients are defined by 4 temperatures at 4 depths
 first temp is at surface, 4th at the depth where the ice is temperate*/
/*------ define temperatures at turning point depths between gradients are derived from linear interpolation*/
float  tempsurfaceAWS=-4.6;     /*estimated start surface temperature*/
float  tempturnpoint1AWS=-8.4;    /*temp at first turning point below surface*/
float  tempturnpoint2AWS=-8.75;    /*temp at second turning point below surface*/

float  depthtempturnpoint1 = 1.2;		/* depth of first turning point (where temp=tempgrad2)  */
float  depthtempturnpoint2 = 3.0;		/* depth of second turning point (where temp=tempgrad3) */
float  depthtemperate = 30.;		/* depth where ice is temperate (where temp=tempgrad3b  */

float  templapserate1 = -0.0;  /* lapse rate for spatial extrapolation of surface temp, degrees/100 m */
/*negative values mean decrease in temperature with increasing elevation*/
float  templapserate2 = -0.0;  /* lapse rate for spatial extrapolation of temp of 1. turning point */
/*next turning point is assumed spatially constant*/

/*============= INITIALIZATION OF SNOW DENSITY PROFEILS - in case init_snowlayerdensity = 2   =============*/
/*compute 3 density gradients between surface and depth of lowest firn layer*/
float  denssurface=150;     /*density at surface,  kg/m3 */
float  densturnpoint1=450;    /*density at first turning point below surface*/
float  densturnpoint2=460;    /*density at second turning point below surface*/

float  depthdensturnpoint1 = 0.3;		/* depth (m) of first turning point (where dens=densgrad2)  */
float  depthdensturnpoint2 = 1;		/* depth of second turning point (where dens=densgrad3) */
float  depthdensbottom = 1.4;		/* depth of snowpack  */
float  densfirn=900;   /*maximum density allowed, density at depthbottom*/

/*============= INITIALIZATION OF FIRN DENSITY PROFEILS - in case init_firnlayerdensity = 2   =============*/
/* calculate firnlayer density by a linear gradient*/
float firndensitytop = 600;     /*firn density at firn-snow interface,  kg/m3*/
float firndensitybottom = 800;  /*firn density at firndepthbottom*/
float firndepthbottom = 20;     /*depth at which firndensitybottom is reached, in m*/
/*deepest depth at which firnlayerdensity is needed*/
/*----- END OF VARIABLE THAT NEED TO BE ADJUSTED FOR INITIALIZATION OF SUBSURFACE PROFILES ---------------*/

int  supericegrid=0;     /* 0 = all grids at normal vertical resolution, 1 = chosen grids at larger vertical resolution 2 = all grids at larger resolution*/
int  typealbedo=1;   /*snow albedo equation: 0=Oerlemans, 1=Oerlemans/Zuo, 2=douville */
int  typeconduc=2;      /*5 functions to compute ice conductivity*/
int  typedens=3;		/*3 functions to compute densification of the dry snowpack*/
int  skin_or_inter=1;	/*0=skin temperature formulation, 1=linear interpolation upper 2 snow levels*/
int  tsurfextrapolation=2;   /*extrapolation to surface temp 1 = upper layer and surf, 2 = upper 2 layers and surf*/
int  resoutlinesmax = 50000;   /*exit program, if more lines written to res.out*/
int  bs_eachday_yes = 0;   /*summer balance written to file every day - for Dyurgerov method, 1=yes, 0=no*/

/*for function albedosnowdepth, snow albedo according to oerlemans and knap 98*/
float  snowstar=0.01; /*Carleen=0.040 * 0.011;  */  /*characteristic snowdepth in m snow*/
float  timestarwet=5; /*Carleen=9*21.9;*/	/*characteristic timescale wet surface in days*/
float  timestardry0=40;	/*Carleen=40 * 30;*/   /*characteristic timescale dry surface at 0C in days*/
float  timestardry10=2000;  /*Carleen=2000*characteristic timescale dry surface below -10C in days*/
/*for function slushformation, time scales according to Zuo and Oerlemans 1996 */
double  timec1 = 0.05;	/*time scale for runoff of surficial water on a steep surface (days)*/
double  timec2 = 20.;		/*time scale for runoff of surficial water on a horizontal surface (days)*/
double  timec3 = 2.;		/*time scale for runoff of surficial water on a surface with slope of 1 degree (days)*/
double  factc4 = 10.;  		/*factor with which surface water runsoff faster compared to internal runoff*/

/*for function albedosnowpoly for zongo glacier, function by J.E. Sicart, see PhD thesis p.243 */
double Czgo = 0.02 ;   /* h per mm we, parametre albk3 de hock */
double nzgo = 10 ;       /* in days */
double ezgo = 6 ;        /* in mm we */

/*for parameterization of longwave radiation according to Konzelmann, function longinstationkonzel*/
float bKonzel   = 0.442;   /* value in Konzelmann et al.  b=0.484 */
float eocKonzel = 0.968;   /* emissivity overcast in Konzelmann et al. eoc=0.952 */
float pKonzel   = 2;       /* value in Konzelmann et al.  p=4 */

/******************************************/
int    readsnowalbedo=0;   /*1=snow albedo read from another climate station file, daily means*/
/*========== END OF HIDDEN OPTIONS ==================================================*/

float  daysscreenoutput = 1;   /*output to screen every X day*/
int  resoutlines=0;  /*number of times unrealistic values are generated*/
int  calcgridyes;   /*compute whole grid (=1) or only climate station cell (=2), set in input.dat*/
int  ncolskurz;   /*for king george island, to cut columns in output grids*/

int    energymethod=0;      /*is set to 1 in meltmod.c*/
int    degreedaymethod=0;   /*set to 1 in degree.c*/
int    notcalc;
int    stoploop = 0;   /*end condition for time loop*/

int    scalingyes;     /*V-A scaling*/
float  gammaVA;    /*exponent in V-A scaling relationship*/
float  c_coefficient;    /*coefficient in V-A scaling relationship*/

/**** FILE NAMES ********/
char   *namedgm=NULL,*namedgmdrain=NULL,*namedgmglac=NULL;  /* Filenames */
char   *namedgmslope=NULL,*namedgmaspect=NULL,*nameklima=NULL;
char   *namedgmskyview=NULL,*nameinitialsnow=NULL;
char   *namedatesmassbal=NULL;
// RH char   inpath[81]="",outpath[81]="";    /*Path for Input/Output-files*/
char   inpath[201]="",outpath[201]="";    /*Path for Input/Output-files*/
char   namealb[20][31];             /*name of albedo-file of one day*/
char   namealbedo[31];              /*variable for all albedo files */
char   namealbedoout[31];           /*output file for grid output*/
char   nameshade[31],nameexkorr[31],namedirect[31],namedirect2[21]; /*OUTPUT-FILES*/
char   namesolhor[31],nameglobal[31],namediffus[31],nameswbal[21];
char   namelongin[31],namelongout[31],namenetrad[31],namesensible[31];
char   namelatent[31],namerainen[31],nameenbal[31],namemelt[31],nameabla[31];
char   namesurf[31],namesurftemp[31],namewritesnow[31];
char   namesnowfree[31];
char   namewinterbal[61],namesummerbal[61],namemassbal[61];
char   outgridname[20][31],outareaname[61],outspecificmassbalname[61];
char   namemassbalprofile[61];
char   dummy[101];
char   nametempgrid[61];

/********* FILES **********/
FILE   *indgm=NULL,*indgmdrain=NULL,*indgmglac=NULL;       /* input files */
FILE   *indgmslope=NULL,*indgmaspect=NULL,*inklima=NULL;
FILE   *inalbedo=NULL;
FILE   *indgmskyview=NULL,*ininitialsnow=NULL;
FILE   *indatesmassbal=NULL;
FILE   *outshade=NULL,*outexkorr=NULL,*outdirect=NULL;     /*OUTPUT-FILES*/
FILE   *outsolhor=NULL,*outglobal=NULL,*outswbal=NULL,*outnetrad=NULL;
FILE   *outdirect2=NULL,*outdiffus=NULL,*outalbedo=NULL,*outlongin=NULL,*outlongout=NULL;
FILE   *outsensible=NULL,*outlatent=NULL,*outrainen=NULL,*outenbal=NULL,*outmelt=NULL;
FILE   *outabla=NULL,*outsurf=NULL,*outsurftemp=NULL,*outwritesnow=NULL;
FILE   *outgrid[20], *outarea=NULL;
FILE   *outsnowfree, *outspecificmassbal=NULL;
FILE   *outsummerbal=NULL,*outwinterbal=NULL,*outmassbal=NULL;
FILE   *outcontrol=NULL;    /*CONTROL FILE*/
FILE   *inshade=NULL;    /*shading files, if shading read from files*/
FILE   *indirect=NULL;   /*direct radiation if read from files*/
FILE	*outmassbalprofile=NULL;
FILE   *intempgrid=NULL;   /*temperature grid read from file*/
FILE   *inprecipindexgrid;   /*precipitation index map read from file*/
FILE   *inprecipreadgrid;   /*precipitation grid read from file*/

/*********** ARRAYS *****************************/
float  **griddgm, **griddgmdrain, **griddgmglac;  /* 2 dim. Arrays grid data */
float  **SLOPE,**ASP;           /* 2-D-Grids slope and aspect */
float  **SHAPE,**SHADE,**EXKORR;     /* 2-D-Grids Shade and correction factor*/
float  **DIRECT,**DIRECT2;      /*clear-sky, direct separated from global rad*/
float  **DIFFUS,**ALBEDO,**LONGIN,**LONGOUT;
float  **surface;                  /*is grid firn,snow,ice? */
float  **SKYVIEW;    /* **SNOW  converted to double below 2/2005*/;
float  **GLOBAL,**SWBAL,**NETRAD;  /* 2-D-Arrays for radiation */
float  **SENSIBLE,**LATENT;        /* 2-D-Array for turbulent fluxes */

/* float  **ENBAL,**MELT,**ABLA;    made to doubles for snow model 2/2005*/

float  **precipreadgrid;    /*NEW 4/2010 to read precip grids from file for each time step*/
float  **precipindexmap;    /*NEW 4/2010 to read precip index map from file once*/
float  **rainenergy;
float  **ICEHEAT;
float  **tempint;        /*INTERPOLATED AIR TEMPERATURE*/
float  **surftemp;       /*SURFACE TEMPERATURE*/
float  **NEGBAL;         /*NEGATIVE ENERGY BALANCE*/
float  **ALBALT;         /*ALBEDO OF PRECEEDING TIME STEP*/
float  **snowtofirn;   /*boolean to decide if albedo drop, if snow has melted on firn*/
float  **numbdays;     /*number of day since snow fall, for snow albedo*/
float  **ndbefsnow;    /*number of days before snow fall*/
float  **ALBBEFSNOW;   /*snow albedo before snow fall*/
float  **WINTERBAL,**SUMMERBAL,**MASSBALcum;   /*mass balance*/
float  plusminus;

/*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
/*SNOW etc converted to double instead of float as before*/
float  **wrtSNOW,**wrtENBAL,**wrtMELT,**wrtABLA,**RUNOFF; /*CHR added */
double **SNOW,**ENBAL,**MELT,**ABLA;      /*ENERGY BALANCE, MELT mm*/
/*============================================================*/

double **meanSHADEday,**meanSHADEall;    /*ARRAYS FOR DAILY AND PERIOD MEANS*/
double **meanEXKORRday,**meanEXKORRall;
double **meanSOLHORday,**meanSOLHORall;  /*DIRECT RADIATION UNOBSTRUCTED HOR. SURFACE*/
double **meanDIRECTday,**meanDIRECTall;  /*DIRECT RAD SLOPE/SHADE-CORRECTED*/
double **meanDIRECT2day,**meanDIRECT2all;  /*DIRECT RAD CLOUD REDUCED*/
double **meanDIFFUSday,**meanDIFFUSall;
double **meanGLOBALday,**meanGLOBALall;
double **meanALBEDOday,**meanALBEDOall;
double **meanSWBALday,**meanSWBALall;    /*SHORT-WAVE RADIATION BALANCE*/
double **meanLONGINday,**meanLONGINall;
double **meanLONGOUTday,**meanLONGOUTall;
double **meanNETRADday,**meanNETRADall;
double **meanSENSIBLEday,**meanSENSIBLEall;
double **meanLATENTday,**meanLATENTall;
double **meanRAINENday,**meanRAINENall;
double **meanENBALday,**meanENBALall;
double **meanMELTday,**meanMELTall;
double **meanABLAday,**meanABLAall;
double **meanSURFTEMPday,**meanSURFTEMPall;   /*SURFACE TEMPERATURE*/

/********** WHICH GRID OUTPUT *********/
int    shayes,exkyes,solyes,diryes,dir2yes,difyes,albyes,gloyes;
int    swbyes,linyes,loutyes,netyes,senyes,latyes,rainyes,balyes,melyes,ablyes,posyes,ddfyes;
int    surftempyes,surfyes,snowyes;
int    snow2zeroeachyearyes,snowfreeyes;
int    winterbalyes,summerbalyes;
int	datesfromfileyes;

/********** WHICH METHOD USED FOR ENERGY BALANCE COMPONENTS **********/
int    methodinisnow;   /*read from file or generated internally*/
int    methodsnowalbedo;    /*snow albedo constant or generated*/
int    methodglobal;    /*direct and diffuse separated or not*/
int    methodlonginstation;  /*from measurements or parameterization*/
int    methodlongin;    /*longwave radiation constant or variable in space*/
int    methodsurftempglac;    /*surface temp 0 or lowered by iteration*/
int    methodturbul,methodiceheat,methodnegbal;

/********** TIME SERIES OUTPUT **************************/
double areashade,areaexkorr;    /*spatial means over whole glacier*/
double areasolhor,areadirect,areadirect2,areadiffus;
double areaalbedo,areaglobal,areareflect,areaswbal,arealongin,arealongout,areanetrad;
double areasensible,arealatent,arearain,areaenbal,areamelt,areaabla;
double areasurftemp;

double laenge, breite;                    /*longitude, latitude deg */
double reflongitude;              /*longitude time refers to*/
int    rowclim,colclim;           /* row and col of climate station */
int    climoutsideyes=0;    /*climate station inside or outside grid*/
float  heightclim;           /*elevation of climate station*/

float  gridsize, timestep;   /*read from input.dat*/
float  jdbeg, jdend;                      /* julian day begin, end  */
float  jdlasttimestep = -1;   /*needed for albedocloud function*/
float  winterjdbeg,winterjdend,summerjdbeg,summerjdend;
float  yearmeasmassbal;     /*year in file with measured mass balances*/
int    readdatesmassbalyes=1;   /*boolean variable to avoid allocating dates more than once*/
float  cumwinterbal=0.0;    /*cumulative annual area-averaged mass balances*/
float  cumsummerbal=0.0;
float  cumnetbal=0.0;


float  yearbeg, yearend;
int    formatclimdata;    /*format of climate data, meaning of midnight time*/
int    maxcol;                  /*number of columns in climate file */
int    coltemp, colhum, colwind;          /* column of parameter    */
int    colglob, colref, colnet, colprec;  /* in climate input file  */
int    collongin,collongout,colcloud,coltempgradvarying;

char   rest[200];                 /* to ignore rest of row or whole row */
float  jd,zeit,year;                                  /*julian day, hours*/
float  jd2;
float  temp,hum,wind,glob,ref=0,net,prec,cloud;               /*climate data*/

int    methodtempinterpol;   /*method to distribute air temperature across area*/
int    methodprecipinterpol;   /*method to distribute precipitation across area*/
float  tempgrad,precgrad,precgradhigh,snowmultiplierglacier,snowmultiplierrock;    /*for temp,precip interpolation*/
float  precgradelev;      /*beyond this elevation 2. precipgradient*/
float  tempscenario;     /*read temperature is changed by this amount*/
float  precscenario;     /*read precipitation is changed by this percentage*/
int    monthtempgradyes,monthtempscenyes,monthprecipscenyes;
float  monthtempgrad[13],monthtempscen[13],monthprecipscen[13]; /*monthly changes*/
float  ERAtempshift=0,ERAwindshift=0;

int    n_albfiles;       /*number of albedofiles*/
float  jdstartalb[21];   /*first day for which the albedo file of same index is used*/
int    ialb=1;           /*see startalbedo*/
int    do_out;           /*controlling frequency of output files*/
int    do_out_area;      /*if time series of spatial mean to file*/
int    disyes;                   /*=1 if discharge to be calculated */
int    nsteps=0;       /*number of time steps of whole period of calculations*/
int    outgridnumber;          /*number of grid outputfiles*/
int    stnrow[201],stncol[201];    /*row and column of grid to be written to output*/
int    firnreadyes;     /*if firn file has been read*/
float  p,e;                           /*air pressure, vapour pressure */
float  z0w,z0wice;          /*roughness length for ice*/
float  z0T,z0e;          /*roughness length for temp/vapour*/
float  PhiM=0,PhiH=0,PhiE=0;       /*stability functions wind, heat*/
float  frictionveloc=0;     /*friction velocity*/
float  MoninL;              /*Monin-Obukhov-length*/

float  *x;                      /* first 12 rows in grid files */

float  iceflux = 0;
int    ncols, nrows;           /*number of columns, rows*/
float  xll,yll,cs,nodata;      /*left lower corner,cellsize,missing value*/

int    firstrow,lastrow;       /*first row which contains a cell to be calculated*/
int    *firstcol,*lastcol;     /*first,last columns which is glacierized in DTM*/

float  P0 = 101325.0;           /* standard pressure  [Pa] */
float  startratio,ratio;        /*glob/calculated direct radiation*/
float  prozdiffuse;             /*percentage of diffuse radiation of direct radiation*/
float  trans;                   /*transmissivity for calculation of direct radiation*/
float  LWin;                    /*longwave incoming radiation at station, assumed constant*/
float  LWinsky;                 /*sky radiation, if climate station was unobstructed*/
float  LWout=315.6;                   /* assumption melting surface */
float  albsnow,albslush,albice,albrock,albfirn;  /*albedo*/
float  albmin;                  /*minimum snow albedo if albedo generated*/
float  snowalbincrease;     /*percent increase of snow alb per 100 m altitude*/

int    outglobnet[201];     /* measured glob and net written to station outputfile*/
float  topofatm;          /*top of atmosphere radiation*/
float  solarconst=1368.0;          /*solar constant*/
float  ratiodir2dir;    /*ratio of clear-sky to actual direct*/
float  diffus0;         /*diffuse radiation of unobstructed sky*/
float  meanalb;         /*mean albedo of entire area*/
float  surftemplapserate;  /*decrease of surftemp with increasing elevation*/

float  albk1,albk2,albk3;   /*coefficients for snow albedo*/
float  ndstart;             /*number of days after snow fall, for snow albedo*/
float  albiceproz;          /*decrease of ice alb with elevation in % */
float  daysnow=1;       /*snow cover written to file if jd dividable by this value*/

int    nglac;                  /*number of drainage basin grid cells*/
float  albedosnow[1000];        /*measured snow albedo - daily means*/
float  directstationhoriz;     /*direct radiation if climate station grid horizontal*/
int    shadefromfile=0;        /*1=shading is read from files*/
int    directfromfile;         /*1=direct radiation is read from files*/
char   pathdirectfile[100];    /*path of shading/direct input files*/
float  daysdirect = 1;         /*files only exist every number of days defined here*/
float  jddirect;               /*julian day of direct file prior to start day*/

float  albpercentagechange=0;  /*albedo modification according to clouds (Jonsell et al)*/
int    cumumeltyes;            /*1=cumulated melt written to file*/
float  cm_or_m;                /*unit of cumulated melt cm or m*/
float  iterstep;               /*iteration step for surface temp*/
float  iterationend=10;        /*set to ENBAL, as long as neg, iteration*/
int    iternumber=0;           /*number of iteration steps per grid cell*/
float  e0=611;                 /*saturation vapour pressure of ice, snow*/
float  lessmelt=0;             /*if energy balance negative and surf temp iterated*/
float  surftempstationalt=0;   /*surface temperature at climate station at start*/
int    slopestation = 1;       /*if 0 = slope at station set to 0, if 1=actual slope*/
float  z0proz;          /*% increase of zo with decreasing elevation*/
float  z0min,z0max;     /*min and max z0w ice*/
float  dividerz0T;             /*z0Temp is z0wind divided by this value*/
float  dividerz0snow;          /*z0snow is z0ice divided by this value*/
int    method_z0Te;     /*according to Andreas=1, fixed ratio=2*/

int  itswinter,itssummer;   /*for finding winter/summer period for massbalance*/
int  write2fileyes = 0;  /*for surface grids and snow cover files*/
int  newday=1;   /*counter of jd to be written to output: surface or snow cover*/
int  numbersnowdaysout;  /*number of days to be written to output*/
float jdsurface[31];   /*jd to be written to output*/

float winterjdbegmeas[2050];
float summerjdbegmeas[2050];
float nextyear;  /*needed if multi-year mass bal is computed and dates differ*/
float elevbeltmin,beltwidth;   /*for mass balance profiles*/
int   numberbelt;    /*number of elevation belts for bn profiles*/
float areabelt[101];   /*area of each elevation belt*/
float winterbalprofile[101],summerbalprofile[101],massbalprofile[101];
/*max 100 elevation belts for mass balance profiles*/

float ratioglobToA,ratioglobToAold=0.5;   /*for cloud dependence albedo*/

/************* PRECIPITATION *******/
float  T0;            /*threshold temperature for rain - snow*/
float  preccorr;
float  precip;        /*corrected and interpolated precipitation*/
float  rainprec,snowprec;
float  **RAIN;        /*grid of liquid precipitation*/

/***** for DISCHARGE calculation **************/

FILE   *firnfile;
FILE   *qcalcfile;       /*Output file of calculated discharge*/
FILE   *qmeasfile;       /*Input file of measured discharge*/
float   **FIRN;          /*thickness of firn cover*/
double  **qfirnopt,**qsnowopt,**qiceopt,**qrockopt;  /*for optimization*/
double  **f2,**sumf0x,**sumf0x2;
double  **r2,**f02;
double  **f2ln,**sumf0xln,**sumf0x2ln;
double  **r2ln,**f02ln;
double  **volumeopt;

char   namedgmfirn[30];
char   nameqcalc[30];

float  dischkons;
float  meltwat;
float  totalwat;        /*melt plus rain per timestep*/
float  sumk1=0,sumk2=0,sumk3=0,sumk4=0;     /*water volumes of reservoirs (firn,snow,ice)*/
int    onlyglacieryes;    /* =1 if melt is not calculated for whole basin*/
float  glacierpart;         /*glacierized percentage*/
float  firnkons,snowkons,icekons,rockkons;   /*storage constants k*/
float  qground;
float  qfirnstart,qsnowstart,qicestart,qrockstart;  /*starting values*/
float  qfirnsim,qsnowsim,qicesim,qrocksim;        /*for simulation*/
double  qdischdiffsum=0;   /*cumulated difference meas - calc discharge*/
double volumesim=0,volumemeas=0;    /*cumulated discharge volume*/
float  jdstartr2diff;
int    disyesopt;
float  startopt1,startopt2;     /*start values of optimization parameters*/
float  stepopt1,stepopt2;
int    anzahlopt1=1,anzahlopt2=1;
char   optkA[30],optkB[30];

int     status;      /* Variable to control reading from file */

float   jddisch,yeardisch;    /*julian day of discharge input file*/
float   zeitdisch;
float   qdischber;       /*calcualated discharge*/
float   qdischgem;       /*measured discharge*/
float   qdischdiff;      /*calcualted minus measured discharge*/
float   qdischdiffln;    /*logarithmic*/

int     coldis;
float   nodis;            /*nodata value of discharge file*/

char   namematrix[21];      /*name of r2-file, optimization run*/
FILE   *r2file;
int    iiopt=1,jjopt=1;
int    nstepsdis=0;

/***** for Joergs global radiation routines****/
int hmaxcalculated = 0;   /*fuer groesste Hoehe des DGM, damit nur 1x berechnet*/

long   tag,monat,tagnr;
int    mon[13]= {31,31,28,31,30,31,30,31,31,30,31,30,31};  /*number of days in month*/
double stunde;
double dekli,timeeq,stuwi,cosz,sonnenstand;         /*Function sonnenhoehe*/
double cosOM;
double aspect,slope,azimuth,zenith;
float  InV,InVtag;   /* Intervallaenge in Min sowie Anzahl der Intervalle pro Tag*/

double  sunazimuthangle,sunaltitudeangle,sunzenithangle;
double  cosincidence;
double  pi  = 3.141592654;

double  timeequation[14];
/* gibt die Verschiebung der realen Sonnenzeit gegenueber der
mittleren Sonnenzeit in min an (Elliptische Bahn) das Array steht
 an jeder Seite um eis ueber, d.h., es kann auch mit monat-1
 bzw. monat+1 gerechnet werden*/

float  timekorr; /*Zeitverschiebung der Zeitzone gegebueber den Zeitangaben der gemessenen Daten,
		    wenn die Daten in UTC angegeben werden, die Zeitzone aber MEZ ist, dann ist
		    timekorr=1.0 zu setzen usw.  */

float  hmax;     /* max elevation in indgm */
float  gewicht,sp,spa,spe;
float  cossunazim;

float  **Isenk;    /* potential clear sky solar radiation radiation Wh/m2*/
float  **Isenksum;
float  **strlsumme;         /*sum of Isenk over time interval*/
float  se;         /*=zeit*/
double *sr,*ss;
float  faktor;
int    dx,dy,fx,fy,vx,vy; /* fuer die Geradengleichungen */
float  dz;

double sunr,suns;     /* Sonnenauf- und Sonnenuntergangszeiten in hh.hh (dezimal) */
int    exkorrindex,shapeindex;
int    nostrl;
int    cn,count;      /* nach je count Intervallen werden neue Werte berechnet */
int    split;         /* number of intervals per time step for shade calculations */
/*  L.G. !!!!! Using these to loop may be dangerous !!!
 *  gcc is an optimizing compiler, if anything is multi-threaded gcc has the
 *  ability to change one of these while its being used in a different loop.
 *  These guys get used all over the place, but need to go if the code is multi-threaded
 * */
int    i,j,k,m;

/* end LG */

float  exkorstationhoriz;  /*radiation correction factor if station was horizontal*/
float  radius2sun;         /*for calculation top of atmosphere and direct radiation*/
float dark_a,dark_e,inta,inte,split_temp,Anteil_dunkel,Anteil_hell;
/*neu 22.7.1997*/


/*=========================================================*/
/*FOR TEMPERATURE INDEX METHOD - needed in degree.c*/
/*=========================================================*/

int    ddmethod;         /*which temperature index method*/
FILE   *outpos=NULL;     /*output grid file of pos temperatures*/
FILE   *outddf=NULL;     /*output grid file of degree day factors*/
char   namepos[80],nameddf[80];   /*names of output grid files*/
float  **TEMPOS,**DDFCALC;        /*2-D arrays*/
double **meanTEMPOSday,**meanTEMPOSall;
double **meanDDFCALCday,**meanDDFCALCall;
double areapos,areaddf;

float  meltfactor;       /*for modified temp index method*/
float  DDFsnow,DDFice;   /*DDF for snow/ice surface*/
float  radfactorice,radfactorsnow;   /*different one for ice and snow*/
float  areatemp;         /*spatial mean of air temp*/
float  areatpos;         /*spatial mean of positive temp*/
float  DDFcalc;          /*melt devided by sum of pos temp*/
float  DDF;              /*DDFice or DDFsnow, needed for optimization*/
float  radfactor;  /*radfactorice or radfactorsnow, needed for optimization*/
int    ddfoptyes=0;      /*optimization of melt parameter*/
float  debrisfactor;     /*melt reduction factor for debris cover on glacier*/

int  maxmeltstakes;       /*number of locations to be written to melt file*/
int  meltoutrow[101];      /*row of location*/
int  meltoutcol[101];      /*column of location*/
float  melt_xcoordinate[101];      /*x-coordinate of stake location*/
float  melt_ycoordinate[101];      /*y-coordinate of stake location*/
float  stn_xcoordinate[101];      /*x-coordinate of output station*/
float  stn_ycoordinate[101];      /*y-coordinate of output station*/

char outmeltstakename[80],outmassbalstakename[80];
FILE *outmeltstakes=NULL,*outmassbalstakes=NULL;
int  coordinatesyes;

float jdphoto;  /*for VERNAGTFERNER application*/

/*============================================================*/
/*======= for SNOWMODEL by Carleen Tijm-Reijmer, 2/2005=======*/
double ***layerdepth;       /*3d matrix with layer depth*/
double ***layerthickness;   /*3d matrix with layer thicknesses*/
double ***layerdensity;     /*3D matrix density of the snow layer*/
double ***layermass;        /*3D matrix mass (density*thickness) of the snow layer*/
double ***layertemperature; /*3D matrix temperature of the snow layer*/
double ***layerrhocp;       /*3D matrix dens*cp of the snow layer*/
double ***layerwatercont;   /*3D matrix watercontent layer*/
double ***layerrefreeze;    /*3D matrix total refrozen mass layer*/
float  ***layerid;          /*3D matrix defines snow layer=1, firnlayer=2 icelayer=3*/
float  **layeramount;       /*number of layers*/
float  **layeramountcold;   /*number of layers used to calculate the cold content*/
double **snowlayersum;      /*new snow depth in m snow not yet thick enough for nwe layer*/
double **snowlayer;         /*snow depth in m snow*/
double **meltlayer;         /*melted layer in m ice*/
double **MELTsum;           /*sum of melted snow on interpolated time steps*/
double **ABLAsum;           /*sum of ablation on interpolated time steps*/
float  **RUNOFFsum;         /*sum of melted snow + rain on interpolated time steps*/
float  **SNOWsum;           /*sum of snow precipitation on interpolated time steps*/
float  **SNOWinit;          /*initial snow height*/
float  **MASSBALcumstake;   /*sum of lost snow height for stakes*/
float  **SUMMERBALST;   	/*sum of lost snow height over summer for stakes*/
float  **SNOWswitch;		/*snow still snow covered (0) or not (1) (firn or ice at surface)*/
float  **sumSNOWprec;       /*sum of snow precipitation */
float  **sumRAINprec;       /*sum of rain precipitation */
float  **sumMASS;           /*sum of mass snow/firn/ice layer */
float  **DIRECTold;         /*direct radiation previous time step in case of time interpolation*/
float  **superice;          /*layer thickness superimposed ice in m ice*/
float  **tmpsuperice;       /*layer thickness superimposed ice in m ice*/
float  **watercontent;      /*total water content layer in kg*/
float  **capwatercontent;   /*total capilary water firn content layer in kg*/
float  **slwatercontent;    /*total water content firn layer in kg*/
float  **slushdepth;        /*depth of top slush layer in m snow*/
float  **slushthickness;    /*thickness of slush layer in m snow*/
double **surfacewater;      /*water amount on top snow/ice surface*/
float  **coldcontentsnow;   /*cold content snow layer, measure of temperate or not*/
float  **coldcontentice;    /*cold content total layer, measure of temperate or not*/
double *conduc;             /*layer conductivity*/
double *conducdtdz;         /*layer conductivitytimes temperature lapserate*/
int    ndepths;             /*maximum number of vertical layers*/
int    factinter;           /*factor for interpolation between two time steps*/
int    factsubsurfout;      /*factor for subsurf output to file 1 = every hour, 24 = ones per day at midnight */
int    offsetsubsurfout=0;  /*offfsetfactor for subsurf output to make print at noon possible */
int    inter;               /* counter for interpolation*/
float  thicknessfirst;      /*layer thickness of first layer*/
float  thicknessdeep;       /*maximum layer thickness*/
float  factorthickness=0.08;     /*factor used to determine coefficient of thickness increase*/
float  depthdeep;           /*maximum depth model*/
double denswater=1000;      /*water density kg/m3 */
double densice=900.0;         /*ice density kg/m3 */
float  diffdensice=0.01;    /*subtract from densice to prevent division by 0*/
double denssnow;            /*density of fresh snowfall kg/m3*/
float  cpice=2009;          /*specific heat capacity of ice Jkg-1K-1 (2097)*/
double rhocpice;            /*volumetric heat capacity of ice Jm-3K-1 densice * cpice */
double irrwatercont;        /*fraction of space irreducible filled with water*/
double sourceskin;          /*energy form skin layer temperature above 0*/
int  irrwatercontyes;     /*logical meaning 0 = constant irreducible water content 1 = density dependent according to Schneider and Jansson (2004) 2 = density dependent according to Coleou and Lesaffre (1998)*/
double freshsnowlayer;      /*freshsnowlayer thickness after melt calculation*/
int  percolationyes;        /*logical meaning 0 = dry 1 = meltwater percolation and refreezing*/
int  slushformationyes;     /*logical meaning 0 = not 1 = meltwater accumulation in layer*/
int  densificationyes;      /*logical meaning 0 = not 1 = densification of dry snow due to aging*/
int  wetstartyes;           /*logical meaning 0 = dry start, 1 = wet start*/
int  runoffyes,superyes,wateryes,surfwateryes,slushyes,coldsnowyes,coldtotyes;   /*for grid output*/
int  precipyes=1;				/*logical meaning 0 = no, 1 = yes sum precip output */
int  intaccumyes=1;				/*logical meaning 0 = no, 1 = yes internal accumulation output */
int klinesmax;					/* number of profile lines defined in snowinput*/
float iline1[10],iline2[10],jline1[10],jline2[10];      /* start (1) and end (2) coordinates profile lines*/

double jdold,tempold,humold,globold,refold,netold,LWinold,LWoutold;
double gradtemp,gradhum,gradglob,graddirect,gradref,gradnet,gradLWin,gradLWout;
float  timesteporig;
double sumrunoff,sumpercolation,summelt,sumrain;
float  **accyear;
float  q,q0;
float  ustar,thstar,qstar;
float  PhiM0,PhiH0,PhiE0;         /*stability functions wind, heat surface*/
char   outsubsurflinename[61];
FILE   *outsubsurf[50];
FILE   *outsubsurfline[11];
double source;   /*is ENBAL of considered grid cell*/

float taccur=0.01;
float tinterv=40.;
float tsurf1,tsurf2;
double balancetsurf, tbisection;
int kspechum;
double tspechum;

/*** name grid output***/
char   namerunoff[31],namesuper[31],namewater[31],namesurfwater[31],nameslush[21];
char   namecoldsnow[31],namecoldtot[31],namesnprecip[31],nameraprecip[31],namemass[31];
char   namecapwat[31],nameslwat[31],namesummerbalst[61];
char   outmassbalstakename2[80];
FILE   *outrunoff=NULL,*outsuper=NULL,*outwater=NULL,*outsurfwater=NULL;    /*outgridfiles*/
FILE   *outslush=NULL,*outcoldsnow=NULL,*outcoldtot=NULL,*outsnprecip=NULL,*outraprecip=NULL;
FILE   *outmass=NULL,*outcapwat=NULL,*outslwat=NULL,*outsummerbalst=NULL;
FILE   *outmassbalstakes2=NULL;
double **meanrunoffday,**meanrunoffall;
double **meansupericeday,**meansupericeall;     /*superimposed ice*/
double **meanwatercontentday,**meanwatercontentall;     /*water content*/
double **meansurfacewaterday,**meansurfacewaterall;     /*surface water*/
float  **wrtsurfacewater;
double **meanslushthicknessday,**meanslushthicknessall;     /*slush*/
double **meancoldcontentsnowday,**meancoldcontentsnowall;     /*cold content snow*/
double **meancoldcontenticeday,**meancoldcontenticeall;     /*cold content all*/
/*============================================================*/

