#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../Math/triangle.h"
#include "../Math/group.h"
#include "../Math/object.h"

//returns a model(group)
//TO-DO:: make parameter for generating vertex normals or if vn present in file dont generate
hittable* ParseOBJ(const char* filepath, float scale, bool generate_normals){ 
          
     std::cout<<"Loading OBJ file ("<<filepath<<")\n";
     std::ifstream OBJfile(filepath);
     
     if(!OBJfile){
         std::cerr<<"OBJ file not found!"<<std::endl;
     }
     
     std::string line;
     int ignore_count=0;
     int num_vertices = 0;
     int num_vertex_normals = 0;
     int num_triangles = 0;
     std::vector<Vec4> vertices; //file will have 1-based index
     std::vector<hittable*> children;
     std::vector<Vec4> triangle_indices; // this will have the face points indices
     std::vector<Vec4> vertex_normals; //parallel to vertices (1 based index)
     std::vector<Vec4> generated_vertex_normals;
     Vec4 max_bound(-999999999,-99999999,-9999999);
     Vec4 min_bound(999999999,99999999,9999999);
     
     while(std::getline(OBJfile, line)){
         std::istringstream str_stream(line);
         std::string word;
         str_stream >> word;
         

         if(word == "v"){      
               Vec4 vertex;
               str_stream >> vertex.x;           
               str_stream >> vertex.y;
               str_stream >> vertex.z;
               
               //vertex.toString();
               vertex = vertex*scale;
               vertices.push_back(vertex);
               
               if(generate_normals == true){
                    // push a zero vector at the corresponding vertex normal index
                    // so when generating weighted normals just add the weighted normal per vertex to it
                    // vertex_normals.push_back(Vec4(0,0,0,0)); 
                    generated_vertex_normals.push_back(Vec4(0,0,0,0));
               }
               
               if(vertex.x>max_bound.x) max_bound.x = vertex.x;
               if(vertex.y>max_bound.y) max_bound.y = vertex.y;
               if(vertex.z>max_bound.z) max_bound.z = vertex.z;
               if(vertex.x<min_bound.x) min_bound.x = vertex.x;
               if(vertex.y<min_bound.y) min_bound.y = vertex.y;
               if(vertex.z<min_bound.z) min_bound.z = vertex.z;
               
               num_vertices++;
         }
         else if(word == "vn"){
               Vec4 vertex_normal;
               str_stream >> vertex_normal.x;           
               str_stream >> vertex_normal.y;
               str_stream >> vertex_normal.z;
               vertex_normals.push_back(vertex_normal);

               num_vertex_normals++;
         }
         else if(word == "f"){
               int vert_index_arr[3] = {-1,-1,-1}; // A, B, C
               int vert_normal_index_arr[3] = {-1,-1,-1}; 

               //int i = 0;
               // while(str_stream >> word){
               for(int i=0; i<3; i++){
                    str_stream >> word;
                    size_t pos1 = word.find('/');
                    size_t pos2 = word.find('/', pos1+1);

                    // int v_index = -1, vn_index = -1, vt_index = -1;

                    if(pos1 == std::string::npos){
                         // format is v v v
                         // std::cout << "format is v v v \n";
                         vert_index_arr[i] = std::stoi(word);
                         // std::cout << vert_index_arr[i] << " "<< vn_index << " " << vt_index <<std::endl;
                    }
                    else if(pos2 == std::string::npos){
                         // format is v/t v/t v/t
                         // std::cout << "format is v/vt v/vt v/vt \n";
                         vert_index_arr[i] = std::stoi(word.substr(0, pos1));
                         // vt_index = std::stoi(word.substr(pos1+1));
                         // std::cout << vert_index_arr[i] << " "<< vn_index << " " << vt_index <<std::endl;
                    }
                    else if(pos1 == pos2-1){
                         // format is v//vn v//vn v//vn
                         // std::cout << "format is v//vn v//vn v//vn \n";
                         vert_index_arr[i] = std::stoi(word.substr(0, pos1));
                         vert_normal_index_arr[i] = std::stoi(word.substr(pos2+1));
                         // std::cout << vert_index_arr[i] << " "<< vn_index << " " << vt_index <<std::endl;
                    }
                    else{
                         // format is v/vt/vn
                         // std::cout << "format is v/vt/vn v/vt/vn v/vt/vn \n";
                         vert_index_arr[i] = std::stoi(word.substr(0, pos1));
                         // vt_index = std::stoi(word.substr(pos1+1, pos2-pos1-1));
                         vert_normal_index_arr[i] = std::stoi(word.substr(pos2+1));
                         // std::cout << vert_index_arr[i] << " "<< vn_index << " " << vt_index <<std::endl;
                    }

                    //i++;
               }

               int A,B,C;
               A = vert_index_arr[0];
               B = vert_index_arr[1];
               C = vert_index_arr[2];
               
               // move this inside the v v v format block
               if(generate_normals==true){
                    Vec4 e1 = vertices[B-1]-vertices[A-1];
                    Vec4 e2 = vertices[C-1]-vertices[A-1];
                    Vec4 normal = cross(e1,e2); //weighted normal not normalized
                    Vec4 n1 = generated_vertex_normals[A-1];
                    Vec4 n2 = generated_vertex_normals[B-1];
                    Vec4 n3 = generated_vertex_normals[C-1];
                    n1 = n1 + normal;
                    n2 = n2 + normal;
                    n3 = n3 + normal;
                    generated_vertex_normals[A-1] = n1;
                    generated_vertex_normals[B-1] = n2;
                    generated_vertex_normals[C-1] = n3;
                    //add the weighted normal to Vec4s at the 3 vertex_normal indices
               }
                    
               
               hittable* tri = new Triangle(vertices[A-1],vertices[B-1],vertices[C-1]);
               if (!generate_normals && !vertex_normals.empty()){
                   tri->set_vertex_normals(
                    vertex_normals[vert_normal_index_arr[0]-1],
                    vertex_normals[vert_normal_index_arr[1]-1],
                    vertex_normals[vert_normal_index_arr[2]-1]
                    ); 
               }
               // add the point indices parallel to children pos in the face array
               // basically triangle_indices stores the vertex indeces of the triangle in the children array
               // so triangle_indices[1] will have the vertex_indices for the triangle at children[1] for later use
               triangle_indices.push_back(Vec4(float(A),float(B),float(C),1)); 
               children.push_back(tri);
               num_triangles++;

         }
         else{
              ignore_count ++;
         }
         
         
     }

     // now all the vertex normals are generated
     if(generate_normals==true){
          // normalize all generated vertex normals;
          // normalized after its added by weights 
          for(int j=0; j<generated_vertex_normals.size(); j++){
               generated_vertex_normals[j].normalize();
               // vertex_normals[j].toString();
          }
          for(int i=0; i<children.size(); i++){
               Vec4 indices = triangle_indices[i];
               int A = int(indices.x);
               int B = int(indices.y);
               int C = int(indices.z);
               children[i]->set_vertex_normals(generated_vertex_normals[A-1],generated_vertex_normals[B-1],generated_vertex_normals[C-1]);
          }
     }
          
      
     std::cout<<"  "<<ignore_count<<" lines ignored \n  "<<num_vertices<<" vertices... \n  ";
     std::cout <<"  "<<num_vertex_normals<<" vertex normals... \n"<<num_triangles<<" triangles... \n";
     hittable* model = new group(children);
     model->set_bounds(min_bound,max_bound);
     return model;
     
           
}

