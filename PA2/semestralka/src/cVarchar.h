/** \file cVarchar.h
 *  Header for cVarchar class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#ifndef CVARCHAR_H
#define CVARCHAR_H

#include <string>
#include "cObj.h"

/// This class is used for storing values of VARCHAR type
class cVarchar : public cObj
{
    public:
        /// Constructor that accepts any string variable
        cVarchar(const std::string & x);
        /// Destructor
        ~cVarchar();
        /// Returns the value
        /** \return Value in its natural type */
        std::string print() const;
    private:
        std::string value;
};

#endif // CVARCHAR_H
