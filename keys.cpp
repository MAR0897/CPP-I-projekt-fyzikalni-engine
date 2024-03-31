#include "header.h"


//ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    
        if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (key == GLFW_KEY_R and action == GLFW_PRESS){
            sh.add_shape((Rectangle{get_mouse_coords(window), Vec{0.5, 0.5}}));    
        }

        if (key == GLFW_KEY_T and action == GLFW_PRESS){
            Vec a = get_mouse_coords(window);
            sh.add_shape((Triangle{a, a+0.2, Vec{a.x+0.2, a.y}}));    
        }

        if (key == GLFW_KEY_C and action == GLFW_PRESS){
            sh.add_shape((Circle{get_mouse_coords(window), 0.2}));    
        }

        if (key == GLFW_KEY_DELETE and action == GLFW_PRESS){
            sh.delete_shape(get_mouse_coords(window));
        }
    
    
}

//Left mouse key
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Get mouse position
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        // Check if the click is inside the button bounds
        std::cout<<mouseX<<" "<<mouseY<<std::endl;
        
    }
}

//Scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    for (auto& shape : sh.shapes){
        if (sh.is_inside(get_mouse_coords(window), shape)) sh.resize_shape(shape, yoffset/100.0);
    }
}


Vec get_mouse_coords(GLFWwindow* window){
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    double Xpos = (mouseX / 640.0) - 1.0;
    double Ypos = 1.0 - (mouseY / 360.0);
    return Vec{Xpos, Ypos};
}