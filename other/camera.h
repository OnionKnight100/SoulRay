#pragma once
#include <iostream>
#include "../Math/Vec4.h"
#include "../Math/ray.h"
#include "../Math/sphere.h"
#include "../Math/object.h"
#include "../Math/intersection.h"
#include "../Math/computations.h"
#include "../Math/material.h"
#include "light.h"
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

Vec4 lighting(Computations comps, pointLight light, bool is_shadowed);
Vec4 reflect(Vec4 incidence, Vec4 normal);
Vec4 getBackgroundColor(ray r, Vec4* background_image, int width, int height);



class camera{
     public:
            string name;
            Vec4 position;
            Vec4 lookat_point;
            float aspect_ratio;
            Vec4* background_image;
            int background_img_height;
            int background_img_width;
            
     private:
             int img_height;
             int img_width;
             float focal_length;
             Vec4 delta_u;
             Vec4 delta_v;
             Vec4 vp_upperleft;
             float vp_height;
             float vp_width;
            
     public:
            camera(){
                     //default values
                     aspect_ratio = 16/9;
                     img_width = 200;
                     std::cout<<"default img width: "<<img_width<<std::endl;                  
                     position = Vec4(0,0,8,Vec4::point);
                     lookat_point = Vec4(0,0,0,Vec4::point);
                     focal_length = 10;
                     vp_height = 7;
            }
            camera(string _name){
                     //default values
                     name = _name;
                     aspect_ratio = 16/9;
                     img_width = 700; 
                     std::cout<<"default img width: "<<img_width<<std::endl;                  
                     position = Vec4(0,0,8,Vec4::point);
                     lookat_point = Vec4(0,0,0,Vec4::point);
                     focal_length = 10;
                     vp_height = 7;
            }
            
            void render(std::vector<hittable*> scene, const char* filePath);
            void initialize();
            void set_properties(float _aspect_ratio, int _img_width, float _focal_length, float _vp_height);  
            void set_position(Vec4 _position, Vec4 _lookat_point);       
            
     private:
            Vec4 returnColor(std::vector<intersection> hits ,ray r,std::vector<hittable*> scene, int remaining);
            Vec4 reflected_color(std::vector<hittable*> scene, Computations comps, int remaining);
            Vec4 shadepixel(Computations comps, ray r, std::vector<hittable*> scene, pointLight light, int remaining);   
            Vec4 gammaCorrection(Vec4 color);      
            void get_hits(ray r, hittable* s , std::vector<intersection>& hits);
            static bool compare_intersections(const intersection& a, const intersection& b);
            int get_hit_index(std::vector<intersection> hits);
            Computations prepare_computations(intersection hit, ray r, std::vector<intersection> hits);
            void prepare_refractive_indices(Computations& comps,intersection hit ,std::vector<intersection> hits);
            Vec4 refracted_color(std::vector<hittable*> scene, Computations comps, int remaining);
            float schlick(Computations comps);
            
             
};
