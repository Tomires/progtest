/** \file enum.cpp
 *  Implementation of heavily used functions used across all the classes
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#include "enum.h"
using namespace std;

string toUpper(const string & original)
{
    string tmp = original;
    transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
    return tmp;
}

string extend(const string & original, const int width)
{
    string temp = original;
    temp.resize(width + 1, ' ');
    return temp;
}
