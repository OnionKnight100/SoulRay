#pragma once
#include <iostream>
#include "Vec4.h"
#include "object.h"
#include "ray.h"
#include "material.h"
#include <vector>


class Triangle : public hittable{
      
      public:
             Vec4 p1,p2,p3;
             Vec4 n1,n2,n3;
             Vec4 e1,e2;
             float U,V;
             Vec4 normal;
             Vec4 albedo;
             Material material;
             Pattern pattern;
             
      public:
             Triangle(){
                  p1 = Vec4(-1,0,0,Vec4::point);
                  p2 = Vec4(1,0,0,Vec4::point);
                  p3 = Vec4(0,1,0,Vec4::point);
                  e1 = p2-p1;
                  e2 = p3-p1;
                  normal = cross(e1,e2);
                  normal.normalize();
                  n1=normal;
                  n2=normal;
                  n3=normal;
                  albedo = Vec4(1,1,1,1);
                  material = Material(); //default material
                  pattern = Pattern();
             }
             Triangle(Vec4 _p1,Vec4 _p2,Vec4 _p3){
                  p1 = _p1;
                  p2 = _p2;
                  p3 = _p3;
                  e1 = p2-p1;
                  e2 = p3-p1;
                  normal = cross(e1,e2);
                  normal.normalize();
                  n1=normal;
                  n2=normal;
                  n3=normal;
                  albedo = Vec4(1,1,1,1);
                  material = Material(); //default material
                  pattern = Pattern();
             }
             void get_values(ray r,std::vector<float>& t_values,std::vector<hittable*> &hitobjects){         
                  
                  Vec4 dir_cross_e2 = cross(r.dir, e2);
                  float determinant = dot(e1,dir_cross_e2);
                  if(std::fabs(determinant)<0.0001){
                  }
                  else{
                       float inv_determinant = 1/determinant;
                       Vec4 p1_to_origin = r.origin - p1;
                       float u = inv_determinant * dot(p1_to_origin, dir_cross_e2);
                       U=u;
                       if(u<0 || u>1){
                       }
                       else{
                            Vec4 origin_cross_e1 = cross(p1_to_origin, e1);
                            float v = inv_determinant * dot(r.dir, origin_cross_e1);
                            V=v;
                            if(v<0 || u+v > 1){
                            }
                            else{
                                 float t = inv_determinant * dot(e2,origin_cross_e1);
                                 t_values.push_back(t);
                                 hitobjects.push_back(this);
                            }
                       }
                  }
             }
             void update_transformation(Vec4 new_pos){
                  p1 = p1 + new_pos;
                  p2 = p2 + new_pos;
                  p3 = p3 + new_pos;
             }

             Material get_material(){
                  return material;
             }
             void set_material(Material _material){
                  material = _material;
             }
             Vec4 get_normal(Vec4 hitpoint){
                  //return smooth normal
                  //return normal;
                  return ((n2*U)+(n3*V)+(n1*(1-U-V)));
             }
             Vec4 get_albedo(){
                  return albedo;
             }
             Vec4 get_albedo(Vec4 hitpoint){
                   
                  return albedo;
                  
                  /*if(pattern.pattern_enabled==true){
                       return pattern.checkered_pattern(hitpoint);
                  }
                  else{
                       return albedo;
                  }*/
             }
             void set_albedo(Vec4 _albedo){
                  albedo = _albedo;
             }
             void set_pattern(bool _pattern_enabled){
                  pattern.pattern_enabled = _pattern_enabled;
             }
             void set_vertex_normals(Vec4 _n1,Vec4 _n2,Vec4 _n3){
                  n1 = _n1;
                  n2 = _n2;
                  n3 = _n3;
             }
             void get_vertices(std::vector<Vec4> &vertexlist){
                   vertexlist[0] = p1;
                   vertexlist[1] = p2;
                   vertexlist[2] = p3;
                  // p1.toString();
                   //p2.toString();
                   //p3.toString();
             }
};
