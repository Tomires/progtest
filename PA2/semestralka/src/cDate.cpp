/** \file cDate.cpp
 *  Implementation of cDate class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */
#include "cDate.h"
using namespace std;

cDate::cDate(const string & x)
{
    string tmp = x;
    tmp.erase(remove(tmp.begin(), tmp.end(), ' '), tmp.end());
    year  = atoi(tmp.substr(0,4).c_str());
    month = atoi(tmp.substr(4,2).c_str());
    day   = atoi(tmp.substr(6,2).c_str());
}

cDate::~cDate()
{

}

string cDate::print() const
{
    stringstream ss;
    ss << year << setfill('0') << setw(2) << month << setfill('0') << setw(2) << day;
    return ss.str();
}
