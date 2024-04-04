#include "header.h"


Shapes sh;
Shapes bgsh;

namespace cas{
    double fixed_delta_time = 0;    // fixed_delta_time = 1/refresh rate of the monitor
    double time_start = 0;
    double time_end = 0;
    double sleep_time = 0;          
}


int main() {

//Initialize GLFW window, error and stuff

    //initialize GLFW
    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW" << std::endl; return -1; }
    //set error callback
    glfwSetErrorCallback(errorCallback);
    //create window (dont worry, will be deleted at the end of main function)
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Super stacker 3", NULL, NULL);
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    //add ground
    bgsh.add_shape(Rectangle{Vec{0.0, GROUND}, Vec{1.0*ST, -GROUND}});  
    for (auto& shape: bgsh.shapes) Shapes::toggle_static(shape);



    


//------------------------------------------------------------------------------------------------------------------------
//Main loop

    while (!glfwWindowShouldClose(window)) {

        cas::time_start = glfwGetTime();


        //sh.update_by_acceleration(cas::fixed_delta_time, Vec{0.0, -0.981}); //gravity
        sh.update_position(cas::fixed_delta_time);
        sh.handle_collisions();

        draw_background();
        sh.draw_all_shapes();






        //swap front and back buffers - when the back one renders with the code above, this function will smoothly swap then.
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();

        cas::time_end = glfwGetTime();
        cas::sleep_time = cas::fixed_delta_time - (cas::time_end-cas::time_start);
        std::this_thread::sleep_for(std::chrono::duration<double>(cas::sleep_time));
    }
//End of main loop
//------------------------------------------------------------------------------------------------------------------------

//End GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

