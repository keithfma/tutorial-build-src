#include <stdio.h>
#include <math.h>

void main()
{ 
    int i;

    printf("\t Number \t\t Square Root of Number\n\n");

    for (i=0; i<=360; ++i)
        printf("\t %d \t\t\t %d \n", i, sqrt((double) i));

}
