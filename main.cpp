#include <stdio.h>
#include <string.h>

int main()
{
    char str[] = "strtok needs to be called several times to split a string";

    char delim[] = " ";

    char *ptr = strtok(str, delim);

    while(ptr != NULL)
    {
        printf("'%s'", ptr);
        ptr = strtok(NULL, delim);
    }
    printf("\n");

    return 0;
}