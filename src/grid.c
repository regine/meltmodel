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

float **matrixreserv(int nrl, int nrh, int ncl, int nch)

{
    /*nrl = number of rows lower limit, nrh = high limit, c = columns*/
    int  i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
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

double **matrixreservdouble(int nrl, int nrh, int ncl, int nch)

{
    /*nrl = number of rows lower limit, nrh = high limit, c = columns*/
    int  i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
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

void freematrix(float **m,int nrl, int nrh, int ncl, int nch)

{
    assert(m!=NULL);
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

void freematrixdouble(double **m,int nrl, int nrh, int ncl, int nch)

{
    assert(m!=NULL);
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


/****************************************************************************/
/* FUNCTION   arrayreserv                                                   */
/* reserves space for a 1d matrix t[ndl..ndh]                               */
/* from Numerical Receipes in C: vector()                                   */
/****************************************************************************/

float *arrayreserv(int ndl, int ndh)
/* allocate a float array with subscript range v[ndl..ndh] */
{
    float *v;
    int ndep=ndh-ndl+1;

    v=(float *)malloc((size_t) ((ndep+1)*sizeof(float)));
    if (!v) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }

    return v-ndl+1;
}

/****************************************************************************/
/* FUNCTION   arrayreservdouble                                             */
/* reserves space for a 1d matrix t[ndl..ndh]                               */
/* from Numerical Receipes in C: dvector()                                  */
/****************************************************************************/

double *arrayreservdouble(int ndl, int ndh)
/* allocate a double vector with subscript range v[ndl..ndh] */
{
    double *v;
    int ndep=ndh-ndl+1;

    v=(double *)malloc((size_t) ((ndep+1)*sizeof(double)));
    if (!v) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }

    return v-ndl+1;
}

/****************************************************************************/
/* FUNCTION freearray.c                                                     */
/*   gives reserved space of 1d matrix free                                 */
/*   from Numerical Receipes in C: free_vetor()                             */
/****************************************************************************/

void freearray(float *v, int ndl, int ndh)
/* free a float vector allocated with arrayreserv() */
{
    free((char *) (v+ndl-1));
    return;
}

/****************************************************************************/
/* FUNCTION freearray.c                                                     */
/*   gives reserved space of 1d matrix free                                 */
/*   from Numerical Receipes in C: free_vetor()                             */
/****************************************************************************/

void freearraydouble(double *v, int ndl, int ndh)
/* free a double vector allocated with arrayreservdouble() */
{
    free((char *) (v+ndl-1));
    return;
}


/****************************************************************************/
/* FUNCTION   tensorreserv                                                  */
/* reserves space for a 3d matrix t[nr..nrh][ncl..nch][ndl..ndh]            */
/* from Numerical Receipes in C: f3tensor()                                 */
/****************************************************************************/

float ***tensorreserv(int nrl, int nrh, int ncl, int nch, int ndl, int ndh)

{
    /*nrl = number of rows lower limit, nrh = high limit, c = columns, d = depth*/
    int  i,j, nrow=nrh-nrl+1, ncol=nch-ncl+1, ndep=ndh-ndl+1;
    float ***t;         /*GRID FILE*/

    /* allocate pointers to pointers to rows*/
    t = (float ***) malloc((size_t)((nrow+1)*sizeof(float**)));
    if (!t) {
        printf("\n ERROR in storage reservation \n\n");
        exit(1);
    }
    t +=1;
    t -=nrl;

    /* allocate pointers to rows and set pointers to them*/
    t[nrl]= (float **) malloc((size_t)((nrow*ncol+1)*sizeof(float*)));
    if (!t[nrl]) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }
    t[nrl] +=1;
    t[nrl] -= ncl;

    /* allocate rows and set pointers to them*/
    t[nrl][ncl]= (float *) malloc((size_t)((nrow*ncol*ndep+1)*sizeof(float)));
    if (!t[nrl][ncl]) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }
    t[nrl][ncl] +=1;
    t[nrl][ncl] -= ncl;

    for (j=ncl+1; j<=nch; j++) t[nrl][j]=t[nrl][j-1]+ndep;
    for (i=nrl+1; i<=nrh; i++) {
        t[i]=t[i-1]+ncol;
        t[i][ncl]=t[i-1][ncl]+ncol*ndep;
        for(j=ncl+1; j<=nch; j++) t[i][j]=t[i][j-1]+ndep;
    }

    /*return pointer to array of pointers to rows */

    return t;
}

/****************************************************************************/
/* FUNCTION   tensorreservdouble                                            */
/* reserves space for a 3d matrix t[nr..nrh][ncl..nch][ndl..ndh]            */
/* from Numerical Receipes in C: f3tensor()                                 */
/****************************************************************************/

double ***tensorreservdouble(int nrl, int nrh, int ncl, int nch, int ndl, int ndh)

{
    /*nrl = number of rows lower limit, nrh = high limit, c = columns, d = depth*/
    int  i,j, nrow=nrh-nrl+1, ncol=nch-ncl+1, ndep=ndh-ndl+1;
    double ***t;         /*GRID FILE*/

    /* allocate pointers to pointers to rows*/
    t = (double ***) malloc((size_t)((nrow+1)*sizeof(double**)));
    if (!t) {
        printf("\n ERROR in storage reservation \n\n");
        exit(1);
    }
    t +=1;
    t -=nrl;

    /* allocate pointers to rows and set pointers to them*/
    t[nrl]= (double **) malloc((size_t)((nrow*ncol+1)*sizeof(double*)));
    if (!t[nrl]) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }
    t[nrl] +=1;
    t[nrl] -= ncl;

    /* allocate rows and set pointers to them*/
    t[nrl][ncl]= (double *) malloc((size_t)((nrow*ncol*ndep+1)*sizeof(double)));
    if (!t[nrl][ncl]) {
        printf(" \n ERROR in storage reservation \n\n");
        exit(1);
    }
    t[nrl][ncl] +=1;
    t[nrl][ncl] -= ncl;

    for (j=ncl+1; j<=nch; j++) t[nrl][j]=t[nrl][j-1]+ndep;
    for (i=nrl+1; i<=nrh; i++) {
        t[i]=t[i-1]+ncol;
        t[i][ncl]=t[i-1][ncl]+ncol*ndep;
        for(j=ncl+1; j<=nch; j++) t[i][j]=t[i][j-1]+ndep;
    }

    /*return pointer to array of pointers to rows */

    return t;
}


/****************************************************************************/
/* FUNCTION freetensor.c                                                    */
/*   gives reserved space of 3d matrix free                                 */
/*   from Numerical Receipes in C: free_f3tensor()                          */
/****************************************************************************/

void freetensor(float ***t,int nrl, int nrh, int ncl, int nch, int ndl, int ndh)

{
    /*free a float f3tensor allocated by f3tensor()*/
    free((char *)(t[nrl][ncl]+ndl-1));
    free((char *)(t[nrl]+ncl-1));
    free((char *)(t+nrl-1));

    return;
}

/****************************************************************************/
/* FUNCTION freetensordouble.c                                               */
/*   gives reserved space of 3d matrix free                                 */
/*   from Numerical Receipes in C: free_f3tensor()                          */
/****************************************************************************/

void freetensordouble(double ***t,int nrl, int nrh, int ncl, int nch, int ndl, int ndh)

{
    /*free a double f3tensor allocated by f3tensor()*/
    free((char *)(t[nrl][ncl]+ndl-1));
    free((char *)(t[nrl]+ncl-1));
    free((char *)(t+nrl-1));

    return;
}

/****************************************************************************/
/* SET TENSOR-ARRAY TO ZERO                                                 */
/****************************************************************************/

void initializeglacier2zero_nodata_tens(int nrows, int ncols, int ndepths, float ***glaciergrid) {
    assert(glaciergrid != NULL);
    int i, j, k;
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++)
            for (k=1; k<=ndepths; k++) {
                if(griddgmglac[i][j] == nodata)   /*outside area to be calculated*/
                    glaciergrid[i][j][k] = nodata;
                else
                    glaciergrid[i][j][k] = 0;  /*must be zero because cumulative, e.g. massbalance*/
            }

    return;
}

/****************************************************************************/
/* SET TENSOR-ARRAY TO ZERO                                                 */
/****************************************************************************/

void initializeglacier2zero_nodata_tensdouble(int nrows, int ncols, int ndepths, double ***glaciergrid) {
    assert(glaciergrid != NULL);
    int i, j, k;
    for (i=1; i<=nrows; i++)
        for (j=1; j<=ncols; j++)
            for (k=1; k<=ndepths; k++) {
                if(griddgmglac[i][j] == nodata)   /*outside area to be calculated*/
                    glaciergrid[i][j][k] = nodata;
                else
                    glaciergrid[i][j][k] = 0;  /*must be zero because cumulative, e.g. massbalance*/
            }

    return;
}

