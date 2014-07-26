/** \file edble.cpp
 *  Implementation of edble class
 *  \author Tomas Havlik <havlito5@fit.cvut.cz> */

#include "edble.h"
using namespace std;

edble::edble()
{

}

edble::~edble()
{

}

int edble::cmd(const string & command)
{
    string tmp = toUpper(getWord(1,command));
    string commandT = command;
    commandT.erase(remove(commandT.begin(), commandT.end(), ';'), commandT.end());
    int result = 0;

    if      (tmp.empty())            return 0;
    else if (!tmp.compare("CREATE")) result = crt(commandT);
    else if (!tmp.compare("INSERT")) result = ins(commandT);
    else if (!tmp.compare("SELECT")) return sel(commandT);
    else if (!tmp.compare("DELETE")) result = del(commandT);
    else if (!tmp.compare("LOAD"))   return load(commandT);
    else if (!tmp.compare("SAVE"))   return save(commandT);

    if(result == 1){
        if(!tmp.compare("DELETE")){
            for(int i = commands.size() - 1; i >= 0; i--){
                if(commands[i].find("CREATE TABLE " + getWord(3, commandT) + "(") != string::npos
                || commands[i].find("INSERT INTO " + getWord(3, commandT) + " ") != string::npos){
                    commands.erase(commands.begin() + i);
                }
            }
        }
        else{
            transform(commandT.begin(), commandT.begin() + 12, commandT.begin(), ::toupper);
            commands.push_back(commandT);
        }
        return 1;
    }
	else return -1;

    cout << "ERROR: '" << tmp << "'" << " is not a valid command." << endl;
    return 0;
}

/***** PROTECTED PARTS *****/

int edble::crt(const string & line)
{
    string tmp = toUpper(getWord(2,line));

    if(tmp.compare("TABLE") != 0
    || line.find("(") == string::npos
    || line.find(")") == string::npos) return parseError("CREATE");

    vector<string> colNames;
    vector<int> colTypes;

    tmp = line.substr(line.find("(") + 1,line.find_last_of(")") - line.find("(") - 1) + " ";

    int pos = 1;
    while(!getWord(pos,tmp).empty()){
        string name = getWord(pos,tmp);
        string type = toUpper(getWord(pos + 1,tmp));
        type.erase(remove(type.begin(), type.end(), ','), type.end());

        for(unsigned int i = 0; i < colNames.size(); i++){
            if(!colNames[i].compare(name)){
            cout << "ERROR: two columns with identical names have been specified." << endl;
            return -1;
            }
        }

        if(!type.substr(0,8).compare("VARCHAR(")
              && type.find(")") != string::npos){
            string num = type.substr(type.find("(") + 1,type.find(")") - type.find("(") - 1);
            int number = atoi(num.c_str()); // returns 0 on failure

            if(number > 0)
                colTypes.push_back(number);
            else{
                cout << "ERROR: invalid VARCHAR size specified." << endl;
                return -1;
            }
        }

        else if(!type.compare("INTEGER")) colTypes.push_back(INTEGER);
        else if(!type.compare("DOUBLE")) colTypes.push_back(DOUBLE);
        else if(!type.compare("DATE")) colTypes.push_back(DATE);
        else{
            cout << "ERROR: '" << type << "' is not a valid type." << endl;
            return -1;
        }

        colNames.push_back(name);
        pos += 2;
    }

    string dbName = getWord(3,line).substr(0,getWord(3,line).find("("));

    for(unsigned int i = 0; i < tables.size(); i++){
        if(!tables[i].name().compare(dbName)){
            cout << "ERROR: table named '" << dbName << "' already exists." << endl;
            return -1;
        }
    }

    if(!colNames.size()){
        cout << "ERROR: no columns specified." << endl;
        return -1;
    }

    tables.push_back(tableWorks(dbName,colNames,colTypes));
    return 1;
}

int edble::ins(const string & line)
{
    if(toUpper(getWord(2,line)).compare("INTO") != 0
    || toUpper(line).find("VALUES(",0)          == string::npos
    || line.find(")",0)                         == string::npos) return parseError("INSERT");

    int num = -1;

    for(unsigned int i = 0; i < tables.size(); i++){
        if(!tables[i].name().compare(getWord(3,line))){
            num = i;
            break;
        }
    }

    if(num == -1){
        cout << "ERROR: table named '" << getWord(3,line) << "' does not exist." << endl;
        return -1;
    }

    string tmp = line.substr(line.find("(") + 1,line.find_last_of(")") - line.find("(") - 1) + " ";
    return tables[num].ins(tmp);
}

int edble::sel(const string & line) const
{
    // initial parsing
    string what = line.substr(6, toUpper(line).find("FROM") - 6);
    string from = "", where = "";

    if(toUpper(line).find("FROM") == string::npos) parseError("SELECT");

    if(toUpper(line).find("WHERE") == string::npos){
        from = line.substr(toUpper(line).find("FROM") + 4, line.size() - toUpper(line).find("FROM") - 4);
        where = "";
    }
    else{
        from = line.substr(toUpper(line).find("FROM") + 4, toUpper(line).find("WHERE") - toUpper(line).find("FROM") - 4);
        where = line.substr(toUpper(line).find("WHERE") + 5, line.size() - toUpper(line).find("WHERE") - 5);
    }

    what.erase(remove(what.begin(), what.end(), ' '), what.end());
    from.erase(remove(from.begin(), from.end(), ' '), from.end());
    where.erase(remove(where.begin(), where.end(), ' '), where.end());

    int tmp1 = -1;
    unsigned int tmp2 = from.find(",",0);

    if(from.empty()){
        cout << "ERROR: no tables selected." << endl;
        return -1;
    }

    // column names (they are the same across all the tables)

    vector<string> selectedColumns;

    tmp1 = -1;
    tmp2 = what.find(",",0);

    while(tmp2 != string::npos){
        selectedColumns.push_back(what.substr(tmp1 + 1, tmp2 - tmp1 - 1));

        tmp1 = tmp2;
        tmp2 = what.find(",",tmp2 + 1);
    }

    selectedColumns.push_back(what.substr(tmp1 + 1, what.size() - tmp1 - 1));

    // table work

    vector<vector<cRow>> selectedTables;
    vector<cRow> tempTable;

    tmp1 = -1;
    tmp2 = from.find(",",0);

    while(tmp2 != string::npos){
        if(selTables(from.substr(tmp1 + 1, tmp2 - tmp1 - 1),tempTable,selectedColumns) == -1){
            cout << "ERROR: table with name '" << from.substr(tmp1 + 1, tmp2 - tmp1 - 1) << "' not found." << endl;
            return -1;
        }

        selectedTables.push_back(tempTable);
        tempTable.clear();

        tmp1 = tmp2;
        tmp2 = from.find(",",tmp2 + 1);
    }

    if(selTables(from.substr(tmp1 + 1, tmp2 - tmp1 - 1),tempTable,selectedColumns) == -1){
            cout << "ERROR: table with name '" << from.substr(tmp1 + 1, tmp2 - tmp1 - 1) << "' not found." << endl;
            return -1;
    }

    selectedTables.push_back(tempTable);
    tempTable.clear();

    // printing out the tables

    vector<vector<string>> printable;
    vector<string> tmpRow;

    if(!selectedColumns.front().compare("*"))
        selectedColumns.erase(selectedColumns.begin()); // delete the * "flag"

    for(unsigned int i = 0; i < selectedColumns.size(); i++)
        for(unsigned int y = i + 1; y < selectedColumns.size(); y++)
            if(!selectedColumns[i].compare(selectedColumns[y]))
                selectedColumns[y].append("_1");

    switch(selectedTables.size()){
        case 1:
            printable.push_back(selectedColumns);

            for(unsigned int i = 0; i < selectedTables[0].size(); i++){
                for(unsigned int y = 0; y < selectedTables[0][i].getSize(); y++)
                    tmpRow.push_back(selectedTables[0][i].out(y)); // 1. tabulka, i. radek, y. sloupec
                printable.push_back(tmpRow);
                tmpRow.clear();
            }
            break;

        case 2:
            printable.push_back(selectedColumns);

            for(unsigned int i = 0; i < selectedTables[0].size(); i++){
                for(unsigned int j = 0; j < selectedTables[1].size(); j++){
                    for(unsigned int y = 0; y < selectedTables[0][i].getSize(); y++)
                        tmpRow.push_back(selectedTables[0][i].out(y)); // 1. tabulka, i. radek, y. sloupec
                    for(unsigned int z = 0; z < selectedTables[1][j].getSize(); z++)
                        tmpRow.push_back(selectedTables[1][j].out(z)); // 2. tabulka, j. radek, z. sloupec

                    printable.push_back(tmpRow);
                    tmpRow.clear();
                }
            }
            break;

        default:
            cout << "ERROR: unable to process request." << what << endl;
            return -1;
    }

    if(whereDelete(printable, where) == -1){
        cout << "ERROR: invalid where parameter specified." << what << endl;
        return -1;
    }

    printTable(printable);
    return 1;
}

int edble::del(const string & line)
{
    if(toUpper(getWord(2,line)).compare("TABLE") != 0) return parseError("DELETE");

    for(unsigned int i = 0; i < tables.size(); i++){
        if(!tables[i].name().compare(getWord(3,line))){
            tables.erase(tables.begin() + i);
            return 1;
        }
    }

    cout << "ERROR: table named '" << getWord(3,line) << "' does not exist." << endl;
    return -1;
}

int edble::load(const string & line)
{
    if(toUpper(getWord(2,line)).compare("DATABASE") != 0
    || line.find_first_of("\"") == line.find_last_of("\"")
    || line.find_first_of("\"") == string::npos)
        return parseError("LOAD");

    commands.clear();
    tables.clear();

    string li = "";
    ifstream file(line.substr(line.find_first_of("\"") + 1, line.find_last_of("\"") - line.find_first_of("\"") - 1));
    if(file.is_open()){
        while(getline(file, li)){
            cmd(li);
        }
    }
    else{
        cout << "ERROR: database not found. Check the spelling and try again." << endl;
        return -1;
    }

    file.close();
    return 1;
}

int edble::save(const string & line) const
{
    if(toUpper(getWord(2,line)).compare("DATABASE") != 0
    || line.find_first_of("\"") == line.find_last_of("\"")
    || line.find_first_of("\"") == string::npos)
        return parseError("SAVE");

    ofstream file(line.substr(line.find_first_of("\"") + 1, line.find_last_of("\"") - line.find_first_of("\"") - 1));
    for(unsigned int i = 0; i < commands.size(); i++)
        file << commands[i] << endl;
    file.close();
    return 1;
}

/***** PRIVATE PARTS *****/

int edble::parseError(const string & fnName) const
{
    cout << "ERROR: invalid arguments for " << fnName << " provided, please consult the documentation." << endl;
    return -1;
}
string edble::getWord(int index, const string & line) const
{
       int count = 0;
       string word = "";
       for ( unsigned int i = 0 ; i < line.length(); i++){
           if ( isspace(line[i]) != 0 && isspace(line[i+1]) == 0){
                count ++;
                if ( count == index) return word;
                word = "";
           }
           else
                word += line[i];
       }
       return word;
}

int edble::selTables(const string & from, vector<cRow> & outTable, vector<string> & outCols) const
{
    for(unsigned int i = 0; i < tables.size(); i++)
        if(!tables[i].name().compare(from)){
            tables[i].sel(outTable,outCols);
            return 1;
        }
    return -1;
}

void edble::printTable(const vector<vector<string>> & printable) const
{
    unsigned int width [printable[0].size()];

    // traverse the table and get me maximum width for every column

    for(unsigned int i = 0; i < printable[0].size(); i++){
        width[i] = 0; // initialization

        for(unsigned int y = 0; y < printable.size(); y++)
            if(printable[y][i].size() > width[i])
                width[i] = printable[y][i].size();
    }

    // actual printing work

    cout << endl;

    for(unsigned int y = 0; y < printable.size(); y++){
        for(unsigned int i = 0; i < printable[0].size(); i++)
            cout << extend(printable[y][i],width[i]);
        cout << endl;
    }

    cout << endl;
}

int edble::whereDelete(vector<vector<string>> & printable, const string & where) const
{
    if(where.find_first_of("=") == where.find_last_of("=")
    && where.find_first_of("=") != string::npos){

        int col1 = -1, col2 = -1;
        for(unsigned int y = 0; y < printable[0].size(); y++){
            if(!where.substr(0, where.find_first_of("=")).compare(printable[0][y]))
                col1 = y;
            if(!where.substr(where.find_first_of("=") + 1, where.size() - where.find_first_of("=") - 1).compare(printable[0][y]))
                col2 = y;
        }

        if(col1 == -1 || col2 == -1) return -1;

        // delete incompatible rows

        unsigned int i = printable.size() - 1;
        while(i != 0){
            if(printable[i][col1].compare(printable[i][col2]) != 0)
                printable.erase(printable.begin() + i);
            i--;
        }
        return 1;
    }
    else if(where.empty()) return 1;
    else return -1;
}
