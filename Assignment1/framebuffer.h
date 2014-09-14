#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "m33.h"

class FrameBuffer : public Fl_Gl_Window {
public:
  unsigned int *pix;
  int w, h;
  FrameBuffer(int u0, int v0, int _w, int _h);
  void draw();
  void Set(unsigned int bgr);
  void Set(int u, int v, unsigned int color);
  unsigned int Get(int u, int v);
  void SetSafe(int u, int v, unsigned int color);
  void SetChecker(int csize, unsigned int color0, unsigned int color1);
  void DrawPoint(int u, int v, int psize, unsigned int color);
  void DrawSegment(float u0f, float v0f, float u1f, float v1f, 
    unsigned int color);
  void Convolve33(M33 kernel, FrameBuffer *& fb1);

};
