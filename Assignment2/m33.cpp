#include "m33.h"
#include <iostream>
#define PI           3.14159265358979323846  /* pi */

V3& M33::operator[](int i) {
  return rows[i];
}

// contructor: identity matrix
M33::M33() {
	rows[0] = V3(1, 0, 0);
	rows[1] = V3(0, 1, 0);
	rows[2] = V3(0, 0, 1);
}
// Constructor: rotation about {x|y|z} axis alpha degrees
M33::M33(int index, float alpha) {
	float rad = alpha*PI/180;
	if (index == 0) {
		rows[0] = V3(1, 0, 0);
		rows[1] = V3(0, cos(rad), -sin(rad));
		rows[2] = V3(0, sin(rad), cos(rad));
	} else if (index == 1) {
		rows[0] = V3(cos(rad), 0, sin(rad));
		rows[1] = V3(0, 1, 0);
		rows[2] = V3(-sin(rad), 0, cos(rad));
	} else {
		rows[0] = V3(cos(rad), -sin(rad), 0);
		rows[1] = V3(sin(rad), cos(rad), 0);
		rows[2] = V3(0, 0, 1);
	} 
}

ostream& operator<<(ostream &os, M33 m) {
  os << m[0] << endl;
  os << m[1] << endl;
  os << m[2] << endl;
  return os;
}

V3 M33::operator*(V3 v) {
  V3 ret(rows[0]*v, rows[1]*v, rows[2]*v);
  return ret;
}

// read and write access to rows and columns
V3 M33::Column(int index) {
	V3 ret = V3(rows[0][index], rows[1][index], rows[2][index]);
	return ret;
}
// set col?
void M33::SetColumn(int index, V3 v) {
	for (int i = 0; i < 3; i ++) {
		for (int j = 0; j < 3; j ++) {
			if (j == index) {
				rows[i][j] = v[i];
			}
		}
	}
}
float M33::GetDeterminant() {
	return this->Column(0)*(this->Column(1)&this->Column(2));
}
// matrix inversion
M33 M33::Inverse() {
	M33 ret;
	float det = this->GetDeterminant();
	ret[0] = (this->Column(1)&this->Column(2)) / det;
	ret[1] = (this->Column(2)&this->Column(0)) / det;
	ret[2] = (this->Column(0)&this->Column(1)) / det;
	return ret;
}

M33 M33::SafeInverse() {
	if (this->GetDeterminant() == 0.0f) {
		cerr << "ERROR" << endl; 
		exit(0);
	} else {
		return this->Inverse();
	}
}
M33 M33::Transpose() {
	M33 ret;
	ret[0] = this->Column(0);
	ret[1] = this->Column(1);
	ret[2] = this->Column(2);
	return ret;
}


// matrix multiplication
M33 M33::operator*(M33 m1) {
	M33 ret;
	V3 column1 = (*this)*m1.Column(0);
	ret.SetColumn(0, column1);
	V3 column2 = (*this)*m1.Column(1);
	ret.SetColumn(1, column2);
	V3 column3 = (*this)*m1.Column(2);
	ret.SetColumn(0, column3);
	return ret;
}

void M33::Normalize() {
  V3 vs = rows[0] + rows[1] + rows[2];
  float sum = vs[0] + vs[1] + vs[2];

  rows[0] = rows[0] / sum;
  rows[1] = rows[1] / sum;
  rows[2] = rows[2] / sum;
}