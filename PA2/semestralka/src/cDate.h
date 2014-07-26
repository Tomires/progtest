/** \file cDate.h
 *  Header for cDate class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#ifndef CDATE_H
#define CDATE_H

#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "cObj.h"

/// This class is used for storing values of DATE type
class cDate : public cObj
{
    public:
        /// Constructor that accepts a string variable in valid format
        cDate(const std::string & x);
        /// Destructor
        ~cDate();
        /// Returns the value
        /** \return Date in a Y-M-D notation (string) */
        std::string print() const;
    private:
        int year, month, day;
};

#endif // CDATE_H
