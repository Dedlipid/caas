#ifndef PARSER_H
#define PARSER_H
typedef struct node {
    char sym;
    struct node *p, *l, *r;
    int fs, ds, size;
    char f[5], d[5],fnc[150], drv[500];
   } node;
int isbin(char);
void nkill(node*);
node *mp(char*);
#endif
