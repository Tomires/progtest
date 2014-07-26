/** \file cDbl.h
 *  Header for cDbl class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#ifndef CDBL_H
#define CDBL_H

#include <string>
#include <sstream>
#include "cObj.h"

/// This class is used for storing values of DOUBLE type
class cDbl : public cObj
{
    public:
        /// Constructor that accepts any double variable
        cDbl(double x);
        /// Destructor
        ~cDbl();
        /// Returns the value
        /** \return Double converted to string */
        std::string print() const;
    private:
        double value;
};

#endif // CDBL_H
