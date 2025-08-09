#include <stdio.h>
#include <string.h>
#include "F01.h"
#include "F02.h"
#include "F10.h"
#include "F06.h"
void intToStr(int value, char* buffer) {
    sprintf(buffer, "%d", value); 
    //int i = 0, j, isNegative = 0;
    // if (value == 0) {
    //     buffer[i++] = '0';
    //     buffer[i] = '\0';
    //     return;
    // }
    // if (value < 0) {
    //     isNegative = 1;
    //     value = -value;
    // }
    // while (value > 0) {
    //     buffer[i++] = (value % 10) + '0';
    //     value /= 10;
    // }
    // if (isNegative) buffer[i++] = '-';
    // buffer[i] = '\0';
    // for (j = 0; j < i / 2; ++j) {
    //     char temp = buffer[j];
    //     buffer[j] = buffer[i - j - 1];
    //     buffer[i - j - 1] = temp;
    // }
}

void floatToStr(float value, char* buffer) {
    int intPart = (int)value;
    int fracPart = (int)((value - intPart) * 10 + 0.5);
    char intBuf[16], fracBuf[4];
    intToStr(intPart, intBuf);
    intToStr(fracPart, fracBuf);
    strcpy(buffer, intBuf);
    strcat(buffer, ".");
    strcat(buffer, fracBuf);
}

void saveAllUsersToCSV(UserSet* userSet, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file untuk menulis ulang data.\n");
        return;
    }

    const char* header = "id;username;password;role;riwayat_penyakit;suhu_tubuh;tekanan_darah_sistolik;tekanan_darah_diastolik;detak_jantung;saturasi_oksigen;kadar_gula_darah;berat_badan;tinggi_badan;kadar_kolesterol;kadar_kolesterol_ldl;trombosit\n";
    fwrite(header, sizeof(char), strlen(header), file);

    char buffer[64];

    for (int i = 0; i < userSet->count; i++) {
        User* user = &userSet->users[i];
        const char* roleStr = roleToString(user->role);

        // Tulis ID
        intToStr(user->id, buffer);
        fwrite(buffer, sizeof(char), strlen(buffer), file);
        fwrite(";", sizeof(char), 1, file);

        // Tulis username, password, role
        fwrite(user->username, sizeof(char), strlen(user->username), file);
        fwrite(";", sizeof(char), 1, file);
        fwrite(user->password, sizeof(char), strlen(user->password), file);
        fwrite(";", sizeof(char), 1, file);
        fwrite(roleStr, sizeof(char), strlen(roleStr), file);
        fwrite(";", sizeof(char), 1, file);
        // Tulis spek lainnya kalo pasien
        if (user->role == ROLE_PASIEN) {
            fwrite(user->riwayat_penyakit, sizeof(char), strlen(user->riwayat_penyakit), file);
            fwrite(";", sizeof(char), 1, file);

            floatToStr(user->suhu_tubuh, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            intToStr(user->tekanan_darah_sistolik, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            intToStr(user->tekanan_darah_diastolik, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            intToStr(user->detak_jantung, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            floatToStr(user->saturasi_oksigen, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            intToStr(user->kadar_gula_darah, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            floatToStr(user->berat_badan, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            intToStr(user->tinggi_badan, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            intToStr(user->kadar_kolesterol, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            intToStr(user->kadar_kolesterol_ldl, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file); fwrite(";", 1, 1, file);
            intToStr(user->trombosit, buffer); fwrite(buffer, sizeof(char), strlen(buffer), file);
        } else {
            for (int j = 0; j < 12; j++) {
                fwrite(";", 1, 1, file);
            }
        }

        fwrite("\n", sizeof(char), 1, file);
    }

    fclose(file);
}

// int samaIgnoreCase(const char* a, const char* b) {
//     while (*a && *b) {
//         if (tolower(*a) != tolower(*b)) return 0;
//         a++; b++;
//     }
//     return *a == '\0' && *b == '\0';
// }

int generateId(UserSet* userSet) {
    int maxId = 0;
    for (int i = 0; i < userSet->count; i++) {
        if (userSet->users[i].id > maxId) {
            maxId = userSet->users[i].id;
        }
    }
    return maxId + 1;
}

void tambahDokter(UserSet* userSet) {
    if (userSet->loggedInIndex == -1 || userSet->users[userSet->loggedInIndex].role != ROLE_MANAGER) { 
        printf("Akses ditolak. Fitur ini hanya untuk Manager.\n");
        return;
    }

    User newUser;
    printf(">>> TAMBAH_DOKTER\n");

    printf("Username: ");
    scanf("%s", newUser.username);

    for (int i = 0; i < userSet->count; i++) {
        if (strcmp(userSet->users[i].username, newUser.username) == 0 &&
            userSet->users[i].role == ROLE_DOKTER) {
            printf("Sudah ada Dokter dengan username %s!\n", newUser.username);
            return;
        }
    }

    printf("Password: ");
    scanf("%s", newUser.password);

    newUser.role = ROLE_DOKTER;
    newUser.riwayat_penyakit[0] = '\0';
    newUser.id = generateId(userSet);
    newUser.suhu_tubuh = 0;
    newUser.tekanan_darah_sistolik = 0;
    newUser.tekanan_darah_diastolik = 0;
    newUser.detak_jantung = 0;
    newUser.saturasi_oksigen = 0;
    newUser.kadar_gula_darah = 0;
    newUser.berat_badan = 0;
    newUser.tinggi_badan = 0;
    newUser.kadar_kolesterol = 0;
    newUser.kadar_kolesterol_ldl = 0;
    newUser.trombosit = 0;
    newUser.assigned_dokter[0] = '\0';

    userSet->users[userSet->count++] = newUser;

    printf("Dokter %s berhasil ditambahkan!\n", newUser.username);
    // printf(">>> LOGOUT\n");
    // userSet->loggedInIndex = -1;
    // printf("Sampai jumpa!\n");
}



void assignDokter(UserSet* userSet, Data_sistem* sistem) {
    if (userSet->loggedInIndex == -1 || userSet->users[userSet->loggedInIndex].role != ROLE_MANAGER) {
        printf("Akses ditolak. Fitur ini hanya untuk Manager.\n");
        return;
    }

    char username[50], ruangan[10];
    printf(">>>🏷️ ASSIGN_DOKTER\n");
    printf("📝 Username: ");
    scanf("%s", username);
    printf("🗺️ Ruangan: ");
    scanf("%s", ruangan);

    int ruanganIndex = getRuanganIndex(ruangan, sistem);
    if (ruanganIndex == -1) {
        printf("Kode ruangan tidak valid!\n");
        return;
    }
    int idx = -1;
    for (int i = 0; i < userSet->count; i++) {
        if (strcmp(userSet->users[i].username, username) == 0 &&
            userSet->users[i].role == ROLE_DOKTER) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Dokter %s belum terdaftar!\n", username);
        return;
    }

    int dokter_id = userSet->users[idx].id;
    if (getValueMap(&sistem->dokter_ke_idx_ruangan, dokter_id) != IDX_UNDEF) {
        printf("Dokter %s sudah ditugaskan ke ruangan lain. Tidak bisa assign ulang!\n", username);
        return;
    }
    if (sistem->daftar_ruangan[ruanganIndex].dokter_di_ruangan != NULL) {
        printf("Ruangan %s sudah ditempati oleh Dr. %s\n",
               ruangan,
               sistem->daftar_ruangan[ruanganIndex].dokter_di_ruangan->username);
        return;
    }

    strcpy(userSet->users[idx].assigned_dokter, ruangan);
    sistem->daftar_ruangan[ruanganIndex].dokter_di_ruangan = &userSet->users[idx];
    insertMap(&sistem->dokter_ke_idx_ruangan, dokter_id, ruanganIndex);

    printf("➕ Dokter %s berhasil diassign ke ruangan %s!\n", username, ruangan);
    saveConfig(sistem,CONFIG_PATH,userSet);
}
