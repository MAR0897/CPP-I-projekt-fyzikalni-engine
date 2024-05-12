#include "../header/header.h"

//key, mouse and error callbacks

namespace cbs {

    //starting and ending point of swipe select 
    Vec swipe_start;
    Vec swipe_end;
    bool is_mouse_pressed = false;

    //Error callback
    void errorCallback(int error, const char* description){
        std::cerr << "Error: " << description << std::endl;
    }

    //Key callbacks
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

        //Close engine
        if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        //Enable gravity
        if (key == GLFW_KEY_G and action == GLFW_PRESS){
            physics::gravity = !physics::gravity; 
        }
    
        //Deselect all shapes
        if (key == GLFW_KEY_Q and action == GLFW_PRESS){
            physics::sh.deselect();
        }

        //Adding shapes
        if (key == GLFW_KEY_R and action == GLFW_PRESS){
            physics::sh.add_shape((Rectangle{window::get_mouse_coords(window), Vec{0.2, 0.2}}));   
        }
        if (key == GLFW_KEY_T and action == GLFW_PRESS){
            physics::sh.add_shape((Triangle{window::get_mouse_coords(window), 0.15}));    
        }
        if (key == GLFW_KEY_C and action == GLFW_PRESS){
            physics::sh.add_shape((Circle{window::get_mouse_coords(window), 0.1}));    
        }

        //Delete selected shapes
        if (key == GLFW_KEY_DELETE and action == GLFW_PRESS){
            physics::sh.delete_selected();
        }

        //Rotate shapes
        if (key == GLFW_KEY_A and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : physics::sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_spin(shape, 0.0001);   
        }
        if (key == GLFW_KEY_D and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : physics::sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_spin(shape, -0.0001);  
        }

        //Move shapes
        if (key == GLFW_KEY_UP and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : physics::sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_force(shape, Vec{0.0, 2.0});   
        }
        if (key == GLFW_KEY_DOWN and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : physics::sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_force(shape, Vec{0.0, -2.0});  
        }
        if (key == GLFW_KEY_LEFT and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : physics::sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_force(shape, Vec{-2.0, 0.0});   
        }
        if (key == GLFW_KEY_RIGHT and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : physics::sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_force(shape, Vec{2.0, 0.0});  
        }

        //Toggle static shape
        if (key == GLFW_KEY_S and action == GLFW_PRESS){
            for (auto& shape : physics::sh.shapes) if (Shapes::is_inside(window::get_mouse_coords(window), shape)) Shapes::toggle_static(shape);   
        }

        //Creating and saving savestates
        if (key == GLFW_KEY_1 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test1.json");
        if (key == GLFW_KEY_2 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test2.json");
        if (key == GLFW_KEY_3 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test3.json");
        if (key == GLFW_KEY_4 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test4.json");
        if (key == GLFW_KEY_5 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test5.json");
        if (key == GLFW_KEY_6 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test6.json");
        if (key == GLFW_KEY_7 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test7.json");
        if (key == GLFW_KEY_8 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test8.json");
        if (key == GLFW_KEY_9 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test9.json");
        if (key == GLFW_KEY_0 and action == GLFW_PRESS) if (mods & GLFW_MOD_CONTROL) physics::sh.load_to_json("json/test10.json");
    
        //Loading savestates
        if (key == GLFW_KEY_1 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test1.json");
        if (key == GLFW_KEY_2 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test2.json");
        if (key == GLFW_KEY_3 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test3.json");
        if (key == GLFW_KEY_4 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test4.json");
        if (key == GLFW_KEY_5 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test5.json");
        if (key == GLFW_KEY_6 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test6.json");
        if (key == GLFW_KEY_7 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test7.json");
        if (key == GLFW_KEY_8 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test8.json");
        if (key == GLFW_KEY_9 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test9.json");
        if (key == GLFW_KEY_0 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json("json/test10.json");

        //Custom filenames for savestates
        if (key == GLFW_KEY_GRAVE_ACCENT  and action == GLFW_PRESS) physics::sh.load_to_json();
        if (key == GLFW_KEY_GRAVE_ACCENT  and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) physics::sh.load_from_json();
    }

    //Mouse keys
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

        //Right mouse button for swipe select        
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (action == GLFW_PRESS) {
                is_mouse_pressed = true;
                swipe_start = window::get_mouse_coords(window);
            }
            else if (action == GLFW_RELEASE) {
                is_mouse_pressed = false;
                swipe_end = window::get_mouse_coords(window);
                physics::sh.swipe_select(swipe_start, swipe_end);
            }
        }

        //Left mouse button for click select (toggle)
        if (button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS) {
            for (auto& shape : physics::sh.shapes) if (Shapes::is_inside(window::get_mouse_coords(window), shape)) Shapes::toggle_selected(shape);
        }
    }

    //Scroll callback for shape resizing
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

        for (auto& shape : physics::sh.shapes) if (Shapes::is_selected(shape)) Shapes::resize_shape(shape, yoffset/100.0);      
    }
}