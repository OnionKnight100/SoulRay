#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "../Math/Vec4.h"
#include "../Math/ray.h"

using namespace std;

void write_ppm(int width, int height, Vec4* pixel_data);
float pie = 3.14159265;

Vec4* load_ppm(const char* filepath, int& _width, int& _height){
    ifstream Imagefile(filepath);  //
    if(!Imagefile){
         cerr<<"Error in reading file!"<<endl;
         return nullptr;
    }
    
    std::string line;
    
    
    std::getline(Imagefile, line);// magic number
    std::getline(Imagefile, line);// gimp credits
    std::getline(Imagefile, line);// dimensions
    
    std::istringstream imagedimensions(line);
    int width,height;
    imagedimensions >> width >> height;
    _width = width;
    _height = height;
    cout<<"width: "<<width<<","<<"height: "<<height<<endl;
    
    std::getline(Imagefile, line);// max colour
    cout<<"max colour: "<<line<<endl;
    
    Vec4* pixel_data = new Vec4[width*height];
    
    for(int i=0; i<(width*height); i++){
            Vec4 color;
            
            std::getline(Imagefile, line);                  
            std::istringstream(line) >> color.x;
            std::getline(Imagefile, line);
            std::istringstream(line) >> color.y;
            std::getline(Imagefile, line);
            std::istringstream(line) >> color.z;
            
            color = convert_to_01(color);
            pixel_data[i] = color; 
    }
    
    
    return pixel_data;
}

void write_ppm(int width, int height, Vec4* pixel_data){
     //cout<<"hello"<<endl;
     ofstream outputfile("haha_hooho.ppm",ios::trunc);
     
     if(!outputfile.is_open()){
       cerr<<"error in writing file!!"<<endl;
     }
     outputfile<<"P3"<<endl<<width<<" "<<height<<endl<<255<<endl;
     for(int i=0; i<width*height; i++){
             Vec4 color = pixel_data[i];
             outputfile<<color.x<<" "<<color.y<<" "<<color.z<<endl; 
             //outputfile<<color.x/1.5<<" "<<color.y<<" "<<color.z/1.5<<endl; 
     }
     
}

Vec4 getBackgroundColor(ray r, Vec4* background_image, int width, int height){
      Vec4 dir = r.dir;
      dir.normalize();
      
      float theta = acos(dir.y);
      float phi = atan2(dir.z, dir.x);
      
      float u = (phi + pie)/(2*pie);
      float v = theta / pie;
      
      int coord_x = static_cast<int>(u*width);
      int coord_y = static_cast<int>(v*height);
      
      if(coord_x > width-1){
                 coord_x = width;
      }
      if(coord_x < 0){
                 coord_x = 0;
      }
      if(coord_y > height-1){
                 coord_x = height;
      }
      if(coord_y < 0){
                 coord_y = 0;
      }
      //coord_x = std::clamp(coord_x, 0 ,width -1);
      //coord_y = std::clamp(coord_y, 0 ,height -1);
      
      Vec4 color = background_image[coord_x + coord_y*width];
      //color.toString();
      return color;
      
}
