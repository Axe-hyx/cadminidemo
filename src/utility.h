#include "GL/glut.h"
#include <sstream>
#include <iostream>
#include "Manager.h"
using namespace std;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraDistance = -20;
float cameraAngleX;
float cameraAngleY;

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

std::stringstream ss;

#ifndef CALLBACK
#define CALLBACK
#endif
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
extern Factory fc;
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
