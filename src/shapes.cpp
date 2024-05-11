#include "../header/header.h"

//Shapes struct member functions

//=============================================================================================================================================
//constructors, adding and deleting a shape

Shapes::Shapes(const std::vector<Shape>& sh) : shapes(sh) {}
Shapes::Shapes(std::vector<Shape>&& sh) : shapes(std::move(sh)) {}
void Shapes::add_shape(const Shape& sh) { shapes.emplace_back(sh); }
void Shapes::add_shape(Shape&& sh) { shapes.emplace_back(std::move(sh)); }

void Shapes::delete_shape(const Vec& mousepos) {
    shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [&](const Shape& shape) {
        return (is_inside(mousepos, shape) and !is_static(shape));
    }), shapes.end());
}
void Shapes::delete_selected(){
    shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [&](const Shape& shape) {
        return is_selected(shape);
    }), shapes.end());
}
void Shapes::delete_out_of_screen() {
    shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [](const Shape& shape) {
        AABB aabb = Shapes::get_AABB(shape);
        return aabb.max.y < -1.0;
    }), shapes.end());
}

//=============================================================================================================================================
//getters and checkers

Vec& Shapes::get_velocity(Shape& shape) { return std::visit([&](auto& s) -> Vec& { return s.vel; }, shape); }
double& Shapes::get_rot_velocity(Shape& shape) { return std::visit([&](auto& s) -> double& { return s.rotvel; }, shape); }
double Shapes::get_mass(const Shape& shape) { return std::visit([&](auto& s) -> double { return s.mass; }, shape); }
double Shapes::get_density(const Shape& shape) { return std::visit([&](auto& s) -> double { return s.density; }, shape); }
double Shapes::get_inverse_mass(const Shape& shape) { return std::visit([&](auto& s) -> double { return s.inverse_mass; }, shape); }
double Shapes::get_inverse_rot_inertia(const Shape& shape) { return std::visit([&](auto& s) -> double { return s.inverse_rot_inertia; }, shape); }
double Shapes::get_restitution(const Shape& shape) { return std::visit([&](auto& s) -> double { return s.restitution; }, shape); }
double Shapes::get_static_friction(const Shape& shape) { return std::visit([&](auto& s) -> double { return s.friction_s; }, shape); }
double Shapes::get_dynamic_friction(const Shape& shape) { return std::visit([&](auto& s) -> double { return s.friction_d; }, shape); }
Vec Shapes::get_position(const Shape& shape) { return std::visit([&](auto& s) -> Vec { return s.pos; }, shape); }
double Shapes::get_rotation(const Shape& shape) { return std::visit([&](auto& s) -> double { return s.rot; }, shape); }
AABB Shapes::get_AABB(const Shape& shape) { return std::visit([&](auto& s) -> AABB { return s.box; }, shape); }

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

//=============================================================================================================================================
//functions for shape selecting

bool Shapes::is_selected(const Shape& shape) { return std::visit([&](auto& s) -> bool { return s.is_selected; }, shape); }

void Shapes::toggle_selected(Shape& shape) {
    if(!Shapes::is_static(shape)){
        std::visit([](auto& s) { s.is_selected = !s.is_selected; }, shape);
    }
}

void Shapes::swipe_select(const Vec& swipe_start, const Vec& swipe_end) {
    double minX = std::min(swipe_start.x, swipe_end.x); 
    double maxX = std::max(swipe_start.x, swipe_end.x);
    double minY = std::min(swipe_start.y, swipe_end.y); 
    double maxY = std::max(swipe_start.y, swipe_end.y);
    AABB swipe{minX, maxX, minY, maxY};
    for (auto& shape : shapes) {
        if (!is_static(shape)) {
            if (intersect_aabbXaabb(get_AABB(shape), swipe)) std::visit([&](auto& s){ s.is_selected = true; }, shape); 
        }
    }
}

void Shapes::deselect() {
    for (auto& shape : shapes) std::visit([&](auto& s){ s.is_selected = false; }, shape); 
}

//=============================================================================================================================================
//functions for dealing with static shapes

bool Shapes::is_static(const Shape& shape) { return std::visit([&](auto& s) -> bool { return s.is_static; }, shape); }

template<typename S>
void Shapes::m_toggle_static(S& s) {
     if(!s.is_static){
        s.is_selected = false;
        s.is_static = true;
        if constexpr (std::is_same_v<S, Rectangle> or std::is_same_v<S, Triangle>) {
            s.size -= STATIC_SHAPES_OUTLINE;
            s.static_vertices = Shapes::get_vertices(s); 
            s.size += STATIC_SHAPES_OUTLINE;
        }
        s.vel = Vec{0.0,0.0};
        s.rotvel = 0.0;
        s.inverse_mass = 0.0;
        s.inverse_rot_inertia = 0.0;
    }
    else {
        s.is_static = false;
        s.inverse_mass = 1.0/s.mass;
        s.inverse_rot_inertia = 1.0/s.rot_inertia;
    }
}

void Shapes::toggle_static(Shape& shape) {
    std::visit([&](auto& s){ m_toggle_static(s); }, shape); 
}

//=============================================================================================================================================
//helper computing functions for defining the exact shape of an object

//loading correct vertices position into owner shape vector
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

//creating axis aligned bounding box
template<typename S>
AABB Shapes::m_create_AABB(const S& s) {

    AABB aabb;
    double minX = std::numeric_limits<double>::infinity(); 
    double maxX = -std::numeric_limits<double>::infinity();
    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();

    if constexpr (std::is_same_v<S, Rectangle> or std::is_same_v<S, Triangle>) {
        for (const auto& vertex: s.vertices){
                if (vertex.x<minX) minX = vertex.x;
                if (vertex.x>maxX) maxX = vertex.x;
                if (vertex.y<minY) minY = vertex.y;
                if (vertex.y>maxY) maxY = vertex.y;
            }
            aabb = AABB{minX, maxX, minY, maxY};
    }
    else if constexpr (std::is_same_v<S, Circle>) aabb = AABB{s.pos-s.rad, s.pos+s.rad};

    return aabb;
}

AABB Shapes::create_AABB(const Shape& shape) {
    return std::visit([&](auto& s) -> AABB { return m_create_AABB(s); }, shape);
}

//=============================================================================================================================================
//changing shape size

template<typename S, typename doubleORvec>
void Shapes::m_resize_shape(S& s, const double& offset, doubleORvec& to_resize){
    to_resize+=offset;
    if constexpr (!std::is_same_v<S, Circle>) s.vertices = Shapes::get_vertices(s); 
    s.box = Shapes::create_AABB(s);
    Shapes::change_mass(s);
    Shapes::change_rot_inertia(s);
}

void Shapes::resize_shape(Shape& sh, const double& offset){
    std::visit(overloaded {
        [&](Rectangle& r) { if (r.size.norm() > 0.05 or offset > 0.0) m_resize_shape(r, offset, r.size); },
        [&](Triangle& t) { if (t.size > 0.01 or offset > 0.0) m_resize_shape(t, offset, t.size); },
        [&](Circle& c) { if (c.rad > 0.02 or offset > 0.0) m_resize_shape(c, offset, c.rad); },
    }, sh);
}

template<typename S>
void Shapes::change_mass(S& s) {
    if constexpr (std::is_same_v<S, Rectangle>) s.mass = s.size.x*s.size.y*s.density;
    else if constexpr (std::is_same_v<S, Triangle>) s.mass = s.density*s.size*s.size*3.0*std::pow(3, 0.5)/4.0;
    else if constexpr (std::is_same_v<S, Circle>) s.mass = PI*s.rad*s.rad*s.density;
    s.inverse_mass = 1.0/s.mass;
}

template<typename S>
void Shapes::change_rot_inertia(S& s){
    if constexpr (std::is_same_v<S, Rectangle>) s.rot_inertia = (1.0/12.0)*s.mass*s.size.norm2();    // J = 1/12 * m * (a^2+b^2)
    else if constexpr (std::is_same_v<S, Triangle>) s.rot_inertia = (1.0/12.0)*s.mass*s.size*std::pow(3, 0.5); // J = 1/12 * m * a (pro rovnostranny trojuhelnik)
    else if constexpr (std::is_same_v<S, Circle>) s.rot_inertia = 0.5*s.mass*s.rad*s.rad;  // J = 1/2 * m * r^2
    s.inverse_rot_inertia = 1.0/s.rot_inertia;
}

//=============================================================================================================================================
//translation

template<typename S>
void Shapes::m_move_shape(S& s, const Vec& offset, auto operation) {
    operation(s.pos, offset);
    s.box = create_AABB(s);
    if constexpr (!std::is_same_v<S, Circle>) s.vertices = get_vertices(s);
}

void Shapes::move_shape(Shape& sh, const Vec& offset) {
    if(offset != Vec{0.0,0.0} and !is_static(sh)) {
        std::visit([&](auto& s){ m_move_shape(s, offset, [](Vec& v1, const Vec& v2){ return v1+=v2; }); }, sh);
    }
}

void Shapes::move_shape_to(Shape& sh, const Vec& offset) {
    if(offset != Vec{0.0,0.0} and !is_static(sh)) {
        std::visit([&](auto& s){ m_move_shape(s, offset, [](Vec& v1, const Vec& v2){ return v1=v2; }); }, sh);
    }
}

// F1 += F2
void Shapes::add_force(Shape& shape, const Vec& force) {
    if (!is_static(shape)){
        std::visit([&](auto& s){ s.force+=force; }, shape);   
    }
}

//=============================================================================================================================================
//rotation

template<typename S>
void Shapes::m_rotate_shape(S& s, const double& angle, auto operation) {
    operation(s.rot, angle);
    if constexpr (!std::is_same_v<S, Circle>) {
        s.vertices = get_vertices(s);
        s.box = create_AABB(s);
    }
}

void Shapes::rotate_shape(Shape& sh, const double& angle) {
    if(angle != 0.0 and !is_static(sh)) {
        std::visit([&](auto& s){ m_rotate_shape(s, angle, [](double& v1, const double& v2){ return v1+=v2; }); }, sh);
    }
}

void Shapes::add_spin(Shape& sh, const double& speed) {
    if(speed!=0.0){
        std::visit([&](auto& s){ s.rotvel+=speed*s.inverse_rot_inertia; }, sh);
    }
}

//=============================================================================================================================================
//physical step helper functions

// s = v * t (and phi = omega * T)
void Shapes::update_position_and_rotation(const double& delta) {
    for (auto& shape : shapes){
        if(!is_static(shape)){

            Vec& velocity = get_velocity(shape);
            move_shape(shape, velocity*delta);
            double& rot_velocity = get_rot_velocity(shape);
            rotate_shape(shape, rot_velocity*delta);
        }
    }
}

// v = a * t = F/m * t
void Shapes::update_by_force(const double& delta) {
    for (auto& shape : shapes) {
        if (!is_static(shape)) {
            std::visit([&](auto& s){ s.vel+=(s.force/s.mass)*delta; s.force*=0.0;}, shape);
        }
    }
}

// v = a * t
void Shapes::update_by_acceleration(const double& delta, const Vec& acceleration) { 
    for (auto& shape : shapes){
        if (!is_static(shape)){
            std::visit([&](auto& s){ s.vel+=acceleration*delta; }, shape);
        }
    }
}

