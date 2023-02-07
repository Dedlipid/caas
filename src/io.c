#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "eval.h"
#include "drv.h"
#include "printer.h"

static void help() {
    puts(">HELP:");
    puts(">>>f=EXR to add a function, has to be less then 150 chars");
    puts(">>>Dnf to get the nth derivative of an inputed function");
    puts(">>>will misbehave if input is bad or runs out of memeory, name should be <5 chars");
}

int ulen = 0;
char uls[10][5];
node *nls[10];

static int checkf(char *s, int len) {
    int i = 0;
    for (;i < ulen && strncmp(s, uls[i],len);i++);
    return i;
}


static void addf(char *s, int len) {
    int i = 0;
    for (;i < ulen && strncmp(s,uls[i],len-1);i++);
    if (checkf(s, len - 1) < ulen) {
        puts(">ADDF:");
        printf(">>>function is in list, use another name\n");
    } else {
        node *np = mp(s+len);
        nls[ulen] = np;
        strncpy(uls[ulen++], s, len - 1);
    }
}


static void fcalc(char *s, int len) {
    puts(">CALC");
    int i;
    if ((i = checkf(s, len)) < ulen) {
    double x;
    puts(">>>enter eval point");
    scanf("%lf",&x);
    printf(">>%g\n", evl(nls[i],x));
    /*printparse(nls[i]);*/
    } else {
        puts(">>>no such function yet");
    }
}

static void dcalc(char *s, int len) {
    puts(s);
    int i;
    if ((i = checkf(s, len)) < ulen) {
        getd(nls[i]);
        printf(">>:");
        puts(nls[i]->fnc);
    } else {
        char *t = s+1;
        if ((i = checkf(t, len -1)) < ulen) {
            nls[ulen] = getd(nls[i]);
            strncpy(uls[ulen++], s, len);
            puts(nls[ulen-1]->fnc);
        } else {
            puts("no such function to take a derivative of");
        }
    }
}

int main() {
    printf(">>>Enter inputs of the form\
\n>>>f=<EXPR>\n>>>D and E keyword is reserved\n>>>for help enter HELP\n");
    char buf[200] ;
    while (fgets(buf, 199, stdin) != NULL) {
        char *s = strchr(buf, '\n');
        *s = '\0';
        int len = strlen(buf);
        if (!strcmp(buf, "HELP")) {
            help();
        } else if (buf[0] == 'D') {
            dcalc(buf, len);
        } else if (buf[0] == 'E') {
            fcalc(buf + 1, len);
        } else if(len && (s = strchr(buf, '='))){
            if (s++) {
                int len = s - buf;
                while(*s)
                    if (*s++ != ' ') {
                        addf(buf, len);
                        break;
                    }
            }
        }
    }

    exit(0);
}
