/************************************************
 * grid.c
 * This File is a part of the Hock Melt Model
 *
 *  Created on: Jul 3, 2012
 *      Author: Lyman Gillispie
 ************************************************/

#include "grid.h"


/*============================================================*/
/**************************************************************/
/* FUNCTION   matrixreserv                                    */
/* reserviert Speicher fuer eine float Matrix m[nr..nrh][ncl..nch]  */
/*   aus Numerical Receipes in C, page 944                    */
/**************************************************************/

float **matrixreserv(long nrl, long nrh, long ncl, long nch)

{
    /*nrl = number of rows lower limit, nrh = high limit, c = columns*/
    long  i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
    float **m;         /*GRID FILE*/

    /*Speicher fuer die Zeiger auf die einzelnen Zeilen reservieren*/

    m = (float **)calloc((size_t)(nrow+1),sizeof(float*));
    if (!m) {
        printf("\n ERROR in storage reservation \n\n");
        exit(1);
    }
    m+=1;
    m-=nrl;

    /*Speicher fuer die einzelnen Zeilen reservieren und Zeiger auf sie setzen*/

    m[nrl]= (float *)calloc(nrow*ncol+1,sizeof(float));
    if (!m[nrl]) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }
    m[nrl] +=1;
    m[nrl] -= ncl;

    /*Zuweisen der Adressen der Zeilenzeiger mit dem Abstand von je ncol*sizeof(float) */

    for (i=nrl+1; i<=nrh; i++) m[i]=m[i-1]+ncol;

    return m;
}
/***************END SPEICHER RESERVIEREN  FUER MATRIX ********************/



/*============================================================*/
/**************************************************************/
/* FUNCTION   matrixreservdouble                              */
/* reserviert Speicher fuer eine Matrix m[nr..nrh][ncl..nch]  */
/*   fuer DOUBLE :  aus Numerical Receipes in C               */
/**************************************************************/

double **matrixreservdouble(long nrl, long nrh, long ncl, long nch)

{
    /*nrl = number of rows lower limit, nrh = high limit, c = columns*/
    long  i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
    double **m;         /*GRID FILE*/

    /*Speicher fuer die Zeiger auf die einzelnen Zeilen reservieren*/

    m = (double **)calloc((size_t)(nrow+1),sizeof(double*));
    if (!m) {
        printf("\n ERROR in storage reservation \n\n");
        exit(1);
    }
    m+=1;
    m-=nrl;

    /*Speicher fuer die einzelnen Zeilen reservieren und Zeiger auf sie setzen*/

    m[nrl]= (double *)calloc(nrow*ncol+1,sizeof(double));
    if (!m[nrl]) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }
    m[nrl] +=1;
    m[nrl] -= ncl;

    /*Zuweisen der Adressen der Zeilenzeiger mit dem Abstand von je ncol*sizeof(double) */

    for (i=nrl+1; i<=nrh; i++) m[i]=m[i-1]+ncol;

    return m;
}
/***************END SPEICHER RESERVIEREN  FUER MATRIX ********************/

/*******************************************************/
/* SET MARTIX-ARRAY TO ZERO                            */
/*******************************************************/

void initializeglacier2zero_nodata(float **glaciergrid) {
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if(griddgmglac[i][j] == nodata)   /*outside area to be calculated*/
                glaciergrid[i][j] = nodata;
            else
                glaciergrid[i][j] = 0;  /*must be zero because cumulative, e.g. massbalance*/
        }

    return;
}


/*******************************************************/
/* SET DOUBLE MARTIX-ARRAY TO ZERO                            */
/*******************************************************/

void initializeglacier2zero_nodatadouble(double **glaciergrid) {
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if(griddgmglac[i][j] == nodata)   /*outside area to be calculated*/
                glaciergrid[i][j] = nodata;
            else
                glaciergrid[i][j] = 0;  /*must be zero because cumulative, e.g. massbalance*/
        }

    return;
}

void checkgridinputdata_ok() {
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if((griddgmglac[i][j] != nodata) && (griddgmdrain[i][j] == nodata)) {
                printf("\nERROR in drainage basin / glacier input grid data\n");
                printf(" Not all glacier grid cells are part of the drainage basin\n");
                printf(" adjust your glacier grid so that the entire glacier is part of the drainage basin grid\n\n");
                fclose(outcontrol);
                exit(3);
            }
            /*CURRENTLY SNOW MODEL CAN ONLY BE RUN FOR THE GLACIER AND NOT FOR AREA OUTSIDE*/
            if((methodsurftempglac == 4) && ((griddgmglac[i][j] == nodata) && (griddgmdrain[i][j] != nodata))) {
                printf("\nERROR in drainage basin / glacier input grid data\n");
                printf(" snow model is run: in this case glacier and drainage basin grids must be same\n");
                printf(" copy glacier grid under a new name into drainage grid\n\n");
                fclose(outcontrol);
                exit(3);
            }
        }  /*endfor*/
    return;
}


