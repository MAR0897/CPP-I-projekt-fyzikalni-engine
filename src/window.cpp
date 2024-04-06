#include "../header/header.h"

void cbs::errorCallback(int error, const char* description){
    std::cerr << "Error: " << description << std::endl;
}

namespace window{

    void draw_background(){
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        bgsh.draw_all_shapes();
    }

    Vec get_mouse_coords(GLFWwindow* window){
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        double Xpos = (mouseX / 640.0) - 1.0;
        double Ypos = 1.0 - (mouseY / 360.0);
        return Vec{Xpos, Ypos/ST};
    }
}
