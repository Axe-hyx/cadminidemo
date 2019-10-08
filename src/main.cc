#include "operator.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "operator.h"

GLdouble quad[12][3] = {{5, 5, 0},    {15, 5, 0},   {15, -5, 0},  {5, -5, 0},
                        {0, 5, 0},    {-15, 10, 0}, {-5, -10, 0}, {-10, 5, 0},
                        {-0.5, 1, 0}, {-0.5, 2, 0}, {0.5, 2, 0},  {0.5, 1, 0}};
void myIdle(void) { glutPostRedisplay(); }
#ifndef CALLBACK
#define CALLBACK
#endif

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

//------------------------------------------------------------	OnDraw()
//
void CALLBACK PolyLine3DBegin(GLenum type) { glBegin(type); }

void CALLBACK PolyLine3DVertex(GLdouble *vertex) {
  const GLdouble *pointer;
  pointer = (GLdouble *)vertex;
  glColor3d(1.0, 0, 0); //在此设置颜色glVertex3dv(pointer);
}

void CALLBACK PolyLine3DEnd() { glEnd(); }
GLUtesselator *tesser() {
  GLUtesselator *tess;
  tess = gluNewTess();
  gluTessCallback(tess, GLU_TESS_BEGIN, (void(CALLBACK *)()) & PolyLine3DBegin);
  gluTessCallback(tess, GLU_TESS_VERTEX,
                  (void(CALLBACK *)()) & PolyLine3DVertex);
  gluTessCallback(tess, GLU_TESS_END, (void(CALLBACK *)()) & PolyLine3DEnd);
  return tess;
}

/////////////////////////////////////////////////////////////////////////////////

void OnDraw() {

  glClear(GL_STENCIL_BUFFER_BIT);

  GLUtesselator *tess = tesser();
  if (!tess)
    return;
  gluTessBeginPolygon(tess, NULL);

  gluTessBeginContour(tess);
  for (int i = 0; i < 4; i++)
    gluTessVertex(tess, quad[i], quad[i]);
  gluTessEndContour(tess);

  gluTessBeginContour(tess);
  for (int i = 4; i < 8; i++)
    gluTessVertex(tess, quad[i], quad[i]);
  
  gluTessEndContour(tess);

  /*
  Face * f = s->sface;
  do {
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
    f = f->next;
  }while(f!=s->sface);
  */
  
  gluTessEndPolygon(tess);
  glutSwapBuffers();
}

//------------------------------------------------------------	OnInit()
//
void OnInit() {
  // glClearColor(1,1,1,0);
}

//------------------------------------------------------------	OnExit()
//
void OnExit() {}

//------------------------------------------------------------	OnReshape()
//
void OnReshape(int w, int h) {
  // prevents division by zero when minimising window
  if (h == 0)
    h = 1;

  // set the drawable region of the window
  glViewport(0, 0, w, h);

  // set up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // just use a perspective projection
  // gluPerspective(45,(float)w/h,0.1,100);
  if (w <= h)
    glOrtho(-20.0, 20.0, -20.0 * (GLfloat)h / (GLfloat)w,
            20.0 * (GLfloat)h / (GLfloat)w, 0.0, 100.0);
  else
    glOrtho(-20.0, 20.0, -20.0 * (GLfloat)h / (GLfloat)w,
            20.0 * (GLfloat)h / (GLfloat)w, 0.0, 100.0);

  // go back to modelview matrix so we can move the objects about
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
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
  return id; // return handle ID of a display list
}

//------------------------------------------------------------	main()
//
int main(int argc, char **argv) {

  construct_cube(); 

  // initialise glut
  glutInit(&argc, argv);

  // request a depth buffer, RGBA display mode, and we want double buffering
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

  // set the initial window size
  glutInitWindowSize(480, 480);

  // create the window
  glutCreateWindow("fill tess");

  // run our custom initialisation
  OnInit();

  // set the function to use to draw our scene
  glutDisplayFunc(OnDraw);

  // set the function to handle changes in screen size
  glutReshapeFunc(OnReshape);
  //	glutIdleFunc(&myIdle);

  // set the function to be called when we exit
  atexit(OnExit);

  // this function runs a while loop to keep the program running.
  glutMainLoop();

  return 0;
}

