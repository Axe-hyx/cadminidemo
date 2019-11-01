#include "operator.h"
#include <GL/glut.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

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
  void Sweep(int n, float d, const Vertex& t) {
    Solids *p = s[n];
    auto *fup = p->getFace();
    auto *fdown = fup->next;
    sweep(fdown, d, t);
  }
};

Factory fc;
#ifndef CALLBACK
#define CALLBACK
#endif
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraDistance = -20;
float cameraAngleX;
float cameraAngleY;
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
////////debug
stringstream ss;
void CALLBACK tessVertexCB2(const GLvoid *data)

{

  // cast back to double type

  const GLdouble *ptr = (const GLdouble *)data;

  glColor3dv(ptr + 3);

  glVertex3dv(ptr);

  // DEBUG //

  ss << "  glColor3d(" << *(ptr + 3) << ", " << *(ptr + 4) << ", " << *(ptr + 5)
     << ");\n";

  ss << "  glVertex3d(" << *ptr << ", " << *(ptr + 1) << ", " << *(ptr + 2)
     << ");\n";
}
////////
void mouseCB(int button, int state, int x, int y) {
  mouseX = x;
  mouseY = y;

  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      mouseLeftDown = true;
    } else if (state == GLUT_UP)
      mouseLeftDown = false;
  }

  else if (button == GLUT_RIGHT_BUTTON) {
    if (state == GLUT_DOWN) {
      mouseRightDown = true;
    } else if (state == GLUT_UP)
      mouseRightDown = false;
  }
}

void mouseMotionCB(int x, int y) {
  if (mouseLeftDown) {
    cameraAngleY += (x - mouseX);
    cameraAngleX += (y - mouseY);
    mouseX = x;
    mouseY = y;
  }
  if (mouseRightDown) {
    cameraDistance -= (y - mouseY) * 0.2f;
    mouseY = y;
  }

  glutPostRedisplay();
}
GLdouble quad[12][3] = {{5, 5, 0},    {15, 5, 0},   {15, -5, 0},  {5, -5, 0},
                        {0, 5, 0},    {-15, 10, 0}, {-5, -10, 0}, {-10, 5, 0},
                        {-0.5, 1, 0}, {-0.5, 2, 0}, {0.5, 2, 0},  {0.5, 1, 0}};
void myIdle(void) { glutPostRedisplay(); }

//------------------------------------------------------------	OnDraw()
//
void CALLBACK PolyLine3DBegin(GLenum type) { glBegin(type); }

void CALLBACK PolyLine3DVertex(GLdouble *vertex) {
  const GLdouble *pointer;
  pointer = (GLdouble *)vertex;
  glColor3d(1.0, 0, 0); //在此设置颜色
  glVertex3dv(pointer);
  GLdouble *ptr = (GLdouble *)vertex;
  ss << "  glVertex3d(" << *ptr << ", " << *(ptr + 1) << ", " << *(ptr + 2)
     << ");\n";
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
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_STENCIL_BUFFER_BIT);
  glPushMatrix();
  glTranslatef(0, 0, cameraDistance);
  glRotatef(cameraAngleX, 1, 0, 0);
  glRotatef(cameraAngleY, 0, 1, 0);

  GLUtesselator *tess = tesser();
  if (!tess)
    return;
  // gluTessBeginContour(tess);
  // for (int i = 4; i < 8; i++)
  //  gluTessVertex(tess, quad[i], quad[i]);
  // gluTessEndContour(tess);
  for (int i = 0; i < fc.s.size(); ++i) {
    Face *f = fc.s[0]->sface;
    do {
      gluTessBeginPolygon(tess, NULL);
      Loop *l = f->floop;
      cout << f << endl;
      do {
        gluTessBeginContour(tess);
        Halfedge *he = l->ledge;
        do {
          Vertex *v1 = he->getv1();
          gluTessVertex(tess, v1->v, v1->v);
          he = he->next;
        } while (he && he != l->ledge);
        l = l->next;
        gluTessEndContour(tess);
      } while (l && l != f->floop);
      gluTessEndPolygon(tess);
      f = f->next;
    } while (f && f != fc.s[0]->sface);
    cout << endl;
    cout << "===============\n";
    cout << ss.str().c_str() << endl;
    ss.str(""); // clear string buffer
    glPopMatrix();
    glutSwapBuffers();
  }
}

//------------------------------------------------------------	OnInit()
//
void OnInit() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
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

//------------------------------------------------------------	main()
//
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

void inner_loop(std::vector<Vertex> &data) {}
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

std::vector<std::vector<Vertex>> data;
void outward() {
  {
    int vnum;
    cin >> vnum;
    // vector resize 会对齐 无法获取实际元素数量
    for (int k = 0; k < vnum; ++k) {
      Vertex v;
      cin >> v;
      data[0].push_back(v);
    }
    fc.Addplane(data[0]);
  }
}
void inward(int i) {
  int vnum;
  cin >> vnum;
  for (int k = 0; k < vnum; ++k) {
    Vertex v;
    cin >> v;
    data[i + 1].push_back(v);
  }
  fc.AddHole(0, data[i + 1]);
}
void feeddata() {
  using dir = Vertex;
  int loopnum;
  cin >> loopnum;
  data.resize(loopnum);
  outward();
  for (int i = 0; i < loopnum - 1; ++i) {
      inward(i);
  }
  fc.Sweep(0, 1, dir(0,0,20));
}
int Vertex::i = 0;
int main(int argc, char **argv) {
  // construct_cube();
  feeddata();
  // multiple_hole_sweep();
  // initialise glut
  glutInit(&argc, argv);

  // request a depth buffer, RGBA display mode, and we want double buffering
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

  // set the initial window size
  glutInitWindowSize(1080, 1080);

  // create the window
  glutCreateWindow("fill tess");

  // run our custom initialisation
  OnInit();
  glutMouseFunc(mouseCB);
  glutMotionFunc(mouseMotionCB);

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
