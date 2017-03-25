#include <stdio.h>
#include <ctype.h>

void enterReturn() {
	printf("Press 'Enter' to return.");
	getchar();
}

void bitBank() {
	printf("------- Bit Bank -------\n");
}

int again() {
	char ans;
	printf("Again? (y/n) ");
	ans = getchar();
	getchar();
	if(ans == 'y' || ans == 'Y')
		return 1;
	return 0;
}

int choice() {
	char input[16];
	printf("Enter your choice: ");
	gets(input);
	if(isNumber(input))
		return atoi(input);
	else
		return -1;
}

int isNumber(char* str) {
	if(str[0] == '\0')
		return 0;
	int i = 0;
	while(str[i] != '\0') {
		if(!isdigit(str[i]))
			return 0;
		i++;
	}
	return 1;
}
int isFloat(char* str) {
	if(str[0] == '\0')
		return 0;
	int i = 0;
	while(isdigit(str[i]))
		i++;
	if(str[i] == '\0')
		return 1;
	if(str[i] != '.')
		return 0;
	i++;
	while(str[i] != '\0') {
		if(!isdigit(str[i]))
			return 0;
		i++;
	}
	return 1;
}
int getInt(char* intStr, char* msg) {
repeat:
	printf("%s: ", msg);
	gets(intStr);
	if(intStr[0] == '\0')
		return 0;
	if(isNumber (intStr))
		return 1;
	printf("Enter correct value empty string to break.\n");
	goto repeat;

}
int getFloat(char* floatStr, char* msg) {
repeat:
	printf("%s: ", msg);
	gets(floatStr);
	if(floatStr[0] == '\0')
		return 0;
	if(isFloat(floatStr))
		return 1;
	printf("Enter correct value or empty string to break.\n");
	goto repeat;
}
