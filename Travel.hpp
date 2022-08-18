#ifndef _TH_
#define _TH_
#include "Human.hpp"
#include "Location.hpp"
class Travel
{
private:
    std::string driver_name, status;
    Location* origin;
    Location* destination;
    Passenger* passenger;
    int id;
    bool is_hurry;
    double cost;

public:
    Travel(Location* _origin, Location* _destination, Passenger* _passenger, std::string _status, int _id,std::string hurry, double _cost);
    void receive_driver(std::string name);
    void update_status(std::string new_status);
    std::string get_status();
    std::string get_origin();
    std::string get_des();
    std::string get_pass_name();
    int get_id();
    double get_cost();
};
#endif