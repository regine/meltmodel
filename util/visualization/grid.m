% Program grid.m plots an image plot of a grid
% given in the format of all input and output grids of the melt model (binary)
% RUNNING THE PROGRAM:
%  a) grid filename (if file to be plotted is in same directory as grid.m
%  b) grid pathname/filename (if file is elsewhere)
%  c) grid (enter) but you need to give the pathname/filename in the parameter file 'inputreadgrid.m'
% adjust stepx and stepy below to adjust so that tick labels are shown
% properly
% Regine Hock, Nov 2011, update August 2012


function [gsize,cellsize,LL,Zgrid,blankvalue] = grid(filename1);

%======== TO ADJUST BY USER =========================
winnumber = 1;   %window number
stepx = 200;    %determines spacing of tick labels on x-axis
stepy = 100;    %determines spacing of tick labels on y-axis
%=================================


if nargin == 0   %number of arguments, i.e. if only typed 'grid'
  inputreadgrid;   %read filename and path from file inputreadgrid.m (adjust before running)
end

% --- open and read grid file --------
fid = fopen(filename1,'r');
gsize = (fread(fid,2,'float'))'    %read first 2 values (ncols, nrows)
gsize = fliplr(gsize);        %flip left right, i.e. last element becomes first and vice versa


LL = fread(fid,2,'float')          %read xllcorner and yllcorner

% ---- assign header variables (first 12 values of .bin file)
cellsize = fread(fid,1,'float')
blankvalue = fread(fid,1,'float')
numvalue = fread(fid,1,'float')
minvalue = fread(fid,1,'float')
maxvalue = fread(fid,1,'float')
sumvalue = fread(fid,1,'float');
meanvalue = fread(fid,1,'float')
stdvalue = fread(fid,1,'float');

% ---- read grid matrix values -----
Zgrid = fread(fid,([gsize(2),gsize(1)]),'float');
Zgrid = fliplr(Zgrid);
Zgrid = rot90(Zgrid);
Zgrid(Zgrid==blankvalue)=nan;   %put nan to missing values
fclose(fid);

% ---- plot
%image(Zgrid, 'Cdatamapping','scaled');
%imagesc(Zgrid, 'Cdatamapping','scaled');
figure(winnumber)
pcolor(Zgrid);
shading flat;
set(gca,'ydir','reverse', 'XTick', 1:stepx:gsize(2), 'XTickLabel',  LL(1):stepx*cellsize:LL(1)+cellsize*(gsize(2)-1));   %flip upside down
%TTickLabel: drawing is according to rows and columns, but labeling
%according to coordinate values
set(gca,'ydir','reverse', 'YTick', 1:stepy:gsize(2), 'YTickLabel',  LL(2):stepy*cellsize:LL(2)+cellsize*(gsize(1)-1));   %flip upside down
axis image    %to scale so that x and y axis have same equidistance between tics, i.e. no distortion of image

colorbar;