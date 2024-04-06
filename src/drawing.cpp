#include "../header/header.h"

//functions which draw shapes on the screen

void Shapes::draw(const Shape& shape) {

    std::visit(overloaded {
        [&](const Rectangle& r) { 
            glBegin(GL_QUADS);
            if (r.is_selected) glColor3ub(255,0,0);
            else glColor3ub(0,200,0);
            glVertex2d(r.vertices[0].x, r.vertices[0].y*ST);
            glVertex2d(r.vertices[1].x, r.vertices[1].y*ST);
            glVertex2d(r.vertices[2].x, r.vertices[2].y*ST);
            glVertex2d(r.vertices[3].x, r.vertices[3].y*ST);
            glEnd();

            glBegin(GL_LINE_LOOP);
            glColor3ub(0,0,0);
            glVertex2d(r.vertices[0].x, r.vertices[0].y*ST);
            glVertex2d(r.vertices[1].x, r.vertices[1].y*ST);
            glVertex2d(r.vertices[2].x, r.vertices[2].y*ST);
            glVertex2d(r.vertices[3].x, r.vertices[3].y*ST);
            glEnd();
        },
        [&](const Triangle& t) {
            glBegin(GL_TRIANGLES);
            if (t.is_selected) glColor3ub(255,0,0);
            else glColor3ub(0,250,250);
            glVertex2d(t.vertices[0].x, t.vertices[0].y*ST);
            glVertex2d(t.vertices[1].x, t.vertices[1].y*ST);
            glVertex2d(t.vertices[2].x, t.vertices[2].y*ST);
            glEnd();

            glBegin(GL_LINE_LOOP);
            if (t.is_selected) glColor3ub(255,0,0);
            glColor3ub(0,0,0);
            glVertex2d(t.vertices[0].x, t.vertices[0].y*ST);
            glVertex2d(t.vertices[1].x, t.vertices[1].y*ST);
            glVertex2d(t.vertices[2].x, t.vertices[2].y*ST);
            glEnd();
        },
        [&](const Circle& c) {

            glBegin(GL_TRIANGLE_FAN);
            if (c.is_selected) glColor3ub(255,0,0);
            else glColor3ub(0,0,255);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (PI/180.0);
                glVertex2d(c.pos.x + (cos(angle)*c.rad), (c.pos.y + (sin(angle)*c.rad))*ST);
            }
            glEnd();

        },
    }, shape);
}

void Shapes::draw_static(const Shape& shape) {

    std::visit(overloaded {
        [&](const Rectangle& r) { 
            glBegin(GL_QUADS);

            glColor3ub(255,255,255);
            glVertex2d(r.vertices[0].x, r.vertices[0].y*ST);
            glVertex2d(r.vertices[1].x, r.vertices[1].y*ST);
            glVertex2d(r.vertices[2].x, r.vertices[2].y*ST);
            glVertex2d(r.vertices[3].x, r.vertices[3].y*ST);

            glColor3ub(0,0,0);
            glVertex2d(r.static_vertices[0].x, r.static_vertices[0].y*ST);
            glVertex2d(r.static_vertices[1].x, r.static_vertices[1].y*ST);
            glVertex2d(r.static_vertices[2].x, r.static_vertices[2].y*ST);
            glVertex2d(r.static_vertices[3].x, r.static_vertices[3].y*ST);
            
            glEnd();
        },
        [&](const Triangle& t) {
            glBegin(GL_TRIANGLES);
            
            glColor3ub(255,255,255);
            glVertex2d(t.vertices[0].x, t.vertices[0].y*ST);
            glVertex2d(t.vertices[1].x, t.vertices[1].y*ST);
            glVertex2d(t.vertices[2].x, t.vertices[2].y*ST);

            glColor3ub(0,0,0);
            glVertex2d(t.static_vertices[0].x, t.static_vertices[0].y*ST);
            glVertex2d(t.static_vertices[1].x, t.static_vertices[1].y*ST);
            glVertex2d(t.static_vertices[2].x, t.static_vertices[2].y*ST);


            glEnd();
        },
        [&](const Circle& c) {  //buggy result, otherwise works fine
            glBegin(GL_TRIANGLE_FAN);

            glColor3ub(255,255,255);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (PI/180.0);
                glVertex2d(c.pos.x + (cos(angle)*c.rad), (c.pos.y + ((sin(angle)*c.rad)))*ST);
            }
            glColor3ub(0,0,0);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (PI/180.0);
                glVertex2d(c.pos.x + (cos(angle)*(c.rad-STATIC_SHAPES_OUTLINE)), (c.pos.y + (sin(angle)*(c.rad-STATIC_SHAPES_OUTLINE)))*ST);
            }
            glEnd();
        },
    }, shape);

}

void Shapes::draw_all_shapes() const { 
    for (const auto& shape : shapes) {
        if (Shapes::is_static(shape)) Shapes::draw_static(shape); 
        else Shapes::draw(shape);
    }
}