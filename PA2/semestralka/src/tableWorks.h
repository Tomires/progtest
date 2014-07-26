/** \file tableWorks.h
 *  Header for tableWorks class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#ifndef TABLEWORKS_H
#define TABLEWORKS_H

#include <string>
#include <vector>
#include "cRow.h"
#include "enum.h"

/// This is a class that symbolizes tables in a database
class tableWorks
{
    public:
        /** Constructor that accepts name of the table (string), column names
         *(vector of strings) and column types (vector of integers) as parameters*/
        tableWorks(const std::string & name, std::vector<std::string> colN, std::vector<int> colT);
        /// Destructor
        virtual ~tableWorks();
        /// A getter that returns the name of the table
        std::string name() const;
        /// Inserts a line into the table, expects a pre-parsed string
        /** \return 1 if successful, -2 if unsuccessful */
        int ins(const std::string & line);
        /// Selects a customized portion of the table
        /** \return 1 if successful, -2 if unsuccessful */
        int sel(std::vector<cRow> & outRows, std::vector<std::string> & outCols) const;
    private:
        /// Fills the vector with names of all columns
        void gimmeColumns(std::vector<std::string> & colList) const;
        /// Checks if the type of a variable corresponds to a column
        /** \return 1 if successful, 0 if unsuccessful */
        int typeCheck(const std::string & var, int col) const;
        /// Checks if the variable is of INTEGER type
        int isInteger(const std::string & var) const;
        /// Checks if the variable is of DOUBLE type
        int isDouble(const std::string & var) const;
        /// Checks if the variable is of DATE type
        int isDate(const std::string & var) const;
        /// Checks if the variable is of VARCHAR type and smaller than specified length
        int isVarchar(const std::string & var, unsigned int vsize) const;
        std::vector<cRow> rows;
        std::vector<std::string> colNames;
        std::vector<int> colTypes;
        std::string tbName;
};

#endif // TABLEWORKS_H
