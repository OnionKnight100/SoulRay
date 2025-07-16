#pragma once
#include <iostream>
#include "object.h"

class intersection{
      public:
             float t;
             hittable* hitobj;
      public:
             
             intersection(float _t, hittable* _hitobj){
                                t = _t;
                                hitobj = _hitobj;
             }
      public:
             bool operator==(const intersection& other) const{ //operator overloading
                  return t == other.t && hitobj == other.hitobj;
             }
};
