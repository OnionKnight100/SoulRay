#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>

class Vec4{
      public:
             float x,y,z,w;
             enum type
             {
                  vector = 0, point = 1
             };
             
             
      public:
             Vec4(){
                    x = 0; 
                    y = 0;
                    z = 0;
                    w = 1;
             }
             Vec4(float _x,float _y,float _z,float _w =1){ 
                    x = _x; 
                    y = _y;
                    z = _z;
                    w = _w;
             }
             Vec4 operator+(const Vec4& b) const{
                   return Vec4(x + b.x,y + b.y,z + b.z,w + b.w);
             }
             Vec4 operator-(const Vec4& b) const{
                   return Vec4(x - b.x,y - b.y,z - b.z,w - b.w);
             }
             Vec4 operator-() const{    //negate
                   return Vec4(-x,-y,-z,w);
             }
             Vec4 operator*(const float& b) const{
                   return Vec4(x*b,y*b,z*b,w);
             }
             Vec4 operator/(const float& b) const{
                   return Vec4(x/b,y/b,z/b,w);
             }
             
             
      public:
             float magnitude();
             void normalize();
             void toString();
            // void to_255();
           //  void to_01();
             
             
};

float dot(Vec4 a,Vec4 b);
Vec4 cross(Vec4 a,Vec4 b);
Vec4 hadamard_prod(Vec4 a, Vec4 b);
Vec4 convert_to_255(const Vec4& color);
Vec4 convert_to_01(const Vec4& color);
Vec4 clamp_color(Vec4 color);
