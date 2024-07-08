#include <stdio.h>
#include <stdio.h>
#include <string.h>
int main ()
{
    char name1[50]; //declare characters
    char name2[50];
    char temp[50];
    printf("Enter name 1:");//prompt for name 1 and scan it
    scanf("%s", name1);
    printf("Enter name 2:");//prompt for name 2 and scan it
    scanf("%s", name2);
    //print name 1 and name 2 before swapping
    printf("Name 1 before swapping is: %s, and name 2 before swapping is %s\n", name1, name2);
    //swap names using temp
    strcpy(temp, name1);
    strcpy(name1, name2);
    strcpy(name2, temp);
    //print name 1 and name 2 after swapping
    printf("Name 1 after swapping is: %s, and name 2 after swapping is %s\n", name1, name2);
    return 0;
}