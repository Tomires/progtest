/** \file cObj.h
 *  Header for cObj class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#ifndef COBJ_H
#define COBJ_H

#include <string>

/// This is a base class for all variable types used in the database
class cObj
{
    public:
        /// Returns the value of variable, format depends on the type of said variable
        virtual std::string print() const = 0;
        /// Compares two variables
        /** \return Zero if variables are identical */
        int compare(cObj & x) const;
    private:
};

#endif // COBJ_H
