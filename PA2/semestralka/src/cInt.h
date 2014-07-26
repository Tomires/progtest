/** \file cInt.h
 *  Header for cInt class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#ifndef CINT_H
#define CINT_H

#include <string>
#include <sstream>
#include "cObj.h"

/// This class is used for storing values of INTEGER type
class cInt : public cObj
{
    public:
        /// Constructor that accepts any integer variable
        cInt(int x);
        /// Destructor
        ~cInt();
        /// Returns the value
        /** \return Integer converted to string */
        std::string print() const;
    private:
        int value;
};

#endif // CINT_H
