#pragma once

#include "v3.h"
#include "framebuffer.h"
#include "ppc.h"


#include <array>



class TMesh {
public:
  bool enabled;
  V3 *verts, *normals, *cols, *textureC, *textureCC;
  int vertsN;
  unsigned int *tris;
  int trisN;
  AABB *aabb;
  FrameBuffer *textures;

  TMesh() : verts(0), vertsN(0), tris(0), trisN(0), 
    normals(0), cols(0), aabb(0), enabled(false) {};
  TMesh(V3 center, V3 dims, unsigned int color);
  TMesh(char *fileName);
  void LoadBin(char *fname);
  void RenderPoints(PPC *ppc, FrameBuffer *fb, int psize);
  void RenderWireframe(PPC *ppc, FrameBuffer *fb, unsigned int color);
  void SetAABB();
  void Translate(V3 tv);
  V3 GetCenter();
  void Position(V3 newCenter);
  void ScaleToNewDiagonal(float newDiagonal);
  void Scale(float scf);
  void Rotate(V3 aO, V3 aD, float thetad);

  void RenderFilled(PPC *ppc, PPC *lightSource, FrameBuffer *fb, FrameBuffer *texture, FrameBuffer *shadowMap,
	  unsigned int color, V3 L, float ka, float es, int renderMode, int lookupMode, int tilingMode);
  //void RenderFilled(PPC *ppc, FrameBuffer *fb, FrameBuffer *texture,
	  //unsigned int color, V3 L, float ka, float es, int renderMode, int lookupMode, int tilingMode);

  //clipping
  void ClipBbox(FrameBuffer *fb, AABB *bbox);
  std::array<float, 3> KD_perV(V3 lv, V3 *normals, int vi1, int vi2, int vi3);
  std::array<float, 3> KS_perV(V3 lv, V3 C, V3 *normals, int vi1, int vi2, int vi3, float e);

  float KD_perP(V3 lv, V3 n);
  float KS_perP(V3 lv, V3 C, V3 n, float e);

  V3 getSScolor(V3 *color, float ka, std::array<float, 3> kd, std::array<float, 3> ks, 
	  M33 ras_para, char *choice);

  V3 SSIVColor(V3 pv, V3 redABC, V3 greenABC, V3 blueABC);
  V3 SSIN(V3 pv, V3 nxABC, V3 nyABC, V3 nzABC);

  V3 getABC(M33 q, V3 r);
  void TextureMapping(float u, float v, float s, float t, FrameBuffer *fb, FrameBuffer *texture, int lookUpMode, int tilingMode);
  unsigned int neighborLookUp(float s, float t, FrameBuffer *texture);
  unsigned int bilinearLookUp(float s, float t, FrameBuffer *texture);

  unsigned int tilingRepeat(float s, float t, FrameBuffer *texture);
  unsigned int tilingMirror(float s, float t, FrameBuffer *texture);

  void shadowMapping(PPC *ppc, PPC *lightCam, FrameBuffer *fb, FrameBuffer *shadowMap);
  //void surfaceLighting(PPC *ppc, PPC *lightSource, FrameBuffer *fb, FrameBuffer *shadowMap, unsigned int color, float ka, float es, int renderMode);

  void edgeEqa_rastPara(V3 *pverts, int *vinds, V3 ee0, V3 ee1, V3 ee2, M33 rast_inv);
  V3 GetSMCoordinate(PPC *ppc, PPC* lightCam, V3 pv, V3 ABC);
};