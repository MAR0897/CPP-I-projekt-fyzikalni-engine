#pragma once

#include "include.h"
#include "vec2D.h"


//Objects, shapes and so on================================================================================================================

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
        static bool intersect_polyXcirc(const std::vector<Vec>& verts1, const Circle& c, const Vec& polyg_center, double& depth, Vec& normal);
        //Circle x circle intersection check
        static bool intersect_circXcirc(const Circle& c1, const Circle& c2, double& depth, Vec& normal);
        //Projecting vertices of both shapes to find if they intersect (Separating axis theorem)
        static void project_vertices(const std::vector<Vec>& vertices, const Vec& axis, double& max, double& min);
        //converting circle into two vectices to allow projecting then onto the separating axis
        static void project_circle(const Circle& c, const Vec& axis, double& max, double& min);
    //handle collisions based on intersecting
    void handle_collisions();

};


//shape handle representing all the shapes on the screen
extern Shapes sh;       //user placed shape handles
extern Shapes bgsh;     //background shape handles 