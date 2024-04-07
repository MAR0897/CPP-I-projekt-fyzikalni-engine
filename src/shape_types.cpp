#include "../header/header.h"


//Shape types===========================================================================================

Properties::Properties(const double& m, const double& d, const Vec& position, const double& rotation, const Vec& velocity, const double& rv) :
    mass(m), density(d), pos(position), rot(rotation), vel(velocity), rotvel(rv), inverse_mass(1.0/mass) {}
    
Properties::Properties(double&& m, double&& d, Vec&& position, double&& rotation, Vec&& velocity, double&& rv) :
    mass(std::move(m)), density(std::move(d)), pos(std::move(position)), rot(std::move(rotation)), vel(std::move(velocity)),
    rotvel(std::move(rv)), inverse_mass(1.0/mass) {}

AABB::AABB(const Vec& min, const Vec& max) : min(min), max(max) {}
AABB::AABB(Vec&& min, Vec&& max) : min(std::move(min)), max(std::move(max)) {}
AABB::AABB(const double& minX, const double& maxX, const double& minY, const double& maxY) : min(Vec{minX, minY}), max(Vec{maxX, maxY}) {}
AABB::AABB(double&& minX, double&& maxX, double&& minY, double&& maxY) : min(Vec{std::move(minX), std::move(minY)}), max(Vec{std::move(maxX), std::move(maxY)}) {}


Rectangle::Rectangle(const Vec& position, const Vec& s, const Vec& velocity, const double& d, const double& r, const double& rv) :
    size(s), 
    Properties{s.x*s.y*d, d, position, r, velocity, rv} //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
        box = Shapes::create_AABB(*this);
        rot_inertia = (1.0/12.0)*mass*size.norm2();
        inverse_rot_inertia = 1.0/rot_inertia;
    }
Rectangle::Rectangle(Vec&& position, Vec&& s, Vec&& velocity, double&& d, double&& r, double&& rv) : 
    size(std::move(s)),
    Properties{s.x*s.y*d, std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv)} //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
        box = Shapes::create_AABB(*this);
        rot_inertia = (1.0/12.0)*mass*size.norm2();
        inverse_rot_inertia = 1.0/rot_inertia;
    }  

Triangle::Triangle(const Vec& position, const double& s, const Vec& velocity, const double& d, const double& r, const double& rv) :
    size(s), 
    Properties{d*s*s*3.0*std::pow(3, 0.5)/4.0, d, position, r, velocity, rv}  //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
        box = Shapes::create_AABB(*this);
        rot_inertia = (1.0/12.0)*mass*size*std::pow(3, 0.5);
        inverse_rot_inertia = 1.0/rot_inertia;
    } 
Triangle::Triangle(Vec&& position, double&& s, Vec&& velocity, double&& d, double&& r, double&& rv) : 
    size(std::move(s)), 
    Properties{d*s*s*3.0*std::pow(3, 0.5)/4.0, std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv)}  //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
        box = Shapes::create_AABB(*this);
        rot_inertia = (1.0/12.0)*mass*size*std::pow(3, 0.5);
        inverse_rot_inertia = 1.0/rot_inertia;
    } 

Circle::Circle(const Vec& position, const double& radius, const Vec& velocity, const double& d, const double& r, const double& rv) :
    rad(radius),
    Properties{PI*radius*radius*d, d, position, r, velocity, rv}//mass based on area and density
    {
        box = Shapes::create_AABB(*this);
        rot_inertia = 0.5*mass*rad*rad;
    }   
Circle::Circle(Vec&& position, double&& radius, Vec&& velocity, double&& d, double&& r, double&& rv) : 
    rad(std::move(radius)),
    Properties{PI*radius*radius*d, std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv)}//mass based on area and density
    {
        box = Shapes::create_AABB(*this);
        rot_inertia = 0.5*mass*rad*rad;
    }  