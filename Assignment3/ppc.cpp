
#include "ppc.h"
#include "m33.h"

#include "tmesh.h"
#include "framebuffer.h"

#include <fstream>

PPC::PPC(float hfov, int _w, int _h) : w(_w), h(_h) {

  C = V3(0.0f, 0.0f, 0.0f);
  a = V3(1.0f, 0.0f, 0.0f);
  b = V3(0.0f, -1.0f, 0.0f);
  float alpha = hfov / 2.0f * 3.1415926f / 180.0f;
  c = V3(-(float)w / 2.0f, (float)h / 2.0f, -(float)w / (2.0f *tanf(alpha)));

}


bool PPC::Project(V3 P, V3 &PP) {

  M33 cam;
  cam.SetColumn(0, a);
  cam.SetColumn(1, b);
  cam.SetColumn(2, c);

  V3 q = cam.Inverse() * (P-C);

  if (q[2] <= 0.0f)
    return false;

  PP[0] = q[0] / q[2];
  PP[1] = q[1] / q[2];
  PP[2] = 1.0f / q[2];
  return true;

}

void PPC::PositionAndOrient(V3 newC, V3 lap, V3 vpv) {


  V3 newVD = (lap - newC).UnitVector();

  V3 newa = (newVD & vpv).UnitVector();
  newa = newa * a.Length();

  V3 newb = (newVD & newa).UnitVector();
  newb = newb * b.Length();

  V3 newc = newVD*GetF() -newa*(float)w/2.0f
    -newb*(float)h/2.0f;

  a = newa;
  b = newb;
  c = newc;
  C = newC;

}

V3 PPC::GetVD() {

  V3 ret;
  ret = (a & b).UnitVector();
  return ret;

}

float PPC::GetF() {

  float ret;
  ret = GetVD()*c;
  return ret;

}

void PPC::Pan(float deg) {
	a = a.RotatePoint(C, C-b, deg);
	b = b.RotatePoint(C, C-b, deg);
	c = c.RotatePoint(C, C-b, deg);
	C = C;
}

void PPC::Tilt(float deg) {
	a = a.RotatePoint(C, C+a, deg);
	b = b.RotatePoint(C, C+a, deg);
	c = c.RotatePoint(C, C+a, deg);
	C = C;
}

void PPC::Roll(float deg) {
	V3 ab = a&b;
	a = a.RotatePoint(C, C+ab, deg);
	b = b.RotatePoint(C, C+ab, deg);
	c = c.RotatePoint(C, C+ab, deg);
	C = C;
}

void PPC::LeftRightTranslation(int steps) {
	a = a;
	b = b;
	c = c;
	C = C + a.UnitVector()*steps;
}

void PPC::UpDownTranslation(int steps) {
	a = a;
	b = b;
	c = c;
	C = b.UnitVector()*steps;
}

void PPC::FrontBackTranslation(int steps) {
	V3 ab = a&b;
	a = a;
	b = b;
	c = c;
	C = ab.UnitVector()*steps;
}

V3 PPC::GetPointOnImagePlane(float uf, float vf) {
	V3 ret;
	ret = C + c + a*uf + b*vf;
	return ret;
}

V3 PPC::GetPointOnFocalPlane(float uf, float vf, float f) {
	V3 ret;
	ret = C + (c + a*uf + b*vf) * (f/GetF());
	return ret;
}

float PPC::PPu() {
	float ret;
	ret = c*(-1) * a.UnitVector() / a.Length();
	return ret;
}

float PPC::PPv() {
	float ret;
	ret = c*(-1) * b.UnitVector() / b.Length();
	return ret;
}

void PPC::Zoom(float scf) {
	float newF = GetF() * scf;
	V3 vd = GetVD();
	a = a;
	b = b;
	c = a*(-1)*PPu() - b*PPv() + vd*newF;
	C = C;
}

float PPC::Hfov() {
	float hfov;
	hfov = 2 * atan(w / 2 * a.Length() / GetF());
	return hfov;
}

PPC PPC::Interpolate(PPC *ppc1, PPC ret, float i, float n) {
	//PPC ret;
	V3 C0, C1, retC;
	V3 a0, a1, reta;
	V3 b0, b1, retb;
	V3 retc;
	V3 vd0, vd1, retvd;

	C0 = C;
	C1 = ppc1->c;
	retc = C0 + (C1 - C0)*i / (n - 1);

	a0 = a;
	a1 = ppc1->a;
	reta = a0 + (a1 - a0)*i / (n - 1);

	retvd = vd0 + (vd1 - vd0)*i / (n - 1);

	b0 = b;
	b1 = ppc1->b;
	retb = (retvd & reta).UnitVector() * b.Length();

	retc = reta*(-1)*PPu() - retb*PPv() + retvd*GetF();

	PositionAndOrient(retC, retC + retvd, reta & retvd);
	
	ret.C = retC;
	ret.a = reta;
	ret.b = retb;
	ret.c = retc;

	return ret;
}

void PPC::SaveToText(char *fileName) {
	ofstream ofs(fileName);
	if (ofs.is_open()) {
		ofs << Hfov()
			<< w
			<< h
			<< C
			<< a
			<< b
			<< c;
		ofs.close();
	}
}

PPC PPC::LoadFromText(char *fileName) {
	ifstream ifs(fileName);
	float retHfov, retw, reth;
	V3 retC, reta, retb, retc;
	if (ifs.is_open()) {
		ifs >> retHfov
			>> retw
			>> reth
			>> retC
			>> reta
			>> retb
			>> retc;
		ifs.close();
	}

	PPC ret(retHfov, retw, reth);
	ret.C = retC;
	ret.a = reta;
	ret.b = retb;
	ret.c = retc;
	return ret;
}

void PPC::Visualize(PPC *ppc1, FrameBuffer *fb, unsigned int color) {
	V3 p1, p2, p3, p4;
	p1 = GetPointOnFocalPlane(0, 0, h);
	p2 = GetPointOnFocalPlane(w, 0, h);
	p3 = GetPointOnFocalPlane(w, h, h);
	p4 = GetPointOnFocalPlane(0, h, h);

	fb->Draw3DSegment(C, p1, ppc1, color);
	fb->Draw3DSegment(C, p2, ppc1, color);
	fb->Draw3DSegment(C, p3, ppc1, color);
	fb->Draw3DSegment(C, p4, ppc1, color);

	fb->Draw3DSegment(p1, p2, ppc1, color);
	fb->Draw3DSegment(p2, p3, ppc1, color);
	fb->Draw3DSegment(p3, p4, ppc1, color);
	fb->Draw3DSegment(p4, p1, ppc1, color);

}




