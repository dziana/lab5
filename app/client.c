#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <ctype.h>

sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;

int accountExist = 0;
int accountId;
char clientId;
double accountBalance;
char accountType[16];
static int callback_account(void *NotUsed, int argc, char **argv, char **azColName) {
	if(argc == 0) 
		return 0;
	accountExist = 1;
	clientId = atoi(argv[0]);
	accountBalance = atof(argv[1]);
	strcpy(accountType, argv[2]);
	return 0;
}

void startMainMenu();
void logIn();
void startClientMenu();
void accountInformation();
void credit();
void debit();


int isNumber(char* str);
int isFloat(char* str);
int getInt(char* intStr, char* msg);
int getFloat(char* floatStr, char* msg);


int choice();
int again();
void enterReturn();
void bitBank();
int main(int argc, char* argv[]) {
    rc = sqlite3_open("Banking_DB.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
    else {
        fprintf(stderr, "Opened database successfully.\n");
    }
	startMainMenu();
    sqlite3_close(db);
    return 0;
}

void startMainMenu() {
	while(1) {
		system("clear");
   		printf("Welcom to BitBank.\n");
		printf("0. Exit\n1. Log in\n");
		int ans = choice();
        switch (ans) {
            case 0:
				return;
            case 1:
				logIn();
                break;
            default:
				printf("Wrong choice.\n\n");
        }
	}
}

void logIn() {
	char accountNumber[128];
	char pass[128];
	char ans = '\0';
	system("clear");
	printf("Authorization\n");
	login:
	printf("Account number: ");
	gets(accountNumber);
	printf("Pass: ");
	gets(pass);
	char sqlQuery[256];
	sprintf(sqlQuery, "SELECT clientID,  balance, type from BANK_ACCOUNTS WHERE id=%s and password='%s'", accountNumber, pass);
    rc = sqlite3_exec(db, sqlQuery, callback_account, NULL, &zErrMsg);
	if(accountExist) {
			accountId = atoi(accountNumber);
			startClientMenu();
			accountExist = 0;
	}
	else {
		printf("Wrong account number or password. Try again? (y/n) ");
		scanf("%c", &ans);
	}
	if(ans == 'y' || ans == 'Y')
		goto login;

}

void startClientMenu() {
	while(1) {
		system("clear");
		bitBank();
		printf("\rClient menu            \n");
		printf("0. Exit.\n");
		printf("1. Account information.\n");
		printf("2. Credit.\n");
		printf("3. Debit.\n");
		int ans = choice();
		switch (ans) {
				case 0:
						return;
						break;
				case 1:
						accountInformation();
						break;
				case 2:
						credit();
						break;
				case 3:
						debit();
						break;
		}
	}
}

void accountInformation() {
	system("clear");
	bitBank();
	printf("Account information\n");
	printf("Balance: %lf\n", accountBalance);
	printf("Type: %s\n", accountType);
	enterReturn();
}

void credit() {
	char input[32];
	system("clear");
	bitBank();
	printf("Credit\n");
	if(!getFloat(input, "Deposit"))
		return;
	char sqlQuery[256];
	sprintf(sqlQuery, "UPDATE BANK_ACCOUNTS SET balance=balance+%s WHERE id=%d", input, accountId);
    rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
	accountBalance += atof(input);

	printf("The operation was carried out successfully.\n");
	enterReturn();
}

void debit() {
	char input[32];
	system("clear");
	bitBank();
	printf("Debit\n");
	if(!getFloat(input, "Withdraw"))
		return;
	if(atof(input) > accountBalance) {
		printf("Not enough money.\n");
		enterReturn();
		return;
	}
	char sqlQuery[256];
	sprintf(sqlQuery, "UPDATE BANK_ACCOUNTS SET balance=balance-%s WHERE id=%d", input, accountId);
    rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
	accountBalance -= atof(input);
	printf("The operation was carried out successfully.\n");
	enterReturn();
}

