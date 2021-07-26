#include <stdio.h>
#include <cs50.h>

// A comment to pass style check, it just prints hello, nothing special, unless ...
int main(void)
{
    // Gets string as an input and prints it
    string UserName = get_string("Name yourself at once!\n");
    printf("My greeting to you, %s\n", UserName);
}