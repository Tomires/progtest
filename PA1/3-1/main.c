/* 
 * File:   main.c
 * Author: Tomires
 *
 * Created on 2. listopad 2013, 22:32
 */

#ifndef __PROGTEST__
#include <stdio.h>
#endif /* __PROGTEST__ */

/* vase pomocne funkce */
int prestupny(int y){
    int foo = 0;
    if(y % 4 == 0) foo = 1;
    if(y % 100 == 0) foo = 0;
    if(y % 400 == 0) foo = 1;
    if(y % 4000 == 0) foo = 0;
    return foo;
}

int valid(int y, int m, int d, int h, int i){
    if(y < 1600 || m < 1 || m > 12 || h < 0 || h > 23 || i < 0 || i > 59 || d < 0)
        return 0;
    if((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) && (d > 31))
        return 0;
    if((m == 4 || m == 6 || m == 9 || m == 11) && (d > 30))
        return 0;
    if(m == 2){
        if(prestupny(y) && (d > 29))
            return 0;
        if(!prestupny(y) && (d > 28))
            return 0;
    }
    return 1;
}

int position(int y1, int m1, int d1, int h1, int i1,
             int y2, int m2, int d2, int h2, int i2){
    if(y2-y1 > 0) return 1;
    if(y2-y1 < 0) return 0;
    if(m2-m1 > 0) return 1;
    if(m2-m1 < 0) return 0;
    if(d2-d1 > 0) return 1;
    if(d2-d1 < 0) return 0;
    if(h2-h1 > 0) return 1;
    if(h2-h1 < 0) return 0;
    if(i2-i1 >= 0) return 1;
    return 0;
}

long long int uniqtimeidentifier(int y, int m, int d, int h, int i, int position){
    long long int cuckoo;
    int ty, tm, td, th; // position = 0 - zacatek, 1 - konec (intervaly))
    cuckoo = 0;
    
    ty = 1600;
    while(ty<y){
        if(prestupny(ty)) cuckoo+=366*180;
        else cuckoo+=365*180;
        ty++;
    }

    tm = 1;
    while(tm<m){
        if(tm == 1 || tm == 3 || tm == 5 || tm == 7 || tm == 8 || tm == 10 || tm == 12)
            cuckoo+=31*180;
        else if(tm == 2 && prestupny(y))
            cuckoo+=29*180;
        else if(tm == 2 && !prestupny(y))
            cuckoo+=28*180;
        else
            cuckoo+=30*180;
        tm++;
    }
    
    td = 1;
    while(td<d){
        cuckoo+=180;
        td++;
    }
    
    th = 0;
    while(th<=h){
        if(th == h && position == 0 && i == 0) return cuckoo;
        if(th>12) cuckoo+=th-11;
        else if(th==0) cuckoo+=13;
        else cuckoo+=th+1;
        //printf("%d => %d\n",th, cuckoo);
        
        th++;
        
    }
    
    if(i<30) cuckoo--;
    return cuckoo;
    
    /*int cuckoo;
    cuckoo = 0;
    y-=1600;
    
    while(y>0){
        if(prestupny(y+1600)) cuckoo+=366*180;
        else cuckoo+=365*180;
        y--;
    }
    
    while(m>0){
        if(m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
            cuckoo+=31*180;
        if(m == 2 && prestupny(y))
            cuckoo+=29*180;
        if(m == 2 && !prestupny(y))
            cuckoo+=28*180;
        else
            cuckoo+=30*180;
        m--;
    }
    
    while(d>0){
        cuckoo+=180;
        d--;
    }
    
    while(h>=0){
        if(h>12) cuckoo+=h-11;
        if(h==0) cuckoo+=13;
        else cuckoo+=h+1;
        h--;
    }
    
    if(i>=30) cuckoo++;
    return cuckoo;*/
}

int cuckooClock ( int y1, int m1, int d1, int h1, int i1,
                  int y2, int m2, int d2, int h2, int i2, 
                  long long int * cuckoo )
 {
    if(!valid(y1, m1, d1, h1, i1) || !valid(y2, m2, d2, h2, i2))
        return 0;
    
    if(!position(y1, m1, d1, h1, i1, y2, m2, d2, h2, i2))
        return 0;
    
    /*
     * 1h = 1 cuckoo
     * 1d = 24 cuckoo
     * 1m = 672 (28)
     *      696 (29)
     *      720 (30)
     *      744 (31)
     * 1y = 8784 (p)
     *      8760 (!p)
     */
    
    /*int date1, date2;
    date1 = 0; date2 = 0;*/

    * cuckoo = uniqtimeidentifier(y2, m2, d2, h2, i2, 1) - uniqtimeidentifier(y1, m1, d1, h1, i1, 0);
   
    if(y1==y2 && m1==m2 && d1==d2 && h1==h2 && i1==30 && i2==30)
        * cuckoo = 1;
    
    
    /*
    date1+=35063256*(y1/4000);
    y1-=(y1/4000)*4000;
    date1+=3506328*(y1/400);
    y1-=(y1/400)*400;
    date1+=876576*(y1/100);
    y1-=(y1/100)*100;
    date1+=35064*(y1/4);
    y1-=(y1/4)*4;
    date1+=y1*24**180;
    
    date2+=35063256*(y1/4000);
    y1-=(y1/4000)*4000;
    date2+=3506328*(y1/400);
    y1-=(y1/400)*400;
    date2+=876576*(y1/100);
    y1-=(y1/100)*100;
    date2+=35064*(y1/4);
    y1-=(y1/4)*4;
    date2+=y1*8760;
    
    cuckoo=date2-date1;
    printf("%d", cuckoo);*/
    
    return 1;
 }

#ifndef __PROGTEST__
int main ( int argc, char * argv [] ) 
 {
   long long int cuckoo;
   int res;

   res = cuckooClock ( 2013, 10,  1, 13, 15,
                       2013, 10,  1, 18, 45, &cuckoo );
   /* res = 1, cuckoo = 26 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);

   res = cuckooClock ( 2013, 10,  1, 13, 15,
                       2013, 10,  2, 11, 20, &cuckoo );
   /* res = 1, cuckoo = 165 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2013,  1,  1, 13, 15,
                       2013, 10,  5, 11, 20, &cuckoo );
   /* res = 1, cuckoo = 49845 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2012,  1,  1, 13, 15,
                       2012, 10,  5, 11, 20, &cuckoo );
   /* res = 1, cuckoo = 50025 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);

   res = cuckooClock ( 1900,  1,  1, 13, 15,
                       1900, 10,  5, 11, 20, &cuckoo );
   /* res = 1, cuckoo = 49845 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2013, 10,  1,  0,  0,
                       2013, 10,  1, 12,  0, &cuckoo );
   /* res = 1, cuckoo = 102 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2013, 10,  1,  0, 15,
                       2013, 10,  1,  0, 25, &cuckoo );
   /* res = 1, cuckoo = 0 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2013, 10,  1, 12,  0,
                       2013, 10,  1, 12,  0, &cuckoo );
   /* res = 1, cuckoo = 12 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2013, 11,  1, 12,  0,
                       2013, 10,  1, 12,  0, &cuckoo );
   /* res = 0, cuckoo = N/A */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2013, 10, 32, 12,  0,
                       2013, 11, 10, 12,  0, &cuckoo );
   /* res = 0, cuckoo = N/A */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2100,  2, 29, 12,  0,
                       2100,  2, 29, 12,  0, &cuckoo );
   /* res = 0, cuckoo = N/A */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2400,  2, 29, 12,  0,
                       2400,  2, 29, 12,  0, &cuckoo );
   /* res = 1, cuckoo = 12 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   res = cuckooClock ( 2078, 7, 16, 0, 30, 2078, 7, 16, 0, 30, &cuckoo );
   /* res = 1, cuckoo = 12 */
   printf("res = %d, cuckoo = %d\n",res,cuckoo);
   
   return 0;
 }
#endif /* __PROGTEST__ */

