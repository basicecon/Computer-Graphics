#include "tmesh.h"

#include <fstream>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


TMesh::TMesh(V3 center, V3 dims, unsigned int color) {

  enabled = true;
  vertsN = 8;
  verts = new V3[vertsN];
  cols = new V3[vertsN];
  normals = new V3[vertsN];
  textureC = new V3[vertsN]; // texture coordinate s,t 
  textureCC = new V3[vertsN];  // texture coor for compressed texture
  textures = new FrameBuffer(0, 0, 0, 0);

  int vi = 0;
  verts[vi++] = center + V3(-dims[0]/2.0f, +dims[1]/2.0f, +dims[2]/2.0f);
  verts[vi++] = center + V3(-dims[0]/2.0f, -dims[1]/2.0f, +dims[2]/2.0f);
  verts[vi++] = center + V3(+dims[0]/2.0f, -dims[1]/2.0f, +dims[2]/2.0f);
  verts[vi++] = center + V3(+dims[0]/2.0f, +dims[1]/2.0f, +dims[2]/2.0f);

  verts[vi++] = center + V3(-dims[0]/2.0f, +dims[1]/2.0f, -dims[2]/2.0f);
  verts[vi++] = center + V3(-dims[0]/2.0f, -dims[1]/2.0f, -dims[2]/2.0f);
  verts[vi++] = center + V3(+dims[0]/2.0f, -dims[1]/2.0f, -dims[2]/2.0f);
  verts[vi++] = center + V3(+dims[0]/2.0f, +dims[1]/2.0f, -dims[2]/2.0f);

  for (int vi = 0; vi < vertsN; vi++) {
    cols[vi].SetFromColor(color);
  }

  trisN = 12;
  tris = new unsigned int[trisN * 3];
  int tri = 0;

  tris[3*tri+0] = 0;
  tris[3*tri+1] = 1;
  tris[3*tri+2] = 2;
  tri++;
  tris[3*tri+0] = 2;
  tris[3*tri+1] = 3;
  tris[3*tri+2] = 0;
  tri++;
  tris[3*tri+0] = 3;
  tris[3*tri+1] = 2;
  tris[3*tri+2] = 6;
  tri++;
  tris[3*tri+0] = 6;
  tris[3*tri+1] = 7;
  tris[3*tri+2] = 3;
  tri++;
  tris[3*tri+0] = 4;
  tris[3*tri+1] = 0;
  tris[3*tri+2] = 3;
  tri++;
  tris[3*tri+0] = 3;
  tris[3*tri+1] = 7;
  tris[3*tri+2] = 4;
  tri++;

  tris[3*tri+0] = 5;
  tris[3*tri+1] = 6;
  tris[3*tri+2] = 2;
  tri++;
  tris[3*tri+0] = 2;
  tris[3*tri+1] = 1;
  tris[3*tri+2] = 5;
  tri++;

  tris[3*tri+0] = 4;
  tris[3*tri+1] = 7;
  tris[3*tri+2] = 6;
  tri++;
  tris[3*tri+0] = 6;
  tris[3*tri+1] = 5;
  tris[3*tri+2] = 4;
  tri++;

  tris[3*tri+0] = 4;
  tris[3*tri+1] = 5;
  tris[3*tri+2] = 1;
  tri++;
  tris[3*tri+0] = 1;
  tris[3*tri+1] = 0;
  tris[3*tri+2] = 4;
  tri++;

  // random assigning s and t
  textureC[0][0] = 1.0; 
  textureC[0][1] = 1.0; 
  // four texture image in the front plane 2 * 2
  // textureC[0][0] = 2.0; 
  // textureC[0][1] = 2.0;
 
  textureC[1][0] = 0.0;
  textureC[1][1] = 1.0;

  textureC[2][0] = 0.0;
  textureC[2][1] = 0.0;

  textureC[3][0] = 1.0;
  textureC[3][1] = 0.0;
	
  textureC[4][0] = 1.0;
  textureC[4][1] = 0.0;

  textureC[5][0] = 0.0;
  textureC[5][1] = 0.0;

  textureC[6][0] = 0.0;
  textureC[6][1] = 1.0;

  textureC[7][0] = 1.0;
  textureC[7][1] = 1.0;

  // ------------------------
  textureCC[0][0] = 2.0;
  textureCC[0][1] = 2.0;

  textureCC[1][0] = 0.0;
  textureCC[1][1] = 2.0;

  textureCC[2][0] = 0.0;
  textureCC[2][1] = 0.0;

  textureCC[3][0] = 2.0;
  textureCC[3][1] = 0.0;

  textureCC[4][0] = 2.0;
  textureCC[4][1] = 0.0;

  textureCC[5][0] = 0.0;
  textureCC[5][1] = 0.0;

  textureCC[6][0] = 0.0;
  textureCC[6][1] = 1.0;

  textureCC[7][0] = 2.0;
  textureCC[7][1] = 2.0;
}

// constructor: load from file
TMesh::TMesh(char *fileName) {
	ifstream ifs(fileName, ios::binary);
	if (ifs.fail()) {
		cerr << "Cannot Open File" << endl;
		return;;
	}
	ifs.read((char*)&vertsN, sizeof(int));
	char yn;
	ifs.read(&yn, 1); // always xyz
	if (yn != 'y') {
		cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
		return;
	}
	if (verts)
		delete verts;
	verts = new V3[vertsN];

	ifs.read(&yn, 1); // cols 3 floats
	if (cols)
		delete cols;
	cols = 0;
	if (yn == 'y') {
		cols = new V3[vertsN];
	}
	ifs.read(&yn, 1); // normals 3 floats
	if (normals)
		delete normals;
	normals = 0;
	if (yn == 'y') {
		normals = new V3[vertsN];
	}

	ifs.read(&yn, 1); // texture coordinates 2 floats
	float *tcs = 0; // don't have texture coordinates for now
	if (tcs)
		delete tcs;
	tcs = 0;
	if (yn == 'y') {
		tcs = new float[vertsN * 2];
	}

	ifs.read((char*)verts, vertsN * 3 * sizeof(float)); // load verts

	if (cols) {
		ifs.read((char*)cols, vertsN * 3 * sizeof(float)); // load cols
	}

	if (normals)
		ifs.read((char*)normals, vertsN * 3 * sizeof(float)); // load normals

	if (tcs)
		ifs.read((char*)tcs, vertsN * 2 * sizeof(float)); // load texture coordinates

	ifs.read((char*)&trisN, sizeof(int));
	if (tris)
		delete tris;
	tris = new unsigned int[trisN * 3];
	ifs.read((char*)tris, trisN * 3 * sizeof(unsigned int)); // read tiangles

	ifs.close();

	cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fileName << endl;
	cerr << "      xyz " << ((cols) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

	SetAABB();
}


void TMesh::RenderPoints(PPC *ppc, FrameBuffer *fb, int psize) {

  for (int vi = 0; vi < vertsN; vi++) {
    fb->Draw3DPoint(verts[vi], ppc, psize, cols[vi]);
  }

}

void TMesh::RenderWireframe(PPC *ppc, FrameBuffer *fb, unsigned int color) {

	for (int tri = 0; tri < trisN; tri++) {
		int vinds[3];
		vinds[0] = tris[tri * 3 + 0];
		vinds[1] = tris[tri * 3 + 1];
		vinds[2] = tris[tri * 3 + 2];
		for (int vi = 0; vi < 3; vi++) {
			V3 col0, col1;
			if (cols) {
				col0 = cols[vinds[vi]];
				col1 = cols[vinds[(vi + 1) % 3]];
			}
			else {
				col0.SetFromColor(color);
				col1.SetFromColor(color);
			}
			fb->Draw3DSegment(verts[vinds[vi]], verts[vinds[(vi + 1) % 3]], ppc,
				col0, col1);
		}
	}
}

void TMesh::LoadBin(char *fname) {

  ifstream ifs(fname, ios::binary);
  if (ifs.fail()) {
    cerr << "INFO: cannot open file: " << fname << endl;
    return;
  }

  ifs.read((char*)&vertsN, sizeof(int));
  char yn;
  ifs.read(&yn, 1); // always xyz
  if (yn != 'y') {
    cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
    return;
  }
  if (verts)
    delete verts;
  verts = new V3[vertsN];

  ifs.read(&yn, 1); // cols 3 floats
  if (cols)
    delete cols;
  cols = 0;
  if (yn == 'y') {
    cols = new V3[vertsN];
  }
  ifs.read(&yn, 1); // normals 3 floats
  if (normals)
    delete normals;
  normals = 0;
  if (yn == 'y') {
    normals = new V3[vertsN];
  }

  ifs.read(&yn, 1); // texture coordinates 2 floats
  float *tcs = 0; // don't have texture coordinates for now
  if (tcs)
    delete tcs;
  tcs = 0;
  if (yn == 'y') {
    tcs = new float[vertsN*2];
  }

  ifs.read((char*)verts, vertsN*3*sizeof(float)); // load verts

  if (cols) {
    ifs.read((char*)cols, vertsN*3*sizeof(float)); // load cols
  }

  if (normals)
    ifs.read((char*)normals, vertsN*3*sizeof(float)); // load normals

  if (tcs)
    ifs.read((char*)tcs, vertsN*2*sizeof(float)); // load texture coordinates

  ifs.read((char*)&trisN, sizeof(int));
  if (tris)
    delete tris;
  tris = new unsigned int[trisN*3];
  ifs.read((char*)tris, trisN*3*sizeof(unsigned int)); // read tiangles

  ifs.close();

  cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fname << endl;
  cerr << "      xyz " << ((cols) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

  SetAABB();

  enabled = true;

}

void TMesh::SetAABB() {

  if (aabb) {
    delete aabb;
  }
  
  aabb = new AABB(verts[0]);

  for (int vi = 0; vi < vertsN; vi++)
    aabb->AddPoint(verts[vi]);

  cerr << "TMesh AABB: " << aabb->corners[0] << endl << aabb->corners[1] << endl;

}

void TMesh::Translate(V3 tv) {

  for (int vi = 0; vi < vertsN; vi++)
    verts[vi] = verts[vi] + tv;
  SetAABB();
}

V3 TMesh::GetCenter() {

  V3 ret = (aabb->corners[0] + aabb->corners[1])/2.0f;
  return ret;

}

void TMesh::Position(V3 newCenter) {

  V3 oldCenter = GetCenter();
  Translate(newCenter-oldCenter);
  SetAABB();

}

void TMesh::ScaleToNewDiagonal(float newDiagonal) {

  float oldDiagonal = (aabb->corners[1] - aabb->corners[0]).Length();
  float sf = newDiagonal / oldDiagonal;
  float Length();
  V3 oldCenter = GetCenter();
  Position(V3(0.0f, 0.0f, 0.0f));
  Scale(sf);
  Position(oldCenter);
  SetAABB();

}

void TMesh::Scale(float scf) {

  for (int vi = 0; vi < vertsN; vi++) {
    verts[vi] = verts[vi] * scf;
  }

  SetAABB();

}

void TMesh::Rotate(V3 aO, V3 aD, float thetad){
	for (int vi = 0; vi < vertsN; vi++){
		verts[vi] = verts[vi].RotatePointAboutArbitraryAxis(aO, aD, thetad);
	}
}

/*
void TMesh::RenderFilled(PPC *ppc, PPC *lightSource, 
						 FrameBuffer *fb, FrameBuffer *texture, FrameBuffer *shadowMap,
						 unsigned int color, V3 L, float ka, float es, 
						 int renderMode, int lookupMode, int tilingMode) {

	V3 *pverts = new V3[vertsN];
	V3 *spverts = new V3[vertsN];
	bool *viewProj_flag = new bool[vertsN];
	bool *shadProj_flag = new bool[vertsN];
	for (int vi = 0; vi < vertsN; vi++) {
		//Project vertices to view framebuffer
		viewProj_flag[vi] = ppc->Project(verts[vi], pverts[vi]);
		//Project vertices to shadow framebuffer
		shadProj_flag[vi] = lightSource->Project(verts[vi], spverts[vi]);
	}

	for (int tri = 0; tri < trisN; tri++) {
		int vinds[3];
		vinds[0] = tris[tri * 3 + 0];
		vinds[1] = tris[tri * 3 + 1];
		vinds[2] = tris[tri * 3 + 2];

		// Do not render triangle if any of its vertices had an invalid projection on view buffer
		if (!viewProj_flag[vinds[0]] || !viewProj_flag[vinds[1]] || !viewProj_flag[vinds[2]])
			continue;

		for (int vi = 0; vi < 3; vi++){
			V3 col0, col1;
			if (cols) {
				col0 = cols[vinds[vi]];
				col1 = cols[vinds[(vi + 1) % 3]];
			}
			else {
				col0.SetFromColor(color);
				col1.SetFromColor(color);
			}
		}
		// Compute bounding box aabb of projected vertices on view framebuffer
		AABB *bbox = new AABB(pverts[vinds[0]]);
		bbox->AddPoint(pverts[vinds[1]]);
		bbox->AddPoint(pverts[vinds[2]]);
		ClipBbox(fb, bbox);
		// on shadow framebuffer
		AABB *sbbox = new AABB(spverts[vinds[0]]);
		sbbox->AddPoint(spverts[vinds[1]]);
		sbbox->AddPoint(spverts[vinds[2]]);
		ClipBbox(shadowMap, sbbox);

		// Setup edge equations ee and rasterization parameter
		V3 ee0, ee1, ee2;
		M33 rast_inv;
		edgeEqa_rastPara(pverts, vinds, ee0, ee1, ee2, rast_inv);

		float r[3];
		V3 colors[3];

		// Setup screen space linear variation of depth: zABC on view framebuffer
		r[0] = pverts[vinds[0]][2];
		r[1] = pverts[vinds[1]][2];
		r[2] = pverts[vinds[2]][2];
		V3 rZ = V3(r[0], r[1], r[2]);
		V3 zABC = rast_inv*rZ;

		// zbuffer on shadow framebuffer
		r[0] = spverts[vinds[0]][2];
		r[1] = spverts[vinds[1]][2];
		r[2] = spverts[vinds[2]][2];
		V3 srZ = V3(r[0], r[1], r[2]);
		V3 szABC = rast_inv*srZ;


		// Get color of three vertex from fb
		if (cols) {
			colors[0].SetFromColor(cols[vinds[0]].GetColor());
			colors[1].SetFromColor(cols[vinds[1]].GetColor());
			colors[2].SetFromColor(cols[vinds[2]].GetColor());
		}
		else {
			colors[0].SetFromColor(color);
			colors[1].SetFromColor(color);
			colors[2].SetFromColor(color);
		}

		// Setup screen space linear variation of red: redABC	
		r[0] = colors[0][0];
		r[1] = colors[1][0];
		r[2] = colors[2][0];
		V3 rRed = V3(r[0], r[1], r[2]);
		V3 redABC = rast_inv*rRed;

		// Setup screen space linear variation of green: greenABC 
		r[0] = colors[0][1];
		r[1] = colors[1][1];
		r[2] = colors[2][1];
		V3 rGreen = V3(r[0], r[1], r[2]);
		V3 greenABC = rast_inv*rGreen;

		// Setup screen space linear variation of blue: blueABC
		r[0] = colors[0][2];
		r[1] = colors[1][2];
		r[2] = colors[2][2];
		V3 rBlue = V3(r[0], r[1], r[2]);
		V3 blueABC = rast_inv*rBlue;

		// Setup screen space linear variation of nx
		r[0] = normals[vinds[0]][0];
		r[1] = normals[vinds[1]][0];
		r[2] = normals[vinds[2]][0];
		V3 rnx = V3(r[0], r[1], r[2]);
		V3 nxABC = rast_inv*rnx;

		// Setup screen space linear variation of ny
		r[0] = normals[vinds[0]][1];
		r[1] = normals[vinds[1]][1];
		r[2] = normals[vinds[2]][1];
		V3 rny = V3(r[0], r[1], r[2]);
		V3 nyABC = rast_inv*rny;

		// Setup screen space linear variation of nz
		r[0] = normals[vinds[0]][2];
		r[1] = normals[vinds[1]][2];
		r[2] = normals[vinds[2]][2];
		V3 rnz = V3(r[0], r[1], r[2]);
		V3 nzABC = rast_inv*rnz;

		V3 rs, rt;
		// Setup model space -> s 	
		if (tilingMode == 0) {  // no tiling
			if (textureC) {
				r[0] = textureC[vinds[0]][0];
				r[1] = textureC[vinds[1]][0];
				r[2] = textureC[vinds[2]][0];
				rs = V3(r[0], r[1], r[2]);
				//V3 s = rast_inv*rs;

				// Setup model space -> t
				r[0] = textureC[vinds[0]][1];
				r[1] = textureC[vinds[1]][1];
				r[2] = textureC[vinds[2]][1];
				rt = V3(r[0], r[1], r[2]);
				//V3 t = rast_inv*rt;
			}
			else {
				rs = V3(0, 0, 0);
				rt = V3(0, 0, 0);
			}
		}
		else if (tilingMode == 1 || tilingMode == 2) {
			if (textureCC) {
				r[0] = textureCC[vinds[0]][0];
				r[1] = textureCC[vinds[1]][0];
				r[2] = textureCC[vinds[2]][0];
				rs = V3(r[0], r[1], r[2]);
				//V3 s = rast_inv*rs;

				// Setup screen space linear variation of t
				r[0] = textureCC[vinds[0]][1];
				r[1] = textureCC[vinds[1]][1];
				r[2] = textureCC[vinds[2]][1];
				rt = V3(r[0], r[1], r[2]);
				//V3 t = rast_inv*rt;
			}
			else {
				rs = V3(0, 0, 0);
				rt = V3(0, 0, 0);
			}
		}
		else {
			rs = V3(0, 0, 0);
			rt = V3(0, 0, 0);
		}
		

		// texture mapping, model space interpolation, aka correct interporlation
		// calculate D,E,F ------------------------------ 
		M33 V;
		V.SetColumn(0, verts[vinds[0]] - ppc->C);
		V.SetColumn(1, verts[vinds[1]] - ppc->C);
		V.SetColumn(2, verts[vinds[2]] - ppc->C);
		V = V.Inverse();

		M33 ABC;
		ABC.SetColumn(0, ppc->a);
		ABC.SetColumn(1, ppc->b);
		ABC.SetColumn(2, ppc->c);
		
		// get q matrix
		M33 q = V * ABC;
		float D = q[0][0] + q[1][0] + q[2][0];
		float E = q[0][1] + q[1][1] + q[2][1];
		float F = q[0][2] + q[1][2] + q[2][2];
		V3 DEF = V3(D, E, F);

		// for all rows v of aabb
		// for all columns u of row v

		V3 pv;
		for (int v = bbox->corners[0][1]; v < bbox->corners[1][1]; v++){
			for (int u = bbox->corners[0][0]; u < bbox->corners[1][0]; u++){
				// Current pixel is p(u,v)
				// Current pixel vector is pv(u+0.5,v+0.5,1.0)
				pv = V3(u + 0.5, v + 0.5, 1.0);
				// Check whether current pixel is inside triangle
				// if the pixel is on wrong side of any of the triangle edges
				// if(pv*ee0<0 || pv*ee1 || pv*ee2)
				// continue
				if (pv*ee0 < 0 || pv*ee1 < 0 || pv*ee2 < 0)
					continue;

				// Check whether triangle is closer than what was previously
				// seen at this pixel
				// currz = zABC * pv
				// if currz < ZB[p]
				// continue
				// ZB[p] = currz


				// zbuffer on view framebuffer
				float currz = zABC*pv;
				if (fb->IsOutsideFrame((int)u, (int)v))
					continue;
				if (fb->IsFarther((int)u, (int)v, currz))
					continue;
				fb->SetZ((int)u, (int)v, currz);

				// zbuffer (model space) on shadow framebuffer
				V3 zbABC = getABC(q, srZ);
				float smsCurrZ = (zbABC * pv) / (DEF * pv);

				// texture mapping (model space)
				V3 sABC = getABC(q, rs);
				V3 tABC = getABC(q, rt);
				float s = (sABC * pv) / (DEF * pv);
				float t = (tABC * pv) / (DEF * pv);

				// pixel is inside triangle and triangle is visible at pixel
				// compute color of pixel based on current triangle

				if (renderMode == 1){
					cerr << "." << endl;
					unsigned int c = SSIVColor(pv, redABC, greenABC, blueABC).GetColor();
					fb->Set(u, v, c);
				}
				else if (renderMode == 2){
					
					V3 P = ppc->UnProject(V3(u + 0.5, v + 0.5, currz));
					V3 lv = L - P;
					lv.Normalize();
					std::array<float, 3> kd = KD_perV(lv, normals, vinds[0], vinds[1], vinds[2]);
					std::array<float, 3> ks = KS_perV(lv, ppc->C, normals, vinds[0], vinds[1], vinds[2], es);

					//Setup Screen Space linear variation
					V3 lrRedABC = getSScolor(colors, ka, kd, ks, rast_inv, "red");
					V3 lrGreenABC = getSScolor(colors, ka, kd, ks, rast_inv, "green");
					V3 lrBlueABC = getSScolor(colors, ka, kd, ks, rast_inv, "blue");
				
					V3 newColor = SSIVColor(pv, lrRedABC, lrGreenABC, lrBlueABC);
					fb->Set(u, v, newColor.GetColor());
					
				}
				
				else if (renderMode == 3){
					//	surface point at current pixel P
					//	P = ppc->Unproject(u+0.5,v+0.5,currz)
					V3 P = ppc->UnProject(V3(u + 0.5, v + 0.5, currz));
					//	lv = (L-P).normalized()
					V3 lv = L - P;
					lv.Normalize();

					//	kd = lv * n; kd = (kd<0)?0:kd;
					V3 n = SSIN(pv, nxABC, nyABC, nzABC);
					float kd = KD_perP(lv, n);
					float ks = KS_perP(lv, ppc->C, n, es);

					//	FB[p] = color * (ka + (1-ka)*kd);
					V3 origColor = SSIVColor(pv, redABC, greenABC, blueABC);
					V3 newColor = origColor * (ka + (1 - ka)*kd + ks);
					fb->Set(u, v, newColor.GetColor());
				}
				else if (renderMode == 4) { // texture mapping
					//TextureMapping(u, v, s, t, fb, texture, 2, 1);
					TextureMapping(u, v, s, t, fb, texture, lookupMode, tilingMode);
				}
				else if (renderMode == 5) { // shadow mapping
					//cerr << smsCurrZ << ", " << shadowMap->GetZ((int)u, (int)v) << endl;
					if (smsCurrZ > shadowMap->GetZ((int)u, (int)v)) { // in the shadow
						unsigned int c = SSIVColor(pv, redABC, greenABC, blueABC).GetColor();
						fb->Set(u, v, c);
					}
					else { // surface to the light, brighters
						unsigned int c = V3(125, 0, 0).GetColor();
						fb->Set(u, v, c);
					}
				}
			}
		}
	}
	delete[]pverts;
}*/

void TMesh::RenderFilled(PPC *ppc, PPC *lightSource, 
	FrameBuffer *fb, FrameBuffer *texture, FrameBuffer *shadowMap,
	unsigned int color, V3 L, float ka, float es, int renderMode, int lookupMode, int tilingMode) {

	cerr << shadowMap->w << ", " << shadowMap->h << endl;
	V3 *pverts = new V3[vertsN];
	bool *proj_flag = new bool[vertsN];
	for (int vi = 0; vi < vertsN; vi++) {
		//Project verts[vi] to pverts[vi]
		proj_flag[vi] = ppc->Project(verts[vi], pverts[vi]);
	}

	for (int tri = 0; tri < trisN; tri++) {
		int vinds[3];
		vinds[0] = tris[tri * 3 + 0];
		vinds[1] = tris[tri * 3 + 1];
		vinds[2] = tris[tri * 3 + 2];

		// Do not render triangle if any of its vertices had an invalid projection
		if (!proj_flag[vinds[0]] || !proj_flag[vinds[1]] || !proj_flag[vinds[2]])
			continue;

		for (int vi = 0; vi < 3; vi++){
			V3 col0, col1;
			if (cols) {
				col0 = cols[vinds[vi]];
				col1 = cols[vinds[(vi + 1) % 3]];
			}
			else {
				col0.SetFromColor(color);
				col1.SetFromColor(color);
			}
		}
		// Compute bounding box aabb of projected vertices
		AABB *bbox = new AABB(pverts[vinds[0]]);
		bbox->AddPoint(pverts[vinds[1]]);
		bbox->AddPoint(pverts[vinds[2]]);

		// clip bbox within frame
		ClipBbox(fb, bbox);

		// Setup edge equations ee0, ee1, ee2
		float a[3], b[3], c[3];
		float x[3], y[3];
		x[0] = pverts[vinds[0]][0];
		x[1] = pverts[vinds[1]][0];
		x[2] = pverts[vinds[2]][0];
		y[0] = pverts[vinds[0]][1];
		y[1] = pverts[vinds[1]][1];
		y[2] = pverts[vinds[2]][1];

		//ee0
		a[0] = y[1] - y[0];
		b[0] = -x[1] + x[0];
		c[0] = -x[0] * y[1] + y[0] * x[1];
		V3 ee0 = V3(a[0], b[0], c[0]);


		//ee1
		a[1] = y[2] - y[1];
		b[1] = -x[2] + x[1];
		c[1] = -x[1] * y[2] + y[1] * x[2];
		V3 ee1 = V3(a[1], b[1], c[1]);

		//ee2
		a[2] = y[0] - y[2];
		b[2] = -x[0] + x[2];
		c[2] = -x[2] * y[0] + y[2] * x[0];
		V3 ee2 = V3(a[2], b[2], c[2]);

		//Setup rasterization parameter
		M33 rast, rast_inv;
		rast[0] = V3(x[0], y[0], 1);
		rast[1] = V3(x[1], y[1], 1);
		rast[2] = V3(x[2], y[2], 1);
		rast_inv = rast.Inverse();

		float r[3];
		V3 colors[3];

		// Setup screen space linear variation of depth: zABC 
		r[0] = pverts[vinds[0]][2];
		r[1] = pverts[vinds[1]][2];
		r[2] = pverts[vinds[2]][2];
		V3 rZ = V3(r[0], r[1], r[2]);
		V3 zABC = rast_inv*rZ;


		// Get color of three vertex from fb
		if (cols) {
			colors[0].SetFromColor(cols[vinds[0]].GetColor());
			colors[1].SetFromColor(cols[vinds[1]].GetColor());
			colors[2].SetFromColor(cols[vinds[2]].GetColor());
		}
		else {
			colors[0].SetFromColor(color);
			colors[1].SetFromColor(color);
			colors[2].SetFromColor(color);
		}

		// Setup screen space linear variation of red: redABC	
		r[0] = colors[0][0];
		r[1] = colors[1][0];
		r[2] = colors[2][0];
		V3 rRed = V3(r[0], r[1], r[2]);
		V3 redABC = rast_inv*rRed;

		// Setup screen space linear variation of green: greenABC 
		r[0] = colors[0][1];
		r[1] = colors[1][1];
		r[2] = colors[2][1];
		V3 rGreen = V3(r[0], r[1], r[2]);
		V3 greenABC = rast_inv*rGreen;

		// Setup screen space linear variation of blue: blueABC
		r[0] = colors[0][2];
		r[1] = colors[1][2];
		r[2] = colors[2][2];
		V3 rBlue = V3(r[0], r[1], r[2]);
		V3 blueABC = rast_inv*rBlue;

		// Setup screen space linear variation of nx
		r[0] = normals[vinds[0]][0];
		r[1] = normals[vinds[1]][0];
		r[2] = normals[vinds[2]][0];
		V3 rnx = V3(r[0], r[1], r[2]);
		V3 nxABC = rast_inv*rnx;

		// Setup screen space linear variation of ny
		r[0] = normals[vinds[0]][1];
		r[1] = normals[vinds[1]][1];
		r[2] = normals[vinds[2]][1];
		V3 rny = V3(r[0], r[1], r[2]);
		V3 nyABC = rast_inv*rny;

		// Setup screen space linear variation of nz
		r[0] = normals[vinds[0]][2];
		r[1] = normals[vinds[1]][2];
		r[2] = normals[vinds[2]][2];
		V3 rnz = V3(r[0], r[1], r[2]);
		V3 nzABC = rast_inv*rnz;

		V3 rs, rt;
		// Setup screen space linear variation of s 	
		if (tilingMode == 0) {  // no tiling
			if (textureC) {
				r[0] = textureC[vinds[0]][0];
				r[1] = textureC[vinds[1]][0];
				r[2] = textureC[vinds[2]][0];
				rs = V3(r[0], r[1], r[2]);
				//V3 s = rast_inv*rs;

				// Setup screen space linear variation of t
				r[0] = textureC[vinds[0]][1];
				r[1] = textureC[vinds[1]][1];
				r[2] = textureC[vinds[2]][1];
				rt = V3(r[0], r[1], r[2]);
				//V3 t = rast_inv*rt;
			}
			else {
				rs = V3(0, 0, 0);
				rt = V3(0, 0, 0);
			}
		}
		else if (tilingMode == 1 || tilingMode == 2) {
			if (textureCC) {
				r[0] = textureCC[vinds[0]][0];
				r[1] = textureCC[vinds[1]][0];
				r[2] = textureCC[vinds[2]][0];
				rs = V3(r[0], r[1], r[2]);
				//V3 s = rast_inv*rs;

				// Setup screen space linear variation of t
				r[0] = textureCC[vinds[0]][1];
				r[1] = textureCC[vinds[1]][1];
				r[2] = textureCC[vinds[2]][1];
				rt = V3(r[0], r[1], r[2]);
				//V3 t = rast_inv*rt;
			}
			else {
				rs = V3(0, 0, 0);
				rt = V3(0, 0, 0);
			}
		}
		else {
			rs = V3(0, 0, 0);
			rt = V3(0, 0, 0);
		}

		r[0] = pverts[vinds[0]][2]; // if change to spverts -> change view position to shadow map
		r[1] = pverts[vinds[1]][2];
		r[2] = pverts[vinds[2]][2];
		V3 mrZ = V3(r[0], r[1], r[2]);  // for model space 

		// texture mapping, model space interpolation, aka correct interporlation
		// calculate D,E,F ------------------------------ 
		M33 V;
		V.SetColumn(0, verts[vinds[0]] - lightSource->C);
		V.SetColumn(1, verts[vinds[1]] - lightSource->C);
		V.SetColumn(2, verts[vinds[2]] - lightSource->C);
		V = V.Inverse();

		M33 ABC;
		ABC.SetColumn(0, lightSource->a);
		ABC.SetColumn(1, lightSource->b);
		ABC.SetColumn(2, lightSource->c);

		// get q matrix
		M33 q = V * ABC;
		float D = q[0][0] + q[1][0] + q[2][0];
		float E = q[0][1] + q[1][1] + q[2][1];
		float F = q[0][2] + q[1][2] + q[2][2];
		V3 DEF = V3(D, E, F);

		// for all rows v of aabb
		// for all columns u of row v

		V3 pv;
		for (int v = bbox->corners[0][1]; v < bbox->corners[1][1]; v++){
			for (int u = bbox->corners[0][0]; u < bbox->corners[1][0]; u++){
				// Current pixel is p(u,v)
				// Current pixel vector is pv(u+0.5,v+0.5,1.0)
				pv = V3(u + 0.5, v + 0.5, 1.0);
				// Check whether current pixel is inside triangle
				// if the pixel is on wrong side of any of the triangle edges
				// if(pv*ee0<0 || pv*ee1 || pv*ee2)
				// continue
				if (pv*ee0 < 0 || pv*ee1 < 0 || pv*ee2 < 0)
					continue;

				// Check whether triangle is closer than what was previously
				// seen at this pixel
				// currz = zABC * pv
				// if currz < ZB[p]
				// continue
				// ZB[p] = currz

				float currz = zABC*pv;
				if (fb->IsOutsideFrame((int)u, (int)v))
					continue;
				if (fb->IsFarther((int)u, (int)v, currz))
					continue;
				fb->SetZ((int)u, (int)v, currz);

				// model space interpolation -> zbuffer
				//V3 zbABC = getABC(q, mrZ);
				V3 zbABC = getABC(q, rZ);
				float msCurrZ = (zbABC * pv) / (DEF * pv);

				/*texture mapping ----------------------------------------------------------------------*/
				// find s,t for each pixel
				V3 sABC = getABC(q, rs);
				V3 tABC = getABC(q, rt);
				float s = (sABC * pv) / (DEF * pv);
				float t = (tABC * pv) / (DEF * pv);

				// pixel is inside triangle and triangle is visible at pixel
				// compute color of pixel based on current triangle

				if (renderMode == 1){
					unsigned int c = SSIVColor(pv, redABC, greenABC, blueABC).GetColor();
					fb->Set(u, v, c);
				}
				else if (renderMode == 2){

					V3 P = ppc->UnProject(V3(u + 0.5, v + 0.5, currz));
					V3 lv = L - P;
					lv.Normalize();
					std::array<float, 3> kd = KD_perV(lv, normals, vinds[0], vinds[1], vinds[2]);
					std::array<float, 3> ks = KS_perV(lv, ppc->C, normals, vinds[0], vinds[1], vinds[2], es);

					//Setup Screen Space linear variation
					V3 lrRedABC = getSScolor(colors, ka, kd, ks, rast_inv, "red");
					V3 lrGreenABC = getSScolor(colors, ka, kd, ks, rast_inv, "green");
					V3 lrBlueABC = getSScolor(colors, ka, kd, ks, rast_inv, "blue");

					V3 newColor = SSIVColor(pv, lrRedABC, lrGreenABC, lrBlueABC);
					fb->Set(u, v, newColor.GetColor());

				}

				else if (renderMode == 3){
					//	surface point at current pixel P
					//	P = ppc->Unproject(u+0.5,v+0.5,currz)
					V3 P = ppc->UnProject(V3(u + 0.5, v + 0.5, currz));
					//	lv = (L-P).normalized()
					V3 lv = L - P;
					lv.Normalize();

					//	kd = lv * n; kd = (kd<0)?0:kd;
					V3 n = SSIN(pv, nxABC, nyABC, nzABC);
					float kd = KD_perP(lv, n);
					float ks = KS_perP(lv, ppc->C, n, es);

					//	FB[p] = color * (ka + (1-ka)*kd);
					V3 origColor = SSIVColor(pv, redABC, greenABC, blueABC);
					V3 newColor = origColor * (ka + (1 - ka)*kd + ks);
					fb->Set(u, v, newColor.GetColor());
				}
				else if (renderMode == 4) { // texture mapping
					//TextureMapping(u, v, s, t, fb, texture, 2, 1);
					TextureMapping(u, v, s, t, fb, texture, lookupMode, tilingMode);

				}
				else if (renderMode == 5) {
					if (msCurrZ > shadowMap->GetZ((int)u, (int)v)) {
						unsigned int c = SSIVColor(pv, redABC, greenABC, blueABC).GetColor();
						fb->Set(u, v, c);
					}
					else { // the pixel is brighter
						unsigned int c = V3(125, 0, 0).GetColor();
						fb->Set(u, v, c);

					}
				}
			}
		}
	}
	delete[]pverts;
}

// Clip aabb with frame
void TMesh::ClipBbox(FrameBuffer *fb, AABB *bbox) {
	
	if (fb->IsOutsideFrame(bbox->corners[0][0], bbox->corners[0][1])){
		if (bbox->corners[0][0] < 0)
			bbox->corners[0][0] = 0;
		else if (bbox->corners[0][0]>fb->w)
			bbox->corners[0][0] = fb->w - 1;
		if (bbox->corners[0][1] < 0)
			bbox->corners[0][1] = 0;
		else if (bbox->corners[0][1] > fb->h)
			bbox->corners[0][1] = fb->h - 1;
	}
	else if (fb->IsOutsideFrame(bbox->corners[1][0], bbox->corners[1][1])){
		if (bbox->corners[1][0] < 0)
			bbox->corners[1][0] = 0;
		else if (bbox->corners[1][0]>fb->w)
			bbox->corners[1][0] = fb->w - 1;
		if (bbox->corners[1][1] < 0)
			bbox->corners[1][1] = 0;
		else if (bbox->corners[1][1] > fb->h)
			bbox->corners[1][1] = fb->h - 1;
	}
}

// calculate diffuse reflection coefficient kd
std::array<float, 3> TMesh::KD_perV(V3 lv, V3 *normals, int vi1, int vi2, int vi3) {
	
    std::array<float, 3> kd;
	//lv.Normalize();
	kd[0] = lv * normals[vi1];
	kd[1] = lv * normals[vi2];
	kd[2] = lv * normals[vi3];
	kd[0] = (kd[0] < 0) ? 0 : kd[0];
	kd[1] = (kd[1] < 0) ? 0 : kd[1];
	kd[2] = (kd[2] < 0) ? 0 : kd[2];

	return kd;
}

// calculate ks
std::array<float, 3> TMesh::KS_perV(V3 lv, V3 C, V3 *normals, int vi1, int vi2, int vi3, float e){

	std::array<float, 3> ks;
	float lr1 = lv*(C - (normals[vi1] * 2)*(normals[vi1] * C));
	float lr2 = lv*(C - (normals[vi1] * 2)*(normals[vi2] * C));
	float lr3 = lv*(C - (normals[vi1] * 2)*(normals[vi3] * C));
	
	ks[0] = pow(lr1, e);
	ks[1] = pow(lr2, e);	
	ks[2] = pow(lr3, e);

	return ks;
}

float TMesh::KD_perP(V3 lv, V3 n) {
	float kd = lv * n;
	kd = (kd < 0) ? 0 : kd;
	return kd;
}

float TMesh::KS_perP(V3 lv, V3 C, V3 n, float e) {
	float ks = lv*(C - (n * 2)*(n * C));
	return ks;
}

V3 TMesh::getSScolor(V3 *colors, float ka, std::array<float, 3> kd, std::array<float, 3> ks, 
	M33 ras_para, char *choice) {

	char *red = "red";
	char *green = "green";
	char *blue = "blue";
	V3 ret;

	if (strcmp(choice, red) == 0) {
		ret[0] = colors[0][0] * (ka + (1 - ka) * kd[0] + ks[0]);
		ret[1] = colors[1][0] * (ka + (1 - ka) * kd[1] + ks[1]);
		ret[2] = colors[2][0] * (ka + (1 - ka) * kd[2] + ks[2]);
		
	}
	else if (strcmp(choice, green) == 0) {
		ret[0] = colors[0][1] * (ka + (1 - ka) * kd[0] + ks[0]);
		ret[1] = colors[1][1] * (ka + (1 - ka) * kd[1] + ks[1]);
		ret[2] = colors[2][1] * (ka + (1 - ka) * kd[2] + ks[2]);
	}
	else if (strcmp(choice, blue) == 0) {
		ret[0] = colors[0][2] * (ka + (1 - ka) * kd[0] + ks[0]);
		ret[1] = colors[1][2] * (ka + (1 - ka) * kd[1] + ks[1]);
		ret[2] = colors[2][2] * (ka + (1 - ka) * kd[2] + ks[2]);
	}
	else {
		cerr << "error when getting color ABC!" << endl;
	}
	ret = ras_para * ret;
	return ret;

}




// screen space interpolation -- get color
V3 TMesh::SSIVColor(V3 pv, V3 redABC, V3 greenABC, V3 blueABC){
	// if rendering mode is vertex color interpolation = SM1
	//	ssiRed = redABC * pv;
	float ssiRed = redABC * pv;
	//	ssiGreen = greenABC * pv
	float ssiGreen = greenABC * pv;
	//	ssiBlue = blueABC * pv
	float ssiBlue = blueABC * pv;
	//	FB[p] = (ssiRed, ssiGreen, ssiBlue);
	V3 ret = V3(ssiRed, ssiGreen, ssiBlue);
	return ret;
}

// getting normal
V3 TMesh::SSIN(V3 pv, V3 nxABC, V3 nyABC, V3 nzABC){
	// if redering mode is per pixel diffuse lighting = SM3
	V3 n;
	//	n.x = nxABC * pv
	n[0] = nxABC * pv;
	//	n.y = nyABC * pv
	n[1] = nyABC * pv;
	//	n.z = nzABC * pv
	n[2] = nzABC * pv;
	//	normalize n
	n.Normalize();
	return n;
}


V3 TMesh::getABC(M33 q, V3 r) {
	float rA = q[0][0] * r[0] + q[1][0] * r[1] + q[2][0] * r[2];
	float rB = q[0][1] * r[0] + q[1][1] * r[1] + q[2][1] * r[2];
	float rC = q[0][2] * r[0] + q[1][2] * r[1] + q[2][2] * r[2];
	
	V3 rABC = V3(rA, rB, rC);
	return rABC;
}

void TMesh::TextureMapping(float u, float v, float s, float t, 
	FrameBuffer *fb, FrameBuffer *texture, int lookUpMode, int tilingMode) {
	unsigned int color;
	if (lookUpMode == 1) { // neighbor lookup
		if (tilingMode == 1) { // repeat
			color = tilingRepeat(s, t, texture);
		}
		else if (tilingMode == 2) { // mirror
			color = tilingMirror(s, t, texture);
		}
		else { // no tiling
			color = neighborLookUp(s, t, texture);;
		}
	}
	else if (lookUpMode == 2) { // bilinear lookup
		if (tilingMode == 1) {
			color = tilingRepeat(s, t, texture);
		} 
		else if (tilingMode == 2) {
			color = tilingMirror(s, t, texture);
		}
		else { // no tiling
			color = bilinearLookUp(s, t, texture);
		}
	}
	else {
		color = 0x00000000;
	}
	fb->Set(u, v, color);
}

unsigned int TMesh::neighborLookUp(float s, float t, FrameBuffer *texture) {
	float uu = s * texture->w;
	float vv = t * texture->h;

	float x, y;
	if (uu-(int)uu <= 0.5) {
		x = (int)uu;
	} else {
		x = (int)(uu + 1);
	}
	if (vv-(int)vv <= 0.5) {
		y = (int)vv;
	} else {
		y = (int)(vv + 1);
	}
	unsigned int color = texture->Get(x, y);
	//fb->Set(u, v, color);
	//cerr << "(" << s << ", " << t << "), " << "(" << uu << ", " << vv << ") " << "____" << " (" << x << ", " << y << "), color = " << color << endl;
	return color;
}

unsigned int TMesh::bilinearLookUp(float s, float t, FrameBuffer *texture) {
	float uu = s * texture->w;
	float vv = t * texture->h;
	int x = (int)uu;
	int y = (int)vv;
	float u_ratio = uu - x; float u_oppo = 1 - u_ratio;
	float v_ratio = vv - y; float v_oppo = 1 - v_ratio;
	unsigned int color;
	unsigned int ul = texture->Get(x, y);
	unsigned int ur = texture->Get(x + 1, y);
	unsigned int ll = texture->Get(x, y + 1);
	unsigned int lr = texture->Get(x + 1, y + 1);
	color = (ul*u_oppo + ur*u_ratio) * v_oppo + (ll*u_oppo + lr*u_ratio) * v_ratio;
	//fb->Set(u, v, color);
	return color;
}

/*

|-----|-----|
|     |   * |
|-----|-----|

*/
unsigned int TMesh::tilingMirror(float s, float t, FrameBuffer *texture) {
	float uu, vv;

	if (s > 1) {
		uu = ((int)(s+1) - s) * texture->w;
	}
	else if (s < 0) {
		s = -s;
		uu = ((int)(s + 1) - s) * texture->w;
	}
	else { // normal
		uu = s * texture->w;
	}

	if (t > 1) {
		vv = ((int)(t + 1) - t) * texture->h;
	}
	else if (t < 0) {
		t = -t;
		vv = ((int)(t + 1) - t) * texture->h;
	}
	else { // normal
		vv = t * texture->h;
	}
	unsigned int color = texture->Get(uu, vv);
	//fb->Set(u, v, color);
	return color;
}

unsigned int TMesh::tilingRepeat(float s, float t, FrameBuffer *texture) {
	float uu, vv;

	if (s > 1) {
		uu = (s - (int)s) * texture->w;
	}
	else if (s < 0) {
		uu = ( (int)(-s+1)+s ) * texture->w;
	}
	else { // normal
		uu = s * texture->w;
	}
	if (t > 1) {
		vv = (t - (int)t) * texture->h;
	}
	else if (t < 0) {
		vv = ((int)(-t + 1)+t) * texture->h;
	}
	else { // normal
		vv = t * texture->h;
	}

	unsigned int color = texture->Get(uu, vv);
	//fb->Set(u, v, color);
	return color;
}


void TMesh::shadowMapping(PPC *lightCam, FrameBuffer *shadowMap) {

	V3 *pverts = new V3[vertsN];
	bool *proj_flag = new bool[vertsN];
	for (int vi = 0; vi < vertsN; vi++) {
		proj_flag[vi] = lightCam->Project(verts[vi], pverts[vi]);
	}

	for (int tri = 0; tri < trisN; tri++) {
		int vinds[3];
		vinds[0] = tris[tri * 3 + 0];
		vinds[1] = tris[tri * 3 + 1];
		vinds[2] = tris[tri * 3 + 2];

		// Do not render triangle if any of its vertices had an invalid projection
		if (!proj_flag[vinds[0]] || !proj_flag[vinds[1]] || !proj_flag[vinds[2]])
			continue;

		// Compute bounding box aabb of projected vertices
		AABB *bbox = new AABB(pverts[vinds[0]]);
		bbox->AddPoint(pverts[vinds[1]]);
		bbox->AddPoint(pverts[vinds[2]]);

		// clip bbox within frame
		ClipBbox(shadowMap, bbox);

		// Setup edge equations ee0, ee1, ee2
		float a[3], b[3], c[3];
		float x[3], y[3];
		x[0] = pverts[vinds[0]][0];
		x[1] = pverts[vinds[1]][0];
		x[2] = pverts[vinds[2]][0];
		y[0] = pverts[vinds[0]][1];
		y[1] = pverts[vinds[1]][1];
		y[2] = pverts[vinds[2]][1];

		//ee0
		a[0] = y[1] - y[0];
		b[0] = -x[1] + x[0];
		c[0] = -x[0] * y[1] + y[0] * x[1];
		V3 ee0 = V3(a[0], b[0], c[0]);


		//ee1
		a[1] = y[2] - y[1];
		b[1] = -x[2] + x[1];
		c[1] = -x[1] * y[2] + y[1] * x[2];
		V3 ee1 = V3(a[1], b[1], c[1]);

		//ee2
		a[2] = y[0] - y[2];
		b[2] = -x[0] + x[2];
		c[2] = -x[2] * y[0] + y[2] * x[0];
		V3 ee2 = V3(a[2], b[2], c[2]);

		//Setup rasterization parameter
		M33 rast, rast_inv;
		rast[0] = V3(x[0], y[0], 1);
		rast[1] = V3(x[1], y[1], 1);
		rast[2] = V3(x[2], y[2], 1);
		rast_inv = rast.Inverse();

		float r[3];
		// Setup screen space linear variation of depth: zABC 
		r[0] = pverts[vinds[0]][2];
		r[1] = pverts[vinds[1]][2];
		r[2] = pverts[vinds[2]][2];
		V3 rZ = V3(r[0], r[1], r[2]);  // for model space 
		V3 zABC = rast_inv*rZ; // for screen space

		// calculate D,E,F ------------------------------ 
		M33 V;
		V.SetColumn(0, verts[vinds[0]] - lightCam->C);
		V.SetColumn(1, verts[vinds[1]] - lightCam->C);
		V.SetColumn(2, verts[vinds[2]] - lightCam->C);
		V = V.Inverse();

		M33 ABC;
		ABC.SetColumn(0, lightCam->a);
		ABC.SetColumn(1, lightCam->b);
		ABC.SetColumn(2, lightCam->c);

		// get q matrix
		M33 q = V * ABC;
		float D = q[0][0] + q[1][0] + q[2][0];
		float E = q[0][1] + q[1][1] + q[2][1];
		float F = q[0][2] + q[1][2] + q[2][2];
		V3 DEF = V3(D, E, F);

		// for all rows v of aabb
		// for all columns u of row v

		V3 pv;
		for (int v = bbox->corners[0][1]; v < bbox->corners[1][1]; v++){
			for (int u = bbox->corners[0][0]; u < bbox->corners[1][0]; u++){
				// Current pixel is p(u,v)
				// Current pixel vector is pv(u+0.5,v+0.5,1.0)
				pv = V3(u + 0.5, v + 0.5, 1.0);

				if (pv*ee0 < 0 || pv*ee1 < 0 || pv*ee2 < 0)
					continue;

				// screen space interpolation -> zbuffer
				float ssCurrZ = zABC*pv;
				// model space interpolation -> zbuffer
				V3 zABC = getABC(q, rZ);
				float msCurrZ = (zABC * pv) / (DEF * pv);
				
				if (shadowMap->IsOutsideFrame((int)u, (int)v))
					continue;
				if (shadowMap->IsFarther((int)u, (int)v, msCurrZ)) // model space for now
					continue;
				shadowMap->SetZ((int)u, (int)v, msCurrZ);
			}

		}
	}
}

void TMesh::edgeEqa_rastPara(V3 *pverts, int *vinds, V3 ee0, V3 ee1, V3 ee2, M33 rast_inv) {
	// Setup edge equations ee0, ee1, ee2
	float a[3], b[3], c[3];
	float x[3], y[3];
	x[0] = pverts[vinds[0]][0];
	x[1] = pverts[vinds[1]][0];
	x[2] = pverts[vinds[2]][0];
	y[0] = pverts[vinds[0]][1];
	y[1] = pverts[vinds[1]][1];
	y[2] = pverts[vinds[2]][1];

	//ee0
	a[0] = y[1] - y[0];
	b[0] = -x[1] + x[0];
	c[0] = -x[0] * y[1] + y[0] * x[1];
	ee0 = V3(a[0], b[0], c[0]);

	//ee1
	a[1] = y[2] - y[1];
	b[1] = -x[2] + x[1];
	c[1] = -x[1] * y[2] + y[1] * x[2];
	ee1 = V3(a[1], b[1], c[1]);

	//ee2
	a[2] = y[0] - y[2];
	b[2] = -x[0] + x[2];
	c[2] = -x[2] * y[0] + y[2] * x[0];
	ee2 = V3(a[2], b[2], c[2]);
	
	M33 rast;
	rast[0] = V3(x[0], y[0], 1);
	rast[1] = V3(x[1], y[1], 1);
	rast[2] = V3(x[2], y[2], 1);
	rast_inv = rast.Inverse();
}





