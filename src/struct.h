#ifndef STRUCTH
#define STRUCTH
class Solids;
class Face;
class Loop;
class Halfedge;
class Vertex;
#include <iostream>
using namespace std;
class Vertex {
public:
  float v[3];
  Vertex *next, *prev;
  int _id;

  Vertex(float _x, float _y, float _z) : next(nullptr), prev(nullptr) {
    static int i = 0;
    v[0] = _x;
    v[1] = _y;
    v[2] = _z;
    _id = ++i;
  }
  Vertex(const Vertex &ve) : prev(ve.prev), next(ve.next) {
    v[0] = ve.v[0];
    v[1] = ve.v[1];
    v[2] = ve.v[2];
  }
  float operator[](int i) { return v[i]; }
  float x() { return v[0]; }
  float y() { return v[1]; }
  float z() { return v[2]; }
  Vertex *getNext() { return next; }
  Vertex *getPrev() { return prev; }

  bool operator==(const Vertex &vr) {
    if (vr.v[0] == v[0] && vr.v[1] == v[1] && vr.v[2] == v[2])
      return true;
    return false;
  }
  bool operator!=(const Vertex &vr) {
    if (operator==(vr))
      return false;
    return true;
  }
  ostream &operator<<(ostream &o) {
    o << _id-1;
    // o << "(" << v[0] << "," << v[1] << "," << v[2] << ")";
    return o;
  }
};

class Solids {
public:
  Solids *next, *prev;
  Face *sface;
  Solids();

  Solids *getNext() { return next; }
  Solids *getPrev() { return prev; }
  Face *getFace() { return sface; }

  Solids *mvfs();
};

class Halfedge {
public:
  Halfedge *next, *prev;
  Vertex *v1, *v2;
  Loop *wloop;
  Halfedge() {}
  Halfedge(Vertex *_v1, Vertex *_v2) : next(this), prev(this), wloop(nullptr) {
    v1 = _v1;
    v2 = _v2;
  }
  Halfedge *getNext() { return next; }
  Halfedge *getPrev() { return prev; }
  Vertex *getv1() { return v1; }
  Vertex *getv2() { return v2; }
  ostream &operator<<(ostream &o) {
    o << "(";
    v1->operator<<(o);
    o << ",";
    v2->operator<<(o) << ")";
    return o;
  }
};

class Loop {
public:
  Loop *next, *prev;
  Halfedge *ledge;
  Face *lface;
  Loop() {}
  Loop *getNext() { return next; }
  Loop *getPrev() { return prev; }
  Face *getFace() { return lface; }
  Halfedge *getEdge() { return ledge; }
  ostream &operator<<(ostream &o) {
    ledge->operator<<(o);
    Halfedge *p = ledge->next;
    while (p && p != ledge) {
      p->operator<<(o);
      p = p->next;
    }
    return o;
  }
};
class Face {
public:
  Face *next, *prev;
  Loop *floop;
  Solids *fsolid;
  Face();

  Face *getNext() { return next; }
  Face *getPrev() { return prev; }
  Solids *getSolid() { return fsolid; }
  Loop *getLoop() { return floop; }
  ostream &operator<<(ostream &o) {
    floop->operator<<(o);
    Loop *p = floop->next;
    while (p && p != floop) {
      p->operator<<(o);
      p = p->next;
    }
    return o;
  }
};

#endif
