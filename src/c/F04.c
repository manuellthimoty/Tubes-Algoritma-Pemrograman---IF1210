#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "F04.h"
Role convert_string_to_role(const char* str) {
    if (strcmp(str, "manager") == 0) return ROLE_MANAGER;
    else if (strcmp(str, "dokter") == 0) return ROLE_DOKTER;
    else if (strcmp(str, "pasien") == 0) return ROLE_PASIEN;
    return ROLE_NONE;
}

void Parse_File_user(char** nama, Role* role, int* count) {
    FILE* f = fopen(USER_CSV_PATH, "r");
    char line[300];
    *count = 0;

    if (!f) {
        printf("Error opening file\n");
        return;
    }

    fgets(line, sizeof(line), f); 

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        char buffer[MAX_FIELDS_USER][MAX_FIELD_LEN];
        int col = 0, idx = 0, i = 0;

        while (line[i] != '\0' && col < 4) {
            if (line[i] == ';') {
                buffer[col][idx] = '\0';
                col++;
                idx = 0;
                i++;
                continue;
            }
            buffer[col][idx++] = line[i++];
        }

        buffer[col][idx] = '\0'; 
        if (col < 3) continue;

        nama[*count] = malloc(strlen(buffer[1]) + 1);
        strcpy(nama[*count], buffer[1]);
        role[*count] = convert_string_to_role(buffer[3]);
        (*count)++;
    }

    fclose(f);
}

boolean nama_valid(char* _nama, char** _namavalid, int panjang) {
    for (int i = 0; i < panjang; i++) {
        if (strcmp(_nama, _namavalid[i]) == 0) return true;
    }
    return false;
}

Role get_role_by_name(char* nama, char** nama_valid, Role* role_valid, int panjang) {
    for (int i = 0; i < panjang; i++) {
        if (strcmp(nama, nama_valid[i]) == 0) {
            return role_valid[i];
        }
    }
    return ROLE_NONE;
}

char* Lenght_Encoding(char* input) {
    int len = strlen(input), j = 0, rlen;
    char cadangan[50];
    char* hasil = (char*)malloc(sizeof(char) * (len * 2 + 1));
    for (int i = 0; i < len; i++) {
        hasil[j++] = input[i];
        rlen = 1;
        while (i + 1 < len && input[i] == input[i + 1]) {
            rlen++;
            i++;
        }
        sprintf(cadangan, "%d", rlen);
        for (int k = 0; cadangan[k]; k++, j++) {
            hasil[j] = cadangan[k];
        }
    }
    hasil[j] = '\0';
    return hasil;
}

void Buat_Password_Baru(User* a) {
    char username[100], password[100];
    printf("\n >>> LOGIN \n");
    printf("Halo %s %s, silakan daftarkan ulang password anda!\n", 
        a->role == ROLE_MANAGER ? "manager" :
        a->role == ROLE_DOKTER ? "dokter" :
        a->role == ROLE_PASIEN ? "pasien" : "none",
        a->username);
    
    printf("Username: ");
    scanf("%99s", username);

    if (strcmp(username, a->username) == 0) {
        strcpy(a->username, username);
        printf("\nPassword: ");
        scanf("%99s", password);
        if (strcmp(password, "") != 0) {
            strcpy(a->password, password);

            printf("\nSelamat pagi %s %s\n", 
                a->role == ROLE_MANAGER ? "manager" :
                a->role == ROLE_DOKTER ? "dokter" :
                a->role == ROLE_PASIEN ? "pasien" : "none",
                a->username);
        }
        int Id = getIdFromUsername(username,USER_CSV_PATH);
        saveUserToCSV(*a,USER_CSV_PATH,Id);
    }
}

void lupa_password_input(User *a) {
    int count = 0;
    char nama[100], kodeunik[100], *kodeunik_harusnya;
    char* namavalid[100];
    Role rolevalid[100];

    printf("\n>>> LUPA_PASSWORD \n");
    printf("\nUsername: ");
    scanf("%99s", nama);

    Parse_File_user(namavalid, rolevalid, &count);

    if (nama_valid(nama, namavalid, count)) {
        strcpy(a->username, nama);
        a->role = get_role_by_name(a->username, namavalid, rolevalid, count);

        printf("Halo %s %s, silakan daftarkan ulang password anda!", 
            a->role == ROLE_MANAGER ? "manager" :
            a->role == ROLE_DOKTER ? "dokter" :
            a->role == ROLE_PASIEN ? "pasien" : "none",
            nama);

        kodeunik_harusnya = Lenght_Encoding(a->username);

        printf("\nKode Unik: ");
        scanf("%99s", kodeunik);

        if (strcmp(kodeunik, kodeunik_harusnya) != 0) {
            printf("Kode Unik Salah\n");
        } else {
            Buat_Password_Baru(a);
        }

        free(kodeunik_harusnya);
    } else {
        printf("Username not found.\n");
    }

    for (int i = 0; i < count; i++) {
        free(namavalid[i]);
    }
}
