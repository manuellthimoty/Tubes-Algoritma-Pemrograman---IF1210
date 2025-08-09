#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "F01.h"

void login(UserSet* userSet) {
    if (userSet->loggedInIndex != -1) {
        printf("Sudah login sebagai %s\n", userSet->users[userSet->loggedInIndex].username);
        return;
    }
    char inputUser[50], inputPass[50];
    printf(">>> LOGIN\nUsername: ");
    scanf("%s", inputUser);
    printf("Password: ");
    scanf("%s", inputPass);

    for (int i = 0; i < userSet->count; i++) {
        if (strcmp(userSet->users[i].username, inputUser) == 0) {
            if (strcmp(userSet->users[i].password, inputPass) == 0) {
                userSet->loggedInIndex = i;
                if (userSet->users[i].role == ROLE_PASIEN) {
                    printf("Selamat Pagi Pasien %s!\n", inputUser);
                } else if (userSet->users[i].role == ROLE_DOKTER) {
                    printf("Selamat Pagi Dokter %s!\n", inputUser);
                } else if (userSet->users[i].role == ROLE_MANAGER) {
                    printf("Selamat Pagi Manager %s!\n", inputUser);
                } else {
                    printf("Role tidak dikenali untuk pengguna yang bernama %s!\n", inputUser);
                }
                
                return;
            } else {
                printf("Username atau password salah untuk pengguna yang bernama %s!\n", inputUser);
                return;
            }
        }
    }

    printf("Tidak ada Manager, Dokter, atau pun Pasien yang bernama %s!\n", inputUser);
}

void loadUsers(UserSet* userSet, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File %s tidak ditemukan. Memulai dengan user kosong.\n", filename);
        userSet->users = NULL;
        userSet->count = 0;
        userSet->loggedInIndex = -1;
        return;
    }

    // Alokasikan memori untuk menyimpan user
    userSet->users = malloc(MAX_USERS * sizeof(User));
    if (!userSet->users) {
        printf("Gagal mengalokasikan memori untuk users.\n");
        fclose(file);
        return;
    }

    userSet->count = 0;
    userSet->loggedInIndex = -1;

    char line[MAX_LINE];
    fgets(line, MAX_LINE, file); // Lewati header

    while (fgets(line, MAX_LINE, file)) {
        if (userSet->count >= MAX_USERS) {
            printf("Jumlah user melebihi batas maksimum (%d).\n", MAX_USERS);
            break;
        }

        char fields[MAX_FIELDS_USER][MAX_FIELD_LEN] = {0};
        int field_index = 0;
        int char_index = 0;

        // Split line berdasarkan ';'
        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
            if (line[i] == ';') {
                fields[field_index][char_index] = '\0';
                field_index++;
                char_index = 0;
            } else {
                if (char_index < MAX_FIELD_LEN - 1)
                    fields[field_index][char_index++] = line[i];
            }
        }
        fields[field_index][char_index] = '\0';

        // Isi data ke dalam user
        User* u = &userSet->users[userSet->count];
        u->id = atoi(fields[0]);
        strcpy(u->username, fields[1]);
        strcpy(u->password, fields[2]);

        if (strcmp(fields[3], "pasien") == 0) {
            u->role = ROLE_PASIEN;
            strcpy(u->riwayat_penyakit, fields[4]);
            u->suhu_tubuh = atof(fields[5]);
            u->tekanan_darah_sistolik = atoi(fields[6]);
            u->tekanan_darah_diastolik = atoi(fields[7]);
            u->detak_jantung = atoi(fields[8]);
            u->saturasi_oksigen = atof(fields[9]);
            u->kadar_gula_darah = atoi(fields[10]);
            u->berat_badan = atof(fields[11]);
            u->tinggi_badan = atoi(fields[12]);
            u->kadar_kolesterol = atoi(fields[13]);
            u->kadar_kolesterol_ldl = atoi(fields[14]);
            u->trombosit = atoi(fields[15]);
            //u->penyakit = NULL;
            // u->penyakit = malloc(sizeof(Penyakit));
            // if (u->penyakit == NULL) {
            //     //printf("Gagal mengalokasikan memori untuk penyakit user ID %d.\n", u->id);
            //     continue;  
            // }
            // *u->penyakit = make_Empty_Penyakit();
            // u->jumlah_obat_inventory = 0;
            // u->jumlah_obat_yang_diminum = 0;
            
            // strcpy(u->penyakit->nama_penyakit, u->riwayat_penyakit);
            
        } else if (strcmp(fields[3], "dokter") == 0) {
            u->role = ROLE_DOKTER;
            //u->aura=0;
            
        } else if (strcmp(fields[3], "manager") == 0) {
            u->role = ROLE_MANAGER;
        } else {
            u->role = ROLE_NONE;
        }
        //printf("LOAD USER BERHASIL");

        userSet->count++;
    }

    fclose(file);
}
