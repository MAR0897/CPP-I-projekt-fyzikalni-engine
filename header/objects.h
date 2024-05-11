#pragma once

#include "include.h"
#include "vec2D.h"


//=============================================================================================================================================
//Variadic templates for easy std::variant access to different types of geometric shapes
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

//=============================================================================================================================================
//Axis-aligned bounding box (box around points of the shapes that have min and max value in relation to x and y axis)
struct AABB{

    Vec min;
    Vec max;

    AABB() = default;
    AABB(const Vec& min, const Vec& max);
    AABB(Vec&& min, Vec&& max);
    AABB(const double& minX, const double& maxX, const double& minY, const double& maxY);
    AABB(double&& minX, double&& maxX, double&& minY, double&& maxY);
};

//=============================================================================================================================================
//Physical properties of geometric shapes

struct Properties{

    double mass;
    double inverse_mass;        //inverse value of mass for easier calculations
    double density; 
    double restitution;         //vzpruživost - koeficient restituce - pružnost, ale co se týče odrazů - od 0 do 1
    double rot_inertia;         //moment setrvačnosti (takový ten hezký tenzor, který tady naštěstí tenzorem nebude)
    double inverse_rot_inertia; //inverse value of rotational inertia for easier calculations
    double friction_s;          //statické tření 
    double friction_d;          //dynamické tření – tření za pohybu

    Vec pos;                    //center of mass = teziste
    double rot;                 //rotation of an object (0 to 2PI rad)

    Vec vel;                    //velocity  
    double rotvel;              //angular velocity; omega = 2pi*frequency (+ = proti smeru rucicek, - = po smeru rucicek)

    Vec force{0.0, 0.0};        //force applied to the object every iteration

    bool is_static = false;     //object is immovable and black with white margin if static
    bool is_selected = false;   //object is red if selected

    Properties(const double& m, const double& den,
            const Vec& position, const double& rotation, 
            const Vec& velocity, const double& rv,
            const double& rest, const double& sf, const double& df,
            const bool& is_static, const bool& is_selected
            );
    Properties(double&& m, double&& den,
            Vec&& position, double&& rotation, 
            Vec&& velocity, double&& rv,
            double&& rest, double&& sf, double&& df,
            bool&& is_static, bool&& is_selected
            );
};

//=============================================================================================================================================
//Geometric shapes - Rectangle, Triangle and Circle

namespace physics{
    extern const double RESTITUTION;
    extern const double STATIC_FRICTION;
    extern const double DYNAMIC_FRICTION;
}

struct Rectangle : public Properties {

    Vec size; //size.x = a side lenght; size.y = b side lenght
    std::vector<Vec> vertices; 
    std::vector<Vec> static_vertices; //pro hezke vykresleni nehnutelnych objektu
    AABB box;

    Rectangle(const Vec& position, const Vec& size, const double& den = 1.0, const double& rotation = 0.0, 
            const Vec& velocity = Vec{0.0,0.0}, const double& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            const bool& is_static = false, const bool& is_selected = false);
    Rectangle(Vec&& position, const Vec&& size, double&& den = 1.0, double&& rotation = 0.0, 
            Vec&& velocity = Vec{0.0,0.0}, double&& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            bool&& is_static = false, bool&& is_selected = false);
};

struct Triangle : public Properties {

    double size;   //distance from center of mass to a vertex (lets just go with rovnostranne trojuhelniky)
    std::vector<Vec> vertices; 
    std::vector<Vec> static_vertices; //pro hezke vykresleni nehnutelnych objektu
    AABB box;

    Triangle(const Vec& position, const double& size, const double& den = 1.0, const double& rotation = 0.0, 
            const Vec& velocity = Vec{0.0,0.0}, const double& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            const bool& is_static = false, const bool& is_selected = false);
    Triangle(Vec&& position, const double&& size, double&& den = 1.0, double&& rotation = 0.0, 
            Vec&& velocity = Vec{0.0,0.0}, double&& rv = 0.0,
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            bool&& is_static = false, bool&& is_selected = false);
};

struct Circle : public Properties {

    double rad; //radius of the circle
    AABB box;

    Circle(const Vec& position, const double& radius, const double& den = 1.0, const double& rotation = 0.0, 
            const Vec& velocity = Vec{0.0,0.0}, const double& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            const bool& is_static = false, const bool& is_selected = false);
    Circle(Vec&& position, const double&& radius, double&& den = 1.0, double&& rotation = 0.0, 
            Vec&& velocity = Vec{0.0,0.0}, double&& rv = 0.0, 
            const double& rest = physics::RESTITUTION, const double& sf = physics::STATIC_FRICTION, const double& df = physics::DYNAMIC_FRICTION,
            bool&& is_static = false, bool&& is_selected = false);
};

//=============================================================================================================================================
//Shape - std::variant of geometric shapes above

using Shape = std::variant<Rectangle, Triangle, Circle>;

struct Collisions;

struct Shapes{

    std::vector<Shape> shapes;  //shapes handle

    //--------------------------------------------------------------------------------
    //constructors, adding and deleting a shape
        Shapes() = default;
        Shapes(const std::vector<Shape>& sh);
        Shapes(std::vector<Shape>&& sh);
        void add_shape(const Shape& sh);
        void add_shape(Shape&& sh);
        void delete_shape(const Vec& mousepos);
        void delete_selected();
        void delete_out_of_screen();

    //--------------------------------------------------------------------------------
    //loading from and into JSON file for simple object saving (will ask for filename)
        void load_to_json();
        void load_from_json();
        void load_to_json(const std::string& filename);
        void load_from_json(const std::string& filename);

    //--------------------------------------------------------------------------------
    //getters and checkers
        //get velocity of the shape
        static Vec& get_velocity(Shape& shape);
        //get rotational velocity of the shape
        static double& get_rot_velocity(Shape& shape);
        //get mass of the shape
        static double get_mass(const Shape& shape);
        //get density of the shape
        static double get_density(const Shape& shape);
        //get inverse mass of the shape
        static double get_inverse_mass(const Shape& shape);
        //get the inverse rotational inertia for impulse calculation
        static double get_inverse_rot_inertia(const Shape& shape);
        //get coefficient of restitution of the shape
        static double get_restitution(const Shape& shape);
        //get static friction coefficient of the shape
        static double get_static_friction(const Shape& shape);
        //get dynamic friction coefficient of the shape
        static double get_dynamic_friction(const Shape& shape);
        //get shape center (position)
        static Vec get_position(const Shape& shape);
        //get the rotation of the shape (0-360deg)
        static double get_rotation(const Shape& shape);
        //get the axis aligned bounding box
        static AABB get_AABB(const Shape& shape);
        //checks if given mouse coordinates are inside the shape
        static bool is_inside(const Vec& mousepos, const Shape& shape);

    //--------------------------------------------------------------------------------
    //functions for shape selecting
        //whether a certain shape has is_selected value true
        static bool is_selected(const Shape& shape);
        //select or deleselt shape
        static void toggle_selected(Shape& shape);
        //drag cursor to select all shapes that have their AABB within dragged box
        void swipe_select(const Vec& swipe_start, const Vec& swipe_end);
        //deselect all selected shapes
        void deselect();

    //--------------------------------------------------------------------------------
    //functions for dealing with static objects
        //whether a certain shape has is_static value true, which means that its immovable
        static bool is_static(const Shape& shape);
        //set the shape as static
        static void toggle_static(Shape& shape);
            //inner helper function to avoid copying code
            template<typename S>
            static void m_toggle_static(S& s);

    //--------------------------------------------------------------------------------
    //drawing shapes and text on the screen
        //drawing a single shape
        static void draw(const Shape& shape);
        //drawing a static single shape (black with white outline)
        static void draw_static(const Shape& shape);
        //drawing all the shapes in the "shapes" vector
        void draw_all_shapes() const;
        //renderes some statistics in text on the screen of only 1 selected shape!! 
        //(if more are selected, takes only the first selected in the vector of shapes)
        void render_statistics(double x, double y, void* font);
            //inner helper function for easier access to properties
            template<typename S>
            void m_render_statistics(S& s, double x, double y, void* font); 


    //--------------------------------------------------------------------------------
    //helper computing functions for defining the exact shape of an object
        //computes the vertices from a given shape
        static std::vector<Vec> get_vertices(const Shape& shape);
        //creates the axis aligned bounding box
        static AABB create_AABB(const Shape& shape);
            //inner helper function to avoid copying code
            template<typename S>
            static AABB m_create_AABB(const S& s);

    //--------------------------------------------------------------------------------
    //functions for changing shape size
        //changes the size
        static void resize_shape(Shape& sh, const double& offset);
            //inner helper function to avoid copying code
            template<typename S, typename doubleORvec>
            static void m_resize_shape(S& s, const double& offset, doubleORvec& to_resize);
        //change the mass due to resizing
        template<typename S>
        static void change_mass(S& s);
        //change the rotational inertia due to resizing
        template<typename S>
        static void change_rot_inertia(S& s);

    //-------------------------------------------------------------------------------- 
    //shape movement
        //moves the shape by following the cursor
        static void move_shape_to(Shape& sh, const Vec& offset);
        //moves the shape by some value
        static void move_shape(Shape& sh, const Vec& offset);
            //move shape by some offset or move shape to by some offset depending on the operation
            template<typename S>
            static void m_move_shape(S& s, const Vec& offset, auto operation);
        //push a single shape
        static void add_force(Shape& shape, const Vec& force);

    //--------------------------------------------------------------------------------  
    //shape rotation
        //rotate shape
        static void rotate_shape(Shape& sh, const double& angle);
            //inner helper function
            template<typename S>
            static void m_rotate_shape(S& s, const double& angle, auto operation); 
        //change the rotational velocity
        static void add_spin(Shape& sh, const double& speed);

    //-------------------------------------------------------------------------------- 
    //physical step helper functions 
        //update physical properties (position, velocity) by delta time
        //update position with s = v * t
        void update_position_and_rotation(const double& delta);
        //update velocity with v = F/m * t
        void update_by_force(const double& delta);
        //update velocity with v = a * t (mainly or solely used for gravitation)
        void update_by_acceleration(const double& delta, const Vec& acceleration);

    //-------------------------------------------------------------------------------- 
    //intersecting and collisions
        //main function in the physical step - checks if any shapes are intersecting and resolve that intersection by applying resolve_collision()
        void handle_collisions();
        //whether two shapes intersect
        static bool intersect(const Shape& sh1, const Shape& sh2, double& depth, Vec& normal);
            //inner intersect function for executing the correct intersect variant (polyXpoly, polyXcirc, circXcirc)
            template<typename S1, typename S2>
            static bool m_intersect(const S1& s1, const S2& s2, double& depth, Vec& normal);
            //Polygon x polygon intersection check
            static bool intersect_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, const Vec& center1, const Vec& center2, double& depth, Vec& normal);
                //inner helper function
                static bool m_intersect_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, double& depth, Vec& normal, double& max1, double& max2, double& min1, double& min2);
            //Polygon x circle intersection check
            static bool intersect_polyXcirc(const std::vector<Vec>& verts1, const Circle& c, const Vec& polyg_center, double& depth, Vec& normal);
            //Circle x circle intersection check
            static bool intersect_circXcirc(const Circle& c1, const Circle& c2, double& depth, Vec& normal);
            //Intersect check for AABBs of 2 shapes (easier to calculate than the intersecting functions above)
            static bool intersect_aabbXaabb(const AABB& aabb1, const AABB& aabb2);
            //Projecting vertices of both shapes to find if they intersect (Separating axis theorem)
            static void project_vertices(const std::vector<Vec>& vertices, const Vec& axis, double& max, double& min);
            //converting circle into two vectices to allow projecting then onto the separating axis
            static void project_circle(const Circle& c, const Vec& axis, double& max, double& min);
        //advanced formula for resolving collisions with rotation and friction
        static void resolve_collisions(Collisions& contact);
};

//shape handle representing all the shapes on the screen
extern Shapes sh;       //user placed shape handles
extern Shapes bgsh;     //background shape handles 


//=============================================================================================================================================
//Collisions struct for storing contact points between every pair of intersecting shapes

struct Collisions{

    //shapes the collision belongs to
    Shape& sh1;
    Shape& sh2;

    //how much they are intersecting and in which direction they are gonna bounce off
    double depth;
    Vec normal;

    //contact points
    Vec contact1;
    Vec contact2;
    int contact_count;

    Collisions(Shape& sh1, Shape& sh2, const double& depth, const Vec& normal, const Vec& contact1, const Vec& contact2, const int& contact_count);
    Collisions(Shape& sh1, Shape& sh2, double&& depth, Vec&& normal, Vec&& contact1, Vec&& contact2, int&& contact_count);
    
    //all pair of intersecting shapes
    static std::vector<Collisions> contacts;

    //finds where do the colliding shapes contact
    static void find_contact_points(const Shape& sh1, const Shape& sh2, Vec& c_point1, Vec& c_point2, int& c_points_count);
        template<typename S1, typename S2>
        static void m_find_contact_points(const S1& s1, const S2& s2, Vec& c_point1, Vec& c_points2, int& c_points_count);

        //find contact points for two intersecting circles
        static void fcp_circXcirc(const Circle& c1, const Circle& c2, Vec& c_point1, int& c_points_count);
        //find contact points for a polygon and a circle
        static void fcp_polyXcirc(const Circle& c, const std::vector<Vec>& verts, Vec& c_point1, int& c_points_count);
        //find contact points for two intersecting polygons
        static void fcp_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, Vec& c_point1, Vec& c_point2, int& c_points_count);
            //inner helper function for finding contact points for two polygons
            static void m_fcp_polyXpoly(const std::vector<Vec>& verts1, const std::vector<Vec>& verts2, Vec& c_point1, Vec& c_point2, int& c_points_count, Vec& contact, double& min_distance);
        //geometrical distance from a single point to a line (segment)
        static double point_segment_distance(const Vec& p, const Vec& va, const Vec& vb, Vec& contact_point);

};