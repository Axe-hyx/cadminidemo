#include "Manager.h"
#include "operator.h"
#include "utility.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
Factory fc;

std::vector<std::vector<Vertex>> data;
void outward() {
  {
    int vnum;
    cin >> vnum;
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
  outward(); // 处理外环
  for (int i = 0; i < loopnum - 1; ++i) {
    inward(i); //处理内环
  }
  dir dic;
  cin >>dic;
  fc.Sweep(0, 1, dic);
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
