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

using namespace std;
class Human
{
protected:
    string name;
    string role;
    bool is_on_the_road;

public:
    string getname() { return name; }
};
class Driver : public Human
{
public:
    Driver(string _name, string _role)
    {
        name = _name;
        role = _role;
        is_on_the_road = false;
    }
    void start_new_mission() { is_on_the_road = true; }
    void end_mission() { is_on_the_road = false; }
    bool get_on_the_road() { return is_on_the_road; }
};
class Passenger : public Human
{
private:
    bool waiting_for_car;

    int travel_id;

public:
    Passenger(string _name, string _role)
    {
        name = _name;
        role = _role;
        waiting_for_car = false;
        is_on_the_road = false;
        travel_id = 0;
    }
    void receive_id(int id_counter)
    {
        travel_id = id_counter;
    }
    int get_id() { return travel_id; }
    void start_waiting() { waiting_for_car = true; }
    void end_waiting() { waiting_for_car = false; }
    bool get_waiting() { return waiting_for_car; }
    void start_tarvel() { is_on_the_road = true; }
    void end_travel()
    {
        is_on_the_road = false;
        waiting_for_car = false;
    }
    bool get_travel() { return is_on_the_road; }
    void give_id(int id) { travel_id = id; }
};

class Location
{
private:
    string name;
    double latitude;
    double longitude;
    int terrafic_degree;

public:
    Location(string _name, double lat, double longi, int terrafic)
    {
        name = _name;
        latitude = lat;
        longitude = longi;
        terrafic_degree = terrafic;
    }
    string getname() { return name; }
    double getlat() { return latitude; }
    double getlong() { return longitude; }
    int getdrgree() { return terrafic_degree; }
};

class Travel
{
private:
    string driver_name, status;
    Location* origin;
    Location* destination;
    Passenger* passenger;
    int id;
    bool is_hurry;
    double cost;

public:
    Travel(Location* _origin, Location* _destination, Passenger* _passenger, string _status, int _id,string hurry, double _cost)
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
    void receive_driver(string name) { driver_name = name; }
    void update_status(string new_status) { status = new_status; }
    string get_status() { return status; }
    string get_origin() { return origin->getname(); }
    string get_des() { return destination->getname(); }
    string get_pass_name() { return passenger->getname(); }
    int get_id() { return id; }
    double get_cost() {return cost; }
};
class application
{
private:
    vector<Driver *> drivers;
    vector<Passenger *> passengers;
    vector<Location *> locations;
    vector<Travel *> travels;
    int id_counter = 0;

public:
    void add_id() { id_counter++; }
    void fill_locations(string line)
    {
        vector<string> info;
        string word;
        stringstream s(line);
        while (getline(s, word, ','))
        {
            info.push_back(word);
        }
        locations.push_back(new Location(info[0], stof(info[1]), stof(info[2]), stoi(info[3])));
    }
    void read_file(char *path)
    {
        fstream inpt;
        string line;
        inpt.open(path, ios::in);
        getline(inpt, line); // skip first line
        line.clear();
        while (getline(inpt, line))
        {
            fill_locations(line);
        }
        inpt.close();
    }
    Passenger *find_passenger_by_name(string username)
    {
        for (int i = 0; i < passengers.size(); i++)
        {
            if (passengers[i]->getname() == username)
            {
                return passengers[i];
            }
        }
        return NULL;
    }
    Driver *find_driver_by_name(string driver_name)
    {
        for (int i = 0; i < drivers.size(); i++)
        {
            if (drivers[i]->getname() == driver_name)
            {
                return drivers[i];
            }
        }
        return NULL;
    }
    Travel *find_travel_by_id(int id)
    {
        for (int i = 0; i < travels.size(); i++)
        {
            if (travels[i]->get_id() == id)
            {
                return travels[i];
            }
        }
        return NULL;
    }
    Travel *find_travel_by_org_and_dest(Location* destination,Location* origin)
    {
        cout<<destination->getname()<<" "<<origin->getname()<<endl;
        cout<<travels.size();
        for(int i=0; i<travels.size(); i++)
        {
            cout<<travels[i]->get_des()<<" "<<travels[i]->get_origin()<<endl;
            if((travels[i]->get_des() == destination->getname()) && (travels[i]->get_origin() == origin->getname()))
            {
                return travels[i];
            }
        }
        return NULL;
    }
    Location *find_location_by_name(string name)
    {
        for (int i = 0; i < locations.size(); i++)
        {
            if (locations[i]->getname() == name)
            {
                return locations[i];
            }
        }
        return NULL;
    }
    bool check_new(string username)
    {
        try
        {
            for (int i = 0; i < drivers.size(); i++)
            {
                if (drivers[i]->getname() == username)
                {
                    throw runtime_error("Bad Request");
                }
            }
            for (int i = 0; i < passengers.size(); i++)
            {
                if (passengers[i]->getname() == username)
                {
                    throw runtime_error("Bad Request");
                }
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
            return false;
        }
        return true;
    }
    double cal_price_of_travel(Location* origin, Location* destination, bool is_hurry)
    {
        double lat1 = origin->getlat();
        double lat2 = destination->getlat();
        double long1 = origin->getlong();
        double long2 = destination->getlong();
        double terr1 = origin->getdrgree();
        double terr2 = origin-> getdrgree();
      //  cout<<sqrt((pow((lat2 - lat1), 2)) + (pow((long2 - long2), 2)))<<endl;
        double distance = degree_by_km * sqrt((pow((lat2 - lat1), 2)) + (pow((long1 - long2), 2)));
        double cost = distance * (terr1 + terr2)*initial_cost;
        if (is_hurry == true)
        {
            cost *= hurry_factor;
        }
        return cost;
    }
    bool check_standard_appeal(string passenger, string origin, string destination)
    {
        bool check_passenger = false;
        bool check_origin = false;
        bool check_des = false;
        bool ready_to_appeal = true;
        for (int i = 0; i < passengers.size(); i++)
        {
            if (passengers[i]->getname() == passenger)
            {
                check_passenger = true;
                if (passengers[i]->get_travel() == true || passengers[i]->get_waiting() == true)
                {
                    ready_to_appeal = false;
                }
                break;
            }
        }
        for (int i = 0; i < locations.size(); i++)
        {
            if (locations[i]->getname() == origin)
            {
                check_origin = true;
                break;
            }
        }
        for (int i = 0; i < locations.size(); i++)
        {
            if (locations[i]->getname() == destination)
            {
                check_des = true;
                break;
            }
        }
        try
        {
            if (passenger == "" || origin == "" || destination == "" || ready_to_appeal == false)
            {
                throw runtime_error("Bad Request");
            }
            if (check_passenger == false || check_origin == false || check_des == false)
            {
                throw runtime_error(NOT_FOUND);
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
            return false;
        }
        return true;
    }
    void sign_up(vector<string> splited)
    {
        string username, role;
        if (splited[3] == "username")
        {
            username = splited[4];
            role = splited[6];
        }
        else if (splited[3] == "role")
        {
            username = splited[6];
            role = splited[4];
        }
        if (check_new(username) == true)
        {
            if (role == "passenger")
            {
                passengers.push_back(new Passenger(username, role));
            }
            else if (role == "driver")
            {
                drivers.push_back(new Driver(username, role));
            }
            cout << "OK" << endl;
        }
    }
    void post_trip(vector<string> splited)
    {
        string check_in_hurry, user_name, dest, org;
        bool is_hurry = false;
        if (splited[3] == "username")
        {
            user_name = splited[4];
            if (splited[5] == "origin")
            {
                org = splited[6];
                if (splited[7] == "destination")
                {
                    dest = splited[8];
                    check_in_hurry = splited[10];
                }
                else if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    dest = splited[10];
                }
            }
            else if (splited[5] == "destination")
            {
                dest = splited[6];
                if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    org = splited[10];
                }
                else if (splited[7] == "origin")
                {
                    org = splited[8];
                    check_in_hurry = splited[10];
                }
            }
            else if (splited[5] == "in_hurry")
            {
                check_in_hurry = splited[6];
                if (splited[7] == "origin")
                {
                    org = splited[8];
                    dest = splited[10];
                }
                else if (splited[7] == "destination")
                {
                    dest = splited[8];
                    org = splited[10];
                }
            }
        }
        else if (splited[3] == "origin")
        {
            org = splited[4];
            if (splited[5] == "username")
            {
                user_name = splited[6];
                if (splited[7] == "destination")
                {
                    dest = splited[8];
                    check_in_hurry = splited[10];
                }
                else if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    dest = splited[10];
                }
            }
            else if (splited[5] == "destination")
            {
                dest = splited[6];
                if (splited[7] == "username")
                {
                    user_name = splited[8];
                    check_in_hurry = splited[10];
                }
                else if (splited[7] == "in_hurry")
                {
                    user_name = splited[10];
                    check_in_hurry = splited[8];
                }
            }
            else if (splited[5] == "in_hurry")
            {
                check_in_hurry = splited[6];
                if (splited[7] == "username")
                {
                    user_name = splited[8];
                    dest = splited[10];
                }
                if (splited[7] == "destination")
                {
                    user_name = splited[10];
                    dest = splited[8];
                }
            }
        }
        else if (splited[3] == "destination")
        {
            dest = splited[4];
            if (splited[5] == "username")
            {
                if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    org = splited[10];
                }
                else if (splited[7] == "origin")
                {
                    check_in_hurry = splited[10];
                    org = splited[8];
                }
            }
            else if (splited[5] == "origin")
            {
                org = splited[6];
                if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    user_name = splited[10];
                }
                else if (splited[7] == "username")
                {
                    check_in_hurry = splited[10];
                    user_name = splited[8];
                }
            }
            else if (splited[5] == "in_hurry")
            {
                check_in_hurry = splited[6];
                if (splited[7] == "username")
                {
                    user_name = splited[8];
                    org = splited[10];
                }
                else if (splited[7] == "origin")
                {
                    user_name = splited[10];
                    org = splited[8];
                }
            }
        }
        else if (splited[3] == "in_hurry")
        {
            check_in_hurry = splited[4];
            if (splited[5] == "uesrname")
            {
                user_name = splited[6];
                if (splited[7] == "destination")
                {
                    dest = splited[8];
                    org = splited[10];
                }
                else if (splited[7] == "origin")
                {
                    dest = splited[10];
                    org = splited[8];
                }
            }
            else if (splited[5] == "origin")
            {
                org = splited[6];
                if (splited[7] == "destination")
                {
                    dest = splited[8];
                    user_name = splited[10];
                }
                if (splited[7] == "username")
                {
                    dest = splited[10];
                    user_name = splited[8];
                }
            }
            else if (splited[5] == "destination")
            {
                dest = splited[6];
                if (splited[7] == "origin")
                {
                    org = splited[8];
                    user_name = splited[10];
                }
                else if (splited[7] == "username")
                {
                    org = splited[10];
                    user_name = splited[8];
                }
            }
        }
        if(check_in_hurry == "yes")
        {
            is_hurry = true;
        } 
        Location* origin = find_location_by_name(org); 
        Location* destination = find_location_by_name(dest);
        Passenger* _passenger = find_passenger_by_name(user_name);
        if (check_standard_appeal(user_name, org, dest) == true)
        {
            add_id();
            _passenger->start_waiting();
            _passenger->receive_id(id_counter);
            travels.push_back(new Travel(origin, destination, _passenger, "waiting", id_counter, check_in_hurry,cal_price_of_travel(origin, destination, is_hurry)));
            cout << _passenger->get_id() << endl;
        }
    }
    bool check_accept(Driver *driver, Travel *travel)
    {
        try
        {
            if (driver->get_on_the_road() == true || travel->get_status() != "waiting")
            {
                throw runtime_error(BAD_REQUEST);
            }
            else if (travel == NULL)
            {
                throw runtime_error(NOT_FOUND);
            }
            else if (driver == NULL)
            {
                throw runtime_error(PERMISSION_DENIED);
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
            return false;
        }
        return true;
    }
    void accept_travel(vector<string> splited)
    {
        string username;
        int id;
        if (splited[3] == "username")
        {
            username = splited[4];
            id = stoi(splited[6]);
        }
        else if (splited[3] == "id")
        {
            id = stoi(splited[4]);
            username = splited[6];
        }
        Driver *driver = find_driver_by_name(username);
        Travel *travel = find_travel_by_id(id);
        if (check_accept(driver, travel) == true)
        {
            driver->start_new_mission();
            travel->update_status("traveling");
            travel->receive_driver(username);
            cout << "OK" << endl;
        }
    }
    bool check_finishing(Driver *driver, Travel *travel)
    {
        try
        {
            if (driver->get_on_the_road() == false)
            {
                throw runtime_error(BAD_REQUEST);
            }
            else if (travel == NULL)
            {
                throw runtime_error(NOT_FOUND);
            }
            else if (driver == NULL)
            {
                throw runtime_error(PERMISSION_DENIED);
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
            return false;
        }
        return true;
    }
    void finish_travel(vector<string> splited)
    {
        string username;
        int id;
        if (splited[3] == "username")
        {
            username = splited[4];
            id = stoi(splited[6]);
        }
        else if (splited[3] == "id")
        {
            id = stoi(splited[4]);
            username = splited[6];
        }
        Driver *driver = find_driver_by_name(username);
        Travel *travel = find_travel_by_id(id);
        if (check_finishing(driver, travel) == true)
        {
            driver->end_mission();
            travel->update_status("finished");
            cout << "OK" << endl;
        }
    }
    void handle_post_commands(vector<string> splited)
    {
        try
        {
            if (splited.size() < 2)
            {
                throw runtime_error(NOT_FOUND);
            }
            string post_command = splited[1];
            if (post_command == "signup" && splited.size() == 7)
            {
                sign_up(splited);
            }
            else if (post_command == "trips" && splited.size() == 11)
            {
                post_trip(splited);
            }
            else if (post_command == "accept" && splited.size() == 7)
            {
                accept_travel(splited);
            }
            else if (post_command == "finish" && splited.size() == 7)
            {
                finish_travel(splited);
            }
            else
            {
                throw runtime_error(NOT_FOUND);
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
        }
    }
    bool check_get_trip(Human *driver)
    {
        try
        {
            if (driver == NULL)
            {
                throw runtime_error(PERMISSION_DENIED);
            }
            if (travels.size() == 0)
            {
                throw runtime_error("Empty");
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
            return false;
        }
        return true;
    }
    static bool sort_id(Travel* one, Travel* two)
    {
        return (one->get_id() < two->get_id());
    }
    static bool sort_cost(Travel* one, Travel* two)
    {
        return (one->get_cost() > two->get_cost());
    }
    void print_travels_by_cost()
    {
        sort(travels.begin() , travels.end(),application::sort_cost);

        for(int i=0; i<travels.size(); i++)
        {
            Travel* temp = travels[i];
            cout << temp->get_id() << " " << temp->get_pass_name() << " " << temp->get_origin() << " " << temp->get_des() << " " ;
            cout<<fixed<<setprecision(2)<<temp->get_cost()<<" ";
            cout<< temp->get_status() << endl;
        }
        
    }
    void print_travels_by_id()
    {
        sort(travels.begin() , travels.end(),application::sort_id);
        for(int i=0; i<travels.size(); i++)
        {
            Travel* temp = travels[i];
            cout << temp->get_id() << " " << temp->get_pass_name() << " " << temp->get_origin() << " " << temp->get_des() << " " ;
            cout<<fixed<<setprecision(2)<<temp->get_cost()<<" ";
            cout<< temp->get_status() << endl;
        }
    }
    void get_all_trips(vector<string> splited)
    {
        string driver_name,sort_by_cost;
        bool is_sort_by_cost;
        if(splited[3] == "username")
        {
            driver_name = splited[4];
            sort_by_cost = splited[6];
        }
        else if(splited[3] == "sort_by_cost")
        {
            driver_name = splited[6];
            sort_by_cost = splited[4];
        }
        if(sort_by_cost == "yes")
        {
            is_sort_by_cost = true;
        }
        else if(sort_by_cost == "no")
        {
            is_sort_by_cost = false;
        }
        Driver *driver = find_driver_by_name(driver_name);
        if (check_get_trip(driver) == true)
        {
            if(is_sort_by_cost == true)
            {
                print_travels_by_cost();
            }
            else
            {
                print_travels_by_id();
            }
        }
    }
    bool check_one_trip(Driver *driver, Travel *travel)
    {
        try
        {
            if (driver == NULL)
            {
                throw runtime_error("Permission Denied");
            }
            else if (travel == NULL || travel->get_status() == "canceled")
            {
                throw runtime_error("Not Found");
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
            return false;
        }
        return true;
    }
    void get_one_trip(vector<string> splited)
    {
        string driver_name;
        int id;
        if(splited[3] == "username")
        {
            driver_name = splited[4];
            id = stoi(splited[6]);
        }
        else if(splited[3] == "id")
        {
            driver_name = splited[6];
            id = stoi(splited[4]);
        }
        
        Driver *driver = find_driver_by_name(driver_name);
        Travel *travel = find_travel_by_id(id);
        if (check_one_trip(driver, travel) == true)
        {
            cout << travel->get_id() << " " << travel->get_pass_name() << " " << travel->get_origin() << " " << travel->get_des() << " " ;
            cout<<fixed<<setprecision(2)<<travel->get_cost()<<" ";
            cout<< travel->get_status() << endl;
        }
    }
    bool check_get_cost(Location *destination, Location *origin, Human *passenger, Human *driver)
    {
        try
        {
            if (destination == NULL || origin == NULL)
            {
                throw runtime_error(NOT_FOUND);
            }
            else if (passenger == NULL && driver == NULL)
            {
                throw runtime_error(NOT_FOUND);
            }
            else if (passenger == NULL || driver != NULL)
            {
                throw runtime_error(BAD_REQUEST);
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
            return false;
        }
        return true;
    }
    void get_cost_of_trip(vector<string> splited)
    {
        string check_in_hurry, user_name, dest, org;
        bool is_hurry = false;
        if (splited[3] == "username")
        {
            user_name = splited[4];
            if (splited[5] == "origin")
            {
                org = splited[6];
                if (splited[7] == "destination")
                {
                    dest = splited[8];
                    check_in_hurry = splited[10];
                }
                else if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    dest = splited[10];
                }
            }
            else if (splited[5] == "destination")
            {
                dest = splited[6];
                if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    org = splited[10];
                }
                else if (splited[7] == "origin")
                {
                    org = splited[8];
                    check_in_hurry = splited[10];
                }
            }
            else if (splited[5] == "in_hurry")
            {
                check_in_hurry = splited[6];
                if (splited[7] == "origin")
                {
                    org = splited[8];
                    dest = splited[10];
                }
                else if (splited[7] == "destination")
                {
                    dest = splited[8];
                    org = splited[10];
                }
            }
        }
        else if (splited[3] == "origin")
        {
            org = splited[4];
            if (splited[5] == "username")
            {
                user_name = splited[6];
                if (splited[7] == "destination")
                {
                    dest = splited[8];
                    check_in_hurry = splited[10];
                }
                else if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    dest = splited[10];
                }
            }
            else if (splited[5] == "destination")
            {
                dest = splited[6];
                if (splited[7] == "username")
                {
                    user_name = splited[8];
                    check_in_hurry = splited[10];
                }
                else if (splited[7] == "in_hurry")
                {
                    user_name = splited[10];
                    check_in_hurry = splited[8];
                }
            }
            else if (splited[5] == "in_hurry")
            {
                check_in_hurry = splited[6];
                if (splited[7] == "username")
                {
                    user_name = splited[8];
                    dest = splited[10];
                }
                if (splited[7] == "destination")
                {
                    user_name = splited[10];
                    dest = splited[8];
                }
            }
        }
        else if (splited[3] == "destination")
        {
            dest = splited[4];
            if (splited[5] == "username")
            {
                if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    org = splited[10];
                }
                else if (splited[7] == "origin")
                {
                    check_in_hurry = splited[10];
                    org = splited[8];
                }
            }
            else if (splited[5] == "origin")
            {
                org = splited[6];
                if (splited[7] == "in_hurry")
                {
                    check_in_hurry = splited[8];
                    user_name = splited[10];
                }
                else if (splited[7] == "username")
                {
                    check_in_hurry = splited[10];
                    user_name = splited[8];
                }
            }
            else if (splited[5] == "in_hurry")
            {
                check_in_hurry = splited[6];
                if (splited[7] == "username")
                {
                    user_name = splited[8];
                    org = splited[10];
                }
                else if (splited[7] == "origin")
                {
                    user_name = splited[10];
                    org = splited[8];
                }
            }
        }
        else if (splited[3] == "in_hurry")
        {
            check_in_hurry = splited[4];
            if (splited[5] == "uesrname")
            {
                user_name = splited[6];
                if (splited[7] == "destination")
                {
                    dest = splited[8];
                    org = splited[10];
                }
                else if (splited[7] == "origin")
                {
                    dest = splited[10];
                    org = splited[8];
                }
            }
            else if (splited[5] == "origin")
            {
                org = splited[6];
                if (splited[7] == "destination")
                {
                    dest = splited[8];
                    user_name = splited[10];
                }
                if (splited[7] == "username")
                {
                    dest = splited[10];
                    user_name = splited[8];
                }
            }
            else if (splited[5] == "destination")
            {
                dest = splited[6];
                if (splited[7] == "origin")
                {
                    org = splited[8];
                    user_name = splited[10];
                }
                else if (splited[7] == "username")
                {
                    org = splited[10];
                    user_name = splited[8];
                }
            }
        }
        if (check_in_hurry == "yes")
        {
            is_hurry = true;
        }
        Location *destination = find_location_by_name(dest);
        Location *origin = find_location_by_name(org);
        Passenger *passenger = find_passenger_by_name(user_name);
        Driver *driver = find_driver_by_name(user_name);
        if (check_get_cost(destination, origin, passenger, driver) == true)
        {   
            cout<<fixed<<setprecision(2)<<cal_price_of_travel(origin, destination, is_hurry) << endl;
        }
    }
    void handle_get_commands(vector<string> splited)
    {
        bool print_one_travel = false;
        for(int i = 0; i<splited.size(); i++)
        {
            if(splited[i] == "id")
            {
                print_one_travel = true;
                break;
            }
        }
        try
        {
            if (splited.size() < 2)
            {
                throw runtime_error(NOT_FOUND);
            }
            string get_command = splited[1];
            if (get_command == "trips" && splited.size() == 7 && print_one_travel == false)
            {
                get_all_trips(splited);
            }
            else if (get_command == "trips" && splited.size() == 7 && print_one_travel == true)
            {
                get_one_trip(splited);
            }
            else if (get_command == "cost")
            {
                get_cost_of_trip(splited);
            }
            else
            {
                throw runtime_error(NOT_FOUND);
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
        }
    }
    bool check_delete_trip(Passenger *passenger, Travel *travel)
    {
        try
        {
            if (travel == NULL)
            {
                throw runtime_error(NOT_FOUND);
            }
            if (passenger->getname() != travel->get_pass_name())
            {
                throw runtime_error(PERMISSION_DENIED);
            }
            else if (travel->get_status() == traveling || travel->get_status() == finished)
            {
                throw runtime_error(BAD_REQUEST);
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
            return false;
        }
        return true;
    }
    void delete_trip(vector<string> splited)
    {
        string username;
        int id;
        try
        {
            if (splited.size() < 7)
            {
                throw runtime_error(NOT_FOUND);
            }
            if (splited[3] == "username")
            {
                username = splited[4];
                id = stoi(splited[6]);
            }
            else if (splited[3] == "id")
            {
                id = stoi(splited[4]);
                username = splited[6];
            }
            Passenger *passenger = find_passenger_by_name(username);
            Travel *travel = find_travel_by_id(id);
            if (check_delete_trip(passenger, travel) == true)
            {
                travel->update_status("canceled");
                cout << "OK" << endl;
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
        }
    }
    void handle_delete_commands(vector<string> splited)
    {
        try
        {
            if (splited.size() < 2)
            {
                throw runtime_error(NOT_FOUND);
            }
            string delete_command = splited[1];
            if (delete_command == "trips")
            {
                delete_trip(splited);
            }
            else
            {
                throw runtime_error(NOT_FOUND);
            }
        }
        catch (runtime_error &ex)
        {
            cerr << ex.what() << endl;
        }
    }
    vector<string> split_line_by_spaces(string line)
    {
        vector<string> result;
        int start_pos = 0;
        while (start_pos < line.length())
        {
            int comma_pos = line.find(" ", start_pos);
            if (comma_pos == string::npos)
            {
                result.push_back(line.substr(start_pos));
                break;
            }
            result.push_back(line.substr(start_pos, comma_pos - start_pos));
            start_pos = comma_pos + 1;
        }
        return result;
    }
    void manage_commands()
    {
        string line;
        vector<string> splited;
        while (getline(cin, line))
        {
            splited = split_line_by_spaces(line);
            try
            {
                if (splited[0] == POST)
                {
                    handle_post_commands(splited);
                }
                else if (splited[0] == GET)
                {
                    handle_get_commands(splited);
                }
                else if (splited[0] == DELETE)
                {
                    handle_delete_commands(splited);
                }
                else
                {
                    throw runtime_error(BAD_REQUEST);
                }
            }
            catch (runtime_error &ex)
            {
                cerr << ex.what() << endl;
            }
        }
    }
};

int main(int argc, char *argv[])
{
    application UT_Taxi;
    UT_Taxi.read_file(argv[1]);
    UT_Taxi.manage_commands();
}