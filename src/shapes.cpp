#include "../header/header.h"


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

void Shapes::delete_out_of_screen() {
    shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [](const Shape& shape) {
        AABB aabb = Shapes::get_AABB(shape);
        return aabb.max.y < -1.0;
    }), shapes.end());
}

Vec& Shapes::get_velocity(Shape& shape) {
    return std::visit(overloaded {
        [&](Rectangle& r) -> Vec& { return r.vel; },
        [&](Triangle& t) -> Vec& { return t.vel; },
        [&](Circle& c) -> Vec& { return c.vel; },
    }, shape);
}
double& Shapes::get_rot_velocity(Shape& shape) {
    return std::visit(overloaded {
        [&](Rectangle& r) -> double& { return r.rotvel; },
        [&](Triangle& t) -> double& { return t.rotvel; },
        [&](Circle& c) -> double& { return c.rotvel; },
    }, shape);
}
double Shapes::get_mass(const Shape& shape) {
    return std::visit(overloaded {
        [&](const Rectangle& r) -> double { return r.mass; },
        [&](const Triangle& t) -> double { return t.mass; },
        [&](const Circle& c) -> double { return c.mass; },
    }, shape);
}
double Shapes::get_inverse_mass(const Shape& shape) {
    return std::visit(overloaded {
        [&](const Rectangle& r) -> double { return r.inverse_mass; },
        [&](const Triangle& t) -> double { return t.inverse_mass; },
        [&](const Circle& c) -> double { return c.inverse_mass; },
    }, shape);
}
double Shapes::get_inverse_rot_inertia(const Shape& shape) {
    return std::visit(overloaded {
        [&](const Rectangle& r) -> double { return r.inverse_rot_inertia; },
        [&](const Triangle& t) -> double { return t.inverse_rot_inertia; },
        [&](const Circle& c) -> double { return c.inverse_rot_inertia;},
    }, shape);
}
double Shapes::get_restitution(const Shape& shape) {
    return std::visit(overloaded {
        [&](const Rectangle& r) -> double { return r.restitution; },
        [&](const Triangle& t) -> double { return t.restitution; },
        [&](const Circle& c) -> double { return c.restitution; },
    }, shape);
}
Vec Shapes::get_position(const Shape& shape) {
    return std::visit(overloaded {
        [&](const Rectangle& r) -> Vec { return r.pos; },
        [&](const Triangle& t) -> Vec { return t.pos; },
        [&](const Circle& c) -> Vec { return c.pos; },
    }, shape);
}
AABB Shapes::get_AABB(const Shape& shape){
    return std::visit(overloaded {
        [&](const Rectangle& r) -> AABB { return r.box; },
        [&](const Triangle& t) -> AABB { return t.box; },
        [&](const Circle& c) -> AABB { return c.box; },
    }, shape);
}



bool Shapes::is_selected(const Shape& shape) {
    return std::visit(overloaded {
        [&](const Rectangle& r) -> bool { return r.is_selected; },
        [&](const Triangle& t) -> bool { return t.is_selected; },
        [&](const Circle& c) -> bool { return c.is_selected; },
    }, shape);
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
    return std::visit(overloaded {
        [&](const Rectangle& r) -> bool { return r.is_static; },
        [&](const Triangle& t) -> bool { return t.is_static; },
        [&](const Circle& c) -> bool { return c.is_static; },
    }, shape);
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
                r.inverse_mass = 0.0;
                r.inverse_rot_inertia = 0.0;
            }
            else {
                r.is_static = false;
                r.inverse_mass = 1.0/r.mass;
                r.inverse_rot_inertia = 1.0/r.rot_inertia;
            }
        },
        [](Triangle& t) { 
            if(!t.is_static){
                t.is_selected = false;
                t.is_static = true;
                t.size -= STATIC_SHAPES_OUTLINE;
                t.static_vertices = Shapes::get_vertices(t); 
                t.size += STATIC_SHAPES_OUTLINE;
                t.vel = Vec{0.0,0.0};
                t.inverse_mass = 0.0;
                t.inverse_rot_inertia = 0.0;
            }
            else {
                t.is_static = false;
                t.inverse_mass = 1.0/t.mass;
                t.inverse_rot_inertia = 1.0/t.rot_inertia;
            }
        },
        [](Circle& c) { 
            if(!c.is_static){
                c.is_selected = false;
                c.is_static = true;
                c.vel = Vec{0.0,0.0};
                c.inverse_mass = 0.0;
                c.inverse_rot_inertia = 0.0;
            }
            else {
                c.is_static = false;
                c.inverse_mass = 1.0/c.mass;
                c.inverse_rot_inertia = 1.0/c.rot_inertia;
            }
        },
    }, shape);
}


bool Shapes::is_inside(const Vec& mousepos, const Shape& shape) {
        
    double X = mousepos.x;
    double Y = mousepos.y;

    return std::visit(overloaded {
        [&](const Rectangle& r) {
            if ((r.pos.x-r.size.x/2 <= X and X <= r.pos.x + r.size.x/2) and
                (r.pos.y-r.size.y/2 <= Y and Y <= r.pos.y + r.size.y/2)) return true;
            return false;
        },
        [&](const Triangle& t) {
            double d1, d2, d3;
            d1 = (X - t.vertices[1].x) * (t.vertices[0].y - t.vertices[1].y) - (t.vertices[0].x - t.vertices[1].x) * (Y - t.vertices[1].y);
            d2 = (X - t.vertices[2].x) * (t.vertices[1].y - t.vertices[2].y) - (t.vertices[1].x - t.vertices[2].x) * (Y - t.vertices[2].y);
            d3 = (X - t.vertices[0].x) * (t.vertices[2].y - t.vertices[0].y) - (t.vertices[2].x - t.vertices[0].x) * (Y - t.vertices[0].y);
            return not (((d1 < 0) or (d2 < 0) or (d3 < 0)) and ((d1 > 0) or (d2 > 0) or (d3 > 0)));
        },
        [&](const Circle& c) {
            Vec a{X-c.pos.x, Y-c.pos.y};
            if (a.norm() <= c.rad) return true;
            return false;
        },
    }, shape);
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

AABB Shapes::create_AABB(const Shape& shape) {

    AABB aabb;
    double minX = std::numeric_limits<double>::infinity(); 
    double maxX = -std::numeric_limits<double>::infinity();
    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();


    std::visit(overloaded {
        [&](const Rectangle& r) {
        
            for (const auto& vertex: r.vertices){
                if (vertex.x<minX) minX = vertex.x;
                if (vertex.x>maxX) maxX = vertex.x;
                if (vertex.y<minY) minY = vertex.y;
                if (vertex.y>maxY) maxY = vertex.y;
            }

            aabb = AABB{minX, maxX, minY, maxY};
        },
        [&](const Triangle& t) { 

            for (const auto& vertex: t.vertices){
                if (vertex.x<minX) minX = vertex.x;
                if (vertex.x>maxX) maxX = vertex.x;
                if (vertex.y<minY) minY = vertex.y;
                if (vertex.y>maxY) maxY = vertex.y;
            }

            aabb = AABB{minX, maxX, minY, maxY};
        },
        [&](const Circle& c) {
            aabb = AABB{c.pos-c.rad, c.pos+c.rad};
        },
    }, shape);

    return aabb;
}



void Shapes::resize_shape(Shape& sh, const double& offset){
    std::visit(overloaded {
        [&](Rectangle& r) { 
            if (r.size.norm() > 0.05 or offset > 0.0) {
                r.size+=offset;
                r.vertices = Shapes::get_vertices(r); 
                r.box = Shapes::create_AABB(r);
                Shapes::change_mass(sh);
                Shapes::change_rot_inertia(sh);
            }
        },
        [&](Triangle& t) { 
            if (t.size > 0.01 or offset > 0.0) {
                t.size+=offset;
                t.vertices = Shapes::get_vertices(t);
                t.box = Shapes::create_AABB(t);
                Shapes::change_mass(sh);
                Shapes::change_rot_inertia(sh);
            }
        },
        [&](Circle& c) { 
            if (c.rad > 0.02 or offset > 0.0) {
                c.rad+=offset;
                c.box = Shapes::create_AABB(c);
                Shapes::change_mass(sh);
                Shapes::change_rot_inertia(sh);
            }
        },
    }, sh);
}
void Shapes::change_mass(Shape& sh){
    std::visit(overloaded {
        [&](Rectangle& r) { r.mass = r.size.x*r.size.y*r.density; r.inverse_mass = 1.0/r.mass; },
        [&](Triangle& t) { t.mass = t.density*t.size*t.size*3.0*std::pow(3, 0.5)/4.0; t.inverse_mass = 1.0/t.mass; },
        [&](Circle& c) { c.mass = PI*c.rad*c.rad*c.density; c.inverse_mass = 1.0/c.mass;},
    }, sh);
}
void Shapes::change_rot_inertia(Shape& sh){
    std::visit(overloaded {
        [&](Rectangle& r) { r.rot_inertia = (1.0/12.0)*r.mass*r.size.norm2(); },    // J = 1/12 * m * (a^2+b^2)
        [&](Triangle& t) { t.rot_inertia = (1.0/12.0)*t.mass*t.size*std::pow(3, 0.5); }, // J = 1/12 * m * a (pro rovnostranny trojuhelnik)
        [&](Circle& c) { c.rot_inertia = 0.5*c.mass*c.rad*c.rad; }, // J = 1/2 * m * r^2
    }, sh);
}

void Shapes::rotate_shape(Shape& sh, const double& angle){
    if(angle!=0.0){
        std::visit(overloaded {
            [&](Rectangle& r) { 
                r.rot+=angle;
                r.vertices = Shapes::get_vertices(r);
                r.box = Shapes::create_AABB(r);
            },
            [&](Triangle& t) { 
                t.rot+=angle;
                t.vertices = Shapes::get_vertices(t);
                t.box = Shapes::create_AABB(t);
            },
            [&](Circle& c) { c.rot+=angle; },
        }, sh);
    }
}
void Shapes::move_shape_to(Shape& sh, const Vec& offset){
    std::visit(overloaded {
        [&](Rectangle& r) {
            r.pos=offset;
            r.vertices = Shapes::get_vertices(r);
            r.box = Shapes::create_AABB(r);
        },
        [&](Triangle& t) {
            t.pos=offset;
            t.vertices = Shapes::get_vertices(t);
            t.box = Shapes::create_AABB(t);
        },
        [&](Circle& c) {
            c.pos=offset;
            c.box = Shapes::create_AABB(c);
        },
    }, sh);
}
void Shapes::move_shape(Shape& sh, const Vec& offset){
    if(offset != Vec{0.0,0.0} and !Shapes::is_static(sh))
    std::visit(overloaded {
        [&](Rectangle& r) {
            r.pos+=offset;
            r.vertices = Shapes::get_vertices(r);
            r.box = Shapes::create_AABB(r);
        },
        [&](Triangle& t) {
            t.pos+=offset;
            t.vertices = Shapes::get_vertices(t);
            t.box = Shapes::create_AABB(t);
        },
        [&](Circle& c) {
            c.pos+=offset;
            c.box = Shapes::create_AABB(c);
        },
    }, sh);
}


void Shapes::update_position_and_rotation(const double& delta) { // s = v * t (and phi = omega * T)
    for (auto& shape : shapes){
        if(!Shapes::is_static(shape)){

            Vec& velocity = Shapes::get_velocity(shape);

            Shapes::move_shape(shape, velocity*delta);

            double& rot_velocity = Shapes::get_rot_velocity(shape);

            Shapes::rotate_shape(shape, rot_velocity*delta);
        }
    }
}

void Shapes::update_by_force(const double& delta) {// v = a * t = F/m * t
    for (auto& shape : shapes){
        if (!Shapes::is_static(shape)){
            std::visit(overloaded {
            [&](Rectangle& r) { r.vel+=(r.force/r.mass)*delta; r.force*=0.0; },
            [&](Triangle& t) { t.vel+=(t.force/t.mass)*delta; t.force*=0.0; },
            [&](Circle& c) { c.vel+=(c.force/c.mass)*delta; c.force*=0.0; },
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

void Shapes::add_force(Shape& shape, const Vec& force) {
    if (!Shapes::is_static(shape)){
        std::visit(overloaded {
        [&](Rectangle& r) { r.force+=force; },
        [&](Triangle& t) { t.force+=force; },
        [&](Circle& c) { c.force+=force; },
        }, shape);
    }
}
