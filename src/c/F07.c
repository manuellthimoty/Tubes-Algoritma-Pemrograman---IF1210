#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "F07.h"
#include "F02.h" // for roleToString

int compareUserById(const void* a, const void* b) {
    const User* userA = (const User*)a;
    const User* userB = (const User*)b;
    return userA->id - userB->id;
}

int compareUserByUsername(const void* a, const void* b) {
    const User* userA = (const User*)a;
    const User* userB = (const User*)b;
    return strcmp(userA->username, userB->username);
}

void TampilkanUser_Spesifik(UserSet* userSet, int sort, Short_KEY key, Short_KEY_Role role) {
    int banyaknyaUser = userSet->count;

    if (key == SORT_ID) qsort(userSet->users, banyaknyaUser, sizeof(User), compareUserById);
    else if (key == SORT_NAME) qsort(userSet->users, banyaknyaUser, sizeof(User), compareUserByUsername);

    if (role == SORT_DOKTER) {
        printf("ID  | Nama     | Aura\n");
        printf("------------------------\n");
        if (sort == 1) {
            for (int i = 0; i < banyaknyaUser; i++) {
                User u = userSet->users[i];
                if (u.role == ROLE_DOKTER)
                    printf("%-3d | %-8s | %-3d\n", u.id, u.username, u.aura);
            }
        } else {
            for (int i = banyaknyaUser - 1; i >= 0; i--) {
                User u = userSet->users[i];
                if (u.role == ROLE_DOKTER)
                    printf("%-3d | %-8s | %-3d\n", u.id, u.username, u.aura);
            }
        }
    } else if (role == SORT_PASIEN) {
        printf("ID  | Nama        | Penyakit\n");
        printf("-------------------------------\n");
        if (sort == 1) {
            for (int i = 0; i < banyaknyaUser; i++) {
                User u = userSet->users[i];
                if (u.role == ROLE_PASIEN)
                    printf("%-3d | %-8s  | %s\n", u.id, u.username, u.riwayat_penyakit);
            }
        } else {
            for (int i = banyaknyaUser - 1; i >= 0; i--) {
                User u = userSet->users[i];
                if (u.role == ROLE_PASIEN)
                    printf("%-3d | %-8s | %s\n", u.id, u.username, u.riwayat_penyakit);
            }
        }
    }
}

void TampilkanUser(UserSet* userSet, int sort, Short_KEY key) {
    int banyaknyaUser = userSet->count;

    if (key == SORT_ID) qsort(userSet->users, banyaknyaUser, sizeof(User), compareUserById);
    else if (key == SORT_NAME) qsort(userSet->users, banyaknyaUser, sizeof(User), compareUserByUsername);

    printf("ID  | Nama      | Role    | Penyakit\n");
    printf("-------------------------------------\n");

    if (sort == 1) {
        for (int i = 0; i < banyaknyaUser; i++) {
            User u = userSet->users[i];
            printf("%-3d | %-8s | %-7s | %s\n",
                   u.id, u.username, roleToString(u.role),
                   (u.role == ROLE_PASIEN ? u.riwayat_penyakit : "-"));
        }
    } else {
        for (int i = banyaknyaUser - 1; i >= 0; i--) {
            User u = userSet->users[i];
            printf("%-3d | %-8s | %-7s | %s\n",
                   u.id, u.username, roleToString(u.role),
                   (u.role == ROLE_PASIEN ? u.riwayat_penyakit : "-"));
        }
    }
}

void lihatUser(UserSet* userSet) {
    if (userSet->loggedInIndex == -1 || userSet->users[userSet->loggedInIndex].role != ROLE_MANAGER) {
        printf("Akses ditolak. Fitur ini hanya untuk Manager.\n");
        return;
    }

    char kembali[10];

    while (1) {
        printf("\n>>> LIHAT USER <<<\n");
        printf("1. Spesifik\n2. General\n>>> Pilihan: ");
        int input;
        scanf("%d", &input);

        if (input == 1) {
            printf("Role yang ingin dicari:\n1. Pasien\n2. Dokter\n>>> Pilihan: ");
            int pilihan_Role;
            scanf("%d", &pilihan_Role);

            if (pilihan_Role != 1 && pilihan_Role != 2) {
                printf("\nInput Role tidak valid!\n");
                continue;
            }

            printf("\nUrutkan berdasarkan:\n1. ID\n2. Nama\n>>> Pilihan: ");
            int pilihan;
            scanf("%d", &pilihan);

            printf("\nUrutan sort:\n1. ASC (A-Z)\n2. DESC (Z-A)\n>>> Pilihan: ");
            int sort;
            scanf("%d", &sort);

            if ((pilihan != 1 && pilihan != 2) || (sort != 1 && sort != 2)) {
                printf("\nInput Sort tidak valid!\n");
                continue;
            }

            Short_KEY key = (pilihan == 1) ? SORT_ID : SORT_NAME;
            Short_KEY_Role keyRole = (pilihan_Role == 1) ? SORT_PASIEN : SORT_DOKTER;
            TampilkanUser_Spesifik(userSet, sort, key, keyRole);

        } else if (input == 2) {
            printf("\nUrutkan berdasarkan:\n1. ID\n2. Nama\n>>> Pilihan: ");
            int pilihan;
            scanf("%d", &pilihan);

            printf("\nUrutan sort:\n1. ASC (A-Z)\n2. DESC (Z-A)\n>>> Pilihan: ");
            int sort;
            scanf("%d", &sort);

            if ((pilihan != 1 && pilihan != 2) || (sort != 1 && sort != 2)) {
                printf("\nInput Sort tidak valid!\n");
                continue;
            }

            Short_KEY key = (pilihan == 1) ? SORT_ID : SORT_NAME;
            TampilkanUser(userSet, sort, key);

        } else {
            printf("\nInput tidak valid!\n");
            continue;
        }

        // Opsi kembali ke menu utama
        printf("\n>>>>  TEKAN TOMBOL [M] UNTUK KEMBALI KE MAIN MENU  >>>>>\n");
        printf("Input: ");
        scanf("%s", kembali);
        if (strcmp(kembali, "M") == 0 || strcmp(kembali, "m") == 0) {
            break;
        }
    }
}
