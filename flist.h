#ifndef FLIST_H
#define FLIST_H

#include <math.h>
#include "parser.h"

#define FNLEN 4
#define USRLST 5

extern char fn[FNLEN][5]; 
extern double (*fl[FNLEN])(double);

extern char dn[FNLEN][5];
extern double (*dl[FNLEN])(double); 


extern char un[USRLST][5];
extern node ul[USRLST][sizeof(node)];

#endif
