

Face *f0, *f1, *f2, *f3, *f4, *f5, *f6, *f7, *f8, *f9, *f10;
Solids *s;
Vertex v0(0, 0, 0);
Vertex v1(3, 0, 0);
Vertex v2(3, 3, 0);
Vertex v3(0, 3, 0);
Vertex v4(0, 0, 3);
Vertex v5(3, 0, 3);
Vertex v6(3, 3, 3);
Vertex v7(0, 3, 3);

Vertex v8(1, 1, 3); // 4
Vertex v9(2, 1, 3);
Vertex v10(2, 2, 3);
Vertex v11(1, 2, 3);

Vertex v12(1, 1, 0);
Vertex v13(2, 1, 0);
Vertex v14(2, 2, 0);
Vertex v15(1, 2, 0);

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
  kfmrh(f1, f10);
  // cout<<s;
  return;
}

GLuint tesselator(Face *f) {
  GLuint id = glGenLists(1); // create a display list
  if (!id)
    return id; // failed to create a list, return 0

  GLUtesselator *tess = gluNewTess(); // create a tessellator
  if (!tess)
    return 0; // failed to create tessellation object, return 0

  // define concave quad data (vertices only)
  //  0    2
  //  \ \/ /
  //   \3 /
  //    \/
  //    1

  // register callback functions
  gluTessCallback(tess, GLU_TESS_BEGIN, (void(CALLBACK *)())tessBeginCB);
  gluTessCallback(tess, GLU_TESS_END, (void(CALLBACK *)())tessEndCB);
  gluTessCallback(tess, GLU_TESS_ERROR, (void(CALLBACK *)())tessErrorCB);
  gluTessCallback(tess, GLU_TESS_VERTEX, (void(CALLBACK *)())tessVertexCB);

  // tessellate and compile a concave quad into display list
  // gluTessVertex() takes 3 params: tess object, pointer to vertex coords,
  // and pointer to vertex data to be passed to vertex callback.
  // The second param is used only to perform tessellation, and the third
  // param is the actual vertex data to draw. It is usually same as the second
  // param, but It can be more than vertex coord, for example, color, normal
  // and UV coords which are needed for actual drawing.
  // Here, we are looking at only vertex coods, so the 2nd and 3rd params are
  // pointing same address.
  glNewList(id, GL_COMPILE);
  glColor3f(1, 1, 1);
  gluTessBeginPolygon(tess, 0); // with NULL data
  GLdouble quad1[5][3] = {
      {0, 3, 3}, {0, 0, 3}, {3, 0, 3}, {3, 3, 3}, {0, 3, 0}};
  // GLdouble quad1[5][3] = {
  //                        {0, 3, 3}, {3, 3, 3}, {3, 0, 3}, {0, 0, 3}, {0, 3,
  //                        0}};
  gluTessBeginContour(tess);
  gluTessEndContour(tess);

  Loop *l = f->floop;
  do {
    gluTessBeginContour(tess);
    Halfedge *he = l->ledge;
    do {
      Vertex *v1 = he->getv1();
      Vertex *v2 = he->getv2();
      gluTessVertex(tess, v1->v, v1->v);
      he = he->next;
    } while (he && he != l->ledge);
    l = l->next;
    gluTessEndContour(tess);
  } while (l && l != f->floop);
  gluTessEndPolygon(tess);
  glEndList();
  gluDeleteTess(tess); // delete after tessellation
  cout << endl;
  cout << "tesselator face\n";
  cout << "===============\n";
  cout << ss.str().c_str() << endl;
  ss.str(""); // clear string buffer

  // DEBUG //
  // print out actual GL calls that are performed
  return id; // return handle ID of a display list
}

  // glutIdleFunc(idleCB);                       // redraw only every given
  // millisec
  glutReshapeFunc(reshapeCB);
  glutKeyboardFunc(keyboardCB);
  glutMouseFunc(mouseCB);
  glutMotionFunc(mouseMotionCB);

  return handle;
}

