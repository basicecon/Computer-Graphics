#include "tmesh.h"

#include <fstream>
#include <iostream>



TMesh::TMesh(V3 center, V3 dims, unsigned int color) {

  enabled = true;
  vertsN = 8;
  verts = new V3[vertsN];
  cols = new V3[vertsN];
  normals = 0;

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

	/*
  for (int tri = 0; tri < trisN; tri++) {
    int vinds[3];
    vinds[0] = tris[tri*3+0];
    vinds[1] = tris[tri*3+1];
    vinds[2] = tris[tri*3+2];
    for (int vi = 0; vi < 3; vi++) {
      fb->Draw3DSegment(verts[vinds[vi]], verts[vinds[(vi+1)%3]], ppc,
        color);
    }
  } */
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
			//cerr << verts[vinds[vi]] << " and: " << verts[vinds[(vi + 1) % 3]] << endl;
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

void TMesh::RenderFilled(PPC *ppc, FrameBuffer *fb,
	unsigned int color, V3 L, float ka, float es, int renderMode) {

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

		// Clip aabb with frame
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

				// pixel is inside triangle and triangle is visible at pixel
				// compute color of pixel based on current triangle

				if (renderMode == 1){
					unsigned int c = SSIVColor(pv, redABC, greenABC, blueABC).GetColor();
					// cerr << "u = " << u << " v = " << v << " c=" << c << endl;
					fb->Set(u, v, c);
				}
				else if (renderMode == 2){
					float kd[3];
					float ks[3];
					V3 n0, n1, n2;
					V3 P = ppc->UnProject(V3(u + 0.5, v + 0.5, currz));
					V3 lv = L - P; 
					lv.Normalize();
					kd[0] = lv * normals[vinds[0]];
					kd[1] = lv * normals[vinds[1]];
					kd[2] = lv * normals[vinds[2]];
					kd[0] = (kd[0] < 0) ? 0 : kd[0];
					kd[1] = (kd[1] < 0) ? 0 : kd[1];
					kd[2] = (kd[2] < 0) ? 0 : kd[2];

					ks[0] = KS(lv, ppc->C, normals[vinds[0]], es);
					ks[1] = KS(lv, ppc->C, normals[vinds[1]], es);
					ks[2] = KS(lv, ppc->C, normals[vinds[2]], es);

					//Setup Screen Space linear variation lredABC
					r[0] = colors[0][0] * (ka + (1 - ka) * kd[0] + ks[0]);
					r[1] = colors[1][0] * (ka + (1 - ka) * kd[1] + ks[1]);
					r[2] = colors[2][0] * (ka + (1 - ka) * kd[2] + ks[2]);
					V3 lrRed = V3(r[0], r[1], r[2]);
					V3 lredABC = rast_inv*lrRed;

					//Setup Screen Space linear variation lgreenABC
					r[0] = colors[0][1] * (ka + (1 - ka) * kd[0] + ks[0]);
					r[1] = colors[1][1] * (ka + (1 - ka) * kd[1] + ks[1]);
					r[2] = colors[2][1] * (ka + (1 - ka) * kd[2] + ks[2]);
					V3 lrGreen = V3(r[0], r[1], r[2]);
					V3 lgreenABC = rast_inv*lrGreen;

					//Setup Screen Space linear variation lblueABC
					r[0] = colors[0][2] * (ka + (1 - ka) * kd[0] + ks[0]);
					r[1] = colors[1][2] * (ka + (1 - ka) * kd[1] + ks[1]);
					r[2] = colors[2][2] * (ka + (1 - ka) * kd[2] + ks[2]);
					V3 lrBlue = V3(r[0], r[1], r[2]);
					V3 lblueABC = rast_inv*lrBlue;

					V3 newColor = SSIVColor(pv, lredABC, lgreenABC, lblueABC);
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
					float kd = lv * n;
					kd = (kd < 0) ? 0 : kd;
					float ks = KS(lv, ppc->C, n, es);
					//	FB[p] = color * (ka + (1-ka)*kd);
					V3 origColor = SSIVColor(pv, redABC, greenABC, blueABC);
					V3 newColor = origColor * (ka + (1 - ka)*kd + ks);
					fb->Set(u, v, newColor.GetColor());
				}
			}
		}
	}
	delete[]pverts;
}

// SM1
V3 TMesh::SSIVColor(V3 pv, V3 redABC, V3 greenABC, V3 blueABC){
	// if rendering mode is vertext color interpolation = SM1
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

// SM3
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
// Compute ks
float TMesh::KS(V3 L, V3 C, V3 n, float e){
	float ks;
	float lr = L*(C - (n * 2)*(n*C));
	ks = pow(lr, e);
	return ks;
}

