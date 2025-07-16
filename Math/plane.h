#pragma once
#include <iostream>
#include "Vec4.h"
#include "object.h"
#include "ray.h"
#include "material.h"
#include <vector>
#include <cmath>

using namespace std;

class Plane: public hittable{
      public:
             Vec4 centre;
             Vec4 normal;
             Vec4 albedo;
             Material material;
             Pattern pattern;
      public:
             Plane(Vec4 _centre, Vec4 _normal){
                     centre = _centre;
                     normal = _normal;
                     material = Material(); //default material
                     pattern = Pattern();
             }
             Plane(Vec4 _centre, Vec4 _normal, Vec4 _albedo){
                     centre = _centre;
                     normal = _normal;
                     albedo = _albedo;
                     material = Material(); //default material
                     pattern = Pattern();
             }
             Plane(){
                     centre = Vec4(0,-1,0,Vec4::point); //default
                     normal = Vec4(0,1,0,Vec4::vector); //default
                     albedo = Vec4(1,0,0,Vec4::point);
                     material = Material(); //default material
                     pattern = Pattern();
             }
      public:
             void get_values(ray r,std::vector<float>& t_values,std::vector<hittable*> &hitobjects){
                  //std::cout<<"hehe boii!! im plane"<<std::endl;
                    if(dot(r.dir,normal) == 0){
                      //no intersection
                    }
                    else{
                         float t1 = dot((centre - r.origin),normal)/dot(r.dir,normal);
                         t_values.push_back(t1);
                         hitobjects.push_back(this);
                         //std::cout<<"planen htei";
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
                  return normal;
             }
             Vec4 get_albedo(){
                  
                  return albedo;
             }
             Vec4 get_albedo(Vec4 hitpoint){
                  Vec4 e1 = cross(normal,Vec4(1,0,0,0));
                  if(e1.magnitude() == 0){
                     e1 = cross(normal,Vec4(0,0,1,0));
                  }
                  e1.normalize();
                  Vec4 e2 = cross(normal,e1);
                  e2.normalize();
                  
                  float u = dot(e1,hitpoint);
                  float v = dot(e2,hitpoint);
                  //std::cout<<"u: "<<u<<",v: "<<v<<std::endl;
                  int iu = floor(u);
                  int iv = floor(v);
                  if((iu+iv)%2 == 0){
                           return albedo*0.4;
                             
                  }
                  return albedo;
                  /*
                  if(pattern.pattern_enabled==true){
                       _albedo = pattern.stripe_pattern(hitpoint);
                  }
                  else{
                       _albedo = albedo;
                  }*/
             }
             void set_albedo(Vec4 _albedo){
                  albedo = _albedo;
             }
             void set_pattern(bool _pattern_enabled){
                  pattern.pattern_enabled = _pattern_enabled;
             }
};
