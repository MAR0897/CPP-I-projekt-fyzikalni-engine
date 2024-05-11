#pragma once

#include "vec2D.h"


//Objects - Axis aligned boundary box, Geometrical objects (Rectangle, Triangle, Circle)


//=============================================================================================================================================
//Variadic templates for easy std::variant access to different types of geometric shapes
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

//=============================================================================================================================================
//Axis-aligned bounding box (box around points of the shapes that have min and max value in relation to x and y axis)
struct AABB{

    Vec min;
    Vec max;

    AABB() = default;
    AABB(const Vec& min, const Vec& max);
    AABB(Vec&& min, Vec&& max);
    AABB(const double& minX, const double& maxX, const double& minY, const double& maxY);
    AABB(double&& minX, double&& maxX, double&& minY, double&& maxY);
};

//=============================================================================================================================================
//Physical properties of geometric shapes

struct Properties{

    double mass;
    double inverse_mass;        //inverse value of mass for easier calculations
    double density; 
    double restitution;         //vzpruživost - koeficient restituce - pružnost, ale co se týče odrazů - od 0 do 1
    double rot_inertia;         //moment setrvačnosti (takový ten hezký tenzor, který tady naštěstí tenzorem nebude)
    double inverse_rot_inertia; //inverse value of rotational inertia for easier calculations
    double friction_s;          //statické tření 
    double friction_d;          //dynamické tření – tření za pohybu

    Vec pos;                    //center of mass = teziste
    double rot;                 //rotation of an object (0 to 2PI rad)

    Vec vel;                    //velocity  
    double rotvel;              //angular velocity; omega = 2pi*frequency (+ = proti smeru rucicek, - = po smeru rucicek)

    Vec force{0.0, 0.0};        //force applied to the object every iteration

    bool is_static = false;     //object is immovable and black with white margin if static
    bool is_selected = false;   //object is red if selected

    Properties(const double& m, const double& den,
            const Vec& position, const double& rotation, 
            const Vec& velocity, const double& rv,
            const double& rest, const double& sf, const double& df,
            const bool& is_static, const bool& is_selected
            );
    Properties(double&& m, double&& den,
            Vec&& position, double&& rotation, 
            Vec&& velocity, double&& rv,
            double&& rest, double&& sf, double&& df,
            bool&& is_static, bool&& is_selected
            );
};

//=============================================================================================================================================
//Geometric shapes - Rectangle, Triangle and Circle

namespace physics{
    extern const double RESTITUTION;
    extern const double STATIC_FRICTION;
    extern const double DYNAMIC_FRICTION;
}

struct Rectangle : public Properties {

    Vec size; //size.x = a side lenght; size.y = b side lenght
    std::vector<Vec> vertices; 
    std::vector<Vec> static_vertices; //pro hezke vykresleni nehnutelnych objektu
    AABB box;

    Rectangle(const Vec& position, const Vec& size, const double& den = 1.0, const double& rotation = 0.0, 
            const Vec& velocity = Vec{0.0,0.0}, const double& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            const bool& is_static = false, const bool& is_selected = false);
    Rectangle(Vec&& position, const Vec&& size, double&& den = 1.0, double&& rotation = 0.0, 
            Vec&& velocity = Vec{0.0,0.0}, double&& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            bool&& is_static = false, bool&& is_selected = false);
};

struct Triangle : public Properties {

    double size;   //distance from center of mass to a vertex (lets just go with rovnostranne trojuhelniky)
    std::vector<Vec> vertices; 
    std::vector<Vec> static_vertices; //pro hezke vykresleni nehnutelnych objektu
    AABB box;

    Triangle(const Vec& position, const double& size, const double& den = 1.0, const double& rotation = 0.0, 
            const Vec& velocity = Vec{0.0,0.0}, const double& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            const bool& is_static = false, const bool& is_selected = false);
    Triangle(Vec&& position, const double&& size, double&& den = 1.0, double&& rotation = 0.0, 
            Vec&& velocity = Vec{0.0,0.0}, double&& rv = 0.0,
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            bool&& is_static = false, bool&& is_selected = false);
};

struct Circle : public Properties {

    double rad; //radius of the circle
    AABB box;

    Circle(const Vec& position, const double& radius, const double& den = 1.0, const double& rotation = 0.0, 
            const Vec& velocity = Vec{0.0,0.0}, const double& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            const bool& is_static = false, const bool& is_selected = false);
    Circle(Vec&& position, const double&& radius, double&& den = 1.0, double&& rotation = 0.0, 
            Vec&& velocity = Vec{0.0,0.0}, double&& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            bool&& is_static = false, bool&& is_selected = false);
};
