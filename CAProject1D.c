#include <stdio.h>
#include <string.h>
int main ()
{
    int variable = 50;//declare variable
    int *pointer;//declare variable
    pointer = &variable; //Assigns the address of the variable to the pointer
    // Prints the address of the variable and the value stored
    printf("The address is %p and the value is %d\n", &variable, variable);
    // Changes the value of the variable
    variable = 60;
    // Prints the address and the value of the variable
    printf("The address is %p and the value is %d", pointer, *pointer);
    return 0;
}