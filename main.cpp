#include "header.h"

int main() {

//Initialize GLFW window, error and stuff

    //initialize GLFW
    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW" << std::endl; return -1; }
    //set error callback
    glfwSetErrorCallback(errorCallback);
    //create window (dont worry, will be deleted at the end of main function)
    GLFWwindow* window = glfwCreateWindow(1000, 500, "Super stacker 3", NULL, NULL);
    if (!window) { std::cerr << "Failed to create GLFW window" << std::endl; glfwTerminate(); return -1; }
    //create GLFW context
    glfwMakeContextCurrent(window);
    //set window buffers swap interval - vsync (aspon doufam ze to tak funguje)
    glfwSwapInterval(1);


    

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, key_callback);

    
//------------------------------------------------------------------------------------------------------------------------
//Main loop
    while (!glfwWindowShouldClose(window)) {
        

        draw_all_shapes();

        //swap front and back buffers - when the back one renders with the code above, this function will smoothly swap then.
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }
//End of main loop
//------------------------------------------------------------------------------------------------------------------------

//End GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

