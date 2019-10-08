#include "struct.h"
void mvfs(Solids *&s, Vertex &v, Loop *&nloop);

void mev(Loop *l, Vertex &v1, Vertex &v2);

void mef(Loop *l, Vertex &v1, Vertex &v2, Face *&nf, Face *prevface);

void kemr(Loop *l, Vertex &v1, Vertex &v2, Loop *&nloop, Loop *preloop);

void kfmrh(Face *f1, Face *f2);
