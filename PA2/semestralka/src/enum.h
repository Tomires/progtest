/** \file enum.h
 *  Header for enum definitions and heavily used functions used across all the classes
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#ifndef ENUM_H
#define ENUM_H

#include <string>
#include <algorithm>

#define     INTEGER     -1
#define     DOUBLE      -2
#define     DATE        -3
/* positive value represents
a VARCHAR type variable */

/// Converts a string into upper-case
/** \return STRING IN CAPITALS */
std::string toUpper(const std::string & original);
/// Extends a string to a certain length
/** \return Original string with lots of spaces */
std::string extend(const std::string & original, const int width);

#endif // ENUM_H
