#include "../header/header.h"

namespace cbs {

    Vec swipe_start;
    Vec swipe_end;
    bool is_mouse_pressed = false;

    //Keys
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

        if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (key == GLFW_KEY_G and action == GLFW_PRESS){
            physics::gravity = !physics::gravity; 
        }
    
        if (key == GLFW_KEY_Q and action == GLFW_PRESS){
            sh.deselect();
        }

        if (key == GLFW_KEY_R and action == GLFW_PRESS){
            sh.add_shape((Rectangle{window::get_mouse_coords(window), Vec{0.2, 0.2}}));   
        }

        if (key == GLFW_KEY_T and action == GLFW_PRESS){
            sh.add_shape((Triangle{window::get_mouse_coords(window), 0.15}));    
        }

        if (key == GLFW_KEY_C and action == GLFW_PRESS){
            sh.add_shape((Circle{window::get_mouse_coords(window), 0.1}));    
        }


        if (key == GLFW_KEY_DELETE and action == GLFW_PRESS){
            //sh.delete_shape(window::get_mouse_coords(window));
            sh.delete_selected();
        }

        if (key == GLFW_KEY_A and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_spin(shape, 0.00005);   
        }
        if (key == GLFW_KEY_D and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_spin(shape, -0.00005);  
        }

        if (key == GLFW_KEY_UP and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_force(shape, Vec{0.0, 1.0});   
        }
        if (key == GLFW_KEY_DOWN and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_force(shape, Vec{0.0, -1.0});  
        }
        if (key == GLFW_KEY_LEFT and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_force(shape, Vec{-1.0, 0.0});   
        }
        if (key == GLFW_KEY_RIGHT and (action == GLFW_PRESS or action == GLFW_REPEAT)){
            for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::add_force(shape, Vec{1.0, 0.0});  
        }

        if (key == GLFW_KEY_S and action == GLFW_PRESS){
            for (auto& shape : sh.shapes) if (Shapes::is_inside(window::get_mouse_coords(window), shape)) Shapes::toggle_static(shape);   
        }


        //Creating and saving savestates
        if (key == GLFW_KEY_1 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test1.json");
        if (key == GLFW_KEY_2 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test2.json");
        if (key == GLFW_KEY_3 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test3.json");
        if (key == GLFW_KEY_4 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test4.json");
        if (key == GLFW_KEY_5 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test5.json");
        if (key == GLFW_KEY_6 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test6.json");
        if (key == GLFW_KEY_7 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test7.json");
        if (key == GLFW_KEY_8 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test8.json");
        if (key == GLFW_KEY_9 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test9.json");
        if (key == GLFW_KEY_0 and action == GLFW_PRESS) if (!(mods & GLFW_MOD_SHIFT)) sh.load_to_json("json/test10.json");
    
        //Loading savestates
        if (key == GLFW_KEY_1 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test1.json");
        if (key == GLFW_KEY_2 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test2.json");
        if (key == GLFW_KEY_3 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test3.json");
        if (key == GLFW_KEY_4 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test4.json");
        if (key == GLFW_KEY_5 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test5.json");
        if (key == GLFW_KEY_6 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test6.json");
        if (key == GLFW_KEY_7 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test7.json");
        if (key == GLFW_KEY_8 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test8.json");
        if (key == GLFW_KEY_9 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test9.json");
        if (key == GLFW_KEY_0 and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json("json/test10.json");

        //Custom filenames for savestates
        if (key == GLFW_KEY_GRAVE_ACCENT  and action == GLFW_PRESS) sh.load_to_json();
        if (key == GLFW_KEY_GRAVE_ACCENT  and action == GLFW_PRESS) if (mods & GLFW_MOD_SHIFT) sh.load_from_json();
            

    }

    //Mouse keys
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        //if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            
            //for (auto& shape : sh.shapes) if (sh.is_inside(get_mouse_coords(window), shape)) sh.move_shape_to(shape, get_mouse_coords(window));    
            //for (auto& shape : sh.shapes) if (Shapes::is_inside(window::get_mouse_coords(window), shape)) Shapes::toggle_selected(shape);
            
        
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (action == GLFW_PRESS) {
                is_mouse_pressed = true;
                swipe_start = window::get_mouse_coords(window);
            }
            else if (action == GLFW_RELEASE) {
                is_mouse_pressed = false;
                swipe_end = window::get_mouse_coords(window);
                sh.swipe_select(swipe_start, swipe_end);
            }
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS) {
            for (auto& shape : sh.shapes) if (Shapes::is_inside(window::get_mouse_coords(window), shape)) Shapes::toggle_selected(shape);
        }
    }

    //Scroll
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

        for (auto& shape : sh.shapes) if (Shapes::is_selected(shape)) Shapes::resize_shape(shape, yoffset/100.0);      
    }

}