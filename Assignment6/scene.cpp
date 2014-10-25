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
  V3 lightPoint = V3(130, 100, -150);
  V3 objectCenter = V3(0.0f, 0.0f, -150.0f); // default teapot center
  lightCam->PositionAndOrient(lightPoint, objectCenter, V3(0, 1, 0));
  shadowMap = new FrameBuffer(0, 0, 512, 512);

  tmeshes[1] = new TMesh();
  tmeshes[1]->LoadBin("geometry/teapot1k.bin");
  V3 newCenter = V3(0.0f, 0.0f, -150.0f);
  tmeshes[1]->Position(newCenter);
  tmeshes[1]->enabled = true;
  tmeshes[1]->textureC = NULL;
  tmeshes[1]->shadowMapping(lightCam, shadowMap);
  mesh = tmeshes[1];
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

		/*
		V3 L(-300, 30, 80);
		tmeshes[tmi]->RenderWireframe(ppc, fb, 0x00000000);
		renderM = 4; // texture mapping
		tmeshes[tmi]->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, renderM, lookupM, tilingM); */

		//tmeshes[tmi]->RenderFilled(ppc, fb, texture, 0xFF0000FF, V3(0,0,0), 0, 0, 1, 0, 0);
		
		tmeshes[tmi]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 5, 0, 0);
		//cerr << "done render.." << endl;
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
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::Down() {
	V3 L = V3(0, 0, 0);
	float step = -1.0f;
	ppc->UpDownTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::Left() {
	V3 L = V3(0, 0, 0);
	float step = 1.0f;
	ppc->LeftRightTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::Right() {
	V3 L = V3(0, 0, 0);
	float step = -1.0f;
	ppc->LeftRightTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::Front() {
	V3 L = V3(0, 0, 0);
	float step = 1.0f;
	ppc->FrontBackTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::Back() {
	V3 L = V3(0, 0, 0);
	float step = -1.0f;
	ppc->FrontBackTranslation(step);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::Pan() {
	V3 L = V3(0, 0, 0);
	float deg = 1.0f;
	ppc->Pan(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::Tilt() {
	V3 L = V3(0, 0, 0);
	float deg = 1.0f;
	ppc->Tilt(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::Roll() {
	V3 L = V3(0, 0, 0);
	float deg = 1.0f;
	ppc->Roll(deg);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::ZoomIn() {
	V3 L = V3(0, 0, 0);
	float zoom = 1.2f;
	ppc->Zoom(zoom);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
	fb->redraw();
}

void Scene::ZoomOut() {
	V3 L = V3(0, 0, 0);
	float zoom = 0.8f;
	ppc->Zoom(zoom);
	fb->Clear(0xFFFFFFFF, 0.0f);
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 0);
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
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 1);
	//tmeshes[1]->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0, 0, 1);
	renderM = 1;
	tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	fb->redraw();
}

void Scene::SM2() {
	cerr << "ENTERING SM2....!" << endl;
	//default light source info
	V3 L = V3(-300, 30, -80);
	float ka = 0.8f;
	float es = 500;

	fb->Clear(0xFFFFFFFF, 0.0f);
	cerr << "REACHED HERE!" << endl;
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, ka, es, 2);
	//tmeshes[1]->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, ka, es, 2);
	renderM = 2;
	tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	fb->redraw();
}

void Scene::SM3() {
	cerr << "ENTERING SM3....!" << endl;
	//default light source info
	V3 L = V3(-300, 30, -80);
	float ka = 0.8f;
	float es = 500;

	fb->Clear(0xFFFFFFFF, 0.0f);
	//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, ka, es, 3);
	//tmeshes[1]->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, ka, es, 3);
	renderM = 3;
	tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	fb->redraw();
}

void Scene::SM2Play() {
	cerr << "SM2 play!" << endl;
	V3 L = V3(-256.0f, 30.0f, -80.0f);
	V3 move = V3(16, 0, 2);
	int step = 30;
	for (int i = 0; i < step; i++){
		fb->Clear(0xFFFFFFFF, 0.0f);
		//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0.5, 900, 2);
		//tmeshes[1]->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0.5, 900, 2);
		renderM = 2;
		tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);

		L = L + move;
		fb->redraw();
		Fl::check();

	}

}
void Scene::SM3Play() {
	cerr << "SM3 play!" << endl;
	V3 L = V3(-256.0f, 30.0f, -80.0f);
	V3 move = V3(16, 0, 2);
	int step = 30;

	for (int i = 0; i < step; i++){
		fb->Clear(0xFFFFFFFF, 0.0f);
		//mesh->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0.5, 900, 3);
		//tmeshes[1]->RenderFilled(ppc, fb, texture, 0xFF0000FF, L, 0.5, 900, 3);
		renderM = 3;
		tmeshes[1]->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);

		L = L + move;
		fb->redraw();
		Fl::check();
	}/*
	 fb->Clear(0xFFFFFFFF, 0.0f);
	 tmeshes[1]->RenderFilled(ppc, fb, 0xFF0000FF, V3(-900.0f, 30.0f, -80.0f), 0.5, 200, 3);
	 fb->redraw();
	 _sleep(2000);
	 */
}

void Scene::Bilinear() {
	V3 L(0, 0, 0);
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
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	fb->redraw();
}

void Scene::Neighbor() {
	V3 L(0, 0, 0);
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
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	fb->redraw();
}

void Scene::Repeat() {
	V3 L(0, 0, 0);
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
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	fb->redraw();
}

void Scene::Mirror() {
	V3 L(0, 0, 0);
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
	mesh->RenderFilled(ppc, lightCam, fb, texture, shadowMap, 0x00000000, V3(0, 0, 0), 0, 0, 1, 0, 0);
	fb->redraw();
}
/*
void Scene::Browse() {
} */


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







