#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "F15.h"

void status_antrian(User current_user, Data_ruangan* data_ruangan){
    if (current_user.role!=ROLE_PASIEN){
        printf("Akses ditolak. Hanya Pasien yang dapat menggunakan perintah ANTRIAN!!\n");
        return;
    }
    bool found = false;

    for (int i = 0; i<BANYAK_RUANGAN; i++){
        Antrian* antre_luar = data_ruangan[i].antrian_pasien;
        NodeUser* current_luar = antre_luar->depan;
        Antrian* antre_dalam = data_ruangan[i].antrian_pasien_dalam_ruangan;
        NodeUser* current_dalam = antre_dalam->depan;
        int posisi_dalam = 1;
        int posisi_luar = 1;
        
        while (current_luar!=NULL){
            if (strcmp(current_luar->data->username, current_user.username) == 0){
                found = true;

                printf("\n Status Antrian Anda:\n");
                if (data_ruangan[i].dokter_di_ruangan->id != -1 && data_ruangan[i].dokter_di_ruangan->id != -0){
                    printf("Dokter           : %s\n",data_ruangan[i].dokter_di_ruangan->username);
                } else {
                    printf("Dokter           : -\n");
                }

                printf("Ruangan          : %s\n", data_ruangan[i].NamaRuangan);
                printf("Posisi antrian   : %d dari %d\n", posisi_luar, antre_luar->panjang);
                return;
            }
            current_luar = current_luar->next;
            posisi_luar++;
        }
        while (current_dalam!=NULL)
        {
            if (strcmp(current_dalam->data->username, current_user.username) == 0){
                found = true;
                printf("Anda sedang berada di dalam ruangan dokter!");

                // printf("\n Status Antrian Anda:\n");
                // if (data_ruangan[i].dokter_di_ruangan->id != -1 && data_ruangan[i].dokter_di_ruangan->id != -0){
                //     printf("Dokter           : %s\n",data_ruangan[i].dokter_di_ruangan->username);
                // } else {
                //     printf("Dokter           : -\n");
                // }

                // printf("Ruangan          : %s\n", data_ruangan[i].NamaRuangan);
                // printf("Posisi antrian   : %d dari %d\n", posisi_dalam, antre_dalam->panjang);
                return;
            }
            current_dalam = current_dalam->next;
            posisi_dalam++;
        }
        
    }

    if (!found){
        printf("\nAnda belum terdaftar dalam antrian check-up!\n");
        printf("Silakan daftar terlebih dahulu dengan command DAFTAR_CHECKUP.\n");
    }
}



