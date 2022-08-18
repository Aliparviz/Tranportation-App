#ifndef _MC_
#define _MC_
#include "Aplication.hpp"

int main(int argc, char *argv[])
{
    Application UT_Taxi;
    UT_Taxi.read_file(argv[1]);
    UT_Taxi.manage_commands();
}
#endif