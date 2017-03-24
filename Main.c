#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    for(int i = 0; i < argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void showMenu();
void insertInTable();
void selectFrom();
void deleteFrom();
void paramSelectFrom();
void getPhoto();
void updatePhoto(char *id, char *photoName);

sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;

int on = 1;

int main(int argc, char* argv[]) {
    rc = sqlite3_open("Database.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
    else {
        fprintf(stderr, "Opened database successfully.\n");
    }
    showMenu();
    sqlite3_close(db);
    return 0;
}

void showMenu() {
    int choice = 0;
    int autocom = 0;
    char id[3];
    char photoname[16];
    while (1) {
        printf("\nAutocommit mode.\n1.On\n2.Off\n3.Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &autocom);
        switch (autocom) {
            case 1:
                on = 1;
                break;
            case 2:
                on = 0;
                break;
            case 3:
                return;
        }

        printf("\n===== MENU =====\n\n");
        printf("0. Exit\n");
        printf("1. Insert data in database\n");
        printf("2. Select data from database\n");
        printf("3. Delete data from database\n");
        printf("4. Parametrized query\n");
        printf("5. Get photo\n");
        printf("6. Update photo.\n");

        printf("Enter your choice:");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                return;
            case 1:
                insertInTable();
                break;
            case 2:
                selectFrom();
                break;
            case 3:
                deleteFrom();
                break;
            case 4:
                paramSelectFrom();
                break;
            case 5:
                getPhoto();
                break;
            case 6:
                printf("Enter id: ");
                scanf("%s", id);
                printf("Enter photoname: ");
                scanf("%s", photoname);
                int len = strlen(photoname);
                photoname[len] = '\0';
                updatePhoto(id, photoname);
                break;
            default:
                printf("Incorrect choice.\n");
                break;
        }
    }

}

void insertInTable() {
    int len = 0;
    char firstname[16], middlename[16], lastname[16],birthday[12];
    char photoName[20], place[20], country[30], address[30];
    char department[10], position[14], startWork[12];
    char ch;
    char *str = malloc(sizeof(char) * 2000);
    if (on == 0) {
        sql = "BEGIN TRANSACTION;\n";
        strcpy(str, sql);
    }

    do {
        printf("\n= Enter your data for insert =\n");
        printf("Enter 0 if you want to stop operation.\n");
        printf("Firstname: ");
        scanf("%s", firstname);
        len = strlen(firstname);
        firstname[len] = '\0';
        if (strcmp(firstname, "0") == 0) {
            printf("Cancel.\n");
            return;
        }
        printf("Middlename: ");
        scanf("%s", middlename);
        len = strlen(middlename);
        middlename[len] = '\0';
        if (strcmp(middlename, "0") == 0) {
            printf("Cancel.\n");
            return;
        }

        printf("Lastname: ");
        scanf("%s", lastname);
        len = strlen(lastname);
        lastname[len] = '\0';
        if (strcmp(lastname, "0") == 0) {
            printf("Cancel.\n");
            return;
        }

        printf("Birthday: ");
        scanf("%s", birthday);
        len = strlen(birthday);
        birthday[len] = '\0';
        if (strcmp(birthday, "0") == 0) {
            printf("Cancel.\n");
            return;
        }
        /*
        printf("Photoname: ");
        scanf("%s", photoName);
        len = strlen(photoName);
        photoName[len] = '\0';
        if (strcmp(photoName, "0") == 0) {
            printf("Cancel.\n");
            return;
        }
        */
        printf("Place: ");
        scanf("%s", place);
        len = strlen(place);
        place[len] = '\0';
        if (strcmp(place, "0") == 0) {
            printf("Cancel.\n");
            return;
        }

        printf("Country: ");
        scanf("%s", country);
        len = strlen(country);
        country[len] = '\0';
        if (strcmp(country, "0") == 0) {
            printf("Cancel.\n");
            return;
        }

        printf("Address: ");
        scanf("%s", address);
        len = strlen(address);
        address[len] = '\0';
        if (strcmp(address, "0") == 0) {
            printf("Cancel.\n");
            return;
        }

        printf("Department: ");
        scanf("%s", department);
        len = strlen(department);
        department[len] = '\0';
        if (strcmp(department, "0") == 0) {
            printf("Cancel.\n");
            return;
        }

        printf("Position: ");
        scanf("%s", position);
        len = strlen(position);
        position[len] = '\0';
        if (strcmp(position, "0") == 0) {
            printf("Cancel.\n");
            return;
        }

        printf("Start of work: ");
        scanf("%s", startWork);
        len = strlen(startWork);
        startWork[len] = '\0';
        if (strcmp(startWork, "0") == 0) {
            printf("Cancel.\n");
            return;
        }


        strcat(str, "insert into workers (firstname, middlename, lastname, birthday, "
                "place, country, address, department, position, startWork) values(");

        strcat(str, "'");
        strcat(str, firstname);
        strcat(str, "', '");

        strcat(str, middlename);
        strcat(str, "', '");

        strcat(str, lastname);
        strcat(str, "', '");

        strcat(str, birthday);
        strcat(str, "', '");

        strcat(str, place);
        strcat(str, "', '");

        strcat(str, country);
        strcat(str, "', '");

        strcat(str, address);
        strcat(str, "', '");

        strcat(str, department);
        strcat(str, "', '");

        strcat(str, position);
        strcat(str, "', '");

        strcat(str, startWork);
        strcat(str, "');");

        printf("If you want to continue operation enter Y or y: ");
        scanf(" %c", &ch);
    } while (ch == 'Y' || ch == 'y');


    if (on == 0) {
        strcat(str, "COMMIT;\n");
    }

    rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    else {
        fprintf(stdout, "Record created successfully.\n");
    }
    sql = "";
    free(str);
}
void updatePhoto(char * id, char *photoName) {

    FILE *fp = fopen(photoName, "rb");

    if (fp == NULL) {

        fprintf(stderr, "Cannot open image file\n");
        return;
    }

    fseek(fp, 0, SEEK_END);

    int flen = ftell(fp);

    fseek(fp, 0, SEEK_SET);

    char data[flen + 1];
    int size = fread(data, 1, flen, fp);
    fclose(fp);

    sqlite3_stmt *pStmt;

    char str1[150]= "update workers set photo = ? where id = ";
    strcat(str1, id);
    strcat(str1, ";");

    rc = sqlite3_prepare(db, str1, -1, &pStmt, 0);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));

        return;
    }

    sqlite3_bind_blob(pStmt, 1, data, size, SQLITE_STATIC);

    rc = sqlite3_step(pStmt);

    if (rc != SQLITE_DONE) {

        printf("execution failed: %s", sqlite3_errmsg(db));
    }
    else {
        printf("Photo uploaded successfully\n");
    }
    sqlite3_finalize(pStmt);
}

void selectFrom() {
    const char* data = "Callback function called\n";
    sql = "select * from workers";
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully.\n");
    }
}

void deleteFrom() {
    printf("Enter parameter for deleting: ");
    printf("\n0. Exit\n1. ID\n2. Lastname\n");
    int choice = 0;
    scanf("%d", &choice);
    char id[10];
    char lastname[16];
    int len = 0;
    switch (choice) {
        case 0:
            return;
        case 1:
            printf("Enter ID: ");
            scanf("%s", id);
            sql = "delete from workers where id = ";
            char *str = malloc(sizeof(char) * 128);
            strcpy(str, sql);
            strcat(str, id);
            strcat(str, ";");
            rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);

            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else {
                fprintf(stdout, "Operation done successfully\n");
            }
            free(str);
            break;
        case 2:
            printf("Enter lastname: ");
            scanf("%s", lastname);
            len = strlen(lastname);
            lastname[len] = '\0';
            sql = "delete from workers where lastname = '";
            char *s = malloc(sizeof(char) * 128);
            strcpy(s, sql);
            strcat(s, lastname);
            strcat(s, "';" );
            rc = sqlite3_exec(db, s, callback, 0, &zErrMsg);

            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else {
                fprintf(stdout, "Operation done successfully\n");
            }

            free(s);
            break;
        default:
            printf("Incorrect choice.\n");
            break;
    }
}

void paramSelectFrom() {
    int choice = 0;
    int id = 0;
    char lastname[16], country[30];
    sqlite3_stmt * pStmt;
    printf("1.ID\n");
    printf("2.Lastname\n");
    printf("3.Country\n");
    printf("4.Exit\n");
    printf("Your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 0:
            return;

        case 1:
            sql = "select * from workers where id = ?";
            rc = sqlite3_prepare_v2(db, sql, -1,  &pStmt, 0);
            if (rc == SQLITE_OK) {
                printf("Enter id: ");
                scanf("%d", &id);
                sqlite3_bind_int(pStmt, 1, id);
            }
            else {
                fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
                return;
            }

            while(sqlite3_step(pStmt) == SQLITE_ROW) {
                int count = sqlite3_column_count(pStmt);
                for (int i = 0; i < count; i++)
                    printf("%s: %s\n", sqlite3_column_name(pStmt, i), sqlite3_column_text(pStmt, i));
            }
            sqlite3_finalize(pStmt);

            break;
        case 2:
            sql = "select * from workers where lastname = ?";
            rc = sqlite3_prepare_v2(db, sql, -1,  &pStmt, 0);
            if (rc == SQLITE_OK) {
                printf("Enter lastname: ");
                scanf("%s", lastname);
                sqlite3_bind_text(pStmt, 1, lastname, -1, SQLITE_TRANSIENT);
            }
            else {
                fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
                return;
            }

            while(sqlite3_step(pStmt) == SQLITE_ROW) {
                int count = sqlite3_column_count(pStmt);
                for (int i = 0; i < count; i++)
                    printf("%s: %s\n", sqlite3_column_name(pStmt, i), sqlite3_column_text(pStmt, i));
            }
            sqlite3_finalize(pStmt);
            break;
        case 3:
            sql = "select * from workers where country = ?";
            rc = sqlite3_prepare_v2(db, sql, -1,  &pStmt, 0);
            if (rc == SQLITE_OK) {
                printf("Enter country: ");
                scanf("%s", country);
                sqlite3_bind_text(pStmt, 1, country, -1, SQLITE_TRANSIENT);
            }
            else {
                fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
                return;
            }

            while(sqlite3_step(pStmt) == SQLITE_ROW) {
                int count = sqlite3_column_count(pStmt);
                for (int i = 0; i < count; i++)
                    printf("%s: %s\n", sqlite3_column_name(pStmt, i), sqlite3_column_text(pStmt, i));
            }
            sqlite3_finalize(pStmt);
            break;
        default:
            printf("INCORRECT CHOICE!\n");
            break;

    }
}

void getPhoto() {
    char id[4];
    printf("Enter id: ");
    scanf("%s", id);
    FILE *fp = fopen("woman2.jpg", "wb");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open image file\n");
        return;
    }

    sql = "select photo from workers where photo is not null and id = ";
    char str[128];
    strcat(str, sql);
    strcat(str, id);
    strcat(str, ";");

    sqlite3_stmt *pStmt;
    rc = sqlite3_prepare_v2(db, str, -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement\n");
        return;
    }

    rc = sqlite3_step(pStmt);
    int bt = 0;
    if (rc == SQLITE_ROW) {
        bt = sqlite3_column_bytes(pStmt, 0);
    }

    int i;
    i = fwrite(sqlite3_column_blob(pStmt, 0), bt, 1, fp);
    if (i < 1) {
        printf("\nThe photo doesnt exist\n");

    }
    else {
        printf("\nYou can see this photo in directory\n");
    }

    if (ferror(fp)) {

        fprintf(stderr, "fwrite() failed\n");

        return;
    }

    int r = fclose(fp);

    if (r == EOF) {
        fprintf(stderr, "Cannot close file handler\n");
    }

    rc = sqlite3_finalize(pStmt);

}