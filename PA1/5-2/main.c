/* 
 * File:   main.c
 * Author: Tomires
 *
 * Created on 1. prosinec 2013, 16:52
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * 
 */

double dblabs(double n){
    if(n > 0) return n;
    else return -n;
}

int main(int argc, char** argv) {
    double **letadla = (double**)malloc(3*sizeof(double*));
    int len, res;
    char ch1, ch2, ch3;
    len = 0;
    
    printf("Zadejte souradnice:\n");
    while(1){
        letadla = (double**) realloc (letadla, (len+1) * sizeof (double*));
        letadla[len] = (double*) malloc (2*sizeof (double));
        
        ch1 = ' '; ch2 = ' '; ch3 = ' ';
        
        res = scanf(" %c %lf %c %lf %c",&ch1,&letadla[len][0],&ch2,&letadla[len][1],&ch3);
        
        //printf("%c",ch3);
        
        if(ch1 == '[' && (ch2 != ',' || ch3 != ']')){
            free(letadla);
            printf("Nespravny vstup.\n");
            return (EXIT_FAILURE);
        }
            
        
        //if(res == -1) break;
        if(feof(stdin)) break;
        
        len++;
        
        if(res != 5 || ch1 != '[' || ch2 != ',' || ch3 != ']'){
            free(letadla);
            printf("Nespravny vstup.\n");
            return (EXIT_FAILURE);
        }
    }
    
    if(len < 2){
        free(letadla);
        printf("Nespravny vstup.\n");
        return (EXIT_FAILURE);
    }
    
    double minval;
    int count;
    //printf("%lf %lf %lf %lf\n",letadla[0][0],letadla[0][1],pow(dblabs(letadla[1][0]-letadla[0][0]),2),pow(dblabs(letadla[1][1]-letadla[1][0]),2));
    minval = sqrt(pow(dblabs(letadla[1][0]-letadla[0][0]),2)
                + pow(dblabs(letadla[1][1]-letadla[0][1]),2));
    count = 0;
    
    int i, y;
    double temp;
    for(i=0;i<len-1;i++)
	for(y=i+1;y<len;y++){
            temp = sqrt(pow(dblabs(letadla[i][0]-letadla[y][0]),2)
                      + pow(dblabs(letadla[i][1]-letadla[y][1]),2));
            //printf("xVzdalenost nejblizsich letadel: %lf | %d\n",temp, count);
            if(temp < minval && (abs(temp - minval) > 0.000000000001)){
                minval = temp;
                count = 1;
            }
            else if(abs(temp - minval) < 0.000000000001)
                count++;
        }
            
    free(letadla);
    printf("Vzdalenost nejblizsich letadel: %g\n",minval);
    printf("Nalezenych dvojic: %d\n",count);
    return (EXIT_SUCCESS);
}

