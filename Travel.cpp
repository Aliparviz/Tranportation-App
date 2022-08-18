#ifndef _TC_
#define _TC_
#include "Travel.hpp"
Travel::Travel(Location* _origin, Location* _destination, Passenger* _passenger, std::string _status, int _id,std::string hurry, double _cost)
    {
        origin = _origin;
        destination = _destination;
        passenger = _passenger;
        status = _status;
        id = _id;
        if(hurry == "yes")
        {
            is_hurry = true;
        }
        else
        {
            is_hurry = false;
        }
        cost = _cost;
    }
    void Travel::receive_driver(std::string name) { driver_name = name; }
    void Travel::update_status(std::string new_status) { status = new_status; }
    std::string Travel::get_status() { return status; }
    std::string Travel::get_origin() { return origin->getname(); }
    std::string Travel::get_des() { return destination->getname(); }
    std::string Travel::get_pass_name() { return passenger->getname(); }
    int Travel::get_id() { return id; }
    double Travel::get_cost() {return cost; }
#endif