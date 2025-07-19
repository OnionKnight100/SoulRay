#include <cstdlib>
#include <iostream>
#include "Math/Vec4.h"
#include "Math/ray.h"
#include "Math/sphere.h"
#include "Math/cube.h"
#include "Math/triangle.h"
#include "Math/plane.h"
#include "Math/group.h"
#include "Math/object.h"
#include "Math/material.h"
#include "Math/intersection.h"
#include "other/camera.h"
#include "other/light.h"
#include <vector>
#include <string>
#include <ctime>


using namespace std;


void practice();
Vec4* load_ppm(const char* filepath, int& _width, int& _height);
hittable* ParseOBJ(const char* filepath,float scale,bool generate_normals);

int main(int argc, char *argv[])
{   
    // CLI here which will call the renderer
    // CLI will have default values section that we can change
    // Make the CLI like a old school text rpg gui
    // load the obj files for use from here
    // load the environment image for use if any...
    // set the different camera positions, number of cameras to render from
    // set the location to save the file to
    // add a help option for info on stuff
    // add while loop here in which call cli that will in turn call everything
    // 
    practice();
    
    //system("PAUSE");
    return EXIT_SUCCESS;
}



void practice(){
     
    hittable* s1 = new sphere(Vec4(0,1,0,Vec4::point),2);
    hittable* s1_inner = new sphere(Vec4(0,0,0,Vec4::point),1.4);
    hittable* gold = new sphere(Vec4(0,2,0,Vec4::point),1);
    hittable* copper = new sphere(Vec4(0,2,0,Vec4::point),1);
    hittable* silver = new sphere(Vec4(0,0,0,Vec4::point),2);
    
    Vec4 groundnormal = Vec4(0,1,0,0);
    groundnormal.normalize();
    hittable* ground = new Plane(Vec4(0,-4,0,Vec4::point), groundnormal);
    
    s1->set_albedo(Vec4(0,0,0,1));
    s1->set_material(Material(0.1,0.2,0.9,300.0,1,1,1.5));
    s1_inner->set_albedo(Vec4(0,1,0,1));
    s1_inner->set_material(Material(0.1,0.2,0.9,300.0,1,1,1.0));
        
    gold->set_albedo(Vec4(1.0,0.83,0.0,1));
    gold->set_material(Material(0.24,0.75,0.62,51.2,0.9,0,1));
    
    copper->set_albedo(Vec4(0.95,0.63,0.53,1));
    copper->set_material(Material(0.19,0.70,0.256,12.8,0.8,0,1));
    
    silver->set_albedo(Vec4(0.97,0.96,0.91,1));
    silver->set_material(Material(0.19,0.50,0.50,51.2,0.9,0,1));
    
    ground->set_albedo(Vec4(0,1,0,1));
   // ground->set_albedo(Vec4(0,0,0,1));
   // ground->set_material(Material(0.1,0.9,0.9,200.0,0.4));
    
    std::vector<hittable*> children;
    hittable* s7 = new sphere(Vec4(2,1,0,Vec4::point),1);
    hittable* s8 = new sphere(Vec4(-2,1,-3,Vec4::point),1);
    s8->set_albedo(Vec4(0.8,0.2,0.1,1));
    hittable* s9 = new sphere(Vec4(-1,1,3,Vec4::point),1);
    
    children.push_back(s7);
    children.push_back(s8);
    children.push_back(s9);
    
    children.push_back(new sphere(Vec4(-1,2.3,-3,Vec4::point),1));
    
    hittable* triad = new group(children);    
      
    hittable* wall1 = new Plane(Vec4(0,0,-17,Vec4::point), Vec4(0,0,1,Vec4::point));
    wall1->set_albedo(Vec4(1,0,0,1));
    hittable* wall2 = new Plane(Vec4(-19,0,0,Vec4::point), Vec4(1,0,0,Vec4::point));
    wall2->set_albedo(Vec4(1,1,0,1));
      
    
    
    // hittable* pumpkin = ParseOBJ("models/simple/pumpkin.obj",0.1,true);
    // pumpkin->update_transformation(Vec4(5,-1,-1,Vec4::point));
    // pumpkin->divide_model(10);

    hittable* cow = ParseOBJ("models/simple/cow.obj" ,1 ,true);
    cow->update_transformation(Vec4(4,-0.65,4,Vec4::point));
    cow->update_transformation(Vec4(-14,0.2,-15,Vec4::point));
    cow->set_albedo(Vec4(0.8,0.2,0.6,1));
    // ----------------------------------------------------------------
        // Iron
        // cow->set_material(Material(0.1,0.1,1.0,64.0,0.9));
        // cow->set_albedo(Vec4(0.56,0.57,0.58,1));
    // ----------------------------------------------------------------
    
    // //silver
    // cow->set_material(Material(0.19,0.50,0.50,51.2,0.6,0,1));
    // cow->set_albedo(Vec4(0.4,0.4,0.4,1));
    // //glass
    // //cow->set_albedo(Vec4(0,0,0,1));
    // //cow->set_material(Material(0.1,0.2,0.9,300.0,1,1,1.5));
    cow->divide_model(8);

    hittable* teddy = ParseOBJ("models/simple/teddy.obj",0.25,false);
    teddy->update_transformation(Vec4(-4,1,-2,Vec4::point));
    // ----------------------------------------------------------------
        // Iron
        teddy->set_material(Material(0.1,0.1,1.0,64.0,0.9,0,1));
        teddy->set_albedo(Vec4(0.56,0.57,0.58,1));
    // ----------------------------------------------------------------
    // //teddy->set_material(Material(0.19,0.50,0.50,51.2,0.9,0,1));
    // teddy->set_albedo(Vec4(0.97,0.96,0.91,1));
    teddy->divide_model(6);

    hittable* teapot = ParseOBJ("models/simple/teapot.obj",1.9,true);
    // teapot->update_transformation(Vec4(1,-3.7,-2,Vec4::point));
    teapot->update_transformation(Vec4(3,-3.7,-10,Vec4::point));
    // ----------------------------------------------------------------
        // Iron
        // teapot->set_material(Material(0.1,0.1,1.0,64.0,0.9,0,1));
        // teapot->set_albedo(Vec4(0.56,0.57,0.58,1));
    // ----------------------------------------------------------------
    // teapot->set_material(Material(0.19,0.50,0.50,51.2,0.6,0,1));
    teapot->set_albedo(Vec4(0.7,0.8,0.2,1));

    // // material properties as the glass in book cover image
    // // teapot->set_albedo(Vec4(0.373,0.404,0.550,1));
    // // teapot->set_material(Material(0,0.2,1.0,200.0,0.7,0.7,1.5));
    teapot->divide_model(10);

    hittable* dragon = ParseOBJ("models/dragon/dragon.obj" ,1.7 ,true);
    // dragon->update_transformation(Vec4(3.2,-2,-1,1));
    dragon->update_transformation(Vec4(3,-3.5,2,1)); //save
    dragon->set_albedo(Vec4(0.2,0.8,0.7,1)); //really cool colour
     // ----------------------------------------------------------------
        // Iron
        // dragon->set_material(Material(0.02, 0.1, 0.9, 150.0, 0.7));
        // dragon->set_albedo(Vec4(0.1, 0.1, 0.12,1));
        // Custom
        // dragon->set_material(Material(0.1,0.6,0.3,15.0,0));
        // dragon->set_albedo(Vec4(1,0,0.1,1));
    // ----------------------------------------------------------------
    // dragon->set_material(Material(0.19,0.50,0.50,51.2,0.6,0,1));
    // dragon->set_albedo(Vec4(0.4,0.4,0.4,1));
    
    dragon->divide_model(15);

    // hittable* tree = ParseOBJ("extra_models/low_poly_tree/Lowpoly_tree_sample.obj",0.3,false);
    // tree->update_transformation(Vec4(3.2,0,2,1));
    // tree->divide_model(5);

    hittable* matte_sphere = new sphere(Vec4(1,-2,7,Vec4::point),1.3);
    matte_sphere->set_albedo(Vec4(0.3,0.05,0.05,1));
    matte_sphere->set_material(Material(0.1,0.7,0.1,10.0,0));


    std::vector<hittable*> scene;
    // scene.push_back(matte_sphere);
    scene.push_back(teapot);
    // scene.push_back(teddy);
    // scene.push_back(pumpkin);
    scene.push_back(cow); 
    scene.push_back(dragon);
    // scene.push_back(tree);
    // scene.push_back(triad);
    // scene.push_back(silver);
    // scene.push_back(s1);
    // scene.push_back(copper);
    // scene.push_back(s7);
    scene.push_back(ground);
    // scene.push_back(wall1);
    // scene.push_back(wall2);
    
    
    
    
    
    
    double total_time_taken = 0.0;
    
    std::clock_t start_time = std::clock();
    //--------------------background image loading code ------------------------------------------------------------------
    int background_width,background_height;
    // Vec4* background_image = load_ppm("other/Neighbourhood.ppm", background_width, background_height);
    //cout<<background_width<<","<<background_height<<endl;
    //--------------------------------------------------------------------------------------------------------------------
    std::clock_t end_time = std::clock();
    
    double delta_time = double(end_time - start_time)/ CLOCKS_PER_SEC;
    cout<<"--------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"Loaded environment image.. Time taken: "<< delta_time<<" secs..."<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------------------"<<endl;
    total_time_taken += delta_time;
    
    //--------------------render ------------------------------------------------------------------
    camera cam1("camera1");
    // cam1.background_image = background_image;
    cam1.background_img_width = background_width;
    cam1.background_img_height = background_height;
    //cam1.set_position(Vec4(10,8,20,Vec4::point), Vec4(2,0,0,Vec4::point));
    cam1.set_position(Vec4(10,14,20,Vec4::point), Vec4(2,0,0,Vec4::point));
    start_time = std::clock();
    cam1.render(scene,"output/camera1.ppm");
    //--------------------------------------------------------------------------------------------------------------------
    end_time = std::clock();
    delta_time = double(end_time - start_time)/ CLOCKS_PER_SEC;
    cout<<"--------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"camera 1.. Time taken: "<< delta_time<<" secs..."<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------------------"<<endl;
    total_time_taken += delta_time;
    
    
    cout<<"Total time taken: "<< total_time_taken<<" secs..."<<endl;
    
    
    // delete[] background_image;
     
}


