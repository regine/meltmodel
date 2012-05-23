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

%Import the file
%import params: DELIMITER = ' ', HEADERLINES = 2;
newData1 = importdata(filenameclim, ' ', 2);

% ---- ALLOCATE VARIABLES --------
year = newData1.data(:,1);
days = newData1.data(:,2); 
temp = newData1.data(:,4);
precip = newData1.data(:,5);
%Q_Mend = newData1.data(:,6);
%Q_Herbert = newData1.data(:,7);
clear newData1; %this may be unneccesary but it might free up some mem

%=============================================================
% ---- OPEN DISCHARGE MODEL OUTPUT FILE --------
%Import the file
%[year2, days2, time24, qmeas, qcalc, qfirn, qsnow, qice, qground,
%cumvolmeas, cumvolcalc, cumdiffc-m] = textread(filenamemodeldischarge, '%f %f %f %f %f %f %f %f %f %f %f %f', 'headerlines', 1);

%import params: DELIMITER = ' ', HEADERLINES = 1;
newData2 = importdata(filenamemodeldischarge, ' ', 1);

% ---- ALLOCATE VARIABLES --------
year2 = newData2.data(:,1);
days2 = newData2.data(:,2);
qmeas = newData2.data(:,4);    %measured discharge
qcalc = newData2.data(:,5);
qfirn = newData2.data(:,6);
qsnow = newData2.data(:,7);
qice = newData2.data(:,8);
qrock = newData2.data(:,9);

clear newData2; %this may be unneccesary but it might free up some mem

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
mask = ((year >= xlowlimit) & (year <= xupperlimit));%has to be '&' because it is array
x      = year(mask);  
days   = days(mask);
temp   = temp(mask);
precip = precip(mask);

%Date from discharge model file
mask2 = ((year2 >= xlowlimit) & (year2 <= xupperlimit));
x2     = year2(mask2);
days2  = days2(mask2);
x2 = x2 + days2/365;

qmeas  = qmeas(mask2);  %from model output file
qcalc  = qcalc(mask2);
qfirn = qfirn(mask2);
qsnow = qsnow(mask2);
qice  = qice(mask2);
qrock  = qrock(mask2);

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
%title('{Discharge}','FontSize',12)
title('Discharge: measured: blue, calculated=red','FontSize',12)

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
%title('{Discharge}','FontSize',12)
title('Discharge: firn: black, snow: blue, ice: red,  rock=green','FontSize',12)

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
title('{Annual mean discharge (calculated)}','FontSize',12)

end
