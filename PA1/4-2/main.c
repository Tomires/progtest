/* 
 * File:   main.c
 * Author: Tomires
 *
 * Created on 17. listopad 2013, 15:09
 */

#include <stdio.h>
#include <stdlib.h>
/*
 * 
 */

int compare (const void * a, const void * b){
  return ( *(int*)a - *(int*)b );
}

/*int compute(int N, int reservoirs()){
    qsort(reservoirs,N,sizeof(int),compare);
    int min = reservoirs[0][0], max = reservoirs[0][0], i;
    for(i=0;i<N;i++){
        if(reservoirs[i][0]<min) 
            min = reservoirs[i][0];
        if(reservoirs[i][0]>max) max = reservoirs[i][0];
    }
    
}*/

int main(int argc, char** argv) {
    /*int N, i, res[100][3]; // dodelat pro 200 000
    
    printf("Zadejte pocet nadrzi:\n");
    if(scanf("%d",&N)!=1 || (N < 1) || (N > 200000)){
        printf("Nespravny vstup.\n");
        return(EXIT_FAILURE);}
    
    printf("Zadejte parametry nadrzi:\n");
    for(i=0;i<N;i++){
        if(scanf("%d %d %d %d",&res[i][0],&res[i][1],&res[i][2],&res[i][3]) != 4){
            printf("Nespravny vstup.\n");
            return(EXIT_FAILURE);}
    }
    
    compute(N, res);*/
    printf("Zadejte cisla, EOF=konec:\n");
    int res, null;
    int N, numbers[100000], len = 0;
    //while(res = scanf("%d",&N) != EOF && len < 100000){
    while(len < 100000){
        res = scanf("%d",&N);
        if(res == EOF) break;
        if(res == 0){
            printf("Nespravny vstup.\n");
            return (EXIT_FAILURE);
        }
        if(null != 1) null = 1;
        numbers[len] = N;
        len++;
    }
    //printf("%d", len);
    
    if(null != 1){
        printf("Nespravny vstup.\n");
        return (EXIT_FAILURE);
    }
    
    qsort(numbers,len,sizeof(int),compare);
    
    int i, max;
    /*for(i=0;i<len;i++){
        printf("%d\n",numbers[i]);
    }*/
    
    
    //int i; 
    N = 0;
    max = 0;
    int numbers2[100000][2];
    int len2;
    len2 = 0;
    
    for(i=1;i<len;i++){
        if(numbers[i]==numbers[i-1])
            N++;
        else{
            if(N+1 > max) max = N+1;
            numbers2[len2][0] = N+1;
            numbers2[len2][1] = numbers[(i-1)];
            N = 0;
            len2++;
        }
    }
    if(N+1 > max) max = N+1;
    numbers2[len2][0] = N+1;
    numbers2[len2][1] = numbers[i-1];
    
    /*for(i=0;i<=len2;i++){
        printf("%d vyskytu cisla %d\n",numbers2[i][0],numbers2[i][1]);
    }*/
    
    printf("Nejvice vyskytu (%d x) mela cisla: ",max);
    for(i=0;i<=len2;i++)
        if(numbers2[i][0] == max){
            printf("%d", numbers2[i][1]);
            break;
        }
    
    for(i=i+1;i<=len2;i++)
        if(numbers2[i][0] == max)
            printf(", %d", numbers2[i][1]);
    printf("\n");
    
    return (EXIT_SUCCESS);
}

