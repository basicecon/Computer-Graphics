#pragma once
using namespace std;

#include <ostream>
#include "v3.h"

class M33 {
  V3 rows[3];
public:
  M33();
  M33(int index, float alpha);
  V3& operator[](int i);
  V3 Column(int index);
  void SetColumn(int index, V3 v);
  float GetDeterminant();
  M33 Inverse();
  M33 SafeInverse();
  M33 Transpose();
  friend ostream& operator<<(ostream &os, M33 m);
  friend istream& operator>>(istream& is, M33 m);
  V3 operator*(V3 v);
  M33 operator*(M33 m1);
  void Normalize();
};