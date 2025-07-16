#pragma once
#include <iostream>
#include "Vec4.h"
#include "object.h"
#include "ray.h"
#include "material.h"
#include <math.h>
#include <vector>

class cube : public hittable{
      public:
             Vec4 albedo;
             Material material;
             Pattern pattern;
             Vec4 Bmax,Bmin;
             
             
             cube(Vec4 max, Vec4 min){
                 Bmax = max;
                 Bmin = min;
                 albedo = Vec4(0,0,0,1);
                 material = Material(); //default material
                 pattern = Pattern();   
             }
      public:
             void get_values(ray r,std::vector<float>& t_values,std::vector<hittable*> &hitobjects){
                  
                 float tmin = (Bmin.x - r.origin.x)/r.dir.x;
                 float tmax = (Bmax.x - r.origin.x)/r.dir.x;
                 
                 if(tmin>tmax){
                      float temp = tmin;
                      tmin = tmax;
                      tmax = temp;
                 }
                 
                 float tymin = (Bmin.y - r.origin.y)/r.dir.y;
                 float tymax = (Bmax.y - r.origin.y)/r.dir.y;
                 
                 if(tymin>tymax){
                      float temp = tymin;
                      tymin = tymax;
                      tymax = temp;
                 }
                 
                 if((tmin>tymax) or (tymin>tmax)){
                 }
                 else{
                     if(tymin>tmin) tmin = tymin; 
                     if(tymax<tmax) tmax = tymax;
                     
                     float tzmin = (Bmin.z - r.origin.z)/r.dir.z;
                     float tzmax = (Bmax.z - r.origin.z)/r.dir.z;
                     
                     if(tzmin>tzmax){
                        float temp = tzmin;
                        tzmin = tzmax;
                        tzmax = temp;
                     }
                     if((tmin>tzmax) or (tzmin>tmax)){
                     }
                     else{
                          if(tzmin>tmin) tmin = tzmin; 
                          if(tzmax<tmax) tmax = tzmax;
                          
                          t_values.push_back(tmin);
                          hitobjects.push_back(this);
                          t_values.push_back(tmax);
                          hitobjects.push_back(this);
                     }
                 }
             }
             
             void update_transformation(Vec4 new_pos){
                  //centre = centre + new_pos;
             }

             Material get_material(){
                  return material;
             }
             void set_material(Material _material){
                  material = _material;
             }
             Vec4 get_normal(Vec4 hitpoint){
                   return Vec4(0,1,0,0);
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

