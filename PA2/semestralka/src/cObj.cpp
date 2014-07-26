/** \file cObj.cpp
 *  Implementation of cObj class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#include "cObj.h"
using namespace std;
/* Why, hello there, abstract class! */

int cObj::compare(cObj & x) const{
    return print().compare(x.print());
}
