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
  tmeshes[0]->enabled = false;
  
  tmeshes[1] = new TMesh();
  tmeshes[1]->LoadBin("geometry/teapot1k.bin");
  V3 newCenter = V3(0.0f, 0.0f, -150.0f);
  tmeshes[1]->Position(newCenter);
  tmeshes[1]->enabled = true;

  Render();

}

void Scene::Render() {

  //fb->Set(0xFFFFFFFF);
	fb->Clear(0xFFFFFFFF, 0.0f);

	for (int tmi = 0; tmi < tmeshesN; tmi++) {
		if (!tmeshes[tmi]->enabled)
			continue;
		if (tmeshes[tmi]->trisN != 0) {
			tmeshes[tmi]->RenderWireframe(ppc, fb, 0xFF000000);
		}
		else {
			tmeshes[tmi]->RenderPoints(ppc, fb, 3);
		}

		
		V3 L(0, 0, 0);
		if (tmi == 1) {
			tmeshes[tmi]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0.2, 600, 3);
		}
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


