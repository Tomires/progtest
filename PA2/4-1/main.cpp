#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

class MotherfuckingException : public exception
{
  virtual const char* what() const throw()
  {
    return "Something fucked up";
  }
} MotherfuckingException;

const char * deepFry(const char * rawChicken){
    char * friedChicken = new char[strlen(rawChicken) + 1];
    strcpy(friedChicken, rawChicken);
    return friedChicken; // so good
}

struct TTransaction;
struct TAccount;

typedef struct TTransaction
    {
        TTransaction(const char * rec, const char * sig, int amt){
        recipient = deepFry(rec);
        signature = deepFry(sig);
        amount = amt;
        nxt_Transaction = NULL;
        }
        ~TTransaction(void){
            delete nxt_Transaction;
            delete [] recipient;
            delete [] signature;
        }

        friend ostream & operator << (ostream & os, const TTransaction & x){
            if(x.amount < 0)
                return os << " - " << -x.amount << ", to: " << x.recipient << ", sign: " << x.signature << endl;
            else
                return os << " + " << x.amount << ", from: " << x.recipient << ", sign: " << x.signature << endl;
        }

        TTransaction *  nxt_Transaction;
        const char *    recipient;
        const char *    signature;
        int      amount;
    } TTransaction;

    typedef struct TAccount
    {
        TAccount(const char * id, int init){
            accID = deepFry(id);
            initial = init;
            total = init;
            nxt_Account = NULL;
            fst_Transaction = NULL;
        }
        TAccount(TAccount & old){
            accID = deepFry(old.accID);
            initial = old.initial;
            total = old.total;
            nxt_Account = NULL;

            TTransaction * oldT = old.fst_Transaction;

            fst_Transaction = new TTransaction(oldT -> recipient, oldT -> signature, oldT -> amount);
            TTransaction * newT = fst_Transaction;

            oldT = oldT -> nxt_Transaction;

            while(oldT){
            newT -> nxt_Transaction = new TTransaction(oldT -> recipient, oldT -> signature, oldT -> amount);
            oldT = oldT -> nxt_Transaction;
            newT = newT -> nxt_Transaction;
            }

            newT -> nxt_Transaction = NULL;
        }
        ~TAccount(void){
            delete fst_Transaction;
            delete nxt_Account;
            delete [] accID;
        }

        friend ostream & operator << (ostream & os, const TAccount & x){
            TTransaction * tmp = x.fst_Transaction;
            os << x.accID << ":" << endl;
            os << "   " << x.initial << endl;
            while(tmp){
                os << *tmp;
                tmp = tmp -> nxt_Transaction;
            }
            os << " = " << x.total << endl;
            return os;
        }

        int Balance(void){
            return total;
        }

        const char *    accID;
        int             initial;
        int             total;
        TTransaction *  fst_Transaction;
        TAccount *      nxt_Account;
    } TAccount;





class CBank
 {
   public:
    // default constructor
    CBank( void ){
        fst_Account = NULL;
    }
    // copy constructor
    CBank( const CBank & old ){
        int temp = 1;
        TAccount * oldT = old.fst_Account;


        fst_Account = new TAccount(*(oldT));
        oldT = oldT -> nxt_Account;
        TAccount * newT = fst_Account;

        while(oldT){
        if(temp == 1)
            temp = 0;
        else
            newT = newT -> nxt_Account;

            newT -> nxt_Account = new TAccount(*(oldT));
            oldT = oldT -> nxt_Account;
        }

        newT -> nxt_Account -> nxt_Account = NULL;
    }
    // destructor
    ~CBank( void ){
        delete fst_Account;
    }
    // operator =
    CBank & operator = ( const CBank & old ){
        delete fst_Account;

        int temp = 1;
        TAccount * oldT = old.fst_Account;


        fst_Account = new TAccount(*(oldT));
        oldT = oldT -> nxt_Account;
        TAccount * newT = fst_Account;

        while(oldT){
        if(temp == 1)
            temp = 0;
        else
            newT = newT -> nxt_Account;

            newT -> nxt_Account = new TAccount(*(oldT));
            oldT = oldT -> nxt_Account;
        }

        newT -> nxt_Account -> nxt_Account = NULL;

        return *this;
    }

    void vypis()
    {
        TAccount * tmp = fst_Account;
        while(tmp)
        {
            cout << tmp->accID << endl;
            tmp = tmp ->nxt_Account;
        }
    }

    bool                     NewAccount                    ( const char      * accID,
                                                             int               initialBalance ){
        TAccount * tmp = fst_Account;

        if(!tmp){
            fst_Account = new TAccount(deepFry(accID), initialBalance);
            return true;
        }

        while(tmp -> nxt_Account){
            if(strcmp(tmp -> accID, deepFry(accID)) == 0)
                return false;
            tmp = tmp -> nxt_Account;
        }

        if(strcmp(tmp -> accID, deepFry(accID)) == 0)
            return false;

        tmp -> nxt_Account = new TAccount(deepFry(accID), initialBalance);
        return true;
    }

    bool                     Transaction                   ( const char      * debAccID,
                                                             const char      * credAccID,
                                                             int               amount,
                                                             const char      * signature ){

        if(strcmp(debAccID, credAccID) == 0)
            return false;

        TAccount * tmp = fst_Account;
        TAccount * debit = NULL;
        TAccount * credit = NULL;

        if(!fst_Account)
            return false;

        while(tmp){
            if(!debit && strcmp(tmp -> accID, debAccID) == 0)
                debit = tmp;
            if(!credit && strcmp(tmp -> accID, credAccID) == 0)
                credit = tmp;
            tmp = tmp -> nxt_Account;
        }

        if(!(debit && credit))
            return false;

        TTransaction * tmp2 = debit -> fst_Transaction;
        TTransaction * tmpPrev = NULL;
        debit -> total -= amount;

        if(!tmp2)
            debit -> fst_Transaction = new TTransaction(deepFry(credAccID), deepFry(signature), -amount);
        else{
        while(tmp2){
            tmpPrev = tmp2;
            tmp2 = tmp2 -> nxt_Transaction;
        }
        tmpPrev -> nxt_Transaction = new TTransaction(deepFry(credAccID), deepFry(signature), -amount);
        }



        tmp2 = credit -> fst_Transaction;
        tmpPrev = NULL;
        credit -> total += amount;

        if(!tmp2)
            credit -> fst_Transaction = new TTransaction(deepFry(debAccID), deepFry(signature), amount);
        else{
        while(tmp2){
            tmpPrev = tmp2;
            tmp2 = tmp2 -> nxt_Transaction;
        }
        tmpPrev -> nxt_Transaction = new TTransaction(deepFry(debAccID), deepFry(signature), amount);
        }
        return true;
    }

    bool                     TrimAccount                   ( const char      * accID ){
        TAccount * tmp = fst_Account;

        while(tmp){
            if(strcmp(tmp -> accID, accID) == 0)
                break;
            tmp = tmp -> nxt_Account;
        }

        if(tmp){
            delete tmp -> fst_Transaction;
            tmp -> initial = tmp -> total;
            tmp -> fst_Transaction = NULL;
            return true;
        }
        return false;

    }


    // Account ( accID )
    TAccount & Account (const char * id){
        TAccount * tmp = fst_Account;

        while(tmp){
            if(strcmp(tmp -> accID, id) == 0)
                return * tmp;
            tmp = tmp -> nxt_Account;
        }
        throw MotherfuckingException;
        return * tmp;
    }



   private:
    TAccount *      fst_Account;

    /*void copyConstruct(const CBank & old){
        TAccount * oldA = old.fst_Account;
        TAccount * newA = fst_Account;

        newA = new TAccount(oldA -> accID, oldA -> initial);

        TTransaction * oldT = oldA -> fst_Transaction;
        TTransaction * newT = fst_Account -> fst_Transaction;

        newT = new TTransaction(oldT -> recipient, oldT -> signature, oldT -> amount);
        oldT = oldT -> nxt_Transaction;

        while(oldT){
        newT -> nxt_Transaction = new TTransaction(oldT -> recipient, oldT -> signature, oldT -> amount);
        oldT = oldT -> nxt_Transaction;
        newT = newT -> nxt_Transaction;
        }

        oldA = oldA -> nxt_Account;

        while(oldA){
        newA -> nxt_Account = new TAccount(oldA -> accID, oldA -> initial);
        newA = newA -> nxt_Account;

        oldT = oldA -> fst_Transaction;
        newT = newA -> fst_Transaction;

        newT = new TTransaction(oldT -> recipient, oldT -> signature, oldT -> amount);
        oldT = oldT -> nxt_Transaction;

        while(oldT){
        newT -> nxt_Transaction = new TTransaction(oldT -> recipient, oldT -> signature, oldT -> amount);
        oldT = oldT -> nxt_Transaction;
        newT = newT -> nxt_Transaction;
        }

        oldA = oldA -> nxt_Account;
        }
    }*/
 };





ostream & operator << (ostream & os, const TTransaction & x);
ostream & operator << (ostream & os, const TAccount & x);


#ifndef __PROGTEST__
 int main (void){
bool status;
int  balance;
char accCpy[100], debCpy[100], credCpy[100], signCpy[100];
CBank a;
status = a . NewAccount ( "123456", 1000 );
 cout << status << 1 << endl; // status = true
status = a . NewAccount ( "987654", -500 );
 cout << status << 1 << endl; // status = true
status = a . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
 cout << status << 1 << endl; // status = true
status = a . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
 cout << status << 1 << endl; // status = true
status = a . NewAccount ( "111111", 5000 );
 cout << status << 1 << endl; // status = true
status = a . Transaction ( "111111", "987654", 290, "Okh6e+8rAiuT5=" );
 cout << status << 1 << endl; // status = true
balance = a . Account ( "123456" ). Balance ( );
 cout << balance << ":" << "-2190" << endl; // balance = -2190
balance = a . Account ( "987654" ). Balance ( );
 cout << balance << ":" << "2980" << endl; // balance = 2980
balance = a . Account ( "111111" ). Balance ( );
 cout << balance << ":" << "4710" << endl; // balance = 4710
cout << a . Account ( "123456" );
/*
---8<---8<---8<---8<---8<---8<---8<---
123456:
   1000
 - 300, to: 987654, sign: XAbG5uKz6E=
 - 2890, to: 987654, sign: AbG5uKz6E=
 = -2190
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << a . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   -500
 + 300, from: 123456, sign: XAbG5uKz6E=
 + 2890, from: 123456, sign: AbG5uKz6E=
 + 290, from: 111111, sign: Okh6e+8rAiuT5=
 = 2980
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << a . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   5000
 - 290, to: 987654, sign: Okh6e+8rAiuT5=
 = 4710
---8<---8<---8<---8<---8<---8<---8<---
*/
status = a . TrimAccount ( "987654" );
 cout << status << 1 << endl; // status = true

status = a . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
 cout << status << 1 << endl; // status = true
cout << a . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   2980
 + 123, from: 111111, sign: asdf78wrnASDT3W
 = 3103
---8<---8<---8<---8<---8<---8<---8<---
*/

CBank c;
strncpy ( accCpy, "123456", sizeof ( accCpy ) );
status = c . NewAccount ( accCpy, 1000 );
 cout << status << 1 << endl; // status = true
strncpy ( accCpy, "987654", sizeof ( accCpy ) );
status = c . NewAccount ( accCpy, -500 );
 cout << status << 1 << endl; // status = true
strncpy ( debCpy, "123456", sizeof ( debCpy ) );
strncpy ( credCpy, "987654", sizeof ( credCpy ) );
strncpy ( signCpy, "XAbG5uKz6E=", sizeof ( signCpy ) );
status = c . Transaction ( debCpy, credCpy, 300, signCpy );
 cout << status << 1 << endl; // status = true
strncpy ( debCpy, "123456", sizeof ( debCpy ) );
strncpy ( credCpy, "987654", sizeof ( credCpy ) );
strncpy ( signCpy, "AbG5uKz6E=", sizeof ( signCpy ) );
status = c . Transaction ( debCpy, credCpy, 2890, signCpy );
 cout << status << 1 << endl; // status = true
strncpy ( accCpy, "111111", sizeof ( accCpy ) );
status = c . NewAccount ( accCpy, 5000 );
 cout << status << 1 << endl; // status = true
strncpy ( debCpy, "111111", sizeof ( debCpy ) );
strncpy ( credCpy, "987654", sizeof ( credCpy ) );
strncpy ( signCpy, "Okh6e+8rAiuT5=", sizeof ( signCpy ) );
status = c . Transaction ( debCpy, credCpy, 2890, signCpy );
 cout << status << 1 << endl; // status = true
balance = c . Account ( "123456" ). Balance ( );
 cout << balance << ":-2190" << endl; // balance = -2190
balance = c . Account ( "987654" ). Balance ( );
 cout << balance << ":5580" << endl;// balance = 5580
balance = c . Account ( "111111" ). Balance ( );
 cout << balance << ":2210" << endl;// balance = 2110
cout << c . Account ( "123456" );
/*
---8<---8<---8<---8<---8<---8<---8<---
123456:
   1000
 - 300, to: 987654, sign: XAbG5uKz6E=
 - 2890, to: 987654, sign: AbG5uKz6E=
 = -2190
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << c . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   -500
 + 300, from: 123456, sign: XAbG5uKz6E=
 + 2890, from: 123456, sign: AbG5uKz6E=
 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
 = 5580
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << c . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   5000
 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
 = 2110
---8<---8<---8<---8<---8<---8<---8<---
*/
status = c . TrimAccount ( "987654" );
 cout << status << 1 << endl; // status = true
strncpy ( debCpy, "111111", sizeof ( debCpy ) );
strncpy ( credCpy, "987654", sizeof ( credCpy ) );
strncpy ( signCpy, "asdf78wrnASDT3W", sizeof ( signCpy ) );
status = c . Transaction ( debCpy, credCpy, 123, signCpy );
 cout << status << 1 << endl; // status = true
cout << c . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   5580
 + 123, from: 111111, sign: asdf78wrnASDT3W
 = 5703
---8<---8<---8<---8<---8<---8<---8<---
*/

CBank e;
status = e . NewAccount ( "123456", 1000 );
 cout << status << 1 << endl; // status = true
status = e . NewAccount ( "987654", -500 );
 cout << status << 1 << endl; // status = true
status = e . NewAccount ( "123456", 3000 );
 cout << status << 0 << endl;// status = false
status = e . Transaction ( "123456", "666", 100, "123nr6dfqkwbv5" );
 cout << status << 0 << endl;//// status = false
status = e . Transaction ( "666", "123456", 100, "34dGD74JsdfKGH" );
 cout << status << 0 << endl;//// status = false
status = e . Transaction ( "123456", "123456", 100, "Juaw7Jasdkjb5" );
 cout << status << 0 << endl;//// status = false
//balance = e . Account ( "666" ). Balance ( );
 // exception thrown
//cout << e . Account ( "666" );
 // exception thrown
status = e . TrimAccount ( "666" );
 cout << status << 0 << endl;//// status = false

CBank g;
status = g . NewAccount ( "123456", 1000 );
 cout << status << 1 << endl; // status = true
status = g . NewAccount ( "987654", -500 );
 cout << status << 1 << endl; // status = true
status = g . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
 cout << status << 1 << endl; // status = true
status = g . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
 cout << status << 1 << endl; // status = true
status = g . NewAccount ( "111111", 5000 );
 cout << status << 1 << endl; // status = true
status = g . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" );
 cout << status << 1 << endl; // status = true
CBank h ( g );
status = g . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
 cout << status << 1 << endl; // status = true
status = h . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" );
 cout << status << 1 << endl; // status = true
status = g . NewAccount ( "99999999", 7000 );
 cout << status << 1 << endl; // status = true
status = g . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" );
 cout << status << 1 << endl; // status = true
status = g . TrimAccount ( "111111" );
 cout << status << 1 << endl; // status = true
status = g . Transaction ( "123456", "111111", 221, "Q23wr234ER==" );
 cout << status << 1 << endl; // status = true
cout << g . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   -1802
 + 221, from: 123456, sign: Q23wr234ER==
 = -1581
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << g . Account ( "99999999" );
/*
---8<---8<---8<---8<---8<---8<---8<---
99999999:
   7000
 + 3789, from: 111111, sign: aher5asdVsAD
 = 10789
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << g . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   -500
 + 300, from: 123456, sign: XAbG5uKz6E=
 + 2890, from: 123456, sign: AbG5uKz6E=
 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
 + 123, from: 111111, sign: asdf78wrnASDT3W
 = 5703
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << h . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   5000
 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
 - 789, to: 987654, sign: SGDFTYE3sdfsd3W
 = 1321
---8<---8<---8<---8<---8<---8<---8<---
*/
//cout << h . Account ( "99999999" );
 // exception thrown
cout << h . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   -500
 + 300, from: 123456, sign: XAbG5uKz6E=
 + 2890, from: 123456, sign: AbG5uKz6E=
 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
 + 789, from: 111111, sign: SGDFTYE3sdfsd3W
 = 6369
---8<---8<---8<---8<---8<---8<---8<---
*/

CBank i;
CBank j;
status = i . NewAccount ( "123456", 1000 );
 cout << status << 1 << endl; // status = true
status = i . NewAccount ( "987654", -500 );
 cout << status << 1 << endl; // status = true
status = i . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
 cout << status << 1 << endl; // status = true
status = i . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
 cout << status << 1 << endl; // status = true
status = i . NewAccount ( "111111", 5000 );
 cout << status << 1 << endl; // status = true
status = i . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" );
 cout << status << 1 << endl; // status = true
j = i;
status = i . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
 cout << status << 1 << endl; // status = true
status = j . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" );
 cout << status << 1 << endl; // status = true
status = i . NewAccount ( "99999999", 7000 );
 cout << status << 1 << endl; // status = true
status = i . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" );
 cout << status << 1 << endl; // status = true
status = i . TrimAccount ( "111111" );
 cout << status << 1 << endl; // status = true
cout << i . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   -1802
 = -1802
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << j . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   5000
 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
 - 789, to: 987654, sign: SGDFTYE3sdfsd3W
 = 1321
---8<---8<---8<---8<---8<---8<---8<---
*/
}
#endif /* __PROGTEST__ */
