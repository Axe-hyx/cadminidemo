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
  nf->next = nf;
  nf->prev = nf;
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
}

void mef(Loop *l, Vertex &v1, Vertex &v2, Face *&nface, Face *prevface) {
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
      tnext = t->next;

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
  while (*earlyv1out->getv1() != v1) {
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
  // earlyv1out->operator<<(cout) << endl;
  // st->operator<<(cout) << endl;
  // lastv2in->operator<<(cout) << endl;
  // ed->operator<<(cout) << endl;
  linkafter(ed, nne);
  linkafter(nne, st);

  linkafter(lastv2in, npe);
  linkafter(npe, earlyv1out);
  l->ledge = npe;
  nl->ledge = nne; // 最简
  // l->operator<<(cout) << "FACE" << endl;
  // nl->operator<<(cout) << "FACE" << endl;
  linkafter(prevface, nface);
}

// @param v1 outer loop, v2 inner loop, nloop no face
void kemr(Loop *l, Vertex &v1, Vertex &v2, Loop *&nloop, Loop *innerloop) {
  Halfedge *v1in, *v1out;
  Halfedge *v2in, *v2out;
  Halfedge *v12, *v21;
  Halfedge *p = l->ledge;
  bool st = true;
  while (st || p != l->ledge) {
    if (*p->getv1() == v1 && *p->getv2() == v2) {
      v12 = p;
    }
    if (*p->getv2() == v1 && *p->getv1() == v2) {
      v21 = p;
    }
    st = false;
    p = p->next;
  }
  v1in = v12->prev;
  v1out = v21->next;
  v2out = v12->next;
  v2in = v21->prev;
  linkafter(v1in, v1out);
  linkafter(v2in, v2out);

  nloop = new Loop();
  l->ledge = v1in;
  nloop->ledge = v2in;
  linkafter(l, nloop);
}

void kfmrh(Face *f1, Face *f2) {
  Loop *l1 = f1->getLoop(), *l2 = f2->getLoop();
  f2->prev->next = nullptr;
  while (l1->next) {
    l1 = l1->next;
  }
  linkafter(l1, l2);
  free(f2);
}

void sweep() {

}
