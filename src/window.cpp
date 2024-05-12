#include "../header/header.h"

namespace window{

    void draw_background(){
        //draw background
        glClearColor(0.6f, 0.8f, 0.87f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //render statistics
        window::render_text(-0.95, 0.9, GLUT_BITMAP_HELVETICA_18, "Number of shapes: "+std::to_string(physics::sh.shapes.size()));
        physics::sh.render_statistics(-0.95, 0.8, GLUT_BITMAP_HELVETICA_18);
    }

    Vec get_mouse_coords(GLFWwindow* window){
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        double Xpos = (mouseX / (static_cast<double>(WIDTH)/2.0)) - 1.0;
        double Ypos = 1.0 - (mouseY / (static_cast<double>(HEIGHT)/2.0));
        return Vec{Xpos, Ypos/ST};
    }

    void render_text(const double& x, const double& y, void* font, const std::string& str) {
        glColor3ub(0,0,0);
        glRasterPos2d(x, y);
        for (auto ch : str) glutBitmapCharacter(font, ch); //LEAKS MEMORY (neni moje chyba ig?)
    }
}
