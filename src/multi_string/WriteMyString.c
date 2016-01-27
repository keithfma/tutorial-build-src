
/*
 *	WriteMyString.c
 */

#include <stdio.h>

extern char *AnotherString;

void WriteMyString(char *ThisString)
{
    printf("%s\n", ThisString);
    printf("Global Variable = %s\n", AnotherString);
}
