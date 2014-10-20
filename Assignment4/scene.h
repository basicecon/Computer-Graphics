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
  int tmeshesN;

  Scene();
  void DBG();
  void Render();
  //void FlagB();
  

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

  void SM1();
  void SM2();
  void SM3();
  void SM2Play();
  void SM3Play();
};

extern Scene *scene;
