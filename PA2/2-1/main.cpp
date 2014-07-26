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
                         const string    & addr,
                         const string	 & account );
     bool Death        ( const string    & name,
                         const string    & addr );
     bool Income       ( const string    & account,
                         int               amount );
	bool Income       ( const string    & name,
                         const string    & addr,
                         int               amount );
     bool Expense      ( const string    & account,
                         int               amount );
	bool Expense      ( const string    & name,
                         const string    & addr,
                         int               amount );
	bool Audit        ( const string    & name,
                         const string    & addr,
                         	 string	 & account,
                         int             & sumIncome,
                         int             & sumExpense ) const;
private: 
	struct CTax{
		string name;
		string addr;
		string account;
		int ref;
		CTax(const string & _name, const string & _addr, const string & _account, const int & _ref){name = _name; 
											    addr = _addr;
											    account = _account;
											    ref = _ref;}
		bool operator ==(const CTax comp /*const string & _name, const string & _addr*/) const{ return ( (name == comp.name) && (addr == comp.addr) );}
		
	};
	
	struct CTax2{
		string account;
		int ref;
		CTax2(const string & _account, const int & _ref){account = _account;
								ref = _ref;}
		bool operator ==(const CTax2 comp/*const string & _name, const string & _addr*/) const{ return ( (account == comp.account));}		
	};
	
	struct CTax3{
		int incomes;
		int expenses;
		CTax3(){incomes = 0;
			  expenses = 0;}	
	};
	
	vector<CTax> dbTax; // name, addr, ref SORTED
	vector<CTax2> dbTax2; // account, ref SORTED
	vector<CTax3> dbTax3; // incomes, expenses UNSORTED
	
	static bool compTaxpayers( const CTax & L, const CTax & R );
	int fetchTaxpayer( const string & name, const string & addr ) const;
	
	static bool compTaxpayers2( const CTax2 & L, const CTax2 & R );
	int fetchTaxpayer2( const string & account) const;
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

bool CTaxRegister::compTaxpayers2( const CTax2 & L, const CTax2 & R ){
	return L.account < R.account;
}

int CTaxRegister::fetchTaxpayer( const string & name, const string & addr ) const{
	int left = 0;
	int right = dbTax.size() - 1;
	int middle;
	CTax searchval = CTax(name, addr, "", 0);
	
	while(1){
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

int CTaxRegister::fetchTaxpayer2( const string & account) const{
	int left = 0;
	int right = dbTax2.size() - 1;
	int middle;
	CTax2 searchval = CTax2(account, 0);
	
	while(1){
		if(left == right && !(dbTax2[left] == searchval))
			return -1;
		middle = left + (right - left) / 2;
		if(dbTax2[middle] == searchval)
			return middle;
		else if(compTaxpayers2(dbTax2[middle], searchval))
			left = middle + 1;
		else if(middle - 1 > left)
			right = middle - 1; // chodi?
		else
			right = left;		
	}
}

bool CTaxRegister::Birth ( const string    & name,
                           const string    & addr,
                           const string	   & account ){

	if(dbTax.size() == 0){ // pokud je dbTax prazdna, pak uvazujeme, ze dbTax2 je tez prazdna
		dbTax3.push_back(CTax3());
		dbTax.push_back(CTax(name, addr, account,dbTax3.size()-1));
		dbTax2.push_back(CTax2(account,dbTax3.size()-1));
		return true;
	}
	
	int iss = fetchTaxpayer(name, addr);
	if(iss != -1)
		return false;
	
	iss = fetchTaxpayer2(account);
	if(iss != -1)
		return false;
		
	// gimme incomes, expenses
	dbTax3.push_back(CTax3());
	
	
	/*
	unsigned int i = 0;
	while(compTaxpayers(dbTax[i],CTax(name, addr, account,dbTax3.size()-1))){
		if(i == dbTax.size() - 1){
			dbTax.push_back(CTax(name, addr, account,dbTax3.size()-1));
			i = 9999999; // sorry za to lado :(
			break;
		}
		i++;
	}
	if(i != 9999999)
		dbTax.insert(dbTax.begin() + i, CTax(name, addr, account,dbTax3.size()-1));
	*/
	vector<CTax>::iterator low;
	low = lower_bound(dbTax.begin(), dbTax.end(), CTax(name, addr, account,dbTax3.size()-1),compTaxpayers);
	dbTax.insert(low, CTax(name, addr, account,dbTax3.size()-1));
	
	/*
	i = 0;
	while(compTaxpayers2(dbTax2[i],CTax2(account,dbTax3.size()-1))){
		if(i == dbTax2.size() - 1){
			dbTax2.push_back(CTax2(account,dbTax3.size()-1));
			i = 9999999;
			break;
		}
		i++;
	}
	if(i != 9999999)
		dbTax2.insert(dbTax2.begin() + i, CTax2(account,dbTax3.size()-1));
	*/
	vector<CTax2>::iterator low2;
	low2 = lower_bound(dbTax2.begin(), dbTax2.end(), CTax2(account,dbTax3.size()-1),compTaxpayers2);
	dbTax2.insert(low2, CTax2(account,dbTax3.size()-1));
	
	
	
	
	
	
	
	return true;
}
     
bool CTaxRegister::Death ( const string    & name,
                           const string    & addr ){ // neodstranuje zaznam z treti tabulky [validita refu]

	for(unsigned int i=0;i<dbTax.size();i++)
		if(dbTax[i].name == name && dbTax[i].addr == addr){
			//dbTax3.erase(dbTax3.begin() + dbTax[i].ref)
			for(unsigned int y=0;y<dbTax2.size();y++)
				if(dbTax2[y].account == dbTax[i].account)
					dbTax2.erase(dbTax2.begin() + y);
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
		dbTax3[dbTax[i].ref].incomes += amount;
		return true;
	}
	return false;
}

bool CTaxRegister::Income ( const string    & account,
                            int               amount ){
	int i = fetchTaxpayer2(account);
	
	if(i != -1){
		dbTax3[dbTax2[i].ref].incomes += amount;
		return true;
	}
	return false;
}

bool CTaxRegister::Expense ( const string    & name,
                             const string    & addr,
                             int               amount ){
	int i = fetchTaxpayer(name, addr);
	
	if(i != -1){
		dbTax3[dbTax[i].ref].expenses += amount;
		return true;
	}
	return false;
}

bool CTaxRegister::Expense ( const string    & account,
                            int               amount ){
	int i = fetchTaxpayer2(account);
	
	if(i != -1){
		dbTax3[dbTax2[i].ref].expenses += amount;
		return true;
	}
	return false;
}
     
bool CTaxRegister::Audit ( const string    & name,
                           const string    & addr,
                           	   string    & account,
                           int             & sumIncome,
                           int             & sumExpense ) const{
	int i = fetchTaxpayer(name, addr);
	
	if(i != -1){
		sumIncome = dbTax3[dbTax[i].ref].incomes;
		sumExpense = dbTax3[dbTax[i].ref].expenses;
		account = dbTax[i].account;
		return true;
	}
	return false;
}
              
#ifndef __PROGTEST__
int main (int argc, const char * argv[]){
bool   status;
string acct;
int    sumIncome, sumExpense;

CTaxRegister b1;
status = b1 . Birth ( "John Smith", "Oak Road 23", "123/456/789" );
cout << "1" << status << endl; // status = true
status = b1 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" );
cout << "1" << status << endl; // status = true
status = b1 . Birth ( "Peter Hacker", "Main Street 17", "634oddT" );
cout << "1" << status << endl; // status = true
status = b1 . Birth ( "John Smith", "Main Street 17", "Z343rwZ" );
cout << "1" << status << endl; // status = true
status = b1 . Income ( "Xuj5#94", 1000 );
cout << "1" << status << endl; // status = true
status = b1 . Income ( "634oddT", 2000 );
cout << "1" << status << endl; // status = true
status = b1 . Income ( "123/456/789", 3000 );
cout << "1" << status << endl; // status = true
status = b1 . Income ( "634oddT", 4000 );
cout << "1" << status << endl; // status = true
status = b1 . Income ( "Peter Hacker", "Main Street 17", 2000 );
cout << "1" << status << endl; // status = true
status = b1 . Expense ( "Jane Hacker", "Main Street 17", 2000 );
cout << "1" << status << endl; // status = true
status = b1 . Expense ( "John Smith", "Main Street 17", 500 );
cout << "1" << status << endl; // status = true
status = b1 . Expense ( "Jane Hacker", "Main Street 17", 1000 );
cout << "1" << status << endl; // status = true
status = b1 . Expense ( "Xuj5#94", 1300 );
cout << "1" << status << endl; // status = true
status = b1 . Audit ( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense );
cout << "1" << status << " : acct 123/456/789 / " << acct << " : in 3000 / " << sumIncome << " : out 0 / " << sumExpense << endl; // status = true, acct = "123/456/789", sumIncome=3000, sumExpense=0
status = b1 . Audit ( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense );
cout << "1" << status << " : acct Xuj5#94 / " << acct << " : in 1000 / " << sumIncome << " : out 4300 / " << sumExpense << endl; // status = true, acct = "Xuj5#94", sumIncome=1000, sumExpense=4300
status = b1 . Audit ( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense );
cout << "1" << status << " : acct 634oddT / " << acct << " : in 8000 / " << sumIncome << " : out 0 / " << sumExpense << endl; // status = true, acct = "634oddT", sumIncome=8000, sumExpense=0
status = b1 . Audit ( "John Smith", "Main Street 17", acct, sumIncome, sumExpense );
cout << "1" << status << " : acct Z343rwZ / " << acct << " : in 0 / " << sumIncome << " : out 500 / " << sumExpense << endl; // status = true, acct = "Z343rwZ", sumIncome=0, sumExpense=500
status = b1 . Death ( "John Smith", "Main Street 17" );
cout << "1" << status << endl; // status = true

CTaxRegister b2;
status = b2 . Birth ( "John Smith", "Oak Road 23", "123/456/789" );
cout << "1" << status << endl; // status = true
status = b2 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" );
cout << "1" << status << endl; // status = true
status = b2 . Income ( "634oddT", 4000 );
cout << "0" << status << endl; // status = false
status = b2 . Expense ( "John Smith", "Main Street 18", 500 );
cout << "0" << status << endl; // status = false
status = b2 . Audit ( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense );
cout << "0" << status << endl; // status = false
status = b2 . Death ( "Peter Nowak", "5-th Avenue" );
cout << "0" << status << endl; // status = false
status = b2 . Birth ( "Jane Hacker", "Main Street 17", "4et689A" );
cout << "0" << status << endl; // status = false
status = b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" );
cout << "0" << status << endl; // status = false
status = b2 . Death ( "Jane Hacker", "Main Street 17" );
cout << "1" << status << endl; // status = true
status = b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" );
cout << "1" << status << endl; // status = true
status = b2 . Audit ( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense );
cout << "1" << status << " : acct Xuj5#94 / " << acct << " : in 0 / " << sumIncome << " : out 0 / " << sumExpense << endl; // status = true, acct = "Xuj5#94", sumIncome=0, sumExpense=0
status = b2 . Birth ( "Joe Hacker", "Elm Street 23", "AAj5#94" );
cout << "0" << status << endl; // status = false
}
#endif /* __PROGTEST__ */
