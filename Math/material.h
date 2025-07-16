#pragma once
#include <iostream>
#include <cmath>
#include "Vec4.h"

class Material{
      public:
             float ambient;
             float diffuse;
             float specular;
             float shininess;
             float reflective;
             float transparency;
             float refractive_index;
           
      public:
             Material(){
                        ambient = 0.1;
                        diffuse = 0.9;
                        specular = 0.9;
                        shininess = 200.0;
                        reflective = 0.0;
                        transparency = 0.0;
                        refractive_index = 1.0;
             }
             Material(float _ambient, float _diffuse, float _specular, float _shininess, float _reflective){
                            ambient = _ambient;
                            diffuse = _diffuse;
                            specular = _specular;
                            shininess = _shininess;
                            reflective = _reflective;
                            transparency = 0.0;
                            refractive_index = 1.0;
             }
             Material(float _ambient, float _diffuse, float _specular, float _shininess, float _reflective, float _transparency, float _refractive_index){
                            ambient = _ambient;
                            diffuse = _diffuse;
                            specular = _specular;
                            shininess = _shininess;
                            reflective = _reflective;
                            transparency = _transparency;
                            refractive_index = _refractive_index;
             }
             
             void toString(){
                  std::cout<<"("<<ambient<<", "<<diffuse<<", "<<specular<<", "<<shininess<<", "<<reflective<<")"<<std::endl;
             }
             void isglass(){
                  transparency = 1.0;
                  refractive_index = 1.5;
             }
         
             
};

class Pattern{
      public:
             bool pattern_enabled;
             enum pattern_type
             {
                  checkered = 0, striped = 1 //add others
             };
      public:
             Pattern(){
                  pattern_enabled = false;     
             }
             Pattern(bool _pattern_enabled){
                  pattern_enabled = _pattern_enabled;
             }
             Vec4 checkered_pattern(Vec4 coords){ //not working
                   //if(std::fmod( (std::fabs(coords.x)+ std::fabs(coords.y)+ std::fabs(coords.z)), 0.002f) == 0){ //cool!
                   if(int((std::abs(coords.x)+ std::abs(coords.y)+ std::abs(coords.z))*10) % 3 ==0){ //it was not absloute it was GIF lmao
                        return Vec4(1,1,1,1); //white
                   }
                   else{
                        return Vec4(0,0,0,1); //black
                   }
                   
             }
             Vec4 stripe_pattern(Vec4 coords){ // woeking well for these values
                   if((int(std::abs(coords.x)*10) % 2 == 0)){
                        //return Vec4(1,0,0.5,1); 
                        return Vec4(0,0,0,1);                         
                   }
                   else{
                        return Vec4(1,1,1,1);
                   }
             }
             
};
