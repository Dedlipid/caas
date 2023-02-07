#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"
#include "flist.h"
#include "eval.h"

char *var = "x";


double evl(node *np, double x) {
    double s = 0; 
    if (np->fnc[0] == '-')
        x *= -1;
    int i = 0;
        switch(np->sym) {
            case '+':
                s += evl(np->l, x) + evl(np->r, x);
                break;
            case '-':
                s += evl(np->l, x) - evl(np->r, x);
                break;
            case '*':
                s += evl(np->l, x) * evl(np->r, x);
                break;
            case '/':
                s += evl(np->l, x) / evl(np->r, x);
                break;
            case '^':
                s += pow(evl(np->l, x), evl(np->r, x));
                break;
            default:
                if (np->fs) {
                    while(i < FNLEN && strcmp(np->f,fn[i]))
                        i++;
                    s += fl[i](evl(np->l, x));
                } else {
                    if (!strcmp(np->fnc+i,"x")) {
                        s += x;   
                    } else {
                        s += atof(np->fnc);
                    }
                }
        }
    return s;
}

#ifndef EVAL_H
int main(){
    node *np = mp("sin(-x)^2/cos(x)^2");
    printf("%g\n", evl(np, 3.141595/4));
}
#endif
