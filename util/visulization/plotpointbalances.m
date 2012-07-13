% Plot scattergram of measured and modeled point balances
% for each stake in the measured point balance file pull out the period in
% the modeled file (cummassbal.txt) and compute the mass balance only over
% that period, i.e. the difference between the last and first value of that
% time period of cumulative balances
% 20 Nov 2011
% ==================================

clear all;
close all;

inputreadtimeseries;   %read input file with filename etc (separate file)

% ---- OPEN MEASURED MASS BALANCE FILE --------
 [easting, northing, measbal, year1, day1, year2, day2] = textread(filenamepointbalancemeas, '%f %f %f %d %d %d %d ', 'headerlines', 1);

 year1decimal = year1 + day1/365.;
 year2decimal = year2 + day2/365.;
 
% ---- OPEN MODELIED MASS BALANCE FILE --------
 datamodel = importdata(filenamepointbalancemod, ' ',2);
    %refer to data by datamodel.data(:,1)  = display first column of data
    %variable datamodel is a struct

 numbertimesteps =  length(datamodel.data(:,1))    %length of first column randomly chosen, same for all columns

 yearmodel = datamodel.data(:,1);    %array with first column (year)
 daymodel  = datamodel.data(:,2);
 
 yearmodeldecimal = yearmodel + daymodel/365;
  
ii=1;
for ii=1:numberstakes   %for each stake (column)
  
  x = datamodel.data(:,ii+3);   %+3 to hop over the 3 time columns, put stake time series into x
  
  %cut out only the period the measurement refers to (year/day1 to
  %year/day2
  x_short = x( (yearmodeldecimal >= year1decimal(ii) ) & ( yearmodeldecimal <= year2decimal(ii)) );  %has to be '&' because it is array
  
  a = length(x_short);
    modelBn(ii) = (x_short(a) - x_short(1))/100;    %convert cm into m
    measBn(ii)  = measbal(ii);    %measured data is in m
  
  
end
  Bn_measmodel= [measBn;modelBn]';     %put measured and modeled point balance in one array


%============ SCATTERGRAM MEASURED - MODELED ===================
figure(winnumber)
plot(measBn,modelBn,'.','MarkerSize',10);
set(gca,'XGrid','on')
set(gca,'YGrid','on')
xlim([xlow,xhigh]);
title('{ x=obs y=model}','FontSize',12)

% -- plot 1:1 line -------------
x_line = [xlow:xhigh]
y_line = [xlow:xhigh]

hold on;
plot(x_line,y_line,'black');    %on-one line
hold off;


%============ WRITE TO OUTPUT FILE ===================
fout = fopen(filenamepointbal_out, 'w');
for i = 1:length(measBn)
  fprintf(fout, '%f\t %f\n',measBn(i),modelBn(i));
end
fclose(fout)
    