/* 
 * File:   main.c
 * Author: Tomires
 *
 * Created on 24. říjen 2013, 22:46
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * 
 */

double prekryv(double r1, double r2, double dist){
    if(r1>r2)
        return pow(r2,2)*acos((pow(dist,2) + pow(r2,2) - pow(r1,2))/(2*dist*r2)) 
            + pow(r1,2)*acos((pow(dist,2) + pow(r1,2) - pow(r2,2))/(2*dist*r1)) 
            - sqrt((-dist+r1+r2)*(dist+r2-r1)*(dist-r2+r1)*(dist+r1+r2))/2;
    else
        return pow(r1,2)*acos((pow(dist,2) + pow(r1,2) - pow(r2,2))/(2*dist*r1)) 
            + pow(r2,2)*acos((pow(dist,2) + pow(r2,2) - pow(r1,2))/(2*dist*r2)) 
            - sqrt((-dist+r1+r2)*(dist+r1-r2)*(dist-r1+r2)*(dist+r1+r2))/2;
}

double absF(double vstup){
    if(vstup<0)
        return vstup * (-1);
    else
        return vstup;
}

int main(int argc, char** argv) {
    double konst_pres = 0.000000001;
    
    double x1, y1, r1;
    printf("Zadejte parametry kruznice #1:\n");
    int result = scanf("%lf %lf %lf", &x1, &y1, &r1);
    if(result < 3 || r1 <= 0){
        printf("Nespravny vstup.\n");
        return (EXIT_FAILURE);
    }
    
    double x2, y2, r2;
    printf("Zadejte parametry kruznice #2:\n");
    result = scanf("%lf %lf %lf", &x2, &y2, &r2);
    if(result < 3 || r2 <= 0){
        printf("Nespravny vstup.\n");
        return (EXIT_FAILURE);
    }    
    
    /* x1=-48.2991131647392e-31;
         y1=-27.9464314187738e-31;
         r1=12.7254519795283e-31;
         x2=-25.2082785966678e-31;
         y2=-47.4113147848634e-31;
         r2=17.4750169821536e-31;*/
         
    //double dist = sqrt(pow(absF(x2-x1),2)+pow(absF(y2-y1),2));
    double dist = sqrt(absF(x2-x1)*absF(x2-x1)+absF(y2-y1)*absF(y2-y1));
    //printf("Dist: %.32f\n",dist);
    //printf("R1-R2: %.32f\n",r1-r2);
    //printf("R1+R2: %.32f\n",r1+r2);
    
    if(dist < r1 + r2){
        if(dist > absF(r1 - r2))
            printf("Kruznice se protinaji, prekryv: %f\n",prekryv(r1,r2,dist));
        else if(absF(dist - absF(r1 - r2)) < konst_pres)
            if((absF(x1 - x2)<konst_pres) && (absF(y1 - y2)<konst_pres) && (absF(r1 - r2)<konst_pres))
                printf("Kruznice splyvaji, prekryv: %f\n",(M_PI*pow(r1,2)));
            else{
                if(r1 > r2)
                    printf("Vnitrni dotyk, kruznice #2 lezi uvnitr kruznice #1, prekryv: %f\n",(M_PI*pow(r2,2)));
                else
                    printf("Vnitrni dotyk, kruznice #1 lezi uvnitr kruznice #2, prekryv: %f\n",(M_PI*pow(r1,2)));
            }
        else{
            if(r1 > r2)
                printf("Kruznice #2 lezi uvnitr kruznice #1, prekryv: %f\n",(M_PI*pow(r2,2)));
            else
                printf("Kruznice #1 lezi uvnitr kruznice #2, prekryv: %f\n",(M_PI*pow(r1,2)));
        }
    }else if(absF(dist - (r1 + r2)) < konst_pres){
        printf("Vnejsi dotyk, zadny prekryv.\n");
    }else{
        printf("Kruznice lezi vne sebe, zadny prekryv.\n");
    }
    
    return (EXIT_SUCCESS);
}

