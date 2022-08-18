#ifndef _LC_
#define _LC_
#include "Location.hpp"
Location::Location(std::string _name, double lat, double longi, int terrafic)
    {
        name = _name;
        latitude = lat;
        longitude = longi;
        terrafic_degree = terrafic;
    }
    std::string Location::getname() { return name; }
    double Location::getlat() { return latitude; }
    double Location::getlong() { return longitude; }
    int Location::getdrgree() { return terrafic_degree; }

#endif