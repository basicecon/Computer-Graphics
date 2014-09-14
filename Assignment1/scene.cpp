#include "scene.h"
#include "m33.h"

#include <iostream>

using namespace std;

Scene *scene;

Scene::Scene() {

  gui = new GUI();
  gui->show();

  int u0 = 20;
  int v0 = 50;
  int sci = 2;
  int w = sci*320;
  int h = sci*240;
  fb = new FrameBuffer(u0, v0, w, h);
  fb->label("Framebuffer");
  fb->show();
  gui->uiw->position(fb->w+20+20, 50);

  fb->Set(0xFFFFFFFF);

  Render();

}

void Scene::Render() {

    fb->redraw();

}

void Scene::DBG() {

  FrameBuffer *convolvedImage = 0;
  fb->SetChecker(64, 0xFF000000, 0xFFFFFFFF);
  M33 blurk;
  blurk[0] = V3(1.0f, 2.0f, 1.0f);
  blurk[1] = V3(2.0f, 4.0f, 1.0f);
  blurk[2] = V3(1.0f, 2.0f, 1.0f);

  blurk[0] = V3(1.0f, 1.0f, 1.0f);
  blurk[1] = V3(1.0f, 1.0f, 1.0f);
  blurk[2] = V3(1.0f, 1.0f, 1.0f);

  blurk.Normalize();

  fb->Convolve33(blurk, convolvedImage);
  convolvedImage->show();
  return;

  fb->DrawSegment(10.1f, 20.3f, 10.1f, 20.3f, 0xFF000000);
  Render();
  return;

  fb->DrawPoint(200, fb->h-30, 170, 0xFF00FF00);
  Render();
  return;

  unsigned int color0 = 0xFF000000;
  unsigned int color1 = 0xFFFFFFFF;
  int slowMotionFactor = 10;
  for (int csize = slowMotionFactor; 
    csize <= fb->h*slowMotionFactor; csize++) { 
    fb->SetChecker(csize/slowMotionFactor, color0, color1);
    Render();
    Fl::check();
  }
  return;

  cerr << "INFO: pressed the DBG button" << endl;

  unsigned bgr = 0xFF00FFFF;
  fb->Set(bgr);
  Render();

  for (int u = 0; u < fb->w; u++) {
    fb->Set(u, fb->h/2, 0xFF000000);
  }
  Render();

}

void Scene::FlagB () {


  if (gui->FlagB->value()) {
    cerr << "Flag button changed to on" << endl;
  }
  else {
    cerr << "Flag button changed to off" << endl;
  }

}















