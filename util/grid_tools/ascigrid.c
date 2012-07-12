#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#include <math.h>

long double statanz,statmini,statmaxi,sx,sx2;
float stat_nodata;

void clearstat(float nodata) {
    stat_nodata = nodata;
    statanz=0;
    sx=0;
    sx2=0;
}

void addstat(float x) {
    if (x>(stat_nodata+1)) {
        statanz=statanz+1;
        sx=sx+x;
        sx2=sx2+x*x;
        if (statanz==1) {
            statmini=x;
            statmaxi=x;
        } else if (x<statmini) statmini=x;
        else if (x>statmaxi) statmaxi=x;
    }
}

void statistics(float *anzahl,float *mini,float *maxi,float *summe,float *mittel,float *std) {
    *anzahl=(float)statanz;
    *mini=(float)statmini;
    *maxi=(float)statmaxi;
    *summe=(float)sx;
    if (*anzahl>0) *mittel=(float)( sx / *anzahl);
    else *mittel=-9999.0;
    if (*anzahl>2) *std   =(float)( sqrt((sx2-sx*sx / *anzahl)/(*anzahl-1)));
    else *std=-9999.0;
}


FILE *dateiaus;
char *dateinameein,*dateinameaus;
FILE *dateiein;
float x[13];
int    i,j,k;
char   taste;
char   *satz;
char   *teil;
int groesse;
int ncols,nrows;
float xll,yll,cs,nodata;

int main(int anz,char *paramstring[]) {
    satz         = (char *)calloc(1,300);
    teil         = (char *)calloc(1,12);
    dateinameein = (char *)calloc(1,255);
    dateinameaus = (char *)calloc(1,255);



    printf("Umwandeln einer Ascii-Datei in ein Festformat\r\n");
    if (anz<2) {
        printf("\n");
        printf("usage: ascigrid <Ascii-Datei> <festformatdatei>\r\n");
        printf("\n");
        printf(" Ascii-Datei: Dateikopf mit : \r\n");
        printf(" ncols,nrows,xll,yll,cs,nodata jeweils ein Wert pro Zeile\r\n");
        printf(" alle weiteren Werte von links oben nach rechts unten zeilenweise\r\n");
        printf("\n");
        printf("Eingabedatei : ");
        dateinameein=gets(dateinameein);
        printf("\r\nAusgabedatei : ");
        dateinameaus=gets(dateinameaus);
        printf("\r\n");
    } else {
        if (paramstring[1]!=NULL) strcpy(dateinameein,paramstring[1]);
        if (anz>1) if (paramstring[2]!=NULL) strcpy(dateinameaus,paramstring[2]);
    }

    while ((dateiein=fopen((char *)dateinameein,"r"))==NULL) {

        printf("Kann Eingabedatei %s nicht finden bzw ÷ffnen!\r\n",dateinameein);
        printf("Bitte neuen Namen angeben : ");
        dateinameein=gets((char *)dateinameein);
        if (*(char *)dateinameein=='\0') exit(1);
        printf("\r\n");
    }
    while ((dateiaus=fopen(dateinameaus,"wb"))==NULL) {
        printf("Kann Ausgabedatei %s nicht finden bzw ÷ffnen!\r\n",dateinameaus);
        printf("Bitte neuen Namen angeben : ");
        dateinameaus=gets(dateinameaus);
        if (*dateinameaus=='\0') exit(1);
        printf("\r\n");

    }


    fscanf(dateiein,"%12s%d",teil,&ncols);
    fscanf(dateiein,"%12s%d",teil,&nrows);
    fscanf(dateiein,"%12s%f",teil,&xll);
    fscanf(dateiein,"%12s%f",teil,&yll);
    fscanf(dateiein,"%12s%f",teil,&cs);
    fscanf(dateiein,"%12s%f",teil,&nodata);

    x[1]=ncols;
    x[2]=nrows;
    x[3]=xll;
    x[4]=yll;
    x[5]=cs;
    x[6]=nodata;

    groesse=sizeof(x[1]);
    clearstat(nodata);
    fwrite(&x[1],sizeof(float),12,dateiaus);
    for (j=1; j<=nrows; j++) {
        for (i=1; i<=ncols; i++) {
            fscanf(dateiein,"%f",&x[1]);
            fwrite(&x[1],groesse,1,dateiaus);
            addstat(x[1]);
        }
        fgets(satz,300,dateiein);
        printf(".");
    }

    printf("\n");

    statistics(&x[6],&x[7],&x[8],&x[9],&x[10],&x[11]);

    fseek(dateiaus,6*sizeof(float),0);
    fwrite(&x[6],sizeof(float),6,dateiaus);

    fclose(dateiein);
    fclose(dateiaus);

    free(satz);
    free(teil);

    free(dateinameein);
    free(dateinameaus);


}
