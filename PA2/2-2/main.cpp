#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

class CTaxRegister
{
public:
	CTaxRegister ( void );
	~CTaxRegister ( void );
	bool Birth        ( const string    & name,
                         const string    & addr );
     bool Death        ( const string    & name,
                         const string    & addr );
	bool Income       ( const string    & name,
                         const string    & addr,
                         int               amount );
	bool Expense      ( const string    & name,
                         const string    & addr,
                         int               amount );
	bool Audit        ( const string    & name,
                         const string    & addr,
                         int             & sumIncome,
                         int             & sumExpense ) const;
private: 
	struct CTax{
		string name;
		string addr;
		int expenses;
		int incomes;
		CTax(const string & _name, const string & _addr){name = _name; 
											    addr = _addr;
											    expenses = 0;
											    incomes = 0;}
		bool operator ==(const CTax comp /*const string & _name, const string & _addr*/) const{ return ( (name == comp.name) && (addr == comp.addr) );}
	};
	vector<CTax> dbTax; 
	static bool compTaxpayers( const CTax & L, const CTax & R );
	int fetchTaxpayer( const string & name, const string & addr ) const;
};

/******************************************************************/

CTaxRegister::CTaxRegister ( void ){}
CTaxRegister::~CTaxRegister ( void ){
	dbTax.clear();
}

bool CTaxRegister::compTaxpayers( const CTax & L, const CTax & R ){
	if(L.name != R.name)
		return L.name < R.name;
	return L.addr < R.addr;
}

int CTaxRegister::fetchTaxpayer( const string & name, const string & addr ) const{
	int left = 0;
	int right = dbTax.size() - 1;
	int middle;
	CTax searchval = CTax(name, addr);
	
	while(1){
		//cout << "|" << left << "," << right;
		if(left == right && !(dbTax[left] == searchval))
			return -1;
		middle = left + (right - left) / 2;
		if(dbTax[middle] == searchval)
			return middle;
		else if(compTaxpayers(dbTax[middle], searchval))
			left = middle + 1;
		else if(middle - 1 > left)
			right = middle - 1; // chodi?
		else
			right = left;		
	}
}

bool CTaxRegister::Birth ( const string    & name,
                           const string    & addr ){
	
	//kontrola duplicit!!, mozna insert
	//if(find(dbTax.begin(), dbTax.end(), CTax(name, addr)) != dbTax.end())
	//	return false;
	if(dbTax.size() == 0){
		dbTax.push_back(CTax(name, addr));
		return true;
	}
	
	int iss = fetchTaxpayer(name, addr);
	if(iss != -1)
		return false;
	
	
	unsigned int i = 0;
	while(compTaxpayers(dbTax[i],CTax(name, addr))){
		if(i == dbTax.size() - 1){
			dbTax.push_back(CTax(name, addr));
			return true;
		}
		i++;
	}
	dbTax.insert(dbTax.begin() + i, CTax(name, addr));
	/*dbTax.push_back(CTax(name, addr));
	
	int i = dbTax.size() - 1;
	
	if(i == 0)
		return true;
	while(compTaxpayers(dbTax[i],dbTax[i-1])){
		swap(dbTax[i],dbTax[i-1]);
		i--;
		if(i == 0)
			break;
	}*/
	
	//sort(dbTax.begin(), dbTax.end(), compTaxpayers);
	return true;
}
     
bool CTaxRegister::Death ( const string    & name,
                           const string    & addr ){

	for(unsigned int i=0;i<dbTax.size();i++)
		if(dbTax[i].name == name && dbTax[i].addr == addr){
			dbTax.erase(dbTax.begin() + i); // automaticky posun, neni treba sort
			return true;
		}

	return false;
}
     
bool CTaxRegister::Income ( const string    & name,
                            const string    & addr,
                            int               amount ){
	int i = fetchTaxpayer(name, addr);
	
	if(i != -1){
		dbTax[i].incomes += amount;
		return true;
	}
	// nutna optimalizace pulenim
	/*for(unsigned int i=0;i<dbTax.size();i++)
		if(dbTax[i].name == name && dbTax[i].addr == addr){
			dbTax[i].incomes += amount;
			return true;
		}
	*/
	return false;
}

bool CTaxRegister::Expense ( const string    & name,
                             const string    & addr,
                             int               amount ){
	int i = fetchTaxpayer(name, addr);
	
	if(i != -1){
		dbTax[i].expenses += amount;
		return true;
	}
	/*for(unsigned int i=0;i<dbTax.size();i++)
		if(dbTax[i].name == name && dbTax[i].addr == addr){
			dbTax[i].expenses += amount;
			return true;
		}
	*/	
	return false;
}
     
bool CTaxRegister::Audit ( const string    & name,
                           const string    & addr,
                           int             & sumIncome,
                           int             & sumExpense ) const{
	int i = fetchTaxpayer(name, addr);
	
	if(i != -1){
		sumIncome = dbTax[i].incomes;
		sumExpense = dbTax[i].expenses;
		return true;
	}
	/*
	for(unsigned int i=0;i<dbTax.size();i++)
		if(dbTax[i].name == name && dbTax[i].addr == addr){
			sumIncome = dbTax[i].incomes;
			sumExpense = dbTax[i].expenses;
			return true;
		}
	*/
	return false;
}
              
#ifndef __PROGTEST__
int main (int argc, const char * argv[]){
	bool   status;
	int    sumIncome, sumExpense;

CTaxRegister b1;
status = b1 . Birth ( "John Smith", "Oak Road 23" );
cout << "1" << status << endl; // status = true
status = b1 . Birth ( "Jane Hacker", "Main Street 17" );
cout << "1" << status << endl; // status = true
status = b1 . Birth ( "Peter Hacker", "Main Street 17" );
cout << "1" << status << endl; // status = true
status = b1 . Birth ( "John Smith", "Main Street 17" );
cout << "1" << status << endl; // status = true
status = b1 . Income ( "Peter Hacker", "Main Street 17", 2000 );
cout << "1" << status << endl; // status = true
status = b1 . Income ( "Jane Hacker", "Main Street 17", 3000 );
cout << "1" << status << endl; // status = true
status = b1 . Income ( "Peter Hacker", "Main Street 17", 4000 );
cout << "1" << status << endl; // status = true
status = b1 . Expense ( "Jane Hacker", "Main Street 17", 2000 );
cout << "1" << status << endl; // status = true
status = b1 . Expense ( "John Smith", "Main Street 17", 500 );
cout << "1" << status << endl; // status = true
status = b1 . Expense ( "Jane Hacker", "Main Street 17", 100 );
cout << "1" << status << endl; // status = true
status = b1 . Audit ( "John Smith", "Oak Road 23", sumIncome, sumExpense );
cout << "1" << status << " : in 0 / " << sumIncome << " : out 0 / " << sumExpense << endl; // status = true, sumIncome=0, sumExpense=0
status = b1 . Audit ( "Jane Hacker", "Main Street 17", sumIncome, sumExpense );
cout << "1" << status << " : in 3000 / " << sumIncome << " : out 2100 / " << sumExpense << endl; // status = true, sumIncome=3000, sumExpense=2100
status = b1 . Audit ( "Peter Hacker", "Main Street 17", sumIncome, sumExpense );
cout << "1" << status << " : in 6000 / " << sumIncome << " : out 0 / " << sumExpense << endl; // status = true, sumIncome=6000, sumExpense=0
status = b1 . Audit ( "John Smith", "Main Street 17", sumIncome, sumExpense );
cout << "1" << status << " : in 0 / " << sumIncome << " : out 500 / " << sumExpense << endl; // status = true, sumIncome=0, sumExpense=500
status = b1 . Death ( "John Smith", "Main Street 17" );
cout << "1" << status << endl; // status = true

CTaxRegister b2;
status = b2 . Birth ( "John Smith", "Oak Road 23" );
cout << "1" << status << endl; // status = true
status = b2 . Birth ( "Jane Hacker", "Main Street 17" );
cout << "1" << status << endl; // status = true
status = b2 . Income ( "Jane Hacker", "Oak Road 10", 4000 );
cout << "0" << status << endl; // status = false
status = b2 . Expense ( "John Smith", "Main Street 18", 500 );
cout << "0" << status << endl; // status = false
status = b2 . Audit ( "John Nowak", "Second Street 23", sumIncome, sumExpense );
cout << "0" << status << endl; // status = false
status = b2 . Death ( "Peter Nowak", "5-th Avenue" );
cout << "0" << status << endl; // status = false
status = b2 . Birth ( "Jane Hacker", "Main Street 17" );
cout << "0" << status << endl; // status = false
status = b2 . Birth ( "Joe Hacker", "Elm Street 23" );
cout << "1" << status << endl; // status = true
status = b2 . Birth ( "Joe Hacker", "Elm Street 23" );
cout << "0" << status << endl; // status = false
status = b2 . Death ( "Jane Hacker", "Main Street 17" );
cout << "1" << status << endl; // status = true
status = b2 . Audit ( "Jane Hacker", "Main Street 17", sumIncome, sumExpense );
cout << "0" << status << endl; // status = false
status = b2 . Birth ( "Jane Hacker", "Main Street 17" );
cout << "1" << status << endl; // status = true
}
#endif /* __PROGTEST__ */
