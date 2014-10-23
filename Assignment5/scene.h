#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "ppc.h"
#include "tmesh.h"

class Scene {
public:
  
  GUI *gui;
  FrameBuffer *fb;
  PPC *ppc;
  TMesh **tmeshes;
  TMesh *mesh;
  int tmeshesN;
  FrameBuffer *texture;
  
  Scene();
  void Render();
  void Visualize();

  void Up();
  void Down();
  void Left();
  void Right();
  void Front();
  void Back();
  void Pan();
  void Tilt();
  void Roll();
  void ZoomIn();
  void ZoomOut();

  const char* GetPointLightX();
  const char* GetPointLightY();
  const char* GetPointLightZ();
  const char* ka();
  const char* es();

  void SM1();
  void SM2();
  void SM3();
  void SM2Play();
  void SM3Play();

  void Bilinear();
  void Neighbor();
  void Repeat();
  void Mirror();
  void Browse();

  void LoadFile();
  const char* ReadFile();
  //void changeTexture(FrameBuffer *newT, FrameBuffer *fb);
};

extern Scene *scene;
