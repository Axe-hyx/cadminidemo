#ifndef STRUCTH
#define STRUCTH
class Solids;
class Face;
class Loop;
class Halfedge;
class Vertex;
#include <iostream>
template <typename T> void linkafter(T *e1, T *e2) {
  e1->next = e2;
  e2->prev = e1;
}

using namespace std;
class Vertex {
public:
  double v[3];

  Vertex *next, *prev;
  int _id;

  static int i;
  friend istream &operator>>(istream &in, Vertex &ver) {
    in >> ver.v[0] >> ver.v[1] >> ver.v[2];
    return in;
  }
  Vertex() { _id = ++i; }
  Vertex(float _x, float _y, float _z) : next(nullptr), prev(nullptr) {
    static int i = 0;
    v[0] = _x;
    v[1] = _y;
    v[2] = _z;
    //_id = ++i;
  }
  Vertex(const Vertex &ve, int id) : prev(ve.prev), next(ve.next) {
    v[0] = ve.v[0];
    v[1] = ve.v[1];
    v[2] = ve.v[2];
    _id = id;
  }
  Vertex(const Vertex &ve) : prev(ve.prev), next(ve.next) {
    v[0] = ve.v[0];
    v[1] = ve.v[1];
    v[2] = ve.v[2];
    _id = ve._id;
    //_id = ++i;
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
  Vertex operator*(float d) { return Vertex(v[0] * d, v[1] * d, v[2] * d); }
  Vertex operator+(const Vertex &vec) {
    return Vertex(v[0] + vec.v[0], v[1] + vec.v[1], v[2] + vec.v[2]);
  }
  friend ostream &operator<<(ostream &o, const Vertex *v) {
    o << v->_id;
    // o << "(" << v->v[0] << "," << v->v[1] << "," << v->v[2] << ")";
    return o;
  }
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
  friend ostream &operator<<(ostream &o, const Halfedge *he) {
    o << "[";
    o << he->v1;
    o << ",";
    o << he->v2;
    o << "]"
      << " ";
    return o;
  }
};

class Loop {
public:
  Loop *next, *prev;
  Halfedge *ledge;
  Face *lface;
  Loop();
  Loop *getNext() { return next; }
  Loop *getPrev() { return prev; }
  Face *getFace() { return lface; }
  Halfedge *getEdge() { return ledge; }
  friend ostream &operator<<(ostream &o, const Loop *l) {
    o << "\t";
    o << l->ledge;
    Halfedge *p = l->ledge->next;
    while (p && p != l->ledge) {
      o << p;
      p = p->next;
    }
    cout << endl;
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
  Loop *getTail() {
    Loop *p = floop, *np = floop->next;
    while ((np != nullptr) && np != floop) {
      p = np;
      np = np->next;
    }
    return p;
  }
  friend ostream &operator<<(ostream &o, const Face *f) {
    static int i = 0;
    o << "Face:" << i++ << endl;
    o << f->floop;
    Loop *p = f->floop->next;
    while (p && p != f->floop) {
      o << p;
      p = p->next;
    }
    cout << endl;
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
  friend std::ostream &operator<<(std::ostream &o, const Solids *sl) {
    o << sl->sface;
    Face *p = sl->sface->next;
    while (p && p != sl->sface) {
      o << p;
      p = p->next;
    }
    return o;
  }
};

#endif
