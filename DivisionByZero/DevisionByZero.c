/*
  DevisionByZero.c
*/

#pragma check_stack(off)

#include <stdio.h> 

int main(int argc, char* argv[])
{
 	if (argc != 3) 
	{
		printf("Please supply a string as an argument!\n");
		return -1;
	} 
	//printf("enter a number 1:");
	//char num1str[50];
	//fgets(num1str, 50, stdin);
	//printf("enter a number 2:");
	//char num2srt[50];
	//fgets(num2srt, 50, stdin);

	int num1 = atoi(argv[1]);
	int num2 = atoi(argv[2]);

	int result = num1 / num2;

	printf("%d/%d = %d\n", num1, num2, result);

	return 0;
}
