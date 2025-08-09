#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "F18.h"

void EXIT(UserSet* userSet, Data_sistem* sistem) {
    char input[10];
    printf(">>>🏃‍♂️ EXIT\n");

    while (1) {
        if (userSet->loggedInIndex == -1) {
            printf("Anda belum login! Silakan login terlebih dahulu.\n");
            return;
        }
        printf("⚠️ Apakah Anda mau melakukan penyimpanan file yang sudah diubah? (y/n) ");
        scanf("%s", input);

        for (int i = 0; input[i]; i++) {
            if (input[i] >= 'A' && input[i] <= 'Z') {
                input[i] = input[i] + ('a' - 'A');
            }
        }

        if (strcmp(input, "y") == 0) {
            printf("Terimakasih Atas Dedikasi Anda Selama Ini Terhadap Rumah Sakit Nimons, Jasa Anda Akan Selalu Kami Ingat. File dan Data Anda Sudah Tersimpan\n");
            saveConfig(sistem, CONFIG_PATH,userSet);
            saveAllUsersToCSV(userSet, USER_CSV_PATH);

            free(userSet->users);
            exit(0);
        } else if (strcmp(input, "n") == 0) {
            printf("Keluar Program\n");
            free(userSet->users);
            exit(0);
        } else {
            printf("Input Tidak Valid! Harap Masukan Dengan Benar.\n");
        }
    }
}