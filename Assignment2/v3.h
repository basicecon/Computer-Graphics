#pragma once
#include <ostream>
using namespace std;

class V3 {

  float xyz[3];
public:
  
  V3() {};
  V3(float x, float y, float z);
  friend ostream& operator<<(ostream& os, V3 v);
  float& operator[](int i);
  void Normalize();
  V3 operator+(V3 v1);
  V3 operator-(V3 v1);
  float operator*(V3 v1);
  V3 operator&(V3 v1);
  V3 operator/(float scf);
  V3 operator*(float scf);
  V3 TransformCoordinate(V3 newOrigin, V3 x, V3 y, V3 z);
  V3 RotatePoint(V3 v1, V3 v2, float scf);
  V3 RotateVector(V3 n, V3 m, float scf);
  float Length();
  void SetFromColor(unsigned int color);
  unsigned int GetColor();
};