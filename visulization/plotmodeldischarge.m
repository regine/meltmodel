% Plot modeled discharge and climate discharge data (temp, precip)
% Plot time series, daily means
% also plot runoff components (firn, snow, ice)
% compute annual means
% 8 Nov 2011
% ==================================

clear all;
close all;

inputreadtimeseries;    %read filenames etc (separate file)

%=============================================================
% ---- OPEN CLIMATE / DISCHARGE FILE (MEASURED DATA)--------

DELIMITER = ' ';
  HEADERLINES = 2;

% Import the file
newData1 = importdata(filenameclim, DELIMITER, HEADERLINES);

% Create new variables in the base workspace from those fields.
vars = fieldnames(newData1);
for i = 1:length(vars)
    assignin('base', vars{i}, newData1.(vars{i}));
end

% ---- ALLOCATE VARIABLES --------
year = data(:,1);
days = data(:,2); 
temp = data(:,4);
precip = data(:,5);
%Q_Mend = data(:,6);
%Q_Herbert = data(:,7);

%=============================================================
% ---- OPEN DISCHARGE MODEL OUTPUT FILE --------
% Import the file
%[year2, days2, time24, qmeas, qcalc, qfirn, qsnow, qice, qground,
%cumvolmeas, cumvolcalc, cumdiffc-m] = textread(filenamemodeldischarge, '%f %f %f %f %f %f %f %f %f %f %f %f', 'headerlines', 1);

DELIMITER = ' '; 
  HEADERLINES = 1;
newData1= importdata(filenamemodeldischarge, DELIMITER, HEADERLINES);
% Create new variables in the base workspace from those fields.
vars = fieldnames(newData1);
for i = 1:length(vars)
    assignin('base', vars{i}, newData1.(vars{i}));
end

% ---- ALLOCATE VARIABLES --------
year2 = data(:,1);
days2 = data(:,2);
qmeas = data(:,4);    %measured discharge
qcalc = data(:,5);
qfirn = data(:,6);
qsnow = data(:,7);
qice = data(:,8);
qrock = data(:,9);

qmeas(qmeas == -9999 ) = nan;

% set all -9999 values to NAN for plotting
%Q_Mend( Q_Mend == -9999 ) = nan;
%Q_Herbert( Q_Herbert == -9999 ) = nan;

% ------------------------------------------------------------
% --------------- MAKING PLOT ARRAYS SAME LENGTH
% cut Q_Mend array to only include the years that are in the model output file
% Data from climate input file
% cut out only whole years, otherwise annual discharge calculcation below
% is wrong

x      = year((year >= xlowlimit) & (year <= xupperlimit));  %has to be '&' because it is array
days   = days((year >= xlowlimit) & (year <= xupperlimit));
temp   = temp((year >= xlowlimit) & (year <= xupperlimit));
precip = precip((year >= xlowlimit) & (year <= xupperlimit));

%Date from discharge model file
x2     = year2((year2 >= xlowlimit) & (year2 <= xupperlimit));
days2  = days2((year2 >= xlowlimit) & (year2 <= xupperlimit));
  x2 = x2 + days2/365;

qmeas  = qmeas((year2 >= xlowlimit) & (year2 <= xupperlimit));  %from model output file
qcalc  = qcalc((year2 >= xlowlimit) & (year2 <= xupperlimit));
qfirn = qfirn((year2 >= xlowlimit) & (year2  <= xupperlimit));
qsnow = qsnow((year2 >= xlowlimit) & (year2  <= xupperlimit));
qice  = qice((year2 >= xlowlimit) & (year2   <= xupperlimit));
qrock  = qrock((year2 >= xlowlimit) & (year2 <= xupperlimit));

qmeas ( qmeas == -9999 ) = nan;

length(x2)
length(temp)
length(qmeas)

% ======================================================================
% --------- PLOT DATA -----------

figure(winnumber)
subplot(4,1,1);

%---------- Temp ----------------------
plot(x2,temp);
set(gca,'XGrid','on')
set(gca,'YGrid','on')
%plot(days,glo,'thick');

xlim([xlowlimit,xupperlimit]);

%xlabel('t = 0 to 2\pi','FontSize',16)
%ylabel('sin(t)','FontSize',16)
title('{Temperature Juneau}','FontSize',12)

%---------- precip ----------------------
subplot(4,1,2);
plot(x2,precip);
set(gca,'XGrid','on')
set(gca,'YGrid','on')
xlim([xlowlimit,xupperlimit]);
title('{Precipitation Juneau}','FontSize',12)

%======================================================================
%---------- DISCHARGE (modeled and measured)----------------------
subplot(4,1,3);
plot(x2,qmeas,'blue');
set(gca,'XGrid','on')
set(gca,'YGrid','on')
xlim([xlowlimit,xupperlimit]);
title('{Discharge}','FontSize',12)
title('{measured: blue, calculated=red}','FontSize',12)

hold on;
plot(x2,qcalc,'red');    %modeled discharge
hold off;

%======================================================================
%---------- Components: firn, snow, ice ----------------------

subplot(4,1,4);
plot(x2,qfirn,'black');
set(gca,'XGrid','on')
set(gca,'YGrid','on')
xlim([xlowlimit,xupperlimit]);
title('{Discharge}','FontSize',12)
title('{firn: black, snow: blue, ice: red,  rock=green}','FontSize',12)

hold on;
plot(x2,qsnow,'blue');    %modeled discharge
plot(x2,qice,'red');    %modeled discharge
plot(x2,qrock,'green');    %modeled discharge
plot(x2,qcalc,'black');    %modeled discharge

hold off;


%======================================================================
%------------- compute annual means and write to file--------------------------
if (plotannualQ_yes == 1)

 fout = fopen(filenamemodelannualQ, 'w');

 numberyears = xupperlimit - xlowlimit + 1
 annualQ= [1:numberyears]    %make array for annual discharge
 jj=0
 
 xannual = [xlowlimit:xupperlimit];


 for (i=1:numberyears)
   qcalc_cum = 0;
   for(j=1:365)
     jj=jj+1;
     qcalc_cum = qcalc_cum + qcalc(jj);    %add discharge for all days of one year
   end
   
   annualQ(i) = qcalc_cum/365.;    %calculate mean of the sum
   
   fprintf(fout, '%5d\t  %f\n', [xannual(i), annualQ(i)]);
 end

fclose(fout);

mean(annualQ)


%================ PLOT 2 ======================================================
%  plot annual mean discharge

figure(winnumber+1)
plot(xannual,annualQ,'blue');
set(gca,'XGrid','on')
set(gca,'YGrid','on')
xlim([xlowlimit,xupperlimit]);
title('{Annual mean discharge}','FontSize',12)

end
