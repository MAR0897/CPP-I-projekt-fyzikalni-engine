#include "../header/header.h"

//things for physics simulation and some constants

namespace physics {

    int iterations = 50;
    bool gravity = false;
    const double RESTITUTION = 0.5;
    const double STATIC_FRICTION = 0.6;
    const double DYNAMIC_FRICTION = 0.45;
    
    void step(const double& delta, const int& iterations){

        double delta_time = delta/static_cast<double>(iterations);

        for (int it = 0; it<iterations; ++it){

            if(gravity) sh.update_by_acceleration(delta_time, Vec{0.0, -0.981}); //gravity (to vyslo hezky)
            sh.update_by_force(delta_time);
            sh.update_position_and_rotation(delta_time);
            sh.handle_collisions();
        }
    }

    void cap_iterations(){

        int s = physics::sh.shapes.size();

        if (s >= 1) {
            //random function for computing number of iterations based on how many shapes are present on the screen
            if (s <= 140) physics::iterations = std::floor(-20*std::log(s)+100);
            else physics::iterations = 1;
        }
    }
}
