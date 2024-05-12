//the main header, the only one that needs to be included in cpp files

#pragma once

#include "include.h"
#include "vec2D.h"
#include "objects.h"
#include "shapes.h"
#include "collisions.h"


namespace physics {

    const double PI = 3.1415926535;

    //Number of inner iterations for a physics step function for better physics accuracy
    extern int iterations;
    
    //Gravity toggler
    extern bool gravity;

    //One physics calculation frame
    void step(const double& delta, const int& iterations);

    //Cap the number of iterations to keep the simulation accurate when low number of shapes and fast enough for high number of shapes
    void cap_iterations();
}

//Screen management
namespace window {

    //Window sizes
    const int WIDTH = 1280;
    const int HEIGHT = 720;

    //Ground position
    const float GROUND = -0.7f;
    const double STATIC_SHAPES_OUTLINE = 0.01;

    //screen transform constant, multiply y axis with this to get symetrical things
    /*  IMPORTANT: the whole program works with scaled coordinates (due to window width not necessarily being the same as window height),
        only on the input (mouse coords) and output (the shapes you can see) the coordinates are scaled to "normal vision" (mainly or only the shape sizes are a problem)
    */
    const double ST = static_cast<double>(WIDTH)/static_cast<double>(HEIGHT); 

    //draws the background color and texts
    void draw_background();

    //convert coords from pixels to screen proportion (-1.0 to 1.0 on both x and y)
    Vec get_mouse_coords(GLFWwindow* window);

    //inner function for text rendering
    void render_text(const double& x, const double& y, void *font, const std::string& str);
}


//GLFW functions and key management
namespace cbs { //callbacks

    //GLFW error callback
    void errorCallback(int error, const char* description);
    
    //Keys pressing
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}








