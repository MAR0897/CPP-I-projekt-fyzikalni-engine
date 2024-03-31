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
double Vec::norm() { return std::pow(x*x+y*y, 0.5); }


//Objects===========================================================================================

Rectangle::Rectangle(const Vec& position, const Vec& s, const Vec& velocity, const double& m) : pos(position), size(s), vel(velocity), mass(m) {}
Rectangle::Rectangle(Vec&& position, Vec&& s, Vec&& velocity, double&& m) : pos(std::move(position)), size(std::move(s)), vel(std::move(velocity)), mass(std::move(m)) {}

Triangle::Triangle(const Vec& a, const Vec& b, const Vec& c, const Vec& velocity, const double& m) : A(a), B(b), C(c), vel(velocity), mass(m) {}
Triangle::Triangle(Vec&& a, Vec&& b, Vec&& c, Vec&& velocity, double&& m) : A(std::move(a)), B(std::move(b)), C(std::move(c)), vel(std::move(velocity)), mass(std::move(m)) {}

Circle::Circle(const Vec& position, const double& radius, const Vec& velocity, const double& m) : pos(position), rad(radius), vel(velocity), mass(m) {}
Circle::Circle(Vec&& position, double&& radius, Vec&& velocity, double&& m) : pos(std::move(position)), rad(std::move(radius)), vel(std::move(velocity)), mass(std::move(m)) {}


//----------------------------------------------------------------------------------------------------

Shapes::Shapes(const std::vector<Shape>& sh) : shapes(sh) {}
Shapes::Shapes(std::vector<Shape>&& sh) : shapes(std::move(sh)) {}
void Shapes::add_shape(const Shape& sh) { shapes.emplace_back(sh); }
void Shapes::add_shape(Shape&& sh) { shapes.emplace_back(std::move(sh)); }
void Shapes::delete_shape(const Vec& mousepos) {
    shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [&](const Shape& shape) {
        return is_inside(mousepos, shape);
    }), shapes.end());
    
}

bool Shapes::is_inside(const Vec& mousepos, const Shape& shape){
    
    bool inside = false;
    double X = mousepos.x;
    double Y = mousepos.y;

    std::visit(overloaded {
        [&](const Rectangle& r){
            if ((r.pos.x-r.size.x/2 <= X and X <= r.pos.x + r.size.x/2) and
                (r.pos.y-r.size.y/2 <= Y and Y <= r.pos.y + r.size.y/2)) inside = true;
        },
        [&](const Triangle& t){
            double d1, d2, d3;
            d1 = (X - t.B.x) * (t.A.y - t.B.y) - (t.A.x - t.B.x) * (Y - t.B.y);
            d2 = (X - t.C.x) * (t.B.y - t.C.y) - (t.B.x - t.C.x) * (Y - t.C.y);
            d3 = (X - t.A.x) * (t.C.y - t.A.y) - (t.C.x - t.A.x) * (Y - t.A.y);
            inside = not (((d1 < 0) or (d2 < 0) or (d3 < 0)) and ((d1 > 0) or (d2 > 0) or (d3 > 0)));
        },
        [&](const Circle& c){
            Vec a{X-c.pos.x, Y-c.pos.y};
            if (a.norm() <= c.rad) inside = true;
        },
    }, shape);

    return inside;
}

void Shapes::draw(const Shape& shape) {

    std::visit(overloaded {
        [](const Rectangle& r) { 
            glBegin(GL_QUADS);
            glColor3ub(0,255,0);
            glVertex2d(r.pos.x-r.size.x/2, r.pos.y-r.size.y/2);
            glVertex2d(r.pos.x+r.size.x/2, r.pos.y-r.size.y/2);
            glVertex2d(r.pos.x+r.size.x/2, r.pos.y+r.size.y/2);
            glVertex2d(r.pos.x-r.size.x/2, r.pos.y+r.size.y/2);
            glEnd();
        },
        [](const Triangle& t) {
            glBegin(GL_TRIANGLES);
            glColor3ub(0,0,255);
            glVertex2d(t.A.x, t.A.y);
            glVertex2d(t.B.x, t.B.y);
            glVertex2d(t.C.x, t.C.y);
            glEnd();
        },
        [](const Circle& c) {
            glBegin(GL_TRIANGLE_FAN);
            glColor3ub(255,0,0);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (3.1415926535/180.0);
                glVertex2d(c.pos.x + (cos(angle)*c.rad), c.pos.y + (sin(angle)*c.rad));
            }
            glEnd();
        },
    }, shape);

}

void Shapes::draw_all_shapes(){
    for (const auto& shape : shapes) draw(shape);
}

void Shapes::resize_shape(Shape& sh, const double& offset){

    std::visit(overloaded {
        [&](Rectangle& r) { r.size+=offset; },
        [&](Triangle& t) { },
        [&](Circle& c) { c.rad+=offset; },
    }, sh);
}