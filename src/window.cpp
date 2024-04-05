#include "../header/header.h"

void errorCallback(int error, const char* description){
    std::cerr << "Error: " << description << std::endl;
}

void draw_background(){
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    bgsh.draw_all_shapes();
    /*
    
    glBegin(GL_LINES);
    glColor3ub(255, 255, 255);
    glVertex2f(-1.0f, GROUND);
    glVertex2f(1.0f, GROUND+0.0001f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0,0,0);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, GROUND);
    glVertex2f(-1.0f, GROUND);
    glEnd(); */
}