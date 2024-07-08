#include <stdio.h>
#include <string.h>
int main ()
{
    int numbers[6], i = 0, sum = 0, minimum = 0, maximum = 0;
    double average = 0;
    for(i = 0; i < 6; i++)
    {
        printf("Enter number %d:", i+1); //prompt the to enter a number
        scanf("%d", &numbers[i]);//scan the number
        sum += numbers[i];  //compute the sum and average
    }
    average = sum / 6.0; //6.0 so answer is not int
    minimum = numbers[0]; //compute the min and max
    for(i = 0; i < 6; i++)
    {
        if(numbers[i] < minimum) {minimum = numbers[i];}
    }
    maximum = numbers[0]; //compute the max
    for(i = 1; i < 6; i++)
    {
        if(numbers[i] > maximum) {maximum = numbers[i];}
    }
    printf("The maximum is %d\n", maximum);//print minimum
    printf("The minimum is %d\n", minimum);//print maximum
    printf("The average is %.3f\n", average);//print average
    printf("The sum is %d\n", sum);//print sum
    return 0;
}