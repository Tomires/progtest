/** \file cRow.h
 *  Header for cRow class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#ifndef CROW_H
#define CROW_H

#include <string>
#include <iostream>
#include <vector>
#include "cObj.h"
#include "cInt.h"
#include "cDbl.h"
#include "cDate.h"
#include "cVarchar.h"
#include "enum.h"

/// This is a class that symbolizes a single row of a table
class cRow
{
    public:
        /** Constructor that accepts a vector with stored values
         * and another vector with types of said values in integer form.
         * Utilizes the enum present in enum.h */
        cRow(const std::vector<std::string> & values, const std::vector<int> & colTypes);
        /// Destructor
        virtual ~cRow();
        /// Fetches an item
        /** \return value in the specified column */
        std::string out(unsigned int colNum) const;
        /// Returns number of columns
        unsigned int getSize() const;
    private:
        std::vector<cObj *> objects;
};

#endif // CROW_H
