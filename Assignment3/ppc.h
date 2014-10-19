#pragma once

#include "v3.h"
#include <istream>
#include <ostream>
//#include "framebuffer.h"
class FrameBuffer;
using namespace std;

class PPC {
public:
  V3 a, b, c, C;
  int w, h;
  PPC(float hfov, int w, int h);
  bool Project(V3 P, V3 &PP);
  void PositionAndOrient(V3 newC, V3 lap, V3 vpv);
  float GetF();
  V3 GetVD();

  void Pan(float deg);
  void Tilt(float deg);
  void Roll(float deg);

  void LeftRightTranslation(int steps);
  void UpDownTranslation(int steps);
  void FrontBackTranslation(int steps);

  V3 GetPointOnImagePlane(float uf, float vf);
  V3 GetPointOnFocalPlane(float uf, float vf, float f);
  float PPu();
  float PPv();
  void Zoom(float scf);
  float Hfov();

  PPC Interpolate(PPC *ppc1, PPC ret, float i, float n);
  void SaveToText(char *fileName);
  PPC LoadFromText(char *fileName);

  void Visualize(PPC *ppc1, FrameBuffer *fb, unsigned int color);
};