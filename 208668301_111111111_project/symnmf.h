#ifndef SYM_NMF_H
#define SYM_NMF_H


#include <stdio.h>
#include <stdlib.h>

double** sym(double** X, int d, int n);

double** ddg(double** X, int d, int n);

double** norm(double** X, int d, int n);

double** symnmf(double** H,double** W, int k, int n);

#endif 