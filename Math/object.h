// will be the parent class for all objects like sphere and cube
// will have the get_values which will return the intersections
// will have mostly pure virtual functions which will have to be implemented no matter what in subclasses
// 
#pragma once
#include <iostream>
#include "ray.h"
#include "Vec4.h"
#include "material.h"
#include <vector>

class hittable{
      public:
             virtual void get_values(ray r,std::vector<float>& t_values,std::vector<hittable*> &hitobjects) = 0;
             virtual Material get_material() = 0; 
             virtual void set_material(Material _material) = 0;
             virtual Vec4 get_albedo() = 0;
             virtual Vec4 get_albedo(Vec4 hitpoint) = 0;
             virtual void set_albedo(Vec4 _albedo) = 0;
             virtual Vec4 get_normal(Vec4 hitpoint) = 0;
             virtual void set_pattern(bool _pattern_enabled) = 0; // add pattern type too
             virtual void update_transformation(Vec4 new_pos) =0;
             virtual void set_vertex_normals(Vec4 _n1,Vec4 _n2,Vec4 _n3){std::cout<<"trying to get vertexnormals from a non triangle object! \n";}
             virtual void get_vertices(std::vector<Vec4> &vertexlist){std::cout<<"trying to get vertices from a non triangle object!";}
             virtual void set_bounds(Vec4 _min_bound,Vec4 _max_bound){std::cout<<"not a group! \n";}
             virtual void add_bounding_box(){std::cout<<"not a group! \n";}
             virtual void divide_model(int depth){std::cout<<"trying to call divide_model in a non group object!";}
            
            
            
            // vitrual function example (not pure)
            // virtual int return_id(){
            //        return 1; default, children may have diff implemention
            // }
};
