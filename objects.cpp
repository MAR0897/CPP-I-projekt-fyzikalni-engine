#include "header.h"


//Custom 2D vector===========================================================================================
Vec::Vec(const double& X, const double& Y) : x(X), y(Y) {}
Vec::Vec(double&& X, double&& Y) : x(std::move(X)), y(std::move(Y)) {}
Vec::Vec(const Vec& v) : x(v.x), y(v.y) {}
Vec::Vec(Vec&& v) : x(std::move(v.x)), y(std::move(v.y)) {}

Vec& Vec::operator+=(const Vec& v) { x += v.x; y += v.y; return *this; }
Vec& Vec::operator-=(const Vec& v) { x -= v.x; y -= v.y; return *this; }
Vec& Vec::operator*=(const Vec& v) { x *= v.x; y *= v.y; return *this; }
Vec& Vec::operator/=(const Vec& v) { x /= v.x; y /= v.y; return *this; }
Vec& Vec::operator+=(const double& n) { x += n; y += n; return *this; }
Vec& Vec::operator-=(const double& n) { x -= n; y -= n; return *this; }
Vec& Vec::operator*=(const double& n) { x *= n; y *= n; return *this; }
Vec& Vec::operator/=(const double& n) { x /= n; y /= n; return *this; }
Vec& Vec::operator=(const Vec& v) { if(this != &v) {x = v.x; y = v.y;} return *this; }
Vec& Vec::operator=(Vec&& v) { if(this != &v) {x=std::move(v.x); y=std::move(v.y);} return *this; }
bool Vec::operator==(const Vec& other) const { return x == other.x and y == other.y; }
bool Vec::operator!=(const Vec& other) const { return not (x == other.x and y == other.y); }
Vec operator+(const Vec& v1, const Vec& v2) { return Vec(v1.x + v2.x, v1.y + v2.y); }
Vec operator-(const Vec& v1, const Vec& v2) { return Vec(v1.x - v2.x, v1.y - v2.y); }
Vec operator*(const Vec& v1, const Vec& v2) { return Vec(v1.x * v2.x, v1.y * v2.y); }
Vec operator/(const Vec& v1, const Vec& v2) { return Vec(v1.x / v2.x, v1.y / v2.y); }
Vec operator-(const Vec& minusvec) { return Vec( -minusvec.x, -minusvec.y); }
Vec operator+(const Vec& v1, const double& n) { return Vec(v1.x + n, v1.y + n); }
Vec operator-(const Vec& v1, const double& n) { return Vec(v1.x + n, v1.y + n); }
Vec operator*(const Vec& v1, const double& n) { return Vec(v1.x + n, v1.y + n); }
Vec operator/(const Vec& v1, const double& n) { return Vec(v1.x + n, v1.y + n); }


//Objects===========================================================================================

Point::Point(const Vec& position, const Vec& velocity, const double& m) : pos(position), vel(velocity), mass(m) {}
Point::Point(Vec&& position, Vec&& velocity, double&& m) : pos(std::move(position)), vel(std::move(velocity)), mass(std::move(m)) {}

Rectangle::Rectangle(const Vec& position, const Vec& s, const Vec& velocity, const double& m) : pos(position), size(s), vel(velocity), mass(m) {}
Rectangle::Rectangle(Vec&& position, Vec&& s, Vec&& velocity, double&& m) : pos(std::move(position)), size(std::move(s)), vel(std::move(velocity)), mass(std::move(m)) {}

Circle::Circle(const Vec& position, const double& radius, const Vec& velocity, const double& m) : pos(position), rad(radius), vel(velocity), mass(m) {}
Circle::Circle(Vec&& position, double&& radius, Vec&& velocity, double&& m) : pos(std::move(position)), rad(std::move(radius)), vel(std::move(velocity)), mass(std::move(m)) {}

std::vector<Shape> shapes;


void draw(const Shape& shape) {

    std::visit(overloaded {
        [](const Point& p) { },
        [](const Rectangle& r) { 
            glBegin(GL_QUADS);
            glColor3ub(255,0,0);
            glVertex2d(r.pos.x-r.size.x/2, r.pos.y-r.size.y/2);
            glColor3ub(100,100,100);
            glVertex2d(r.pos.x+r.size.x/2, r.pos.y-r.size.y/2);
            glColor3ub(0,255,0);
            glVertex2d(r.pos.x+r.size.x/2, r.pos.y+r.size.y/2);
            glColor3ub(0,0,255);
            glVertex2d(r.pos.x-r.size.x/2, r.pos.y+r.size.y/2);
            glEnd();
        },
        [](const Circle& c) {
            glBegin(GL_TRIANGLE_FAN);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (3.1415926535/180.0);
                glVertex2d(c.pos.x + (cos(angle)*c.rad), c.pos.y + (sin(angle)*c.rad));
            }
            glEnd();
        },
    }, shape);

}

void draw_all_shapes(){
    for (const auto& shape : shapes) draw(shape);
}