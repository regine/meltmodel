% Plot winter, summer, annual mass balance for each yearstart
% data from model output: specificmassbal.txt
% 10 Nov 2011
% ==================================

clear all;
close all;

inputreadtimeseries;    %read filenames etc (separate file)

% ---- OPEN CLIMATE / DISCHARGE FILE --------

DELIMITER = ' ';
HEADERLINES = 1;

% Import the discharge output file
newData2= importdata(filenamemassbal, DELIMITER, HEADERLINES);

% ---- OPEN MASS BALANCE OUTPUT FILE --------
% Create new variables in the base workspace from those fields.
vars = fieldnames(newData2);
for i = 1:length(vars)
    assignin('base', vars{i}, newData2.(vars{i}));
end

% ---- ALLOCATE VARIABLES --------
yearstart = data(:,1);
yearend   = data(:,2); 
Bw = data(:,3);
Bs = data(:,4);
Bn = data(:,5);

% ======= PLOT DATA==============
figure(winnumber)
plot(yearend,Bw);
set(gca,'XGrid','on')
set(gca,'YGrid','on')

xlim([xlowlimit,xupperlimit]);

hold on;
plot(yearend,Bs,'red');
plot(yearend,Bn,'black');

hold off;

%xlabel('t = 0 to 2\pi','FontSize',16)
%ylabel('sin(t)','FontSize',16)
title('{Bw= blue  Bs = red    Bannual=black}','FontSize',12)


%============ COMPUTE AVERAGE BALANCE OVER 2 PERIODS IN ARENDT ET AL., 2009 ===================
% 1948 - 3 Jun 1995 and 3 Jun 1995 - 4 Jun 1999

Bn_1     = Bn((yearend >= 1949) & (yearend <= 1995));
meanBn_1 = mean(Bn_1) 

yearshort2 = yearend((yearend >= 1996) & (yearend <= 1999))
Bn_2     = Bn((yearend >= 1996) & (yearend <= 1999))
meanBn_2 = mean(Bn_2)


