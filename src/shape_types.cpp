#include "../header/header.h"


//Shape types===========================================================================================

Properties::Properties(const double& m, const double& d, const Vec& position, const double& rotation, const Vec& velocity, const double& rv) :
    mass(m), density(d), pos(position), rot(rotation), vel(velocity), rotvel(rv) {}
Properties::Properties(double&& m, double&& d, Vec&& position, double&& rotation, Vec&& velocity, double&& rv) :
    mass(std::move(m)), density(std::move(d)), pos(std::move(position)), rot(std::move(rotation)), vel(std::move(velocity)), rotvel(std::move(rv)) {}

Rectangle::Rectangle(const Vec& position, const Vec& s, const Vec& velocity, const double& m, const double& d, const double& r, const double& rv) :
    size(s), 
    Properties{s.x*s.y*d, d, position, r, velocity, rv} //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
    }
Rectangle::Rectangle(Vec&& position, Vec&& s, Vec&& velocity, double&& m, double&& d, double&& r, double&& rv) : 
    size(std::move(s)),
    Properties{std::move(s.x*s.y*d), std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv)} //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
    }  

Triangle::Triangle(const Vec& position, const double& s, const Vec& velocity, const double& m, const double& d, const double& r, const double& rv) :
    size(s), 
    Properties{d*s*s*3.0*std::pow(3, 0.5)/4.0, d, position, r, velocity, rv}  //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
    } 
Triangle::Triangle(Vec&& position, double&& s, Vec&& velocity, double&& m, double&& d, double&& r, double&& rv) : 
    size(std::move(s)), 
    Properties{std::move(d*s*s*3.0*std::pow(3, 0.5)/4.0), std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv)}  //mass based on area and density
    {
        vertices = Shapes::get_vertices(*this);
        static_vertices = vertices;
    } 

Circle::Circle(const Vec& position, const double& radius, const Vec& velocity, const double& m, const double& d, const double& r, const double& rv) :
    rad(radius),
    Properties{PI*radius*radius*d, d, position, r, velocity, rv} {}   //mass based on area and density
Circle::Circle(Vec&& position, double&& radius, Vec&& velocity, double&& m, double&& d, double&& r, double&& rv) : 
    rad(std::move(radius)),
    Properties{std::move(PI*radius*radius*d), std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv)} {}  //mass based on area and density