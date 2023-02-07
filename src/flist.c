#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "flist.h"
/*functions sorted alphabetically by name*/
char fn[FNLEN][5] = {"exp","cos","ln","sin"};
double (*fl[FNLEN])(double) = {exp, cos, log,sin};

char dn[FNLEN][5] = {"exp", "-sin","1/" ,"cos"};

#ifndef FLIST_H
int main() {
    return 0;
}
#endif