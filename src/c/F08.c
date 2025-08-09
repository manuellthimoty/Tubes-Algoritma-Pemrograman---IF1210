#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "F08.h"

// Helper Functions
void sortUsers(UserSet* userset, int sortKey) {
    if (sortKey == SORT_ID) {
        qsort(userset->users, userset->count, sizeof(User), compareUserById);
    } else if (sortKey == SORT_NAME) {
        qsort(userset->users, userset->count, sizeof(User), compareUserByUsername);
    }
}

void cetakPasien(User u) {
    printf("%-3d | %-8s | %s\n", u.id, u.username, u.riwayat_penyakit);
}

void cetakDokter(User u) {
    printf("%-3d | %-8s | %d\n", u.id, u.username, u.aura);
}

// void tungguKembali() {
//     char dummy;
//     printf("\n>>>>  TEKAN TOMBOL [M] UNTUK KEMBALI KE MAIN MENU  >>>>>\n");
//     printf("Input: ");

//     while ((dummy = getchar()) != '\n' && dummy != EOF) {} // flush newline sisa dari scanf sebelumnya

//     dummy = getchar(); 
// }


// ===========================
// USER
// ===========================
void Cari_User(UserSet* userSet) {
    printf(">>> 🔍 Cari User\n\nCari berdasarkan?\n1. ID\n2. Nama\n>>> Pilihan: ");
    int pilihan;
    scanf("%d", &pilihan);

    if (pilihan == SORT_ID) {
        int id;
        printf(">>> Masukkan nomor ID user: ");
        scanf("%d", &id);
        Mencari_User_bedasarkan_ID(userSet, id);
    } else if (pilihan == SORT_NAME) {
        char name[MAX_LINE];
        printf(">>> Masukkan Nama user: ");
        scanf("%s", name);
        Mencari_User_bedasarkan_UserName(userSet, name);
    } else {
        printf("Input Tidak Valid!\n");
    }

    //tungguKembali();
}

void Mencari_User_bedasarkan_UserName(UserSet* userSet, char* username_dicari) {
    for (int i = 0; i < userSet->count; i++) {
        User u = userSet->users[i];
        if (strcmp(u.username, username_dicari) == 0) {
            printf("ID  | Nama     | Role    | Penyakit\n");
            printf("-------------------------------------\n");
            printf("%-3d | %-8s | %-7s | %s\n", u.id, u.username, roleToString(u.role),
                   (u.role == ROLE_PASIEN ? u.riwayat_penyakit : "-"));
            return;
        }
    }
    printf("User dengan Username '%s' tidak ditemukan.\n", username_dicari);
}

void Mencari_User_bedasarkan_ID(UserSet* userSet, int id_dicari) {
    for (int i = 0; i < userSet->count; i++) {
        User u = userSet->users[i];
        if (u.id == id_dicari) {
            printf("ID  | Nama     | Role    | Penyakit\n");
            printf("-------------------------------------\n");
            printf("%-3d | %-8s | %-7s | %s\n", u.id, u.username, roleToString(u.role),
                   (u.role == ROLE_PASIEN ? u.riwayat_penyakit : "-"));
            return;
        }
    }
    printf("User dengan ID %d tidak ditemukan.\n", id_dicari);
}

// ===========================
// PASIEN
// ===========================
void CariPasien(UserSet* userset) {
    printf(">>> Cari Pasien\n\nCari berdasarkan?\n1. ID\n2. Nama\n3. Penyakit\n>>> Pilihan: ");
    int pilihan;
    scanf("%d", &pilihan);

    if (pilihan == SORT_ID) {
        int id;
        printf(">>> Masukkan nomor ID pasien: ");
        scanf("%d", &id);
        Cari_Pasien_Bedasarkan_ID(userset, id);
    } else if (pilihan == SORT_NAME) {
        char name[MAX_LINE];
        printf(">>> Masukkan Nama pasien: ");
        scanf("%s", name);
        Cari_Pasien_Bedasarkan_Nama(userset, name);
    } else if (pilihan == SORT_PENYAKIT_AURA) {
        char penyakit[MAX_LINE];
        printf(">>> Masukkan Penyakit pasien: ");
        scanf(" %[^\n]", penyakit);
        penyakit[strcspn(penyakit, "\r\n")] = '\0';
        Cari_Pasien_Bedasarkan_Penyakit(userset, penyakit);
    } else {
        printf("Input Tidak Valid!\n");
    }

    //tungguKembali();
}

void Cari_Pasien_Bedasarkan_ID(UserSet* userset, int id) {
    printf("Urutkan berdasarkan?\n1. ID\n2. Nama\n>>> ");
    int pilihan;
    scanf("%d", &pilihan);
    sortUsers(userset, pilihan);

    printf("ID  | Nama     | Penyakit\n");
    printf("-------------------------------\n");

    int found = 0;
    for (int i = 0; i < userset->count; i++) {
        User u = userset->users[i];
        if (u.role == ROLE_PASIEN && u.id == id) {
            cetakPasien(u);
            found = 1;
        }
    }
    if (!found) printf("Pasien dengan ID %d tidak ditemukan.\n", id);
}

void Cari_Pasien_Bedasarkan_Nama(UserSet* userset, char* nama) {
    printf("Urutkan berdasarkan?\n1. ID\n2. Nama\n>>> ");
    int pilihan;
    scanf("%d", &pilihan);
    sortUsers(userset, pilihan);

    printf("ID  | Nama     | Penyakit\n");
    printf("-------------------------------\n");

    int found = 0;
    for (int i = 0; i < userset->count; i++) {
        User u = userset->users[i];
        if (u.role == ROLE_PASIEN && strcmp(u.username, nama) == 0) {
            cetakPasien(u);
            found = 1;
        }
    }
    if (!found) printf("Pasien dengan nama '%s' tidak ditemukan.\n", nama);
}

void Cari_Pasien_Bedasarkan_Penyakit(UserSet* userset, char* penyakit_dicari) {
    printf("Urutkan berdasarkan?\n1. ID\n2. Nama\n>>> ");
    int pilihan;
    scanf("%d", &pilihan);
    sortUsers(userset, pilihan);

    printf("ID  | Nama     | Penyakit\n");
    printf("-------------------------------\n");

    int found = 0;
    for (int i = 0; i < userset->count; i++) {
        User u = userset->users[i];
        if (u.role == ROLE_PASIEN &&
            strcmp(u.riwayat_penyakit, penyakit_dicari) == 0) {
            cetakPasien(u);
            found = 1;
        }
    }
    if (!found) {
        printf("Tidak ditemukan pasien dengan penyakit \"%s\"\n", penyakit_dicari);
    }
}

// ===========================
// DOKTER
// ===========================
void Cari_Dokter(UserSet* userset) {
    printf(">>> Cari Dokter\n\nCari berdasarkan?\n1. ID\n2. Nama\n3. Aura\n>>> Pilihan: ");
    int pilihan;
    scanf("%d", &pilihan);

    if (pilihan == SORT_ID) {
        int id;
        printf(">>> Masukkan nomor ID dokter: ");
        scanf("%d", &id);
        Cari_Dokter_Bedasarkan_ID(userset, id);
    } else if (pilihan == SORT_NAME) {
        char name[MAX_LINE];
        printf(">>> Masukkan Nama dokter: ");
        scanf("%s", name);
        Cari_Dokter_Bedasarkan_Nama(userset, name);
    } else if (pilihan == SORT_PENYAKIT_AURA) {
        int aura;
        printf(">>> Masukkan Jumlah Aura dokter: ");
        scanf("%d", &aura);
        Cari_Dokter_Bedasarkan_Aura(userset, aura);
    } else {
        printf("Input Tidak Valid!\n");
    }

    //tungguKembali();
}

void Cari_Dokter_Bedasarkan_ID(UserSet* userset, int id) {
    printf("Urutkan berdasarkan?\n1. ID\n2. Nama\n>>> ");
    int pilihan;
    scanf("%d", &pilihan);
    sortUsers(userset, pilihan);

    printf("ID  | Nama     | Aura\n");
    printf("-------------------------------\n");

    int found = 0;
    for (int i = 0; i < userset->count; i++) {
        User u = userset->users[i];
        if (u.role == ROLE_DOKTER && u.id == id) {
            cetakDokter(u);
            found = 1;
        }
    }
    if (!found) printf("Dokter dengan ID %d tidak ditemukan.\n", id);
}

void Cari_Dokter_Bedasarkan_Nama(UserSet* userset, char* nama) {
    printf("Urutkan berdasarkan?\n1. ID\n2. Nama\n>>> ");
    int pilihan;
    scanf("%d", &pilihan);
    sortUsers(userset, pilihan);

    printf("ID  | Nama     | Aura\n");
    printf("-------------------------------\n");

    int found = 0;
    for (int i = 0; i < userset->count; i++) {
        User u = userset->users[i];
        if (u.role == ROLE_DOKTER && strcmp(u.username, nama) == 0) {
            cetakDokter(u);
            found = 1;
        }
    }
    if (!found) printf("Dokter dengan nama '%s' tidak ditemukan.\n", nama);
}

void Cari_Dokter_Bedasarkan_Aura(UserSet* userset, int aura) {
    printf("Urutkan berdasarkan?\n1. ID\n2. Nama\n>>> ");
    int pilihan;
    scanf("%d", &pilihan);
    sortUsers(userset, pilihan);

    printf("ID  | Nama     | Aura\n");
    printf("-------------------------------\n");

    int found = 0;
    for (int i = 0; i < userset->count; i++) {
        User u = userset->users[i];
        if (u.role == ROLE_DOKTER && u.aura == aura) {
            cetakDokter(u);
            found = 1;
        }
    }
    if (!found) printf("Tidak ditemukan dokter dengan aura %d.\n", aura);
}
