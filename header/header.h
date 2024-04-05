//the main header, the only one that needs to be included in cpp files

#pragma once

#include "include.h"
#include "vec2D.h"
#include "objects.h"

struct Vec;

const double PI = 3.1415926535;
//Window sizes
const int WIDTH = 1280;
const int HEIGHT = 720;

//screen transform constant, multiply y axis with this to get symetrical things
/*  IMPORTANT: the whole program works with scaled coordinates (due to window width not necessarily being the same as window height),
    only on the input (mouse coords) and output (the shapes you can see) the coordinates are scaled to "normal vision"
    (kdo to sakra vymyslel (ale treba je to vlastne dobre, nevim))
*/
const double ST = static_cast<double>(WIDTH)/static_cast<double>(HEIGHT); 

//Ground position
const float GROUND = -0.7f;
const double STATIC_SHAPES_OUTLINE = 0.01;


namespace physics {

    extern bool gravity;
}



//GLFW functions and window management================================================================================================================
//GLFW error callback
void errorCallback(int error, const char* description);

void draw_background();


//================================================================================================================
//Keys pressing
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//convert coords from pixels to screen proportion (-1.0 to 1.0 on both x and y) (nevim jak se to jmenuje lol)
Vec get_mouse_coords(GLFWwindow* window);




