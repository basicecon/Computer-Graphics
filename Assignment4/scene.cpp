#include "v3.h"

#include <iostream>
#include <stdlib.h>     //for using the function sleep


extern "C" {
	FILE* _iob = NULL;
}
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
		/*
		if (tmeshes[tmi]->trisN != 0) {
			tmeshes[tmi]->RenderWireframe(ppc, fb, 0xFF000000);
		}
		else {
			tmeshes[tmi]->RenderPoints(ppc, fb, 3);
		}*/

		V3 L(0, 0, 0);
		tmeshes[tmi]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
		
	}
	fb->redraw();

}

void Scene::Visualize() {
	float hfov = 55.0f;
	float w = 320;
	float h = 240;
	PPC ppc0(hfov, w, h);
	PPC ppc1(2 * hfov, 2 * w, 2 * h);
	ppc1.C = V3(0, 0, h);
	ppc0.Visualize(&ppc1, fb, 0xFF000000);
	Render();
}

void Scene::Up() {
	V3 L = V3(0, 0, 0);
	float step = 1.0f;
	ppc->UpDownTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::Down() {
	V3 L = V3(0, 0, 0);
	float step = -1.0f;
	ppc->UpDownTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::Left() {
	V3 L = V3(0, 0, 0);
	float step = 1.0f;
	ppc->LeftRightTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::Right() {
	V3 L = V3(0, 0, 0);
	float step = -1.0f;
	ppc->LeftRightTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::Front() {
	V3 L = V3(0, 0, 0);
	float step = 1.0f;
	ppc->FrontBackTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::Back() {
	V3 L = V3(0, 0, 0);
	float step = -1.0f;
	ppc->FrontBackTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::Pan() {
	V3 L = V3(0, 0, 0);
	float deg = 1.0f;
	ppc->Pan(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::Tilt() {
	V3 L = V3(0, 0, 0);
	float deg = 1.0f;
	ppc->Tilt(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::Roll() {
	V3 L = V3(0, 0, 0);
	float deg = 1.0f;
	ppc->Roll(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::ZoomIn() {
	V3 L = V3(0, 0, 0);
	float zoom = 1.2f;
	ppc->Zoom(zoom);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::ZoomOut() {
	V3 L = V3(0, 0, 0);
	float zoom = 0.8f;
	ppc->Zoom(zoom);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}


void Scene::SM1() {
	V3 L = V3(0, 0, 0);
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0, 0, 1);
	fb->redraw();
}

void Scene::SM2() {
	cerr << "ENTERING SM2....!" << endl;
	//default light source info
	V3 L = V3(-300, 30, -80);
	float ka = 0.5f;
	float es = 200;

	L[0] = atof(gui->pointlightX->value());
	L[1] = atof(gui->pointlightY->value());
	L[2] = atof(gui->pointlightZ->value());
	
	ka = atof(gui->ka->value());
	es = atof(gui->es->value());
	
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, ka, es, 2);
	fb->redraw();
}

void Scene::SM3() {
	cerr << "ENTERING SM3....!" << endl;
	//default light source info
	V3 L = V3(-300, 30, -80);
	float ka = 0.5f;
	float es = 200;
	
	L[0] = atof(gui->pointlightX->value());
	L[1] = atof(gui->pointlightY->value());
	L[2] = atof(gui->pointlightZ->value());
	//cerr << L[0] << ", " << L[1] << ", " << L[2] << endl;
	ka = atof(gui->ka->value());
	es = atof(gui->es->value());
	
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, ka, es, 3);
	fb->redraw();
}

void Scene::SM2Play() {
	cerr << "SM2 play!" << endl;
	V3 L = V3(-300.0f, 30.0f, -80.0f);
	V3 move = V3(10, 0, 0);
	int step = 50;
	for (int i = 0; i < step; i++){
		fb->Clear(0xFFFFFFFF, 0.0f);
		tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0.5, 200, 2);
		//cerr << L[0] << ", " << L[1] << ", " << L[2] << endl;
		L = L + move;
		fb->redraw();
		
	}

}
void Scene::SM3Play() {
	cerr << "SM3 play!" << endl;
	V3 L = V3(-100.0f, 30.0f, -80.0f);
	V3 move = V3(5, 0, 0);
	int step = 50;
	
	for (int i = 0; i < step; i++){
		fb->Clear(0xFFFFFFFF, 0.0f);
		tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, L, 0.5, 200, 3);
		//cerr << L[0] << ", " << L[1] << ", " << L[2] << endl;
		L = L + move;
		fb->redraw();
	}/*
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, V3(-900.0f, 30.0f, -80.0f), 0.5, 200, 3);
	fb->redraw();
	_sleep(2000);
	*/
}




