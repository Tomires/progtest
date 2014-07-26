/** \file cInt.cpp
 *  Implementation of cInt class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */


#include "cInt.h"
using namespace std;

cInt::cInt(int x)
{
    value = x;
}

cInt::~cInt()
{

}

string cInt::print() const
{
    stringstream ss;
    ss << value;
    return ss.str();
}
