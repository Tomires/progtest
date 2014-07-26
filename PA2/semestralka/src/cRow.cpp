/** \file cRow.cpp
 *  Implementation of cRow class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */


#include "cRow.h"
using namespace std;

cRow::cRow(const vector<string> & values, const std::vector<int> & colTypes)
{
    for(unsigned int i = 0; i < colTypes.size(); i++){
        switch(colTypes[i]){
            case INTEGER:   objects.push_back(new cInt(atoi(values[i].c_str()))); break;
            case DOUBLE:    objects.push_back(new cDbl(atof(values[i].c_str()))); break;
            case DATE:      objects.push_back(new cDate(values[i])); break;
            default:        objects.push_back(new cVarchar(values[i])); break;
        }
    }
}

cRow::~cRow()
{
    objects.clear();
}

string cRow::out(unsigned int colNum) const
{
    if(colNum >= objects.size()) return "";

    return objects[colNum] -> print();
}

unsigned int cRow::getSize() const
{
    return objects.size();
}
