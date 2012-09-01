% grids2diff
% Plots 2 grids in 2 subplots and the difference between those in a third subplot
% RUNNING THE PROGRAM:
%  a) grids2diff filename (if file to be plotted is in same directory as grids2diff.m)
%  b) grids2diff pathname/filename (if file is elsewhere)
%  c) grids2diff (enter) but you need to give the pathname/filename in the parameter file 'inputreadgrid.m'

% Regine Hock, adjusted in August 2012
% ======================================================

function [gsize,cellsize,LL,Zgrid,blankvalue] = grids2diff(filename1,filename2);

%======== TO ADJUST BY USER =========================
winnumber = 1   %window number
stepx = 5000;    %determines spacing of tick labels on x-axis
stepy = 10;    %determines spacing of tick labels on y-axis
%=================================


if nargin == 0   %number of arguments, i.e. if only typed 'grid'
  inputreadgrid;
end

% --- open and read second grid file ----------

fid =fopen(filename1,'r');
gsize1 = (fread(fid,2,'float'))'    %read first 2 values (ncols, nrows)
gsize1 = fliplr(gsize1);        %flip left right, i.e. last element becomes first and vice versa

LL = fread(fid,2,'float')          %read xllcorner and yllcorner
cellsize1 = fread(fid,1,'float')
blankvalue1 = fread(fid,1,'float')
numvalue1 = fread(fid,1,'float')
minvalue1 = fread(fid,1,'float')
maxvalue1 = fread(fid,1,'float')
sumvalue1 = fread(fid,1,'float');
meanvalue1 = fread(fid,1,'float')
stdvalue1 = fread(fid,1,'float');
Zgrid1 = fread(fid,([gsize1(2),gsize1(1)]),'float');
Zgrid1 = fliplr(Zgrid1);
Zgrid1 = rot90(Zgrid1);
Zgrid1(Zgrid1==blankvalue1)=nan;
fclose(fid);

% --- read second grid file ----------
fid2 =fopen(filename2,'r');
gsize2 = (fread(fid2,2,'float'))'    %read first 2 values (ncols, nrows)
gsize2 = fliplr(gsize2);        %flip left right, i.e. last element becomes first and vice versa

%gsize2 = [190 167]    %Lemmon
%gsize2 = [186 160]    %Davis

LL2 = fread(fid2,2,'float')          %read xllcorner and yllcorner
cellsize2 = fread(fid2,1,'float')
blankvalue2 = fread(fid2,1,'float')
numvalue2 = fread(fid2,1,'float')
minvalue2 = fread(fid2,1,'float')
maxvalue2 = fread(fid2,1,'float')
sumvalue2 = fread(fid2,1,'float');
meanvalue2 = fread(fid2,1,'float')
stdvalue2 = fread(fid2,1,'float');
Zgrid2 = fread(fid2,([gsize2(2),gsize2(1)]),'float');
Zgrid2 = fliplr(Zgrid2);
Zgrid2 = rot90(Zgrid2);
Zgrid2(Zgrid2==blankvalue2)=nan;
fclose(fid2);


% --- plot both grids in supplots and the difference between grid1 and grid2 in third subplot----------

%image(Zgrid, 'Cdatamapping','scaled');
%imagesc(Zgrid, 'Cdatamapping','scaled');
figure(winnumber)

step = 200

subplot(3,1,1);
 pcolor(Zgrid1);
 shading flat;
set(gca,'ydir','reverse', 'XTick', 1:step:gsize1(2), 'XTickLabel',  LL(1):stepx*cellsize1:LL(1)+cellsize1*(gsize1(2)-1));   %flip upside down
%TTickLabel: drawing is according to rows and columns, but labeling according to coordinate values
set(gca,'ydir','reverse', 'YTick', 1:step:gsize1(2), 'YTickLabel',  LL(2):stepy*cellsize1:LL(2)+cellsize1*(gsize1(1)-1));   %flip upside down 
set(gca,'clim',[min(minvalue1,minvalue2),max(maxvalue1,maxvalue2)]);
axis image    
%to scale so that x and y axis have same equidistance between tics, i.e. no distortion of image
colorbar;

subplot(3,1,2);
 pcolor(Zgrid2);
 shading flat;
set(gca,'ydir','reverse', 'XTick', 1:step:gsize2(2), 'XTickLabel',  LL(1):stepx*cellsize2:LL(1)+cellsize2*(gsize2(2)-1));   %flip upside down
%TTickLabel: drawing is according to rows and columns, but labeling
%according to coordinate values
set(gca,'ydir','reverse', 'YTick', 1:step:gsize2(2), 'YTickLabel',  LL(2):stepy*cellsize2:LL(2)+cellsize2*(gsize2(1)-1));   %flip upside down
set(gca,'clim',[min(minvalue1,minvalue2),max(maxvalue1,maxvalue2)]);
axis image    %to scale so that x and y axis have same equidistance between tics, i.e. no distortion of image
 colorbar;
 
subplot(3,1,3);
 pcolor(Zgrid1-Zgrid2);
shading flat;
set(gca,'ydir','reverse', 'XTick', 1:step:gsize1(2), 'XTickLabel',  LL(1):stepx*cellsize1:LL(1)+cellsize1*(gsize1(2)-1));   %flip upside down
%TTickLabel: drawing is according to rows and columns, but labeling
%according to coordinate values
set(gca,'ydir','reverse', 'YTick', 1:step:gsize1(2), 'YTickLabel',  LL(2):stepy*cellsize1:LL(2)+cellsize1*(gsize1(1)-1));   %flip upside down

axis image    %to scale so that x and y axis have same equidistance between tics, i.e. no distortion of image

colorbar;