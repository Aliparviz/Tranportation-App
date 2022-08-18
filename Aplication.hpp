#ifndef _AH_
#define _AH_
#include "Travel.hpp"
#include "Human.hpp"
#include "Location.hpp"
class Application
{
private:
    std::vector<Driver *> drivers;
    std::vector<Passenger *> passengers;
    std::vector<Location *> locations;
    std::vector<Travel *> travels;
    int id_counter = 0;

public:
    void add_id();
    void fill_locations(std::string line);
    void read_file(char *path);
    Passenger* find_passenger_by_name(std::string username);
    Driver* find_driver_by_name(std::string driver_name);
    Travel* find_travel_by_id(int id);
    Travel* find_travel_by_org_and_dest(Location* destination,Location* origin);
    Location* find_location_by_name(std::string name);
    bool check_new(std::string username);
    double cal_price_of_travel(Location* origin, Location* destination, bool is_hurry);
    bool check_standard_appeal(std::string passenger, std::string origin, std::string destination);
    void sign_up(std::vector<std::string> splited);
    void post_trip(std::vector<std::string> splited);
    bool check_accept(Driver *driver, Travel *travel);
    void accept_travel(std::vector<std::string> splited);
    bool check_finishing(Driver *driver, Travel *travel);
    void finish_travel(std::vector<std::string> splited);
    void handle_post_commands(std::vector<std::string> splited);
    bool check_get_trip(Human *driver);
    static bool sort_id(Travel* one, Travel* two);
    static bool sort_cost(Travel* one, Travel* two);
    void print_travels_by_cost();
    void print_travels_by_id();
    void get_all_trips(std::vector<std::string> splited);
    bool check_one_trip(Driver *driver, Travel *travel);
    void get_one_trip(std::vector<std::string> splited);
    bool check_get_cost(Location *destination, Location *origin, Human *passenger, Human *driver);
    void get_cost_of_trip(std::vector<std::string> splited);
    void handle_get_commands(std::vector<std::string> splited);
    bool check_delete_trip(Passenger *passenger, Travel *travel);
    void delete_trip(std::vector<std::string> splited);
    void handle_delete_commands(std::vector<std::string> splited);
    std::vector<std::string> split_line_by_spaces(std::string line);
    void manage_commands();
};
#endif