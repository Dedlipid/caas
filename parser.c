#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define BINLEN 13  /* must be odd */


char bin[BINLEN] = "+-*/^"; /*5 binlen: 13*/

static node *intnd (char *f, int len, struct node *par) {
    node *np = malloc(sizeof(struct node));
    np->sym = 0;
    np->fnc[0] = '\0';  
    strncpy(np->fnc, f, len);
    np->l = NULL;
    np->r = NULL;
    np->p = par;
    np->fs = 0;
    np->size = 0;
    return np;
}


int isbin(char c) {
    for (int i = 0; i < 5; i++ ) {
        if (bin[i] == c)
            return (BINLEN-i);
    }
    return 0;
}


static int **parseinit(const char *s,int len) {
    int isbin(char s);
    int **N;
    if ((N = malloc(len *  sizeof (int*))) == NULL) {
        exit (1);
    }
    int n = 0;
    for (int i = 0; i < len; i++) {
        N[i] =  malloc(2*sizeof(int));
        if (N[i] == NULL) {
            exit(1);
        }
        N[i][0] = n;
        N[i][1] = isbin(s[i]);
        if (s[i] == '(')
            N[i][0] = n++;
        if (s[i] == ')')
            N[i][0] = --n;
    }
    return N;
}


static int ffix(int *l, int len, char *s, int **n) {
    int min = -1, cmin = -1, bmin = -1, 
        imin = 0, pmax = 0;
    for (int i = 0; i < len; i++) {
        if (n[i][1] && !n[imin][1]) { 
            min = n[i][0];
            pmax = n[i][1];
            imin = i;
        } else if ((n[i][0] < min && n[i][1]) || (n[i][0] == min && n[i][1] >= pmax)) {
            min = n[i][0];
            pmax = n[i][1];
            imin = i;
        }
        if (!(s[i] == '(' || s[i] == ')') && (n[i][0] < cmin || (cmin < 0))) 
            cmin = n[i][0];
        if ((s[i] == '(') && (n[i][0] < bmin || (bmin < 0)))
            bmin = n[i][0];
    }    
    int fix = 0; 
    if (bmin >= 0) {
        if (min < 0) {
            fix = cmin ;
        } else {
            fix = min;
            if (fix > cmin)
                fix = cmin;
        }
        fix -= bmin;
    }

    l[0] = imin - fix; l[1] = min - fix ; 
    l[2] = bmin ; l[3] = cmin - fix;
    return fix;
}
// TODO:make the things stored non fixed in the struct and malloc as accessed, nkill will 
// have to be adjusted accordingly
// make sure it fucking errors out right
static void part(int **n, int len, char *s, struct node *np) {
    struct node *intnd(char *s, int len, struct node *p);
    int l[4];
    int fix = ffix(l, len, s, n);
    int imin = l[0]; int min = l[1]; 
    int bmin = l[2]; int cmin = l[3];
    if (fix) {;
        s += fix;
        len -= 2 * fix;
        s[len] = '\0';
        strcpy(np->fnc, s);
    }
    struct node *L;
    if (min >= 0 && min <= cmin) {
        struct node *R;
        np -> sym = s[imin]; 
        s[imin] = '\0';
        L = intnd(s, imin, np); 
        R = intnd(s + imin + 1, len - imin -1, np);
        np->l = L; 
        np->r = R; 
        part(n + fix, imin, s, L); 
        part(n +fix + imin + 1, len - imin - 1, s + imin + 1, R);
    } else if ((min >= 0 || bmin >= 0)) {
        int i = 0;
        s[len - 1] = '\0';
        while (i < len && s[i++] != '(');
        if (i < len) {
            strncpy(np->f, s, i - 1);
            np->fs = 1;
            L = intnd(s + i, len - i - 1, np);
            np->l = L;
            part(n + fix + i, len - i - 1, s + i, L);
        }
    } else {
        ;
    }
}


static void clean(char *s,char *t, int len) {
    while (len--)
        if (*s++ != ' ')
            *t++ = *(s-1);
    *t = '\0';
}
void clean2(char *s) {
    char *t = s;
    enum {
        igmin,
        igpls,
        cpy
    } state = cpy;
    while (*t == '+' || *t == '-') {
        ;
    }
    while (*t && *(t+1)) {
        if (*t == '+'){
            state = igpls;
            
        } else if (*t == '-') {
            state = igmin;
        } else {
            state = cpy;
        }
        t++;
    }
}

void nkill(struct node *top) {
    if (top->l)
        free(top->l);
    if (top->r)
        free(top->r);
    free(top);
}


node *mp(char *s) {
    int len = strlen(s);
    char t[200]; clean(s, t, len);
    struct node *top = intnd(t, strlen(t), NULL);
    len = strlen(t);
    int **n = parseinit(s, strlen(t));
    part(n, strlen(t), t, top);
    if (len)
        while(len)
            free(n[--len]);
    free(n);
    return top;
}
#ifndef PARSER_H 
int main() {
    char *s = "       1    -     x      ";
    int len = strlen(s);
    char t[200]; clean(s, t, len);
    puts(t);
    exit(0);
}
#endif
