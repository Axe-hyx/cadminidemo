#include "operator.h"

Face *f0, *f1, *f2, *f3, *f4, *f5, *f6;
Solids *s;
Vertex v0(0, 0, 0);
Vertex v1(3, 0, 0);
Vertex v2(3, 3, 0);
Vertex v3(0, 3, 0);
Vertex v4(0, 0, 3);
Vertex v5(3, 0, 3);
Vertex v6(3, 3, 3);
Vertex v7(0, 3, 3);

Vertex v8(1, 1, 3); //4
Vertex v9(2, 1, 3);
Vertex v10(2, 2, 3);
Vertex v11(1, 2, 3);

Loop *lp0, *lp1, *lp2, *lp3, *lp4, *lp5;

void testcube() {
  mvfs(s, v0, lp0);
  f0 = s->sface;
  mev(lp0, v0, v1);
  mev(lp0, v1, v2);
  mev(lp0, v2, v3);
  mev(lp0, v0, v4);
  mef(lp0, v3, v0, f1, f0);
  mev(lp0, v3, v7);
  mef(lp0, v7, v4, f2, f1);
  mev(lp0, v1, v5);
  mef(lp0, v4, v5, f3, f2);
  mev(lp0, v2, v6);
  mef(lp0, v5, v6, f4, f3);
  mef(lp0, v7, v6, f5, f4);
}
void cases() {
  mvfs(s, v0, lp0);
  f0 = s->sface;
  mev(lp0, v0, v1);
  mev(lp0, v1, v2);
  mev(lp0, v2, v3);
  mev(lp0, v0, v4);
  mev(lp0, v3, v7);
  mev(lp0, v1, v5);
  mev(lp0, v2, v6);
  mef(lp0, v3, v0, f1, f0);
  mef(lp0, v7, v4, f2, f1);
  mef(lp0, v4, v5, f3, f2);
  mef(lp0, v5, v6, f4, f3);
  mef(lp0, v7, v6, f5, f4);
  lp5 = f5->floop;
  mev(lp5, v7, v11);
  mev(lp5, v11, v10);
  mev(lp5, v10, v9);
  mev(lp5, v9, v8);
  mef(lp5, v11, v8, f6, f5);

  /* problem in this steps
  mev(lp5, v7, v11);
  mev(lp5, v11, v10);
  mev(lp5, v10, v9);
  mev(lp5, v11, v8);
  mef(lp5, v8, v9, f6, f5);
  */
}

int main() {
  cases();
  // testcube();
  return 0;
}
