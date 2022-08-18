#ifndef _AC_
#define _AC_
#include "Aplication.hpp"
void Application:: add_id() { id_counter++; }
void Application::fill_locations(std::string line)
{
    std::vector<std::string> info;
    std::string word;
    std::stringstream s(line);
    while (std::getline(s, word, ','))
    {
        info.push_back(word);
    }
    locations.push_back(new Location(info[0], stod(info[1]), stod(info[2]), stoi(info[3])));
}
void Application::read_file(char *path)
{
    std::fstream inpt;
    std::string line;
    inpt.open(path, std::ios::in);
    getline(inpt, line); // skip first line
    line.clear();
    while (getline(inpt, line))
    {
        fill_locations(line);
    }
    inpt.close();
}
Passenger* Application::find_passenger_by_name(std::string username)
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
Driver* Application::find_driver_by_name(std::string driver_name)
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
Travel* Application::find_travel_by_id(int id)
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
Travel* Application::find_travel_by_org_and_dest(Location* destination,Location* origin)
{
    std::cout<<destination->getname()<<" "<<origin->getname()<<std::endl;
    std::cout<<travels.size();
    for(int i=0; i<travels.size(); i++)
    {
        std::cout<<travels[i]->get_des()<<" "<<travels[i]->get_origin()<<std::endl;
        if((travels[i]->get_des() == destination->getname()) && (travels[i]->get_origin() == origin->getname()))
        {
            return travels[i];
        }
    }
    return NULL;
}
Location* Application::find_location_by_name(std::string name)
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
bool Application::check_new(std::string username)
{
    try
    {
        for (int i = 0; i < drivers.size(); i++)
        {
            if (drivers[i]->getname() == username)
            {
                throw std::runtime_error("Bad Request");
            }
        }
        for (int i = 0; i < passengers.size(); i++)
        {
            if (passengers[i]->getname() == username)
            {
                throw std::runtime_error("Bad Request");
            }
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
double Application::cal_price_of_travel(Location* origin, Location* destination, bool is_hurry)
{
    double lat1 = origin->getlat();
    double lat2 = destination->getlat();
    double long1 = origin->getlong();
    double long2 = destination->getlong();
    double terr1 = origin->getdrgree();
    double terr2 = destination-> getdrgree();
    double distance = degree_by_km * sqrt((pow((lat2 - lat1), 2)) + (pow((long2 - long1), 2)));
    double cost = distance * (terr1 + terr2)*initial_cost;
    if (is_hurry == true)
    {
        cost *= hurry_factor;
    }
    return cost;
}
bool Application::check_standard_appeal(std::string passenger, std::string origin, std::string destination)
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
            throw std::runtime_error("Bad Request");
        }
        if (check_passenger == false || check_origin == false || check_des == false)
        {
            throw std::runtime_error(NOT_FOUND);
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
void Application::sign_up(std::vector<std::string> splited)
{
    std::string username, role;
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
        std::cout << "OK" << std::endl;
    }
}
void Application::post_trip(std::vector<std::string> splited)
{
    std::string check_in_hurry, user_name, dest, org;
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
        std::cout << _passenger->get_id() << std::endl;
    }
}
bool Application::check_accept(Driver *driver, Travel *travel)
{
    try
    {
        if (driver->get_on_the_road() == true || travel->get_status() != "waiting")
        {
            throw std::runtime_error(BAD_REQUEST);
        }
        else if (travel == NULL)
        {
            throw std::runtime_error(NOT_FOUND);
        }
        else if (driver == NULL)
        {
            throw std::runtime_error(PERMISSION_DENIED);
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
void Application::accept_travel(std::vector<std::string> splited)
{
    std::string username;
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
        std::cout << "OK" << std::endl;
    }
}
bool Application::check_finishing(Driver *driver, Travel *travel)
{
    try
    {
        if (driver->get_on_the_road() == false)
        {
            throw std::runtime_error(BAD_REQUEST);
        }
        else if (travel == NULL)
        {
            throw std::runtime_error(NOT_FOUND);
        }
        else if (driver == NULL)
        {
            throw std::runtime_error(PERMISSION_DENIED);
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
void Application::finish_travel(std::vector<std::string> splited)
{
    std::string username;
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
        std::cout << "OK" << std::endl;
    }
}
void Application::handle_post_commands(std::vector<std::string> splited)
{
    try
    {
        if (splited.size() < 2)
        {
            throw std::runtime_error(NOT_FOUND);
        }
        std::string post_command = splited[1];
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
            throw std::runtime_error(NOT_FOUND);
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
bool Application::check_get_trip(Human *driver)
{
    try
    {
        if (driver == NULL)
        {
            throw std::runtime_error(PERMISSION_DENIED);
        }
        if (travels.size() == 0)
        {
            throw std::runtime_error("Empty");
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
bool Application::sort_id(Travel* one, Travel* two)
{
    return (one->get_id() < two->get_id());
}
bool Application::sort_cost(Travel* one, Travel* two)
{
    return (one->get_cost() > two->get_cost());
}
void Application::print_travels_by_cost()
{
    sort(travels.begin() , travels.end(),Application::sort_cost);

    for(int i=0; i<travels.size(); i++)
    {
        Travel* temp = travels[i];
        std::cout << temp->get_id() << " " << temp->get_pass_name() << " " << temp->get_origin() << " " << temp->get_des() << " " ;
        std::cout<<std::fixed<<std::setprecision(2)<<temp->get_cost()<<" ";
        std::cout<< temp->get_status() << std::endl;
    }
    
}
void Application::print_travels_by_id()
{
    sort(travels.begin() , travels.end(), Application::sort_id);
    for(int i=0; i<travels.size(); i++)
    {
        Travel* temp = travels[i];
        std::cout << temp->get_id() << " " << temp->get_pass_name() << " " << temp->get_origin() << " " << temp->get_des() << " " ;
        std::cout<<std::fixed<<std::setprecision(2)<<temp->get_cost()<<" ";
        std::cout<< temp->get_status() << std::endl;
    }
}
void Application::get_all_trips(std::vector<std::string> splited)
{
    std::string driver_name,sort_by_cost;
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
bool Application::check_one_trip(Driver *driver, Travel *travel)
{
    try
    {
        if (driver == NULL)
        {
            throw std::runtime_error("Permission Denied");
        }
        else if (travel == NULL || travel->get_status() == "canceled")
        {
            throw std::runtime_error("Not Found");
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
void Application::get_one_trip(std::vector<std::string> splited)
{
    std::string driver_name;
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
        std::cout << travel->get_id() << " " << travel->get_pass_name() << " " << travel->get_origin() << " " << travel->get_des() << " " ;
        std::cout<<std::fixed<<std::setprecision(2)<<travel->get_cost()<<" ";
        std::cout<< travel->get_status() << std::endl;
    }
}
bool Application::check_get_cost(Location *destination, Location *origin, Human *passenger, Human *driver)
{
    try
    {
        if (destination == NULL || origin == NULL)
        {
            throw std::runtime_error(NOT_FOUND);
        }
        else if (passenger == NULL && driver == NULL)
        {
            throw std::runtime_error(NOT_FOUND);
        }
        else if (passenger == NULL || driver != NULL)
        {
            throw std::runtime_error(BAD_REQUEST);
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
void Application::get_cost_of_trip(std::vector<std::string> splited)
{
    std::string check_in_hurry, user_name, dest, org;
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
        std::cout<<std::fixed<<std::setprecision(2)<<cal_price_of_travel(origin, destination, is_hurry) << std::endl;
    }
}
void Application::handle_get_commands(std::vector<std::string> splited)
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
            throw std::runtime_error(NOT_FOUND);
        }
        std::string get_command = splited[1];
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
            throw std::runtime_error(NOT_FOUND);
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
bool Application::check_delete_trip(Passenger *passenger, Travel *travel)
{
    try
    {
        if (travel == NULL)
        {
            throw std::runtime_error(NOT_FOUND);
        }
        if (passenger->getname() != travel->get_pass_name())
        {
            throw std::runtime_error(PERMISSION_DENIED);
        }
        else if (travel->get_status() == traveling || travel->get_status() == finished)
        {
            throw std::runtime_error(BAD_REQUEST);
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
void Application::delete_trip(std::vector<std::string> splited)
{
    std::string username;
    int id;
    try
    {
        if (splited.size() < 7)
        {
            throw std::runtime_error(NOT_FOUND);
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
            std::cout << "OK" << std::endl;
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
void Application::handle_delete_commands(std::vector<std::string> splited)
{
    try
    {
        if (splited.size() < 2)
        {
            throw std::runtime_error(NOT_FOUND);
        }
        std::string delete_command = splited[1];
        if (delete_command == "trips")
        {
            delete_trip(splited);
        }
        else
        {
            throw std::runtime_error(NOT_FOUND);
        }
    }
    catch (std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
std::vector<std::string> Application::split_line_by_spaces(std::string line)
{
    std::vector<std::string> result;
    int start_pos = 0;
    while (start_pos < line.length())
    {
        int comma_pos = line.find(" ", start_pos);
        if (comma_pos == std::string::npos)
        {
            result.push_back(line.substr(start_pos));
            break;
        }
        result.push_back(line.substr(start_pos, comma_pos - start_pos));
        start_pos = comma_pos + 1;
    }
    return result;
}
void Application::manage_commands()
{
    std::string line;
    std::vector<std::string> splited;
    while (std::getline(std::cin, line))
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
                throw std::runtime_error(BAD_REQUEST);
            }
        }
        catch (std::runtime_error &ex)
        {
            std::cerr << ex.what() << std::endl;
        }
    }
}

#endif    
