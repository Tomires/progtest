#ifndef __PROGTEST__
#include "common.h"
using namespace std;
#endif /* __PROGTEST__ */

class MotherfuckingException : public exception
{
  virtual const char* what() const throw()
  {
    return "Something fucked up";
  }
} MotherfuckingException;

class CBase
 {
public:
    virtual                    ~CBase   ( void ) { }
    string                      Name    ( void ) const { return name; }
    friend ostream & operator   <<      ( ostream & os, CBase & x ){ x.Out(os); return os; }

    virtual bool                Cmp     ( const CBase * x ) const { return false; }
    virtual string              Type    ( void ) const { return "NOTHING"; }
    virtual void                Out     ( ostream & os ) const { }

    string name;
 };

class CRecA : public CBase
 {
public:
                                CRecA   ( string _name, CIPv4 _address ){
                                    name = _name; address = _address;}
                                CRecA( const CRecA & old ){
                                    name = old.name; address = old.address;}
    virtual bool                Cmp     ( const CBase * x ) const {
        const CRecA * a = dynamic_cast<const CRecA *>(x);
        return (name == a -> name && address == a -> address);
    }
    virtual string              Type    ( void ) const { return "A"; }
    virtual void                Out     ( ostream & os ) const { os << name << " A " << address << endl; }

    CIPv4 address;
 };

class CRecAAAA : public CBase
 {
public:
                                CRecAAAA( string _name, CIPv6 _address ){
                                    name = _name; address = _address;}
                                CRecAAAA( const CRecAAAA & old ){
                                    name = old.name; address = old.address;}
    virtual bool                Cmp     ( const CBase * x ) const {
        const CRecAAAA * a = dynamic_cast<const CRecAAAA *>(x);
        return (name == a -> name && address == a -> address);
    }
    virtual string              Type    ( void ) const { return "AAAA"; }
    virtual void                Out     ( ostream & os ) const { os << name << " AAAA " << address << endl; }

    CIPv6 address;
 };

class CRecMX : public CBase
 {
public:
                                CRecMX  ( string _name, string _server, int _priority ){
                                    name = _name; server = _server; priority = _priority;}
                                CRecMX  ( const CRecMX & old ){
                                    name = old.name; server = old.server; priority = old.priority;}
    virtual bool                Cmp     ( const CBase * x ) const {
        const CRecMX * a = dynamic_cast<const CRecMX *>(x);
        return (name == a -> name && server == a -> server && priority == a -> priority);
    }
    virtual string              Type    ( void ) const { return "MX"; }
    virtual void                Out     ( ostream & os ) const { os << name << " MX " << priority << " " << server << endl; }

    string server;
    int priority;
 };

class CCollection
 {
public:
                                CCollection ( void ) { }
                                CCollection ( const CCollection * old ) { data = old -> data; }
    CBase &            operator []          ( unsigned int position ) const {
        if(position >= data.size()) throw MotherfuckingException;
        return *data[position]; }                    // &*?
    int                         Count       ( void ) const { return data.size(); }
    friend ostream &   operator <<          ( ostream & os, const CCollection & x ){
        for( unsigned int i = 0; i < x.data.size(); i++ )
            (x.data[i]) -> Out(os);
        return os;
    }

    vector<CBase *> data;
 };

class CZone
 {
public:
                                CZone   ( string _name ) { name = _name; }
                                CZone   ( const CZone & old ) { col = new CCollection(old.col); name = old.name; }
    bool                        Add     ( const CBase & x ){

        for( unsigned int i = 0; i < col.data.size(); i++ )
            if( (*col.data[i]).Type() == x.Type() && (*col.data[i]).Cmp(&x) ) return false;

        if( x.Type() == "A" ){
            CRecA * a = new CRecA(dynamic_cast<const CRecA &>(x));
            col.data.push_back(a);
        }
        else if( x.Type() == "AAAA" ){
            CRecAAAA * a = new CRecAAAA(dynamic_cast<const CRecAAAA &>(x));
            col.data.push_back(a);
        }

        else if( x.Type() == "MX" ){
            CRecMX * a = new CRecMX(dynamic_cast<const CRecMX &>(x));
            col.data.push_back(a);
        }

        return true;
    }


    bool                        Del     ( const CBase & x ){
        for( unsigned int i = 0; i < col.data.size(); i++ )
            if( (*col.data[i]).Type() == x.Type() && (*col.data[i]).Cmp(&x) ){
                col.data.erase(col.data.begin() + i);
                return true;
            }
        return false;
    }
    CCollection                 Search  ( string name ) const {
        CCollection a = new CCollection();
        for( unsigned int i = 0; i < col.data.size(); i++ )
            if((*col.data[i]).Name() == name )
                    a.data.push_back(col.data[i]);
        return a;
    }
    CZone &            operator =       ( const CZone & old ){
        col = old.col;
        return *this;
    }
    friend ostream &   operator <<      ( ostream & os, const CZone & x ){
        //os << x.col;
        os << x.name << endl;
        for( unsigned int i = 0; i < x.col.data.size(); i++ ){
            if( i == x.col.data.size() - 1 ) os << " \\- ";
            else                             os << " +- ";
            (*x.col.data[i]).Out(os);
        }
        return os;
    }

    string name;
    CCollection col;
 };

#ifndef __PROGTEST__
int main ( void )
 {
#include "tests.inc"
   return 0;
 }
#endif /* __PROGTEST__ */
