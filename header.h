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
#include <limits>

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

extern bool gravity;


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
    friend std::ostream& operator<<(std::ostream& os, const Vec& vec);
    double norm() const;                                    //Euclidean norm
    double distance(const Vec& other) const;                //distance between 2 points
    Vec& normalize();                                       //normalize, so that euclidean norm is 1
    double dot(const Vec& other) const;                     //dot product
    double cross(const Vec& other) const;                   //will be always perpendicular to the screen, we just need the magnitude
    Vec& rotation_transform(const double& angle);           //transform with a rotation matrix to rotate things
    Vec& perpendiculate();  // :)                           //transforms vector into a kolmy vector (swaps x and y and changes x into -x)

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

    bool is_static = false;
    bool is_selected = false;

    Properties(const double& m, const double& d, const Vec& position, const double& rotation, const Vec& velocity, const double& rv);
    Properties(double&& m, double&& d, Vec&& position, double&& rotation, Vec&& velocity, double&& rv);
};


struct Rectangle : public Properties {

    Vec size; //size.x = a side lenght; size.y = b side lenght
    std::vector<Vec> vertices; 
    std::vector<Vec> static_vertices; //pro hezke vykresleni nehnutelnych objektu :)

    Rectangle(const Vec& position, const Vec& s, const Vec& velocity = Vec(0,0), const double& m = 1.0, const double& d = 1.0, 
        const double& r = 0.0, const double& rv = 0.0);
    Rectangle(Vec&& position, Vec&& s, Vec&& velocity = Vec(0,0), double&& m = 1.0, double&& d = 1.0,
         double&& r = 0.0, double&& rv = 0.0);
};

struct Triangle : public Properties {

    double size;   //distance from center of mass to a vertex (lets just go with rovnostranne trojuhelniky)
    std::vector<Vec> vertices; 
    std::vector<Vec> static_vertices; //pro hezke vykresleni nehnutelnych objektu :)

    Triangle(const Vec& position, const double& s, const Vec& velocity = Vec(0,0), const double& m = 1.0, const double& d = 1.0,
        const double& r = 0.0, const double& rv = 0.0);
    Triangle(Vec&& position, double&& s, Vec&& velocity = Vec(0,0), double&& m = 1.0, double&& d = 1.0,
        double&& r = 0.0, double&& rv = 0.0);
};

struct Circle : public Properties {

    double rad; //radius of the circle

    Circle(const Vec& position, const double& radius, const Vec& velocity = Vec(0,0), const double& m = 1.0, const double& d = 1.0,
        const double& r = 0.0, const double& rv = 0.0);
    Circle(Vec&& position, double&& radius, Vec&& velocity = Vec(0,0), double&& m = 1.0, double&& d = 1.0,
        double&& r = 0.0, double&& rv = 0.0);
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

    //whether a certain shape has is_selected value true
    static bool is_selected(const Shape& shape);
    //select or deleselt shape
    static void toggle_selected(Shape& shape);
    //whether a certain shape has is_static value true, which means that its immovable
    static bool is_static(const Shape& shape);
    //set the shape as static
    static void toggle_static(Shape& shape);

    //checks if given coordinates are inside the shape
    static bool is_inside(const Vec& mousepos, const Shape& shape);

    //computes the vertices fro given shape
    static std::vector<Vec> get_vertices(const Shape& shape);
    //drawing a single shape
    static void draw(const Shape& shape);
    //drawing a static single shape (black with white outline)
    static void draw_static(const Shape& shape);
    //drawing all the shapes in the "shapes" vector
    void draw_all_shapes() const;

    //changes the size
    static void resize_shape(Shape& sh, const double& offset);
    //rotate shape
    static void rotate_shape(Shape& sh, const double& angle);
    //moves the shape by following the cursor
    static void move_shape_to(Shape& sh, const Vec& offset);
    //moves the shape by some value
    static void move_shape(Shape& sh, const Vec& offset);

    //update physical properties (position, velocity) by delta time
    //update position with s = v * t
    void update_position(const double& delta);
    //update velocity with v = F/m * t
    void update_by_force(const double& delta, const Vec& force);
    //update velocity with v = a * t (mainly or solely used for gravitation)
    void update_by_acceleration(const double& delta, const Vec& acceleration);

    //whether two shapes intersect
    static bool intersect(const Shape& sh1, const Shape& sh2, double& depth, Vec& normal);
        //Polygon x polygon intersection check
        static bool intersect_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, const Vec& center1, const Vec& center2, double& depth, Vec& normal);
        //Polygon x circle intersection check
        static bool intersect_polyXcirc(const std::vector<Vec>& verts1, const Circle& c);
        //Circle x circle intersection check
        static bool intersect_circXcirc(const Circle& c1, const Circle& c2, double& depth, Vec& normal);
        //Projecting vertices of both shapes to find if they intersect (Separating axis theorem)
        static void project_vertices(const std::vector<Vec>& vertices, const Vec& axis, double& max, double& min);
    //handle collisions based on intersecting
    void handle_collisions();

};


//shape handle representing all the shapes on the screen
extern Shapes sh;       //user placed shape handles
extern Shapes bgsh;     //background shape handles 



