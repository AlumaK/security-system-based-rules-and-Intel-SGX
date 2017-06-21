/*
  ExceedingArrayLimits.c
*/

#pragma check_stack(off)

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int index;
	int value;
	int i;

	char indexStr[50];
	char valueStr[50];

	printf("enter the index number: ");
	gets(indexStr);
	printf("enter the new value: ");
	gets(valueStr);

	index = atoi(indexStr);
	value = atoi(valueStr);

	arr[index] = value;

	printf("array = ");
	for (i = 0; i < 10; i++)
	{
		printf("%d", arr[i]);
		if (i != 9)
		{
			printf(", ");
		}
	}
	printf("\n");
	return 0;
}
