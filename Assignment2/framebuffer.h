#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "m33.h"
#include "tiffio.h"

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
  void DrawSegment(float u0f, float v0f, float u1f, float v1f, unsigned int color);
  void DrawCircle(float uf, float vf, float r, int alpha, unsigned int color);
  void Convolve33(M33 kernel, FrameBuffer *& fb1);
  TIFF* LoadTiff(const char* fileName, FrameBuffer *&displayFb);
  void SaveTiff(const char* fileName);
  //TIFF* AdjustBrightness(const char* fileName, int percentage, FrameBuffer *&fb);
  void AdjustBrightness(int percentage);
  void MultiplyScanlineByFactor(uint32* scanline, uint32* tmp, uint32 factor);
  void EdgeDetection(M33 k);
  void DrawDot(float uf, float vf, float r, unsigned int color);
};
