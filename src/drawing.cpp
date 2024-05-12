#include "../header/header.h"

//functions which draw shapes on the screen

void Shapes::draw(const Shape& shape) {

    std::visit(overloaded {
        [&](const Rectangle& r) { 
            glBegin(GL_QUADS);
            if (r.is_selected) glColor3ub(255,0,0);
            else glColor3ub(0,200.0/r.density,0);
            glVertex2d(r.vertices[0].x, r.vertices[0].y*window::ST);
            glVertex2d(r.vertices[1].x, r.vertices[1].y*window::ST);
            glVertex2d(r.vertices[2].x, r.vertices[2].y*window::ST);
            glVertex2d(r.vertices[3].x, r.vertices[3].y*window::ST);
            glEnd();

            glBegin(GL_LINE_LOOP);
            glColor3ub(0,0,0);
            glVertex2d(r.vertices[0].x, r.vertices[0].y*window::ST);
            glVertex2d(r.vertices[1].x, r.vertices[1].y*window::ST);
            glVertex2d(r.vertices[2].x, r.vertices[2].y*window::ST);
            glVertex2d(r.vertices[3].x, r.vertices[3].y*window::ST);
            glEnd();
        },
        [&](const Triangle& t) {
            glBegin(GL_TRIANGLES);
            if (t.is_selected) glColor3ub(255,0,0);
            else glColor3ub(245.0/t.density, 209.0/t.density, 51.0/t.density);
            glVertex2d(t.vertices[0].x, t.vertices[0].y*window::ST);
            glVertex2d(t.vertices[1].x, t.vertices[1].y*window::ST);
            glVertex2d(t.vertices[2].x, t.vertices[2].y*window::ST);
            glEnd();

            glBegin(GL_LINE_LOOP);
            if (t.is_selected) glColor3ub(255,0,0);
            glColor3ub(0,0,0);
            glVertex2d(t.vertices[0].x, t.vertices[0].y*window::ST);
            glVertex2d(t.vertices[1].x, t.vertices[1].y*window::ST);
            glVertex2d(t.vertices[2].x, t.vertices[2].y*window::ST);
            glEnd();
        },
        [&](const Circle& c) {

            glBegin(GL_TRIANGLE_FAN);
            if (c.is_selected) glColor3ub(255,0,0);
            else glColor3ub(0,0,255/c.density);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (physics::PI/180.0);
                glVertex2d(c.pos.x + (cos(angle)*c.rad), (c.pos.y + (sin(angle)*c.rad))*window::ST);
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
            glVertex2d(r.vertices[0].x, r.vertices[0].y*window::ST);
            glVertex2d(r.vertices[1].x, r.vertices[1].y*window::ST);
            glVertex2d(r.vertices[2].x, r.vertices[2].y*window::ST);
            glVertex2d(r.vertices[3].x, r.vertices[3].y*window::ST);

            glColor3ub(0,0,0);
            glVertex2d(r.static_vertices[0].x, r.static_vertices[0].y*window::ST);
            glVertex2d(r.static_vertices[1].x, r.static_vertices[1].y*window::ST);
            glVertex2d(r.static_vertices[2].x, r.static_vertices[2].y*window::ST);
            glVertex2d(r.static_vertices[3].x, r.static_vertices[3].y*window::ST);
            
            glEnd();
        },
        [&](const Triangle& t) {
            glBegin(GL_TRIANGLES);
            
            glColor3ub(255,255,255);
            glVertex2d(t.vertices[0].x, t.vertices[0].y*window::ST);
            glVertex2d(t.vertices[1].x, t.vertices[1].y*window::ST);
            glVertex2d(t.vertices[2].x, t.vertices[2].y*window::ST);

            glColor3ub(0,0,0);
            glVertex2d(t.static_vertices[0].x, t.static_vertices[0].y*window::ST);
            glVertex2d(t.static_vertices[1].x, t.static_vertices[1].y*window::ST);
            glVertex2d(t.static_vertices[2].x, t.static_vertices[2].y*window::ST);


            glEnd();
        },
        [&](const Circle& c) {  //looks buggy, but works fine
            glBegin(GL_TRIANGLE_FAN);

            glColor3ub(255,255,255);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (physics::PI/180.0);
                glVertex2d(c.pos.x + (cos(angle)*c.rad), (c.pos.y + ((sin(angle)*c.rad)))*window::ST);
            }
            glColor3ub(0,0,0);
            for (int i = 0; i <= 360; ++i) {
                double angle = static_cast<double>(i) * (physics::PI/180.0);
                glVertex2d(c.pos.x + (cos(angle)*(c.rad-window::STATIC_SHAPES_OUTLINE)), (c.pos.y + (sin(angle)*(c.rad-window::STATIC_SHAPES_OUTLINE)))*window::ST);
            }
            glEnd();
        },
    }, shape);

}

void Shapes::draw_all_shapes() const { 
    for (const auto& shape : shapes) {
        if (is_static(shape)) draw_static(shape); 
        else draw(shape);
    }
}

void Shapes::render_statistics(double x, double y, void* font) {
    for (auto& shape : shapes) {
        if (is_selected(shape)) {
            std::visit([&](auto& s) { m_render_statistics(s, x, y, font); }, shape);
            break;
        }
    }
}

template<typename S>
void Shapes::m_render_statistics(S& s, double x, double y, void* font) {
    window::render_text(x-0.0, y-0.0, font, "Velocity: ["+std::to_string(s.vel.x)+"x, "+std::to_string(s.vel.y)+"y]");
    window::render_text(x-0.0, y-0.1, font, "Rot. vel.: "+std::to_string(s.rotvel));
    window::render_text(x-0.0, y-0.2, GLUT_BITMAP_HELVETICA_12, "Mass: "+std::to_string(s.mass));
    window::render_text(x-0.0, y-0.25, GLUT_BITMAP_HELVETICA_12, "Rotational invertia: "+std::to_string(s.rot_inertia));
    window::render_text(x-0.0, y-0.3, GLUT_BITMAP_HELVETICA_12, "Restitution: "+std::to_string(s.restitution));
    window::render_text(x-0.0, y-0.35, GLUT_BITMAP_HELVETICA_12, "Static friction: "+std::to_string(s.friction_s));
    window::render_text(x-0.0, y-0.4, GLUT_BITMAP_HELVETICA_12, "Dynamic friction: "+std::to_string(s.friction_d));
    window::render_text(x-0.0, y-0.45, GLUT_BITMAP_HELVETICA_12, "Density: "+std::to_string(s.density));
}