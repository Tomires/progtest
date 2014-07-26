#ifndef __PROGTEST__
#include <cstring>
#include <iostream>
#include <iomanip>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#include <iterator>

using namespace std;
#endif /* __PROGTEST__ */


template <typename _Type, typename _Comparator = std::less<typename _Type::value_type> >
class CIndex
 {
   public:
             CIndex ( const _Type & sequenceToIndex,
                      const _Comparator & comparator = _Comparator () )
        : var(sequenceToIndex), cmp(comparator){ }
            ~CIndex ( void ){ }
    set<int> Search ( const _Type & searchFor ) const{
        //string test2 = "aaaaaaau aaauaaaau";
        //string se = "aaaa";
        set<int> tmp;

        for (typename _Type::const_iterator it = var.begin();
            it != var.end(); ++it) {
            //cout << *it << endl;
            //cout << (!cmp(*it,'g') && !cmp('g',*it)) << endl;
            typename _Type::const_iterator sit = searchFor.begin();

            if(searchFor.size() == 0)
                tmp.insert(std::distance(var.begin(),it));

            if(!cmp(*it,*sit) && !cmp(*sit,*it))
            {
                if(searchFor.size() == 1)
                    tmp.insert(std::distance(var.begin(),it));
                else{
                sit++;
                typename _Type::const_iterator yt = it;
                yt++;
                while(sit != searchFor.end())
                {
                    //cout << *yt << " | " << *sit << ":" << !(!cmp(*yt,*sit) && !cmp(*sit,*yt)) << endl;
                    if(cmp(*yt,*sit) || cmp(*sit,*yt))
                        break;
                        //cout << std::distance(test2.begin(),it) << " ";
                    sit++; yt++;
                    if(sit == searchFor.end())
                        tmp.insert(std::distance(var.begin(),it));
                }
                }
            }
        }
        return tmp;
    }

   private:
    // todo
    _Type var;
    _Comparator cmp;
 };

 #ifndef __PROGTEST__

 class CStrComparator
 {
   public:
                   CStrComparator ( bool caseSensitive )
                    : m_CaseSensitive ( caseSensitive ) { }
    bool           operator () ( const string & a, const string & b ) const
     {
       return m_CaseSensitive ? strcasecmp ( a . c_str (), b . c_str () ) < 0 : a < b ;
     }
    bool           m_CaseSensitive;
 };

bool upperCaseCompare ( const char & a, const char & b )
 {
   return toupper ( a ) < toupper ( b );
 }

void listSet(const set<int> & something){
    set<int>::iterator it;
    for(it = something.begin(); it != something.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
}

int main( void )
{
CIndex <string> t0 ( "abcabcabc" );
set<int> r0 = t0 . Search ( "a" );
listSet(r0); // 0 3 6
set<int> r1 = t0 . Search ( "abc" );
listSet(r1); // 0 3 6
set<int> r2 = t0 . Search ( "abcabc" );
listSet(r2); // 0 3

CIndex <string> t1 ( "abcababc" );
set<int> r3 = t1 . Search ( "a" );
listSet(r3); // 0 3 5
set<int> r4 = t1 . Search ( "abc" );
listSet(r4); // 0 5
set<int> r5 = t1 . Search ( "abcabc" );
listSet(r5); //

CIndex <string> t2 ( "kokokokoskokosokos" );
set<int> r6 = t2 . Search ( "kos" );
listSet(r6); // 6 11 15
set<int> r7 = t2 . Search ( "kokos" );
listSet(r7); // 4 9

CIndex <string> t3 ( "aaaaaaau aaauaaaau" );
set<int> r8 = t3 . Search ( "aa" );
listSet(r8); // 0 1 2 3 4 5 9 10 13 14 15
set<int> r9 = t3 . Search ( "aaa" );
listSet(r9); // 0 1 2 3 4 9 13 14
set<int> r10 = t3 . Search ( "aaaa" );
listSet(r10); // 0 1 2 3 13

CIndex <string> t4 ( "automatIc authentication automotive auTOmation raut" );
set<int> r11 = t4 . Search ( "auto" );
listSet(r11); // 0 25
set<int> r12 = t4 . Search ( "aut" );
listSet(r12); // 0 10 25 48
set<int> r13 = t4 . Search ( "tic" );
listSet(r13); // 16
set<int> r14 = t4 . Search ( "trunk" );
listSet(r14); //
set<int> r15 = t4 . Search ( "a" );
listSet(r15); // 0 5 10 19 25 36 41 48
set<int> r16 = t4 . Search ( "" );
listSet(r16); // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
 // 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50

CIndex <string, bool(*)(const char &, const char & )> t5 ( "automatIc authentication automotive auTOmation raut", upperCaseCompare );
set<int> r17 = t5 . Search ( "auto" );
listSet(r17); // 0 25 36
set<int> r18 = t5 . Search ( "aut" );
listSet(r18); // 0 10 25 36 48
set<int> r19 = t5 . Search ( "tic" );
listSet(r19); // 6 16
set<int> r20 = t5 . Search ( "trunk" );
listSet(r20); //
set<int> r21 = t5 . Search ( "a" );
listSet(r21); // 0 5 10 19 25 36 41 48
set<int> r22 = t5 . Search ( "" );
listSet(r22); // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
 // 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50


CIndex <list<string>, CStrComparator> t6 ( list<string>{ "Hello", "world", "test", "this", "foo", "TEsT", "this", "done" }, CStrComparator ( false ) );
set<int> r23 = t6 . Search ( list<string>{"test", "this", "foo"} );
listSet(r23); // 2
set<int> r24 = t6 . Search ( list<string>{"test", "this"} );
listSet(r24); // 2

CIndex <list<string>, CStrComparator> t7 ( list<string>{ "Hello", "world", "test", "this", "foo", "TEsT", "this", "done" }, CStrComparator ( true ) );
set<int> r25 = t7 . Search ( list<string>{"test", "this", "foo"} );
listSet(r25); // 2
set<int> r26 = t7 . Search ( list<string>{"test", "this"} );
listSet(r26); // 2 5

}
#endif /* __PROGTEST__ */
