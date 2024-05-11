#pragma once

#include "shapes.h"

//Collisions struct for storing contact points between every pair of intersecting shapes

struct Collisions{

    //shapes the collision belongs to
    Shape& sh1;
    Shape& sh2;

    //how much they are intersecting and in which direction they are gonna bounce off
    double depth;
    Vec normal;

    //contact points
    Vec contact1;
    Vec contact2;
    int contact_count;

    Collisions(Shape& sh1, Shape& sh2, const double& depth, const Vec& normal, const Vec& contact1, const Vec& contact2, const int& contact_count);
    Collisions(Shape& sh1, Shape& sh2, double&& depth, Vec&& normal, Vec&& contact1, Vec&& contact2, const int& contact_count);
    
    //all pair of intersecting shapes
    static std::vector<Collisions> contacts;

    //finds where do the colliding shapes contact
    static void find_contact_points(const Shape& sh1, const Shape& sh2, Vec& c_point1, Vec& c_point2, int& c_points_count);
        template<typename S1, typename S2>
        static void m_find_contact_points(const S1& s1, const S2& s2, Vec& c_point1, Vec& c_points2, int& c_points_count);

        //find contact points for two intersecting circles
        static void fcp_circXcirc(const Circle& c1, const Circle& c2, Vec& c_point1, int& c_points_count);
        //find contact points for a polygon and a circle
        static void fcp_polyXcirc(const Circle& c, const std::vector<Vec>& verts, Vec& c_point1, int& c_points_count);
        //find contact points for two intersecting polygons
        static void fcp_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, Vec& c_point1, Vec& c_point2, int& c_points_count);
            //inner helper function for finding contact points for two polygons
            static void m_fcp_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, Vec& c_point1, Vec& c_point2, int& c_points_count, Vec& contact, double& min_distance);
        //geometrical distance from a single point to a line (segment)
        static double point_segment_distance(const Vec& p, const Vec& va, const Vec& vb, Vec& contact_point);

    //advanced formula for resolving collisions with rotation and friction
    static void resolve_collisions(Collisions& contact);
};