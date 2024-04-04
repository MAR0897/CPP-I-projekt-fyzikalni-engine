#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <functional>
#include <cmath>
#include <string_view>
#include <variant>
#include <chrono>
#include <thread>

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


//Custom 2D vector================================================================================================================

struct Vec{

    double x;
    double y;

    Vec() = default;
    Vec(const double& X, const double& Y);
    Vec(double&& X, double&& Y);
    Vec(const Vec& v);
    Vec(Vec&& v);

    Vec& operator+=(const Vec& v);
    Vec& operator-=(const Vec& v);
    Vec& operator*=(const Vec& v);
    Vec& operator/=(const Vec& v);
    Vec& operator+=(const double& n);
    Vec& operator-=(const double& n);
    Vec& operator*=(const double& n);
    Vec& operator/=(const double& n);
    Vec& operator=(const Vec& v);
    Vec& operator=(Vec&& v);
    bool operator==(const Vec& other) const;
    bool operator!=(const Vec& other) const;
    friend Vec operator+(const Vec& v1, const Vec& v2);
    friend Vec operator-(const Vec& v1, const Vec& v2);
    friend Vec operator*(const Vec& v1, const Vec& v2);
    friend Vec operator/(const Vec& v1, const Vec& v2);
    friend Vec operator-(const Vec& minusvec);
    friend Vec operator+(const Vec& v1, const double& n);
    friend Vec operator-(const Vec& v1, const double& n);
    friend Vec operator*(const Vec& v1, const double& n);
    friend Vec operator/(const Vec& v1, const double& n);
    double norm();                                  //Euclidean norm
    double distance(const Vec& other);              //distance between 2 points
    Vec& normalize();                               //normalize, so that euclidean norm is 1
    double dot(const Vec& other);                   //dot product
    double cross(const Vec& other);                 //will be always perpendicular to the screen, we just need the magnitude
    Vec& rotation_transform(const double& angle);   //transform with a rotation matrix to rotate things

};


//Objects================================================================================================================

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct Properties{

    double mass;
    double density;

    Vec pos;        //center of mass = teziste
    double rot;     //rotation of an object (0 to 2PI rad)

    Vec vel;        //velocity  
    double rotvel;  //angular velocity; omega = 2pi*frequency

    bool is_static;
    bool is_selected;

    Properties(const double& m, const double& d, const Vec& position, const double& rotation, const Vec& velocity, const double& rv, bool ist, bool ise);
    Properties(double&& m, double&& d, Vec&& position, double&& rotation, Vec&& velocity, double&& rv, bool ist, bool ise);
};


struct Rectangle : public Properties {

    Vec size; //size.x = a side lenght; size.y = b side lenght

    Rectangle(const Vec& position, const Vec& s, const Vec& velocity = Vec(0,0), const double& m = 1.0, const double& d = 1.0, 
        const double& r = 0.0, const double& rv = 0.0, bool ist = false, bool ise = false);
    Rectangle(Vec&& position, Vec&& s, Vec&& velocity = Vec(0,0), double&& m = 1.0, double&& d = 1.0,
         double&& r = 0.0, double&& rv = 0.0, bool ist = false, bool ise = false);
};

struct Triangle : public Properties {

    double size;   //distance from center of mass to a vertex (lets just go with rovnostranne trojuhelniky)

    Triangle(const Vec& position, const double& s, const Vec& velocity = Vec(0,0), const double& m = 1.0, const double& d = 1.0,
        const double& r = 0.0, const double& rv = 0.0, bool ist = false, bool ise = false);
    Triangle(Vec&& position, double&& s, Vec&& velocity = Vec(0,0), double&& m = 1.0, double&& d = 1.0,
        double&& r = 0.0, double&& rv = 0.0, bool ist = false, bool ise = false);
};

struct Circle : public Properties {

    double rad; //radius of the circle

    Circle(const Vec& position, const double& radius, const Vec& velocity = Vec(0,0), const double& m = 1.0, const double& d = 1.0,
        const double& r = 0.0, const double& rv = 0.0, bool ist = false, bool ise = false);
    Circle(Vec&& position, double&& radius, Vec&& velocity = Vec(0,0), double&& m = 1.0, double&& d = 1.0,
        double&& r = 0.0, double&& rv = 0.0, bool ist = false, bool ise = false);
};

using Shape = std::variant<Rectangle, Triangle, Circle>;

//vector of shapes (shapes handle)
struct Shapes{

    std::vector<Shape> shapes;

    Shapes() = default;
    Shapes(const std::vector<Shape>& sh);
    Shapes(std::vector<Shape>&& sh);
    void add_shape(const Shape& sh);
    void add_shape(Shape&& sh);
    void delete_shape(const Vec& mousepos);
    //drawing all the shapes in the "shapes" vector
    void draw_all_shapes() const;
    //update physical properties (position, velocity) by delta time
    void update_position(const double& delta);
    void update_by_force(const double& delta, const Vec& force);
    void update_by_acceleration(const double& delta, const Vec& acceleration);

    void handle_intersections();

};

//functions for individual shapes
namespace shf{
    //whether two shapes intersect
    bool intersect(const Shape& sh1, const Shape& sh2);
    //whether a certain shape has is_selected value true
    bool is_selected(const Shape& shape);
    //select or deleselt shape
    void seldesel(Shape& shape);
    //checks if given coordinates are inside the shape
    bool is_inside(const Vec& mousepos, const Shape& shape);
    //drawing a single shape
    void draw(const Shape& shape);
    //changes the size
    void resize_shape(Shape& sh, const double& offset);
    //rotate shape
    void rotate_shape(Shape& sh, const double& angle);
    //moves the shape by following the cursor
    void move_shape_to(Shape& sh, const Vec& offset);
    //moves the shape by some value
    void move_shape(Shape& sh, const Vec& offset);
    //computes the vertices fro given shape
    std::vector<Vec> get_vertices(const Shape& shape);
}


//shape handle representing all the shapes on the screen
extern Shapes sh;       //user placed shape handles
extern Shapes bgsh;     //background shape handles 



