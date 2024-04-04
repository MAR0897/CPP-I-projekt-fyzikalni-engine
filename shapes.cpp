#include "header.h"


//Shapes struct----------------------------------------------------------------------------------------------------

Shapes::Shapes(const std::vector<Shape>& sh) : shapes(sh) {}
Shapes::Shapes(std::vector<Shape>&& sh) : shapes(std::move(sh)) {}
void Shapes::add_shape(const Shape& sh) { shapes.emplace_back(sh); }
void Shapes::add_shape(Shape&& sh) { shapes.emplace_back(std::move(sh)); }

void Shapes::delete_shape(const Vec& mousepos) {
    shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [&](const Shape& shape) {
        return (Shapes::is_inside(mousepos, shape) and !Shapes::is_static(shape));
    }), shapes.end());
}


bool Shapes::is_selected(const Shape& shape) {
    bool selected = false;
    std::visit(overloaded {
        [&](const Rectangle& r) { if (r.is_selected) selected = true; },
        [&](const Triangle& t) { if (t.is_selected) selected = true; },
        [&](const Circle& c) { if (c.is_selected) selected = true; },
    }, shape);
    return selected;
}
void Shapes::toggle_selected(Shape& shape) {
    if(!Shapes::is_static(shape)){
        std::visit(overloaded {
            [](Rectangle& r) { r.is_selected = !r.is_selected; },
            [](Triangle& t) { t.is_selected = !t.is_selected; },
            [](Circle& c) { c.is_selected = !c.is_selected; },
        }, shape);
    }
}
bool Shapes::is_static(const Shape& shape) {
    bool static_shape = false;
    std::visit(overloaded {
        [&](const Rectangle& r) { if (r.is_static) static_shape = true; },
        [&](const Triangle& t) { if (t.is_static) static_shape = true; },
        [&](const Circle& c) { if (c.is_static) static_shape = true; },
    }, shape);
    return static_shape;
}
void Shapes::toggle_static(Shape& shape) {
    std::visit(overloaded {
        [](Rectangle& r) { 
            if(!r.is_static){
                r.is_selected = false;
                r.is_static = true;
                r.size -= STATIC_SHAPES_OUTLINE;
                r.static_vertices = Shapes::get_vertices(r); 
                r.size += STATIC_SHAPES_OUTLINE;
                r.vel = Vec{0.0,0.0};
            }
            else r.is_static = false;
        },
        [](Triangle& t) { 
            if(!t.is_static){
                t.is_selected = false;
                t.is_static = true;
                t.size -= STATIC_SHAPES_OUTLINE;
                t.static_vertices = Shapes::get_vertices(t); 
                t.size += STATIC_SHAPES_OUTLINE;
                t.vel = Vec{0.0,0.0};
            }
            else t.is_static = false;
        },
        [](Circle& c) { 
            if(!c.is_static){
                c.is_selected = false;
                c.is_static = true;
                c.vel = Vec{0.0,0.0};
            }
            else c.is_static = false;
        },
    }, shape);
}


bool Shapes::is_inside(const Vec& mousepos, const Shape& shape) {
        
    bool inside = false;
    double X = mousepos.x;
    double Y = mousepos.y;

    std::visit(overloaded {
        [&](const Rectangle& r){
            if ((r.pos.x-r.size.x/2 <= X and X <= r.pos.x + r.size.x/2) and
                (r.pos.y-r.size.y/2 <= Y and Y <= r.pos.y + r.size.y/2)) inside = true;
        },
        [&](const Triangle& t){
            double d1, d2, d3;
            d1 = (X - t.vertices[1].x) * (t.vertices[0].y - t.vertices[1].y) - (t.vertices[0].x - t.vertices[1].x) * (Y - t.vertices[1].y);
            d2 = (X - t.vertices[2].x) * (t.vertices[1].y - t.vertices[2].y) - (t.vertices[1].x - t.vertices[2].x) * (Y - t.vertices[2].y);
            d3 = (X - t.vertices[0].x) * (t.vertices[2].y - t.vertices[0].y) - (t.vertices[2].x - t.vertices[0].x) * (Y - t.vertices[0].y);
            inside = not (((d1 < 0) or (d2 < 0) or (d3 < 0)) and ((d1 > 0) or (d2 > 0) or (d3 > 0)));
        },
        [&](const Circle& c){
            Vec a{X-c.pos.x, Y-c.pos.y};
            if (a.norm() <= c.rad) inside = true;
        },
    }, shape);

    return inside;
}


std::vector<Vec> Shapes::get_vertices(const Shape& shape) {

    std::vector<Vec> vertices;
    
    std::visit(overloaded {
        [&](const Rectangle& r) { 
            vertices.emplace_back(Vec{r.size.x/2, r.size.y/2});
            vertices.emplace_back(Vec{-r.size.x/2, r.size.y/2});
            vertices.emplace_back(Vec{-r.size.x/2, -r.size.y/2});
            vertices.emplace_back(Vec{r.size.x/2, -r.size.y/2});
            for(auto& vertex : vertices){
                vertex.rotation_transform(r.rot);   //rotate the vectors around point (0,0)
                vertex+=r.pos;                      //move the vertices to their position
            }
        },
        [&](const Triangle& t) { 
            vertices.emplace_back(Vec{0, t.size});
            vertices.emplace_back(Vec{t.size*std::pow(3, 0.5)/2, -(t.size/2)});
            vertices.emplace_back(Vec{-t.size*std::pow(3, 0.5)/2, -(t.size/2)});
            for(auto& vertex : vertices){
                vertex.rotation_transform(t.rot); 
                vertex+=t.pos;
            }
        },
        [&](const Circle& c) {},
    }, shape);

    return vertices;
}

void Shapes::draw(const Shape& shape) {

    std::vector<Vec> vertices;

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
        },
        [&](const Triangle& t) {
            glBegin(GL_TRIANGLES);

            if (t.is_selected) glColor3ub(255,0,0);
            else glColor3ub(0,250,250);
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

    std::vector<Vec> vertices;

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
        [&](const Circle& c) {
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



void Shapes::resize_shape(Shape& sh, const double& offset){
    std::visit(overloaded {
        [&](Rectangle& r) { r.size+=offset; r.vertices = Shapes::get_vertices(r); },
        [&](Triangle& t) { t.size+=offset; t.vertices = Shapes::get_vertices(t); },
        [&](Circle& c) { c.rad+=offset; },
    }, sh);
}
void Shapes::rotate_shape(Shape& sh, const double& angle){
    if(angle!=0.0){
        std::visit(overloaded {
            [&](Rectangle& r) { r.rot+=angle; r.vertices = Shapes::get_vertices(r); },
            [&](Triangle& t) { t.rot+=angle; t.vertices = Shapes::get_vertices(t); },
            [&](Circle& c) { c.rot+=angle; },
        }, sh);
    }
}
void Shapes::move_shape_to(Shape& sh, const Vec& offset){
    std::visit(overloaded {
        [&](Rectangle& r) { r.pos=offset; r.vertices = Shapes::get_vertices(r); },
        [&](Triangle& t) { t.pos=offset; t.vertices = Shapes::get_vertices(t); },
        [&](Circle& c) { c.pos=offset; },
    }, sh);
}
void Shapes::move_shape(Shape& sh, const Vec& offset){
    if(offset != Vec{0.0,0.0} and !Shapes::is_static(sh))
    std::visit(overloaded {
        [&](Rectangle& r) { r.pos+=offset; r.vertices = Shapes::get_vertices(r); },
        [&](Triangle& t) { t.pos+=offset; t.vertices = Shapes::get_vertices(t); },
        [&](Circle& c) { c.pos+=offset; },
    }, sh);
}

void Shapes::update_position(const double& delta) { // s = v * t
    for (auto& shape : shapes){
        if(!Shapes::is_static(shape)){

            Vec velocity{0,0};

            std::visit(overloaded {
            [&](Rectangle& r) { velocity = r.vel; },
            [&](Triangle& t) { velocity = t.vel; },
            [&](Circle& c) { velocity = c.vel; },
            }, shape);

            Shapes::move_shape(shape, velocity*delta);
        }
    }
}

void Shapes::update_by_force(const double& delta, const Vec& force) {// v = a * t = F/m * t
    for (auto& shape : shapes){
        if (!Shapes::is_static(shape)){
            std::visit(overloaded {
            [&](Rectangle& r) { r.vel+=(force/r.mass)*delta; },
            [&](Triangle& t) { t.vel+=(force/t.mass)*delta; },
            [&](Circle& c) { c.vel+=(force/c.mass)*delta; },
            }, shape);
        }
    }
}

void Shapes::update_by_acceleration(const double& delta, const Vec& acceleration) { // v = a * t
    for (auto& shape : shapes){
        if (!Shapes::is_static(shape)){
            std::visit(overloaded {
            [&](Rectangle& r) { r.vel+=acceleration*delta; },
            [&](Triangle& t) { t.vel+=acceleration*delta; },
            [&](Circle& c) { c.vel+=acceleration*delta; },
            }, shape);
        }
    }
}



bool Shapes::intersect(const Shape& sh1, const Shape& sh2, double& depth, Vec& normal){

    bool do_they_intersect = false;

    std::visit(overloaded {
        [&](const Rectangle& r1, const Rectangle& r2) {
            
        },
        [&](const Rectangle& r, const Triangle& t) {
            
        },
        [&](const Rectangle& r, const Circle& c) {
            
        },
        [&](const Triangle& t, const Rectangle& r) {
            
        },
        [&](const Triangle& t1, const Triangle& t2) {
            
        },
        [&](const Triangle& t, const Circle& c) {
            
        },
        [&](const Circle& c, const Rectangle& r) {
            
        },
        [&](const Circle& c, const Triangle& t) {
            
        },
        [&](const Circle& c1, const Circle& c2) {

            double centers_distance = c1.pos.distance(c2.pos);
            double radii_sum = c1.rad + c2.rad;

            if(centers_distance<radii_sum) {
                depth = radii_sum - centers_distance;
                normal = (c1.pos-c2.pos).normalize();
                do_they_intersect = true;
            }
        }
    }, sh1, sh2);

    return do_they_intersect;
}

void Shapes::handle_collisions(){

    for (auto it = shapes.begin(); it!=shapes.end(); ++it){
        for (auto it2 = std::next(it); it2!=shapes.end(); ++it2){
            double depth = 0.0;
            Vec normal{0.0, 0.0};
            if (Shapes::intersect(*it, *it2, depth, normal)){
                Shapes::move_shape(*it, normal * depth/2.0);
                Shapes::move_shape(*it2, -normal * depth/2.0);  //normaly musi byt minusovane takhle!!!
            }
        }
    }

}






   


    

   
