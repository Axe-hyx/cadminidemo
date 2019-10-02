#include "operator.h"
#include "cstring"
#include "struct.h"
void mvfs(Solids *&s, Vertex &v, Loop *&nloop) {
  s = new Solids();
  Face *nf = new Face();
  Loop *nl = new Loop();
  Halfedge *nhe = new Halfedge(&v, &v);
  s->sface = nf;
  nf->fsolid = s;
  nf->floop = nl;
  nl->lface = nf;

  nl->ledge = nhe;
  nhe->wloop = nl;

  nloop = nl;
}

void mkpair(Halfedge *&he1, Halfedge *&he2, Vertex &v1, Vertex &v2) {
  he1 = new Halfedge(&v1, &v2);
  he2 = new Halfedge(&v2, &v1);
  he1->next = he2;
  he1->prev = he2;
  he2->next = he1;
  he2->prev = he1;
}


// @param v1 in loop, v2 outward
void mev(Loop *l, Vertex &v1, Vertex &v2) {
  Halfedge *hep = l->getEdge();
  Halfedge *nne, *npe;
  mkpair(nne, npe, v1, v2);
  if (hep->next == hep) {
    delete hep;
    l->ledge = nne;
    l->operator<<(cout) << endl;
    return;
  }
  Halfedge *p = l->getEdge();
  while (*p->getv2() != v1) {
    p = p->next;
  }
  Halfedge *pnext = p->next;
  p->next = nne;
  nne->prev = p;
  pnext->prev = npe;
  npe->next = pnext;
  l->operator<<(cout) << endl;
}

template<typename T>
void linkafter(T *e1, T *e2) {
  e1->next = e2;
  e2->prev = e1;
}
void mef(Loop *l, Vertex &v1, Vertex &v2, Face *&nface, Face*prevface) {
  int size = 512;
  Halfedge *npe, *nne;
  mkpair(nne, npe, v1, v2);
  nface = new Face();
  Loop *nl = new Loop();
  nface->floop = nl;
  nl->lface = nface;

  int *cnt = new int[size];
  memset(cnt, 0, sizeof(int) * size);

  Halfedge *tmp = l->getEdge();
  while (*tmp->getv1() != v2) {
    tmp = tmp->next;
  }
  Halfedge *v1out = tmp;
  while (*v1out->getv1() != v1) {
    v1out = v1out->prev;
  }
  tmp = v1out;
  Halfedge *v2in = tmp;
  while (*v2in->getv2() != v2) {
    v2in = v2in->next;
  }
  Halfedge *p = v2in;
  //v1out->operator<<(cout)<<endl;;
  //v2in->operator<<(cout)<<endl;;
  while (p != v1out) { // 去除多环
    if (cnt[p->getv2()->_id] > 0) {
      Halfedge *rptr = p;
      while (rptr && rptr->getv1() != p->getv2()) {
        rptr = rptr->prev;
      }
      Halfedge *p1 = rptr->prev, *p2 = p->next;
      linkafter(p1, p2);
      Halfedge *t = p->next, *tnext;
      while (t && t->getv2() != p->getv2()) {
        cnt[t->getv2()->_id] = 0;
        t = t->next;
      }
      //p->operator<<(cout) << endl;
      //rptr->operator<<(cout) << endl;
      //t->operator<<(cout) << endl;
      tnext = t->next;
      //tnext->operator<<(cout) << endl;
      //char c = getchar();

      linkafter(p, tnext);
      linkafter(t, rptr);
      cnt[p->getv2()->_id] = 0;
    }
    ++cnt[p->getv2()->_id];
    p = p->next;
  }
  Halfedge *lastv2in;
  Halfedge *earlyv1out;
  tmp = l->ledge;
  while (*tmp->getv1() != v2) {
    tmp = tmp->next;
  }
  earlyv1out = tmp;
  while(*earlyv1out->getv1()!=v1){
    earlyv1out = earlyv1out->next;
  }
  tmp = earlyv1out;
  lastv2in = tmp;
  while (*lastv2in->getv2() != v2) {
    lastv2in = lastv2in->prev;
  }

  Halfedge *st, *ed;
  st = lastv2in->next;
  ed = earlyv1out->prev;
  linkafter(ed, nne);
  linkafter(nne,st);

  linkafter(lastv2in, npe);
  linkafter(npe, earlyv1out);
  l->ledge = npe;
  nl->ledge = nne;// 最简
  l->operator<<(cout)<<"FACE"<<endl;
  nl->operator<<(cout)<<"FACE"<<endl;
  linkafter(prevface, nface);
}

