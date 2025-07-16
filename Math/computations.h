#pragma once
#include <iostream>
#include "sphere.h"
#include "object.h"
#include "Vec4.h"

struct Computations{
      public:
             float t;
             sphere obj;
             hittable* hitobj;
             Vec4 hitpoint;
             Vec4 overhitpoint;
             Vec4 underhitpoint;
             Vec4 eye_vec;
             Vec4 normal_vec;
             Vec4 reflect_vec;
             bool inside;
             float n1; //refractive indices
             float n2;
             
             
};
