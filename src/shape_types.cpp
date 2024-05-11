#include "../header/header.h"


//Shape types===========================================================================================

Properties::Properties(const double& m, const double& den,
            const Vec& position, const double& rotation, 
            const Vec& velocity, const double& rv,
            const double& rest, const double& sf, const double& df,
            const bool& is_static, const bool& is_selected
            ) :
    mass(m), density(den), pos(position), rot(rotation), vel(velocity), rotvel(rv), inverse_mass(1.0/mass),
    restitution(rest), friction_s(sf), friction_d(df), is_static(is_static), is_selected(is_selected) {}
    
Properties::Properties(double&& m, double&& den,
            Vec&& position, double&& rotation, 
            Vec&& velocity, double&& rv,
            double&& rest, double&& sf, double&& df,
            bool&& is_static, bool&& is_selected
            ) :
    mass(std::move(m)), density(std::move(den)), pos(std::move(position)), rot(std::move(rotation)), vel(std::move(velocity)),
    rotvel(std::move(rv)), inverse_mass(1.0/mass), restitution(std::move(rest)), friction_s(std::move(sf)), friction_d(std::move(df)),
    is_static(std::move(is_static)), is_selected(std::move(is_selected)) {}


AABB::AABB(const Vec& min, const Vec& max) : min(min), max(max) {}
AABB::AABB(Vec&& min, Vec&& max) : min(std::move(min)), max(std::move(max)) {}
AABB::AABB(const double& minX, const double& maxX, const double& minY, const double& maxY) : min(Vec{minX, minY}), max(Vec{maxX, maxY}) {}
AABB::AABB(double&& minX, double&& maxX, double&& minY, double&& maxY) : min(Vec{std::move(minX), std::move(minY)}), max(Vec{std::move(maxX), std::move(maxY)}) {}


Rectangle::Rectangle(const Vec& position, const Vec& size, const double& den, const double& rotation, 
            const Vec& velocity, const double& rv, const double& rest, const double& sf, const double& df,
            const bool& is_static, const bool& is_selected) :
                size(size), 
                Properties{size.x*size.y*den, den, position, rotation, velocity, rv, rest, sf, df, is_static, is_selected} //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
        box = Shapes::create_AABB(*this);
        rot_inertia = (1.0/12.0)*mass*size.norm2();
        inverse_rot_inertia = 1.0/rot_inertia;
    }

Rectangle::Rectangle(Vec&& position, const Vec&& size, double&& den, double&& rotation, 
            Vec&& velocity, double&& rv, const double& rest, const double& sf, const double& df,
            bool&& is_static, bool&& is_selected) : 
                size(std::move(size)),
                Properties{size.x*size.y*den, std::move(den), std::move(position), std::move(rotation), std::move(velocity), std::move(rv),
                std::move(rest), std::move(sf), std::move(df), std::move(is_static), std::move(is_selected)} //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
        box = Shapes::create_AABB(*this);
        rot_inertia = (1.0/12.0)*mass*size.norm2();
        inverse_rot_inertia = 1.0/rot_inertia;
    }  

Triangle::Triangle(const Vec& position, const double& size, const double& den, const double& rotation, 
            const Vec& velocity, const double& rv, const double& rest, const double& sf, const double& df,
            const bool& is_static, const bool& is_selected) :
                size(size),  
                Properties{den*size*size*3.0*std::pow(3, 0.5)/4.0, den, position, rotation, velocity, rv, rest, sf, df, is_static, is_selected}  //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
        box = Shapes::create_AABB(*this);
        rot_inertia = (1.0/12.0)*mass*size*std::pow(3, 0.5);
        inverse_rot_inertia = 1.0/rot_inertia;
    } 
    
Triangle::Triangle(Vec&& position, const double&& size, double&& den, double&& rotation, 
            Vec&& velocity, double&& rv, const double& rest, const double& sf, const double& df,
            bool&& is_static, bool&& is_selected) : 
                size(size),
                Properties{den*size*size*3.0*std::pow(3, 0.5)/4.0, std::move(den), std::move(position), std::move(rotation), std::move(velocity), std::move(rv),
                std::move(rest), std::move(sf), std::move(df), std::move(is_static), std::move(is_selected)}  //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
        box = Shapes::create_AABB(*this);
        rot_inertia = (1.0/12.0)*mass*size*std::pow(3, 0.5);
        inverse_rot_inertia = 1.0/rot_inertia;
    } 

Circle::Circle(const Vec& position, const double& radius, const double& den, const double& rotation, 
            const Vec& velocity, const double& rv, const double& rest, const double& sf, const double& df,
            const bool& is_static, const bool& is_selected) :
                rad(radius),
                Properties{PI*radius*radius*den, den, position, rotation, velocity, rv, rest, sf, df, is_static, is_selected}//mass based on area and density
    {
        box = Shapes::create_AABB(*this);
        rot_inertia = 0.5*mass*rad*rad;
        inverse_rot_inertia = 1.0/rot_inertia;
    }  

Circle::Circle(Vec&& position, const double&& radius, double&& den, double&& rotation, 
            Vec&& velocity, double&& rv, const double& rest, const double& sf, const double& df,
            bool&& is_static, bool&& is_selected) :
                rad(radius),
                Properties{PI*radius*radius*den, std::move(den), std::move(position), std::move(rotation), std::move(velocity), std::move(rv),
                std::move(rest), std::move(sf), std::move(df), std::move(is_static), std::move(is_selected)}//mass based on area and density
    {
        box = Shapes::create_AABB(*this);
        rot_inertia = 0.5*mass*rad*rad;
        inverse_rot_inertia = 1.0/rot_inertia;
    }  