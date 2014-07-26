/** \file cDbl.cpp
 *  Implementation of cDbl class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */


#include "cDbl.h"
using namespace std;

cDbl::cDbl(double x)
{
    value = x;
}

cDbl::~cDbl()
{

}

string cDbl::print() const
{
    stringstream ss;
    ss << value;
    return ss.str();
}
