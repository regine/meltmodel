% Adjusted in Nov 2012


function [gsize,cellsize,LL,Zgrid,blankvalue] = grid(filename,winnumber);

if nargin == 0   %number of arguments, i.e. if only typed 'grid'
  inputreadgrid;
end

fid =fopen(filename,'r');
gsize = (fread(fid,2,'float'))'    %read first 2 values (ncols, nrows)
gsize = fliplr(gsize);        %flip left right, i.e. last element becomes first and vice versa


%gsize = [190 167]


LL = fread(fid,2,'float')          %read xllcorner and yllcorner
cellsize = fread(fid,1,'float');
blankvalue = fread(fid,1,'float')
numvalue = fread(fid,1,'float')
minvalue = fread(fid,1,'float')
maxvalue = fread(fid,1,'float')
sumvalue = fread(fid,1,'float');
meanvalue = fread(fid,1,'float')
stdvalue = fread(fid,1,'float');
Zgrid = fread(fid,([gsize(2),gsize(1)]),'float');
Zgrid = fliplr(Zgrid);
Zgrid = rot90(Zgrid);
Zgrid(Zgrid==blankvalue)=nan;
fclose(fid);


%image(Zgrid, 'Cdatamapping','scaled');
%imagesc(Zgrid, 'Cdatamapping','scaled');
figure(winnumber)
pcolor(Zgrid);

shading flat;
set(gca,'ydir','reverse', 'XTick', 1:50:gsize(2), 'XTickLabel',  LL(1):50*cellsize:LL(1)+cellsize*(gsize(2)-1));   %flip upside down
%TTickLabel: drawing is according to rows and columns, but labeling
%according to coordinate values
set(gca,'ydir','reverse', 'YTick', 1:50:gsize(2), 'YTickLabel',  LL(2):50*cellsize:LL(2)+cellsize*(gsize(1)-1));   %flip upside down
axis image    %to scale so that x and y axis have same equidistance between tics, i.e. no distortion of image

colorbar;