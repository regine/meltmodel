% --------------------------------------------------------
% DEFINES VARIABLES FOR PLOT PROGRAMS OF MELT MODEL
% paths and files to read, etc
%   Nov 2011
% --------------------------------------------------------

winnumber = 1;   %windownumber
xlowlimit   = 1955;     %first year to be plotted
xupperlimit = 1960;     %last year not to be plotted


   % ----- CLIMATE / DISCHARGE DATA INPUT FILE
filenameclim = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/lemon_example/input/climatedischargeinput1949-2011.txt';
%filenameclimfuture = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/input/CCSM2005_2100pointB.txt';


   % ----- PLOT MEASURED AND MODELED DISCHARGE AND TEMP/PRECIP:   PROGRAM plotmodeldischarge.m -----------------
   % ----- ALSO COMPUTE AND PLOT ANNUAL MEAN DISCHARGE FROM DAILY DATA ---
% --- MEASURED DISCHARGE

filenamemodeldischarge = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/lemon_example/output/discharge.txt';
filenamemodelannualQ = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/lemon_example/output/modelannualdischarge.txt';

%filenamemodeldischarge = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputHerbert/out1/discharge.txt';
%filenamemodelannualQ = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputHerbert/out1/modelannualdischarge.txt';

%filenamemodeldischarge = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputDavis/out1/discharge.txt';
%filenamemodelannualQ = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputDavis/out1/modelannualdischarge.txt';

%filenamemodeldischarge = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputMendenhall/out1/discharge.txt';
%filenamemodelannualQ = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputMendenhall/out1/modelannualdischarge.txt';

% --- FUTURE DISCHARGE SCENARIO
%filenamemodeldischarge = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/output/outscenario1/discharge.txt';
%filenamemodelannualQ = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/output/outscenario1/modelannualdischarge.txt';

%filenamemodeldischarge = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputHerbert/outscenario1/discharge.txt';
%filenamemodelannualQ = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputHerbert/outscenario1/modelannualdischarge.txt';

%filenamemodeldischarge = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputDavis/outscenario1/discharge.txt';
%filenamemodelannualQ = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputDavis/outscenario1/modelannualdischarge.txt';

%filenamemodeldischarge = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputMendenhall/outscenario1/discharge.txt';
%filenamemodelannualQ ='/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputMendenhall/outscenario1/modelannualdischarge.txt';

  plotannualQ_yes = 1

  
% ----- PLOT FUTURE DISCHARGE:  PROGRAM plotmodeldischargefuture.m

  
   % ----- PLOT ANNUAL GLACIER-WIDE MASS BALANCES:   PROGRAM plotmassbal.m
   % -----------------
%filenamemassbal = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputMendenhall/out5/specificmassbal.txt';
%filenamemassbal = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputHerbert/out1/specificmassbal.txt';
%filenamemassbal = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/output/out1/specificmassbal.txt';
%filenamemassbal = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/outputDavis/out1/specificmassbal.txt';



%filenameCCSM = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/input/CCSM1960_2005interpol.txt';
%filenameCCSM = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/input/CCSM1960_2005pointA.txt';
%filenameCCSM = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/input/CCSM1960_2005pointB.txt';

   % ----- PLOT POINT BALANCES:   plotpointbalances.m -----------------
filenamepointbalancemeas = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/input/Mendenhall_massbal.txt';
filenamepointbalancemod = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/lemon_example/output/cummassbal.txt';
filenamepointbal_out = '/Users/cminor/Documents/work/hock_mass_balance/code/melt_mod/lemon_example/output/pointbal_meas_mod.txt';

numberstakes = 66;
xlow = -20;  xhigh= 10;    %axis tick limits
