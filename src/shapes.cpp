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

Vec& Shapes::get_velocity(Shape& shape) {
    std::visit(overloaded {
        [&](Rectangle& r) -> Vec& { return r.vel; },
        [&](Triangle& t) -> Vec& { return t.vel; },
        [&](Circle& c) -> Vec& { return c.vel; },
    }, shape);
}
double Shapes::get_mass(const Shape& shape) {
    double mass;
    std::visit(overloaded {
        [&](const Rectangle& r) { mass = r.mass; },
        [&](const Triangle& t) { mass = t.mass; },
        [&](const Circle& c) { mass = c.mass; },
    }, shape);
    return mass;
}
double Shapes::get_inverse_mass(const Shape& shape) {
    double mass;
    std::visit(overloaded {
        [&](const Rectangle& r) { mass = r.inverse_mass; },
        [&](const Triangle& t) { mass = t.inverse_mass; },
        [&](const Circle& c) { mass = c.inverse_mass; },
    }, shape);
    return mass;
}
double Shapes::get_restitution(const Shape& shape) {
    double restitution;
    std::visit(overloaded {
        [&](const Rectangle& r) { restitution = r.restitution; },
        [&](const Triangle& t) { restitution = t.restitution;  },
        [&](const Circle& c) { restitution = c.restitution;  },
    }, shape);
    return restitution;
}



bool Shapes::is_selected(const Shape& shape) {
    bool is_selected = false;
    std::visit(overloaded {
        [&](const Rectangle& r) { if(r.is_selected) is_selected = true; },
        [&](const Triangle& t) { if(t.is_selected) is_selected = true;  },
        [&](const Circle& c) { if(c.is_selected) is_selected = true;  },
    }, shape);
    return is_selected;
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
    bool is_static = false;
    std::visit(overloaded {
        [&](const Rectangle& r) { if(r.is_static) is_static = true; },
        [&](const Triangle& t) { if(t.is_static) is_static = true; },
        [&](const Circle& c) { if(c.is_static) is_static = true; },
    }, shape);
    return is_static;
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
            }
            else {
                r.is_static = false;
                r.inverse_mass = 1.0/r.mass;
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
            }
            else {
                t.is_static = false;
                t.inverse_mass = 1.0/t.mass;
            }
        },
        [](Circle& c) { 
            if(!c.is_static){
                c.is_selected = false;
                c.is_static = true;
                c.vel = Vec{0.0,0.0};
                c.inverse_mass = 0.0;
            }
            else {
                c.is_static = false;
                c.inverse_mass = 1.0/c.mass;
            }
        },
    }, shape);
}


bool Shapes::is_inside(const Vec& mousepos, const Shape& shape) {
        
    bool is_inside = false;
    double X = mousepos.x;
    double Y = mousepos.y;

    std::visit(overloaded {
        [&](const Rectangle& r) {
            if ((r.pos.x-r.size.x/2 <= X and X <= r.pos.x + r.size.x/2) and
                (r.pos.y-r.size.y/2 <= Y and Y <= r.pos.y + r.size.y/2)) is_inside = true;
        },
        [&](const Triangle& t) {
            double d1, d2, d3;
            d1 = (X - t.vertices[1].x) * (t.vertices[0].y - t.vertices[1].y) - (t.vertices[0].x - t.vertices[1].x) * (Y - t.vertices[1].y);
            d2 = (X - t.vertices[2].x) * (t.vertices[1].y - t.vertices[2].y) - (t.vertices[1].x - t.vertices[2].x) * (Y - t.vertices[2].y);
            d3 = (X - t.vertices[0].x) * (t.vertices[2].y - t.vertices[0].y) - (t.vertices[2].x - t.vertices[0].x) * (Y - t.vertices[0].y);
            is_inside = not (((d1 < 0) or (d2 < 0) or (d3 < 0)) and ((d1 > 0) or (d2 > 0) or (d3 > 0)));
        },
        [&](const Circle& c) {
            Vec a{X-c.pos.x, Y-c.pos.y};
            if (a.norm() <= c.rad) is_inside = true;
        },
    }, shape);

    return is_inside;
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
        [&](Rectangle& r) { r.size+=offset; r.vertices = Shapes::get_vertices(r); Shapes::change_mass(sh); },
        [&](Triangle& t) { t.size+=offset; t.vertices = Shapes::get_vertices(t); Shapes::change_mass(sh); },
        [&](Circle& c) { c.rad+=offset; Shapes::change_mass(sh); },
    }, sh);
}
void Shapes::change_mass(Shape& sh){
    std::visit(overloaded {
        [&](Rectangle& r) { r.mass = r.size.x*r.size.y*r.density; r.inverse_mass = 1.0/r.mass; },
        [&](Triangle& t) { t.mass = t.density*t.size*t.size*3.0*std::pow(3, 0.5)/4.0; t.inverse_mass = 1.0/t.mass; },
        [&](Circle& c) { c.mass = PI*c.rad*c.rad*c.density; c.inverse_mass = 1.0/c.mass;},
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




bool Shapes::intersect(const Shape& sh1, const Shape& sh2, double& depth, Vec& normal){

    bool do_they_intersect = false;

    std::visit(overloaded {
        [&](const Rectangle& r1, const Rectangle& r2) { do_they_intersect = Shapes::intersect_polyXpoly(r1.vertices, r2.vertices, r1.pos, r2.pos, depth, normal); },
        [&](const Rectangle& r, const Triangle& t) { do_they_intersect = Shapes::intersect_polyXpoly(r.vertices, t.vertices, r.pos, t.pos, depth, normal); },
        [&](const Rectangle& r, const Circle& c) { do_they_intersect = Shapes::intersect_polyXcirc(r.vertices, c, r.pos, depth, normal); },
        [&](const Triangle& t, const Rectangle& r) { do_they_intersect = Shapes::intersect_polyXpoly(t.vertices, r.vertices, t.pos, r.pos, depth, normal); },
        [&](const Triangle& t1, const Triangle& t2) { do_they_intersect = Shapes::intersect_polyXpoly(t1.vertices, t2.vertices, t1.pos, t2.pos, depth, normal); },
        [&](const Triangle& t, const Circle& c) { do_they_intersect = Shapes::intersect_polyXcirc(t.vertices, c, t.pos, depth, normal); },
        [&](const Circle& c, const Rectangle& r) { do_they_intersect = Shapes::intersect_polyXcirc(r.vertices, c, r.pos, depth, normal); normal = -normal; }, //divne ja vim, ale bez toho to nefahci
        [&](const Circle& c, const Triangle& t) { do_they_intersect = Shapes::intersect_polyXcirc(t.vertices, c, t.pos, depth, normal); normal = -normal; }, //divne ja vim, ale bez toho to nefahci
        [&](const Circle& c1, const Circle& c2) { do_they_intersect = Shapes::intersect_circXcirc(c1, c2, depth, normal); }
    }, sh1, sh2);

    return do_they_intersect;
}

bool Shapes::intersect_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, const Vec& center1, const Vec& center2, double& depth, Vec& normal){

    depth = std::numeric_limits<double>::infinity();
    double max1, max2, min1, min2;

    for (auto it = verts1.begin(); it!=verts1.end(); ++it){     //iterate over edges of the first polygon

        Vec va{*it};                                                //take a vertex
        Vec vb;                                                     
        if(std::next(it) != verts1.end()) vb = *std::next(it);      //and take its neightbor to get the edge
        else vb = *verts1.begin();
        
        Vec axis = (vb-va).perpendiculate().normalize();            //get the axis we are going to test

        Shapes::project_vertices(verts1, axis, max1, min1);
        Shapes::project_vertices(verts2, axis, max2, min2);

        if ((min1 >= max2) or (min2 >= max1)) return false;         //if both mins were smaller than maxs, the polygons would intersect
        
        double axis_depth = std::min(max2-min1, max1-min2);         //calculate how much they are intersecting

        if (axis_depth<depth){
            depth = axis_depth;
            normal = axis;
        } 
    }

    for (auto it = verts2.begin(); it!=verts2.end(); ++it){     //iterate over edges of the second polygon

        Vec va{*it};                                                //take a vertex
        Vec vb;                                                     
        if(std::next(it) != verts2.end()) vb = *std::next(it);      //and take its neightbor to get the edge
        else vb = *verts2.begin();
        
        Vec axis = (vb-va).perpendiculate().normalize();            //get the axis we are going to test

        Shapes::project_vertices(verts1, axis, max1, min1);
        Shapes::project_vertices(verts2, axis, max2, min2);

        if ((min1 >= max2) or (min2 >= max1)) return false;         //if both mins were smaller than maxs, the polygons would intersect

        double axis_depth = std::min(max2-min1, max1-min2);         //calculate how much they are intersecting

        if (axis_depth<depth){
            depth = axis_depth;
            normal = axis;
        }
    }

    if (normal.dot(center1-center2) < 0.0) normal = -normal;    //check the correct orientation of the normal
    return true;
}


bool Shapes::intersect_polyXcirc(const std::vector<Vec>& verts1, const Circle& c, const Vec& polyg_center, double& depth, Vec& normal){

    depth = std::numeric_limits<double>::infinity();
    double max1, max2, min1, min2;

    for (auto it = verts1.begin(); it!=verts1.end(); ++it){     //iterate over edges of the second polygon

        Vec va{*it};                                                //take a vertex
        Vec vb;                                                     
        if(std::next(it) != verts1.end()) vb = *std::next(it);      //and take its neightbor to get the edge
        else vb = *verts1.begin();

        Vec axis = (vb-va).perpendiculate().normalize();            //get the axis we are going to test

        Shapes::project_vertices(verts1, axis, max1, min1);
        Shapes::project_circle(c, axis, max2, min2);
        
        if ((min1 >= max2) or (min2 >= max1)) return false;         //if both mins were smaller than maxs, the polygons would intersect
        
        double axis_depth = std::min(max2-min1, max1-min2);         //calculate how much they are intersecting

        if (axis_depth<depth){
            depth = axis_depth;
            normal = axis;
        }
    }

    double closest_vertex_distance = std::numeric_limits<double>::infinity();
    auto index = verts1.begin();
    
    for (auto it = verts1.begin(); it!=verts1.end(); ++it){
        double distance = c.pos.distance(*it);
        if (distance < closest_vertex_distance) {
            closest_vertex_distance = distance;
            index = it;
        }
    }

    Vec axis = (*index-c.pos).normalize();                 

    Shapes::project_vertices(verts1, axis, max1, min1);
    Shapes::project_circle(c, axis, max2, min2);
    
    if ((min1 >= max2) or (min2 >= max1)) return false;        

    double axis_depth = std::min(max2-min1, max1-min2);     

    if (axis_depth<depth){      //jestli se to buguje, tak urcite nekde tu je chyba
        depth = axis_depth;
        normal = axis;
        //if(normal.dot(c.pos-polyg_center) < 0.0) normal = -normal;
    }
    if (normal.dot(polyg_center-c.pos) < 0.0) normal = -normal;    //check the correct orientation of the normal

    return true;

}
bool Shapes::intersect_circXcirc(const Circle& c1, const Circle& c2, double& depth, Vec& normal){
    double centers_distance = c1.pos.distance(c2.pos);
    double radii_sum = c1.rad + c2.rad;

    if(centers_distance<radii_sum) {
        depth = radii_sum - centers_distance;
        normal = (c1.pos-c2.pos).normalize();
        return true;
    }
    return false;
}

void Shapes::project_vertices(const std::vector<Vec>& vertices, const Vec& axis, double& max, double& min){

    max = -std::numeric_limits<double>::infinity(); //std::numeric_limits<double>::min(); VRACI NULU!!!!!!
    min = std::numeric_limits<double>::infinity();

    for (const auto& vertex : vertices){
        double projected = vertex.dot(axis);
        if (projected > max) max = projected;
        if (projected < min) min = projected;
    }
}

void Shapes::project_circle(const Circle& c, const Vec& axis, double& max, double& min){

    Vec circle_vector = axis*c.rad;
    Vec p1 = c.pos + circle_vector;
    Vec p2 = c.pos - circle_vector;

    min = p1.dot(axis);
    max = p2.dot(axis);
    if(min>max) std::swap(min, max);
}


void Shapes::handle_collisions(){

    for (auto it = shapes.begin(); it!=shapes.end(); ++it){
        for (auto it2 = std::next(it); it2!=shapes.end(); ++it2){
            double depth = 0.0;
            Vec normal{0.0, 0.0};
            if (Shapes::intersect(*it, *it2, depth, normal)){

                Shapes::move_shape(*it, normal * depth/2.0);
                Shapes::move_shape(*it2, -normal * depth/2.0);  //normaly musi byt minusovane takhle!!!

                Shapes::resolve_collisions(*it, *it2, depth, normal);

                
            }
        }
    }
}

void Shapes::resolve_collisions(Shape& shape1, Shape& shape2, double& depth, Vec& normal) {

    Vec& vel1 = Shapes::get_velocity(shape1);
    Vec& vel2 = Shapes::get_velocity(shape2);
    double inv_m1 = Shapes::get_inverse_mass(shape1);
    double inv_m2 = Shapes::get_inverse_mass(shape2);
    double re1 = Shapes::get_restitution(shape1);
    double re2 = Shapes::get_restitution(shape2);

    Vec relative_vel = vel2 - vel1;

    if(relative_vel.dot(normal) > 0.0) {

        double e = std::min(re1, re2);
        double j = (-(1.0 + e) * relative_vel.dot(normal)) / (inv_m1 + inv_m2);
        Vec impulse = normal * j;
        vel1 -= impulse * inv_m1;
        vel2 += impulse * inv_m2;
    }
}






   


    

   
