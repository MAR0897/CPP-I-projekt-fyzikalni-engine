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
double Vec::distance(const Vec& other) { return std::pow((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y), 0.5);}
Vec& Vec::normalize() { x/=norm(); y/=norm(); return *this; }
double Vec::dot(const Vec& other) { return x*other.x+y*other.y; }
double Vec::cross(const Vec& other) { return x*other.y-y*other.x; }
Vec& Vec::transform(const double& angle) {
    double temp = x;
    x = cos(angle)*x - sin(angle)*y;
    y = sin(angle)*temp +cos(angle)*y;
    return *this;
}


//Objects===========================================================================================

Properties::Properties(const double& m, const double& d, const Vec& position, const double& rotation, const Vec& velocity, const double& rv, bool ist, bool ise) :
    mass(m), density(d), pos(position), rot(rotation), vel(velocity), rotvel(rv), is_static(ist), is_selected(ise) {}
Properties::Properties(double&& m, double&& d, Vec&& position, double&& rotation, Vec&& velocity, double&& rv, bool ist, bool ise) :
    mass(std::move(m)), density(std::move(d)), pos(std::move(position)), rot(std::move(rotation)), vel(std::move(velocity)), rotvel(std::move(rv)), is_static(ist), is_selected(ise) {}

Rectangle::Rectangle(const Vec& position, const Vec& s, const Vec& velocity, const double& m, const double& d, const double& r, const double& rv, bool ist, bool ise) :
    size(s), Properties{m, d, position, r, velocity, rv, ist, ise} {}
Rectangle::Rectangle(Vec&& position, Vec&& s, Vec&& velocity, double&& m, double&& d, double&& r, double&& rv, bool ist, bool ise) : 
    size(std::move(s)), Properties{std::move(m), std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv), ist, ise} {}

Triangle::Triangle(const Vec& position, const double& s, const Vec& velocity, const double& m, const double& d, const double& r, const double& rv, bool ist, bool ise) :
    size(s), Properties{m, d, position, r, velocity, rv, ist, ise} {}
Triangle::Triangle(Vec&& position, double&& s, Vec&& velocity, double&& m, double&& d, double&& r, double&& rv, bool ist, bool ise) : 
    size(std::move(s)), Properties{std::move(m), std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv), ist, ise} {}

Circle::Circle(const Vec& position, const double& radius, const Vec& velocity, const double& m, const double& d, const double& r, const double& rv, bool ist, bool ise) :
    rad(radius), Properties{m, d, position, r, velocity, rv, ist, ise} {}
Circle::Circle(Vec&& position, double&& radius, Vec&& velocity, double&& m, double&& d, double&& r, double&& rv, bool ist, bool ise) : 
    rad(std::move(radius)), Properties{std::move(m), std::move(d), std::move(position), std::move(r), std::move(velocity), std::move(rv), ist, ise} {}


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

bool Shapes::is_selected(const Shape& shape) const {
    bool selected = false;
    std::visit(overloaded {
        [&](const Rectangle& r) { if (r.is_selected) selected = true; },
        [&](const Triangle& t) { if (t.is_selected) selected = true; },
        [&](const Circle& c) { if (c.is_selected) selected = true; },
    }, shape);
    return selected;
}

void Shapes::seldesel(Shape& shape) {
    std::visit(overloaded {
        [](Rectangle& r) { r.is_selected = !r.is_selected; },
        [](Triangle& t) { t.is_selected = !t.is_selected;  },
        [](Circle& c) { c.is_selected = !c.is_selected; },
    }, shape);
}


bool Shapes::is_inside(const Vec& mousepos, const Shape& shape) const {
    
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
            std::vector<Vec> tP = get_vertices(t);
            d1 = (X - tP[1].x) * (tP[0].y - tP[1].y) - (tP[0].x - tP[1].x) * (Y - tP[1].y);
            d2 = (X - tP[2].x) * (tP[1].y - tP[2].y) - (tP[1].x - tP[2].x) * (Y - tP[2].y);
            d3 = (X - tP[0].x) * (tP[2].y - tP[0].y) - (tP[2].x - tP[0].x) * (Y - tP[0].y);
            inside = not (((d1 < 0) or (d2 < 0) or (d3 < 0)) and ((d1 > 0) or (d2 > 0) or (d3 > 0)));
        },
        [&](const Circle& c){
            Vec a{X-c.pos.x, Y-c.pos.y};
            if (a.norm() <= c.rad) inside = true;
        },
    }, shape);

    return inside;
}

void Shapes::draw(const Shape& shape) const {

    std::vector<Vec> vertices;

    std::visit(overloaded {
        [&](const Rectangle& r) { 
            vertices = get_vertices(r);
            glBegin(GL_QUADS);

            if (r.is_selected){
                glColor3ub(0,0,0);
                glVertex2d(vertices[0].x+0.003, (vertices[0].y+0.003)*ST);
                glVertex2d(vertices[1].x-0.003, (vertices[1].y+0.003)*ST);
                glVertex2d(vertices[2].x-0.003, (vertices[2].y-0.003)*ST);
                glVertex2d(vertices[3].x+0.003, (vertices[3].y-0.003)*ST);
            }
            glColor3ub(0,255,0);
            glVertex2d(vertices[0].x, vertices[0].y*ST);
            glVertex2d(vertices[1].x, vertices[1].y*ST);
            glVertex2d(vertices[2].x, vertices[2].y*ST);
            glVertex2d(vertices[3].x, vertices[3].y*ST);
            
            glEnd();
        },
        [&](const Triangle& t) {
            vertices = get_vertices(t);
            glBegin(GL_TRIANGLES);
            glColor3ub(0,0,255);
            glVertex2d(vertices[0].x, vertices[0].y*ST);
            glVertex2d(vertices[1].x, vertices[1].y*ST);
            glVertex2d(vertices[2].x, vertices[2].y*ST);
            glEnd();
        },
        [&](const Circle& c) {
            glBegin(GL_TRIANGLE_FAN);
            glColor3ub(255,0,0);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (PI/180.0);
                glVertex2d(c.pos.x + (cos(angle)*c.rad), c.pos.y + ((sin(angle)*c.rad))*ST);
            }
            glEnd();
        },
    }, shape);

}

void Shapes::draw_all_shapes() const {
    for (const auto& shape : shapes) draw(shape);
}

void Shapes::resize_shape(Shape& sh, const double& offset){
    std::visit(overloaded {
        [&](Rectangle& r) { r.size+=offset; },
        [&](Triangle& t) { t.size+=offset; },
        [&](Circle& c) { c.rad+=offset; },
    }, sh);
}
void Shapes::rotate_shape(Shape& sh, const double& angle){
    std::visit(overloaded {
        [&](Rectangle& r) { r.rot+=angle; },
        [&](Triangle& t) { t.rot+=angle; },
        [&](Circle& c) { c.rot+=angle; },
    }, sh);
}
void Shapes::move_shape_to(Shape& sh, const Vec& offset){
    std::visit(overloaded {
        [&](Rectangle& r) { r.pos=offset; },
        [&](Triangle& t) { t.pos=offset; },
        [&](Circle& c) { c.pos=offset; },
    }, sh);
}
void Shapes::move_shape(Shape& sh, const Vec& offset){
    std::visit(overloaded {
        [&](Rectangle& r) { r.pos+=offset; },
        [&](Triangle& t) { t.pos+=offset; },
        [&](Circle& c) { c.pos+=offset; },
    }, sh);
}


std::vector<Vec> Shapes::get_vertices(const Shape& shape) const {

    std::vector<Vec> vertices;

    std::visit(overloaded {
        [&](const Rectangle& r) { 
            vertices.emplace_back(Vec{r.size.x/2, r.size.y/2});
            vertices.emplace_back(Vec{-r.size.x/2, r.size.y/2});
            vertices.emplace_back(Vec{-r.size.x/2, -r.size.y/2});
            vertices.emplace_back(Vec{r.size.x/2, -r.size.y/2});
            for(auto& vertex : vertices){
                vertex.transform(r.rot);
                vertex+=r.pos;
            }
        },
        [&](const Triangle& t) { 
            vertices.emplace_back(Vec{0, t.size});
            vertices.emplace_back(Vec{t.size*std::pow(3, 0.5)/2, -(t.size/2)});
            vertices.emplace_back(Vec{-t.size*std::pow(3, 0.5)/2, -(t.size/2)});
            for(auto& vertex : vertices){
                vertex.transform(t.rot);
                vertex+=t.pos;
            }
        },
        [&](const Circle& c) {},
    }, shape);

    return vertices;
}