#include "../Math/Vec4.h"
#include "../Math/ray.h"
#include "../Math/sphere.h"
#include "../Math/object.h"
#include "../Math/intersection.h"
#include "../Math/computations.h"
#include "../Math/material.h"
#include "light.h"
#include <vector>
#include <algorithm>
#include <iostream>

Vec4 reflect(Vec4 incidence, Vec4 normal);

Vec4 lighting(Computations comps, pointLight light, bool is_shadowed){ 
      // add albedo in material maybe    
      Material material;
      material = comps.hitobj->get_material();
      Vec4 albedo;
      //comps.hitobj->get_albedo(albedo,comps.hitpoint);
      albedo = comps.hitobj->get_albedo(comps.hitpoint);
      
      Vec4 effective_color = hadamard_prod(albedo, light.color);
      
      Vec4 light_vec = light.position - comps.hitpoint;
      light_vec.normalize();
      
      Vec4 ambient = effective_color * material.ambient;
      Vec4 diffuse;
      Vec4 specular;
      
      //comps.normal_vec.toString();
      float light_dot_normal = dot(light_vec, comps.normal_vec);
      if(light_dot_normal < 0){
            diffuse = Vec4(0,0,0,1); //color black
            specular = Vec4(0,0,0,1);                        
      }
      else{
           diffuse = effective_color * (material.diffuse * light_dot_normal);
            
           Vec4 reflect_vec = reflect(-light_vec, comps.normal_vec);
           float reflect_dot_eye = dot(reflect_vec, comps.eye_vec);
           
           if(reflect_dot_eye < 0){
                 specular = Vec4(0,0,0,1); //color black
           }
           else{
                float shininess_factor = std::pow(reflect_dot_eye , material.shininess);
                specular = effective_color * (material.specular * shininess_factor);
           }
      }
      
      
      if(is_shadowed == true){
           return ambient;
      }
      else{
           return ambient + diffuse + specular;         
      }
      
}

Vec4 reflect(Vec4 incidence, Vec4 normal){
      return incidence - (normal * (2 * dot(incidence, normal)));
}
