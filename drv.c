#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "flist.h"
#include "drv.h"

/*ideally will eventually be made to use malloc*/
static int size(node *np) {
    int sum = 0, R = 0, L = 0;   
    if (np->sym) {
        L = size(np->l);
        R = size(np->r);
        switch (np->sym) {
            case '+':
                if (L) 
                    strcpy(np->drv, np->l->drv);
                if (L && R)
                    strcat(np->drv, "+");
                if (R)
                    strcat(np->drv, np->r->drv);

                break;
            case '-':
                if (L) 
                    strcpy(np->drv, np->l->drv);
                if (R) {
                    if (L)
                        strcat(np->drv, "-(");
                    else 
                        strcpy(np->drv,"0-(");
                    strcat(np->drv, np->r->drv);
                    strcat(np->drv, ")");
                }
                break;
            case '*':
                if (L) {
                    strcpy(np->drv,"(");
                    strcat(np->drv, np->l->drv);
                    strcat(np->drv,")*(");
                    strcat(np->drv, np->r->fnc);
                    strcat(np->drv, ")");
                }
                if (L && R) { 
                    strcat(np->drv, "+(");
                    strcat(np->drv, np->l->fnc);
                    strcat(np->drv, ")*(");
                    strcat(np->drv, np->r->drv);
                    strcat(np->drv, ")");
                }
                if (R && !L) {
                    strcpy(np->drv,"(");
                    strcat(np->drv, np->l->fnc);
                    strcat(np->drv,")*(");
                    strcat(np->drv, np->r->drv);
                    strcat(np->drv, ")");
                }
                break;
            case '/':
                if (L || R)
                    strcpy(np->drv, "(");
                if (L) {
                    strcat(np->drv,"(");
                    strcat(np->drv, np->l->drv);
                    strcat(np->drv,")*(");
                    strcat(np->drv, np->r->fnc);
                    strcat(np->drv, ")");
                }
                if (L && R) { 
                    strcat(np->drv, "-(");
                    strcat(np->drv, np->l->fnc);
                    strcat(np->drv, ")*(");
                    strcat(np->drv, np->r->drv);
                    strcat(np->drv, ")");
                }
                if (R && !L) {
                    strcpy(np->drv,"(0-(");
                    strcat(np->drv, np->l->fnc);
                    strcat(np->drv,")*(");
                    strcat(np->drv, np->r->drv);
                    strcat(np->drv, ")");
                }
                if (R || L) {
                    strcat(np->drv,")/(");
                    strcat(np->drv, np->r->fnc);
                    strcat(np->drv, ")^2");
                }
                break;            
            case '^':
                /* (L)^(R-1)*(R)+(L)^(R)*ln(L) */
                if (L || R) {
                    char  *t, *s;
                    if (L) { 
                        strcpy(np->drv, "(");
                        strcat(np->drv, np->l->fnc);
                        strcat(np->drv, ")^(");
                        strcat(np->drv, np->r->fnc);
                        strcat(np->drv,"-1)*(");
                        strcat(np->drv, np->r->fnc);
                        strcat(np->drv, ")");
                        if (R) 
                            strcat(np->drv, "+");
                    }
                    if (R) { 
                        if (L)
                            strcat(np->drv, "(");
                        else
                            strcpy(np->drv, "(");
                        strcat(np->drv, np->l->fnc);
                        strcat(np->drv, ")^(");
                        strcat(np->drv, np->r->fnc);
                        strcat(np->drv,")*ln(");
                        strcat(np->drv, np->l->fnc);
                        strcat(np->drv, ")");
                    }
                }
                break;
            default:
                ;
        }
    } else if (np->fs) {
        int s = 0;
        if ((s = size(np->l))) {
            strcpy(np->drv, np->d);
            strcat(np->drv, "(");
            strcat(np->drv, np->l->fnc);
            strcat(np->drv,")*(");
            strcat(np->drv, np->l->drv);
            strcat(np->drv, ")");
        };  /* ()x2 + '*' */
    } else {
        if (np->fnc[0] == '-')
            L++;
        sum += (strcmp(np->fnc+L,"x")) ? 0: 1+L;
        if (sum) 
            strcpy(np->drv,"1");
    }
    return (np->size = strlen(np->drv));
}


static void setd(node *np) {
    int i = 0;
    if (np->sym) {
        setd(np->l);
        setd(np->r);
    } else if (np->fs) {
        while(i < FNLEN && strcmp(np->f, fn[i]))
                i++;
        if (i == FNLEN) {
            return;
        }
        strcpy(np->d, dn[i]);
        setd(np->l);
    } else {
        if (!strcmp(np->fnc,"x"))
            strcpy(np->d,"1");
        else
            strcpy(np->d,"0");
    }
}


node *getd(node *np) {
    setd(np);
    size(np);
    node *dp = mp(np->drv);
    return dp;
}
#ifndef DRV_H
int main() {
    exit(0);
}
#endif
