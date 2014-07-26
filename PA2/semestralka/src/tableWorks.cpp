/** \file tableWorks.cpp
 *  Implementation tableWorks class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#include "tableWorks.h"
using namespace std;

tableWorks::tableWorks(const string & name, vector<string> colN, vector<int> colT)
{
    tbName = name;
    colNames = colN;
    colTypes = colT;
}

tableWorks::~tableWorks()
{

}

string tableWorks::name() const
{
    return tbName;
}

int tableWorks::ins(const string & line)
{
    unsigned int col = 0;
    int tmp1 = -1;
    unsigned int tmp2 = line.find(",",0);
    vector<string> parsed;

    if(line.empty()){
        cout << "ERROR: cannot insert an empty line." << endl;
        return -2;
    }

    while(tmp2 != string::npos){
        parsed.push_back(line.substr(tmp1 + 1, tmp2 - tmp1 - 1));

        if(!typeCheck(parsed.back(),col)){
        cout << "ERROR: type mismatch for '" << parsed.back() << "'." << endl;
        return -2;
        }

        col++;
        tmp1 = tmp2;
        tmp2 = line.find(",",tmp2 + 1);
    }

    parsed.push_back(line.substr( tmp1 + 1, line.size() - tmp1 - 1));

    if(!typeCheck(parsed.back(),col)){
        cout << "ERROR: type mismatch for '" << parsed.back() << "'." << endl;
        return -2;
    }

    if(col != colNames.size() - 1){
        cout << "ERROR: inputted values do not match the number of columns." << endl;
        return -2;
    }

    rows.push_back(cRow(parsed, colTypes));

    return 1;
}

int tableWorks::sel(vector<cRow> & outRows, vector<string> & columns) const
{
    vector<unsigned int> columnpos;

    if(!columns.front().compare("*")){
        gimmeColumns(columns);
        for (unsigned int y = 0; y < colNames.size(); y++)
            columnpos.push_back(y);
    }
    else{

        for(unsigned int i = 0; i < columns.size(); i++){
            for(unsigned int y = 0; y < colNames.size(); y++){
                if(!columns[i].compare(colNames[y])){
                    columnpos.push_back(y);
                    break;
                }
            }
        }

        if(columnpos.size() != columns.size()){
            cout << "ERROR: column with the specified name does not exist in the table." << endl;
            return -2;
        }
    }

    // end of parsing

    vector<int> colTypesN;
    vector<string> parsed;

    for(unsigned int y = 0; y < columnpos.size(); y++){
        colTypesN.push_back(colTypes[y]);
    }

    for(unsigned int i = 0; i < rows.size(); i++){
        for(unsigned int y = 0; y < columnpos.size(); y++){
            parsed.push_back(rows[i].out(columnpos[y]));
        }
        outRows.push_back(cRow(parsed, colTypesN));
        parsed.clear();
    }

    return 1;
}

/***** PRIVATE PARTS *****/

void tableWorks::gimmeColumns(std::vector<string> & colList) const
{
    for(unsigned int i = 0; i < colNames.size(); i++){
        colList.push_back(colNames[i]);
    }
    return;
}

int tableWorks::typeCheck(const string & var, int col) const
{
    string tmp = var;

    tmp.erase(remove(tmp.begin(), tmp.end(), ' '), tmp.end());
    switch(colTypes[col]){
        case INTEGER:
            return isInteger(tmp);
        case DOUBLE:
            return isDouble(tmp);
        case DATE:
            return isDate(tmp);
        default:
            return isVarchar(tmp, colTypes[col]);
    }
}

int tableWorks::isInteger(const string & var) const
{
    for(unsigned int i = 0; i < var.size(); i++)
        if(!isdigit(var[i]))
            return 0;
    return 1;
}

int tableWorks::isDate(const string & var) const
{
    if(atoi(var.c_str()) < 19000000
    || atoi(var.c_str()) > 99999999) return 0;

    int year  = atoi(var.substr(0,4).c_str());
    int month = atoi(var.substr(4,2).c_str());
    int day   = atoi(var.substr(6,2).c_str());

    int maxd = 0;
    switch(month){
        case 1:
            maxd = 31; break;
        case 2: if((!(year % 4)
                && (year % 100) != 0)
                || !(year % 400))
                    {maxd = 29; break;}
                 else
                    {maxd = 28; break;}
        case 3:
            maxd = 31; break;
        case 4:
            maxd = 30; break;
        case 5:
            maxd = 31; break;
        case 6:
            maxd = 30; break;
        case 7:
            maxd = 31; break;
        case 8:
            maxd = 31; break;
        case 9:
            maxd = 30; break;
        case 10:
            maxd = 31; break;
        case 11:
            maxd = 30; break;
        case 12:
            maxd = 31; break;
        default:
            return 0;
    }

    if(day < 1 || day > maxd) return 0;

    return 1;
}

int tableWorks::isDouble(const string & var) const
{
    for(unsigned int i = 0; i < var.size(); i++)
        if(!isdigit(var[i]) && i != var.find_first_of("."))
            return 0;
    return 1;
}

int tableWorks::isVarchar(const string & var, unsigned int vsize) const
{
    if(var.find_first_of("\"") == 0
    && var.find_last_of("\"") == var.size() - 1
    && var.size() <= (vsize + 2)) return 1;
    else return 0;
}
