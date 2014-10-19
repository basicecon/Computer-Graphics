#include "scene.h"
#include "m33.h"
#include "v3.h"

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

  float hfov = 55.0f;
  ppc = new PPC(hfov, w, h);

  tmeshesN = 2;
  tmeshes = new TMesh*[tmeshesN];

  V3 center(7.0f, -10.0f, -65.0f);
  V3 dims(20.0f, 10.0f, 45.0f);
  unsigned int color = 0xFF00FF00;
  tmeshes[0] = new TMesh(center, dims, color);
  tmeshes[0]->enabled = true;
  
  tmeshes[1] = new TMesh();
  tmeshes[1]->LoadBin("geometry/teapot1k.bin");
  V3 newCenter = V3(0.0f, 0.0f, -150.0f);
  tmeshes[1]->Position(newCenter);

  Render();

}

void Scene::Render() {

  fb->Set(0xFFFFFFFF);
  for (int tmi = 0; tmi < tmeshesN; tmi++) {
    if (!tmeshes[tmi]->enabled)
      continue;
    tmeshes[tmi]->RenderWireframe(ppc, fb, 0xFF000000);
    tmeshes[tmi]->RenderPoints(ppc, fb, 3);
  }
  fb->redraw();

}

void Scene::DBG() {

/*
  {
    V3 C0 = ppc->C;
    V3 C1(0.0f, 180.0f, -30.0f);
    V3 lap = tmeshes[1]->GetCenter();
    V3 vpv = V3(0.0f, 1.0f, 0.0f);
    int stepsN = 100;
    for (int si = 0; si < stepsN; si++) {
      V3 newC = C0 + (C1-C0)*(float)si/(float)(stepsN-1);
      ppc->PositionAndOrient(newC, lap, vpv);
      Render();
      Fl::check();
    }

    return;

  }

  {

    tmeshes[0]->enabled = false;
    int stepsN = 100;
    float d0 = 10.0f, d1 = 200.0f;
    for (int si = 0; si < stepsN; si++) {
      float newDiagonal = d0 + (d1-d0)*(float) si / (float) (stepsN-1);
      tmeshes[1]->ScaleToNewDiagonal(newDiagonal);
      Render();
      Fl::check();
    }


  }*/

  tmeshes[1]->RenderWireframe(ppc, fb, 0xFF000000);
  tmeshes[1]->RenderPoints(ppc, fb, 7);
  Render();
  return;
  tmeshes[0]->RenderWireframe(ppc, fb, 0xFF000000);
  tmeshes[0]->RenderPoints(ppc, fb, 17);
  Render();
  return;

  V3 verts[8];


  for (int j = 0; j < 1000; j++) {

    float a = 20.0f / 2.0f;
    float zn = -40.0f;
    int i = 0;
    verts[i++] = V3(-a, +a, zn);
    verts[i++] = V3(-a, -a, zn);
    verts[i++] = V3(+a, -a, zn);
    verts[i++] = V3(+a, +a, zn);

    verts[i++] = V3(-a, +a, zn-a*2.0f);
    verts[i++] = V3(-a, -a, zn-a*2.0f);
    verts[i++] = V3(+a, -a, zn-a*2.0f);
    verts[i++] = V3(+a, +a, zn-a*2.0f);

    V3 PP[8];;
    for (int i = 0; i < 8; i++) {
      if (!ppc->Project(verts[i], PP[i]))
        continue;
      if (i < 4)
        fb->DrawPoint((int)PP[i][0], (int)PP[i][1], 17, 0xFF0000FF);
      else
        fb->DrawPoint((int)PP[i][0], (int)PP[i][1], 17, 0xFF0000AA);
    }

    for (int i = 0; i < 4; i++) {
      fb->DrawSegment(PP[i][0], PP[i][1], PP[i+4][0], PP[i+4][1], 0xFF000000);
      fb->DrawSegment(PP[i][0], PP[i][1], PP[(i+1)%4][0], PP[(i+1)%4][1], 0xFF000000);
      fb->DrawSegment(PP[i+4][0], PP[i+4][1], PP[(i+1)%4+4][0], PP[(i+1)%4+4][1], 0xFF000000);
    }

    Render();
    Fl::check();
    fb->Set(0xFFFFFFFF);
    ppc->C = ppc->C + ppc->a*0.01f;


  }
  return;

  V3 P0(-20.0f, 0.0f, -100.0f);
  V3 P1(+20.0f, 0.0f, -100.0f);
  int stepsN = 1000;
  for (int stepi = 0; stepi < stepsN; stepi++) {
    float f = (float) stepi / (float) (stepsN-1);
    V3 P = P0 + (P1-P0)*f;
    fb->Set(0xFFFFFFFF);
    V3 PP;
    ppc->Project(P, PP);
    fb->DrawPoint((int)PP[0], (int)PP[1], 7, 0xFF0000FF);
    fb->redraw();
    Fl::check();
  }
  return;

  V3 P(0.0f, 0.0f, -100.0f);
  V3 Pp;
  if (!ppc->Project(P, Pp))
    cerr << "INVALID PROJECTION" << endl;
  else
    cerr << "Projection: " << Pp << endl;
  return;

  M33 m;
  m[0] = V3(1.0f, -3.0f, 0.0f);
  m[1] = V3(3.0f, 1.0f, 5.0f);
  m[2] = V3(-2.0f, 3.0f, 10.0f);
  cerr << "m: " << endl << m << endl;
  cerr << "m_1: " << endl << m.Inverse() << endl;
  cerr << "m x m_1: " << endl << m.Inverse()*m << endl;
  cerr << "m_1 x m: " << endl << m * m.Inverse() << endl;
  return;

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

void Scene::Visualize(){
	float hfov = 55.0f;
	float w = 320;
	float h = 240;
	PPC ppc0(hfov, w, h);
	PPC ppc1(2 * hfov, 2 * w, 2 * h);
	ppc1.C = V3(0, 0, h);
	ppc0.Visualize(&ppc1, fb, 0xFF000000);
	Render();
}
