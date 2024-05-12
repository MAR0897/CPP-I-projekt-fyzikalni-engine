#include "../header/header.h"

//Custom 2D vector

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
bool Vec::nearly_equal(const Vec& other) const { return distance2(other) <= Vec::nearly_equal_threshold * Vec::nearly_equal_threshold; }
bool Vec::nearly_equal(const double& a, const double& b) { return std::abs(a-b) <= Vec::nearly_equal_threshold; }

Vec operator+(const Vec& v1, const Vec& v2) { return Vec(v1.x + v2.x, v1.y + v2.y); }
Vec operator-(const Vec& v1, const Vec& v2) { return Vec(v1.x - v2.x, v1.y - v2.y); }
Vec operator*(const Vec& v1, const Vec& v2) { return Vec(v1.x * v2.x, v1.y * v2.y); }
Vec operator/(const Vec& v1, const Vec& v2) { 
    if (v2.x == 0.0 or v2.y == 0.0) {
        std::cout<<"Error: division by zero"<<std::endl;
        return Vec(v1);
    }
    return Vec(v1.x / v2.x, v1.y / v2.y); 
}

Vec operator-(const Vec& minusvec) { return Vec( -minusvec.x, -minusvec.y); }
Vec operator+(const Vec& v1, const double& n) { return Vec(v1.x + n, v1.y + n); }
Vec operator-(const Vec& v1, const double& n) { return Vec(v1.x - n, v1.y - n); }
Vec operator*(const Vec& v1, const double& n) { return Vec(v1.x * n, v1.y * n); }
Vec operator/(const Vec& v1, const double& n) { 
    if (n == 0.0) {
        std::cout<<"Error: division by zero"<<std::endl;
        return Vec(v1);
    }
    return Vec(v1.x / n, v1.y / n); }

std::ostream& operator<<(std::ostream& os, const Vec& vec) { os<<"("<<vec.x<<", "<<vec.y<<")"<<std::endl; return os; }

double Vec::norm() const { return std::pow(x*x+y*y, 0.5); }
double Vec::norm2() const { return x*x+y*y; }
double Vec::distance(const Vec& other) const { return std::pow((x-other.x)*(x-other.x)+(y-other.y)*(y-other.y), 0.5); }
double Vec::distance2(const Vec& other) const { return (x-other.x)*(x-other.x)+(y-other.y)*(y-other.y); }
Vec& Vec::to_unit() { *this/=this->norm(); return *this; }
double Vec::dot(const Vec& other) const { return x*other.x+y*other.y; }
double Vec::cross(const Vec& other) const { return x*other.y-y*other.x; }

Vec& Vec::rotation_transform(const double& angle) {
    double temp = x;
    x = cos(angle)*x - sin(angle)*y;
    y = sin(angle)*temp + cos(angle)*y;
    return *this;
}

Vec& Vec::to_normal(){
    std::swap(x, y);
    x = -x;
    return *this;
}