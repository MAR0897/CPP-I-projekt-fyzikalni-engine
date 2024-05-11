#include "../header/header.h"

void cbs::errorCallback(int error, const char* description){
    std::cerr << "Error: " << description << std::endl;
}

namespace window{

    void draw_background(){
        glClearColor(0.6f, 0.8f, 0.87f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        bgsh.draw_all_shapes();
    }

    Vec get_mouse_coords(GLFWwindow* window){
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        double Xpos = (mouseX / (static_cast<double>(WIDTH)/2.0)) - 1.0;
        double Ypos = 1.0 - (mouseY / (static_cast<double>(HEIGHT)/2.0));
        return Vec{Xpos, Ypos/ST};
    }

    void render_text(double&& x, double&& y, void* font, std::string&& str) {
        glColor3ub(0,0,0);
        glRasterPos2d(x, y);
        for (auto ch : str) glutBitmapCharacter(font, ch);
    }
}
