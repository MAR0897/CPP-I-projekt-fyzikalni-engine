#include "../header/header.h"


//Keys
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    
        if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (key == GLFW_KEY_G and action == GLFW_PRESS){
            gravity = !gravity; 
        }

        if (key == GLFW_KEY_R and action == GLFW_PRESS){
            sh.add_shape((Rectangle{get_mouse_coords(window), Vec{0.2, 0.2}}));   
        }

        if (key == GLFW_KEY_T and action == GLFW_PRESS){
            sh.add_shape((Triangle{get_mouse_coords(window), 0.15}));    
        }

        if (key == GLFW_KEY_C and action == GLFW_PRESS){
            sh.add_shape((Circle{get_mouse_coords(window), 0.1}));    
        }


        if (key == GLFW_KEY_DELETE and action == GLFW_PRESS){
            sh.delete_shape(get_mouse_coords(window));
        }

        if (key == GLFW_KEY_A and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::rotate_shape(shape, 0.05*PI);   
        }
        if (key == GLFW_KEY_D and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::rotate_shape(shape, -0.05*PI);  
        }

        if (key == GLFW_KEY_UP and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::move_shape(shape, Vec{0.0, 0.02});   
        }
        if (key == GLFW_KEY_DOWN and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::move_shape(shape, Vec{0.0, -0.02});  
        }
        if (key == GLFW_KEY_LEFT and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::move_shape(shape, Vec{-0.02, 0.0});   
        }
        if (key == GLFW_KEY_RIGHT and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::move_shape(shape, Vec{0.02, 0.0});  
        }

        if (key == GLFW_KEY_S and action == GLFW_PRESS){
            for (auto& shape : sh.shapes) if (Shapes::is_inside(get_mouse_coords(window), shape)) Shapes::toggle_static(shape);   
        }
    
}

//Mouse keys
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        
        //for (auto& shape : sh.shapes) if (sh.is_inside(get_mouse_coords(window), shape)) sh.move_shape_to(shape, get_mouse_coords(window));    
        for (auto& shape : sh.shapes) if (Shapes::is_inside(get_mouse_coords(window), shape)) Shapes::toggle_selected(shape);
        
        
    }
}

//Scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::resize_shape(shape, yoffset/100.0);    
        
}


Vec get_mouse_coords(GLFWwindow* window){
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    double Xpos = (mouseX / 640.0) - 1.0;
    double Ypos = 1.0 - (mouseY / 360.0);
    return Vec{Xpos, Ypos/ST};
}