#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <ctype.h>

sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;

int id = -1;
int isAdmin = -1;
char accountId[16];

static int callback_accountId(void *NotUsed, int argc, char **argv, char **azColName) {
	strcpy(accountId, argv[0]);
	return 0;
}

static int callback_login(void *NotUsed, int argc, char **argv, char **azColName) {
	id = atoi(argv[0]);
	isAdmin = atoi(argv[1]);
	return 0;
}

int idExist;
char type[16];
char balance[40];
char overdraft[16];
static int callback_exist(void *NotUsed, int argc, char **argv, char **azColName) {
	idExist = 1;
	return 0;
}

static int callback_client_overdraft(void *NotUsed, int argc, char **argv, char **azColName) {
		idExist = 1;
		strcpy(overdraft, argv[0]);
		return 0;
}

static int callback_clientId(void *result, int argc, char **argv, char **azColoName) {
		result = atoi(argv[0]);
}

static int callback_account_type(void *NotUsed, int argc, char **argv, char **azColName) {
		idExist = 1;
		strcpy(type, argv[0]);
		return 0;
}

static int callback_account_balance(void *NotUsed, int argc, char **argv, char **azColName) {
		idExist = 1;
		strcpy(balance, argv[0]);
		return 0;
}


void startMainMenu();
void logIn();
void startAdminMenu();
void addAccount();
void deleteAccount();
void changeAccount();
void addClient();
void deleteClient();
void addOverdraft();
void startOperatorMenu();
void depositMoneyOnCard();
void performTranslationBetweenAccounts();
int getPhoto(char* photo, int *size);
int getTransactionFee(char* id);

int isNumber(char* str);
int isFloat(char* str);
int getInt(char* intStr);
int getFloat(char* floatStr, char* msg);
int choice();
int again();

int getId(char* idStr, char* idtype, char* tableName, char* whatSelect, int callBack(void*, int, char**, char**)) {
	char sqlQuery[128];
repeat:
	printf("%s ID: ", idtype);
	gets(idStr);
	if(idStr[0] == '\0')
		return 0;
	if(isNumber (idStr))
		goto nextstep;
	printf("Wrong ID format. Enter correct id or empty string to break.\n");
	goto repeat;
nextstep:
	sprintf(sqlQuery, "SELECT %s from %s WHERE id=%s", whatSelect, tableName, idStr);
	rc = sqlite3_exec(db, sqlQuery, callBack, NULL, &zErrMsg);
	if(idExist) {
		idExist = 0;
		return 1;
	}
	else {
			printf("%s doesn't exist.\n", idtype);
			goto repeat;
	}
}


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
   		printf("Welcome to BitBank.\n");
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
	char login[128];
	char pass[128];
	char firstname[32];
	char lastname[32];
	char ans = '\0';
	system("clear");
	printf("Authorization\n");
	do {
			printf("Login:");
			gets(login);

			printf("Pass: ");
			gets(pass);
			char sqlQuery[256];
			sprintf(sqlQuery, "SELECT id, isAdmin from BANK_USERS WHERE login='%s' and password='%s'", login, pass);
			rc = sqlite3_exec(db, sqlQuery, callback_login, NULL, &zErrMsg);
			if(id != -1) {
					switch (isAdmin) {
							case 1:
									startAdminMenu();
									break;
							case 2:
									startOperatorMenu();
									break;
							default:
									printf("Error: admin value is not correct\n");
									return;
					}
					id = -1;
			}
			else {
					printf("Wrong login or password. Try again? (y/n) ");
					scanf("%c", &ans);
					getchar();
			}
	}
	while(ans == 'y' || ans == 'Y');

}

void startAdminMenu() {
	while(1) {
		system("clear");
		printf("\rAdmin terminal            \n");
		printf("0. Exit.\n");
		printf("1. Add account.\n");
		printf("2. Add client.\n");
		printf("3. Add overdraft.\n");
		printf("4. Change account.\n");
		printf("5. Delete account.\n");
		printf("6. Delete client.\n");
		int ans = choice();
		switch (ans) {
				case 0:
						return;
						break;
				case 1:
						addAccount();
						break;
				case 2:
						addClient();
						break;
				case 3:
						addOverdraft();
						break;
				case 4:
						changeAccount();
						break;
				case 5:
						deleteAccount();
						break;
				case 6:
						deleteClient();
						break;
		}
	}
}

void addOverdraft() {
	char clientId[16];
	system("clear");
	printf("Add overdraft.\n");
	do {
		if(!getId(clientId, "Client", "BANK_CLIENTS", "overdraft_debt", callback_client_overdraft))
			return;
		
	} while(again());
}

void addAccount() {
	char clientId[16];
	char balance[16];
	char accountType[16];
	char password[32];
	system("clear");
    do {
        printf("\n= Enter account data =\n");
        printf("Enter empty string if you want to stop operation.\n");
		if(!getId(clientId, "Client", "BANK_CLIENTS", "*", callback_exist))
			return;

        if (!getFloat(balance, "Balance")) {
            printf("Cancel.\n");
            return;
        }

        printf("Account type:\n");
		printf("1. Current\n");
		printf("2. Savings\n");
		int ans = choice();
		switch(ans) {
			case 1:
				strcpy(accountType, "Current");
				break;
			case 2:
				strcpy(accountType, "Savings");
				break;
			default:
            	printf("Cancel.\n");
            	return;
		}

        printf("Password: ");
		gets(password);
        if (password[0] == '\0') {
            printf("Cancel.\n");
            return;
        }

		char sqlQuery[255];
		sprintf(sqlQuery, "insert into BANK_ACCOUNTS (clientID, balance, type, password) values('%s', '%s', '%s', '%s')", clientId, balance, accountType, password);

	    rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
		sprintf(sqlQuery, "SELECT MAX(id) from BANK_ACCOUNTS");
		rc = sqlite3_exec(db, sqlQuery, callback_accountId, NULL, &zErrMsg);
		printf("AccoutntId: %s\n", accountId);
    } while (again());
}

void deleteAccount() {
	char input[16];
	system("clear");
	do {
		if(!getId(input, "Account", "BANK_ACCOUNTS", "*",  callback_exist))
			return;
		char sqlQuery[256];
		sprintf(sqlQuery, "DELETE from BANK_ACCOUNTS WHERE id=%s", input);
		rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
	} while(again());
}

void changeAccount() {
	char inputId[128];
	char inputNewType[64];
	system("clear");
	do {
			if(!getId(inputId, "Account", "BANK_ACCOUNTS", "type", callback_account_type))
				return;
			char sqlQuery[128];
			printf("Type: %s\n", type);
			printf("Set type: ");
			gets(inputNewType);
			sprintf(sqlQuery, "UPDATE BANK_ACCOUNTS SET type='%s' WHERE id=%s", inputNewType, inputId);
			rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
	} while(again());
	
}

void addClient() {
	char firstname[32];
	char lastname[32];
	char pasportNumber[32];
	char birthday[16];
	system("clear");
    do {
        printf("\n= Enter user data =\n");
        printf("Enter empty string if you want to stop operation.\n");
        printf("Firstname: ");
		gets(firstname);
        if (firstname[0] == '\0') {
            printf("Cancel.\n");
            return;
        }

        printf("Lastname: ");
		gets(lastname);
        if (strcmp(lastname, "") == 0) {
            printf("Cancel.\n");
            return;
        }

        printf("Pasport number: ");
		gets(pasportNumber);
        if (strcmp(pasportNumber, "") == 0) {
            printf("Cancel.\n");
            return;
        }
        printf("Birthday: ");
		gets(birthday);
        if (strcmp(birthday, "") == 0) {
            printf("Cancel.\n");
            return;
        }

		char* photo;
		int size;
		if(!getPhoto(photo, &size)) {
            printf("Cancel.\n");
            return;
        }
			
		char sqlQuery[255];
		sprintf(sqlQuery, "insert into BANK_CLIENTS (firstname, lastname, passportNumber, dateOfBirth, photo) values('%s', '%s', '%s', '%s', ?)", firstname, lastname, pasportNumber, birthday);
    	sqlite3_stmt *pStmt;
		rc = sqlite3_prepare(db, sqlQuery, -1, &pStmt, 0);   // подготовка Sql запроса для выполнения

		if (rc != SQLITE_OK) {
				fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
				return;
		}

		sqlite3_bind_blob(pStmt, 1, photo, size, SQLITE_STATIC); // вставляем нашу картинку


		rc = sqlite3_step(pStmt);

		if (rc != SQLITE_DONE) {

				printf("execution failed: %s", sqlite3_errmsg(db));
		}

		sqlite3_finalize(pStmt);

    } while (again());
}

void deleteClient() {
	char input[16];
	system("clear");
	do {
		if(getId(input, "Client", "BANK_CLIENTS", "*", callback_exist)) {
			char sqlQuery[255];
			sprintf(sqlQuery, "DELETE from BANK_CLIENTS WHERE id=%s", input);
	    	rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
		}
		else
			return;
	} while(again());
}

void startOperatorMenu() {
	while(1) {
		system("clear");
		printf("\rOperator terminal            \n");
		printf("0. Exit.\n");
		printf("1. Deposit money on card.\n");
		printf("2. Perform translation between accounts.\n");
		int ans = choice();
		switch (ans) {
				case 0:
						return;
						break;
				case 1:
						depositMoneyOnCard();
						break;
				case 2:
						performTranslationBetweenAccounts();
						break;
		}
	}
}

void depositMoneyOnCard() {
	char inputId[16];
	printf("Deposit money on card\n");
	system("clear");
	do {
		if(getId(inputId, "Account", "BANK_ACCOUNTS", "balance", callback_account_balance)) {
				char sqlQuery[255];
				char depositMoney[64];
				getFloat(depositMoney,"Deposit ($)");
				sprintf(sqlQuery, "UPDATE BANK_ACCOUNTS SET balance=%lf WHERE id=%s", atof(balance)+atof(depositMoney), inputId);
				rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
		}
		else
			return;
	} while(again());
}

void performTranslationBetweenAccounts() {
	char inputIdFrom[16];
	char inputIdTo[16];
	double balanceFrom;
	double balanceTo;
	printf("Perform translation between accounts.\n");
	system("clear");
	do {
		if(!getId(inputIdFrom, "Account from which money will transferred", "BANK_ACCOUNTS", "balance", callback_account_balance))
			return;
		balanceFrom = atof(balance);
		if(!getId(inputIdTo, "Account to which money will transferred", "BANK_ACCOUNTS", "balance", callback_account_balance))
			return;
		balanceTo = atof(balance);
		char sqlQuery[255];
		char translationMoney[64];
		getFloat(translationMoney, "Translate ($)");
		if(atof(translationMoney) > balanceFrom) {
			printf("Not enough money on first account.\n");
			enterReturn();
			return;
		}
		int commission;
		commission = getTransactionFee(inputIdFrom);

		sprintf(sqlQuery, "UPDATE BANK_ACCOUNTS SET balance=balance-%s WHERE id=%s", translationMoney, inputIdFrom);
		rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
		sprintf(sqlQuery, "UPDATE BANK_ACCOUNTS SET balance=balance+%s WHERE id=%s", translationMoney, inputIdTo);
		rc = sqlite3_exec(db, sqlQuery, NULL, NULL, &zErrMsg);
	} while(again());
}

int getPhoto(char* photo, int *size) {
	char photoName[128];
repeat:
	printf("Photo name: ");
	gets(photoName);
	if(photoName[0] == '\0')
		return 0;
    FILE *fp = fopen(photoName, "rb"); // открываем нашу картинку в режиме чтения.

    if (fp == NULL) {
		printf("Enter correct photo name or empty string to break.\n");
		goto repeat;
    }

    fseek(fp, 0, SEEK_END); // ставим указатель на конец файла, ибо нам надо узнать размер картинки
    if (ferror(fp)) {       //обработка ошибки

        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }

        return 0;
    }

    int flen = ftell(fp); // ftell возвращает размер картинки (количество байт от начала файла)

    if (flen == -1) {

        perror("error occurred");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }

        return 0;
    }

    fseek(fp, 0, SEEK_SET); // ставим указатель на начало файла

    if (ferror(fp)) {       // обработка ошибки

        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }

        return 0;
    }

    photo = malloc(sizeof(char)*(flen + 1));  // массив для хранения данных о картинке

    *size = fread(photo, 1, flen, fp);    // считывает данные из файла и сохраняет их в массиве data

    if (ferror(fp)) {

        fprintf(stderr, "fread() failed\n");
        int r = fclose(fp);

        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }

        return 0;
    }

    int r = fclose(fp);

    if (r == EOF) {
        fprintf(stderr, "Cannot close file handler\n");
    }

	return 1;
}

int getTransactionFee(char* accountId) {
	char sqlQuery[256]; 
	int clientId;
	char maxNumTransaction[16];

	sprintf(sqlQuery, "SELECT clientID from BANK_ACCOUNTS WHERE id=%s", accountId);
	rc = sqlite3_exec(db, sqlQuery, callback_clientId, &clientId, &zErrMsg);
	printf("%d", clientId);
	getchar();
	
}
