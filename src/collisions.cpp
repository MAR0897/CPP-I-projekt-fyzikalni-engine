#include "../header/header.h"

//functions and special structure for handling collisions

void Shapes::handle_collisions(){

    for (auto it = shapes.begin(); it!=shapes.end(); ++it){
        for (auto it2 = std::next(it); it2!=shapes.end(); ++it2){
            double depth = 0.0;
            Vec normal{0.0, 0.0};

            if(Shapes::is_static(*it) and Shapes::is_static(*it2)) continue;

            if (Shapes::intersect(*it, *it2, depth, normal)){

                if(Shapes::is_static(*it)){
                    Shapes::move_shape(*it2, -normal * depth);
                }
                else if(Shapes::is_static(*it2)){
                    Shapes::move_shape(*it, normal * depth);
                }
                else {
                    Shapes::move_shape(*it, normal * depth/2.0);
                    Shapes::move_shape(*it2, -normal * depth/2.0);  //normaly musi byt minusovane takhle!!!
                }
                
                
                Collisions::contacts.emplace_back(Collisions{*it, *it2, depth, normal, Vec{0.0, 0.0}, Vec{0.0, 0.0}, 1});
            }
        }
    }

    for (auto& contact : Collisions::contacts){
        Shapes::resolve_collisions(contact.sh1, contact.sh2, contact.depth, contact.normal);
    }
    Collisions::contacts.clear();

}

void Shapes::resolve_collisions(Shape& shape1, Shape& shape2, double& depth, Vec& normal) {

    Vec& vel1 = Shapes::get_velocity(shape1);
    Vec& vel2 = Shapes::get_velocity(shape2);
    double inv_m1 = Shapes::get_inverse_mass(shape1);
    double inv_m2 = Shapes::get_inverse_mass(shape2);
    double re1 = Shapes::get_restitution(shape1);
    double re2 = Shapes::get_restitution(shape2);

    Vec relative_vel = vel2 - vel1;

    if(relative_vel.dot(normal) < 0.0) return;
    
    double e = std::min(re1, re2);
    double j = (-(1.0 + e) * relative_vel.dot(normal)) / (inv_m1 + inv_m2);
    Vec impulse = normal * j;

    vel1 -= impulse * inv_m1;
    vel2 += impulse * inv_m2;
    
}


//================================================================================================================

Collisions::Collisions(Shape& sh1, Shape& sh2, const double& depth, const Vec& normal, const Vec& contact1, const Vec& contact2, const int& contact_count) :
    sh1(sh1), sh2(sh2), depth(depth), normal(normal)
    {
        if (contact_count == 1) {
            this->contact1 = contact1;
            this->contact2 = Vec{0.0, 0.0};
        }
        else {
            this->contact1 = contact1;
            this->contact1 = contact2;
        }
    }
Collisions::Collisions(Shape& sh1, Shape& sh2, double&& depth, Vec&& normal, Vec&& contact1, Vec&& contact2, int&& contact_count) :
    sh1(sh1), sh2(sh2), depth(std::move(depth)), normal(std::move(normal))
    {
        if (contact_count == 1) {
            this->contact1 = std::move(contact1);
            this->contact2 = Vec{0.0, 0.0};
        }
        else {
            this->contact1 = std::move(contact1);
            this->contact1 = std::move(contact2);
        }
    }