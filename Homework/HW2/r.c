#include <stdio.h>
#include <sting.h>

int main()
{
char tokenstring[] = "12 23 3 4 5";
char seps[] = " ";
char* token;
int var;
int input[5];
int i = 0;

token = strtok (tokenstring, seps);
while (token != NULL)
{
    sscanf (token, "%d", &var);
    input[i++] = var;

    token = strtok (NULL, seps);
}
}
