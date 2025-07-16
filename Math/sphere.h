#pragma once
#include <iostream>
#include "Vec4.h"
#include "object.h"
#include "ray.h"
#include "material.h"
#include <vector>

using namespace std;

class sphere: public hittable{
      public:
             Vec4 centre;
             float radius;
             Vec4 albedo; 
             Material material;
             Pattern pattern;
             
             
      public:
             sphere(){
                      centre = Vec4(0,0,0,Vec4::point);
                      radius = 1;
                      albedo = Vec4(1,1,1,1);
                      material = Material(); //default material
                      pattern = Pattern();
             }
             sphere(const Vec4& _centre, const float& _radius){
                          centre = _centre;
                          radius = _radius;
                          albedo = Vec4(1,1,1,1);
                          material = Material(); //default material
                          pattern = Pattern();
             }
      public:
             void get_values(ray r,std::vector<float>& t_values,std::vector<hittable*> &hitobjects){
                  float A,B,C,D;
                  A = dot(r.dir, r.dir);
                  B = 2* dot(r.origin - centre, r.dir); 
                  C = dot(r.origin - centre, r.origin - centre) - radius * radius;
                                 
                  D = B*B - 4*A*C; 
                  
                  if(D >= 0){
                      float t1 = (-B - sqrt(D))/2*A;
                      float t2 = (-B + sqrt(D))/2*A; 
                      
                      t_values.push_back(t1);
                      hitobjects.push_back(this);
                      t_values.push_back(t2);
                      hitobjects.push_back(this);
                  }
             }
             
             void update_transformation(Vec4 new_pos){
                  centre = centre + new_pos;
             }

             Material get_material(){
                  return material;
             }
             void set_material(Material _material){
                  material = _material;
             }
             Vec4 get_normal(Vec4 hitpoint){
                  return hitpoint - centre;
             }
             Vec4 get_albedo(){
                  return albedo;
             }
             Vec4 get_albedo(Vec4 hitpoint){
                  if(pattern.pattern_enabled==true){
                       return pattern.checkered_pattern(hitpoint);
                  }
                  else{
                       return albedo;
                  }
             }
             void set_albedo(Vec4 _albedo){
                  albedo = _albedo;
             }
             void set_pattern(bool _pattern_enabled){
                  pattern.pattern_enabled = _pattern_enabled;
             }
             
};
