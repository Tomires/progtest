/** \file edble.h
 *  Header for edble class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

/** \mainpage
*  This project implements a simple database
*  It accepts SQL style commands and supports load/save operations */

#ifndef EDBLE_H
#define EDBLE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "tableWorks.h"
#include "enum.h"

/// The main class of eDBle application. Represents a database
class edble
{
    public:
        /// Implicit constructor
        edble();
        /// Destructor
        virtual ~edble();
        /// Parses a command and sends it to a respective method. A crossroad of sorts
        /** \return 1 if successful, a negative number if unsuccessful (either -1 or -2 depending on the depth of the issue)*/
        int cmd(const std::string & command);
    protected:
        /// Creates a new table
        int crt(const std::string & line);
        /// Inserts a new line to an existing table
        int ins(const std::string & line);
        /// Selects data from an existing table
        int sel(const std::string & line) const;
        /// Deletes an existing table
        int del(const std::string & line);
        /// Loads database from a file
        int load(const std::string & line);
        /// Saves database to a file
        int save(const std::string & line) const;
    private:
        /// Writes a generalized error message on the standard output stream
        /** \return -1*/
        int parseError(const std::string & fnName) const;
        /// Gets a word from the line at the selected index using whitespace as a delimiter
        /** \return said word*/
        std::string getWord(int index, const std::string & line) const;
        /// Selects tables based on parameters
        /** \return 1 if successful, -2 if unsuccessful*/
        int selTables(const std::string & from, std::vector<cRow> & outTable, std::vector<std::string> & outCols) const;
        /// Prints selected and parsed table to standard output
        void printTable(const std::vector<std::vector<std::string>> & printable) const;
        /// Selects tables based on parameters
        /** \return 1 if successful, -1 if unsuccessful*/
        int whereDelete(std::vector<std::vector<std::string>> & printable, const std::string & where) const;
        std::vector<tableWorks> tables;
        std::vector<std::string> commands;
};

#endif // EDBLE_H
