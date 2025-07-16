#pragma once
#include "../Math/Vec4.h"



class pointLight{
      public:
             Vec4 position;
             Vec4 color;
             
      public:
             pointLight(){
             }
             pointLight(Vec4 _pos, Vec4 _color){
                        position = _pos;
                        color = _color;
             }
};
