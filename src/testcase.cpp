#include "struct.h"
#include "operator.h"


Face *f0, *f1, *f2, *f3, *f4, *f5, *f6, *f7, *f8, *f9, *f10;
Solids *s;
Vertex v0(-10, -10, 0);
Vertex v1(-10, 10, 0);
Vertex v2(10, 10, 0);
Vertex v3(10, -10, 0);
Vertex v4(-10, -10, 20);
Vertex v5(-10, 10, 20);
Vertex v6(10, 10, 20);
Vertex v7(10, -10, 20);

Vertex v8(-5, -5, 20); // 4
Vertex v9(-5, 5, 20);
Vertex v10(5, 5, 20);
Vertex v11(5, -5, 20);

Vertex v12(-5, -5, 0);
Vertex v13(-5, 5, 0);
Vertex v14(5, 5, 0);
Vertex v15(5, -5, 0);

Vertex v20(-10, -20, 0);
Vertex v21(10, -20, 0);
Vertex v22(10, 20, 0);
Vertex v23(-10, 20, 0);
Vertex v24(-5, -15, 0);
Vertex v25(5, -15, 0);
Vertex v26(5, -5, 0);
Vertex v27(-5, -5, 0);

Vertex v28(-5, 5, 0);
Vertex v29(5, 5, 0);
Vertex v30(5, 15, 0);
Vertex v31(-5, 15, 0);

Loop *lp0, *lp1, *lp2, *lp3, *lp4, *lp5, *lp6, *lp7;

void test_2dcube() {
  mvfs(s, v0, lp0);
  f0 = s->sface;
  mev(lp0, v0, v1);
  mev(lp0, v1, v2);
  mev(lp0, v2, v3);
  mef(lp0, v3, v0, f1, f0);
  mev(lp0, v0, v8);
  mev(lp0, v8, v5);
  mev(lp0, v5, v6);
  mev(lp0, v6, v7);
  cout << s;
  return;

  mev(lp0, v3, v7);
  mef(lp0, v7, v4, f2, f1);
  mev(lp0, v1, v5);
  mef(lp0, v4, v5, f3, f2);
  mev(lp0, v2, v6);
  mef(lp0, v5, v6, f4, f3);
  mef(lp0, v7, v6, f5, f4);
}

void construct_cube() {
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
  // TODO: fix face problem, wrong f6 face here
  mef(lp5, v8, v11, f6, f5);
  lp7 = f6->floop;
  /* problem in this steps
  mev(lp5, v7, v11);
  mev(lp5, v11, v10);
  mev(lp5, v10, v9);
  mev(lp5, v11, v8);
  mef(lp5, v8, v9, f6, f5);
  */
  kemr(lp5, v7, v11, lp6, lp5);

  mev(lp7, v8, v12);
  mev(lp7, v9, v13);
  mev(lp7, v10, v14);
  mev(lp7, v11, v15);
  mef(lp7, v15, v12, f7, f6);
  mef(lp7, v12, v13, f8, f7);
  mef(lp7, v13, v14, f9, f8);
  mef(lp7, v14, v15, f10, f9);
  kfmrh(f1, f6);
  linkafter(f5, f7);
  return;
}
void construct_sweep() {

  mvfs(s, v0, lp0);
  f0 = s->sface;
  mev(lp0, v0, v1);
  mev(lp0, v1, v2);
  mev(lp0, v2, v3);
  mef(lp0, v3, v0, f1, f0);
  mev(lp0, v3, v15);
  mev(lp0, v15, v14);
  mev(lp0, v14, v13);
  mev(lp0, v13, v12);
  mef(lp0, v12, v15, f2, f1);
  kemr(lp0, v3, v15, lp1, lp0);
  join(f2, f1);
  sweep(f1, 1, Vertex(0, 0, 20));
}
void construct_qube() {
  mvfs(s, v0, lp0);
  f0 = s->sface;
  mev(lp0, v0, v1);
  mev(lp0, v1, v2);
  mev(lp0, v2, v3);
  mef(lp0, v3, v0, f1, f0);
}
void multiple_hole_sweep() {

  mvfs(s, v20, lp0);
  f0 = s->sface;
  mev(lp0, v20, v21);
  mev(lp0, v21, v22);
  mev(lp0, v22, v23);
  mef(lp0, v23, v20, f1, f0);

  mev(lp0, v20, v24);

  mev(lp0, v24, v25);
  mev(lp0, v25, v26);
  mev(lp0, v26, v27);
  mef(lp0, v24, v27, f2, f1);

  kemr(lp0, v20, v24, lp1, lp0);
  join(f2, f1);

  mev(lp0, v23, v31);
  mev(lp0, v31, v28);
  mev(lp0, v28, v29);
  mev(lp0, v29, v30);
  mef(lp0, v30, v31, f2, f1);
  join(f2, f1);
  kemr(lp0, v23, v31, lp5, lp1);
  sweep(f1, 1, Vertex(0, 0, 20));
}
