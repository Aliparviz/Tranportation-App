#ifndef _LH_
#define _LH_
#include "Human.hpp"
class Location
{
private:
    std::string name;
    double latitude;
    double longitude;
    int terrafic_degree;

public:
    Location(std::string _name, double lat, double longi, int terrafic);
    std::string getname();
    double getlat();
    double getlong();
    int getdrgree();
};
#endif