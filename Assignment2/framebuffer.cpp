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

  pix[(h-1-v)*w+u] = color;

}

unsigned int FrameBuffer::Get(int u, int v) {
	//cerr << "u = " << u << ", v = " << v << endl;
	//cerr << pix[(h-1-v)*w+u] << endl;
  return pix[(h-1-v)*w+u];

}

void FrameBuffer::SetSafe(int u, int v, unsigned int color) {

  if (u < 0 || u > w-1 || v < 0 || v > h-1)
    return;

  Set(u, v, color);

}


void FrameBuffer::SetChecker(int csize, unsigned int color0, 
  unsigned int color1) {

  for (int v = 0; v < h; v++) {
    int cv = v / csize;
    for (int u = 0; u < w; u++) {
      int cu = u / csize;
      if ( (cu+cv) % 2)
        Set(u, v, color1);
      else
        Set(u, v, color0);
    }
  }

}


void FrameBuffer::DrawPoint(int u, int v, int psize, unsigned int color) {

  for (int i = v - psize/2; i <= v + psize/2; i++) {
    for (int j = u - psize/2; j <= u + psize/2; j++) {
      SetSafe(j, i, color);
    }
  }

}

void FrameBuffer::DrawSegment(float u0f, float v0f, float u1f, float v1f, 
  unsigned int color) {

    float du = fabsf(u1f-u0f);
    float dv = fabsf(v1f-v0f);
    int stepsN = (du < dv) ? 1 + (int) dv : 1 + (int) du;

    V3 startingPoint(u0f, v0f, 0.0f);
    V3 endingPoint(u1f, v1f, 0.0f);
    V3 currentPoint = startingPoint;
    int segsN;
    if (stepsN == 1)
      segsN = 1;
    else
      segsN = stepsN-1;
    V3 segmentStep = (endingPoint - startingPoint) / (float) segsN;
    for (int i = 0; i < stepsN; i++) {
      int u = (int) currentPoint[0];
      int v = (int) currentPoint[1];
      SetSafe(u, v, color);
      currentPoint = currentPoint + segmentStep;
    }
}
void FrameBuffer::DrawCircle(float uf, float vf, float r, int alpha, unsigned int color) {
	float rad = alpha*PI/180;
	V3 startingPoint(uf, vf+r, 0.0f);
	V3 currentPrev = startingPoint;
	V3 currentNext(uf + r*sin(rad), vf + r*cos(rad), 0.0f);
	int stepsN = 360 / alpha;
	int currAlpha = alpha;
	float currRad = rad;

	for (int i = 0; i < stepsN; i ++) {
		this->DrawSegment(currentPrev[0], currentPrev[1], 
			currentNext[0], currentNext[1], color);
		currentPrev = currentNext;
		currAlpha += alpha;

		if (currAlpha <= 90) {
			currRad = currAlpha*PI/180;
			currentNext = V3(uf + r*sin(currRad), vf + r*cos(currRad), 0.0f);	
		} else if (currAlpha > 90 && currAlpha <= 180) {
			currRad = (currAlpha-90)*PI/180;
			currentNext = V3(uf + r*cos(currRad), vf - r*sin(currRad), 0.0f);
		} else if (currAlpha > 180 && currAlpha <= 270) {
			currRad = (270-currAlpha)*PI/180;
			currentNext = V3(uf - r*cos(currRad), vf - r*sin(currRad), 0.0f);
		} else {
			currRad = (360-currAlpha)*PI/180;
			currentNext = V3(uf - r*sin(currRad), vf + r*cos(currRad), 0.0f);
		}
	}
}

void FrameBuffer::DrawDot(float u, float v, float r, unsigned int color) {
	for (float i = u-r; i <= u+r; i ++) {
		for (float j = v-r; j <= v+r; j ++) {
			if ((i-u)*(i-u) + (j-v)*(j-v) <= r*r) {
				this->DrawPoint(i, j, 1.0f, color);
			}
		}
	}
}


void FrameBuffer::Convolve33(M33 kernel, FrameBuffer *&fb1) {

  fb1 = new FrameBuffer(30 + w, 30, w, h);

  for (int v = 1; v < h-1; v++) {
    for (int u = 1; u < w-1; u++) {
      V3 newColor(0.0f, 0.0f, 0.0f);
      for (int vi = -1; vi <= 1; vi++) {
        for (int ui = -1; ui <= 1; ui++) {
          V3 currColor;
          currColor.SetFromColor(Get(u+ui, v+vi));
          newColor = newColor + currColor * kernel[vi+1][ui+1];
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
		} else {
			cout << "READING THE IMAGE..." << fileName << endl;
			//fb->redraw();
			
		}
		TIFFClose(tiffFile);
		return tiffFile;
	} else {
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
	
	for (uint32 row = 0; row < h; row ++) {
		memcpy(buf, &image[(h-row-1)*linebytes], linebytes);
		if (TIFFWriteScanline(tiffFile, buf, row, 0) < 0) {
			exit(0);
		}
	}
	(void)TIFFClose(tiffFile);
	if (buf) {
		_TIFFfree(buf);
	}
}

// adjust brightness
/*
TIFF* FrameBuffer::AdjustBrightness(const char* fileName, int percentage, FrameBuffer *&fb) {
	float factor = (float)percentage / 100.0f;
	
	TIFF* tiffFile = TIFFOpen(fileName, "r");
	TIFF* output = TIFFOpen("processed.tif", "w");
	uint32 w, h;

	TIFFGetField(tiffFile, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tiffFile, TIFFTAG_IMAGELENGTH, &h);
	fb = new FrameBuffer(0, 0, w, h);
	//tdata_t buf = _TIFFmalloc(TIFFScanlineSize(tiffFile));
	int len = TIFFScanlineSize(tiffFile);
	uint32* scanline = new uint32[TIFFScanlineSize(tiffFile)];
	uint32* tmp = new uint32[len];

	for (uint32 i = 0; i < h; i ++) {
		//TIFFReadScanline(tiffFile, scanline, i);
		
		for (uint32 j = 0; j < w; j ++) {
			unsigned int color = V3(i, j, 0.0f).GetColor();
			unsigned int newColor = color * factor;
			fb->Set(i, j, newColor);
			
		}
		//MultiplyScanlineByFactor(scanline, tmp, factor);
		//TIFFWriteScanline(output, scanline, i);
	}
	return output;
}*/
void FrameBuffer::AdjustBrightness(int percentage) {
	float factor = (float)percentage / 100.0f;
	V3 currColor, newColor;
	for (int i = 0; i < w-1; i ++) {
		for (int j = 0; j < h-1; j ++) {
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
			for (int du = -1; du <= 1; du ++){
				for (int dv = -1; dv <= 1; dv ++){
					currColor.SetFromColor(Get(u + du, v + dv));
					newColor = newColor + currColor*k[dv + 1][du + 1];
				}
			}
			c = newColor.GetColor();
			Set(u, v, c);
		}
	}
}