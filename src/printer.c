#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "printer.h"

void printparse(node *p) {
     printf("|fnc: %s|drv: %s|\n", p->fnc, p->drv);
     printf("|sym: %c|f: %s|size: %d|\n", 
             p->sym, p->f, p->size);
     if (p->l)
         printparse(p->l);
     if (p->r)
         printparse(p->r);
}
//eventually size and gethf should be merged 
//using realloc and shifting the string pointer
static int size(node *np) {
    int s = 0;
    if (np->sym){
        s += size(np->l);
        s += ((isbin(np->sym) < isbin(np->l->sym))) ? 2 : 0;
        s += size(np->r)+1;
        s += ((isbin(np->sym) < isbin(np->r->sym))) ? 2 : 0;   
    } else if (np->fs) {
        s += strlen(np->f) + 2 + size(np->l); 
    } else 
        s += strlen(np->fnc);
    np->size =  s;
    return s;
}


static void getfh(node *np, char *s) {
    if (np->sym) { 
        int i = (isbin(np->sym) < isbin(np->l->sym)) ? 2 : 0;
        if (i)
           s[0] = '(';
        getfh(np->l, s + i/2);
        if (i)
            strcat(s,")");
        s[np->l->size+i] = np->sym;
        int j = (isbin(np->sym) < isbin(np->r->sym)) ? 2 : 0;
        if (j)
            strcat(s,"(");
        getfh(np->r, s + 1 + np->l->size + i + j/2 );
        if (j)
            strcat(s,")");
    } else if (np->fs){
        strcpy(s, np->f);
        int len = strlen(np->f);
        strcat(s,"(");
        getfh(np->l, s + len + 1);
        strcat(s,")");
    } else {
        strcat(s, np->fnc);
    }
}


char *getf(node *np) {
    int len = size(np);
    char *s = malloc(len+1);
    s = memset(s, 0, len + 1);
    size(np);
    getfh(np, s);
    return s;
}

#ifndef PRINTER_H
int main(){
    printf("fncm.h\n");
    exit(0);
}
#endif
