#include "../header/header.h"

//loading from and into JSON file for simple object saving

void Shapes::load_to_json() {

    std::string filename;
    std::cout<<"Please enter a desired location to create a savestate in: \n(format: json/\"your_name\".json)"<<std::endl;
    std::cin>>filename;
    
    load_to_json(filename);

}

void Shapes::load_from_json() {

    std::string filename;
    std::cout<<"Please enter a valid json file to load a savestate from:"<<std::endl;
    std::cin>>filename;

    load_from_json(filename);

}


void Shapes::load_to_json(const std::string& filename) {
    
    std::cout<<"Loading shapes to "<<filename<<std::endl;

    Json::Value root(Json::arrayValue);

    for (auto& s : shapes) {
        Json::Value p;  //properties
        Vec vel = get_velocity(s);
        Vec pos = get_position(s);

        std::visit(overloaded {
        [&](Rectangle& r) { p["shape type"] = "Rectangle"; p["size"].append(r.size.x); p["size"].append(r.size.y); },
        [&](Triangle& t) { p["shape type"] = "Triangle"; p["size"] = (t.size); },
        [&](Circle& c) { p["shape type"] = "Circle"; p["radius"] = (c.rad); },
        }, s);
        
        p["position"].append(pos.x);
        p["position"].append(pos.y);
        p["rotation"] = get_rotation(s);
        p["velocity"].append(vel.x);
        p["velocity"].append(vel.y);
        p["rotational velocity"] = get_rot_velocity(s);
        p["density"] = get_density(s);
        p["restitution"] = get_restitution(s);
        p["static friction"] = get_static_friction(s);
        p["dynamic friction"] = get_dynamic_friction(s);
        p["is static?"] = is_static(s);
        p["is selected?"] = is_selected(s);
        root.append(p);
    }

    std::ofstream output_file;
    output_file.open(filename);
    output_file << root;
    std::cout<<"Shapes loaded to "<<filename<<std::endl;
}

void Shapes::load_from_json(const std::string& filename) {

    std::ifstream input_file(filename);
    if (input_file.is_open()) {
        
        std::cout<<"Loading shapes to "<<filename<<std::endl;

        shapes.clear(); //delete all present shapes
        Json::Value root(Json::arrayValue);
        input_file>>root;

        for (const auto& p : root){
            if (p["shape type"] == "Rectangle") {
                shapes.emplace_back(Rectangle{
                    Vec{p["position"][0].asDouble(), p["position"][1].asDouble()},
                    Vec{p["size"][0].asDouble(), p["size"][1].asDouble()},
                    p["density"].asDouble(),
                    p["rotation"].asDouble(),
                    Vec{p["velocity"][0].asDouble(), p["velocity"][1].asDouble()},
                    p["rotational velocity"].asDouble(),
                    p["restitution"].asDouble(),
                    p["static friction"].asDouble(),
                    p["dynamic friction"].asDouble(),
                    false,
                    p["is selected?"].asBool()
                });
            }
            else if (p["shape type"] == "Triangle") {
                shapes.emplace_back(Triangle{
                    Vec{p["position"][0].asDouble(), p["position"][1].asDouble()},
                    p["size"].asDouble(),
                    p["density"].asDouble(),
                    p["rotation"].asDouble(),
                    Vec{p["velocity"][0].asDouble(), p["velocity"][1].asDouble()},
                    p["rotational velocity"].asDouble(),
                    p["restitution"].asDouble(),
                    p["static friction"].asDouble(),
                    p["dynamic friction"].asDouble(),
                    false,
                    p["is selected?"].asBool()
                });
            }
            else {
                shapes.emplace_back(Circle{
                    Vec{p["position"][0].asDouble(), p["position"][1].asDouble()},
                    p["radius"].asDouble(),
                    p["density"].asDouble(),
                    p["rotation"].asDouble(),
                    Vec{p["velocity"][0].asDouble(), p["velocity"][1].asDouble()},
                    p["rotational velocity"].asDouble(),
                    p["restitution"].asDouble(),
                    p["static friction"].asDouble(),
                    p["dynamic friction"].asDouble(),
                    false,
                    p["is selected?"].asBool()
                });
            }

            //if shape is static use the function instead of constructor to handle inverse values of mass and rotational inertia
            if (p["is static?"].asBool() and shapes.size()>0) toggle_static(shapes[shapes.size()-1]);
        }

        std::cout<<"Savestate from "<<filename<<" successfully loaded to screen"<<std::endl;
    }

    else std::cout<<filename<<" could not be opened."<<std::endl;
}