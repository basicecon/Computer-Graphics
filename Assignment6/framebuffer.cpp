#include "framebuffer.h"
#include "v3.h"
#include <iostream>
#include "scene.h"
#include "tiffio.h"
#include "assert.h"

extern "C" {
	FILE* _iob = NULL;
}

#define PI           3.14159265358979323846

FrameBuffer::FrameBuffer(int u0, int v0,
	int _w, int _h) : Fl_Gl_Window(u0, v0, _w, _h, 0) {

	w = _w;
	h = _h;
	pix = new unsigned int[w*h];
	zb = new float[w*h];

}


void FrameBuffer::draw() {

	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

}


void FrameBuffer::Set(unsigned int bgr) {

	for (int i = 0; i < w*h; i++) {
		pix[i] = bgr;
	}

}

void FrameBuffer::Set(int u, int v, unsigned int color) {

	pix[(h - 1 - v)*w + u] = color;

}

unsigned int FrameBuffer::Get(int u, int v) {
	//cerr << "u = " << u << ", v = " << v << endl;
	//cerr << pix[(h-1-v)*w+u] << endl;
	return pix[(h - 1 - v)*w + u];

}

void FrameBuffer::SetSafe(int u, int v, unsigned int color) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return;

	Set(u, v, color);

}


void FrameBuffer::SetChecker(int csize, unsigned int color0,
	unsigned int color1) {

	for (int v = 0; v < h; v++) {
		int cv = v / csize;
		for (int u = 0; u < w; u++) {
			int cu = u / csize;
			if ((cu + cv) % 2)
				Set(u, v, color1);
			else
				Set(u, v, color0);
		}
	}

}


void FrameBuffer::DrawPoint(int u, int v, int psize, unsigned int color) {

	for (int i = v - psize / 2; i <= v + psize / 2; i++) {
		for (int j = u - psize / 2; j <= u + psize / 2; j++) {
			SetSafe(j, i, color);
		}
	}

}


void FrameBuffer::DrawSegment(V3 pp0, V3 pp1, V3 c0, V3 c1) {

	float u0f = pp0[0];
	float u1f = pp1[0];
	float v0f = pp0[1];
	float v1f = pp1[1];

	float du = fabsf(u1f - u0f);
	float dv = fabsf(v1f - v0f);
	int stepsN = (du < dv) ? 1 + (int)dv : 1 + (int)du;

	V3 startingPoint(pp0);
	V3 endingPoint(pp1);
	V3 currentPoint;
	int segsN;
	if (stepsN == 1)
		segsN = 1;
	else
		segsN = stepsN - 1;
	V3 segmentStep = (endingPoint - startingPoint) / (float)segsN;
	V3 colorStep = (c1 - c0) / (float)segsN;
	int i;
	V3 cc;
	for (i = 0,
		currentPoint = startingPoint,
		cc = c0;
	i < stepsN;
	i++,
		currentPoint = currentPoint + segmentStep,
		cc = cc + colorStep) {
		int u = (int)currentPoint[0];
		int v = (int)currentPoint[1];
		if (IsOutsideFrame(u, v))
			continue;
		if (IsFarther(u, v, currentPoint[2]))
			continue;
		SetZ(u, v, currentPoint[2]);
		Set(u, v, cc.GetColor());
	}

}

void FrameBuffer::DrawDot(float u, float v, float r, unsigned int color) {
	for (float i = u - r; i <= u + r; i++) {
		for (float j = v - r; j <= v + r; j++) {
			if ((i - u)*(i - u) + (j - v)*(j - v) <= r*r) {
				this->DrawPoint(i, j, 1.0f, color);
			}
		}
	}
}

void FrameBuffer::DrawCircle(float cx, float cy, float r, float size, unsigned int color){
	float x, y, r2;
	r2 = r * r;
	DrawPoint(cx, cy + r, size, color);
	DrawPoint(cx, cy - r, size, color);
	DrawPoint(cx + r, cy, size, color);
	DrawPoint(cx - r, cy, size, color);

	y = r;
	x = 1.0f;
	y = (float)(sqrt(r2 - 1) + 0.5);

	while (x < y){
		DrawPoint(cx + x, cy + y, size, color);
		DrawPoint(cx + x, cy - y, size, color);
		DrawPoint(cx - x, cy + y, size, color);
		DrawPoint(cx - x, cy - y, size, color);
		DrawPoint(cx + y, cy + x, size, color);
		DrawPoint(cx + y, cy - x, size, color);
		DrawPoint(cx - y, cy + x, size, color);
		DrawPoint(cx - y, cy - x, size, color);
		x += 1;
		y = (float)(sqrt(r2 - x*x) + 0.5);
	}
	if (x == y){
		DrawPoint(cx + x, cy + y, size, color);
		DrawPoint(cx + x, cy - y, size, color);
		DrawPoint(cx - x, cy + y, size, color);
		DrawPoint(cx - x, cy - y, size, color);
	}
}


void FrameBuffer::Convolve33(M33 kernel, FrameBuffer *&fb1) {

	fb1 = new FrameBuffer(30 + w, 30, w, h);

	for (int v = 1; v < h - 1; v++) {
		for (int u = 1; u < w - 1; u++) {
			V3 newColor(0.0f, 0.0f, 0.0f);
			for (int vi = -1; vi <= 1; vi++) {
				for (int ui = -1; ui <= 1; ui++) {
					V3 currColor;
					currColor.SetFromColor(Get(u + ui, v + vi));
					newColor = newColor + currColor * kernel[vi + 1][ui + 1];
				}
			}
			unsigned int newc = newColor.GetColor();
			fb1->Set(u, v, newc);
		}
	}

}

// load tiff image file 
TIFF* FrameBuffer::LoadTiff(const char* fileName, FrameBuffer *&displayFb) {
	if (fileName == NULL) {
		cerr << "Please enter a valid file name" << endl;
		return NULL;
	}

	TIFF* tiffFile = TIFFOpen(fileName, "r");
	uint32 w, h;

	if (tiffFile) {
		TIFFGetField(tiffFile, TIFFTAG_IMAGEWIDTH, &w);
		TIFFGetField(tiffFile, TIFFTAG_IMAGELENGTH, &h);

		displayFb = new FrameBuffer(20, 20, w, h);

		if (!TIFFReadRGBAImage(tiffFile, w, h, displayFb->pix, 0)) {
			cerr << "ERROR READING IMAGE" << endl;
		}
		else {
			cout << "READING THE IMAGE..." << fileName << endl;
			//fb->redraw();

		}
		TIFFClose(tiffFile);
		return tiffFile;
	}
	else {
		cerr << "CANNOT OPEN FILE " << fileName << endl;
		exit(0);
	}
}

// save tiff image file

void FrameBuffer::SaveTiff(const char* fileName) {
	TIFF* tiffFile = TIFFOpen(fileName, "w");
	int sampleperpix = 4;
	char *image = (char*)pix;

	TIFFSetField(tiffFile, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(tiffFile, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(tiffFile, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(tiffFile, TIFFTAG_SAMPLESPERPIXEL, sampleperpix);
	TIFFSetField(tiffFile, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(tiffFile, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tiffFile, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(tiffFile, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tiffFile, w*sampleperpix));
	tsize_t linebytes = sampleperpix*w;
	unsigned char *buf = NULL;

	buf = (TIFFScanlineSize(tiffFile) <= linebytes) ? (unsigned char*)_TIFFmalloc(linebytes) : (unsigned char*)_TIFFmalloc(TIFFScanlineSize(tiffFile));

	for (uint32 row = 0; row < h; row++) {
		memcpy(buf, &image[(h - row - 1)*linebytes], linebytes);
		if (TIFFWriteScanline(tiffFile, buf, row, 0) < 0) {
			exit(0);
		}
	}
	(void)TIFFClose(tiffFile);
	if (buf) {
		_TIFFfree(buf);
	}
}

void FrameBuffer::AdjustBrightness(int percentage) {
	float factor = (float)percentage / 100.0f;
	V3 currColor, newColor;
	for (int i = 0; i < w - 1; i++) {
		for (int j = 0; j < h - 1; j++) {
			currColor.SetFromColor(Get(i, j));
			newColor = currColor*factor;
			unsigned int color = newColor.GetColor();
			Set(i, j, color);
		}
	}
}
/*
void FrameBuffer::MultiplyScanlineByFactor(uint32* scanline, uint32* tmp, uint32 factor) {
memcpy(tmp, scanline, sizeof(scanline));
for (uint32 i = 0; i < sizeof(tmp); i ++) {
tmp[i] *= factor;
}
memcpy(scanline, tmp, sizeof(tmp));
}
*/

void FrameBuffer::EdgeDetection(M33 k) {

	V3 currColor, newColor;
	unsigned int c;

	for (int u = 1; u < w - 1; u++){
		for (int v = 1; v < h - 1; v++){
			newColor = V3(0.0f, 0.0f, 0.0f);
			for (int du = -1; du <= 1; du++){
				for (int dv = -1; dv <= 1; dv++){
					currColor.SetFromColor(Get(u + du, v + dv));
					newColor = newColor + currColor*k[dv + 1][du + 1];
				}
			}
			c = newColor.GetColor();
			Set(u, v, c);
		}
	}
}

void FrameBuffer::Draw3DPoint(V3 pt, PPC *ppc, int psize, V3 color) {

	V3 ppt;
	if (!ppc->Project(pt, ppt))
		return;
	DrawPoint((int)ppt[0], (int)ppt[1], psize, color.GetColor());
}


void FrameBuffer::Draw3DSegment(V3 p0, V3 p1, PPC *ppc, V3 c0, V3 c1) {

	V3 pp0, pp1;
	if (!ppc->Project(p0, pp0))
		return;
	if (!ppc->Project(p1, pp1))
		return;
	DrawSegment(pp0, pp1, c0, c1);

}

bool FrameBuffer::IsOutsideFrame(int u, int v) {

	bool ret;
	ret = u < 0 || u > w - 1 || v < 0 || v > h - 1;
	return ret;

}

void FrameBuffer::Clear(unsigned int bgr, float z0) {

	Set(bgr);
	for (int i = 0; i < w*h; i++) {
		zb[i] = z0;
	}

}

void FrameBuffer::SetZ(int u, int v, float currz) {

	zb[(h - 1 - v)*w + u] = currz;

}

float FrameBuffer::GetZ(int u, int v) {
	return zb[(h - 1 - v)*w + u];
}

bool FrameBuffer::IsFarther(int u, int v, float currz) {

	if (zb[(h - 1 - v)*w + u] >= currz)
		return true;

	return false;

}

bool FrameBuffer::ApproximateEql(int u, int v, float currz) {
	float diff = zb[(h - 1 - v)*w + u] - currz;
	if (diff > 0) {
		if (diff < 0.05)
			return true;
	}
	else {
		if (diff < -0.05)
			return true;
	}
	return false;
}
