#include <iostream>
#include "edble.h"
using namespace std;

int main()
{
    cout << " ______     _____     ______     __         ______    " << endl;
    cout << "/\\  ___\\   /\\  __-.  /\\  == \\   /\\ \\       /\\  ___\\   " << endl;
    cout << "\\ \\  __\\   \\ \\ \\/\\ \\ \\ \\  __<   \\ \\ \\____  \\ \\  __\\   " << endl;
    cout << " \\ \\_____\\  \\ \\____-  \\ \\_____\\  \\ \\_____\\  \\ \\_____\\ " << endl;
    cout << "  \\/_____/   \\/____/   \\/_____/   \\/_____/   \\/_____/ " << endl << endl;
    cout << "               *** Welcome to eDBle! ***" << endl;
    cout << endl;
    cout << "Supported commands:" << endl;
    cout << "CREATE TABLE <table name>(<col. header><col. type>, <...>)" << endl;
    cout << "   -> creates a table with specified columns" << endl;
    cout << "INSERT INTO <table name> VALUES(<value>, <...>)" << endl;
    cout << "   -> inserts a line into an existing table" << endl;
    cout << "   |!| number of values and data types must match the columns" << endl;
    cout << "SELECT <col. name>,<...> FROM <table name>,<...> WHERE <col. name>=<value>" << endl;
    cout << "   -> prints out columns from specified tables where a condition is met" << endl;
    cout << "   |?| WHERE is an optional parameter" << endl;
    cout << "DELETE TABLE <table name>" << endl;
    cout << "   -> deletes an existing table" << endl;
    cout << "LOAD DATABASE <filename>" << endl;
    cout << "   -> loads a previously saved database" << endl;
    cout << "SAVE DATABASE <filename>" << endl;
    cout << "   -> saves the current database to a file" << endl;
	cout << "EXIT" << endl;
    cout << "   -> self-explanatory" << endl;
    cout << endl;
    cout << "Supported data types and examples:" << endl;
    cout << "INTEGER    [-69, 0, 1337]" << endl;
    cout << "DOUBLE       [3.14, 2.71]" << endl;
    cout << "DATE           [20140611]" << endl;
    cout << "VARCHAR(n)  [\"something\"]" << endl;
    cout << endl;

    edble DB;

    while(1){
        cout << "> ";
        string line = "";
        if(cin.eof()) break;
        if(getline(cin, line)){
            if(!toUpper(line).compare("EXIT")) break;
            DB.cmd(line);
        }
    }

    return 0;
}
