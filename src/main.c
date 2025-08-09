#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HeaderSemuaADT.h"
#include "BackToMenu.h"
#include "F01.h"
#include "F02.h"
#include "F03.h"
#include "F04.h"
#include "F05.h"
#include "F06.h"
#include "F07.h"
#include "F08.h"
#include "F09.h"
#include "F10.h"
#include "F11.h"
#include "F12.h"
#include "F13.h"
#include "F14.h"
#include "F15.h"
#include "F16.h"
#include "F17.h"
#include "F18.h"
#include "B02.h"

void printAsciiArt() {
    printf("██████╗ ██╗   ██╗███╗   ███╗ █████╗ ██╗  ██╗    ███████╗ █████╗ ██╗  ██╗██╗████████╗\n");
    printf("██╔══██╗██║   ██║████╗ ████║██╔══██╗██║  ██║    ██╔════╝██╔══██╗██║ ██╔╝██║╚══██╔══╝\n");
    printf("██████╔╝██║   ██║██╔████╔██║███████║███████║    ███████╗███████║█████╔╝ ██║   ██║   \n");
    printf("██╔══██╗██║   ██║██║╚██╔╝██║██╔══██║██╔══██║    ╚════██║██╔══██║██╔═██╗ ██║   ██║   \n");
    printf("██║  ██║╚██████╔╝██║ ╚═╝ ██║██║  ██║██║  ██║    ███████║██║  ██║██║  ██╗██║   ██║   \n");
    printf("╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝    ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝   ╚═╝   \n");
    printf("                                                                                  \n");
    printf("                ███╗   ██╗██╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗               \n");
    printf("                ████╗  ██║██║████╗ ████║██╔═══██╗████╗  ██║██╔════╝               \n");
    printf("                ██╔██╗ ██║██║██╔████╔██║██║   ██║██╔██╗ ██║███████╗               \n");
    printf("                ██║╚██╗██║██║██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║               \n");
    printf("                ██║ ╚████║██║██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║               \n");
    printf("                ╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝               \n");
}

int main() {
    printAsciiArt();

    // Inisialisasi
    Data_sistem datasistem = {0};
    UserSet userSet = {0};
    Daftar_Penyakit daftar_penyakit = {0};
    Sistem_pengobatan sistemObat = {0};

    loadUsers(&userSet, USER_CSV_PATH);
    inisialisasi_sistem_pengobatan(&sistemObat);
    loadPenyakit(&daftar_penyakit);
    baca_obat(&sistemObat);
    Load_Obat_Penyakit_CSV(&sistemObat, &daftar_penyakit);
    load_ruangan_config(&datasistem, &userSet);
    inisialisasiMap(&datasistem);

    int choice;
    while (1) {
        User u;
        if (userSet.loggedInIndex != -1) {
            u = userSet.users[userSet.loggedInIndex];
        }

        // Tampilkan Menu Utama
        printf("\n========================================🏥 MENU UTAMA 🏥========================================\n");
        if (userSet.loggedInIndex == -1) {
            printf("1.🔐 Login\n2.📝 Register Pasien\n3.❓ Lupa Password\n4.📘 Help\n0.🏃‍♂️ Keluar\n");
        } else {
            printf("1.🔓 Logout\n2.📘 Help\n3.🗺️  Lihat Denah\n");
            if (u.role == ROLE_MANAGER) {
                printf("4.👥 Lihat User\n5.🔍 Cari User\n6.🧑‍⚕️🔎 Cari Pasien\n7.🩺🔎 Cari Dokter\n");
                printf("8.📋 Lihat Antrian\n9.➕🧑‍⚕️ Tambah Dokter\n10.🏷️ Assign Dokter\n11.🗺️ Ubah Denah\n12.🔄 Pindah Dokter\n");
            } else if (u.role == ROLE_DOKTER) {
                printf("4.🧾 Diagnosis\n5.💊 Ngobatin\n");
            } else if (u.role == ROLE_PASIEN) {
                printf("4.🥹 Boleh Pulang gak Dok?\n5.📅 Daftar Checkup\n6.⏳ Antrian Saya\n7.💊 Minum Obat\n8.🧪 Minum Penawar\n");
            }
            printf("0.🏃‍♂️ Keluar\n");
        }

        printf("Pilihan: ");
        scanf("%d", &choice);

        if (userSet.loggedInIndex == -1) {
            switch (choice) {
                case 1: login(&userSet); break;
                case 2: registerPasien(&userSet); break;
                case 3: {
                    User temp;
                    lupa_password_input(&temp);
                    for (int i = 0; i < userSet.count; i++) {
                        if (strcmp(userSet.users[i].username, temp.username) == 0) {
                            strcpy(userSet.users[i].password, temp.password);
                            break;
                        }
                    }
                    break;
                }
                case 4: Help(&userSet); break;
                case 0: saveAllUsersToCSV(&userSet, USER_CSV_PATH);
                        saveConfig(&datasistem, CONFIG_PATH, &userSet);
                        EXIT(&userSet, &datasistem); return 0;
                default: printf("Pilihan tidak valid!\n");
            }
        } else {
            if (choice == 0) {
                saveAllUsersToCSV(&userSet, USER_CSV_PATH);
                saveConfig(&datasistem, CONFIG_PATH, &userSet);
                EXIT(&userSet, &datasistem); return 0;
            }

            switch (choice) {
                case 1: logout(&userSet); break;
                case 2: Help(&userSet); break;
                case 3: {
                    load_ruangan_config(&datasistem, &userSet);
                    lihat_denah(&datasistem);
                    Lihat_Detail_Denah(&datasistem);
                    break;
                }
                default:
                    switch (u.role) {
                        case ROLE_MANAGER:
                            switch (choice) {
                                case 4: lihatUser(&userSet); break;
                                case 5: Cari_User(&userSet); break;
                                case 6: CariPasien(&userSet); break;
                                case 7: Cari_Dokter(&userSet); break;
                                case 8: load_ruangan_config(&datasistem, &userSet);
                                        lihat_semua_antrian(datasistem.daftar_ruangan, &datasistem); break;
                                case 9: tambahDokter(&userSet);
                                        saveAllUsersToCSV(&userSet, USER_CSV_PATH); break;
                                case 10: load_ruangan_config(&datasistem, &userSet);
                                         assignDokter(&userSet, &datasistem);
                                         saveAllUsersToCSV(&userSet, USER_CSV_PATH); break;
                                case 11: Ubah_Denah(&datasistem, &userSet); break;
                                case 12: Pindah_Dokter(&userSet, &datasistem); break;
                                default: printf("Pilihan tidak valid untuk Manager!\n");
                            } break;

                        case ROLE_DOKTER:
                            switch (choice) {
                                case 4: load_ruangan_config(&datasistem, &userSet);
                                        loadPenyakit(&daftar_penyakit);
                                        Diagnosis(daftar_penyakit, &userSet, &datasistem); break;
                                case 5: load_ruangan_config(&datasistem, &userSet);
                                        baca_obat(&sistemObat);
                                        Load_Obat_Penyakit_CSV(&sistemObat, &daftar_penyakit);
                                        Ngobatin(&userSet, &datasistem, &sistemObat); break;
                                default: printf("Pilihan tidak valid untuk Dokter!\n");
                            } break;

                        case ROLE_PASIEN:
                            switch (choice) {
                                case 4: boleh_pulang_gak_dok(&userSet, &sistemObat, &datasistem); break;
                                case 5: load_ruangan_config(&datasistem, &userSet);
                                        antrianDaftarCheckUp(&userSet, &datasistem); break;
                                case 6: load_ruangan_config(&datasistem, &userSet);
                                        status_antrian(u, datasistem.daftar_ruangan); break;
                                case 7: Minum_Obat(&userSet, &sistemObat); break;
                                case 8: Minum_Penawar(&userSet); break;
                                default: printf("Pilihan tidak valid untuk Pasien!\n");
                            } break;
                        default: printf("Role tidak dikenali!\n");
                    }
            }
        }

        back();
    }

    free(userSet.users);
    return 0;
}
