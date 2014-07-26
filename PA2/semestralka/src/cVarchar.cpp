/** \file cVarchar.cpp
 *  Implementation of cVarchar class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */


#include "cVarchar.h"
using namespace std;

cVarchar::cVarchar(const string & x)
{
    value = x.substr(x.find_first_of("\"") + 1, x.find_last_of("\"") - x.find_first_of("\"") - 1);
}

cVarchar::~cVarchar()
{

}

string cVarchar::print() const
{
    return value;
}
