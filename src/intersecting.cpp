#include "../header/header.h"

//functions for checking if two shapes intersect
bool Shapes::intersect(const Shape& sh1, const Shape& sh2, double& depth, Vec& normal){
    return std::visit([&](auto& s1, auto& s2) -> bool { return m_intersect(s1, s2, depth, normal); }, sh1, sh2);
}

//nesmis menit const a mutable v argumentu funkce, jinak to std::is_same_v nezkousne!!!!!!!
template<typename S1, typename S2>
bool Shapes::m_intersect(const S1& s1, const S2& s2, double& depth, Vec& normal) {

    //if both shapes are circles
    if constexpr (std::is_same_v<S1, Circle> and std::is_same_v<S2, Circle>) return intersect_circXcirc(s1, s2, depth, normal); 
    //if none of the shapes are circles
    else if constexpr ((std::is_same_v<S1, Triangle> or std::is_same_v<S1, Rectangle>) and (std::is_same_v<S2, Triangle> or std::is_same_v<S2, Rectangle>))
        return intersect_polyXpoly(s1.vertices, s2.vertices, s1.pos, s2.pos, depth, normal); 
    //if exactly one shape is circle
    else {
        if constexpr (std::is_same_v<S1, Circle>) { bool res = intersect_polyXcirc(s2.vertices, s1, s2.pos, depth, normal); normal = -normal; return res; }
        else if constexpr (std::is_same_v<S2, Circle>)  return intersect_polyXcirc(s1.vertices, s2, s1.pos, depth, normal); 
    }
    return false;
}

bool Shapes::m_intersect_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, double& depth, Vec& normal, double& max1, double& max2, double& min1, double& min2) {

    for (auto it = verts1.begin(); it!=verts1.end(); ++it){     //iterate over edges of the first polygon

        Vec va{*it};                                                    //take a vertex
        Vec vb;                                                         //make place for second vertex
        if(std::next(it) != verts1.end()) vb = *std::next(it);          //and take its neightbor to get the edge
        else vb = *verts1.begin();                                          //else take the first vertex
        Vec axis = (vb-va).to_normal().to_unit();                       //get the axis we are going to test
        Shapes::project_vertices(verts1, axis, max1, min1);             //project vertices of the first polygon on the separating axis
        Shapes::project_vertices(verts2, axis, max2, min2);             //project vertices of the second polygon on the separating axis
        if ((min1 >= max2) or (min2 >= max1)) return false;             //if both mins were smaller than maxs, the polygons would intersect
        double axis_depth = std::min(max2-min1, max1-min2);             //calculate how much they are intersecting
        //might not be as readable, but its nicely stored on one line, instead of 4 lines, so it fits better :)
        if (axis_depth<depth) { depth = axis_depth; normal = axis; }    //if smaller value for depth found, update our depth and assign the currect axis to the normal
    }
    return true;
}


bool Shapes::intersect_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, const Vec& center1, const Vec& center2, double& depth, Vec& normal){

    depth = std::numeric_limits<double>::infinity();
    double max1, max2, min1, min2;

    if (!m_intersect_polyXpoly(verts1, verts2, depth, normal, max1, max2, min1, min2)) return false;
    if (!m_intersect_polyXpoly(verts2, verts1, depth, normal, max1, max2, min1, min2)) return false;

    if (normal.dot(center1-center2) < 0.0) normal = -normal;    //check the correct orientation of the normal
    return true;
}


bool Shapes::intersect_polyXcirc(const std::vector<Vec>& verts1, const Circle& c, const Vec& polyg_center, double& depth, Vec& normal){

    depth = std::numeric_limits<double>::infinity();
    double max1, max2, min1, min2;

    for (auto it = verts1.begin(); it!=verts1.end(); ++it){         //iterate over edges of the second polygon

        Vec va{*it};                                                //take a vertex
        Vec vb;                                                     //make a place for second vertex
        if(std::next(it) != verts1.end()) vb = *std::next(it);      //and take its neightbor to get the edge
        else vb = *verts1.begin();                                      //else take the first vertex
        Vec axis = (vb-va).to_normal().to_unit();                   //get the axis we are going to test
        Shapes::project_vertices(verts1, axis, max1, min1);         //project vertices of the polygon on the axis
        Shapes::project_circle(c, axis, max2, min2);                //project the circle on the axis
        if ((min1 >= max2) or (min2 >= max1)) return false;         //if both mins were smaller than maxs, the polygons would intersect
        double axis_depth = std::min(max2-min1, max1-min2);         //calculate how much they are intersecting
        if (axis_depth<depth){ depth = axis_depth; normal = axis; } //if smaller value for depth found, update our depth and assign the currect axis to the normal
    }

    double closest_vertex_distance = std::numeric_limits<double>::infinity();
    auto index = verts1.begin();
    
    //find the closest polygon vertex to the circle center
    for (auto it = verts1.begin(); it!=verts1.end(); ++it){
        double distance = c.pos.distance(*it);
        if (distance < closest_vertex_distance) {
            closest_vertex_distance = distance;
            index = it;
        }
    }

    //and create a axis from it and do the same again as described above
    Vec axis = (*index-c.pos).to_unit();                 
    Shapes::project_vertices(verts1, axis, max1, min1);
    Shapes::project_circle(c, axis, max2, min2);
    if ((min1 >= max2) or (min2 >= max1)) return false;        
    double axis_depth = std::min(max2-min1, max1-min2);     
    if (axis_depth<depth){ depth = axis_depth; normal = axis; }
    if (normal.dot(polyg_center-c.pos) < 0.0) normal = -normal;    //check the correct orientation of the normal

    return true;

}
bool Shapes::intersect_circXcirc(const Circle& c1, const Circle& c2, double& depth, Vec& normal){
    double centers_distance = c1.pos.distance(c2.pos);
    double radii_sum = c1.rad + c2.rad;

    if(centers_distance<radii_sum) {
        depth = radii_sum - centers_distance;
        normal = (c1.pos-c2.pos).to_unit();
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
//helper functions for projecting vertices on the axis and getting the maxs and mins 

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