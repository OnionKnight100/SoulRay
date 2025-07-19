#include "Vec4.h"

void Vec4::normalize(){
     float mag = sqrt(x*x +y*y +z*z +w*w);
      x = x / mag; 
      y = y / mag;
      z = z / mag;
}

float Vec4::magnitude(){
      return sqrt(x*x +y*y +z*z +w*w);
}

void Vec4::toString(){
     std::cout<<"("<<x<<","<<y<<","<<z<<","<<w<<")"<<std::endl;
}

float dot(Vec4 a,Vec4 b){
      return a.x * b.x + a.y * b.y + a.z * b.z;
}         
Vec4 cross(Vec4 a,Vec4 b){
      return Vec4((a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x),0);
}
Vec4 hadamard_prod(Vec4 a, Vec4 b){
      return Vec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w);
}
Vec4 convert_to_255(const Vec4& color){
      return Vec4(float(int(color.x*255)),float(int(color.y*255)),float(int(color.z*255)),1);
}
Vec4 convert_to_01(const Vec4& color){
      return Vec4(color.x/255,color.y/255,color.z/255,1);
}
Vec4 clamp_color(Vec4 color){      
      return Vec4(
      std::clamp(color.x, 0.0f, 1.0f),
      std::clamp(color.y, 0.0f, 1.0f),
      std::clamp(color.z, 0.0f, 1.0f),
      1.0f
      );
}
