#pragma once
#include "operator.h"
#include "struct.h"
#include <vector>

class Factory {
public:
  vector<Solids *> s;
  void Addplane(std::vector<Vertex> &data) {
    Solids *solid;
    Loop *firstloop;
    mvfs(solid, data[0], firstloop);
    for (int i = 1; i < data.size(); ++i) {
      mev(firstloop, data[i - 1], data[i]);
    }
    Face *nface;
    mef(firstloop, data[data.size() - 1], data[0], nface, solid->getFace());
    s.push_back(solid);
  }

  void AddHole(int n, std::vector<Vertex> &data) {
    Solids *p = s[n];
    auto *fout = p->getFace();
    auto *fin = fout->next;
    auto *loop = fout->getLoop();
    mev(loop, *loop->getEdge()->getv1(), data[0]);
    for (int i = 1; i < data.size(); ++i) {
      mev(loop, data[i - 1], data[i]); // addr mess
    }
    Face *nface;
    mef(loop, data[data.size() - 1], data[0], nface, fin);
    Loop *nloop;
    kemr(loop, *loop->getEdge()->getv1(), data[0], nloop, fout->getTail());
    join(nface, fin);
  }
  /*
    @prama n, the id of solid modile;
    @prama d, travese distance along dir;
    @prama dir, Vertex specificate direction
  */
  void Sweep(int n, float d, const Vertex &t) {
    Solids *p = s[n];
    auto *fup = p->getFace();
    auto *fdown = fup->next;
    sweep(fdown, d, t);
  }
};
