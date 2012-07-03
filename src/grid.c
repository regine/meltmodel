/************************************************
 * grid.c
 * This File is a part of the Hock Melt Model
 *
 *  Created on: Jul 3, 2012
 *      Author: Lyman Gillispie
 ************************************************/

#include "grid.h"

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"

extern float nodata;
extern float **griddgmglac;


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

/*******************************************************************/
/* FUNCTION freematrix.c*/
/*   gibt reservierten Speicherplatz der Gridfiles frei ******/
/*******************************************************************/

void freematrix(float **m,long nrl, long nrh, long ncl, long nch)

{
    /*Freigeben des Speichers auf die Array-Zeilen*/
    free((char *)(m[nrl]+ncl-1));

    /*Freigeben des Speichers auf das Pointer-Array*/
    free((char *)(m+nrl-1));

    return;
}


/*****************************************************************/
/* FUNCTION freematrixdouble.c*/
/*   gibt reservierten Speicherplatz der Gridfiles frei ******/
/*****************************************************************/

void freematrixdouble(double **m,long nrl, long nrh, long ncl, long nch)

{
    /*Freigeben des Speichers auf die Array-Zeilen*/
    free((char *)(m[nrl]+ncl-1));

    /*Freigeben des Speichers auf das Pointer-Array*/
    free((char *)(m+nrl-1));

    return;
}

/*****************************************************************/


/*******************************************************/
/* SET MARTIX-ARRAY TO ZERO                            */
/*******************************************************/

void initializeglacier2zero_nodata(int nrows, int ncols, float **glaciergrid) {
    assert(glaciergrid != NULL);
    assert(griddgmglac != NULL);
    int i, j;
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

void initializeglacier2zero_nodatadouble(int nrows, int ncols, double **glaciergrid) {
    assert(glaciergrid != NULL);
    assert(griddgmglac != NULL);
    int i, j;
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++) {
            if(griddgmglac[i][j] == nodata)   /*outside area to be calculated*/
                glaciergrid[i][j] = nodata;
            else
                glaciergrid[i][j] = 0;  /*must be zero because cumulative, e.g. massbalance*/
        }

    return;
}




