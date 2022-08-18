#ifndef _HC_
#define _HC_
#include "Human.hpp"
std::string Human::getname() { return name; }
Driver::Driver(std::string _name, std::string _role)
    {
        name = _name;
        role = _role;
        is_on_the_road = false;
    }
void Driver::start_new_mission() { is_on_the_road = true; }
void Driver::end_mission() { is_on_the_road = false; }
bool Driver::get_on_the_road() { return is_on_the_road; }

Passenger::Passenger(std::string _name, std::string _role)
{
    name = _name;
    role = _role;
    waiting_for_car = false;
    is_on_the_road = false;
    travel_id = 0;
}
void Passenger::receive_id(int id_counter)
{
    travel_id = id_counter;
}
int Passenger::get_id() { return travel_id; }
void Passenger::start_waiting() { waiting_for_car = true; }
void Passenger::end_waiting() { waiting_for_car = false; }
bool Passenger::get_waiting() { return waiting_for_car; }
void Passenger::start_tarvel() { is_on_the_road = true; }
void Passenger::end_travel()
{
    Passenger::is_on_the_road = false;
    Passenger::waiting_for_car = false;
}
bool Passenger::get_travel() { return is_on_the_road; }
void Passenger::give_id(int id) { travel_id = id; }
#endif