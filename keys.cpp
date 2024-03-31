#include "header.h"


//ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    switch(key){
        case GLFW_KEY_ESCAPE : if (action == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }


        case GLFW_KEY_S: if (action == GLFW_PRESS){
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            double glXPos = (xPos / 400.0) - 1.0; // Assuming window width is 800
            double glYPos = 1.0 - (yPos / 300.0);

            shapes.emplace_back(Rectangle{Vec{glXPos, glYPos}, Vec{0.5, 0.5}});    
        }
    }
    
}

//Left mouse key
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Get mouse position
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        // Convert mouse coordinates to OpenGL coordinates
        mouseY = 600.0f - mouseY;
        // Check if the click is inside the button bounds
        
    }
}