#include "../header/header.h"

//Time measurement for syncing the simulation with the real world
namespace cas {

    double fixed_delta_time = 0;    // fixed_delta_time = 1/refresh rate of the monitor
    double time_start = 0;
    double time_end = 0;
    double sleep_time = 0;  

    void sleep_thread() {
        sleep_time = fixed_delta_time - (time_end-time_start);
        if (sleep_time > 0.0) std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
    }       
}


namespace physics {

    Shapes sh;
}

//================================================================================================================================

int main(int argc, char **argv) {

    //Initialization

    //initialize glut for text rendering
    glutInit(&argc, argv);

    //initialize GLFW
    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW" << std::endl; return -1; }

    //set error callback
    glfwSetErrorCallback(cbs::errorCallback);

    //create window
    GLFWwindow* window = glfwCreateWindow(window::WIDTH, window::HEIGHT, "Physics engine v0.42", NULL, NULL);
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

    //add ground and possibly some other static shapes
    physics::sh.add_shape(Rectangle{Vec{0.0, window::GROUND}, Vec{1.0*window::ST, -window::GROUND}});  
    for (auto& shape: physics::sh.shapes) Shapes::toggle_static(shape);


//------------------------------------------------------------------------------------------------------------------------
//Main loop
    while (!glfwWindowShouldClose(window)) {

        //Start timer
        cas::time_start = glfwGetTime();

        //random function for computing number of iterations based on how many shapes are present on the screen
        physics::cap_iterations();

        //physics calculations
        physics::step(cas::fixed_delta_time, physics::iterations);

        //draw background and render text
        window::draw_background();
        
        //draw shapes on the screen and delete the out-of-screen ones
        physics::sh.draw_all_shapes();
        physics::sh.delete_out_of_screen();

        //swap front and back buffers - when the back one renders with the code above, this function will smoothly swap them
        glfwSwapBuffers(window);
        
        //process keyboard, mouse, etc events
        glfwPollEvents();

        //End the timer and sleep the main thread for some time to wait for the fixed delta time to pass by
        cas::time_end = glfwGetTime();
        cas::sleep_thread();
    }
//End of main loop
//------------------------------------------------------------------------------------------------------------------------

    //Terminate
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
