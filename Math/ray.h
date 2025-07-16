#pragma once
#include <iostream>
#include "Vec4.h"


class ray{
      public:
             Vec4 origin;
             Vec4 dir;
             
      public:
             ray(){
                   origin = Vec4(0,0,0,Vec4::point);
                   dir = Vec4(1,1,1,Vec4::vector);
             }
             ray(const Vec4& _origin, const Vec4& _dir){
                       origin = _origin;
                       dir = _dir;
             }
             Vec4 hitpoint(float t){
                   return (origin + dir * t);
             }
};

