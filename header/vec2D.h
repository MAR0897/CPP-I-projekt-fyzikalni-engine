#pragma once

#include "include.h"

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
    bool nearly_equal(const Vec& other) const;
    static bool nearly_equal(const double& a, const double& b);
    static constexpr double nearly_equal_threshold = 0.00001;
    friend Vec operator+(const Vec& v1, const Vec& v2);
    friend Vec operator-(const Vec& v1, const Vec& v2);
    friend Vec operator*(const Vec& v1, const Vec& v2);
    friend Vec operator/(const Vec& v1, const Vec& v2);
    friend Vec operator-(const Vec& minusvec);
    friend Vec operator+(const Vec& v1, const double& n);
    friend Vec operator-(const Vec& v1, const double& n);
    friend Vec operator*(const Vec& v1, const double& n);
    friend Vec operator/(const Vec& v1, const double& n);
    friend std::ostream& operator<<(std::ostream& os, const Vec& vec);
    double norm() const;                                    //Euclidean norm = lenght of a vector
    double norm2() const;                                   //Euclidean norm squared = lenght squared for comparisons
    double distance(const Vec& other) const;                //distance between 2 points
    double distance2(const Vec& other) const;               //distance between 2 points squared for comparisons
    Vec& normalize();                                       //normalize, so that euclidean norm is 1
    double dot(const Vec& other) const;                     //dot product
    double cross(const Vec& other) const;                   //will be always perpendicular to the screen, we just need the magnitude
    Vec& rotation_transform(const double& angle);           //transform with a rotation matrix to rotate things
    Vec& perpendiculate();  // :)                           //transforms vector into a kolmy vector (swaps x and y and changes x into -x)

};