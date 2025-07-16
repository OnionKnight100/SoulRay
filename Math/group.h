#pragma once
#include <iostream>
#include "Vec4.h"
#include "object.h"
#include "ray.h"
#include "material.h"
#include "cube.h"
#include <vector>

using namespace std;

struct Bounding_box{
       public:
          Vec4 Bmin;
          Vec4 Bmax;
          
          Bounding_box(){
             Bmin = Vec4(9999999,9999999,9999999,1);
             Bmax = Vec4(-9999999,-9999999,-9999999,1);
          }
          
          bool contains_triangle(hittable* triangle){
               std::vector<Vec4> vertices;
               vertices.reserve(3);
               triangle->get_vertices(vertices);
               //vertices[0].toString();
               //vertices[1].toString();
               //vertices[2].toString();
               for(int i=0;i<3;i++){
                   if(vertices[i].x<Bmin.x || vertices[i].y<Bmin.y || vertices[i].z<Bmin.z) return false;
                   if(vertices[i].x>Bmax.x || vertices[i].y>Bmax.y || vertices[i].z>Bmax.z) return false;
               }
               return true;
          }
          
          bool check_intersection(ray r){
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
                      return false;
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
                        return false;
                      }
                      else{
                        if(tzmin>tmin) tmin = tzmin; 
                        if(tzmax<tmax) tmax = tzmax;
                                          
                        return true;
                      }
                    }
                }
              
};


class group: public hittable{
      public:
             //will contain other children groups as well, since groups are hittable inherited 
             std::vector<hittable*> children; 
             Bounding_box b_box;
             Vec4 max_bound;
             Vec4 min_bound;
             Vec4 centre;
             Vec4 albedo; 
             Material material;
             Pattern pattern;
             
             
      public:
             group(std::vector<hittable*> _children){
                      children.reserve(_children.size()+1);
                      for(int i=0;i<_children.size();i++){
                              children.push_back(_children[i]);
                      }
                      centre = Vec4(0,0,0,Vec4::point);
                      albedo = Vec4(1,1,1,1);
                      material = Material(); //default material
                      pattern = Pattern();
             }
      public:
             void get_values(ray r,std::vector<float>& t_values, std::vector<hittable*> &hitobjects){
                  //check for bounding boxes here
                  if(b_box.check_intersection(r)){
                      for(int i=0;i<children.size();i++){
                          children[i]->get_values(r, t_values,hitobjects);
                      } 
                  }   
             
             }  
             void update_transformation(Vec4 new_pos){
                  for(int i=0;i<children.size();i++){
                      children[i]->update_transformation(new_pos);
                  }
                  b_box.Bmin = b_box.Bmin + new_pos;
                  b_box.Bmax = b_box.Bmax + new_pos;
             }          
             Material get_material(){
                  return material;
             }
             void set_material(Material _material){
                  for(int i=0;i<children.size();i++){
                      children[i]->set_material(_material);
                  } 
             }
             Vec4 get_normal(Vec4 hitpoint){
                   std::cout<<"error: get_normal being called on group! \n";
                   return Vec4(0,0,0,Vec4::vector);
                   
                  //empty, no need here since it never gets called
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
                  for(int i=0;i<children.size();i++){
                      children[i]->set_albedo(_albedo);
                  }
             }
             void set_pattern(bool _pattern_enabled){
                  pattern.pattern_enabled = _pattern_enabled;
             }
             void set_bounds(Vec4 _min_bound,Vec4 _max_bound){
                  b_box.Bmin = _min_bound;
                  b_box.Bmax = _max_bound;
                  // b_box.Bmin.toString();
                  // b_box.Bmax.toString();
             }
             void divide_model(int depth){
                  if(depth >0){
                           Bounding_box b_boxleft,b_boxright;
                           std::vector<hittable*> leftchildren,rightchildren;
                           std::vector<int> removedchildren; //will store indices of children to be removed from the current children list
                           
                           float xside = b_box.Bmax.x - b_box.Bmin.x;
                           float yside = b_box.Bmax.y - b_box.Bmin.y;
                           float zside = b_box.Bmax.z - b_box.Bmin.z;
                           float longestside = fmaxf(xside,fmaxf(yside,zside));
                           
                           //cout<<xside<<" "<<yside<<" "<<zside<<" "<<longestside<<" \n";
                           if(xside == longestside){
                                b_boxleft.Bmax = b_box.Bmax;
                                b_boxleft.Bmax.x = b_boxleft.Bmax.x - longestside/2;
                                b_boxleft.Bmin = b_box.Bmin;
                                b_boxright.Bmin = b_box.Bmin;
                                b_boxright.Bmin.x = b_boxright.Bmin.x + longestside/2;
                                b_boxright.Bmax = b_box.Bmax;
                           }
                           else if(yside == longestside){
                                b_boxleft.Bmax = b_box.Bmax;
                                b_boxleft.Bmax.y = b_boxleft.Bmax.y - longestside/2;
                                b_boxleft.Bmin = b_box.Bmin;
                                b_boxright.Bmin = b_box.Bmin;
                                b_boxright.Bmin.y = b_boxright.Bmin.y + longestside/2;
                                b_boxright.Bmax = b_box.Bmax;
                           }
                           else{
                                b_boxleft.Bmax = b_box.Bmax;
                                b_boxleft.Bmax.z = b_boxleft.Bmax.z - longestside/2;
                                b_boxleft.Bmin = b_box.Bmin;
                                b_boxright.Bmin = b_box.Bmin;
                                b_boxright.Bmin.z = b_boxright.Bmin.z + longestside/2;
                                b_boxright.Bmax = b_box.Bmax;
                           }
                           
                           for(int i=0;i<children.size();i++){
                               if(b_boxleft.contains_triangle(children[i])){
                                     //cout<<"itr"<<i<<" left contains triangle \n";
                                     leftchildren.push_back(children[i]);
                                     removedchildren.push_back(i);
                                     
                               }
                               else if(b_boxright.contains_triangle(children[i])){
                                     //cout<<"itr"<<i<<" right contains triangle \n";
                                     rightchildren.push_back(children[i]);
                                     removedchildren.push_back(i);
                               }
                               else{
                                   //cout<<"itr"<<i<<" none contains triangle, (current) \n";
                               }       
                           }                         
                           //std::cout<<"finished sorting children \n";     
                         // this will loop backwards so the sorting in the main children array doesnt't get disturbed  
                           for(int i=removedchildren.size(); i>0; i--){
                               children.erase(children.begin() + removedchildren[i-1]);
                           }
                          // std::cout<<"finished removing children \n";
                           
                           hittable* leftgroup = new group(leftchildren);
                           leftgroup->set_bounds(b_boxleft.Bmin,b_boxleft.Bmax);
                           hittable* rightgroup = new group(rightchildren);
                           rightgroup->set_bounds(b_boxright.Bmin,b_boxright.Bmax);
                           
                           children.push_back(leftgroup);
                           children.push_back(rightgroup);
                           
                           leftgroup->divide_model(depth-1);
                           rightgroup->divide_model(depth-1);
                          
                  }
                                             
                     /*
                     make two bounding boxes
                            bbox left and bbox right
                     make two vectors of hittablepointers
                          vector left,right
                     for every children in this group
                         if bboxleft.contains(child)
                            vector left.add(child)
                            remove child from current group children 
                         else if bboxright.contains(child)
                            vector right.add()child
                            remove child from current group children
                         else do nothing (child stays in current group)
                     loop end
                     make two groups
                     group left with children vector left with bbox left
                     group right with children vector right with bbox right
                     add group left and right as this group's child
                     call divide model on group left with depth-1
                     call divide model on group right with depth-1
                     */
             }
};


