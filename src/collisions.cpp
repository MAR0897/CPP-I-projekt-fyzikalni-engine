#include "../header/header.h"

//functions and special structure for handling collisions

void Shapes::handle_collisions(){

    for (auto it = shapes.begin(); it!=shapes.end(); ++it){                                     //loop through the shapes vector
        for (auto it2 = std::next(it); it2!=shapes.end(); ++it2){                                   //get a pair of shapes through second loop
            double depth = 0.0;                                                                         //initialize zero value depth
            Vec normal{0.0, 0.0};                                                                       //initialize zero value normal

            if (Shapes::is_static(*it) and Shapes::is_static(*it2)) continue;                           //if both shapes are static, skip them

            if (!Shapes::intersect_aabbXaabb(Shapes::get_AABB(*it), Shapes::get_AABB(*it2))) continue;  //if shapes' AABBs dont intersect, skip them

            if (Shapes::intersect(*it, *it2, depth, normal)){                                           //if the shapes do intersect:

                if(Shapes::is_static(*it)){                                                                 //if first shape is static:
                    Shapes::move_shape(*it2, -normal * depth);                                                  //move only the second by a full depth, to the normal direction
                }   
                else if(Shapes::is_static(*it2)){                                                           //if second shape is static:
                    Shapes::move_shape(*it, normal * depth);                                                    //move only the first by full depth, to the normal direction
                }           
                else {                                                                                      //if none of the shapes are static:
                    Shapes::move_shape(*it, normal * depth/2.0);                                                //move both shapes by half depth, to the normal direction
                    Shapes::move_shape(*it2, -normal * depth/2.0);                                              //normaly musi byt minusovane takhle!!!
                }
                
                Vec c_point1 = {0.0, 0.0};                                                                  //initialize contact points (max 2 are used)
                Vec c_point2 = {0.0, 0.0};
                int c_points_count = 0;

                Collisions::find_contact_points(*it, *it2, c_point1, c_point2, c_points_count);             //find the contact points and store them to resolve them later

                Collisions::contacts.emplace_back(Collisions{*it, *it2, depth, normal, c_point1, c_point2, c_points_count});
            }
        }
    }

    for (auto& contact : Collisions::contacts) Shapes::resolve_collisions(contact);            //resolve colisions based on calculated contact points
    Collisions::contacts.clear();                                                               //delete all contact points for this one physics simulation iteration

}

void Shapes::resolve_collisions(Collisions& c){

    Vec& vel1 = get_velocity(c.sh1);
    Vec& vel2 = get_velocity(c.sh2);
    Vec pos1 = get_position(c.sh1);
    Vec pos2 = get_position(c.sh2);
    double inv_m1 = get_inverse_mass(c.sh1);
    double inv_m2 = get_inverse_mass(c.sh2);
    double inv_ri1 = get_inverse_rot_inertia(c.sh1);
    double inv_ri2 = get_inverse_rot_inertia(c.sh2);
    double re1 = get_restitution(c.sh1);
    double re2 = get_restitution(c.sh2);
    double& rv1 = get_rot_velocity(c.sh1);
    double& rv2 = get_rot_velocity(c.sh2);

    double e = std::min(re1, re2);

    std::vector<Vec> c_points = {c.contact1, c.contact2};
    std::vector<Vec> impulses(2);
    std::vector<Vec> dis1(2);
    std::vector<Vec> dis2(2);

    for (int i = 0; i<c.contact_count; i++){

        Vec distance1 = c_points[i] - pos1;
        Vec distance2 = c_points[i] - pos2;

        dis1[i] = distance1;
        dis2[i] = distance2;

        Vec r1 = distance1.to_normal();
        Vec r2 = distance2.to_normal();

        Vec angular_vel1 = r1 * rv1;
        Vec angular_vel2 = r2 * rv2;

        Vec relative_vel = (vel2 + angular_vel2) - (vel1 + angular_vel1);

        double rel_vel_mag = relative_vel.dot(c.normal);

        if(rel_vel_mag < 0.0) return;
        
        double r1dn = r1.dot(c.normal);
        double r2dn = r2.dot(c.normal);

        double j = (-(1.0 + e) * rel_vel_mag) / (inv_m1 + inv_m2 + r1dn*r1dn*inv_ri1 + r2dn*r2dn*inv_ri2);
        j /= static_cast<double>(c.contact_count);

        Vec impulse = c.normal * j;
        impulses[i] = impulse;
    }

    for (int i = 0; i<c.contact_count; i++){

        vel1 += -impulses[i] * inv_m1;
        vel2 += impulses[i] * inv_m2;
        rv1 += -dis1[i].cross(impulses[i]) * inv_ri1;
        rv2 += dis2[i].cross(impulses[i]) * inv_ri2;
    }
}



//------------------------------------------------------------------------------------------------------------------------------------

void Collisions::find_contact_points(const Shape& sh1, const Shape& sh2, Vec& c_point1, Vec& c_point2, int& c_points_count){
    std::visit([&](auto& s1, auto& s2) { m_find_contact_points(s1, s2, c_point1, c_point2, c_points_count); }, sh1, sh2);
}

template<typename S1, typename S2>
void Collisions::m_find_contact_points(const S1& s1, const S2& s2, Vec& c_point1, Vec& c_point2, int& c_points_count) {

    //if both shapes are circles
    if constexpr (std::is_same_v<S1, Circle> and std::is_same_v<S2, Circle>) fcp_circXcirc(s1, s2, c_point1, c_points_count); 
    //if none of the shapes are circles
    else if constexpr ((std::is_same_v<S1, Triangle> or std::is_same_v<S1, Rectangle>) and (std::is_same_v<S2, Triangle> or std::is_same_v<S2, Rectangle>))
        fcp_polyXpoly(s1.vertices, s2.vertices, c_point1, c_point2, c_points_count); 
    //if exactly one shape is circle
    else {
        if constexpr (std::is_same_v<S1, Circle>) fcp_polyXcirc(s1, s2.vertices, c_point1, c_points_count);
        else if constexpr (std::is_same_v<S2, Circle>)  fcp_polyXcirc(s2, s1.vertices, c_point1, c_points_count); 
    }
}

void Collisions::fcp_circXcirc(const Circle& c1, const Circle& c2, Vec& c_point1, int& c_points_count){

    //if the two circles intersect, they have a single contact point, which can be found easily like this:

    Vec direction = (c2.pos-c1.pos).to_unit();
    c_point1 = c1.pos + direction*c1.rad;
    c_points_count = 1;
}

void Collisions::fcp_polyXcirc(const Circle& c, const std::vector<Vec>& verts, Vec& c_point1, int& c_points_count){

    //if a circle and a polygon intersect, we have to find the minimum distance between the circle center and the polygon edges 
    //    and the point representing the minimum distance will be our contact point

    double min_distance = std::numeric_limits<double>::infinity();
    Vec contact;

    for (auto it = verts.begin(); it!=verts.end(); ++it){

        Vec va{*it};                                               
        Vec vb;                                                     
        if(std::next(it) != verts.end()) vb = *std::next(it);     
        else vb = *verts.begin();

        double distance = Collisions::point_segment_distance(c.pos, va, vb, contact);

        if (distance<min_distance) {
            min_distance = distance;
            c_point1 = contact;
        }
    }

    c_points_count = 1;
}

void Collisions::fcp_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, Vec& c_point1, Vec& c_point2, int& c_points_count){

    //for polygon and polygon contact point we have to iterate over both polygon and the algorithm is pretty much the same as in polyXcirc

    double min_distance = std::numeric_limits<double>::infinity();
    Vec contact;
    
    m_fcp_polyXpoly(verts1, verts2, c_point1, c_point2, c_points_count, contact, min_distance);
    m_fcp_polyXpoly(verts2, verts1, c_point1, c_point2, c_points_count, contact, min_distance);
}

void Collisions::m_fcp_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, Vec& c_point1, Vec& c_point2, int& c_points_count, Vec& contact, double& min_distance) {

    for (auto it = verts1.begin(); it!=verts1.end(); ++it){
        for (auto it2 = verts2.begin(); it2!=verts2.end(); ++it2){

            Vec va{*it2};                                               
            Vec vb;                                                     
            if(std::next(it2) != verts2.end()) vb = *std::next(it2);     
            else vb = *verts2.begin();

            double distance = Collisions::point_segment_distance(*it, va, vb, contact);

            if (Vec::nearly_equal(distance, min_distance)) {
                if (!contact.nearly_equal(c_point1)){
                    c_point2 = contact;
                    c_points_count = 2;
                }
            }
            else if (distance<min_distance) {
                min_distance = distance;
                c_point1 = contact;
                c_points_count = 1;
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------------------
//helper function for finding contact points

double Collisions::point_segment_distance(const Vec& p, const Vec& va, const Vec& vb, Vec& contact_point){

    Vec ab = vb-va;
    Vec ap = p-va;
    double projection = ab.dot(ap);
    double length2 = ab.norm2();
    double d = projection/length2;

    if (d <= 0.0) contact_point = va;
    else if (d >= 1.0) contact_point = vb;
    else contact_point = va + ab*d;

    return p.distance2(contact_point);
}


//================================================================================================================
//Collision struct constrcutors

Collisions::Collisions(Shape& sh1, Shape& sh2, const double& depth, const Vec& normal, const Vec& contact1, const Vec& contact2, const int& contact_count) :
    sh1(sh1), sh2(sh2), depth(depth), normal(normal), contact_count(contact_count)
    {
        if (contact_count == 1) {
            this->contact1 = contact1;
        }
        else {
            this->contact1 = contact1;
            this->contact2 = contact2;
        }
    }
Collisions::Collisions(Shape& sh1, Shape& sh2, double&& depth, Vec&& normal, Vec&& contact1, Vec&& contact2, int&& contact_count) :
    sh1(sh1), sh2(sh2), depth(std::move(depth)), normal(std::move(normal)), contact_count(std::move(contact_count))
    {
        if (contact_count == 1) {
            this->contact1 = std::move(contact1);
        }
        else {
            this->contact1 = std::move(contact1);
            this->contact2 = std::move(contact2);
        }
    }