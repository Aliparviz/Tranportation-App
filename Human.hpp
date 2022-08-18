#ifndef _HH_
#define _HH_
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include<bits/stdc++.h>
#define comma ','
#define POST "POST"
#define DELETE "DELETE"
#define GET "GET"
#define NOT_FOUND "Not Found"
#define traveling "traveling"
#define finished "finished"
#define BAD_REQUEST "Bad_Request"
#define initial_cost 10000
#define hurry_factor 1.2
#define degree_by_km 110.5
#define PERMISSION_DENIED "Permission Denied"

class Human
{
protected:
    std::string name;
    std::string role;
    bool is_on_the_road;

public:
    std::string getname();
};
class Driver : public Human
{
public:
    Driver(std::string _name, std::string _role);
    void start_new_mission();
    void end_mission();
    bool get_on_the_road();
};
class Passenger : public Human
{
private:
    bool waiting_for_car;

    int travel_id;

public:
    Passenger(std::string _name, std::string _role);
    void receive_id(int id_counter);
    int get_id();
    void start_waiting();
    void end_waiting();
    bool get_waiting();
    void start_tarvel();
    void end_travel();
    bool get_travel();
    void give_id(int id);
};
#endif