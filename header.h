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

struct Vec;


//GLFW functions================================================================================================================
//GLFW error callback
void errorCallback(int error, const char* description);


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
    double norm();

};


//Objects================================================================================================================

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


struct Rectangle{

    double mass;
    Vec pos;  //center of mass
    Vec size; //size.x = a side lenght; size.y = b side lenght
    Vec vel; 

    Rectangle(const Vec& position, const Vec& s, const Vec& velocity = Vec(0,0), const double& m = 1.0);
    Rectangle(Vec&& position, Vec&& s, Vec&& velocity = Vec(0,0), double&& m = 1.0);
};

struct Triangle{

    double mass;
    Vec A, B, C;
    Vec vel; 

    Triangle(const Vec& a, const Vec& b, const Vec& c, const Vec& velocity = Vec(0,0), const double& m = 1.0);
    Triangle(Vec&& a, Vec&& b, Vec&& c, Vec&& velocity = Vec(0,0), double&& m = 1.0);
};

struct Circle{

    double mass;
    double rad;
    Vec pos;    //center of mass
    Vec vel;

    Circle(const Vec& position, const double& radius, const Vec& velocity = Vec(0,0), const double& m = 1.0);
    Circle(Vec&& position, double&& radius, Vec&& velocity = Vec(0,0), double&& m = 1.0);
};

using Shape = std::variant<Rectangle, Triangle, Circle>;


struct Shapes{

    std::vector<Shape> shapes;

    Shapes() = default;
    Shapes(const std::vector<Shape>& sh);
    Shapes(std::vector<Shape>&& sh);
    void add_shape(const Shape& sh);
    void add_shape(Shape&& sh);
    void delete_shape(const Vec& mousepos);
    //checks if given coordinates are inside the shape
    bool is_inside(const Vec& mousepos, const Shape& shape);
    //drawing a single shape using std::visit
    void draw(const Shape& shape);
    //drawing all the shapes in the "shapes" vector
    void draw_all_shapes();
    
    void resize_shape(Shape& sh, const double& offset);
};

//representing all the shapes on the screen
extern Shapes sh;



