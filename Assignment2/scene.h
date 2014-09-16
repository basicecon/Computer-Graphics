#pragma once

#include "gui.h"
#include "framebuffer.h"

class Scene {
public:
  
  GUI *gui;
  FrameBuffer *fb;
  Scene();
  void DBG();
  void Render();
  void FlagB();
  void EdgeDetect();
  void LoadButton();
  void SaveButton();
  void AdjBrightness();
  void DrawMyInitial();
};

extern Scene *scene;
