#include "camera.h"

void camera::initialize(){
     
     // something is wrong with aspect ratio code, ill fix it later
     img_height = img_width;//int(img_width/aspect_ratio);  
     vp_width = vp_height * (float(img_width)/img_height);
     
     //std::cout<<"viewport dimensions: "<<vp_width<<","<<vp_height<<std::endl;
     
     Vec4 global_up_vec(0,1,0,0);
     Vec4 forward_vec = lookat_point - position;
     forward_vec.normalize();
     Vec4 right_vec = cross(forward_vec, global_up_vec);
     if(right_vec.magnitude() <= 0.0001){
         right_vec = cross(forward_vec, Vec4(1,0,0,0));
     }
     Vec4 up_vec = cross(right_vec, forward_vec);
     right_vec.normalize();
     up_vec.normalize();
     
     Vec4 vp_u = right_vec * vp_width;
     Vec4 vp_v = -(up_vec * vp_height);
     delta_u = vp_u/float(img_width);
     delta_v = vp_v/float(img_height);
     vp_upperleft = position - (vp_u/2) - (vp_v/2) + (forward_vec*focal_length);
     //std::cout<<"upper left pixel coords: "<<vp_upperleft.x<<" "<<vp_upperleft.y<<" "<<vp_upperleft.z<<std::endl;

}

//code starts here called by the main function
void camera::render(std::vector<hittable*> scene, const char* filePath){
     //calls 
     initialize();

     ofstream Imagefile(filePath, ios::trunc);
     if(!Imagefile){
         cerr<<"Error in making file!"<<endl;
     }
     
     std::cout<<"started render from "<<name<<"..."<<std::endl;
     int total_itr = img_height*img_width;
     int current = 0;
     
     Imagefile<<"P3"<<endl<<img_width<<" "<<img_height<<endl<<"255"<<endl;
     for(int i=0; i<img_height; i++){
             for(int j=0; j<img_width; j++){
                                               
                    Vec4 current_pixel = vp_upperleft + (delta_u * j) + (delta_v * i);                     
                    Vec4 dir = current_pixel-position;                  
                    dir.normalize();
                     
                    ray r(position, dir);
                      
                    std::vector<intersection> hits;
                    for(int k=0; k<scene.size(); k++){//scene loop 
                         get_hits(r, scene[k], hits);
                    }
                     
                    Vec4 pixel_color = returnColor(hits, r, scene, 5);

                    pixel_color = gammaCorrection(pixel_color); // gamma correction
                    pixel_color = clamp_color(pixel_color);     // clamps the color to 0-1
                    pixel_color = convert_to_255(pixel_color); // converts to 0-255 values
                     
                    Imagefile<<pixel_color.x<<" "<<pixel_color.y<<" "<<pixel_color.z<<endl;
                    current++;       
             }
             std::cout<<current<<" pixels rendered out of "<<total_itr<<std::endl;
     }
     std::cout<<"completed render from "<<name<<"..."<<std::endl;
     //delete[] background_image;
      
}
Vec4 camera::gammaCorrection(Vec4 color){
     float gamma = 2.2f;
     float invGamma = 1.0f / gamma;

     color.x = pow(color.x, invGamma);
     color.y = pow(color.y, invGamma);
     color.z = pow(color.z, invGamma);
     color.w = 1.0f; 
     return color;
}


Vec4 camera::returnColor(std::vector<intersection> hits ,ray r,std::vector<hittable*> scene ,int remaining){
      
      Vec4 skyblue = convert_to_01(Vec4(135,206,235,1)); // skyblue color     
      Vec4 background_color = skyblue;  
       
      if(hits.size() == 0){
            //NOTE!! if changing to backgroundimage, also chage the last else block for it to work
          // return getBackgroundColor(r, background_image, background_img_width, background_img_height);
          return background_color;           
      }
      else{
           std::sort(hits.begin(), hits.end(), compare_intersections);
           int hit_index = get_hit_index(hits);
           
           if(hit_index >= 0){    
                                          
                pointLight light(Vec4(40,100,100,Vec4::point), Vec4(1,1,1,1));
                Computations comps = prepare_computations(hits[hit_index], r, hits);
                
                return shadepixel(comps, r, scene, light, remaining);
           }
           else{
               // return getBackgroundColor(r, background_image, background_img_width, background_img_height);
               return background_color; 
           }
      }
      
}

Computations camera::prepare_computations(intersection hit, ray r, std::vector<intersection> hits){
       Computations comps;  
       comps.t = hit.t;
       comps.hitobj = hit.hitobj;
       comps.hitpoint = r.hitpoint(hit.t);
       comps.eye_vec = -r.dir;
       comps.normal_vec = comps.hitobj->get_normal(r.hitpoint(hit.t));
      // comps.normal_vec.toString();
       comps.normal_vec.normalize();
       if(dot(comps.eye_vec, comps.normal_vec) < 0){
            comps.inside = true;
            comps.normal_vec = -comps.normal_vec;
       }
       else{
            comps.inside = false;
       }
       comps.reflect_vec = reflect(r.dir, comps.normal_vec);
       comps.overhitpoint = comps.hitpoint + comps.normal_vec*(0.0001); 
       prepare_refractive_indices(comps, hit, hits);
       //std::cout<<comps.n1<<" "<<comps.n2<<std::endl;
       comps.underhitpoint = comps.hitpoint - comps.normal_vec*(0.0001);
            
       return comps;    
}

void camera::prepare_refractive_indices(Computations& comps,intersection hit ,std::vector<intersection> hits){
     //cout<<"preparing indices..."<<endl;
     std::vector<hittable*> container;
     for(int i=0; i<hits.size(); i++){
             if(hits[i] == hit){
                 if(container.empty()){
                      comps.n1 = 1.0;       
                 }
                 else{
                      Material material;                      
                      material = container.back()->get_material();
                      comps.n1 = material.refractive_index;
                 }
             }
             bool i_removed = false;
             for(int j=0; j < container.size(); j++){ 
                     if(container[j] == hits[i].hitobj){
                          container.erase(container.begin() + j);
                          i_removed = true;         
                     }
                     
             }
             if(i_removed == false){
                  container.push_back(hits[i].hitobj);
             }
             
             if(hits[i] == hit){
                 if(container.empty()){
                      comps.n2 = 1.0;
                 }
                 else{
                      Material material;                      
                      material = container.back()->get_material();
                      comps.n2 = material.refractive_index;
                 }
                 break;
             }
             
     }
}

Vec4 camera::shadepixel(Computations comps, ray r, std::vector<hittable*> scene, pointLight light, int remaining){
      //shadow calculations
      Vec4 shadow_ray_dir = light.position - comps.overhitpoint;
      float distance = shadow_ray_dir.magnitude();
      shadow_ray_dir.normalize();
      Vec4 shadow_ray_origin = comps.overhitpoint;
      bool is_shadowed = false;
      
      ray shadow_ray(shadow_ray_origin, shadow_ray_dir);
      std::vector<intersection> hits;
      for(int k=0; k<scene.size(); k++){
              get_hits(shadow_ray, scene[k], hits);
      }
      std::sort(hits.begin(), hits.end(), compare_intersections);
      int hit_index = get_hit_index(hits);
      if(hit_index > 0){
              if(hits[hit_index].t < distance){
                   is_shadowed = true;                
              }     
      }
      
      Vec4 surface_color = lighting(comps, light, is_shadowed);
      Vec4 reflected = reflected_color(scene, comps, remaining); 
      Vec4 refracted = refracted_color(scene, comps, remaining);
      Material material;
      material = comps.hitobj->get_material();
      
      if(material.reflective >0 && material.transparency > 0){
           float reflectance = schlick(comps);    
           //std::cout<<reflectance<<std::endl;
           return surface_color + (reflected * reflectance) + refracted * (1- reflectance);
      }
      return surface_color + reflected + refracted;
}

float camera::schlick(Computations comps){
      float cos = dot(comps.eye_vec, comps.normal_vec);
      cos = fabs(cos);
      
      if(comps.n1> comps.n2){
           //std::cout<<"safsadf"<<std::endl;
           float n = comps.n1/comps.n2;
           float sin2_t = n*n * (1- cos*cos);
           if(sin2_t > 1.0){
                
                return 1.0;
           }
           float cos_t = std::sqrt(1-sin2_t);
           cos = cos_t;
      }
      //cout<<comps.n1<<","<<comps.n2<<endl;
      float r0 = (comps.n1 - comps.n2)/(comps.n1 + comps.n2);
      r0 = r0*r0;
      return r0 + (1-r0) * (std::pow((1-cos),5));
}

Vec4 camera::refracted_color(std::vector<hittable*> scene, Computations comps, int remaining){
      Material material;
      material = comps.hitobj->get_material();
      
      if(material.transparency == 0){
           return Vec4(0,0,0,1);
      }
      else{
          // cout<<comps.n1<<","<<comps.n2<<endl;
           float n_ratio = comps.n1/comps.n2;
           //cout<<n_ratio<<endl;
           float cos_i = dot(comps.eye_vec, comps.normal_vec);
           float sin2_t = n_ratio*n_ratio * (1- (cos_i*cos_i));
           
           if(sin2_t > 1){
                return Vec4(0,0,0,1); // total internal reflection
                
           }
           float cos_t = std::sqrt(1.0 - sin2_t);
           Vec4 dir = comps.normal_vec * (n_ratio*cos_i - cos_t) - (comps.eye_vec * n_ratio);
           ray refracted_ray(comps.underhitpoint, dir);
           std::vector<intersection> hits;
           
           for(int k=0; k<scene.size(); k++){//scene loop 
                get_hits(refracted_ray, scene[k], hits);
           }
           
           Vec4 color = returnColor(hits, refracted_ray, scene, remaining-1);  
          // color.toString();
           return color * material.transparency;
      }
      
}

Vec4 camera::reflected_color(std::vector<hittable*> scene, Computations comps, int remaining){
      Material material;
      material = comps.hitobj->get_material();
      
      if(remaining <= 0){
           return Vec4(0,0,0,1);
      }
      if(material.reflective == 0){
           return Vec4(0,0,0,1);                  
      }
      else{
           ray reflect_ray(comps.overhitpoint, comps.reflect_vec);
           std::vector<intersection> hits;
           
           for(int k=0; k<scene.size(); k++){//scene loop 
                get_hits(reflect_ray, scene[k], hits);
           }
           Vec4 color = returnColor(hits, reflect_ray, scene, remaining-1);
           return color * material.reflective;
      }
      
}

int camera::get_hit_index(std::vector<intersection> hits){
       int hit_index = -1;     
       for(int i=0; i<hits.size(); i++){
             if(hits[i].t >=0){
                    hit_index = i;
                    break;     
             }
       }
       return hit_index;
}

void camera::get_hits(ray r, hittable* s , std::vector<intersection>& hits){ 
      
      std::vector<float> t_values;
      std::vector<hittable*> hitobjects;

      s->get_values(r, t_values, hitobjects);
             
      for(int j=0; j<t_values.size(); j++){
           hits.push_back(intersection(t_values[j], hitobjects[j])); 
      } 
                                               
}


bool camera::compare_intersections(const intersection& a, const intersection& b){
     return a.t < b.t;
}

void camera::set_properties(float _aspect_ratio, int _img_width, float _focal_length, float _vp_height){
     aspect_ratio = _aspect_ratio;
     img_width = _img_width;                  
     focal_length = _focal_length;
     vp_height = _vp_height;
}

void camera::set_position(Vec4 _position, Vec4 _lookat_point){
     std::cout<<"setting new camera positions.."<<std::endl;
     position = _position;
     lookat_point = _lookat_point;
}
