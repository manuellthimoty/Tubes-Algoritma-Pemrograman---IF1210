#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  
#include "F02.h"

int CaseInsensitive(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (tolower(*s1) != tolower(*s2)) return 0;
        s1++; s2++;
    }
    return *s1 == *s2;
}

const char* roleToString(Role role) {
    switch (role) {
        case ROLE_MANAGER: return "manager";
        case ROLE_DOKTER:  return "dokter";
        case ROLE_PASIEN:  return "pasien";
        default:           return "unknown";
    }
}

void saveUserToCSV(User user, const char* filename, int id) {
    FILE* file = fopen(filename, "a");

    if (!file) {
        perror("Gagal membuka file untuk menulis.\n");
        return;
    }

    char buffer[300];
    if (user.role==ROLE_PASIEN){
        snprintf(buffer, sizeof(buffer), "%d;%s;%s;%s;%s;%f;%d;%d;%d;%f;%d;%f;%d;%d;%d;%d\n",
            id,
            user.username,
            user.password,
            roleToString(user.role),
            user.riwayat_penyakit,
            user.suhu_tubuh,
            user.tekanan_darah_sistolik,
            user.tekanan_darah_diastolik,
            user.detak_jantung,
            user.saturasi_oksigen,
            user.kadar_gula_darah,
            user.berat_badan,
            user.tinggi_badan,
            user.kadar_kolesterol,
            user.kadar_kolesterol_ldl,
            user.trombosit
        );
    }else{
        snprintf(buffer, sizeof(buffer), "%d;%s;%s;%s;;;;;;;;;;;;;;\n",
            id,
            user.username,
            user.password,
            roleToString(user.role)
        );
    }
    
    fputs(buffer, file);
    fclose(file);
}
void modify_user_csv(int line_number, const char *new_line) {
    FILE *original_file, *temp_file;
    char buffer[MAX_LINE];
    int current_line = 1;

    original_file = fopen(USER_CSV_PATH, "r");
    if (original_file == NULL) {
        perror("Error opening user.csv");
        exit(EXIT_FAILURE);
    }

    temp_file = fopen(TEMP_CSV_PATH, "w");
    if (temp_file == NULL) {
        perror("Error opening temp.csv");
        fclose(original_file);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, MAX_LINE, original_file) != NULL) {
        if (current_line == line_number) {
            size_t len = strlen(new_line);
            if (len > 0 && new_line[len - 1] == '\n') {
                fputs(new_line, temp_file);
            } else {
                fprintf(temp_file, "%s\n", new_line);
            }
        } else {
            fputs(buffer, temp_file);
        }
        current_line++;
    }

    fclose(original_file);
    fclose(temp_file);

    if (remove(USER_CSV_PATH) != 0) {
        perror("Error removing original file");
        exit(EXIT_FAILURE);
    }
    if (rename(TEMP_CSV_PATH, USER_CSV_PATH) != 0) {
        perror("Error renaming temporary file");
        exit(EXIT_FAILURE);
    }
}

void registerPasien(UserSet* userSet) {
    char username[50], password[50];
    printf(">>> REGISTER PASIEN\n");

    printf("Username: ");
    scanf("%49s", username);

    for (int i = 0; i < userSet->count; i++) {
        if (CaseInsensitive(userSet->users[i].username, username)) {
            printf("Registrasi gagal! Username %s sudah digunakan.\n", username);
            return;
        }
    }

    printf("Password: ");
    scanf("%49s", password);
    // printf("Dokter Penanggung Jawab: ");
    // scanf("%49s", assignedDokter);

    userSet->users = realloc(userSet->users, sizeof(User) * (userSet->count + 1));

    strcpy(userSet->users[userSet->count].username, username);
    strcpy(userSet->users[userSet->count].password, password);
    userSet->users[userSet->count].role = ROLE_PASIEN;
    //strcpy(userSet->users[userSet->count].assigned_dokter, assignedDokter); // hanya disimpan di memori

    int maxId = 0;
    for (int i = 0; i < userSet->count; i++) {
        if (i + 1 > maxId) maxId = i + 1;
    }

    saveUserToCSV(userSet->users[userSet->count], USER_CSV_PATH, maxId + 1);

    printf("Pasien %s berhasil ditambahkan.\n", username);

    userSet->count++;
}

int getIdFromUsername(const char* username, const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Gagal membuka file: %s\n", filepath);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char* token = strtok(line, ";");
        if (!token) continue;

        int id = atoi(token);
        token = strtok(NULL, ";");  
        if (!token) continue;

        if (CaseInsensitive(token, username)) {
            fclose(file);
            return id;
        }
    }

    fclose(file);
    return -1;
}
