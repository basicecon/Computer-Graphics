#include "scene.h"
#include "m33.h"

#include <iostream>
#include <string>
#include <sstream>
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
  DrawMyInitial();

}

void Scene::Render() {

    fb->redraw();

}

void Scene::DBG() {
/*
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
*/
	/*
  fb->DrawSegment(10.1f, 20.3f, 100.1f, 200.3f, 0xFF000000);
  Render();*/
  fb->DrawCircle(50.0f, 50.0f, 20.0f, 30.0f, 0xFF000000);
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


void Scene::LoadButton() {
	//char* fileName = NULL;
	//const char* fileName = "C:\\Users\\wang1247\\Documents\\Visual Studio 2012\\Projects\\InteractiveGraphics\\test.tif";
	cout << "Please Enter File Name: " << endl;
	std::string input;
	cin >> input;
	if (!input.empty()) {
		char* fileName = new char[input.length() + 1];
		strcpy(fileName, input.c_str());
		FrameBuffer* displayFb = new FrameBuffer(0, 0, 0, 0);
		TIFF* tiff = fb->LoadTiff(fileName, displayFb);
		delete [] fileName;
		displayFb->show();
		fb = displayFb;
	} else {
		cerr << "Invalid File Name" << endl;
		exit(0);
	}
	
	//Render();
	return;
}

void Scene::EdgeDetect() {
	M33 k;
	k[0] = V3(-1.0f, -1.0f, -1.0f);
	k[1] = V3(-1.0f, 8.0f, -1.0f);
	k[2] = V3(-1.0f, -1.0f, -1.0f);

	fb->EdgeDetection(k);
	Render();
	return;
}


void Scene::SaveButton() {
	cout << "Please enter file name you want to save to: " << endl;
	std::string input;
	cin >> input;
	if (!input.empty()) {
		char* fileName = new char[input.length() + 1];
		strcpy(fileName, input.c_str());
		fb->SaveTiff(fileName);
		delete [] fileName;
	} else {
		cerr << "Invalid File Name" << endl;
		exit(0);
	}
	return;	
}

void Scene::AdjBrightness() {
	/*
	cout << "Please Enter File Name: " << endl;
	std::string input;
	cin >> input;
	if (!input.empty()) {
		char* fileName = new char[input.length() + 1];
		strcpy(fileName, input.c_str());
		int input = (int)gui->AdjBrightness->value();
		TIFF* output = fb->AdjustBrightness(fileName, input, *&fb);
		delete [] fileName;
	} else {
		cerr << "Please enter a valid file name" << endl;
	}
	fb->show();
	return;	
	*/
	const char* input = gui->AdjBrightness->value();
	int percentage = atoi(input);
	cerr << "percentage = " << percentage << endl;
	fb->AdjustBrightness(percentage);
	Render(); // needs redraw!!!!
	return;
}

// MYW
void Scene::DrawMyInitial() {
	//first letter "M"
	int width = fb->w;
	int height = fb->h;

	for (float i = width/8; i < 3*width/16; i++){
		float j = -8*height*i/width + 7*height/4;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	for (float i = 3*width/16; i < width/4; i ++) {
		float j = 4*height*i/width - height/2;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	for (float i = width/4; i < 5*width/16; i ++) {
		float j = -4*height*i/width + 3*height/2;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	for (float i = 5*width/16; i <= 3*width/8; i ++) {
		float j = 8*height*i/width - 9*height/4;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	// second letter "Y"
	for (float i = 3*width/8; i < width/2; i ++) {
		float j = 2*height*i/width - height/2;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	for (float i = width/2; i < 5*width/8; i ++) {
		float j = -2*height*i/width + 3*height/2;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	float i = width/2;
	for (float j = height/2; j <= 3*height/4; j ++) {
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	// third letter "W"
	for (float i = 5*width/8; i < 11*width/16; i ++) {
		float j = 8*height*i/width - 19*height/4;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	for (float i = 11*width/16; i < 3*width/4; i ++) {
		float j = -4*height*i/width + 7*height/2;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	for (float i = 3*width/4; i < 13*width/16; i ++) {
		float j = 4*height*i/width - 5*height/2;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	for (float i = 13*width/16; i <= 7*width/8; i ++) {
		float j = -8*height*i/width + 29*height/4;
		fb->DrawDot(i, j, 7, 0xcc3399);
	}
	Render();
}















