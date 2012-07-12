/* program gridasci */
/* konvertiert Grids aus Direktzugrifssdatei fuer Pascal in asci-Format (single) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


FILE *dateiaus;
char *dateinameein,*dateinameaus;
FILE *dateiein;
float  x[13];
double temp[10];
int    i,j,k;
char   taste;
char   *satz;
char   *teil;
int groesse;
char   digits;
int    s,d;
char   *param;
char   *frmt;
int    ncols,nrows;
float  xll,yll,cs,nodata;


int main(int argc,char *paramstring[]) {
    satz         = (char *)calloc(1,FILENAME_MAX);
    teil         = (char *)calloc(1,12);
    dateinameein = (char *)calloc(1,FILENAME_MAX);
    dateinameaus = (char *)calloc(1,FILENAME_MAX);
    param        = (char *)calloc(1,20);
    frmt         = (char *)calloc(1,30);

    printf("Umwandeln einer Ascii-Datei in ein Festformat, entspricht asciigrid\n");
    if (argc<3) {
        printf("\n");
        printf("usage: gridasci <festformatdatei> <Ascii-Datei> [<dezimalstellen>]\n");
        printf("\n");
        printf(" Ascii-Datei: Format wie bei gridascii aus ARC/Info, alsoDateikopf mit:\n");
        printf(" ncols,nrows,xll,yll,cs,nodata eweils ein Wert pro Zeile\n");
        printf("\nEingabedatei : ");
        dateinameein=fgets(dateinameein, FILENAME_MAX, stdin);
        printf("\nAusgabedatei : ");
        dateinameaus=fgets(dateinameaus, FILENAME_MAX, stdin);
        printf("\n");
    } else {
        if (paramstring[1]!=NULL) strcpy(dateinameein,paramstring[1]);
        if (argc>1) if (paramstring[2]!=NULL) strcpy(dateinameaus,paramstring[2]);
    }

    while ((dateiein=fopen(dateinameein,"rb"))==NULL) {
        printf("Kann Eingabedatei %s nicht finden bzw ÷ffnen!\r\n",dateinameein);
        printf("Bitte neuen Namen angeben : ");
        dateinameein=fgets(dateinameein, FILENAME_MAX, stdin);
        if (dateinameein[0]=='\0') exit(1);
        printf("\r\n");
    }
    while ((dateiaus=fopen(dateinameaus,"w"))==NULL) {
        printf("Kann Ausgabedatei %s nicht finden bzw ÷ffnen!\r\n",dateinameaus);
        printf("Bitte neuen Namen angeben : ");
        dateinameaus=fgets(dateinameaus, FILENAME_MAX, stdin);
        if (dateinameaus[0]=='\0') exit(1);
        printf("\r\n");
    }


    param[0] = '0';
    if (argc == 4) strcpy(param,paramstring[3]);
    digits = param[0];
    d = 0;
    s = 6;
    switch (digits) {
    case '0':
        sprintf(frmt,"%%6.0f ");
        break;
    case '1':
        sprintf(frmt,"%%6.1f ");
        break;
    case '2':
        sprintf(frmt,"%%6.2f ");
        break;
    case '3':
        sprintf(frmt,"%%6.3f ");
        break;
    case '4':
        sprintf(frmt,"%%6.4f ");
        break;
    case '5':
        sprintf(frmt,"%%7.5f ");
        break;
    case '6':
        sprintf(frmt,"%%8.6f ");
        break;
    case '7':
        sprintf(frmt,"%%9.7f ");
        break;
    case '8':
        sprintf(frmt,"%%10.8f ");
        break;
    case '9':
        sprintf(frmt,"%%10.8g ");
        break;
    default :
        sprintf(frmt,"%%6.2f ");
    }

    groesse = sizeof(x[1]);

    printf("Groesse eines Records: %d\n",groesse);

    for (i=1; i<=12; i++) fread(&x[i],groesse,1,dateiein);

    ncols  = (int)x[1];
    nrows  = (int)x[2];
    nodata = x[6];

    fprintf(dateiaus,"ncols        %10.0f\n",x[1]);
    fprintf(dateiaus,"nrows        %10.0f\n",x[2]);
    fprintf(dateiaus,"xllcorner    %10.3f\n",x[3]);
    fprintf(dateiaus,"yllcorner    %10.3f\n",x[4]);
    fprintf(dateiaus,"cellsize     %10.4f\n",x[5]);
    fprintf(dateiaus,"nodata_value %10.4f\n",x[6]);


    for (j=1; j<=nrows; j++) {
        for (i=1; i<=ncols; i++) {
            fread(&x[1],groesse,1,dateiein);
            fprintf(dateiaus,frmt,x[1]);
        }
        fprintf(dateiaus,"\n");
        printf(".");
    }
    printf("\n");



    fclose(dateiein);
    fclose(dateiaus);
    free(param);
    free(dateinameein);
    free(dateinameaus);
    free(satz);
    free(teil);
    free(frmt);

    return 0;
}

