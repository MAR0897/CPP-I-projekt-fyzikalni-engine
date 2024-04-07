#include "../header/header.h"

//functions for checking if two shapes intersect

bool Shapes::intersect(const Shape& sh1, const Shape& sh2, double& depth, Vec& normal){

    return std::visit(overloaded {
        [&](const Rectangle& r1, const Rectangle& r2) -> bool { return Shapes::intersect_polyXpoly(r1.vertices, r2.vertices, r1.pos, r2.pos, depth, normal); },
        [&](const Rectangle& r, const Triangle& t) -> bool { return Shapes::intersect_polyXpoly(r.vertices, t.vertices, r.pos, t.pos, depth, normal); },
        [&](const Rectangle& r, const Circle& c) -> bool { return Shapes::intersect_polyXcirc(r.vertices, c, r.pos, depth, normal); },
        [&](const Triangle& t, const Rectangle& r) -> bool { return Shapes::intersect_polyXpoly(t.vertices, r.vertices, t.pos, r.pos, depth, normal); },
        [&](const Triangle& t1, const Triangle& t2) -> bool { return Shapes::intersect_polyXpoly(t1.vertices, t2.vertices, t1.pos, t2.pos, depth, normal); },
        [&](const Triangle& t, const Circle& c) -> bool { return Shapes::intersect_polyXcirc(t.vertices, c, t.pos, depth, normal); },
        [&](const Circle& c, const Rectangle& r) -> bool { bool res = Shapes::intersect_polyXcirc(r.vertices, c, r.pos, depth, normal); normal = -normal; return res; }, //divne ja vim, ale bez toho to nefahci
        [&](const Circle& c, const Triangle& t) -> bool { bool res = Shapes::intersect_polyXcirc(t.vertices, c, t.pos, depth, normal); normal = -normal; return res; }, //divne ja vim, ale bez toho to nefahci
        [&](const Circle& c1, const Circle& c2) -> bool { return Shapes::intersect_circXcirc(c1, c2, depth, normal); }
    }, sh1, sh2);

}

bool Shapes::intersect_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, const Vec& center1, const Vec& center2, double& depth, Vec& normal){

    depth = std::numeric_limits<double>::infinity();
    double max1, max2, min1, min2;

    for (auto it = verts1.begin(); it!=verts1.end(); ++it){     //iterate over edges of the first polygon

        Vec va{*it};                                                //take a vertex
        Vec vb;                                                     
        if(std::next(it) != verts1.end()) vb = *std::next(it);      //and take its neightbor to get the edge
        else vb = *verts1.begin();
        
        Vec axis = (vb-va).perpendiculate().normalize();            //get the axis we are going to test

        Shapes::project_vertices(verts1, axis, max1, min1);
        Shapes::project_vertices(verts2, axis, max2, min2);

        if ((min1 >= max2) or (min2 >= max1)) return false;         //if both mins were smaller than maxs, the polygons would intersect
        
        double axis_depth = std::min(max2-min1, max1-min2);         //calculate how much they are intersecting

        if (axis_depth<depth){
            depth = axis_depth;
            normal = axis;
        } 
    }

    for (auto it = verts2.begin(); it!=verts2.end(); ++it){     //iterate over edges of the second polygon

        Vec va{*it};                                                //take a vertex
        Vec vb;                                                     
        if(std::next(it) != verts2.end()) vb = *std::next(it);      //and take its neightbor to get the edge
        else vb = *verts2.begin();
        
        Vec axis = (vb-va).perpendiculate().normalize();            //get the axis we are going to test

        Shapes::project_vertices(verts1, axis, max1, min1);
        Shapes::project_vertices(verts2, axis, max2, min2);

        if ((min1 >= max2) or (min2 >= max1)) return false;         //if both mins were smaller than maxs, the polygons would intersect

        double axis_depth = std::min(max2-min1, max1-min2);         //calculate how much they are intersecting

        if (axis_depth<depth){
            depth = axis_depth;
            normal = axis;
        }
    }

    if (normal.dot(center1-center2) < 0.0) normal = -normal;    //check the correct orientation of the normal
    return true;
}


bool Shapes::intersect_polyXcirc(const std::vector<Vec>& verts1, const Circle& c, const Vec& polyg_center, double& depth, Vec& normal){

    depth = std::numeric_limits<double>::infinity();
    double max1, max2, min1, min2;

    for (auto it = verts1.begin(); it!=verts1.end(); ++it){     //iterate over edges of the second polygon

        Vec va{*it};                                                //take a vertex
        Vec vb;                                                     
        if(std::next(it) != verts1.end()) vb = *std::next(it);      //and take its neightbor to get the edge
        else vb = *verts1.begin();

        Vec axis = (vb-va).perpendiculate().normalize();            //get the axis we are going to test

        Shapes::project_vertices(verts1, axis, max1, min1);
        Shapes::project_circle(c, axis, max2, min2);
        
        if ((min1 >= max2) or (min2 >= max1)) return false;         //if both mins were smaller than maxs, the polygons would intersect
        
        double axis_depth = std::min(max2-min1, max1-min2);         //calculate how much they are intersecting

        if (axis_depth<depth){
            depth = axis_depth;
            normal = axis;
        }
    }

    double closest_vertex_distance = std::numeric_limits<double>::infinity();
    auto index = verts1.begin();
    
    for (auto it = verts1.begin(); it!=verts1.end(); ++it){
        double distance = c.pos.distance(*it);
        if (distance < closest_vertex_distance) {
            closest_vertex_distance = distance;
            index = it;
        }
    }

    Vec axis = (*index-c.pos).normalize();                 

    Shapes::project_vertices(verts1, axis, max1, min1);
    Shapes::project_circle(c, axis, max2, min2);
    
    if ((min1 >= max2) or (min2 >= max1)) return false;        

    double axis_depth = std::min(max2-min1, max1-min2);     

    if (axis_depth<depth){
        depth = axis_depth;
        normal = axis;
    }
    if (normal.dot(polyg_center-c.pos) < 0.0) normal = -normal;    //check the correct orientation of the normal

    return true;

}
bool Shapes::intersect_circXcirc(const Circle& c1, const Circle& c2, double& depth, Vec& normal){
    double centers_distance = c1.pos.distance(c2.pos);
    double radii_sum = c1.rad + c2.rad;

    if(centers_distance<radii_sum) {
        depth = radii_sum - centers_distance;
        normal = (c1.pos-c2.pos).normalize();
        return true;
    }
    return false;
}

bool Shapes::intersect_aabbXaabb(const AABB& box1, const AABB& box2){

    if ((box1.max.x <= box2.min.x) or (box2.max.x <= box1.min.x) or
        (box1.max.y <= box2.min.y) or (box2.max.y <= box1.min.y)) return false;

    return true;
}





//--------------------------------------------------------------------------------------------------------------------------

void Shapes::project_vertices(const std::vector<Vec>& vertices, const Vec& axis, double& max, double& min){

    max = -std::numeric_limits<double>::infinity(); //std::numeric_limits<double>::min(); VRACI NULU!!!!!!
    min = std::numeric_limits<double>::infinity();

    for (const auto& vertex : vertices){
        double projected = vertex.dot(axis);
        if (projected > max) max = projected;
        if (projected < min) min = projected;
    }
}

void Shapes::project_circle(const Circle& c, const Vec& axis, double& max, double& min){

    Vec circle_vector = axis*c.rad;
    Vec p1 = c.pos + circle_vector;
    Vec p2 = c.pos - circle_vector;

    min = p1.dot(axis);
    max = p2.dot(axis);
    if(min>max) std::swap(min, max);
}