/* vulnerable.c */
#include<stdio.h>
#include<string.h>

void overflow()
{	
char buffer[7];

FILE *input = fopen("badfile", "r");

fread(buffer, sizeof(char), 40, input);

printf("Read from file: %s\n", buffer);
}
int main(int argc, char *argv[]) {

overflow();

return 0;
}

