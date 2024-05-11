#include "../header/header.h"


Shapes sh;
Shapes bgsh;
std::vector<Collisions> Collisions::contacts;


namespace cas {
    double fixed_delta_time = 0;    // fixed_delta_time = 1/refresh rate of the monitor
    double time_start = 0;
    double time_end = 0;
    double sleep_time = 0;          
}

namespace physics {

    int iterations = 50;
    bool gravity = false;
    const double RESTITUTION = 0.5;
    const double STATIC_FRICTION = 0.7;
    const double DYNAMIC_FRICTION = 0.55;


    void step(const double& delta, const int& iterations){

        double delta_time = delta/static_cast<double>(iterations);

        for (int it = 0; it<iterations; ++it){

            if(gravity) sh.update_by_acceleration(delta_time, Vec{0.0, -0.981}); //gravity (to vyslo hezky)
            sh.update_by_force(delta_time);
            sh.update_position_and_rotation(delta_time);
            sh.handle_collisions();

        }
    }
}


int main(int argc, char **argv) {

    glutInit(&argc, argv);

//Initialize GLFW window, error and stuff
    //initialize GLFW
    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW" << std::endl; return -1; }
    //set error callback
    glfwSetErrorCallback(cbs::errorCallback);
    //create window (dont worry, will be deleted at the end of main function)
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Physics engine v0.42", NULL, NULL);
    if (!window) { std::cerr << "Failed to create GLFW window" << std::endl; glfwTerminate(); return -1; }
    //create GLFW context
    glfwMakeContextCurrent(window);
    //set window buffers swap interval - vsync (aspon doufam ze to tak funguje)
    glfwSwapInterval(1);

    //get FPS of the monitor the engine is running on
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor) {
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        if (mode) cas::fixed_delta_time = (1.0/static_cast<double>(mode->refreshRate));
    }

    //set key callbacks
    glfwSetMouseButtonCallback(window, cbs::mouse_button_callback);
    glfwSetScrollCallback(window, cbs::scroll_callback);
    glfwSetKeyCallback(window, cbs::key_callback);

    //add ground
    sh.add_shape(Rectangle{Vec{0.0, GROUND}, Vec{1.0*ST, -GROUND}});  
    for (auto& shape: sh.shapes) Shapes::toggle_static(shape);



    


//------------------------------------------------------------------------------------------------------------------------
//Main loop
    while (!glfwWindowShouldClose(window)) {
        cas::time_start = glfwGetTime();

        //random function for computing number of iterations based on how many shapes are present on the screen
        int s = sh.shapes.size();
        if (s >= 1) {
            if (s <= 140) physics::iterations = std::floor(-20*std::log(s)+100+1.0/s);
            else physics::iterations = 1;
        }

        //physics calculations
        physics::step(cas::fixed_delta_time, physics::iterations);


        window::draw_background();
        
        sh.draw_all_shapes();
        sh.delete_out_of_screen();

          
        //swap front and back buffers - when the back one renders with the code above, this function will smoothly swap them
        glfwSwapBuffers(window);
        //process keyboard, mouse, etc events
        glfwPollEvents();

        cas::time_end = glfwGetTime();
        cas::sleep_time = cas::fixed_delta_time - (cas::time_end-cas::time_start);
        if (cas::sleep_time > 0.0) std::this_thread::sleep_for(std::chrono::duration<double>(cas::sleep_time));
    }
//End of main loop
//------------------------------------------------------------------------------------------------------------------------

//End GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

