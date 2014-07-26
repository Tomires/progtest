/* 
 * File:   main.c
 * Author: Tomires
 *
 * Created on 30. říjen 2013, 20:28
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * 
 */

int PHI(int n){
    int i, /*y,*/ it, nt, count;
    count = 0;
    for(i=1;i<=n;i++){
        it = i; nt = n;
        while(it != nt){
            if(it > nt)
                it = it - nt;
            else
                nt = nt - it;}
        if(nt > 1)
            count++;
        /*for(y=2;y<=i;y++){
            //printf("%d | %d\n",i,y);
            if(i%y == 0 && n%y == 0){
                //printf("<<<<<\n");
                count++;
                break;
            }
        }*/            
    }
    return (n-count);
}

int main(int argc, char** argv) {
    int num;
    printf("Zadejte n:\n");
    
    if(scanf("%d",&num)==1)
        if(num > 0){
            printf("phi(%d) = %d\n",num,PHI(num));
            return(EXIT_SUCCESS);
        }
    
    printf("Nespravny vstup.\n");
    return (EXIT_FAILURE);
}

