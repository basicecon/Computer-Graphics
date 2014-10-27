#include "scene.h"
#include "m33.h"
#include "v3.h"
#include "gui.h"

#include <iostream>
#include <sstream>

using namespace std;

Scene *scene;
int renderM = 1; //default SSIV 
int lookupM = 1; // default neighbor
int tilingM = 0; // default no tiling
float kaf = 0.0f; 
float esf = 0.0f;
V3 L = V3(0, 0, 0);

Scene::Scene() {

  gui = new GUI();
  cerr << "dfdfd" << endl;
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

  V3 center(7.0f, -16.0f, -65.0f);
  V3 dims(12.0f, 8.0f, 20.0f);
  unsigned int color = 0x00000000;
  tmeshes[0] = new TMesh(center, dims, color);
  tmeshes[0]->enabled = false;

  // texture framebuffer --------------------------------------------------------------

  texture = new FrameBuffer(20, 20, 200, 200); // checker
  texture->SetChecker(10, 0xFF9933, 0xFF94FF);
  //texture->show();

  // shadow mapping -------------------------------------------------------------------

  lightCam = new PPC(hfov, w, h);
  V3 lightPoint = V3(130, -100, -150);
  V3 objectCenter = V3(0.0f, 0.0f, -150.0f); // default teapot center
  lightCam->PositionAndOrient(lightPoint, objectCenter, V3(0, 1, 0));
  shadowMap = new FrameBuffer(0, 0, 512, 512);

  tmeshes[1] = new TMesh();
  tmeshes[1]->LoadBin("geometry/teapot1k.bin");
  V3 newCenter = V3(0.0f, 0.0f, -150.0f);
  tmeshes[1]->Position(newCenter);
  tmeshes[1]->enabled = true;
  tmeshes[1]->textureC = NULL;
  tmeshes[1]->shadowMapping(ppc, lightCam, fb, shadowMap);
  mesh = tmeshes[1];
  Render();

}

void Scene::Render() {

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
		renderM = 5;
		tmeshes[tmi]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, lookupM, tilingM);
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
	float step = 1.0f;
	ppc->UpDownTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, lookupM, tilingM);
	fb->redraw();
}

void Scene::Down() {
	float step = -1.0f;
	ppc->UpDownTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::Left() {
	float step = 1.0f;
	ppc->LeftRightTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::Right() {
	float step = -1.0f;
	ppc->LeftRightTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::Front() {
	float step = 1.0f;
	ppc->FrontBackTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::Back() {
	float step = -1.0f;
	ppc->FrontBackTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::Pan() {
	float deg = 1.0f;
	ppc->Pan(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::Tilt() {
	float deg = 1.0f;
	ppc->Tilt(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::Roll() {
	float deg = 1.0f;
	ppc->Roll(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::ZoomIn() {
	float zoom = 1.2f;
	ppc->Zoom(zoom);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

void Scene::ZoomOut() {
	float zoom = 0.8f;
	ppc->Zoom(zoom);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, 0, 0);
	fb->redraw();
}

const char* Scene::GetPointLightX() {/*
									 cerr << "--------" << endl;
									 if (gui->pointlightX == 0)
									 cerr << "gui->inputX is NULL" << endl;
									 if (gui->pointlightX->value() == 0)
									 cerr << "gui->inputX->value() is NULL" << endl; */
	return gui->pointlightX->value();
}
const char* Scene::GetPointLightY() {
	return gui->pointlightY->value();
}
const char* Scene::GetPointLightZ() {
	return gui->pointlightZ->value();
}
const char* Scene::ka() {
	return gui->ka->value();
}
const char* Scene::es() {
	return gui->es->value();
}


void Scene::SM1() {
	V3 L = V3(-300, 30, 80);
	fb->Clear(0xFFFFFFFF, 0.0f);
	renderM = 1;
	tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, 0, 0, 1, 0, 0);
	fb->redraw();
}

void Scene::SM2() {
	V3 LL = V3(-300, 30, -80);
	float ka = 0.8f;
	float es = 500;
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, LL, ka, es, 2, 0, 0);
	fb->redraw();
}

void Scene::SM3() {
	V3 LL = V3(-300, 30, -80);
	float ka = 0.8f;
	float es = 500;
	fb->Clear(0xFFFFFFFF, 0.0f);
	tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, ka, es, 3, 0, 0);
	fb->redraw();
}

void Scene::SM2Play() {
	V3 L = V3(-256.0f, 30.0f, -80.0f);
	float ka = 0.5;
	float es = 900;
	V3 move = V3(16, 0, 2);
	int step = 30;
	for (int i = 0; i < step; i++){
		fb->Clear(0xFFFFFFFF, 0.0f);
		tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, ka, es, 2, 0, 0);

		L = L + move;
		fb->redraw();
		Fl::check();

	}

}
void Scene::SM3Play() {
	V3 L = V3(-256.0f, 30.0f, -80.0f);
	float ka = 0.5;
	float es = 900;
	V3 move = V3(16, 0, 2);
	int step = 30;

	for (int i = 0; i < step; i++){
		fb->Clear(0xFFFFFFFF, 0.0f);
		renderM = 3;
		tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, ka, es, 3, 0, 0);

		L = L + move;
		fb->redraw();
		Fl::check();
	}
}

void Scene::Bilinear() {
	renderM = 4;
	lookupM = 2;
	tilingM = 0; //?
	cerr << "bilinear lookup! + ";
	if (tilingM == 1) {
		cerr << "repeat tiling!" << endl;
	}
	else if (tilingM == 2) {
		cerr << "mirror tiling!" << endl;
	}
	else {
		cerr << "no tiling mode!" << endl;
	}
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, lookupM, tilingM);
	fb->redraw();
}

void Scene::Neighbor() {
	renderM = 4;
	lookupM = 1;
	tilingM = 0;
	cerr << "neighbor lookup! + ";
	if (tilingM == 1) {
		cerr << "repeat tiling!" << endl;
	}
	else if (tilingM == 2) {
		cerr << "mirror tiling!" << endl;
	}
	else {
		cerr << "no tiling!" << endl;
	}
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, lookupM, tilingM);
	fb->redraw();
}

void Scene::Repeat() {
	renderM = 4;
	tilingM = 1;
	if (lookupM == 1) {
		cerr << "neighbor lookup!";
	}
	else if (lookupM == 2) {
		cerr << "bilinear lookup!";
	}
	else {
		cerr << "invalid lookup!";
	}
	cerr << " + repeat tiling!" << endl;
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, lookupM, tilingM);
	fb->redraw();
}

void Scene::Mirror() {
	renderM = 4;
	tilingM = 2;
	if (lookupM == 1) {
		cerr << "neighbor lookup!";
	}
	else if (lookupM == 2) {
		cerr << "bilinear lookup!";
	}
	else {
		cerr << "invalid lookup!";
	}
	cerr << " + mirror tiling!" << endl;
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, L, kaf, esf, renderM, lookupM, tilingM);
	fb->redraw();
}

void Scene::Shadow() {
	V3 L = V3(130, -110, 0);
	/*
	if (gui->pointLightx->value() && gui->pointLighty->value() && gui->pointLightz->value()){
		L[0] = atof(pointLightx());
		L[1] = atof(pointLighty());
		L[2] = atof(pointLightz());
	}*/
	FrameBuffer *shadowMap = new FrameBuffer(0, 0, 512, 512);

	// create two meshes
	tmeshes[2] = new TMesh();
	tmeshes[2]->LoadBin("geometry/teapot1k.bin");
	V3 newC = V3(-20.0f, 0.0f, -200.0f);
	tmeshes[2]->Position(newC);

	tmeshes[3] = new TMesh();
	tmeshes[3]->LoadBin("geometry/teapot1k.bin");
	newC = V3(20.0f, 0.0f, -250.0f);
	tmeshes[3]->Position(newC);

	// render shadow map
	FrameBuffer *shadowfb = fb;
	PPC *lightView = new PPC(130, -100, 0);
	lightView->C = L;
	fb->Clear(0xFFFFFFFF, 0);
	//tmeshes[2]->RenderFilled(lightView, lightView, fb, 0xFFFFFFFF, L, 0, 0, 5, 0, 0, shadowMap);
	//tmeshes[3]->RenderFilled(lightView, lightView, fb, 0xFFFFFFFF, L, 0, 0, 5, 0, 0, shadowMap);
	tmeshes[2]->shadowMapping(ppc, lightView, fb, shadowMap);
	tmeshes[3]->shadowMapping(ppc, lightView, fb, shadowMap);
	shadowMap->show();

	// render scene
	renderM = 5;
	tmeshes[2]->RenderFilled(ppc, lightView, fb, texture, shadowMap, 0xFF0000FF, L, 0, 0, renderM, lookupM, 3);
	tmeshes[3]->RenderFilled(ppc, lightView, fb, texture, shadowMap, 0xFF0000FF, L, 0, 0, renderM, lookupM, 3);

	fb->redraw();
}

/* Read filename */
const char* Scene::ReadFile(){
	//cerr << "The file name is: " << gui->Filename->value() << endl;
	cerr << "FILE OPEN: " << gui->Filename << endl;
	//return gui->Filename;
	const char* file1 = "C:\\Users\\Kathy Wang\\Documents\\Visual Studio 2013\\Projects\\InteractiveGraphics\\texture.tiff";
	//const char* file2 = "C:\\Users\\Kathy Wang\\Documents\\Visual Studio 2013\\Projects\\InteractiveGraphics\\rubik.tiff";
	return file1;
}

/* Load file button */
void Scene::LoadFile(){
	FrameBuffer *tiffImage;
	const char* file = ReadFile();
	if (file == NULL)
		cerr << "FILENAME EMPTY!" << endl;
	else{
		//FrameBuffer *newT = new FrameBuffer(0,0,100,100);
		TIFF *ret = texture->LoadTiff(file, texture);
		cerr << "LoadTiff return: " << ret << endl;
		texture->show();
		Render();
	}

}







